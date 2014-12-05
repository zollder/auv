//============================================================================
// Name        : auv.cpp
// Author      : zollder
// Copyright   :
// Description :
//============================================================================

#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include "imu/IMU.h"
#include "sys/PWM.h"

using namespace std;

int main()
{
//	IMU imu;
//	imu.execute(1);

	PWM pwm;
	pwm.initialize();
	pwm.getPeriodHz(11);
	pwm.getDuty(11);

	pwm.setPeriod(1, 2);
	pwm.setDuty(11, 50);
	pwm.getDuty(11);
	pwm.start(11);
//	pwm.getPeriod(1);
}
