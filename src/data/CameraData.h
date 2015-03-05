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
		int camId = 0;

		// front (f) camera data holders
		int objId_f = 0;
		int objColor_f = 0;
		float objOffsetX_f = 0;
		float objOffsetY_f = 0;
		float radius_f = 0;
		float objWidth_f = 0;
		float objHeight_f = 0;
		float objAngle_f = 0;

		// bottom (b) camera data holders
		int objtId_b = 0;
		int objColor_b = 0;
		float objOffsetX_b = 0;
		float objOffsetY_b = 0;
		float radius_b = 0;
		float objWidth_b = 0;
		float objHeight_b = 0;
		float objAngle_b = 0;

		// read/write protection mechanism
		Mutex mutex;
};

#endif
