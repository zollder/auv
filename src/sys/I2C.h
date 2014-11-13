/*
 * I2C.h
 *
 *  Created on: 23.08.2013
 *      Author: tuuzdu
 */

#ifndef I2C_H_
#define I2C_H_

#define BUFFER_SIZE 0x40	// 64 in decimal

class I2C {

private:
	int I2CBus;
	int I2CAddress;
//	char dataBuffer[BUFFER_SIZE];

public:
	char dataBuffer[BUFFER_SIZE];

	I2C(int bus, int address);

	char readI2CDeviceByte(char address);
	int writeI2CDeviceByte(char address, char value);
	int readI2CDeviceMultipleByte(char address, int quantity);
};

//extern I2C Wire;

#endif /* I2C_H_ */
