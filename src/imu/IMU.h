/*
 *	IMU.h
 *	Created on: 16.11.2014
 *	Author: zollder
 */
#include "../commons/Vector.h"
#include "Matrix.h"
#include "Vectors.h"

#include "../devices/L3G.h"
#include "../devices/LSM303.h"

#include <stdio.h>
#include <unistd.h>
#include <math.h>

using namespace std;

#ifndef IMU_h
#define IMU_h

/** LSM303 accelerometer:
 *  - range: ±8g,
 *  - sensitivity: 8g/(2^16/2) = 0.244 mg/digit;
 *  - gravity: 1g = (2^16)/2/8g = 4096
 *  gravity value is equivalent to 1G of accelerometer raw data */
#define GRAVITY 4096

/** L3GD20H gyroscope:
 * - range: 2000 dps full scale
 * - sensivity: 2000/(2^16/2) = 61 mdps/digit (approx 70mdps/digit)
 * - gain: 1dps = 0.070 */
#define GYRO_GAIN 0.07
#define GYRO_GAIN_RAD 0.00122173

#define ToRad(x) ((x)*0.01745329252)  // *pi/180
#define ToDeg(x) ((x)*57.2957795131)  // *180/pi

/** Returns scaled ADC raw gyro data in rad/s */
#define Gyro_Scaled_X(x) ((x)*GYRO_GAIN_RAD)
#define Gyro_Scaled_Y(x) ((x)*GYRO_GAIN_RAD)
#define Gyro_Scaled_Z(x) ((x)*GYRO_GAIN_RAD)

/** Magnetometer calibration constants. */
#define M_X_MIN -1587
#define M_Y_MIN -1596
#define M_Z_MIN -1677
#define M_X_MAX 2295
#define M_Y_MAX 1972
#define M_Z_MAX 1918

/** Integrator and proportionality constants */
#define Kp_ROLLPITCH 0.02
#define Ki_ROLLPITCH 0.00002
#define Kp_YAW 1.2
#define Ki_YAW 0.00002

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define abs(x) ((x)>0?(x):-(x))

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

class IMU
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		/** Constructor. */
		IMU();

		/** Destructor. */
		~IMU();

		//-----------------------------------------------------------------------------------------
		// Methods
		//-----------------------------------------------------------------------------------------
		void initialize(void);
		void calibrateMagnetometer(void);
		void calculateOffset(void);

		void execute(int mode);

		void readGyroscope(void);
		void readAccelerometer(void);
		void readMagnetometer(void);
		void readSensors(void)
		{
			readGyroscope();
			readAccelerometer();
			readMagnetometer();
		};

		void calculateHeading(void);
		void printData(int mode);

		void normalize(void);
		void correctDrift(void);
		void updateMatrix(void);
		void calculateEulerAngles(void);

		//-----------------------------------------------------------------------------------------
		// Instance variables
		//-----------------------------------------------------------------------------------------

		/** Sensor data holders. */
		Vector<int> gyroData = {0,0,0};;
		Vector<int> accData = {0,0,0};;
		Vector<int> magData = {0,0,0};;
		Vector<float> corrMagData = {0,0,0};;

		float magHeading = 0;

		/**
		 * Correct XYZ directions of gyro, accelerometer and magnetometer.
		 * X-axis (forward), Y-axis (right), Z-axis (down).
		 * pitch+ (nose up), roll+ (right wing down), yaw+ (clockwise)
		 */
		int SENSOR_SIGN[9] = {1,1,1,1,1,1,1,1,1};

		/** Gyro & accelerometer raw data and offset holders. */
		int rawData[6] = {0,0,0,0,0,0};
		int offset[6] = {0,0,0,0,0,0};

		/** Gyro (turn rate) and accelerometer (acceleration) vector data holders. */
		float accelVector[3] = {0,0,0};
		float gyroVector[3] = {0,0,0};

		/** DCM algorithm-related data holders. */
		float correctedGyroVector[3] = {0,0,0};	//Corrected Gyro_Vector data
		float omegaProp[3] = {0,0,0};	// Proportional correction
		float omegaInteg[3] = {0,0,0};	// Integrator
		float Omega[3] = {0,0,0};

		/** Integration time constant (DCM algorithm). */
		float G_Dt = 0.02;

		/** Euler angles. */
		float roll = 0;
		float pitch = 0;
		float yaw = 0;

		float errorRollPitch[3] = {0,0,0};
		float errorYaw[3] = {0,0,0};

		float dcm[3][3] = { {1,0,0},{0,1,0},{0,0,1} };		/** estimated DCM */
		float updMatrix[3][3] = { {0,1,2},{3,4,5},{6,7,8} }; //Gyros here
		float tempMatrix[3][3] = { {0,0,0},{0,0,0},{0,0,0} };

		unsigned int counter = 0;

	private:

		long convert_to_dec(float x);

		/** Device objects. */
		L3G* gyro;
		LSM303* compass;
};

#endif
