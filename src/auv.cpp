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
	printf("[KPI::MAIN] START\n");

	SensorData* sensorData = new SensorData();
	DesiredData* targetData = new DesiredData();
	CameraData* camData = new CameraData();

	ImuThread* imuThread = new ImuThread(sensorData);
	//DmuThread* dmuThread = new DmuThread(sensorData);

	DataService* dataService = new DataService(sensorData, targetData, camData);

	ServerThread* serverThread = new ServerThread( 5000, 2 , dataService);

	serverThread->start();

	imuThread->start();
	//dmuThread->start();

    char key;
    do
    {
        key = getchar();

    }while(key != ESC);

    printf("[KPI::MAIN]::Escape Character Triggered\n");

    if( serverThread->stop() != 0 )
    {
    	printf("[KPI::MAIN] failed stop\n");
    	serverThread->kill();
    }

    delete serverThread;
    delete sensorData;
    delete targetData;
    delete camData;
    delete dataService;
    //delete dmuThread;
    delete imuThread;

    printf("[KPI::MAIN] END\n");

    return EXIT_SUCCESS;
}
