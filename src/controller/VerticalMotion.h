/*
 *	VerticalMotion.h
 *	Created on: 17.12.2014
 *	Author: zollder
 */

#include "../sys/BaseThread.h"
#include "../sys/FdTimer.h"
#include "../data/SensorData.h"
#include "../data/DesiredData.h"
#include "../sys/PWM.h"

#ifndef verticalnotion_h
#define verticalmotion_h

#define INTERVAL 0.1
#define PWM_MODULE_ID 1

//-----------------------------------------------------------------------------------------
// VerticalMotion interface.
//-----------------------------------------------------------------------------------------
class VerticalMotion : public BaseThread
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		VerticalMotion(SensorData* sData, DesiredData* dData);

		// destructor
		~VerticalMotion();

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
		float currentPitch = 0;
		int currentDepth = 0;
		int desiredDepth = 0;

		/* duty cycle related instance variables */
		int currentDuty1 = 0;
		int currentDuty2 = 0;

		int baseDuty = 0;

		int correctiveDuty1 = 0;
		int correctiveDuty2 = 0;

		/* private helper methods */
		void getherData(void);
		void calculateBaseDuty(void);
		void calculateCorrectiveDuties(void);
		void adjustDutyCycle(void);
};

#endif
