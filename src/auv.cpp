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

#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>


using namespace std;

#define MAX_BUS 64

int mymillis()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec) * 1000 + (tv.tv_usec)/1000;
}

int main(int argc, char *argv[])
{

	IMUnit* imu = new IMUnit();
	imu->initialize();

	int counter = 0;
	int timer  = mymillis();
	long timer_old;

	while(1)
	{
		if((mymillis()-timer)>=20)  // Main loop runs at 50Hz
		{
			counter++;
			timer_old = timer;
			timer = mymillis();
			if (timer > timer_old)
				imu->G_Dt = (timer-timer_old)/1000.0;    // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
			else
				imu->G_Dt = 0;

			// *** DCM algorithm
			// Data adquisition
			imu->readGyroscope();
			imu->readAccelerometer();

			if (counter > 5)
			{
				counter=0;
				imu->readMagnetometer();    // Read I2C magnetometer
				imu->findMagnetometerHeading(); // Calculate magnetic heading
			}

			// Calculations...
			imu->updateMatrix();
			imu->normalize();
			imu->correctDrift();
			imu->calculateEulerAngles();

			imu->printData(1);
		}
	}

//	while(1)
//	{
//	    if (timer > timer_old)
//	    	imu->G_Dt = (timer-timer_old)/1000.0;    // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
//	    else
//	    	imu->G_Dt = 0;
//		//read ACC and GYR data
//		imu->readGyroscope();
//		imu->readAccelerometer();
//		imu->readMagnetometer();
//
//		//Each loop should be at least 40ms.
//		while((mymillis()-timer) < 20)
//		{
//			usleep(100);
//		}
//
//		printf("Loop Time %d\t", mymillis()-timer);
//    }
//	L3G gyro;
//	LSM303 compass;
//
//	gyro.enable();
//	compass.enable();
//
//	gyro.readGyroscopeData();
//	compass.readAccelerometerData();
//	compass.readMagnetometerData();

//	imu->calibrateMagnetometer();

//	imu->readAccelerometer();
//	imu->readGyroscope();
//	imu->readMagnetometer();

//	for(int i=0; i<1000; i++)
//	{
//		imu->readAccelerometer();
//		imu->readGyroscope();
//		imu->readMagnetometer();
//		imu->findMagnetometerHeading();
//		imu->updateMatrix();
//		imu->normalize();
//		imu->correctDrift();
//		imu->calculateEulerAngles();
//
//		printf("h:%.2f, r:%.2f, p:%.2f, y%.2f\n", imu->magHeading, imu->roll, imu->pitch, imu->yaw);
//
//		usleep(100000);
//	}
//
//	imu->findMagnetometerHeading();
//	while(counter > 0)
//	{
//		counter--;
//		usleep(200000);
//	}
//
	delete imu;
}
