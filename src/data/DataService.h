/*
 *	DataService.h
 *  Created on: 07.02.2015
 *	Author: zollder
 */
#include "SensorData.h"
#include "DesiredData.h"
#include "CameraData.h"

#ifndef dataservice_h
#define dataservice_h

//-----------------------------------------------------------------------------------------
// DataService interface.
//-----------------------------------------------------------------------------------------
class DataService
{

	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		DataService(SensorData* sData, DesiredData* dData, CameraData* cData)
		{
			sensorData = sData;
			desiredData = dData;
			cameraData = cData;

			memset(dataHolder, 0, sizeof(dataHolder));	//TODO maybe remove not necessary
		}

		// destructor
		~DataService() {}

		float* getData()
		{
			// copy sensor data into array

			//copySensorData();
			//copyDesiredData();
			//copyCameraData();

			//TODO DEBUG
			copyFakeSensorData();
			copyFakeDesiredData();
			copyFakeCameraData();

			return dataHolder;
		}

		int getSize()
		{
			return sizeof(dataHolder);
		}

	private:

		SensorData* sensorData;
		DesiredData* desiredData;
		CameraData* cameraData;

		float a=25.0;
		float b=1;
		float c=2;
		float d=3;
		float e=4;
		float f=5;
		float g=6;
		float h=7;
		float i=8;
		float j=9;
		float k=10;
		float l=11;


		float dataHolder[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

		void copyFakeSensorData()
		{
			sensorData->mutex.lock();
				dataHolder[0] = a;
				dataHolder[1] += b;
				dataHolder[2] += c;
				dataHolder[3] += d;
				dataHolder[4] += e;
			sensorData->mutex.unlock();
		}
		void copyFakeDesiredData()
		{
			desiredData->mutex.lock();
				dataHolder[5] += f;
				dataHolder[6] += g;
				dataHolder[7] += h;
				dataHolder[8] += i;
			desiredData->mutex.unlock();
		}

		void copyFakeCameraData()
		{
			cameraData->mutex.lock();
				dataHolder[9] += j;
				dataHolder[10] += k;
				dataHolder[11] += l;
			cameraData->mutex.unlock();
		}

		void copySensorData()
		{
			sensorData->mutex.lock();
				dataHolder[0] = sensorData->roll;
				dataHolder[1] = sensorData->pitch;
				dataHolder[2] = sensorData->yaw;
				dataHolder[3] = sensorData->depth;
				dataHolder[4] = sensorData->temperature;
			sensorData->mutex.unlock();
		}

		void copyDesiredData ()
		{
			desiredData->mutex.lock();
				dataHolder[5] = desiredData->heading;
				dataHolder[6] = desiredData->depth;
				dataHolder[7] = desiredData->speed;
				dataHolder[8] = desiredData->reverse ? 1 : 0;
			desiredData->mutex.unlock();
		}

		void copyCameraData()
		{
			cameraData->mutex.lock();
				dataHolder[9] = cameraData->color;
				dataHolder[10] = cameraData->x;
				dataHolder[11] = cameraData->y;
			cameraData->mutex.unlock();
		}


};

#endif
