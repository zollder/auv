//============================================================================
// Name        : auv.cpp
// Author      : Main Program Execution Capstone AUV
//============================================================================

#include "communication/ServerThread.h"
#include "communication/ClientThread.h"

#include "imu/ImuThread.h"
#include "dmu/DmuThread.h"
#include "controller/AltitudeController.h"
#include "controller/HeadingController.h"
#include "controller/HorizontalMotion.h"
#include "dmu/DMU.h"
#include "data/DataService.h"
#include "sys/PWM.h"

using namespace std;

int main(int argc, char *argv[])
{
	syslog(LOG_NOTICE,"[KPI::MAIN] START");

	/*object creation*/
	SensorData* sensorData 		= new SensorData();
	DesiredData* targetData 	= new DesiredData();
	CameraData* camData 		= new CameraData();
	DataService* dataService 	= new DataService(sensorData, targetData, camData);
	SocketServer* socketServer 	= new SocketServer(SERVER_PORT, SERVER_CLIENTS, dataService);

	/*threading children*/
	ServerThread* socketServerThread 			= new ServerThread(socketServer);
	ImuThread* imuThread 						= new ImuThread(sensorData);
//	DmuThread* dmuThread = new DmuThread(sensorData);
//	VerticalMotion* verticalMotionThread = new VerticalMotion(sensorData, targetData);
	HeadingController* headingControllerThread 	= new HeadingController(sensorData, targetData);
//	HorizontalMotion* horizontalMotionThread = new HorizontalMotion(sensorData, targetData);

	/*starting mechanisms*/
	socketServerThread->start();
	imuThread->start();
//	dmuThread->start();
//	verticalMotionThread->start();
	headingControllerThread->start();
//	horizontalMotionThread->start();

	/*waiting for children*/
	socketServerThread->join();
	imuThread->join();
//	dmuThread->join();
//	verticalMotionThread->join();
	headingControllerThread->join();
//	horizontalMotionThread->join();

    /*cleanup*/
	delete socketServer;
	delete socketServerThread;
//	delete horizontalMotionThread;
	delete headingControllerThread;
//	delete verticalMotionThread;
//	delete dmuThread;
	delete imuThread;
	delete targetData;
	delete sensorData;

    syslog(LOG_NOTICE,"[KPI::MAIN] END");
    return EXIT_SUCCESS;
}
