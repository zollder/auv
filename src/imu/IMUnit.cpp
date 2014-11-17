/*
 *	IMUnit.cpp
 *	Created on: 16.11.2014
 *	Author: zollder
 */
#include "IMUnit.h"

#include <math.h>

	/**-----------------------------------------------------------------------------------------
	 * Constructor
	 * -----------------------------------------------------------------------------------------*/
	IMUnit::IMUnit()
	{
		printf("Constructing IMU ...\n");

		// instantiate devices
		gyro = new L3G();
		compass = new LSM303();

		// initialize sensor data holders
		gyroData = new SensorData<int>(0, 0, 0);
		accelData = new SensorData<int>(0, 0, 0);
		magData = new SensorData<int>(0, 0, 0);
		corrMagData = new SensorData<float>(0, 0, 0);

		// initialize holders
		roll = 0;
		pitch = 0;
		yaw = 0;
		magHeading = 0;
		counter = 0;
		G_Dt = 0.04;
	}

	/**-----------------------------------------------------------------------------------------
	 * Destructor
	 * -----------------------------------------------------------------------------------------*/
	IMUnit::~IMUnit()
	{
		printf("Destroying IMU ...\n");

		delete corrMagData;
		delete magData;
		delete accelData;
		delete gyroData;
		delete compass;
		delete gyro;
	}

	void IMUnit::initialize()
	{
		gyro->enable();
		compass->enable();

		// accumulate sensor readings
		for(int i=0; i<32; i++)
		{
			readGyroscope();
			readAccelerometer();
			for(int i=0; i<6; i++)
				sensorOffset[i] = sensorOffset[i] + sensorData[i];

			usleep(40000);
		}

		// populate offset holder
		for(int i=0; i<6; i++)
			sensorOffset[i] = sensorOffset[i]/32;

		// gravity compensation
		sensorOffset[5] = sensorOffset[5] - GRAVITY*sensorSign[5];

		printf("offset: %d, %d, %d, %d, %d, %d", sensorOffset[0], sensorOffset[1], sensorOffset[2],
				sensorOffset[3], sensorOffset[4], sensorOffset[5]);
	}

	void IMUnit::readGyroscope()
	{
		gyro->readGyroscopeData();

		sensorData[0] = gyro->gyroRawData.x;
		sensorData[1] = gyro->gyroRawData.y;
		sensorData[2] = gyro->gyroRawData.z;

		gyroData->x = sensorSign[0] * (sensorData[0] - sensorOffset[0]);
		gyroData->y = sensorSign[1] * (sensorData[1] - sensorOffset[1]);
		gyroData->z = sensorSign[2] * (sensorData[2] - sensorOffset[2]);

//		printf("gX: %d \tgY: %d  \tgZ: %.d\n", gyroData->x, gyroData->y, gyroData->z);
	}

	void IMUnit::readAccelerometer()
	{
		compass->readAccelerometerData();

		sensorData[3] = compass->accelRawData.x;
		sensorData[4] = compass->accelRawData.y;
		sensorData[5] = compass->accelRawData.z;

		accelData->x = sensorSign[3] * (sensorData[3] - sensorOffset[3]);
		accelData->y = sensorSign[4] * (sensorData[4] - sensorOffset[4]);
		accelData->z = sensorSign[5] * (sensorData[5] - sensorOffset[5]);

//		printf("aX: %d \taY: %d  \taZ: %d\n", accelData->x, accelData->y, accelData->z);
	}

	void IMUnit::readMagnetometer()
	{
		compass->readMagnetometerData();

		magData->x = sensorSign[6] * compass->magnetRawData.x;
		magData->y = sensorSign[7] * compass->magnetRawData.y;
		magData->z = sensorSign[8] * compass->magnetRawData.z;

//		printf("raw: %d, %d, %d   \n", magData->x, magData->y, magData->z);
	}

	void IMUnit::setCalibrationMode(bool mode)
	{
		calibrationMode = mode;
	}

	void IMUnit:: findMagnetometerHeading()
	{
		float compensatedMagX, compensatedMagY;
		float cos_roll, sin_roll, cos_pitch, sin_pitch;

		cos_roll = cos(roll);
		sin_roll = sin(roll);
		cos_pitch = cos(pitch);
		sin_pitch = sin(pitch);

		// adjust for LSM303 compass axis offsets/sensitivity differences by scaling to +/-0.5 range
		corrMagData->x = (float) (magData->x - sensorSign[6] * M_X_MIN) / (M_X_MAX - M_X_MIN) - sensorSign[6]*0.5;
		corrMagData->y = (float) (magData->y - sensorSign[7] * M_Y_MIN) / (M_Y_MAX - M_Y_MIN) - sensorSign[7]*0.5;
		corrMagData->z = (float) (magData->z - sensorSign[8] * M_Z_MIN) / (M_Z_MAX - M_Z_MIN) - sensorSign[8]*0.5;

		printf("corrected: %.2f, %.2f, %.2f   \n", corrMagData->x, corrMagData->y, corrMagData->z);
		//  printf ("\033[1Aroll: %.2f \tpitch: %.2f  \tyaw: %.2f\n", c_magnetom_x, c_magnetom_y, c_magnetom_z);

		// Calculate tilt-compensated magnetic fields for X and Y axis:
		compensatedMagX = corrMagData->x * cos_pitch +
						  corrMagData->y * sin_roll * sin_pitch +
						  corrMagData->z * cos_roll * sin_pitch;
		compensatedMagY = corrMagData->y * cos_roll - corrMagData->z * sin_roll;

		// Calculate magnetic heading
		magHeading = atan2(-compensatedMagY, compensatedMagX);

//		printf ("Magnetometer heading: %.2f\n", magnetometerHeading);
	}

	/**
		Returns the angular difference in the horizontal plane between the "from" vector and north, in degrees.
		Description of heading algorithm:
		Shift and scale the magnetic reading based on calibration data to find the North vector.
		Use the acceleration readings to determine the Up vector (gravity is measured as an upward acceleration).
		The cross product of North and Up vectors is East. The vectors East and North form a basis for the
		horizontal plane. The From vector is projected into the horizontal plane and the angle between
		the projected vector and horizontal north is returned.
	*/
	float IMUnit:: getHeading()
	{
		readMagnetometer();
		readAccelerometer();

		vector<int> tempMagData = {magData->x, magData->y, magData->z};
		vector<int> tempAccData = {accelData->x, accelData->y, accelData->z};

		// subtract offset (average of min and max) from magnetometer readings
		tempMagData.x -= - (M_X_MIN + M_X_MAX)/2;
		tempMagData.y -= - (M_Y_MIN + M_Y_MAX)/2;
		tempMagData.z -= - (M_Z_MIN + M_Z_MAX)/2;

		// calculate North and East
		vector<float> east;
		vector<float> north;
		vector<int> reference = {1,0,0};

		findCrossProduct(&tempMagData, &tempAccData, &east);
		normalizeData(&east);
		findCrossProduct(&tempAccData, &east, &north);
		normalizeData(&north);

		float heading = ToDeg(atan2(findDotProduct(&east, &reference), findDotProduct(&north, &reference)));
		return heading;
	}

	void IMUnit::normalize()
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

	void IMUnit::correctDrift()
	{
		float magHeadingX;
		float magHeadingY;
		float errorCourse;

		//Compensation the Roll, Pitch and Yaw drift.
		static float Scaled_Omega_P[3];
		static float Scaled_Omega_I[3];
		float accelMagnitude;
		float accelWeight;

		//*****Roll and Pitch***************

		// Calculate accelerometer vector magnitude
		accelMagnitude = sqrt(accelVector[0]*accelVector[0] + accelVector[1]*accelVector[1] + accelVector[2]*accelVector[2]);
		accelMagnitude = accelMagnitude/GRAVITY; // Scale to gravity.

		// Dynamic weighting of accelerometer info (reliability filter)
		// Weight for accelerometer info (<0.5G = 0.0, 1G = 1.0 , >1.5G = 0.0)
		accelWeight = constrain(1 - 2*abs(1 - accelMagnitude), 0, 1); //

		Vector_Cross_Product(&errorRollPitch[0], &accelVector[0], &DCM_Matrix[2][0]); //adjust the ground of reference
		Vector_Scale(&Omega_P[0], &errorRollPitch[0], Kp_ROLLPITCH * accelWeight);

		Vector_Scale(&Scaled_Omega_I[0], &errorRollPitch[0], Ki_ROLLPITCH * accelWeight);
		Vector_Add(Omega_I, Omega_I, Scaled_Omega_I);

		//*****YAW***************
		// We make the gyro YAW drift correction based on compass magnetic heading

		magHeadingX = cos(magHeading);
		magHeadingY = sin(magHeading);
		errorCourse = (DCM_Matrix[0][0] * magHeadingY) - (DCM_Matrix[1][0] * magHeadingX); //Calculating YAW error
		Vector_Scale(errorYaw, &DCM_Matrix[2][0], errorCourse); //Applys the yaw correction to the XYZ rotation of the aircraft, depeding the position.

		Vector_Scale(&Scaled_Omega_P[0], &errorYaw[0], Kp_YAW); //.01proportional of YAW.
		Vector_Add(Omega_P, Omega_P, Scaled_Omega_P); //Adding  Proportional.

		Vector_Scale(&Scaled_Omega_I[0], &errorYaw[0], Ki_YAW); //.00001Integrator
		Vector_Add(Omega_I, Omega_I, Scaled_Omega_I); //adding integrator to the Omega_I
	}

	void IMUnit::updateMatrix()
	{
		gyroVector[0] = Gyro_Scaled_X(accelData->x); //gyro x roll
		gyroVector[1] = Gyro_Scaled_Y(accelData->y); //gyro y pitch
		gyroVector[2] = Gyro_Scaled_Z(accelData->z); //gyro Z yaw

		accelVector[0] = accelData->x;
		accelVector[1] = accelData->y;
		accelVector[2] = accelData->z;

		Vector_Add(&Omega[0], &gyroVector[0], &Omega_I[0]); //adding proportional term
		Vector_Add(&Omega_Vector[0], &Omega[0], &Omega_P[0]); //adding Integrator term

		//Accel_adjust();    //Remove centrifugal acceleration.   We are not using this function in this version - we have no speed measurement

		if(!calibrationMode)	 // with drift correction
		{
			Update_Matrix[0][0] = 0;
			Update_Matrix[0][1] = -G_Dt * Omega_Vector[2]; //-z
			Update_Matrix[0][2] = G_Dt * Omega_Vector[1]; //y
			Update_Matrix[1][0] = G_Dt * Omega_Vector[2]; //z
			Update_Matrix[1][1] = 0;
			Update_Matrix[1][2] = -G_Dt * Omega_Vector[0]; //-x
			Update_Matrix[2][0] = -G_Dt * Omega_Vector[1]; //-y
			Update_Matrix[2][1] = G_Dt * Omega_Vector[0]; //x
			Update_Matrix[2][2] = 0;
		}
		else	// no drift correction
		{
			Update_Matrix[0][0]=0;
			Update_Matrix[0][1]=-G_Dt*gyroVector[2]; //-z
			Update_Matrix[0][2]=G_Dt*gyroVector[1];//y
			Update_Matrix[1][0]=G_Dt*gyroVector[2];//z
			Update_Matrix[1][1]=0;
			Update_Matrix[1][2]=-G_Dt*gyroVector[0];
			Update_Matrix[2][0]=-G_Dt*gyroVector[1];
			Update_Matrix[2][1]=G_Dt*gyroVector[0];
			Update_Matrix[2][2]=0;
		}

		Matrix_Multiply(DCM_Matrix, Update_Matrix, Temporary_Matrix); //a*b=c

		for (int x = 0; x < 3; x++) //Matrix Addition (update)
			for (int y = 0; y < 3; y++)
				DCM_Matrix[x][y] += Temporary_Matrix[x][y];
	}

	void IMUnit::calculateEulerAngles()
	{
		pitch = -asin(DCM_Matrix[2][0]);
		roll = atan2(DCM_Matrix[2][1], DCM_Matrix[2][2]);
		yaw = atan2(DCM_Matrix[1][0], DCM_Matrix[0][0]);
	}

	template <typename Ta, typename Tb, typename To> void IMUnit::findCrossProduct(const vector<Ta> *a,const vector<Tb> *b, vector<To> *out)
	{
	  out->x = (a->y * b->z) - (a->z * b->y);
	  out->y = (a->z * b->x) - (a->x * b->z);
	  out->z = (a->x * b->y) - (a->y * b->x);
	}

	template <typename Ta, typename Tb> float IMUnit::findDotProduct(const vector<Ta> *a, const vector<Tb> *b)
	{
	  return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
	}

	void IMUnit::normalizeData(vector<float> *a)
	{
	  float mag = sqrt(findDotProduct(a, a));
	  a->x /= mag;
	  a->y /= mag;
	  a->z /= mag;
	}
