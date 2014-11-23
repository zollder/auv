/*
 * IMU.cpp
 *
 *  Created on: 03.10.2013
 *      Author: tuuzdu
 */

#include "../devices/L3G.h"
#include "../devices/LSM303.h"
#include "IMU.h"

L3G gyro;
LSM303 compass;

void IMU_Init() {
	gyro.enable();
	compass.enable();
}

void Read_Gyro() {
	gyro.readGyroscopeData();

	AN[0] = gyro.gyroRawData.x;
	AN[1] = gyro.gyroRawData.y;
	AN[2] = gyro.gyroRawData.z;
	gyro_x = SENSOR_SIGN[0] * (AN[0] - AN_OFFSET[0]);
	gyro_y = SENSOR_SIGN[1] * (AN[1] - AN_OFFSET[1]);
	gyro_z = SENSOR_SIGN[2] * (AN[2] - AN_OFFSET[2]);
}

// Reads x,y and z accelerometer registers
void Read_Accel() {
	compass.readAccelerometerData();

	AN[3] = compass.accelRawData.x;
	AN[4] = compass.accelRawData.y;
	AN[5] = compass.accelRawData.z;
	accel_x = SENSOR_SIGN[3] * (AN[3] - AN_OFFSET[3]);
	accel_y = SENSOR_SIGN[4] * (AN[4] - AN_OFFSET[4]);
	accel_z = SENSOR_SIGN[5] * (AN[5] - AN_OFFSET[5]);
}

void Read_Compass() {
	compass.readMagnetometerData();
	magnetom_x = SENSOR_SIGN[6] * compass.magnetRawData.x;
	magnetom_y = SENSOR_SIGN[7] * compass.magnetRawData.y;
	magnetom_z = SENSOR_SIGN[8] * compass.magnetRawData.z;
}

