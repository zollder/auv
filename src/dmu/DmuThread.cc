/*
 *	DmuThread.cpp
 *  Created on: 18.01.2015
 *	Author: zollder
 */

#include "DmuThread.h"

//---------------------------------------------------------------------------------------------
// DmuThread controller thread implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	DmuThread::DmuThread(SensorData* sensorData_p)
	{
		printf("Constructing DmuThread thread...\n");

		setThreadId(T5_ID);
		timer = new FdTimer(getThreadId(), DMU_CONVERT_INTERVAL);
		dmu = new DMU();

		sensorData = sensorData_p;
	}

	//-----------------------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------------------
	DmuThread::~DmuThread()
	{
		printf("Destroying DmuThread thread ...\n");
		delete dmu;
		delete timer;
	}

	//-----------------------------------------------------------------------------------------
	// Overrides BaseThread's run() method
	//-----------------------------------------------------------------------------------------
	void* DmuThread::run()
	{
		dmu->enable();

		timer->start();

		while(1)
		{
			timer->waitTimerEvent();

			// read raw DMU values
			dmu->readSensor();

			// calculate temperature
			dmu->calculateTemperature();

			// calculate temperature-compensated pressure
			dmu->calculatePressure();

			// calculate current depth
			dmu->calculateDepth();

			// save depth value to the shared data holder
			sensorData->mutex.lock();
				sensorData->depth = dmu->depth;
			sensorData->mutex.unlock();

			printf("DMU data:   %10.2f    %10.2f    %10d \n", dmu->temperature, dmu->pressure, dmu->depth);
		}

		return NULL;
	}

