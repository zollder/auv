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
	int I2C::readI2CDeviceByte(char regAddress) {

		char filename[BUS_SIZE];
		int fileDescriptor = this->openI2CBus(filename, sizeof(filename));
		this->initiateCommunication(fileDescriptor);

		// reset register address
		char writeBuffer[1];
		writeBuffer[0] = regAddress;

		ssize_t writeResult = write(fileDescriptor, writeBuffer, 1);
		if (writeResult != 1)
			printf("Failed to reset address.\n");

		int bytesRead = read(fileDescriptor, this->dataBuffer, 1);
		if (bytesRead)
			printf("Failed to read a byte.\n");

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

	//-------------------------------------------------------------------------------------------


	uint8_t I2C::readByte(uint8_t address) {

		int file = openConnection();

		uint8_t buffer[1];
		buffer[0] = address;

		if (write(file, buffer, 1) != 1) {
			msg_error("Can not write data. Address %d.", I2CAddress);
		}

		uint8_t value[1];

		if (read(file, value, 1) != 1) {
			msg_error("Can not read data. Address %d.", I2CAddress);
		}

		close(file);

		return value[0];
	}


	int I2C::openConnection() {
		int file;

		if ((file = open("/dev/i2c-2", O_RDWR)) < 0) {
			msg_error("%s do not open. Address %d.", "/dev/i2c-2", I2CAddress);
			exit(1);
		}

		if (ioctl(file, I2C_SLAVE, I2CAddress) < 0) {
			msg_error("Can not join I2C Bus. Address %d.", I2CAddress);
			exit(1);
		}

		return file;
	}

	int I2C::ms5803_init() {
	    int r, i;
	    uint8_t dynamicAddress[1] = {(uint8_t) 0xA0};

	    if ((i2c_fd = open(MS5803_DEV, O_RDWR)) < 0)
	        return -1;

	    /* set the port options and set the address of the device */
	    if (ioctl(i2c_fd, I2C_SLAVE, MS5803_I2C_ADDRESS) < 0)
	        return -1;

	    /* reset the sensor */
	    r = write(i2c_fd, (const unsigned char[]) {0x1e}, 1);
	    if (r != 1)
	        return -1;
	    usleep(3000);

	    /* read the calibration coefficients and store them in `calib_coeff`
	       array */
	    for (i = 0; i < 8; i++) {
	    	dynamicAddress[0] = (uint8_t) 0xa0 + i * 2;
	        r = write(i2c_fd,  dynamicAddress, 1);
	        if (r != 1)
	            return -1;

	        r = read(i2c_fd, &calib_coeff[i], 2);
	        calib_coeff[i] = BSWAP16(calib_coeff[i]);
	        printf("MS5803 DEBUG: calibration coefficient %d: %d\n", i, calib_coeff[i]);
	        if (r != 2)
	            return -1;
	    }

	    return 0;
	}
