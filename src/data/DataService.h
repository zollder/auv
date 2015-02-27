/*
 *	DataService.h
 *  Created on: 07.02.2015
 *	Author: zollder
 */

#include <stdio.h>

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
			this->copySensorData();
			this->copyDesiredData();
			this->copyCameraData();
			return dataHolder;
		}

		int getSize()
		{
			return sizeof(dataHolder);
		}

		void saveData(float data[15])
		{
			/**
			 * Use id (1st array element) to choose an appropriate data holder to save the data to
			 * 1: front camera
			 * 2: bottom camera
			 */
			int camId = (int) data[0];
			switch (camId)
			{
				case 1:
					cameraData->mutex.lock();
						cameraData->objId_f = (int) data[1];
						cameraData->objColor_f = (int) data[2];
						cameraData->objOffsetX_f = data[3];
						cameraData->objOffsetY_f = data[4];
						cameraData->objAngle_f = data[5];
						cameraData->radius_f = data[6];
						cameraData->objWidth_f = data[7];
						cameraData->objHeight_f = data[8];
					cameraData->mutex.unlock();
					break;

				case 2:
					cameraData->mutex.lock();
						cameraData->objtId_b = (int) data[1];
						cameraData->objColor_b = (int) data[2];
						cameraData->objOffsetX_b = data[3];
						cameraData->objOffsetY_b = data[4];
						cameraData->objAngle_b = data[5];
						cameraData->radius_b = data[6];
						cameraData->objWidth_b = data[7];
						cameraData->objHeight_b = data[8];
					cameraData->mutex.unlock();
					break;

				default:
					printf("Unsupported id: %d.\n", camId);
			};
		}

	private:

		SensorData* sensorData;
		DesiredData* desiredData;
		CameraData* cameraData;

		float dataHolder[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

		void copySensorData()
		{
			printf("Reading sensor data ...\n");
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

		// 1: front camera data, 2: bottom camera data
		void copyCameraData()
		{
			printf("Reading camera data ...\n");
			cameraData->mutex.lock();
				dataHolder[9] = cameraData->objOffsetX_f;
				dataHolder[10] = cameraData->objOffsetY_f;
				dataHolder[11] = cameraData->objAngle_f;
				dataHolder[12] = cameraData->objOffsetX_b;
				dataHolder[13] = cameraData->objOffsetY_b;
				dataHolder[14] = cameraData->objAngle_b;
			cameraData->mutex.unlock();
		}
};

#endif
