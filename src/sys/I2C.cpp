/*
 *	I2C.cpp
 *  Created on: 10.11.2014
 *	Author: zollder
 */
#include "I2C.h"

using namespace std;

	//-----------------------------------------------------------------------------------------
	/** Constructor. */
	//-----------------------------------------------------------------------------------------
	I2C::I2C(int bus, int address)
	{
		printf("Constructing I2C ...\n");
		I2CBus = bus;
		I2CAddress = address;
	}

	I2C::~I2C()
	{
		printf("Destroying I2C ...\n");
	}

	//-----------------------------------------------------------------------------------------
	/** Writes a register value to specified register address. */
	//-----------------------------------------------------------------------------------------
	int I2C::writeI2CDeviceByte(char regAddress, char regValue)
	{
		char filename[BUS_SIZE];
		int fileDescriptor = this->openI2CBus(filename, sizeof(filename));
		this->initiateCommunication(fileDescriptor);

		// prepare data and write to IMU
		char writeBuffer[2];
		writeBuffer[0] = regAddress;
		writeBuffer[1] = regValue;

		ssize_t writeResult = write(fileDescriptor, writeBuffer, 2);
		if (writeResult != 2) {
			printf("Failed to write to I2C device.\n");
			exit(1);
		}
//		else
//			printf("Completed writing to I2C device.\n");

		close(fileDescriptor);
		return 0;
	}

	//-----------------------------------------------------------------------------------------
	/** Reads specified number of bytes starting from specified register address. */
	//-----------------------------------------------------------------------------------------
	int I2C::readI2CDeviceMultipleByte(char regAddress, int quantity) {

		char filename[BUS_SIZE];
		int fileDescriptor = this->openI2CBus(filename, sizeof(filename));
		this->initiateCommunication(fileDescriptor);

		// reset register address
		char writeBuffer[1];
		writeBuffer[0] = regAddress;

		ssize_t writeResult = write(fileDescriptor, writeBuffer, 1);
		if (writeResult != 1)
			printf("Failed to reset address.\n");
//		else
//			printf("Address reset success.\n");

		//int numberBytes = BUFFER_SIZE;
		int bytesRead = read(fileDescriptor, this->dataBuffer, quantity);
		if (bytesRead < 0)
			printf("Failed to read byte stream.\n");
//		else
//			printf("Byte stream read success.\n");

		close(fileDescriptor);

//		printf("Number of bytes read: %d\n", bytesRead);
//		for (int i=0; i<quantity; i++)
//			printf("Byte %02d is 0x%02x\n", i, dataBuffer[i]);

		return 0;
	}

	//-----------------------------------------------------------------------------------------
	/**
	 * Reads one byte from specified register address.
	 * TODO: revise and re-implement.
	 */
	//-----------------------------------------------------------------------------------------
	char I2C::readI2CDeviceByte(char address) {

		cout << "Starting BMA180 I2C sensor state byte read" << endl;
		char namebuf[BUS_SIZE];
		snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", I2CBus);
		int file;
		if ((file = open(namebuf, O_RDWR)) < 0) {
			cout << "Failed to open Sensor on " << namebuf << " I2C Bus" << endl;
			return (1);
		}
		if (ioctl(file, I2C_SLAVE, I2CAddress) < 0) {
			cout << "I2C_SLAVE address " << I2CAddress << " failed..." << endl;
			return (2);
		}

		char buf[1];
		buf[0] = address;
		if (write(file, buf, 1) != 1) {
			cout << "Failed to Reset Address in readFullSensorState() " << endl;
		}

		unsigned char buffer[1];
		buffer[0] = address;
		if (read(file, buffer, 2) != 2) {
			cout << "Failure to read value from I2C Device address." << endl;
		}
		close(file);
		//cout << (int) buffer [0] << endl;
		return buffer[0];
	}

	//-----------------------------------------------------------------------------------------
	/** Opens I2C bus. Returns a file descriptor. */
	//-----------------------------------------------------------------------------------------
	int I2C::openI2CBus(char* filename, size_t fileSize)
	{
		// open I2C bus
		snprintf(filename, fileSize, "/dev/i2c-%d", I2CBus);
		int fileDescriptor = open(filename, O_RDWR);
		if (fileDescriptor < 0)
		{
			perror("Failed to open the i2c bus");
			exit(1);
		}
//		else
//			printf("I2C bus opened.\n");

		return fileDescriptor;
	}

	//-----------------------------------------------------------------------------------------
	/** Initiates communication for specified bus (requires a file descriptor). */
	//-----------------------------------------------------------------------------------------
	void I2C::initiateCommunication(int fileDescriptor)
	{
		// initiate communication
		int ioCtrResult = ioctl(fileDescriptor, I2C_SLAVE, I2CAddress);
		if (ioCtrResult < 0)
		{
			printf("Failed to acquire bus access and/or talk to slave.\n");
			exit(1);
		}
//		else
//			printf("Communication initiated.\n");
	}
