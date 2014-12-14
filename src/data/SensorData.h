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

		double pressure = 0;
};

#endif
