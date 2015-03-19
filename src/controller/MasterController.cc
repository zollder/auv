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
		periodicTimer = new FdTimer(getThreadId(), MC_INTERVAL);
		delayTimer = new FdTimer(MC_SH_TIMER, 0);

		dataService = service;
		imuThread = new ImuThread(dataService->sensorData);
		dmuThread = new DmuThread(dataService->sensorData);

		altitudeControllerThread = new AltitudeController(dataService);
		headingControllerThread = new HeadingController(dataService);
		horizontalMotionThread = new HorizontalMotion(dataService);
	}

	//-----------------------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------------------
	MasterController::~MasterController()
	{
		printf("Destroying MasterController thread ...\n");
		delete periodicTimer;
		delete delayTimer;

		delete horizontalMotionThread;
		delete headingControllerThread;
		delete altitudeControllerThread;

		delete dmuThread;
		delete imuThread;
	}

	//-----------------------------------------------------------------------------------------
	// Overrides BaseThread's run() method
	//-----------------------------------------------------------------------------------------
	void* MasterController::run()
	{
		this->startDataCollection();
		this->saveCurrentPosition();
		this->startMotionControllers();

		periodicTimer->start();

		while(1)
		{
			periodicTimer->waitTimerEvent();

			this->copyMeasuredData();

			int mission = this->detectMission();
			switch (mission)
			{
				case 1:
					this->submerge();
					break;
				case 2:
					this->emerge();
					break;
				case 3:
					this->followLine();
					break;
				default:
					this->executeDefault();
			};
		}

		return NULL;
	}

	//-----------------------------------------------------------------------------------------
	/** Copies measured values from shared data holders for processing. */
	//-----------------------------------------------------------------------------------------
	void MasterController::copyMeasuredData()
	{
		sensorData = dataService->sensorData->getSensorData();
		bottomCam = dataService->cameraData->getDataBottom();
		frontCam = dataService->cameraData->getDataFront();
	}

	//-----------------------------------------------------------------------------------------
	/** Identifies current mission based on the current sensor and camera data.
	 *  Categorizes the mission and returns corresponding mission ID. */
	//-----------------------------------------------------------------------------------------
	int MasterController::detectMission()
	{
		// submerge/emerge mission
		if (sensorData.m3 < minDepth)
			return 1;

		// line following mission
		if (bottomCam.m4 * bottomCam.m5 >= 7800)
			return 2;

		// gate-passing mission
		if (frontCam.m4 * frontCam.m5 >= 5000)
			return 3;

		// default mission
		return 0;
	}

	//-----------------------------------------------------------------------------------------
	/**  */
	//-----------------------------------------------------------------------------------------
	void MasterController::submerge()
	{
		// set mission instructions
		dataService->desiredData->resetData();
		dataService->desiredData->mutex.lock();
			dataService->desiredData->depth = 100;
		dataService->desiredData->mutex.unlock();
	}

	//-----------------------------------------------------------------------------------------
	/**  */
	//-----------------------------------------------------------------------------------------
	void MasterController::emerge()
	{
		// set mission instructions
		dataService->desiredData->resetData();
	}

	//-----------------------------------------------------------------------------------------
	/**  */
	//-----------------------------------------------------------------------------------------
	void MasterController::followLine()
	{
	}

	//-----------------------------------------------------------------------------------------
	/**  */
	//-----------------------------------------------------------------------------------------
	void MasterController::executeDefault()
	{
	}

//-----------------------------------------------------------------------------------------
// Helper methods (used only at initialization stage)
//-----------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	/** Starts data collector threads.
	 *  Waits specified period of time required for initialization procedures. */
	//-----------------------------------------------------------------------------------------
	void MasterController::startDataCollection()
	{
		// start data collection
		imuThread->start();
		dmuThread->start();

		// wait N seconds, start single-shot timer and wait until it expires
		delayTimer->setInterval(initTimer);
		delayTimer->startSingle();
		delayTimer->waitTimerEvent();
	}

	//-----------------------------------------------------------------------------------------
	/** Saves current device position (heading and depth).
	 *  Sets initial desired values. */
	//-----------------------------------------------------------------------------------------
	void MasterController::saveCurrentPosition()
	{
		sensorData = dataService->sensorData->getSensorData();

		currentHeading = desiredHeading = sensorData.m2;
		currentDepth = desiredDepth = sensorData.m3;

		// update desired data
		dataService->desiredData->mutex.lock();
			dataService->desiredData->heading = sensorData.m2;
			dataService->desiredData->depth = sensorData.m3;
		dataService->desiredData->mutex.unlock();
	}

	//-----------------------------------------------------------------------------------------
	/** Starts motion controller threads. */
	//-----------------------------------------------------------------------------------------
	void MasterController::startMotionControllers()
	{
		// start data collection
		altitudeControllerThread->start();
		headingControllerThread->start();
		horizontalMotionThread->start();
	}
