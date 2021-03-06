/*
 *	IMU.cpp
 *	Created on: 16.11.2014
 *	Author: zollder
 */

#include "IMU.h"

	/**-----------------------------------------------------------------------------------------
	 * Constructor
	 * -----------------------------------------------------------------------------------------*/
	IMU::IMU(float interval)
	{
		printf("Constructing IMU ...\n");

		// instantiate devices
		gyro = new L3G();
		compass = new LSM303();
		G_Dt = interval;
	}

	/**-----------------------------------------------------------------------------------------
	 * Destructor
	 * -----------------------------------------------------------------------------------------*/
	IMU::~IMU()
	{
		printf("Destroying IMU ...\n");

		delete compass;
		delete gyro;
	}

	//-----------------------------------------------------------------------------------------
	/** Initializes sensors, reads, interprets and prints data in specified format. */
	//-----------------------------------------------------------------------------------------
	void IMU::execute(int mode)
	{
		this->initialize();

		unsigned long timer = millis();
		unsigned long oldTimer;
		unsigned int printCounter = 0;

		while (1)
		{
			if((millis()-timer) >= 20)  // Main loop runs at 50Hz
			{
				counter++;
				oldTimer = timer;
				timer = millis();
				if (timer > oldTimer)
				{
					G_Dt = (timer-oldTimer)/1000.0;
					printf("G_Dt: %f\n", G_Dt);
				}
				else
					G_Dt = 0;
			}

			this->readGyroscope();
			this->readAccelerometer();

			if (counter > 5)
			{
				counter = 0;
				this->readMagnetometer();
				this->calculateHeading();
			}

			this->updateMatrix();
			this->normalize();
			this->correctDrift();
			this->calculateEulerAngles();

			printCounter++;
			if (printCounter > 5)
			{
				printCounter = 0;
				this->printData(mode);
			}
		}

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

		//DEBUG
		//printf("offset: %d, %d, %d, %d, %d, %d\n", offset[0], offset[1], offset[2], offset[3], offset[4], offset[5]);
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

		// Tilt compensated Magnetic filed X:
		magX = corrMagData.x * cos_pitch + corrMagData.y * sin_roll * sin_pitch + corrMagData.z * cos_roll * sin_pitch;

		// Tilt compensated Magnetic filed Y:
		magY = corrMagData.y * cos_roll - corrMagData.z * sin_roll;

		// Magnetic Heading
		magHeading = atan2(-magY, magX);
	}

	//-----------------------------------------------------------------------------------------
	/** Normalizes DCM vectors. */
	//-----------------------------------------------------------------------------------------
	void IMU::normalize()
	{
		float error = 0;
		float temp[3][3];
		float renorm = 0;

		error = -utils.calculateDotProduct(&dcm[0][0], &dcm[1][0]) * .5; //eq.19

		utils.scaleVector(&temp[0][0], &dcm[1][0], error); //eq.19
		utils.scaleVector(&temp[1][0], &dcm[0][0], error); //eq.19

		utils.addVector(&temp[0][0], &temp[0][0], &dcm[0][0]); //eq.19
		utils.addVector(&temp[1][0], &temp[1][0], &dcm[1][0]); //eq.19

		utils.calculateCrossProduct(&temp[2][0], &temp[0][0], &temp[1][0]); // c= a x b //eq.20

		renorm = .5 * (3 - utils.calculateDotProduct(&temp[0][0], &temp[0][0])); //eq.21
		utils.scaleVector(&dcm[0][0], &temp[0][0], renorm);

		renorm = .5 * (3 - utils.calculateDotProduct(&temp[1][0], &temp[1][0])); //eq.21
		utils.scaleVector(&dcm[1][0], &temp[1][0], renorm);

		renorm = .5 * (3 - utils.calculateDotProduct(&temp[2][0], &temp[2][0])); //eq.21
		utils.scaleVector(&dcm[2][0], &temp[2][0], renorm);
	}

	//-----------------------------------------------------------------------------------------
	/** Corrects gyro's roll and pitch based on accelerometer readings.
	 *  Corrects gyro's yaw based on calculated magnetometer heading. */
	//-----------------------------------------------------------------------------------------
	void IMU::correctDrift(void)
	{
		float magHeadingX, magHeadingY, errorCourse;

		// Roll, Pitch and Yaw drift compensation variables.
		static float scaledOmegaP[3], scaledOmegaI[3];
		float accelMagnitude, accelWeight;

		// 1. roll & pitch correction

		// Calculate accelerometer vector magnitude
		accelMagnitude = sqrt(accelVector[0]*accelVector[0] + accelVector[1]*accelVector[1] + accelVector[2]*accelVector[2]);
		accelMagnitude = accelMagnitude / GRAVITY; // Scale to gravity.

		// Dynamic accelerometer weighting (reliability filter)
		// Weight for accelerometer info (<0.5G = 0.0, 1G = 1.0 , >1.5G = 0.0)
		accelWeight = constrain(1 - 2*abs(1 - accelMagnitude), 0, 1); //

		// adjust reference ground
		utils.calculateCrossProduct(&errorRollPitch[0], &accelVector[0], &dcm[2][0]);
		utils.scaleVector(&omegaProp[0], &errorRollPitch[0], Kp_ROLLPITCH * accelWeight);

		utils.scaleVector(&scaledOmegaI[0], &errorRollPitch[0], Ki_ROLLPITCH * accelWeight);
		utils.addVector(omegaInteg, omegaInteg, scaledOmegaI);

		// 2. yaw drift correction based on compass heading

		magHeadingX = cos(magHeading);
		magHeadingY = sin(magHeading);

		// calculate yaw error
		errorCourse = (dcm[0][0] * magHeadingY) - (dcm[1][0] * magHeadingX);

		// apply yaw correction to XYZ rotation, depending on the device position.
		utils.scaleVector(errorYaw, &dcm[2][0], errorCourse);

		utils.scaleVector(&scaledOmegaP[0], &errorYaw[0], Kp_YAW);	// .01 proportional of YAW.
		utils.addVector(omegaProp, omegaProp, scaledOmegaP);			// adding  proportional term.

		utils.scaleVector(&scaledOmegaI[0], &errorYaw[0], Ki_YAW);	// .00001Integrator
		utils.addVector(omegaInteg, omegaInteg, scaledOmegaI);		// adding integrator to the Omega_I
	}

	//-----------------------------------------------------------------------------------------
	/** Updates DCM using the latest sensor readings (dt). */
	//-----------------------------------------------------------------------------------------
	void IMU::updateMatrix(void)
	{
		gyroVector[0] = Gyro_Scaled_X(gyroData.x); //gyro x roll
		gyroVector[1] = Gyro_Scaled_Y(gyroData.y); //gyro y pitch
		gyroVector[2] = Gyro_Scaled_Z(gyroData.z); //gyro Z yaw

		accelVector[0] = accData.x;
		accelVector[1] = accData.y;
		accelVector[2] = accData.z;

		// adding proportional and integrator terms
		utils.addVector(&Omega[0], &gyroVector[0], &omegaInteg[0]);
		utils.addVector(&correctedGyroVector[0], &Omega[0], &omegaProp[0]);

		updMatrix[0][0] = 0;
		updMatrix[0][1] = -G_Dt * correctedGyroVector[2]; //-z
		updMatrix[0][2] = G_Dt * correctedGyroVector[1]; //y
		updMatrix[1][0] = G_Dt * correctedGyroVector[2]; //z
		updMatrix[1][1] = 0;
		updMatrix[1][2] = -G_Dt * correctedGyroVector[0]; //-x
		updMatrix[2][0] = -G_Dt * correctedGyroVector[1]; //-y
		updMatrix[2][1] = G_Dt * correctedGyroVector[0]; //x
		updMatrix[2][2] = 0;

		// a*b=c
		utils.multiplyMatrices(dcm, updMatrix, tempMatrix);

		// add matrices (update)
		for (int x = 0; x < 3; x++)
			for (int y = 0; y < 3; y++)
				dcm[x][y] += tempMatrix[x][y];
	}

	//-----------------------------------------------------------------------------------------
	/** Calculates Euler angles from DCM. */
	//-----------------------------------------------------------------------------------------
	void IMU::calculateEulerAngles(void)
	{
		pitch = -asin(dcm[2][0]);
		roll = atan2(dcm[2][1], dcm[2][2]);
		yaw = -atan2(dcm[1][0], dcm[0][0]);
	}

	//-----------------------------------------------------------------------------------------
	/** Converts Euler angles to degrees and stores data in the imuDataDegrees data holder. */
	//-----------------------------------------------------------------------------------------
	void IMU::convertToDegrees(void)
	{
		imuDataDegrees.x = ToDeg(roll);
		imuDataDegrees.y = ToDeg(pitch);
		imuDataDegrees.z = ToDeg(yaw);
	}

	//-----------------------------------------------------------------------------------------
	/** Prints interpreted sensor data according to the specified mode. */
	//-----------------------------------------------------------------------------------------
	void IMU::printData(int mode)
	{
		if (mode == 1)
			printf("Angle:   %10.2f    %10.2f    %10.2f \n", ToDeg(roll), ToDeg(pitch), ToDeg(yaw));
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
					utils.convert_to_dec(dcm[0][0]),
					utils.convert_to_dec(dcm[0][1]),
					utils.convert_to_dec(dcm[0][2]),
					utils.convert_to_dec(dcm[1][0]),
					utils.convert_to_dec(dcm[1][1]),
					utils.convert_to_dec(dcm[1][1]),
					utils.convert_to_dec(dcm[2][0]),
					utils.convert_to_dec(dcm[2][1]),
					utils.convert_to_dec(dcm[2][2]));
		}
		else
		{
			printf("Unknown mode");
			exit(1);
		}
	}

	//-----------------------------------------------------------------------------------------
	/** Returns min/max values respectively. */
	//-----------------------------------------------------------------------------------------
	int IMU::min(int a, int b)
	{
		return ((a < b) ? (a) : (b));
	}

	int IMU::max(int a, int b)
	{
		return ((a > b) ? (a) : (b));
	}
