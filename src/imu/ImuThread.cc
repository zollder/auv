/*
 *	ImuThread.cpp
 *  Created on: 13.12.2014
 *	Author: zollder
 */

#include "ImuThread.h"

//---------------------------------------------------------------------------------------------
// ImuThread subclass implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	ImuThread::ImuThread(SensorData* sensorData_p)
	{
		printf("Constructing ImuThread ...\n");

		setThreadId(1);
		timer = new FdTimer(getThreadId(), INTERVAL);
		imu = new IMU(INTERVAL);
		sensorData = sensorData_p;
	}

	//-----------------------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------------------
	ImuThread::~ImuThread()
	{
		printf("Destroying ImuThread ...\n");
		delete imu;
		delete timer;
	}

	//-----------------------------------------------------------------------------------------
	// Overrides BaseThread's run() method
	//-----------------------------------------------------------------------------------------
	void* ImuThread::run()
	{
		timer->start();
		printf("Timer %d started.\n", timer->getTimerId());

		imu->initialize();

		int counter = 0;
		while(counter < 1000)
		{
			timer->waitTimerEvent();

			imu->readSensors();
			imu->calculateAngles();

			sensorData->mutex.lock();
				sensorData->roll = imu->imuDataDegrees.x;
				sensorData->pitch = imu->imuDataDegrees.y;
				sensorData->yaw = imu->imuDataDegrees.z;
			sensorData->mutex.unlock();

			// TODO: remove, used for testing and debugging purposes only
			imu->printData(1);

			counter++;
		}

		return NULL;
	}
