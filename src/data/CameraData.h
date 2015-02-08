/*
 *	CameraData.h
 *  Created on: 13.12.2014
 *	Author: zollder
 */

#include "Mutex.h"

#ifndef cameradata_h
#define cameradata_h

//-----------------------------------------------------------------------------------------
// CameraData holder definition.
//-----------------------------------------------------------------------------------------
class CameraData
{

	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		CameraData(){};

		// destructor
		~CameraData(){};

		int x = 0;
		int y = 0;
		int color = 0;

		// read/write protection mechanism
		Mutex mutex;
};

#endif
