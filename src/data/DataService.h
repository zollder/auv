/*
 *	DataService.h
 *  Created on: 07.02.2015
 *	Author: zollder
 */

#include <stdio.h>

#include "../commons/Config.h"
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

//TODO		void reset(char ip)
//		{
//			if (ip == "192.168.0.11")
//				printf("bottom server dead");
//			else if (ip == "192.168.0.12")
//				printf("front server dead");
//		}

	private:

		SensorData* sensorData;
		DesiredData* desiredData;
		CameraData* cameraData;

		float dataHolder[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

		void copySensorData()
		{
			/*printf("Reading sensor data ...\n");*/
			sensorData->mutex.lock();
				dataHolder[0] = sensorData->roll;
				dataHolder[1] = sensorData->pitch;
				dataHolder[2] = sensorData->yaw;
				dataHolder[3] = sensorData->depth;
			sensorData->mutex.unlock();
		}

		void copyDesiredData ()
		{
			/*printf("Reading desired data ...\n");*/
			desiredData->mutex.lock();
				dataHolder[4] = (float) desiredData->heading;
				dataHolder[5] = (float) desiredData->depth;
				dataHolder[6] = (float) desiredData->speed;
				dataHolder[7] = desiredData->reverse ? 1.0 : 0.0;
				dataHolder[8] = desiredData->drift ? 1.0 : 0.0;
			desiredData->mutex.unlock();
		}

		// 1: front camera data, 2: bottom camera data
		void copyCameraData()
		{
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
