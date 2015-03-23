/*
 *	MasterController.h
 *	Created on: 17.12.2014
 *	Author: zollder
 */

#include "../sys/FdTimer.h"
#include "../commons/Config.h"
#include "../data/DataService.h"
#include "../commons/Dto.h"

#include "../sys/BaseThread.h"
#include "../imu/ImuThread.h"
#include "../dmu/DmuThread.h"
#include "AltitudeController.h"
#include "HeadingController.h"
#include "HorizontalMotion.h"


#ifndef mastercontroller_h
#define mastercontroller_h

//-----------------------------------------------------------------------------------------
// MasterController interface.
//-----------------------------------------------------------------------------------------
class MasterController : public BaseThread
{
	const double initTimer = 5.0;
	const int minDepth = 50;

	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		MasterController(DataService* service);

		// destructor
		~MasterController();

		// overrides BaseThread's run() method
		void* run();

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:

		ImuThread* imuThread;
		DmuThread* dmuThread;

		AltitudeController* altitudeControllerThread;
		HeadingController* headingControllerThread;
		HorizontalMotion* horizontalMotionThread;

		FdTimer* periodicTimer;
		FdTimer* delayTimer;

		/* data source definitions */
		DataService* dataService;

		/* actual/measured sensor data holders */
		dto<int> sensorData;
		dto<int> frontCam;
		dto<int> bottomCam;

		/* position-related local data holders */
		int currentDepth = 0;
		int desiredDepth = 0;

		int currentHeading = 0;
		int desiredHeading = 0;

		int currentSpeed = 0;
		int desiredSpeed = 0;
		bool reverse = false;
		bool drift = false;
		int driftDirection = 0;

		void startDataCollection(void);
		void saveCurrentPosition(void);
		void startMotionControllers(void);

		// copies latest data into local variables
		void copyMeasuredData(void);
		void saveDesiredData(void);

		/* mission detector */
		int detectMission(void);

		/* mission executors */
		void submerge(void);
		void emerge(void);
		void followLine(void);
		void passGate(void);
		void executeControlPanel(void);
		void maneuver(void);
		void executeDefault(void);
};

#endif
