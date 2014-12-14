//============================================================================
// Name        : auv.cpp
// Author      : zollder
// Copyright   :
// Description :
//============================================================================

#include "imu/ImuThread.h"

int main()
{
	printf("Main thread started!\n");

	Mutex mutex;
	SensorData* sensorData = new SensorData();

	ImuThread* thread1 = new ImuThread(mutex, sensorData);
	thread1->start();
	thread1->join();

	delete thread1;
	delete sensorData;


//	IMU imu;
//	imu.execute(1);

//	PWM pwm;
//	pwm.initialize();
//	pwm.getPeriodHz(11);
//	pwm.getDuty(11);
//
//	pwm.setPeriod(1, 2);
//	pwm.setDuty(11, 50);
//	pwm.getDuty(11);
//	pwm.start(11);
//	pwm.getPeriod(1);
}
