/*
 *	AltitudeController.cpp
 *  Created on: 17.12.2014
 *	Author: zollder
 */

#include "AltitudeController.h"

//---------------------------------------------------------------------------------------------
// AltitudeController controller thread implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	AltitudeController::AltitudeController(SensorData* sensorData_p, DesiredData* desiredData_p)
	{
		printf("Constructing AltitudeController controller thread...\n");

		setThreadId(AC_THREAD_ID);
		timer = new FdTimer(getThreadId(), AC_INTERVAL);

		pwm = new PWM();
		altitudePid = new PID(ALT_KP, ALT_KI, ALT_KD);
		pitchPid = new PID(PITCH_KP, PITCH_KI, PITCH_KD);

		sensorData = sensorData_p;
		desiredData = desiredData_p;
	}

	//-----------------------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------------------
	AltitudeController::~AltitudeController()
	{
		printf("Destroying AltitudeController controller thread ...\n");
		delete pwm;
		delete timer;
		delete altitudePid;
		delete pitchPid;
	}

	//-----------------------------------------------------------------------------------------
	// Overrides BaseThread's run() method
	//-----------------------------------------------------------------------------------------
	void* AltitudeController::run()
	{
		altitudePid->reset();
		pitchPid->reset();

		pwm->setPeriod(PWM_MODULE_2_ID, BASE_PERIOD);

		pwm->setDuty(21, 0);
		pwm->setDuty(22, 0);

		pwm->start(21);
		pwm->start(22);

		timer->start();

		while(1)
		{
			timer->waitTimerEvent();

			// read current and desired position-related values
			this->getherData();

			// calculate base duty cycle
			this->calculateAltitudeDuty();

			// calculate corrective duty cycle for each motor
			this->calculatePitchDuty();

			// write calculated duty cycle values
			this->adjustDutyCycle();

//			printf("----------------------------------1:%d 2:%d\n", lastFrontDuty, lastRearDuty);
		}

		return NULL;
	}

	//-----------------------------------------------------------------------------------------
	/** Sets altitude controller PID coefficients. */
	//-----------------------------------------------------------------------------------------
	void AltitudeController::setAltitudePidValues(float p, float i, float d)
	{
		this->altitudePid->setPidCoefficients(p, i, d);
	}

	//-----------------------------------------------------------------------------------------
	/** Sets pitch controller PID coefficients. */
	//-----------------------------------------------------------------------------------------
	void AltitudeController::setPitchPidValues(float p, float i, float d)
	{
		this->pitchPid->setPidCoefficients(p, i, d);
	}

	//-----------------------------------------------------------------------------------------
	/** Copies measured and desired values from shared data holders for processing. */
	//-----------------------------------------------------------------------------------------
	void AltitudeController::getherData()
	{
		desiredData->mutex.lock();
			desiredDepth = desiredData->depth;
		desiredData->mutex.unlock();

		sensorData->mutex.lock();
			actualPitch = sensorData->pitch;
			actualDepth = sensorData->depth;
		sensorData->mutex.unlock();
	}

	//-----------------------------------------------------------------------------------------
	/** Calculates duty cycle for both motors based on the desired depth. */
	//-----------------------------------------------------------------------------------------
	void AltitudeController::calculateAltitudeDuty()
	{
		baseDuty = altitudePid->calculate(desiredDepth, actualDepth, AC_INTERVAL);
	}

	//-----------------------------------------------------------------------------------------
	/** Calculates corrective pitch duty value for each motor. */
	//-----------------------------------------------------------------------------------------
	void AltitudeController::calculatePitchDuty()
	{
		pitchDuty = pitchPid->calculate(0, actualPitch, AC_INTERVAL)/2;
	}

	//-----------------------------------------------------------------------------------------
	/** Adjusts duty cycle of each motor based on processed measured and desired data. */
	//-----------------------------------------------------------------------------------------
	void AltitudeController::adjustDutyCycle()
	{
		// find front/rear motor duties
		frontDuty = baseDuty - pitchDuty;
		rearDuty = baseDuty + pitchDuty;

		// verify if the difference is large enough to apply the changes, if necessary
		if (abs(frontDuty - lastFrontDuty) > 1)
		{
			pwm->setDuty(21, frontDuty);
			lastFrontDuty = frontDuty;
		}

		if (abs(rearDuty - lastRearDuty) > 1)
		{
			pwm->setDuty(22, rearDuty);
			lastRearDuty = rearDuty;
		}

		printf("----------------------------------1:%d 2:%d\n", frontDuty, rearDuty);
	}

