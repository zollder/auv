/*
 *	ImuThread.h
 *	Created on: 13.12.2014
 *	Author: zollder
 */

#include "../sys/BaseThread.h"
#include "../sys/FdTimer.h"
#include "../data/SensorData.h"
#include "../imu/IMU.h"
#include "../commons/Config.h"

#ifndef imuthread_h
#define imuthread_h

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
