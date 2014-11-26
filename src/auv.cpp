//============================================================================
// Name        : auv.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include "devices/L3G.h"
#include "devices/LSM303.h"
#include "sys/Timer.h"
#include "imu/IMU.h"

using namespace std;

unsigned int counter=0;


int main()
{
	IMU imu;
	imu.initialize();
//	calibrateMagnetometer();

	unsigned long timer1 = millis();
	printf("timer1: %lu   ", timer1);
	usleep(100);
	printf("diff: %lu   \n", millis() - timer1);

	unsigned long timer = millis();
	unsigned long oldTimer;
	unsigned int printCounter = 0;
	while (1)
	{
		if((millis()-timer) >= 20)  // Main loop runs at 50Hz
		{
			counter++;
			oldTimer = timer;
			timer = millis();
			if (timer > oldTimer)
				imu.G_Dt = (timer-oldTimer)/1000.0;
			else
				imu.G_Dt = 0;
		}

		imu.readGyroscope();
		imu.readAccelerometer();

		if (counter > 5)
		{
			counter = 0;
			imu.readMagnetometer();
			imu.calculateHeading();
		}

		imu.updateMatrix();
		imu.normalize();
		imu.correctDrift();
		imu.calculateEulerAngles();

		printCounter++;
		if (printCounter > 5)
		{
			printCounter = 0;
			imu.printData(1);
		}
	}
}
