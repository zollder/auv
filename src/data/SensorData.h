/*
 *	SensorData.h
 *  Created on: 13.12.2014
 *	Author: zollder
 */

#include "Mutex.h"
#include "../commons/Dto.h"

#ifndef sensordata_h
#define sensordata_h

//-----------------------------------------------------------------------------------------
// SensorData interface.
//-----------------------------------------------------------------------------------------
class SensorData
{

	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		struct MeasuredData
		{
			int pitch = 0;
			int yaw = 0;
			int depth = 0;
		};

		// constructor
		SensorData() {}

		// destructor
		~SensorData() {}

		float roll = 2;
		float pitch = 3;
		float yaw = 4;

		float depth = 0;
		float temperature = 0;

		// read/write protection mechanism
		Mutex mutex;

		//-----------------------------------------------------------------------------------------
		/** Copies and returns sensor data in a Dto structure of integers. */
		//-----------------------------------------------------------------------------------------
		dto<int> getSensorData()
		{
			dto<int> data;
			mutex.lock();
				data.m1 = (int) pitch;
				data.m2 = (int) yaw;
				data.m3 = (int) depth;
			mutex.unlock();

			return data;
		}
};

#endif
