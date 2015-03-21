//============================================================================
// Name        : auv.cpp
// Author      : Main Program Execution Capstone AUV
//============================================================================

#include "communication/ServerThread.h"
#include "communication/ClientThread.h"

//#include "imu/ImuThread.h"
//#include "dmu/DmuThread.h"
//#include "controller/AltitudeController.h"
//#include "controller/HeadingController.h"
//#include "controller/HorizontalMotion.h"
#include "controller/MasterController.h"
#include "data/DataService.h"

using namespace std;

int main(int argc, char *argv[])
{
	syslog(LOG_NOTICE,"[KPI::MAIN] START");

	SensorData* sensorData = new SensorData();
	DesiredData* targetData = new DesiredData();
	CameraData* camData = new CameraData();

	DataService* dataService = new DataService(sensorData, targetData, camData);
	SocketServer* socketServer = new SocketServer(dataService, PORT, CLIENTS);
	SocketClient* clientFront = new SocketClient(dataService, RETRY_TIMER_ID, PORT, U3_FRONT_IP);
	SocketClient* clientBottom = new SocketClient(dataService, RETRY_TIMER_ID, PORT, U3_BOTTOM_IP);



	ServerThread* socketServerThread = new ServerThread(socketServer, SERVER_THREAD_ID);
	ClientThread* frontClientThread = new ClientThread(clientFront, CLIENT_U3_FRONT_ID, CLIENT_U3_FRONT_INTERVAL);
	ClientThread* bottomClientThread = new ClientThread(clientBottom, CLIENT_U3_BOTTOM_ID, CLIENT_U3_BOTTOM_INTERVAL);
//	ImuThread* imuThread = new ImuThread(sensorData);
//	DmuThread* dmuThread = new DmuThread(sensorData);

//	AltitudeController* altitudeControllerThread = new AltitudeController(dataService);
//	HeadingController* headingControllerThread = new HeadingController(dataService);
//	HorizontalMotion* horizontalMotionThread = new HorizontalMotion(dataService);

//	MasterController* controller = new MasterController(dataService);
//	controller->start();
//	controller->join();
//	delete controller;

	socketServerThread->start();
	frontClientThread->start();
	bottomClientThread->start();
//	imuThread->start();
//	dmuThread->start();
//	altitudeControllerThread->start();
//	headingControllerThread->start();
//	horizontalMotionThread->start();
//
	socketServerThread->join();
	frontClientThread->join();
	bottomClientThread->join();
//	imuThread->join();
//	dmuThread->join();
//	altitudeControllerThread->join();
//	headingControllerThread->join();
//	horizontalMotionThread->join();
//
//	delete imuThread;
	delete bottomClientThread;
	delete frontClientThread;
	delete socketServerThread;
	delete clientBottom;
//	delete clientFront;
	delete socketServer;
//	delete horizontalMotionThread;
//	delete headingControllerThread;
//	delete altitudeControllerThread;
//	delete dmuThread;
	delete dataService;
	delete camData;
	delete targetData;
	delete sensorData;

//	DMU dmu;
//	dmu.enable();
//	for (int i = 0; i < 20; i++)
//	{
//		dmu.readSensor();
//		dmu.calculateTemperature();
//		dmu.calculatePressure();
//		printf("Actual temperature: %0.2f\n", dmu.temperature);
//		printf("Actual pressure: %0.2f\n", dmu.pressure);
//	}

//	IMU imu;
//	imu.execute(1);

//	PWM pwm;
//	pwm.getPeriodHz(31);
//	pwm.getDuty(31);
//
////	pwm.setPeriod(3, 50);
//
//	pwm.start(31);
//
//	pwm.setDuty(31, 0);
//
//	for (int i = 0; i < 5; i++)
//	{
//		pwm.setDuty(31, i);
//		pwm.setDuty(32, i);
//		sleep(2);
//	}
//
//	for (int i = 0; i > -5; i--)
//	{
//		pwm.setDuty(31, i);
//		pwm.setDuty(32, i);
//		sleep(2);
//	}

//	pwm.setDuty(31, -5);
//	sleep(5);

//	pwm.setDuty(31, 0);
//	pwm.setDuty(32, 0);
//	pwm.stop(31);
//	pwm.stop(32);


        //DEBUGImuThread* imuThread = new ImuThread(sensorData);
        //DmuThread* dmuThread = new DmuThread(sensorData);

        //DEBUGimuThread->start();
        //dmuThread->start();

        //delete dmuThread;
        //DEBUGdelete imuThread;

    //server start
//    if( argc == 1 || !(strcmp(argv[1], "server") ) )
//    {
//       // PWM pwm;
//       // pwm.initialize();
//       // pwm.getPeriodHz(11);
//       // pwm.getDuty(11);
//
//		SensorData* sensorData = new SensorData();
//		DesiredData* targetData = new DesiredData();
//		CameraData* camData = new CameraData();
//
//		DataService* dataService = new DataService(sensorData, targetData, camData);
//
//		ServerThread* serverThread = new ServerThread( 5000, 2 , dataService);
//
//		serverThread->start();
//
//		char key;
//		do
//		{
//			//key = getchar();
//
//		}while(key != ESC);
//
//		syslog(LOG_NOTICE,"[KPI::MAIN]::Escape Character Triggered");
//
//
//		delete serverThread;
//		delete sensorData;
//		delete targetData;
//		delete camData;
//		delete dataService;
//    }
//    else if (!( strcmp(argv[1], "client")) )
//    {
//    	ClientThread* clientThread = new ClientThread(5000, "192.168.10.50");
//    	clientThread->start();
//
//    	char key;
//    	do
//    	{
//    		//key = getchar();
//
//    	}while(key != ESC);
//
//    	syslog(LOG_NOTICE,"[KPI::MAIN]::Escape Character Triggered");
//
//    	delete clientThread;
//    }
//    else
//    {
//    	syslog(LOG_ERR,"[KPI::MAIN] Wrong Options entered, possible ones are : client or server");
//    	return EXIT_FAILURE;
//    }

    syslog(LOG_NOTICE,"[KPI::MAIN] END");
    return EXIT_SUCCESS;
}
