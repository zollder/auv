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
#include "imu/Compass.h"
#include "imu/DCM.h"
#include "imu/Matrix.h"

using namespace std;

// X axis pointing forward
// Y axis pointing to the right
// and Z axis pointing down.
// Positive pitch : nose up
// Positive roll : right wing down
// Positive yaw : clockwise
int SENSOR_SIGN[9] = {1,1,1,1,1,1,1,1,1};
float G_Dt=0.02;

int AN[6]; //array that stores the gyro and accelerometer data
int AN_OFFSET[6]={0,0,0,0,0,0}; //Array that stores the Offset of the sensors

int gyro_x;
int gyro_y;
int gyro_z;
int accel_x;
int accel_y;
int accel_z;
int magnetom_x;
int magnetom_y;
int magnetom_z;
float c_magnetom_x;
float c_magnetom_y;
float c_magnetom_z;
float MAG_Heading;

float Accel_Vector[3]= {0,0,0}; //Store the acceleration in a vector
float Gyro_Vector[3]= {0,0,0};//Store the gyros turn rate in a vector
float Omega_Vector[3]= {0,0,0}; //Corrected Gyro_Vector data
float Omega_P[3]= {0,0,0};//Omega Proportional correction
float Omega_I[3]= {0,0,0};//Omega Integrator
float Omega[3]= {0,0,0};

// Euler angles
float roll;
float pitch;
float yaw;

float errorRollPitch[3]= {0,0,0};
float errorYaw[3]= {0,0,0};

unsigned int counter=0;

float DCM_Matrix[3][3]= { {1,0,0},{0,1,0},{0,0,1} };
float Update_Matrix[3][3]={{0,1,2},{3,4,5},{6,7,8}}; //Gyros here
float Temporary_Matrix[3][3]={ {0,0,0},{0,0,0},{0,0,0} };

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

// vector structure
template <typename T> struct vector
{ T x, y, z; };

void setup_IMU()
{
	IMU_Init();
	usleep (20000);
	for(int i=0; i<32; i++)    // We take some readings...
    {
		Read_Gyro();
		Read_Accel();
		for(int y=0; y<6; y++)   // Cumulate values
			AN_OFFSET[y] += AN[y];
		usleep (20000);
    }

	for(int y=0; y<6; y++)
		AN_OFFSET[y] = AN_OFFSET[y]/32;

	AN_OFFSET[5]-=GRAVITY*SENSOR_SIGN[5];

	usleep (20000);
	counter=0;
}

void calibrateMagnetometer()
{
	vector<int> runningMin = {2047, 2047, 2047};
	vector<int> runningMax = {-2048, -2048, -2048};

	for(int i=0; i<500; i++)
	{
		Read_Compass();

		runningMin.x = min(runningMin.x, magnetom_x);
		runningMin.y = min(runningMin.y, magnetom_y);
		runningMin.z = min(runningMin.z, magnetom_z);

		runningMax.x = max(runningMax.x, magnetom_x);
		runningMax.y = max(runningMax.y, magnetom_y);
		runningMax.z = max(runningMax.z, magnetom_z);

		printf ("minX:%d   minY:%d   minZ:%d   maxX:%d   maxY:%d   maxZ:%d\n",
			runningMin.x, runningMin.y, runningMin.z, runningMax.x, runningMax.y, runningMax.z);

		usleep (100000);
	}
}

int main(int argc, char *argv[])
{
	setup_IMU();
//	calibrateMagnetometer();

	unsigned long timer1 = millis();
	printf("timer1: %lu   ", timer1);
	usleep(100);
	printf("diff: %lu   \n", millis() - timer1);

	unsigned long timer = millis();
	unsigned long oldTimer;
	while (1)
	{
		if((millis()-timer) >= 20)  // Main loop runs at 50Hz
		{
			counter++;
			oldTimer = timer;
			timer = millis();
			if (timer > oldTimer)
				G_Dt = (timer-oldTimer)/1000.0;
			else
				G_Dt = 0;
		}

		Read_Gyro();
		Read_Accel();

		if (counter > 5)
		{
			counter = 0;
			Read_Compass();
			Compass_Heading();
		}

		Matrix_update();
		Normalize();
		Drift_correction();
		Euler_angles();

		printf("Angle:   %.2f    %.2f    %.2f \n", ToDeg(roll), ToDeg(pitch), ToDeg(yaw));
	}




	//	IMU* imu = new IMU();
//	imu->initialize();
//
//	int counter = 0;
//	int timer  = mymillis();
//	long timer_old;
//
//	while(1)
//	{
//		if((mymillis()-timer)>=20)  // Main loop runs at 50Hz
//		{
//			counter++;
//			timer_old = timer;
//			timer = mymillis();
//			if (timer > timer_old)
//				imu->G_Dt = (timer-timer_old)/1000.0;    // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
//			else
//				imu->G_Dt = 0;
//
//			// *** DCM algorithm
//			// Data adquisition
//			imu->readGyroscope();
//			imu->readAccelerometer();
//
//			if (counter > 5)
//			{
//				counter=0;
//				imu->readMagnetometer();    // Read I2C magnetometer
//				imu->findMagnetometerHeading(); // Calculate magnetic heading
//			}
//
//			// Calculations...
//			imu->updateMatrix();
//			imu->normalize();
//			imu->correctDrift();
//			imu->calculateEulerAngles();
//
//			imu->printData(1);
//		}
//	}

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
//	delete imu;
}
