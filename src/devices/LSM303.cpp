/*
 *	LSM303.cpp
 *  Created on: 10.11.2014
 *	Author: zollder
 */

#include "LSM303.h"

	// TODO: calibrate magnetometer
	// TODO: implement software timer

	LSM303::LSM303()
	{
		printf("Constructing LSM303 ...\n");
		accelerometerWire = new I2C(I2C_BUS, ACC_MAG_ADDRESS);
		magnetometerWire = new I2C(I2C_BUS, ACC_MAG_ADDRESS);
	}

	LSM303::~LSM303()
	{
		printf("Destroying LSM303 ...\n");
		delete accelerometerWire;
		delete magnetometerWire;
	}

	//-----------------------------------------------------------------------------------------
	/** Turns on and configures LSM303's accelerometer and magnetometer. */
	//-----------------------------------------------------------------------------------------
	void LSM303::enable(void) {
		/*
		 * CTRL1 register (accelerometer)
		 * 0-3: 0101 - data rate 50Hz
		 * 4: 0 - continuous update
		 * 5: 1 - Z-axis enabled
		 * 6: 1 - Y-axis enabled
		 * 7: 1 - X-axis enabled
		 * result: 01010111 => 0x47
		 */
		writeAccelerometerRegister(CTRL_REG1_A, 0x57);

		/*
		 * CTRL2 register (accelerometer)
		 * 0-1: 00 - anti-alias filter (773Hz)
		 * 2-4: 011 - ±8g full-scale acceleration
		 * 5: 0 - reserved
		 * 6: 0 - self-test disabled
		 * 7: 0 - 4-wire serial interface
		 * result: 00011000 => 0x18
		 */
		writeAccelerometerRegister(CTRL_REG2_A, 0x18);

		/*
		 * CTRL5 register (magnetometer)
		 * 0: 0 - temperature sensor disabled
		 * 1-2: 11 - high resolution
		 * 3-5: 100 - data rate 50Hz
		 * 6-7: 00 - interrupt requests
		 * result: 01110000 => 0x70
		 */
		writeMagnetometerRegister(CTRL_REG5_A, 0x70);

		/*
		 * CTRL6 register (magnetometer)
		 * 0: 0 - reserved
		 * 1-2: 10 - ±8 gauss full-scale
		 * 3-7: 00000 - reserved
		 * result: 01000000 => 0x40
		 */
		writeMagnetometerRegister(CTRL_REG6_A, 0x40);

		/*
		 * CTRL7 register (magnetometer)
		 * 0-1: 00 - high-pass filtermode - normal mode (resets axis registers)
		 * 2: 0 - bypass acceleration data filter
		 * 3: 0 - temperature sensor only mode OFF
		 * 4: 0 - reserved
		 * 5: 0 - magnetic data low-power mode OFF
		 * 6-7: 00 - continuous-conversion mode ON
		 * result: 00000000 => 0x00
		 */
		writeMagnetometerRegister(CTRL_REG7_A, 0x00);
	}

	//-----------------------------------------------------------------------------------------
	/** Writes accelerometer register. */
	//-----------------------------------------------------------------------------------------
	void LSM303::writeAccelerometerRegister(char reg, char value)
	{
		accelerometerWire->writeI2CDeviceByte(reg, value);
	}

	//-----------------------------------------------------------------------------------------
	/** Writes magnetometer register. */
	//-----------------------------------------------------------------------------------------
	void LSM303::writeMagnetometerRegister(char reg, char value)
	{
		magnetometerWire->writeI2CDeviceByte(reg, value);
	}

	//-----------------------------------------------------------------------------------------
	/** Reads an accelerometer register. */
	//-----------------------------------------------------------------------------------------
	char LSM303::readAccelerometerRegister(char reg)
	{
		return accelerometerWire->readI2CDeviceByte(reg);
	}

	//-----------------------------------------------------------------------------------------
	/** Reads a magnetometer register. */
	//-----------------------------------------------------------------------------------------
	char LSM303::readMagnetometerRegister(char reg)
	{
		return magnetometerWire->readI2CDeviceByte(reg);
	}

	//-----------------------------------------------------------------------------------------
	/** Reads accelerometer registers and stores the values in a RawData structure. */
	//-----------------------------------------------------------------------------------------
	void LSM303::readAccelerometerData(void)
	{
		// asserts the MSB of the address to get the accelerometer
		// to do slave-transmit sub-address updating.
		// XYZ order, little endian.
		accelerometerWire->readI2CDeviceMultipleByte(OUT_X_L_A | (1 << 7), 6);

		char xla = accelerometerWire->dataBuffer[0];
		char xha = accelerometerWire->dataBuffer[1];
		char yla = accelerometerWire->dataBuffer[2];
		char yha = accelerometerWire->dataBuffer[3];
		char zla = accelerometerWire->dataBuffer[4];
		char zha = accelerometerWire->dataBuffer[5];

		// combine high and low bytes (16-bits resolution)
		accelRawData.x = (int) convertMsbLsb(xha, xla);
		accelRawData.y = (int) convertMsbLsb(yha, yla);
		accelRawData.z = (int) convertMsbLsb(zha, zla);

//		printf("%7d %7d %7d", accelRawData.x, accelRawData.y, accelRawData.z);
	}

	//-----------------------------------------------------------------------------------------
	/** Reads magnetometer registers and stores the values in a RawData structure. */
	//-----------------------------------------------------------------------------------------
	void LSM303::readMagnetometerData(void)
	{
		// assert MSB of the address to get the magnetometer
		// to do slave-transmit sub-address updating (specific to LSM303D).
		// XYZ order, little endian.
		magnetometerWire->readI2CDeviceMultipleByte(D_OUT_X_L_M | (1 << 7), 6);

		char xlm = magnetometerWire->dataBuffer[0];
		char xhm = magnetometerWire->dataBuffer[1];
		char ylm = magnetometerWire->dataBuffer[2];
		char yhm = magnetometerWire->dataBuffer[3];
		char zlm = magnetometerWire->dataBuffer[4];
		char zhm = magnetometerWire->dataBuffer[5];

		// combine high and low bytes
		magnetRawData.x = (int) convertMsbLsb(xhm, xlm);
		magnetRawData.y = (int) convertMsbLsb(yhm, ylm);
		magnetRawData.z = (int) convertMsbLsb(zhm, zlm);

//		printf("%7d %7d %7d\n", magnetRawData.x, magnetRawData.y, magnetRawData.z);
	}

	//-----------------------------------------------------------------------------------------
	/** Combines high & low bytes. */
	//-----------------------------------------------------------------------------------------
	short LSM303::convertMsbLsb(char msb, char lsb)
	{
		short temp = (msb << 8 | lsb);
		return temp;
	}
