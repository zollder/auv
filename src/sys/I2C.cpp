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
		wordBuffer[0] = {0x00};
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

		ssize_t result = write(fileDescriptor, writeBuffer, 2);
		if (result != 2) {
			printf("Failed to write to I2C device.\n");
			exit(1);
		}

		close(fileDescriptor);
		return 0;
	}

	//-----------------------------------------------------------------------------------------
	/** Writes a value to a global (device) address. */
	//-----------------------------------------------------------------------------------------
	int I2C::writeI2CDeviceByte(char value)
	{
		char filename[BUS_SIZE];
		int fileDescriptor = this->openI2CBus(filename, sizeof(filename));
		this->initiateCommunication(fileDescriptor);

		// prepare data and write to IMU
		char writeBuffer[1];
		writeBuffer[0] = value;

		ssize_t result = write(fileDescriptor, writeBuffer, 1);
		if (result != 1) {
			printf("Failed to write to I2C device.\n");
			exit(1);
		}

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

		//int numberBytes = BUFFER_SIZE;
		int bytesRead = read(fileDescriptor, this->dataBuffer, quantity);
		if (bytesRead < 0)
			printf("Failed to read byte stream.\n");

		close(fileDescriptor);
		return 0;
	}

	//-----------------------------------------------------------------------------------------
	/** Reads one byte from specified register address. */
	//-----------------------------------------------------------------------------------------
	int I2C::readI2CDeviceWord(char regAddress) {

		char filename[BUS_SIZE];
		int fileDescriptor = this->openI2CBus(filename, sizeof(filename));
		this->initiateCommunication(fileDescriptor);

		// reset register address
		uint8_t writeBuffer[1];
		writeBuffer[0] = (uint8_t) regAddress;

		ssize_t writeResult = write(fileDescriptor, writeBuffer, 1);
		if (writeResult != 1)
			printf("Failed to reset address.\n");

		int bytesRead = read(fileDescriptor, &wordBuffer, 2);
		if (bytesRead != 2)
			printf("Failed to read word.\n");

		// swap bytes
		wordBuffer[0] = BSWAP16(wordBuffer[0]);

		close(fileDescriptor);
		return 0;
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
	}
