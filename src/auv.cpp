//============================================================================
// Name        : auv.cpp
// Author      : zollder
// Copyright   :
// Description :
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

#include "imu/IMU.h"
#include "sys/PWM.h"
#include "sys/FdTimer.h"

using namespace std;

int thread_1_count;
int thread_2_count = 0;

void *thread_1 (void *arg)
{
	thread_1_count = 0;
	FdTimer* timer1 = new FdTimer(1, 2.0);

	printf ("Thread 1 period 2ms\n");
	timer1->start();
	while (1)
	{
		timer1->waitTimerEvent();
		thread_1_count++;
		printf ("Thread 1:%d\n",thread_1_count);
	}
	delete timer1;
	return NULL;
}

void *thread_2 (void *arg)
{
	thread_2_count = 0;
	FdTimer* timer2 = new FdTimer(2, 0.1);

	printf ("Thread 1 period 1s\n");
	timer2->start();
	while (1)
	{
		timer2->waitTimerEvent();
		thread_2_count++;
		printf ("---------------------Thread 2:%d\n",thread_2_count);
	}
	delete timer2;
	return NULL;
}

int main()
{
	pthread_t t_1;
	pthread_t t_2;

	printf ("Periodic threads using timerfd\n");

	pthread_create (&t_1, NULL, thread_1, NULL) ;
	pthread_create (&t_2, NULL, thread_2, NULL) ;

	sleep (100);
	printf ("thread1 %d iterations\n", thread_1_count);
	printf ("thread2 %d iterations\n", thread_2_count);
	return 0 ;


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
