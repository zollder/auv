/*
 *	DMU.h
 *	Created on: 07.01.2015
 *	Author: zollder
 */

#include "../devices/MS5803.h"
#include <stdio.h>

using namespace std;

#ifndef amu_h
#define amu_h

/**
 * DMU (Depth Measurement Unit) class implementation.
 * Interprets raw data measured by the depth sensor.
 * Calculates temperature-compensated pressure and converts it to the units of distance.
 */
class DMU
{
	const int density = 1025;				// 1025/1000 kg/m3 for sea/fresh water
	const int sea_level_pressure = 1010;	// mBar (1ATM = 1.01BAR)
	const float gravity = 9.80665;			// m/s2

	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		/** Constructor. */
		DMU();

		/** Destructor. */
		~DMU();

		//-----------------------------------------------------------------------------------------
		// Methods and structures
		//-----------------------------------------------------------------------------------------

		/** Turns on and configures DMU. */
		void enable(void);

		/** Reads temperature and pressure data. */
		void readSensor(void);

		/** Calculates temperature with 2nd order temperature compensation
		 *  Refer to MS5803 data sheet for algorithm details.
		 *  It is assumed that raw sensor data is available. */
		void calculateTemperature(void);

		/** Calculates temperature-compensated pressure.
		 *  It is assumed that raw sensor and temperature data is available. */
		void calculatePressure(void);

		/** Converts temperature-compensated pressure (BARs) into the units of distance (cm).
		 *  It is assumed that temperature-compensated pressure is available.
		 *  Note: Note: depth = pressure / (fluid density x gravity). */
		void calculateDepth(void);

		//-----------------------------------------------------------------------------------------
		// Instance variables
		//-----------------------------------------------------------------------------------------

		float temperature = 0;
		float pressure = 0;
		int depth = 0;

		//-----------------------------------------------------------------------------------------
		// Private members
		//-----------------------------------------------------------------------------------------
		private:

			//-----------------------------------------------------------------------------------------
			// Methods and structures
			//-----------------------------------------------------------------------------------------

			// offset and sensivity at actual temperature
			int64_t offset = 0;
			int64_t sensivity = 0;

			//-----------------------------------------------------------------------------------------
			// Instance variables
			//-----------------------------------------------------------------------------------------
			MS5803* sensor;
};

#endif

