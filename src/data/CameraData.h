/*
 *	CameraData.h
 *  Created on: 13.12.2014
 *	Author: zollder
 */

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
		CameraData();

		// destructor
		~CameraData();

		int x = 0;
		int y = 0;
		int color = 0;
};

#endif
