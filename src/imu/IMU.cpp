/*
 *	IMU.cpp
 *	Created on: 16.11.2014
 *	Author: zollder
 */
#include "IMU.h"

	/**-----------------------------------------------------------------------------------------
	 * Constructor
	 * -----------------------------------------------------------------------------------------*/
	IMU::IMU()
	{
		printf("Constructing IMU ...\n");

		// instantiate devices
		gyro = new L3G();
		compass = new LSM303();
	}

	/**-----------------------------------------------------------------------------------------
	 * Destructor
	 * -----------------------------------------------------------------------------------------*/
	IMU::~IMU()
	{
		printf("Destroying IMU ...\n");

//		delete corrMagData;
//		delete magData;
//		delete accelData;
//		delete gyroData;
		delete compass;
		delete gyro;
	}

	//-----------------------------------------------------------------------------------------
	/** Enables and configures gyro and compass devices. */
	//-----------------------------------------------------------------------------------------
	void IMU::initialize()
	{
		gyro->enable();
		compass->enable();

		usleep (20000);
		calculateOffset();
	}

	//-----------------------------------------------------------------------------------------
	/** Finds and prints the boundary (max & min) magnetometer readings for all axes. */
	//-----------------------------------------------------------------------------------------
	void IMU::calibrateMagnetometer()
	{
		Vector<int> runningMin = {2047, 2047, 2047};
		Vector<int> runningMax = {-2048, -2048, -2048};

		printf ("Rotate the sensor in all directions to get the boundary magnetometer readings.");

		for(int i=0; i<500; i++)
		{
			readMagnetometer();

			runningMin.x = min(runningMin.x, magData.x);
			runningMin.y = min(runningMin.y, magData.y);
			runningMin.z = min(runningMin.z, magData.z);

			runningMax.x = max(runningMax.x, magData.x);
			runningMax.y = max(runningMax.y, magData.y);
			runningMax.z = max(runningMax.z, magData.z);

			printf ("minX:%d   minY:%d   minZ:%d   maxX:%d   maxY:%d   maxZ:%d\n",
				runningMin.x, runningMin.y, runningMin.z, runningMax.x, runningMax.y, runningMax.z);

			usleep (100000);
		}
	}

	//-----------------------------------------------------------------------------------------
	/** Calculates sensor offsets base on accumulated sensor readings.
	 *  Saves calculated data in the sensor offset data holder and prints the results. */
	//-----------------------------------------------------------------------------------------
	void IMU::calculateOffset()
	{
		// accumulate sensor readings
		for(int i=0; i<32; i++)
		{
			readGyroscope();
			readAccelerometer();
			for(int i=0; i<6; i++)
				offset[i] += rawData[i];

			usleep(20000);
		}

		// populate offset data holder
		for(int i=0; i<6; i++)
			offset[i] = offset[i]/32;

		// gravity compensation
		offset[5]= offset[5] - GRAVITY*SENSOR_SIGN[5];

		printf("offset: %d, %d, %d, %d, %d, %d\n",
				offset[0], offset[1], offset[2], offset[3], offset[4], offset[5]);
	}

	//-----------------------------------------------------------------------------------------
	/** Copies raw readings from the gyroscope object and saves them locally.
	 *  Initializes gyroscope vector with updated raw values by applying desired
	 *  sensor sign and previously calculated offset to raw data. */
	//-----------------------------------------------------------------------------------------
	void IMU::readGyroscope()
	{
		gyro->readGyroscopeData();

		rawData[0] = gyro->gyroRawData.x;
		rawData[1] = gyro->gyroRawData.y;
		rawData[2] = gyro->gyroRawData.z;

		gyroData.x = SENSOR_SIGN[0] * (rawData[0] - offset[0]);
		gyroData.y = SENSOR_SIGN[1] * (rawData[1] - offset[1]);
		gyroData.z = SENSOR_SIGN[2] * (rawData[2] - offset[2]);

		/* printf("%d  %d  %d     \n", gyroData->x, gyroData->y, gyroData->z); */
	}

	//-----------------------------------------------------------------------------------------
	/** Copies raw accelerometer readings from the compass object and saves them locally.
	 *  Initializes accelerometer vector with updated raw values by applying desired
	 *  sensor sign and previously calculated offset to raw data. */
	//-----------------------------------------------------------------------------------------
	void IMU::readAccelerometer()
	{
		compass->readAccelerometerData();

		rawData[3] = compass->accelRawData.x;
		rawData[4] = compass->accelRawData.y;
		rawData[5] = compass->accelRawData.z;

		accData.x = SENSOR_SIGN[3] * (rawData[3] - offset[3]);
		accData.y = SENSOR_SIGN[4] * (rawData[4] - offset[4]);
		accData.z = SENSOR_SIGN[5] * (rawData[5] - offset[5]);

		/*printf("%d  %d  %d     \n", accelData->x, accelData->y, accelData->z);*/
	}

	//-----------------------------------------------------------------------------------------
	/** Copies raw magnetometer readings from the compass object and saves them locally.
	 *  Initializes magnetometer vector with updated raw values by applying desired sensor sign. */
	//-----------------------------------------------------------------------------------------
	void IMU::readMagnetometer()
	{
		compass->readMagnetometerData();

		magData.x = SENSOR_SIGN[6] * compass->magnetRawData.x;
		magData.y = SENSOR_SIGN[7] * compass->magnetRawData.y;
		magData.z = SENSOR_SIGN[8] * compass->magnetRawData.z;

		/*printf("raw: %d  %d  %d     \n", magData->x, magData->y, magData->z);*/
	}

	//-----------------------------------------------------------------------------------------
	/** Calculates tilt-compensated magnetic heading. */
	//-----------------------------------------------------------------------------------------
	void IMU::calculateHeading()
	{
		float magX, magY;
		float cos_roll, sin_roll, cos_pitch, sin_pitch;

		cos_roll = cos(roll);
		sin_roll = sin(roll);
		cos_pitch = cos(pitch);
		sin_pitch = sin(pitch);

		// adjust for LSM303 compass axis offsets/sensitivity differences by scaling to +/-0.5 range
		corrMagData.x = (float) (magData.x - SENSOR_SIGN[6] * M_X_MIN) / (M_X_MAX - M_X_MIN) - SENSOR_SIGN[6] * 0.5;
		corrMagData.y = (float) (magData.y - SENSOR_SIGN[7] * M_Y_MIN) / (M_Y_MAX - M_Y_MIN) - SENSOR_SIGN[7] * 0.5;
		corrMagData.z = (float) (magData.z - SENSOR_SIGN[8] * M_Z_MIN) / (M_Z_MAX - M_Z_MIN) - SENSOR_SIGN[8] * 0.5;

		/*printf ("\033[1Aroll: %.2f \tpitch: %.2f  \tyaw: %.2f\n", c_magnetom_x, c_magnetom_y, c_magnetom_z);*/

		// Tilt compensated Magnetic filed X:
		magX = corrMagData.x * cos_pitch + corrMagData.y * sin_roll * sin_pitch + corrMagData.z * cos_roll * sin_pitch;

		// Tilt compensated Magnetic filed Y:
		magY = corrMagData.y * cos_roll - corrMagData.z * sin_roll;

		// Magnetic Heading
		magHeading = atan2(-magY, magX);

		/*printf ("\033[1Aroll: %.2f\n", MAG_Heading);*/
	}

	void IMU::normalize()
	{
		float error = 0;
		float temporary[3][3];
		float renorm = 0;

		error = -Vector_Dot_Product(&DCM_Matrix[0][0], &DCM_Matrix[1][0]) * .5; //eq.19

		Vector_Scale(&temporary[0][0], &DCM_Matrix[1][0], error); //eq.19
		Vector_Scale(&temporary[1][0], &DCM_Matrix[0][0], error); //eq.19

		Vector_Add(&temporary[0][0], &temporary[0][0], &DCM_Matrix[0][0]); //eq.19
		Vector_Add(&temporary[1][0], &temporary[1][0], &DCM_Matrix[1][0]); //eq.19

		Vector_Cross_Product(&temporary[2][0], &temporary[0][0], &temporary[1][0]); // c= a x b //eq.20

		renorm = .5 * (3 - Vector_Dot_Product(&temporary[0][0], &temporary[0][0])); //eq.21
		Vector_Scale(&DCM_Matrix[0][0], &temporary[0][0], renorm);

		renorm = .5 * (3 - Vector_Dot_Product(&temporary[1][0], &temporary[1][0])); //eq.21
		Vector_Scale(&DCM_Matrix[1][0], &temporary[1][0], renorm);

		renorm = .5 * (3 - Vector_Dot_Product(&temporary[2][0], &temporary[2][0])); //eq.21
		Vector_Scale(&DCM_Matrix[2][0], &temporary[2][0], renorm);
	}

	void IMU::correctDrift(void)
	{
		float mag_heading_x;
		float mag_heading_y;
		float errorCourse;
		//Compensation the Roll, Pitch and Yaw drift.
		static float Scaled_Omega_P[3];
		static float Scaled_Omega_I[3];
		float Accel_magnitude;
		float Accel_weight;

		//*****Roll and Pitch***************

		// Calculate the magnitude of the accelerometer vector
		Accel_magnitude = sqrt(Accel_Vector[0] * Accel_Vector[0] + Accel_Vector[1] * Accel_Vector[1] + Accel_Vector[2] * Accel_Vector[2]);
		Accel_magnitude = Accel_magnitude / GRAVITY; // Scale to gravity.
		// Dynamic weighting of accelerometer info (reliability filter)
		// Weight for accelerometer info (<0.5G = 0.0, 1G = 1.0 , >1.5G = 0.0)
		Accel_weight = constrain(1 - 2*abs(1 - Accel_magnitude),0,1); //

		Vector_Cross_Product(&errorRollPitch[0], &Accel_Vector[0], &DCM_Matrix[2][0]); //adjust the ground of reference
		Vector_Scale(&Omega_P[0], &errorRollPitch[0], Kp_ROLLPITCH * Accel_weight);

		Vector_Scale(&Scaled_Omega_I[0], &errorRollPitch[0], Ki_ROLLPITCH * Accel_weight);
		Vector_Add(Omega_I, Omega_I, Scaled_Omega_I);

		//*****YAW***************
		// We make the gyro YAW drift correction based on compass magnetic heading

		mag_heading_x = cos(magHeading);
		mag_heading_y = sin(magHeading);
		errorCourse = (DCM_Matrix[0][0] * mag_heading_y) - (DCM_Matrix[1][0] * mag_heading_x); //Calculating YAW error
		Vector_Scale(errorYaw, &DCM_Matrix[2][0], errorCourse); //Applys the yaw correction to the XYZ rotation of the aircraft, depeding the position.

		Vector_Scale(&Scaled_Omega_P[0], &errorYaw[0], Kp_YAW); //.01proportional of YAW.
		Vector_Add(Omega_P, Omega_P, Scaled_Omega_P); //Adding  Proportional.

		Vector_Scale(&Scaled_Omega_I[0], &errorYaw[0], Ki_YAW); //.00001Integrator
		Vector_Add(Omega_I, Omega_I, Scaled_Omega_I); //adding integrator to the Omega_I
	}

	void IMU::updateMatrix(void)
	{
		Gyro_Vector[0] = Gyro_Scaled_X(gyroData.x); //gyro x roll
		Gyro_Vector[1] = Gyro_Scaled_Y(gyroData.y); //gyro y pitch
		Gyro_Vector[2] = Gyro_Scaled_Z(gyroData.z); //gyro Z yaw

		Accel_Vector[0] = accData.x;
		Accel_Vector[1] = accData.y;
		Accel_Vector[2] = accData.z;

		Vector_Add(&Omega[0], &Gyro_Vector[0], &Omega_I[0]); //adding proportional term
		Vector_Add(&Omega_Vector[0], &Omega[0], &Omega_P[0]); //adding Integrator term

		Update_Matrix[0][0] = 0;
		Update_Matrix[0][1] = -G_Dt * Omega_Vector[2]; //-z
		Update_Matrix[0][2] = G_Dt * Omega_Vector[1]; //y
		Update_Matrix[1][0] = G_Dt * Omega_Vector[2]; //z
		Update_Matrix[1][1] = 0;
		Update_Matrix[1][2] = -G_Dt * Omega_Vector[0]; //-x
		Update_Matrix[2][0] = -G_Dt * Omega_Vector[1]; //-y
		Update_Matrix[2][1] = G_Dt * Omega_Vector[0]; //x
		Update_Matrix[2][2] = 0;


		Matrix_Multiply(DCM_Matrix, Update_Matrix, Temporary_Matrix); //a*b=c

		for (int x = 0; x < 3; x++) //Matrix Addition (update)
			for (int y = 0; y < 3; y++)
				DCM_Matrix[x][y] += Temporary_Matrix[x][y];
	}

	void IMU::calculateEulerAngles(void)
	{
		pitch = -asin(DCM_Matrix[2][0]);
		roll = atan2(DCM_Matrix[2][1], DCM_Matrix[2][2]);
		yaw = atan2(DCM_Matrix[1][0], DCM_Matrix[0][0]);
	}

	void IMU::printData(int mode)
	{
		if (mode == 1)
			printf("Angle:   %.2f    %.2f    %.2f \n", ToDeg(roll), ToDeg(pitch), ToDeg(yaw));
		else if (mode == 2)
		{
			printf("%d  %d  %d  |  %d  %d  %d  |  %d  %d  %d  \n",
					gyroData.x, gyroData.y, gyroData.z,
					accData.x, accData.y, accData.z,
					magData.x, magData.y, magData.z);
		}
		else if (mode == 3)
		{
			printf("%ld  %ld  %ld  |  %ld  %ld  %ld  |  %ld  %ld  %ld  \n",
					convert_to_dec(DCM_Matrix[0][0]),
					convert_to_dec(DCM_Matrix[0][1]),
					convert_to_dec(DCM_Matrix[0][2]),
					convert_to_dec(DCM_Matrix[1][0]),
					convert_to_dec(DCM_Matrix[1][1]),
					convert_to_dec(DCM_Matrix[1][1]),
					convert_to_dec(DCM_Matrix[2][0]),
					convert_to_dec(DCM_Matrix[2][1]),
					convert_to_dec(DCM_Matrix[2][2]));
		}
		else
		{
			printf("Unknown mode");
			exit(1);
		}
	}

	long IMU::convert_to_dec(float x)
	{
	  return x*10000000;
	}
