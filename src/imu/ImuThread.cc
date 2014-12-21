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

		setThreadId(4);
		timer = new FdTimer(getThreadId(), IMU_INTERVAL);
		imu = new IMU(IMU_INTERVAL);
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

		imu->initialize();

		int counter = 0;
		while(1)
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
//			imu->printData(1);

			counter++;
		}

		return NULL;
	}
