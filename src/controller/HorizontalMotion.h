/*
 *	HorizontalMotion.h
 *	Created on: 20.12.2014
 *	Author: zollder
 */

#include "../sys/BaseThread.h"
#include "../sys/FdTimer.h"
#include "../data/SensorData.h"
#include "../data/DesiredData.h"
#include "../sys/PWM.h"

#ifndef horizontalmotion_h
#define horizontalmotion_h

#define T3_INTERVAL 0.05	// 50 ms

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
		HorizontalMotion(SensorData* sData, DesiredData* dData);

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
		DesiredData* desiredData;
		SensorData* sensorData;

		/* motion-related flags */
		bool moving = false;
		bool drifting = false;
		bool reverse  = false;
		int speed = 0;

		/* predefined speed levels */
		int speedLevel[9] = {0,5,10,15,20,25,50,75,100};

		bool rightDrift = false;
		bool leftDrift = false;

		/* duty cycle related instance variables */
		int currentDuty = 0;
		int newDuty = 0;

		/* private helper methods */
		void getherData(void);
		void calculateDuty(void);
		void adjustDutyCycle(void);
};

#endif
