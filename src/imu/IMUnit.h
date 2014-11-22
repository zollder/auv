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

/** maximum accelerometer weight in accelerometer-gyro fusion formula. */
#define ACCEL_WEIGHT 0.04
/** accelerometer sensitivity in g/digit. */
#define ACCEL_SENSIVITY 0.000244

/** LSM303 accelerometer:
 *  - range: ±8g,
 *  - sensitivity: 8g/(2^16/2) = 0.244 mg/digit;
 *  - gravity: 1g = (2^16)/2/8g = 4096
 */
#define GRAVITY 4096  //this equivalent to 1G of accelerometer raw data

/** L3GD20H gyroscope:
 * - range: 2000 dps full scale
 * - sensivity: 2000/(2^16/2) = 61 mdps/digit (approx 70mdps/digit)
 * - gain: 1dps = 0.070 */
#define GYRO_GAIN 0.07
#define GYRO_GAIN_RAD 0.00122173

#define ToRad(x) ((x)*0.01745329252)  // *pi/180
#define ToDeg(x) ((x)*57.2957795131)  // *180/pi

/** Returns scaled ADC raw gyro data in rad/s */
#define Gyro_Scaled_X(x) ((x)*ToRad(GYRO_GAIN))
#define Gyro_Scaled_Y(x) ((x)*ToRad(GYRO_GAIN))
#define Gyro_Scaled_Z(x) ((x)*ToRad(GYRO_GAIN))

/** LSM303 magnetometer calibration constants. */
#define M_X_MIN -1522
#define M_X_MAX 2208
#define M_Y_MIN -1567
#define M_Y_MAX 1947
#define M_Z_MIN -1574
#define M_Z_MAX 1885

#define Kp_ROLLPITCH 0.02
#define Ki_ROLLPITCH 0.00002
#define Kp_YAW 1.2
#define Ki_YAW 0.00002

/** Gyro data mode: CALIBRMODE:0 (with drift), OUTPUTMODE:1 (no drift)*/
#define OUTPUTMODE 1
#define CALIBRMODE 0

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define abs(x) ((x)>0?(x):-(x))

// magnetometer calibration-related definitions
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

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
		void calibrateMagnetometer();
		void printData(int mode);
		long convert_to_dec(float x);

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
		float Omega_Proportional[3] = {0,0,0}; //Omega Proportional correction
		float Omega_Integrator[3] = {0,0,0}; //Omega Integrator
		float Omega[3] = {0,0,0};

		/** Integration time constant (DCM algorithm). */
		float G_Dt;

		float magHeading;

		/** Euler angles. */
		float roll;
		float pitch;
		float yaw;

		float errorRollPitch[3] = {0,0,0};
		float errorYaw[3] = {0,0,0};

		float DCM_Matrix[3][3] = { {1,0,0},{0,1,0},{0,0,1} };		/** estimated DCM */
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
