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
	pwm.initialize(1);
	pwm.getPeriod(1);
	pwm.setPeriod(1, 1);
	pwm.getPeriod(1);
}
