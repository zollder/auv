//============================================================================
// Name        : auv.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <unistd.h>

#include "IMU/L3G.h"
#include "IMU/LSM303.h"

using namespace std;

int main(int argc, char *argv[])
{
	L3G gyro;
	LSM303 compass;

	cout << "Enabling gyro ..." << endl;
	gyro.enableDefault();

	cout << "Enabling compass ..." << endl;
	compass.enableDefault();
}
