/*
 *	HeadingController.h
 *	Created on: 20.12.2014
 *	Author: zollder
 */

#include "../sys/BaseThread.h"
#include "../sys/FdTimer.h"
#include "../data/SensorData.h"
#include "../data/DesiredData.h"
#include "../sys/PWM.h"

#ifndef headingcontroller_h
#define headingcontroller_h

#define HC_THREAD_ID 4
#define HC_INTERVAL 0.03	// 30 ms

//-----------------------------------------------------------------------------------------
// HeadingController interface.
//-----------------------------------------------------------------------------------------
class HeadingController : public BaseThread
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		HeadingController(SensorData* sData, DesiredData* dData);

		// destructor
		~HeadingController();

		// overrides BaseThread's run() method
		void* run();

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:

		FdTimer* timer;
		PWM* pwm;

		/* data source definitions */
		DesiredData* desiredData;
		SensorData* sensorData;

		/* position-related data holders */
		float currentYaw = 0;
		int normalizedYaw = 0;

		int currentHeading = 0;
		float desiredHeading = 0;

		// drift-related instance variables
		bool rightDrift = false;
		bool leftDrift = false;
		float driftAngle = 0;

		/* duty cycle related instance variables */
		int currentDuty3 = 0;
		int currentDuty4 = 0;

		int newDuty3 = 0;
		int newDuty4 = 0;

		/* private helper methods */
		void getherData(void);
		void calculateCorrectiveDuty(void);
		void calculateDriftDuty(void);
		void adjustDutyCycle(void);
};

#endif
