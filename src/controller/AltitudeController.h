/*
 *	AltitudeController.h
 *	Created on: 17.12.2014
 *	Author: zollder
 */

#include "../sys/BaseThread.h"
#include "../sys/FdTimer.h"
#include "../sys/PWM.h"
#include "../data/DataService.h"
#include "../commons/Config.h"

#include "PID.h"

#ifndef altitudecontroller_h
#define altitudecontroller_h

//-----------------------------------------------------------------------------------------
// AltitudeController interface.
//-----------------------------------------------------------------------------------------
class AltitudeController : public BaseThread
{
	/* altitude controller PID coefficients (constants) */
	const float altKp = 0;
	const float altKi = 0;
	const float altKd = 0;

	/* pitch controller coefficients */
	const float pitchKp = 0.25;
	const float pitchKi = 0;
	const float pitchKd = 0;

	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		AltitudeController(DataService* service);

		// destructor
		~AltitudeController();

		// overrides BaseThread's run() method
		void* run();

		/* sets PID coefficients for altitude and pitch controllers */
		void setAltitudePidValues(float, float, float);
		void setPitchPidValues(float, float, float);

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:

		FdTimer* timer;
		PWM* pwm;

		PID* altitudePid;
		PID* pitchPid;

		/* data source definitions */
		DataService* dataService;

		/* position-related data holders */
		float actualPitch = 0;
		int actualDepth = 0;
		int desiredDepth = 0;

		/* duty cycle related instance variables */
		int baseDuty = 0;
		int pitchDuty = 0;

		int frontDuty = 0;
		int rearDuty = 0;

		int lastFrontDuty = 0;
		int lastRearDuty = 0;


		/* private helper methods */
		void getherData(void);
		void calculateAltitudeDuty(void);
		void calculatePitchDuty(void);
		void adjustDutyCycle(void);
};

#endif
