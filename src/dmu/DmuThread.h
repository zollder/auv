/*
 *	DmuThread.h
 *	Created on: 07.01.2015
 *	Author: zollder
 */

#include "../sys/BaseThread.h"
#include "../sys/FdTimer.h"
#include "../data/SensorData.h"
#include "../dmu/DMU.h"

#ifndef dmuthread_h
#define dmuthread_h

#define DMU_READ_INTERVAL 0.09	// 9ms
#define DMU_CONVERT_INTERVAL 0.50	// 9ms

//-----------------------------------------------------------------------------------------
// DmuThread interface.
//-----------------------------------------------------------------------------------------
class DmuThread : public BaseThread
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		DmuThread(SensorData* data);

		// destructor
		~DmuThread();

		// overrides BaseThread's run() method
		void* run();

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:
		float temperature = 0;
		float pressure = 0;
		int depth = 0;

		FdTimer* timer;
		DMU* dmu;
		SensorData* sensorData;
};

#endif
