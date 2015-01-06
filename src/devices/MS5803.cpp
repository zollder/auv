/*
 *	MS5803.cpp
 *  Created on: 05.01.2015
 *	Author: zollder
 */

#include "MS5803.h"

/**
 * MS5803 pressure & temperature sensor (PTS) I2C wrapper implementation.
 * */

	/** Constructor. */
	MS5803::MS5803()
	{
		printf("Constructing MS5803 ...\n");
		ptsWire = new I2C(PTS_I2C_BUS, PTS_ADDRESS);
	}

	/** Destructor. */
	MS5803::~MS5803()
	{
		printf("Destroying MS5803 ...\n");
		delete ptsWire;
	}

	//-----------------------------------------------------------------------------------------
	/** Resets the PTS device and reads PROM calibration constants.
	 * 	Must be called before the actual sensor read. */
	//-----------------------------------------------------------------------------------------
	void MS5803::initialize()
	{
		ptsWire->ms5803_init();
//		this->reset();
//		usleep(3000);
//		this->readCoefficients();
	}

	//-----------------------------------------------------------------------------------------
	/** Resets PTS device. */
	//-----------------------------------------------------------------------------------------
	void MS5803::reset(void)
	{
		printf("resetting ...\n");
		writeValue(PTS_RESET);
	}

	//-----------------------------------------------------------------------------------------
	/** Reads calibration constants from PTS's PROM.
	 *  Stores result in the local raw data holder. */
	//-----------------------------------------------------------------------------------------
	void MS5803::readCoefficients()
	{
		printf("reading coefficients ...\n");

		printf("writing address %d ...\n", PROM_C1);
		writeValue(PROM_C1);
		usleep(3000);
		printf("reading value %d ...\n", PROM_C1);
		uint8_t byte = ptsWire->readByte(PROM_C1);
//		char lsb = ptsWire->dataBuffer[0];
//		char msb = ptsWire->dataBuffer[1];
//		coefficient[0] = (uint16_t) convertMsbLsb(msb, lsb);
		printf("%7d\n", (int)byte);

//		for (int i = 0; i <= 7; i++)
//		{
//			printf("writing address %d ...\n", i);
//			writeValue(PROM_BASE + i*2);
//
//			printf("reading sensor %d ...\n", i);
//			ptsWire->readI2CDeviceMultipleByte(PTS_ADDRESS, 2);
//			char lsb = ptsWire->dataBuffer[0];
//			char msb = ptsWire->dataBuffer[1];
//			coefficient[i] = (uint16_t) convertMsbLsb(msb, lsb);
//
//			printf("%7d\n", coefficient[i]);
//		}

//		printf("%7d %7d %7d \n", gyroRawData.x, gyroRawData.y, gyroRawData.z);
	}

	//-----------------------------------------------------------------------------------------
	/** Writes a gyro register. */
	//-----------------------------------------------------------------------------------------
	void MS5803::writeValue(char value)
	{
		ptsWire->writeI2CDeviceByte(value);
	}

	//-----------------------------------------------------------------------------------------
	/** Combines high & low bytes. */
	//-----------------------------------------------------------------------------------------
	short MS5803::convertMsbLsb(char msb, char lsb)
	{
		short temp = (msb << 8 | lsb);
		return temp;
	}
