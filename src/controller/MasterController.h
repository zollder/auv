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

		/* position-related local data holders */
		int actualDepth = 0;
		float actualPitch = 0;
		int normalizedPitch = 0;
		int currentDepth = 0;
		int desiredDepth = 0;

		/* duty cycle related instance variables */
		int currentDuty1 = 0;
		int currentDuty2 = 0;

		int baseDuty = 0;

		int correctiveDuty1 = 0;
		int correctiveDuty2 = 0;

		/* private helper methods */
		void detectMission(void);
		void executeDefault(void);
		void executeLine(void);
		void executeGate(void);
		void executeControlPanel(void);
		void executeManeuver(void);
};

#endif
