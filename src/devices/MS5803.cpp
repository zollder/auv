/*
 *	MS5803.cpp
 *  Created on: 05.01.2015
 *	Author: zollder
 *	inspired by: https://github.com/vic320/Arduino-MS5803-14BA/blob/master/MS5803.cpp
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
		delayTimer = new FdTimer(DMU_SH_TIMER, DMU_READ_TIMER_DELAY);
	}

	/** Destructor. */
	MS5803::~MS5803()
	{
		printf("Destroying MS5803 ...\n");
		delete ptsWire;
	}

	//-----------------------------------------------------------------------------------------
	/** Resets PTS device. */
	//-----------------------------------------------------------------------------------------
	void MS5803::reset(void)
	{
		printf("resetting ...\n");
		writeValue(PTS_RESET);
		usleep(3000);
	}

	//-----------------------------------------------------------------------------------------
	/** Reads calibration constants from PTS's PROM.
	 *  Stores result in the local raw data holder. */
	//-----------------------------------------------------------------------------------------
	void MS5803::readCoefficients()
	{
		printf("reading coefficients ...\n");

		ptsWire->readI2CDeviceWord(PROM_C1);
		rawData.pSensitivity = ptsWire->wordBuffer[0];

		ptsWire->readI2CDeviceWord(PROM_C2);
		rawData.pOffset = ptsWire->wordBuffer[0];

		ptsWire->readI2CDeviceWord(PROM_C3);
		rawData.tSensitivity = ptsWire->wordBuffer[0];

		ptsWire->readI2CDeviceWord(PROM_C4);
		rawData.tOffset = ptsWire->wordBuffer[0];

		ptsWire->readI2CDeviceWord(PROM_C5);
		rawData.tReference = ptsWire->wordBuffer[0];

		ptsWire->readI2CDeviceWord(PROM_C6);
		rawData.tTemperature = ptsWire->wordBuffer[0];

		printf("\n");
		printf("pSensitivity: %d\n", rawData.pSensitivity);
		printf("pOffset: %d\n", rawData.pOffset);
		printf("tSensitivity: %d\n", rawData.tSensitivity);
		printf("tOffset: %d\n", rawData.tOffset);
		printf("tReference: %d\n", rawData.tReference);
		printf("tTemperature: %d\n", rawData.tTemperature);
		printf("\n");
	}

	//-----------------------------------------------------------------------------------------
	/** Validates sensor coefficients using the CRC value at index 7 of the sensor PROM.
	 *  TODO: implement*/
	//-----------------------------------------------------------------------------------------
	void MS5803::validateCoefficients()
	{
		printf("validating coefficients ...\n");

		ptsWire->readI2CDeviceWord(PROM_CRC);
		sensorCRC4 = ptsWire->wordBuffer[0];

		int x = 1;

		printf("\n");
		printf("Coefficients are valid: %s\n", x ? "true" : "false");
		printf("\n");
	}

	//-----------------------------------------------------------------------------------------
	/** Reads and converts raw pressure values with maximum precision. */
	//-----------------------------------------------------------------------------------------
	void MS5803::readPressure(void)
	{
		printf("reading pressure ...\n");

		// send sensor conversion command
		writeValue(ADC_PRESSURE);

		// start single-shot timer and wait until it expires
		delayTimer->startSingle();
		delayTimer->waitTimerEvent();

		// read converted values (24 bits)
		ptsWire->readI2CDeviceMultipleByte(PTS_READ, 3);
		rawData.pressure =  (ptsWire->dataBuffer[0] << 16) |
							(ptsWire->dataBuffer[1] << 8) |
							(ptsWire->dataBuffer[2]);

		printf("\n");
		printf("raw pressure: %d\n", rawData.pressure);
		printf("\n");
	}

	//-----------------------------------------------------------------------------------------
	/** Reads and converts raw temperature values with maximum precision. */
	//-----------------------------------------------------------------------------------------
	void MS5803::readTemperature(void)
	{
		printf("reading temperature ...\n");

		// send sensor conversion command
		writeValue(ADC_TEMPRATURE);

		// start single-shot timer and wait until it expires
		delayTimer->startSingle();
		delayTimer->waitTimerEvent();

		// read converted values (24 bits)
		ptsWire->readI2CDeviceMultipleByte(PTS_READ, 3);
		rawData.temperature =  (ptsWire->dataBuffer[0] << 16) |
							(ptsWire->dataBuffer[1] << 8) |
							(ptsWire->dataBuffer[2]);

		printf("\n");
		printf("raw temperature: %d\n", rawData.temperature);
		printf("\n");
	}

	//-----------------------------------------------------------------------------------------
	/** Writes specified value to the sensor global (default) address. */
	//-----------------------------------------------------------------------------------------
	void MS5803::writeValue(char value)
	{
		ptsWire->writeI2CDeviceByte(value);
	}
