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

		// front (f) camera data holders
		int objtId_f = 0;
		int objColor_f = 0;
		int objOffsetX_f = 0;
		int objOffsetY_f = 0;
		int radius_f = 0;
		int objWidth_f = 0;
		int objHeight_f = 0;
		int objAngle_f = 0;

		// bottom (b) camera data holders
		int objtId_b = 0;
		int objColor_b = 0;
		int objOffsetX_b = 0;
		int objOffsetY_b = 0;
		int radius_b = 0;
		int objWidth_b = 0;
		int objHeight_b = 0;
		int objAngle_b = 0;

		// read/write protection mechanism
		Mutex mutex;
};

#endif
