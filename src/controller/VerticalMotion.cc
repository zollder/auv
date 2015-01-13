/*
 *	VerticalMotion.cpp
 *  Created on: 17.12.2014
 *	Author: zollder
 */

#include "VerticalMotion.h"

//---------------------------------------------------------------------------------------------
// VerticalMotion controller thread implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	VerticalMotion::VerticalMotion(SensorData* sensorData_p, DesiredData* desiredData_p)
	{
		printf("Constructing VerticalMotion controller thread...\n");

		setThreadId(T1_ID);
		timer = new FdTimer(getThreadId(), T1_INTERVAL);
		pwm = new PWM();

		sensorData = sensorData_p;
		desiredData = desiredData_p;
	}

	//-----------------------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------------------
	VerticalMotion::~VerticalMotion()
	{
		printf("Destroying VerticalMotion controller thread ...\n");
		delete pwm;
		delete timer;
	}

	//-----------------------------------------------------------------------------------------
	// Overrides BaseThread's run() method
	//-----------------------------------------------------------------------------------------
	void* VerticalMotion::run()
	{
		timer->start();

		pwm->setPeriod(PWM_MODULE_1_ID, BASE_PERIOD);

		pwm->setPolarity(11, 0);
		pwm->setPolarity(12, 0);

		pwm->setDuty(11, 0);
		pwm->setDuty(12, 0);

		pwm->start(11);
		pwm->start(12);

		int counter = 0;
		while(1)
		{
			timer->waitTimerEvent();

			// read current and desired position-related values
			this->getherData();

			// calculate base duty cycle
			this->calculateBaseDuty();

			// calculate corrective duty cycle for each motor
			this->calculateCorrectiveDuties();

			// write calculated duty cycle values
			this->adjustDutyCycle();

			printf("----------------------------------1:%d 2:%d\n", currentDuty1, currentDuty2);

			counter++;
		}

		return NULL;
	}

	//-----------------------------------------------------------------------------------------
	/** Copies measured and desired values from shared data holders for processing. */
	//-----------------------------------------------------------------------------------------
	void VerticalMotion::getherData()
	{
//		printf("Calculating corrective duty values ...\n");
		desiredData->mutex.lock();
			desiredDepth = desiredData->depth;
		desiredData->mutex.unlock();

		sensorData->mutex.lock();
			currentPitch = sensorData->pitch;
			currentDepth = sensorData->depth;
		sensorData->mutex.unlock();
	}

	//-----------------------------------------------------------------------------------------
	/** Calculates base duty cycle for both motors based on the desired depth.
	 *  Stores calculated values in the corresponding instance variables. */
	//-----------------------------------------------------------------------------------------
	void VerticalMotion::calculateBaseDuty()
	{
		// TODO: needs control algorithm
//		printf("Calculating base duty value ...\n");

		// for testing purposes only
		baseDuty = 40;
	}

	//-----------------------------------------------------------------------------------------
	/** Calculates corrective duty values for each motor based on the IMU pitch readings..
	 *  Stores calculated values in the corresponding instance variables. */
	//-----------------------------------------------------------------------------------------
	void VerticalMotion::calculateCorrectiveDuties()
	{
		// TODO: needs control algorithm
//		printf("Calculating corrective duty values ...\n");

		// for testing purposes only
		normalizedPitch = currentPitch/2;
		if (currentPitch < 0)
		{
			correctiveDuty1 = normalizedPitch;
			correctiveDuty2 = -normalizedPitch;
		}
		else
		{
			correctiveDuty1 = normalizedPitch;
			correctiveDuty2 = -normalizedPitch;
		}
	}

	//-----------------------------------------------------------------------------------------
	/** Adjusts duty cycle of each motor based on processed measured and desired data. */
	//-----------------------------------------------------------------------------------------
	void VerticalMotion::adjustDutyCycle()
	{
//		printf("Adjusting duty cycle for motors 1 & 2 ...\n");

		// verify if the difference is large enough to apply the changes, if necessary
		int newDuty1 = baseDuty + correctiveDuty1;
		int newDuty2 = baseDuty + correctiveDuty2;

		if (abs(newDuty1 - currentDuty1) > 1)
		{
			pwm->setDuty(11, newDuty1);
			currentDuty1 = newDuty1;
		}

		if (abs(newDuty2 - currentDuty2) > 1)
		{
			pwm->setDuty(12, newDuty2);
			currentDuty2 = newDuty2;
		}
	}

