/*
 *	DesiredData.h
 *  Created on: 17.12.2014
 *	Author: zollder
 */

#include "Mutex.h"

#ifndef desireddata_h
#define desireddata_h

//-----------------------------------------------------------------------------------------
// DesiredData interface.
//-----------------------------------------------------------------------------------------
class DesiredData
{

	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		DesiredData() {}

		// destructor
		~DesiredData() {}

		// desired depth in cm (integer can be used)
		int depth = 0;

		// desired direction (degrees relative to North)
		int heading = 0;

		/* desired horizontal (forward/reverse) speed (predefined levels, from 0 to 8) */
		int speed = 0;

		// desired drift properties
		bool drift = false;
		int driftDirection = 0;	// 0:left, 1:right
		float driftAngle = 0;

		// forward/reverse flag
		bool reverse = false;

		// read/write protection mechanism
		Mutex mutex;
};

#endif
