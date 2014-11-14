//============================================================================
// Name        : auv.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <iostream>
#include <string>
#include <unistd.h>

#include "IMU/L3G.h"
#include "IMU/LSM303.h"

using namespace std;

#define MAX_BUS 64

int main(int argc, char *argv[])
{
//	L3G gyro;
	LSM303 compass;

//	cout << "Enabling gyro ..." << endl;
//	gyro.enableDefault();

//	cout << "Enabling compass ..." << endl;
//	compass.enableDefault();

	// =========================================test:
    int file;
    char filename[40];
    const char *buffer;

	// open the bus
    sprintf(filename,"/dev/i2c-1");
	if ((file = open(filename, O_RDWR)) < 0) {
	    /* ERROR HANDLING: you can check errno to see what went wrong */
	    perror("Failed to open the i2c bus");
	    exit(1);
	}
	else
		cout << "Bus successfully opened." << endl;

	// initiate communication
	int deviceAddress = 0b00011101;
	if (ioctl(file, I2C_SLAVE, deviceAddress) < 0) {
	    printf("Failed to acquire bus access and/or talk to slave.\n");
	    exit(1);
	}
	else
		printf("Communication successfully initiated.\n");

	// read from IMU
	unsigned char readBuffer[1];
	readBuffer[0] = 0x20;
	ssize_t readResult = read(file, readBuffer, 2);
    if (readResult != 2)
    {
        printf("Failed to read from the i2c bus.\n");
        printf("\n\n");
    }
    else
        printf("Read successfully.\n");

	char writeBuffer[1];
	writeBuffer[0] = 0x25;
	writeBuffer[1] = 0x40;
    ssize_t writeResult = write(file, writeBuffer, 2);
	if (writeResult != 2) {
		printf("Failed to write to I2C device.\n");
		return (3);
	}
	else
		printf("Wrote successfully.\n");
}
