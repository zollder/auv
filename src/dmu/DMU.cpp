/*
 *	DMU.cpp
 *  Created on: 08.01.2015
 *	Author: zollder
 *	Inspired by: https://github.com/sparkfun/MS5803-14BA_Breakout/tree/master/Libraries/Arduino/SFE_MS5803_14BA
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
	/** Calculates temperature with 2nd order temperature compensation
	 *  Refer to MS5803 data sheet for algorithm details.
	 *  Note: the powers of 2 are replaced with bit shifts, where applicable.
	 *  example: (<<8) means x(2^8) and (>>23) means /(2^23)*/
	//-----------------------------------------------------------------------------------------
	void DMU::calculateTemperature()
	{
		// copy raw temperature value
		int32_t rawTemperature = sensor->rawData.temperature;

		// find a difference between measured and reference temperature: dT = rawTemperature - Tref
		int32_t dT = rawTemperature - ((int32_t) sensor->rawData.tReference << 8);

		// measure 1st order temperature = 20C + dT x TEMPSENS
		int32_t measuredTemp = ((int64_t) dT * sensor->rawData.tTemperature) >> 23;
		int32_t calculatedTemp = 2000 + measuredTemp;

		// perform 2nd order temperature compensation
		int64_t measuredTemp2, offset2, sensivity2;
		if (calculatedTemp < 2000)
		{
			// t < 20C
			int32_t measuredTempSquare = measuredTemp * measuredTemp;	// pre-calculate
			measuredTemp2 = 3 * ((int64_t)dT*dT >> 33);
			offset2 = 3 * measuredTempSquare / 2;
			sensivity2 = 5 * measuredTempSquare / 8;
		}
		else
		{
			// t > 20C
			measuredTemp2 = 7 * ((int64_t)dT*dT >> 37);
			offset2 = measuredTemp * measuredTemp / 16;
			sensivity2 = 0;
		}

		// convert to actual temperature and save
		calculatedTemp = calculatedTemp - measuredTemp2;
		temperature = (float) calculatedTemp / 100;

		offset = ((int64_t) sensor->rawData.pOffset << 16) +
				(((int64_t) sensor->rawData.tOffset * dT) >> 7);

		sensivity = ((int64_t) sensor->rawData.pSensitivity << 15) +
				(((int64_t) sensor->rawData.tSensitivity * dT) >> 8);

		offset = offset - offset2;
		sensivity = sensivity - sensivity2;
	}

	//-----------------------------------------------------------------------------------------
	/** Calculates temperature-compensated pressure using sensor coefficients.
	 *  Refer to MS5803 data sheet for corresponding algorithm details. */
	//-----------------------------------------------------------------------------------------
	void DMU::calculatePressure()
	{
		// variables
		int32_t rawPressure = sensor->rawData.pressure;

		/* Calculate temperature-compensated pressure (with 0.1 mBar resolution). */
		int32_t calculatedPressure = ((((rawPressure * sensivity) >> 21) - offset) >> 15);

		// convert to mBar and save
		this->pressure = calculatedPressure / 10;

	}

	//-----------------------------------------------------------------------------------------
	/** Converts calculated pressure (mBARs) into the units of distance (cm) in fresh water.
	 *  It is assumed that temperature-compensated pressure is calculated and available.
	 *  Note: depth = pressure / (fluid density x gravity). */
	//-----------------------------------------------------------------------------------------
	void DMU::calculateDepth()
	{
		/* P(pascal) = P(mBar) x 100
		 * P actual = P measured - P sea-level
		 * depth (centimeters) = depth (meters) x 100 */

		depth_tmp = ((pressure - sea_level_pressure) * 100) * 100 / (density * gravity);
		depth_tmp = depth_tmp + DEPTH_COMPENSATOR;

		//disregard connection errors that causes high negative values.
		if (depth_tmp > 0 && depth_tmp < 600)
			depth = depth_tmp;
	}
