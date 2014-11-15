/*
 *	LSM303.cpp
 *  Created on: 10.11.2014
 *	Author: zollder
 */

#include "LSM303.h"
#include "../sys/I2C.h"
#include <stdio.h>

// LSM303D uses same address for magnetometer and accelerometer
#define ACC_MAG_ADDRESS 0x1D		// 00011101b
#define I2C_BUS	1

	// TODO: calibrate magnetometer
	// TODO: implement software timer

	// Initialize I2Cs
	I2C accelerometerWire = I2C(I2C_BUS, ACC_MAG_ADDRESS);
	I2C magnetometerWire = I2C(I2C_BUS, ACC_MAG_ADDRESS);

	//-----------------------------------------------------------------------------------------
	/** Turns on and configures LSM303's accelerometer and magnetometer. */
	//-----------------------------------------------------------------------------------------
	void LSM303::enable(void) {
		/*
		 * CTRL1 register (accelerometer)
		 * 0-3: 0001 - 3.125Hz
		 * 4: 0 - continuous update
		 * 5: 1 - Z-axis enabled
		 * 6: 1 - Y-axis enabled
		 * 7: 1 - X-axis enabled
		 * result: 00010111 => 0x17
		 */
		writeAccelerometerRegister(CTRL_REG1_A, 0x17);

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
		 * 3-5: 000 - data rate 3.125Hz
		 * 6-7: 00 - interrupt requests
		 * result: 01100000 => 0x60
		 */
		writeMagnetometerRegister(CTRL_REG5_A, 0x60);

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
	/** Writes an accelerometer register. */
	//-----------------------------------------------------------------------------------------
	void LSM303::writeAccelerometerRegister(char reg, char value)
	{
		accelerometerWire.writeI2CDeviceByte(reg, value);
	}

	//-----------------------------------------------------------------------------------------
	/** Writes magnetometer register. */
	//-----------------------------------------------------------------------------------------
	void LSM303::writeMagnetometerRegister(char reg, char value)
	{
		magnetometerWire.writeI2CDeviceByte(reg, value);
	}

	// Reads an accelerometer register
	char LSM303::readAccelerometerRegister(char reg)
	{
		return accelerometerWire.readI2CDeviceByte(reg);
	}

	// Reads a magnetometer register
	char LSM303::readMagnetometerRegister(char reg)
	{
		return magnetometerWire.readI2CDeviceByte(reg);
	}

	//-----------------------------------------------------------------------------------------
	/** Reads accelerometer registers and stores the values in a RawData structure. */
	//-----------------------------------------------------------------------------------------
	void LSM303::readAccelerometerData(void)
	{
		// asserts the MSB of the address to get the accelerometer
		// to do slave-transmit sub-address updating.
		accelerometerWire.readI2CDeviceMultipleByte(OUT_X_L_A | (1 << 7), 6);

		char xla = accelerometerWire.dataBuffer[0];
		char xha = accelerometerWire.dataBuffer[1];
		char yla = accelerometerWire.dataBuffer[2];
		char yha = accelerometerWire.dataBuffer[3];
		char zla = accelerometerWire.dataBuffer[4];
		char zha = accelerometerWire.dataBuffer[5];

		// combine high and low bytes (16-bits resolution)
		accelData.x = (float) convertMsbLsb(xha, xla);
		accelData.y = (float) convertMsbLsb(yha, yla);
		accelData.z = (float) convertMsbLsb(zha, zla);

		printf("aroll: %.2f \tapitch: %.2f  \tayaw: %.2f\n", accelData.x, accelData.y, accelData.z);
	}

	//-----------------------------------------------------------------------------------------
	/** Reads magnetometer registers and stores the values in a RawData structure. */
	//-----------------------------------------------------------------------------------------
	void LSM303::readMagnetometerData(void)
	{
		// assert MSB of the address to get the magnetometer
		// to do slave-transmit sub-address updating (specific to LSM303D).
		magnetometerWire.readI2CDeviceMultipleByte(D_OUT_X_L_M | (1 << 7), 6);

		char xhm = magnetometerWire.dataBuffer[0];
		char xlm = magnetometerWire.dataBuffer[1];
		char zhm = magnetometerWire.dataBuffer[2];
		char zlm = magnetometerWire.dataBuffer[3];
		char yhm = magnetometerWire.dataBuffer[4];
		char ylm = magnetometerWire.dataBuffer[5];

		// combine high and low bytes
		magnetData.x = (float) convertMsbLsb(xhm, xlm);
		magnetData.y = (float) convertMsbLsb(yhm, ylm);
		magnetData.z = (float) convertMsbLsb(zhm, zlm);

		printf("mroll: %.2f \tmpitch: %.2f \tmyaw: %.2f\n", magnetData.x, magnetData.y, magnetData.z);
	}

	//-----------------------------------------------------------------------------------------
	/**
	 * Combines high & low bytes.
	 * There is no need to discard 4 lower bits as LSM303D has 16-bit resolution.
	 */
	//-----------------------------------------------------------------------------------------
	short LSM303::convertMsbLsb(char msb, char lsb)
	{
		short temp = (msb << 8 | lsb);
		return temp;
	}
