/*
 *	IMUnit.h
 *	Created on: 16.11.2014
 *	Author: zollder
 */
#include "Vector.h"
#include "Matrix.h"
#include "SensorData.h"

#include "../devices/L3G.h"
#include "../devices/LSM303.h"

#include <stdio.h>
#include <unistd.h>

using namespace std;

#ifndef imunit_h
#define imunit_h

/** LSM303 accelerometer: 8g sensitivity, 3.8 mg/digit; 1g = (2^16)/2/8g = 4096 */
#define GRAVITY 4096  //this equivalent to 1G of accelerometer raw data

#define ToRad(x) ((x)*0.01745329252)  // *pi/180
#define ToDeg(x) ((x)*57.2957795131)  // *180/pi

/** L3G4200D gyro: 2000 dps full scale, 70 mdps/digit, 1dps = 0.07 */
#define Gyro_Gain_X 0.07 //X axis Gyro gain
#define Gyro_Gain_Y 0.07 //Y axis Gyro gain
#define Gyro_Gain_Z 0.07 //Z axis Gyro gain

/** Returns scaled ADC raw gyro data in rad/s */
#define Gyro_Scaled_X(x) ((x)*ToRad(Gyro_Gain_X))
#define Gyro_Scaled_Y(x) ((x)*ToRad(Gyro_Gain_Y))
#define Gyro_Scaled_Z(x) ((x)*ToRad(Gyro_Gain_Z))

/** LSM303 magnetometer calibration constants. */
#define M_X_MIN -3030
#define M_X_MAX 4630
#define M_Y_MIN -3160
#define M_Y_MAX 4000
#define M_Z_MIN -3200
#define M_Z_MAX 3830

#define Kp_ROLLPITCH 0.02
#define Ki_ROLLPITCH 0.00002
#define Kp_YAW 1.2
#define Ki_YAW 0.00002

/** Gyro data mode: CALIBRMODE:0 (with drift), OUTPUTMODE:1 (no drift)*/
#define OUTPUTMODE 1
#define CALIBRMODE 0

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define abs(x) ((x)>0?(x):-(x))

class IMUnit
{
	public:

		/** Constructor. */
		IMUnit();

		/** Destructor. */
		~IMUnit();

	    template <typename T> struct vector
	    { T x, y, z; };

		void initialize(void);
		void setCalibrationMode(bool);
		void readGyroscope(void);
		void readAccelerometer(void);
		void readMagnetometer(void);
		void findMagnetometerHeading(void);
		float getHeading();
		void normalize(void);
		void correctDrift(void);
		void updateMatrix(void);
		void calculateEulerAngles(void);

	    // vector functions
	    template <typename Ta, typename Tb, typename To> static void findCrossProduct(const vector<Ta> *a, const vector<Tb> *b, vector<To> *out);
	    template <typename Ta, typename Tb> static float findDotProduct(const vector<Ta> *a,const vector<Tb> *b);
	    static void normalizeData(vector<float> *a);

		/** Sensor data holders. */
		SensorData<int>* gyroData;
		SensorData<int>* accelData;
		SensorData<int>* magData;
		SensorData<float>* corrMagData;

		/**
		 * Correct XYZ directions of gyro, accelerometer and magnetometer.
		 * X-axis (forward), Y-axis (right), Z-axis (down).
		 * pitch+ (nose up), roll+ (right wing down), yaw+ (clockwise)
		 */
		int sensorSign[9] = {1,1,1,-1,-1,-1,1,1,1};

		/** Gyro & accelerometer data and offset holders. */
		int sensorData[6] = {0,0,0,0,0,0};
		int sensorOffset[6] = {0,0,0,0,0,0};

		/** Gyro (turn rate) and accelerometer (acceleration) vector data holders. */
		float accelVector[3] = {0,0,0};
		float gyroVector[3] = {0,0,0};

		/** DCM algorithm-related data holders. */
		float Omega_Vector[3] = {0,0,0}; //Corrected Gyro_Vector data
		float Omega_P[3] = {0,0,0}; //Omega Proportional correction
		float Omega_I[3] = {0,0,0}; //Omega Integrator
		float Omega[3] = {0,0,0};

		/** Integration time constant (DCM algorithm). */
		float G_Dt;

		/** Euler angles. */
		float roll;
		float pitch;
		float yaw;
		float magHeading;

		float errorRollPitch[3] = {0,0,0};
		float errorYaw[3] = {0,0,0};

		float DCM_Matrix[3][3] = { {1,0,0},{0,1,0},{0,0,1} };
		float Update_Matrix[3][3] = { {0,1,2},{3,4,5},{6,7,8} }; //Gyros here
		float Temporary_Matrix[3][3] = { {0,0,0},{0,0,0},{0,0,0} };


		unsigned int counter;

	private:

		/** Device objects. */
		L3G* gyro;
		LSM303* compass;

		bool calibrationMode = false;
};

#endif
