/*
 *	SensorData.h
 *  Created on: 13.12.2014
 *	Author: zollder
 */

#include "Mutex.h"

#ifndef sensdata_h
#define sensdata_h

//-----------------------------------------------------------------------------------------
// SensorData interface.
//-----------------------------------------------------------------------------------------
class SensorData
{

	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		SensorData() {}

		// destructor
		~SensorData() {}

		float roll = 0;
		float pitch = 0;
		float yaw = 0;

		int depth = 0;
		float temperature = 0;

		// read/write protection mechanism
		Mutex mutex;
};

#endif
