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

#include "devices/L3G.h"
#include "devices/LSM303.h"
#include "imu/IMUnit.h"

using namespace std;

#define MAX_BUS 64

int main(int argc, char *argv[])
{
//	L3G gyro;
//	LSM303 compass;
//
//	gyro.enable();
//	compass.enable();
//
//	gyro.readGyroscopeData();
//	compass.readAccelerometerData();
//	compass.readMagnetometerData();

	IMUnit* imu = new IMUnit();
	imu->initialize();
	imu->setCalibrationMode(false);
//	imu->findMagnetometerHeading();
	int counter = 300;
	while(counter > 0)
	{
		imu->readGyroscope();
		imu->readAccelerometer();
		imu->readMagnetometer();

		imu->findMagnetometerHeading();
		imu->updateMatrix();
		imu->normalize();
		imu->correctDrift();
		imu->calculateEulerAngles();

		printf("h:%.2f, r:%.2f, p:%.2f, y%.2f\n", imu->magHeading, imu->roll, imu->pitch, imu->yaw);
		counter--;
		usleep(200000);
	}

	delete imu;
}
