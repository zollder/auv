//============================================================================
// Name        : auv.cpp
// Author      : zollder
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

#define ESC 27

int main(int argc, char *argv[])
{
	syslog(LOG_NOTICE,"[KPI::MAIN] START");

	SensorData* sensorData = new SensorData();
	DesiredData* targetData = new DesiredData();
//	CameraData* camData = new CameraData();
//	DataService* dataService = new DataService(sensorData, targetData, camData);
//	dataService->getData();



//	SocketServerThread* thread1 = new SocketServerThread(dataService);

	ImuThread* imuThread = new ImuThread(sensorData);
//	DmuThread* dmuThread = new DmuThread(sensorData);
//	VerticalMotion* verticalMotionThread = new VerticalMotion(sensorData, targetData);
	HeadingController* headingControllerThread = new HeadingController(sensorData, targetData);
//	HorizontalMotion* horizontalMotionThread = new HorizontalMotion(sensorData, targetData);
//
	imuThread->start();
//	dmuThread->start();
//	verticalMotionThread->start();
	headingControllerThread->start();
//	horizontalMotionThread->start();
//
	imuThread->join();
//	dmuThread->join();
//	verticalMotionThread->join();
	headingControllerThread->join();
//	horizontalMotionThread->join();
//
//	delete horizontalMotionThread;
	delete headingControllerThread;
//	delete verticalMotionThread;
//	delete dmuThread;
	delete imuThread;
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
