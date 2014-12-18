/*
 *	SensorData.h
 *  Created on: 13.12.2014
 *	Author: zollder
 */

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

		double roll = 0;
		double pitch = 0;
		double yaw = 0;

		int depth = 0;

		// read/write protection mechanism
		Mutex mutex;
};

#endif
