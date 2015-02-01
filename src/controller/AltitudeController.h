/*
 *	AltitudeController.h
 *	Created on: 17.12.2014
 *	Author: zollder
 */

#include "../sys/BaseThread.h"
#include "../sys/FdTimer.h"
#include "../data/SensorData.h"
#include "../data/DesiredData.h"
#include "../sys/PWM.h"
#include "../sys/PID.h"

#ifndef altitudecontroller_h
#define altitudecontroller_h

#define AC_THREAD_ID 6
#define AC_INTERVAL 0.05	// 20 ms

//-----------------------------------------------------------------------------------------
// AltitudeController interface.
//-----------------------------------------------------------------------------------------
class AltitudeController : public BaseThread
{
	/* altitude controller PID coefficients (constants) */
	const float altKp = 100;
	const float altKi = 20;
	const float altKd = 10;


	/* pitch controller coefficients */
	const float pitchKp = 100;
	const float pitchKi = 20;
	const float pitchKd = 10;

	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		AltitudeController(SensorData* sData, DesiredData* dData);

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
		DesiredData* desiredData;
		SensorData* sensorData;

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
