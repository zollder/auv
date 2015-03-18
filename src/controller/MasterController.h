/*
 *	MasterController.h
 *	Created on: 17.12.2014
 *	Author: zollder
 */

#include "../sys/BaseThread.h"
#include "../sys/FdTimer.h"
#include "../data/DataService.h"
#include "../commons/Config.h"

#ifndef mastercontroller_h
#define mastercontroller_h

//-----------------------------------------------------------------------------------------
// MasterController interface.
//-----------------------------------------------------------------------------------------
class MasterController : public BaseThread
{
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

		FdTimer* timer;

		/* data source definitions */
		DataService* dataService;

		float sensorData[10] = {0,0,0,0,0};
		float camData[10] = {0,0,0,0,0,0,0,0,0,0};

		/* position-related local data holders */
		int currentDepth = 0;
		int actualDepth = 0;
		int desiredDepth = 0;

		int currentHeading = 0;
		int actualHeading = 0;
		int desiredHeading = 0;

		int currentSpeed = 0;
		int desiredSpeed = 0;
		bool reverse = false;

		bool drift = false;
		int driftDirection = 0;


		// copies latest data into local variables
		void copyMeasuredData();
		void saveDesiredData();

		/* mission detector */
		int detectMission(void);

		/* mission executors */
		void executeSubmerge(void);
		void excecuteEmerge(void);
		void executeDefault(void);
		void executeLine(void);
		void executeGate(void);
		void executeControlPanel(void);
		void executeManeuver(void);
};

#endif
