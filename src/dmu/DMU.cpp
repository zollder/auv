/*
 *	DMU.cpp
 *  Created on: 08.01.2015
 *	Author: zollder
 */

#include "DMU.h"

	/** Constructor. */
	DMU::DMU()
	{
		printf("Constructing DMU ...\n");
		sensor = new MS5803();
	}

	/** Destructor. */
	DMU::~DMU()
	{
		printf("Destroying DMU ...\n");
		delete sensor;
	}

	//-----------------------------------------------------------------------------------------
	/** Turns on and configures DMU.
	 *  Resets the PTS device and reads PROM calibration constants.
	 *  Validates retrieved coefficients using the sensor CRC4 value.
	 * 	Must be called before the actual sensor read. */
	//-----------------------------------------------------------------------------------------
	void DMU::enable(void)
	{
		sensor->reset();
		sensor->readCoefficients();
		// sensor->validateCoefficients();
	}

	//-----------------------------------------------------------------------------------------
	/** Reads pressure and temperature raw data. */
	//-----------------------------------------------------------------------------------------
	void DMU::readSensor()
	{
		sensor->readTemperature();
		sensor->readPressure();
	}

	//-----------------------------------------------------------------------------------------
	/** Calculates temperature-compensated depth. */
	//-----------------------------------------------------------------------------------------
	void DMU::calculateDepth()
	{

	}
