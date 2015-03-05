/*
 *	HorizontalMotion.cpp
 *  Created on: 20.12.2014
 *	Author: zollder
 */

#include "HorizontalMotion.h"

//---------------------------------------------------------------------------------------------
// HorizontalMotion controller thread implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	HorizontalMotion::HorizontalMotion(SensorData* sensorData_p, DesiredData* desiredData_p)
	{
		printf("Constructing HorizontalMotion controller thread...\n");

		setThreadId(HM_THREAD_ID);
		timer = new FdTimer(getThreadId(), HM_INTERVAL);
		pwm = new PWM();

		sensorData = sensorData_p;
		desiredData = desiredData_p;
	}

	//-----------------------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------------------
	HorizontalMotion::~HorizontalMotion()
	{
		printf("Destroying HorizontalMotion controller thread ...\n");
		delete pwm;
		delete timer;
	}

	//-----------------------------------------------------------------------------------------
	// Overrides BaseThread's run() method
	//-----------------------------------------------------------------------------------------
	void* HorizontalMotion::run()
	{
		timer->start();

		pwm->setPeriod(PWM_MODULE_3_ID, BASE_PERIOD);

		pwm->setPolarity(31, 0);
		pwm->setPolarity(32, 0);

		pwm->setDuty(31, 0);
		pwm->setDuty(32, 0);

		pwm->start(31);
		pwm->start(32);

		while(1)
		{
			timer->waitTimerEvent();

			// read current and desired position-related values
			this->getherData();

			// write calculated duty cycle values
			this->adjustDutyCycle();
		}

		return NULL;
	}

	//-----------------------------------------------------------------------------------------
	/** Copies measured and desired values from shared data holders for processing. */
	//-----------------------------------------------------------------------------------------
	void HorizontalMotion::getherData()
	{
		/*printf("Collecting data ...\n");*/
		desiredData->mutex.lock();
			this->drift = desiredData->drift;
			this->driftDirection = desiredData->driftDirection;
			this->reverse = desiredData->reverse;
			this->newSpeed = desiredData->speed;
		desiredData->mutex.unlock();

//		if (drift)
//			this->drifting = true;
//		else
//			this->drifting = false;
	}

	//-----------------------------------------------------------------------------------------
	/** Adjusts duty cycle of each motor. */
	//-----------------------------------------------------------------------------------------
	void HorizontalMotion::adjustDutyCycle()
	{
		if ( reverse == false)
		{
			// verify if the difference is large enough to apply the changes, if necessary
			if (newSpeed != currentSpeed)
			{
				pwm->setDuty(31, speedLevel[newSpeed]);
				pwm->setDuty(32, speedLevel[newSpeed]);
				currentSpeed = newSpeed;
			}
		}
		else
		{
			if (newSpeed != currentSpeed)
			{
				pwm->setDuty(31, -(speedLevel[newSpeed]) );
				pwm->setDuty(32, -(speedLevel[newSpeed]) );
				currentSpeed = newSpeed;
			}
		}

	}

