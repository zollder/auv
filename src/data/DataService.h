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
		}

		// destructor
		~DataService() {}

		float* getData()
		{
			// copy sensor data into array
			copySensorData();
			copyDesiredData();
			copyCameraData();

			return dataHolder;
		}

	private:

		SensorData* sensorData;
		DesiredData* desiredData;
		CameraData* cameraData;

		float dataHolder[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

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
