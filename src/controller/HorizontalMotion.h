/*
 *	HorizontalMotion.h
 *	Created on: 20.12.2014
 *	Author: zollder
 */

#include "../sys/BaseThread.h"
#include "../sys/FdTimer.h"
#include "../data/DataService.h"
#include "../sys/PWM.h"
#include "../commons/Config.h"

#ifndef horizontalmotion_h
#define horizontalmotion_h

//-----------------------------------------------------------------------------------------
// HorizontalMotion interface.
//-----------------------------------------------------------------------------------------
class HorizontalMotion : public BaseThread
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		HorizontalMotion(DataService* service);

		// destructor
		~HorizontalMotion();

		// overrides BaseThread's run() method
		void* run();

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:

		FdTimer* timer;
		PWM* pwm;

		/* data source definitions */
		DataService* dataService;

		/* motion-related flags */
		bool reverse  = false;
		bool drift = false;
		int driftDirection = 0;	// 0:left, 1:right

		int newSpeed = 0;
		int currentSpeed = 0;

		/* predefined speed levels */
		int speedLevel[7] = {0,5,15,30,50,75,100};


		/* private helper methods */
		void getherData(void);
		void adjustDutyCycle(void);
};

#endif
