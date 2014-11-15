#include "L3G.h"
#include "../sys/I2C.h"
#include <stdio.h>

#define GYRO_ADDRESS 0xD6	// read/write bit is set by ioCtr in I2C.cpp
#define I2C_BUS	1

	// Construct I2C class
	I2C gyroscopeWire = I2C(I2C_BUS, GYRO_ADDRESS);

	// Turns on the L3G's gyro and places it in normal mode.
	void L3G::enable(void)
	{
		/*
		 * CTRL1 register (gyro)
		 * 0-1 (DR1 & DR0): 01 - data rate (ODR) = 25Hz
		 * 2-3 (BW1 & BW0): 00 - data rate (ODR) = 25Hz
		 * 4 (PD): 1 - normal power mode
		 * 5-7 (Zen, Xen, Yen): 111 - enable ZXY axis
		 * result: 01001111 => 0x4F
		 */
		writeRegister(L3G_CTRL_REG1, 0x4F);

		/*
		 * CTRL2 register (gyro) - default selections:
		 * 0: 0 - disabled external edge-sensitive trigger
		 * 1: 0 - disabled level-sensitive trigger
		 * 2-3: 00 - normal HP filter mode
		 * 4-7: 0000 - HP cut-off frequency = 2Hz (for ODR = 25Hz)
		 * result: 00000000 => 0x00
		 */
		writeRegister(L3G_CTRL_REG2, 0x00);

		/*
		 * CTRL4 register (gyro)
		 * 0 (BDU): 0 - block data update, normal mode
		 * 1 (BLE): 0 data LSB @ lower address
		 * 2-3 (FS1-FS0): 11 - full-scale selection, 2000 dps
		 * 4 (IMPen): 0 - level-sensitive latched disabled
		 * 5-6 (ST2-ST1): 00 - self-test disabled
		 * 7 (SIM): 0 - SPI serial interface mode (4-wires)
		 * result: 00110000 => 0x30
		 */
		writeRegister(L3G_CTRL_REG4, 0x30);
	}

	// Writes a gyro register
	void L3G::writeRegister(char reg, char value)
	{
		gyroscopeWire.writeI2CDeviceByte(reg, value);
	}

	// Reads a gyro register
	char L3G::readRegister(char reg)
	{
		return gyroscopeWire.readI2CDeviceByte(reg);
	}

	// Reads the 3 gyro channels and stores them in vector g
	void L3G::readGyroscopeData()
	{
		// assert the MSB of the address to get the gyro
		// to do slave-transmit subaddress updating.
		gyroscopeWire.readI2CDeviceMultipleByte(L3G_OUT_X_L | (1 << 7), 6);

		char xlg = gyroscopeWire.dataBuffer[0];
		char xhg = gyroscopeWire.dataBuffer[1];
		char ylg = gyroscopeWire.dataBuffer[2];
		char yhg = gyroscopeWire.dataBuffer[3];
		char zlg = gyroscopeWire.dataBuffer[4];
		char zhg = gyroscopeWire.dataBuffer[5];

		// combine high and low bytes
		gyroData.x = (float) convertMsbLsb(xhg, xlg);
		gyroData.y = (float) convertMsbLsb(yhg, ylg);
		gyroData.z = (float) convertMsbLsb(zhg, zlg);

		printf("X: %.2f \tY: %.2f  \tZ: %.2f\n", gyroData.x, gyroData.y, gyroData.z);
	}

	short L3G::convertMsbLsb(char msb, char lsb)
	{
		short temp = (msb << 8 | lsb);
		return temp;
	}
