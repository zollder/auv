//============================================================================
// Name        : auv.cpp
// Author      : zollder
// Copyright   :
// Description :
//============================================================================

#include "imu/ImuThread.h"
#include "controller/VerticalMotion.h"
#include "controller/HeadingMotion.h"
#include "controller/HorizontalMotion.h"
#include "devices/MS5803.h"

int main()
{
//	printf("Main thread started!\n");
//
//	SensorData* sensorData = new SensorData();
//	DesiredData* targetData = new DesiredData();
//
//	ImuThread* imuThread = new ImuThread(sensorData);
//	VerticalMotion* verticalMotionThread = new VerticalMotion(sensorData, targetData);
//	HeadingMotion* headingMotionThread = new HeadingMotion(sensorData, targetData);
//	HorizontalMotion* horizontalMotionThread = new HorizontalMotion(sensorData, targetData);
//
//	imuThread->start();
//	verticalMotionThread->start();
//	headingMotionThread->start();
//	horizontalMotionThread->start();
//
//	imuThread->join();
//	verticalMotionThread->join();
//	headingMotionThread->join();
//	horizontalMotionThread->join();
//
//	delete horizontalMotionThread;
//	delete headingMotionThread;
//	delete verticalMotionThread;
//	delete imuThread;
//	delete targetData;
//	delete sensorData;

	MS5803 sensor;
	sensor.initialize();

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
}
