/*
 *	CameraData.h
 *  Created on: 13.12.2014
 *	Author: zollder
 */

#include "Mutex.h"
#include "../commons/Dto.h"

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

		//-----------------------------------------------------------------------------------------
		/** Copies and returns front camera data as an array of floats. */
		//-----------------------------------------------------------------------------------------
		dto<int> getDataFront()
		{
			dto<int> data;
			mutex.lock();
				data.m1 = (int) objOffsetX_f;
				data.m2 = (int) objOffsetY_f;
				data.m3 = (int) objAngle_f;
				data.m4 = (int) objWidth_f;
				data.m5 = (int) objHeight_f;
				data.m6 = (int) radius_f;
			mutex.unlock();

			return data;
		}

		//-----------------------------------------------------------------------------------------
		/** Copies and returns bottom camera data as an array of floats. */
		//-----------------------------------------------------------------------------------------
		dto<int> getDataBottom()
		{
			dto<int> data;
			mutex.lock();
				data.m1 = (int) objOffsetX_b;
				data.m2 = (int) objOffsetY_b;
				data.m3 = (int) objAngle_b;
				data.m4 = (int) objWidth_b;
				data.m5 = (int) objHeight_b;
				data.m6 = (int) radius_b;
			mutex.unlock();

			return data;
		}
};

#endif
