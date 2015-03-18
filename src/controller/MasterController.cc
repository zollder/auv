/*
 *	MasterController.cpp
 *  Created on: 20.12.2014
 *	Author: zollder
 */

#include "MasterController.h"

//---------------------------------------------------------------------------------------------
// MasterController controller thread implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	MasterController::MasterController(DataService* service)
	{
		printf("Constructing MasterController thread...\n");

		setThreadId(MC_THREAD_ID);
		timer = new FdTimer(getThreadId(), HM_INTERVAL);

		this->dataService = service;
	}

	//-----------------------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------------------
	MasterController::~MasterController()
	{
		printf("Destroying MasterController thread ...\n");
		delete timer;
	}

	//-----------------------------------------------------------------------------------------
	// Overrides BaseThread's run() method
	//-----------------------------------------------------------------------------------------
	void* MasterController::run()
	{
		timer->start();



		while(1)
		{
			timer->waitTimerEvent();

			// read current and desired position-related values
//			this->getherData();

			// write calculated duty cycle values
//			this->adjustDutyCycle();
		}

		return NULL;
	}

//	//-----------------------------------------------------------------------------------------
//	/** Copies measured values from shared data holders for processing. */
//	//-----------------------------------------------------------------------------------------
//	void MasterController::copyMeasuredData()
//	{
//		dataService->cameraData->mutex.lock();
//			dataHolder = dataService->getData();
//		dataService->cameraData->mutex.unlock();
//	}
//
//	//-----------------------------------------------------------------------------------------
//	/** Adjusts duty cycle of each motor. */
//	//-----------------------------------------------------------------------------------------
//	void MasterController::adjustDutyCycle()
//	{
//		if (reverse == false)
//		{
//			// verify if the difference is large enough to apply the changes, if necessary
//			if (newSpeed != currentSpeed)
//			{
//				pwm->setDuty(32, speedLevel[newSpeed]);
//				currentSpeed = newSpeed;
//			}
//		}
//		else
//		{
//			if (newSpeed != currentSpeed)
//			{
//				pwm->setDuty(32, -(speedLevel[newSpeed]));
//				currentSpeed = newSpeed;
//			}
//		}
//
//	}

