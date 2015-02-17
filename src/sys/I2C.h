/*
 *	I2C.h
 *  Created on: 10.11.2014
 *	Author: zollder
 */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <iostream>
#include <stdint.h>

#ifndef I2C_H_
#define I2C_H_

#define BUS_SIZE 64

#define BSWAP16(v) ((v << 8) & 0xFF00 | (v >> 8) & 0xFF)

class I2C
{
	public:

		char dataBuffer[BUS_SIZE];
		uint16_t wordBuffer[1];

		/** Constructor. */
		I2C(int bus, int address);
		~I2C();

		/** Writes a register value to specified register address. */
		int writeI2CDeviceByte(char address, char value);

		/** Writes a value to a device address. */
		int writeI2CDeviceByte(char value);

		/** Reads one word from specified register address. */
		int readI2CDeviceWord(char address);

		/** Reads specified number of bytes starting from specified register address. */
		int readI2CDeviceMultipleByte(char address, int quantity);

	private:

		int I2CBus;
		int I2CAddress;

		/** Opens I2C bus. Returns a file descriptor. */
		int openI2CBus(char* filename, size_t fileSize);

		/** Initiates communication for specified bus (requires a file descriptor). */
		void initiateCommunication(int fileDescriptor);
};


#endif /* I2C_H_ */
