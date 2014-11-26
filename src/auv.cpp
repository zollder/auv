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

using namespace std;

int main()
{
	IMU imu;
	imu.execute(1);
}
