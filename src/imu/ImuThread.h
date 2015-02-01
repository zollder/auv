/*
 *	ImuThread.h
 *	Created on: 13.12.2014
 *	Author: zollder
 */

#include "../sys/BaseThread.h"
#include "../sys/FdTimer.h"
#include "../data/SensorData.h"
#include "../imu/IMU.h"

#ifndef imuthread_h
#define imuthread_h

#define IMU_THREAD_ID 1
#define IMU_INTERVAL 0.02	// 20ms

//-----------------------------------------------------------------------------------------
// ImuThread interface.
//-----------------------------------------------------------------------------------------
class ImuThread : public BaseThread
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		ImuThread(SensorData* data);

		// destructor
		~ImuThread();

		// overrides BaseThread's run() method
		void* run();

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:
		double rollLocal = 0;
		double pitchLocal = 0;
		double yawLocal = 0;

		FdTimer* timer;
		IMU* imu;
		SensorData* sensorData;
};

#endif
