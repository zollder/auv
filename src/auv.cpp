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

//	gyro.enable();
	compass.enable();

//	gyro.readGyroscopeData();
	compass.readAccelerometerData();
	compass.readMagnetometerData();
}
