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

#ifndef I2C_H_
#define I2C_H_

#define BUS_SIZE 64

#define msg_error(M, ...) printf("[ERROR]:"M"\n",##__VA_ARGS__);
#define msg_warning(M, ...) printf("[WARNING]:"M"\n",##__VA_ARGS__);

#define BSWAP16(v) (v << 8) & 0xFF00 | (v >> 8) & 0xFF

static int i2c_fd;

/* MS5803 calibration data */
static uint16_t calib_coeff[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define MS5803_DEV "/dev/i2c-2"
#define MS5803_I2C_ADDRESS 0x76

class I2C
{
	public:

		char dataBuffer[BUS_SIZE];

		/** Constructor. */
		I2C(int bus, int address);
		~I2C();

		/** Writes a register value to specified register address. */
		int writeI2CDeviceByte(char address, char value);

		/** Writes a value to a device address. */
		int writeI2CDeviceByte(char value);

		/** Reads one byte from specified register address. */
		int readI2CDeviceByte(char address);

		/** Reads specified number of bytes starting from specified register address. */
		int readI2CDeviceMultipleByte(char address, int quantity);

		uint8_t readByte(uint8_t address);

		int ms5803_init();

	private:

		int I2CBus;
		int I2CAddress;

		/** Opens I2C bus. Returns a file descriptor. */
		int openI2CBus(char* filename, size_t fileSize);

		/** Initiates communication for specified bus (requires a file descriptor). */
		void initiateCommunication(int fileDescriptor);

		int openConnection();
};


#endif /* I2C_H_ */
