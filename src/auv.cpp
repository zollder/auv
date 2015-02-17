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

//	SensorData* sensorData = new SensorData();
//	DesiredData* targetData = new DesiredData();
//	CameraData* camData = new CameraData();
//	DataService* dataService = new DataService(sensorData, targetData, camData);
//	dataService->getData();



//	SocketServerThread* thread1 = new SocketServerThread(dataService);

//	ImuThread* imuThread = new ImuThread(sensorData);
//	DmuThread* dmuThread = new DmuThread(sensorData);
//	VerticalMotion* verticalMotionThread = new VerticalMotion(sensorData, targetData);
//	HeadingMotion* headingMotionThread = new HeadingMotion(sensorData, targetData);
//	HorizontalMotion* horizontalMotionThread = new HorizontalMotion(sensorData, targetData);
//
//	imuThread->start();
//	dmuThread->start();
//	verticalMotionThread->start();
//	headingMotionThread->start();
//	horizontalMotionThread->start();
//
//	imuThread->join();
//	dmuThread->join();
//	verticalMotionThread->join();
//	headingMotionThread->join();
//	horizontalMotionThread->join();
//
//	delete horizontalMotionThread;
//	delete headingMotionThread;
//	delete verticalMotionThread;
//	delete dmuThread;
//	delete imuThread;
//	delete targetData;
//	delete sensorData;

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
//	pwm.initialize();
//	pwm.getPeriodHz(11);
//	pwm.getDuty(11);
//
//	pwm.setPeriod(1, 500);
//
//	pwm.setPolarity(11, 0);
//	pwm.setPolarity(12, 0);
//
//	pwm.setDuty(11, 0);
//	pwm.setDuty(12, 0);
//
//	pwm.start(11);
//	pwm.start(12);
//	printf("duty1: %d, duty2: %d\n", 0, 0);
//
//	sleep(10);
//
//	pwm.setDuty(11, 15);
//	pwm.setDuty(12, 5);
//	printf("duty1: %d, duty2: %d\n", 15, 5);
//
//	sleep(10);
//
//	pwm.setDuty(11, 30);
//	pwm.setDuty(12, 10);
//	printf("duty1: %d, duty2: %d\n", 30, 10);
//
//	sleep(10);
//
//	pwm.setDuty(11, 0);
//	pwm.setDuty(12, 50);
//	printf("duty1: %d, duty2: %d\n", 0, 50);
//
//	sleep(10);
//
//	pwm.setDuty(11, 100);
//	pwm.setDuty(12, 0);
//	printf("duty1: %d, duty2: %d\n", 100, 0);
//
//	pwm.getDuty(11);
//	pwm.getDuty(12);
//
//	pwm.getPeriod(1);

        //DEBUGImuThread* imuThread = new ImuThread(sensorData);
        //DmuThread* dmuThread = new DmuThread(sensorData);

        //DEBUGimuThread->start();
        //dmuThread->start();

        //delete dmuThread;
        //DEBUGdelete imuThread;

    //server start
    if( argc == 1 || !(strcmp(argv[1], "server") ) )
    {
       // PWM pwm;
       // pwm.initialize();
       // pwm.getPeriodHz(11);
       // pwm.getDuty(11);

		SensorData* sensorData = new SensorData();
		DesiredData* targetData = new DesiredData();
		CameraData* camData = new CameraData();

		DataService* dataService = new DataService(sensorData, targetData, camData);

		ServerThread* serverThread = new ServerThread( 5000, 2 , dataService);

		serverThread->start();

		char key;
		do
		{
			//key = getchar();

		}while(key != ESC);

		syslog(LOG_NOTICE,"[KPI::MAIN]::Escape Character Triggered");


		delete serverThread;
		delete sensorData;
		delete targetData;
		delete camData;
		delete dataService;
    }
    else if (!( strcmp(argv[1], "client")) )
    {
    	ClientThread* clientThread = new ClientThread(5000, "192.168.10.50");
    	clientThread->start();

    	char key;
    	do
    	{
    		//key = getchar();

    	}while(key != ESC);

    	syslog(LOG_NOTICE,"[KPI::MAIN]::Escape Character Triggered");

    	delete clientThread;
    }
    else
    {
    	syslog(LOG_ERR,"[KPI::MAIN] Wrong Options entered, possible ones are : client or server");
    	return EXIT_FAILURE;
    }

    syslog(LOG_NOTICE,"[KPI::MAIN] END");
    return EXIT_SUCCESS;
}
