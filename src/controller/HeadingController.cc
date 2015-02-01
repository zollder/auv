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
	}

	//-----------------------------------------------------------------------------------------
	// Overrides BaseThread's run() method
	//-----------------------------------------------------------------------------------------
	void* HeadingController::run()
	{
		timer->start();

		pwm->setPeriod(PWM_MODULE_2_ID, BASE_PERIOD);

		pwm->setPolarity(21, 0);
		pwm->setPolarity(22, 0);

		pwm->setDuty(21, 0);
		pwm->setDuty(22, 0);

		pwm->start(21);
		pwm->start(22);

		while(1)
		{
			timer->waitTimerEvent();

			// read current and desired position-related values
			this->getherData();

			// write calculated duty cycle values
			this->adjustDutyCycle();

			// determine if the device should turn or drift and use an appropriate algorithm
			if (rightDrift or leftDrift)
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
			rightDrift = desiredData->rightDrift;
			leftDrift = desiredData->leftDrift;
			driftAngle = desiredData->driftAngle;
		desiredData->mutex.unlock();

		sensorData->mutex.lock();
			currentYaw = sensorData->yaw;
		sensorData->mutex.unlock();
	}

	//-----------------------------------------------------------------------------------------
	/** Calculates corrective duty for each motor based on the IMU yaw readings.
	 *  Stores calculated values in the corresponding instance variables. */
	//-----------------------------------------------------------------------------------------
	void HeadingController::calculateCorrectiveDuty()
	{
		// TODO: needs control algorithm

		// for testing purposes only
		normalizedYaw = currentYaw/2;
		if (currentYaw > 0)
		{
			newDuty3 = normalizedYaw;
			newDuty4 = normalizedYaw;
		}
		else
		{
			newDuty3 = -normalizedYaw;
			newDuty4 = -normalizedYaw;
		}
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
		if (abs(newDuty3 - currentDuty3) > 0)
		{
			pwm->setDuty(21, newDuty3);
			currentDuty3 = newDuty3;
		}

		if (abs(newDuty4 - currentDuty4) > 0)
		{
			pwm->setDuty(22, newDuty4);
			currentDuty4 = newDuty4;
		}
	}

