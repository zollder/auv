/*
 *	HeadingController.h
 *	Created on: 20.12.2014
 *	Author: zollder
 */

#include "../sys/BaseThread.h"
#include "../sys/FdTimer.h"
#include "../sys/PWM.h"
#include "../data/DataService.h"
#include "../commons/Config.h"

#include "PID.h"

#ifndef headingcontroller_h
#define headingcontroller_h

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
		HeadingController(DataService* service);

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
		PID* yawPid;

		/* data source definitions */
		DataService* dataService;

		/* position-related data holders */
		int actualHeading = 0;
		int desiredHeading = 0;

		// drift-related instance variables
		bool drift = false;
		int driftDirection = 0;	// 0:left, 1:right
		float driftAngle = 0;

		/* duty cycle related instance variables */

		int yawDuty = 0;
		int frontDuty = 0;
		int rearDuty = 0;

		int lastFrontDuty = 0;
		int lastRearDuty = 0;

		/* private helper methods */
		void getherData(void);
		void calculateCorrectiveDuty(void);
		void calculateDriftDuty(void);
		void adjustDutyCycle(void);
};

#endif
