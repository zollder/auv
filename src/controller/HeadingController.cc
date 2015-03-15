/*
 *	HeadingController.cpp
 *  Created on: 20.12.2014
 *	Author: zollder
 */

#include "HeadingController.h"

//---------------------------------------------------------------------------------------------
// HeadingController controller thread implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	HeadingController::HeadingController(SensorData* sensorData_p, DesiredData* desiredData_p)
	{
		printf("Constructing HeadingController controller thread...\n");

		setThreadId(HC_THREAD_ID);
		timer = new FdTimer(getThreadId(), HC_INTERVAL);
		pwm = new PWM();
		yawPid = new PID(YAW_KP, YAW_KI, YAW_KD);

		sensorData = sensorData_p;
		desiredData = desiredData_p;
	}

	//-----------------------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------------------
	HeadingController::~HeadingController()
	{
		printf("Destroying HeadingController controller thread ...\n");
		delete pwm;
		delete timer;
		delete yawPid;
	}

	//-----------------------------------------------------------------------------------------
	// Overrides BaseThread's run() method
	//-----------------------------------------------------------------------------------------
	void* HeadingController::run()
	{
		yawPid->reset();

		pwm->setPeriod(PWM_MODULE_2_ID, PWM_PERIOD_HZ);

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

			// determine if the device should turn or drift and use an appropriate algorithm
			if (drift)
				this->calculateDriftDuty();
			else
				this->calculateCorrectiveDuty();

			// write calculated duty cycle values
			this->adjustDutyCycle();

			/*printf("------------3:%d 4:%d\n", currentDuty3, currentDuty4);*/
		}

		return NULL;
	}

	//-----------------------------------------------------------------------------------------
	/** Copies measured and desired values from shared data holders for processing. */
	//-----------------------------------------------------------------------------------------
	void HeadingController::getherData()
	{
		desiredData->mutex.lock();
			desiredHeading = desiredData->heading;
			drift = desiredData->drift;
			driftDirection = desiredData->driftDirection;
			driftAngle = desiredData->driftAngle;
		desiredData->mutex.unlock();

		sensorData->mutex.lock();
			actualHeading = floor(sensorData->yaw + 0.5);
		sensorData->mutex.unlock();

//		printf("Heading: %10d, \n", actualHeading);
	}

	//-----------------------------------------------------------------------------------------
	/** Calculates corrective duty for each motor based on the IMU yaw readings.
	 *  Stores calculated values in the corresponding instance variables. */
	//-----------------------------------------------------------------------------------------
	void HeadingController::calculateCorrectiveDuty()
	{
		yawDuty = yawPid->calculate(desiredHeading, actualHeading, HC_INTERVAL)/2;

		frontDuty = yawDuty;
		rearDuty = yawDuty;

	}

	//-----------------------------------------------------------------------------------------
	/** Calculates drift duty for each motor based on the yaw and desired heading readings.
	 *  Stores calculated values in the corresponding instance variables. */
	//-----------------------------------------------------------------------------------------
	void HeadingController::calculateDriftDuty()
	{
		// TODO: implement drift algorithm
	}

	//-----------------------------------------------------------------------------------------
	/** Adjusts duty cycle of each motor based on processed measured and desired data. */
	//-----------------------------------------------------------------------------------------
	void HeadingController::adjustDutyCycle()
	{
		// verify if the difference is large enough to apply the changes, if necessary
		if (abs(frontDuty - lastFrontDuty) > 0)
		{
			pwm->setDuty(21, frontDuty);
			lastFrontDuty = frontDuty;
		}

		if (abs(rearDuty - lastRearDuty) > 0)
		{
			pwm->setDuty(22, rearDuty);
			lastRearDuty = rearDuty;
		}
	}

