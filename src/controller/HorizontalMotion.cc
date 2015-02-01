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

			// calculate duty cycle
			this->calculateDuty();

			// write calculated duty cycle values
			this->adjustDutyCycle();

			printf("------------5&6:%d\n", currentDuty);
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
			this->rightDrift = desiredData->rightDrift;
			this->leftDrift = desiredData->leftDrift;
			this->reverse = desiredData->reverse;
			this->speed = desiredData->speed;
		desiredData->mutex.unlock();

		if (rightDrift or leftDrift)
			this->drifting = true;
		else
			this->drifting = false;
	}

	//-----------------------------------------------------------------------------------------
	/** Calculates duty for each motor based on the desired speed.
	 *  Stores calculated values in the corresponding instance variables. */
	//-----------------------------------------------------------------------------------------
	void HorizontalMotion::calculateDuty()
	{
		// TODO: implement forward/reverse direction logic

		/*printf("Calculating duty values ...\n");*/

		if (drifting)
			this->speed = 0;

		newDuty = speedLevel[speed];
	}

	//-----------------------------------------------------------------------------------------
	/** Adjusts duty cycle of each motor. */
	//-----------------------------------------------------------------------------------------
	void HorizontalMotion::adjustDutyCycle()
	{
		/*printf("Adjusting duty cycle for motors 1 & 2 ...\n");*/
/*
		if (reverse)
			// TODO: switch to reverse
		else
			// TODO: switch to forward
*/

		// verify if the difference is large enough to apply the changes, if necessary
		if (newDuty != currentDuty)
		{
			pwm->setDuty(31, newDuty);
			pwm->setDuty(32, newDuty);
			currentDuty = newDuty;
		}
	}

