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

// read/write bit is set by ioCtr in I2C.cpp
#define GYRO_ADDRESS 0x6B
#define I2C_BUS	1

/**
 * DMU (Depth Measurement Unit) class implementation.
 * Interprets raw data measured by the depth sensor.
 * Calculates temperature-compensated pressure and converts it to the units of distance.
 */
class DMU
{
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

		/** Calculates temperature-compensated pressure.
		 *  Converts calculated pressure into the units of distance. */
		void calculateDepth(void);

		//-----------------------------------------------------------------------------------------
		// Instance variables
		//-----------------------------------------------------------------------------------------

		float temperature = 0;
		float pressure = 0;

		//-----------------------------------------------------------------------------------------
		// Private members
		//-----------------------------------------------------------------------------------------
		private:

			//-----------------------------------------------------------------------------------------
			// Methods and structures
			//-----------------------------------------------------------------------------------------

			float previousTemperature = 0;
			float currentTemperature = 0;
			float previousPressure = 0;
			float currentPressure = 0;

			/**  */
			void doSomething(void);

			//-----------------------------------------------------------------------------------------
			// Instance variables
			//-----------------------------------------------------------------------------------------
			MS5803* sensor;
};

#endif

