//============================================================================
// Name        : auv.cpp
// Author      : zollder
//============================================================================

#include "communication/ServerThread.h"

#include "imu/ImuThread.h"
#include "dmu/DmuThread.h"
#include "controller/AltitudeController.h"
#include "controller/HeadingController.h"
#include "controller/HorizontalMotion.h"
#include "dmu/DMU.h"
#include "data/DataService.h"

using namespace std;

#define ESC 27

int main(int argc, char *argv[])
{
	syslog(LOG_NOTICE,"[KPI::MAIN] START");

	SensorData* sensorData = new SensorData();
	DesiredData* targetData = new DesiredData();
	CameraData* camData = new CameraData();

	//DEBUGImuThread* imuThread = new ImuThread(sensorData);
	//DmuThread* dmuThread = new DmuThread(sensorData);

	DataService* dataService = new DataService(sensorData, targetData, camData);

	ServerThread* serverThread = new ServerThread( 5000, 2 , dataService);

	serverThread->start();

	//DEBUGimuThread->start();
	//dmuThread->start();

    char key;
    do
    {
        key = getchar();

    }while(key != ESC);

    syslog(LOG_NOTICE,"[KPI::MAIN]::Escape Character Triggered");


    delete serverThread;
    delete sensorData;
    delete targetData;
    delete camData;
    delete dataService;
    //delete dmuThread;
    //DEBUGdelete imuThread;

    syslog(LOG_NOTICE,"[KPI::MAIN] END");

    return EXIT_SUCCESS;
}
