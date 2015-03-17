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
	AltitudeController::AltitudeController(DataService* service)
	{
		printf("Constructing AltitudeController controller thread...\n");

		setThreadId(AC_THREAD_ID);
		timer = new FdTimer(getThreadId(), AC_INTERVAL);

		pwm = new PWM();
		altitudePid = new PID(ALT_KP, ALT_KI, ALT_KD);
		pitchPid = new PID(PITCH_KP, PITCH_KI, PITCH_KD);

		dataService = service;
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

		pwm->setPeriod(PWM_MODULE_1_ID, BASE_PERIOD);

		pwm->setDuty(11, 0);
		pwm->setDuty(12, 0);

		pwm->start(11);
		pwm->start(12);

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
		dataService->desiredData->mutex.lock();
			desiredDepth = dataService->desiredData->depth;
		dataService->desiredData->mutex.unlock();

		dataService->sensorData->mutex.lock();
			actualPitch = dataService->sensorData->pitch;
			actualDepth = dataService->sensorData->depth;
		dataService->sensorData->mutex.unlock();
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
			pwm->setDuty(11, frontDuty);
			lastFrontDuty = frontDuty;
		}

		if (abs(rearDuty - lastRearDuty) > 1)
		{
			pwm->setDuty(12, rearDuty);
			lastRearDuty = rearDuty;
		}

//		printf("----------------------------------1:%d 2:%d\n", frontDuty, rearDuty);
	}

