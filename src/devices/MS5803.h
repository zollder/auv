/*
 *	LSM303.h
 *	Created on: 04.01.2015
 *	Author: zollder
 */

#include "../sys/I2C.h"
#include "../commons/Vector.h"

#include <stdio.h>

using namespace std;

#ifndef ms5803_h
#define ms5803_h

// read/write bit is set by ioCtr in I2C.cpp
#define PTS_ADDRESS 0x76
#define PTS_I2C_BUS	2

// register addresses

/* commands in HEX */
#define PTS_RESET	0x1E
#define PTS_READ	0x00
#define PTS_CONVERT	0x40

/* sensor coefficients */
#define PROM_BASE	0xA0
#define PROM_C1   	0xA2
#define PROM_C2   	0xA4
#define PROM_C3   	0xA6
#define PROM_C4   	0xA8
#define PROM_C5   	0xAA
#define PROM_C6   	0xAC
#define PROM_CRC  	0xAE

/** MS5803 pressure & temperature sensor (PTS) I2C wrapper interface. */
class MS5803
{
	struct PTSRawData
	{
	    // pressure and temperature data
	    long pressure;    // AdcPressure
	    long temperature; // AdcTemperature

	    // calibration constants
	    int32_t pSensitivity;	// C1: pressure sensitivity
	    int32_t pOffset;		// C2: pressure Offset
	    int32_t tSensitivity;	// C3: temperature coefficient of pressure sensitivity
	    int32_t tOffset;    	// C4: temperature coefficient of pressure offset
	    int32_t tReference;		// C5: reference temperature
	    int32_t tTemperature;   // C6: temperature coefficient of the temperature
	};

	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		/** Constructor. */
		MS5803();

		/** Destructor. */
		~MS5803();

		//-----------------------------------------------------------------------------------------
		// Methods and structures
		//-----------------------------------------------------------------------------------------

		/**
		 * Resets the PTS device and reads PROM calibration constants.
		 * Must be called before the actual sensor read.
		 */
		void initialize(void);

		/** Reads temperature sensor and stores the data in a local raw data holder. */
		void readTemperature(void);

		/** Reads pressure sensor and stores the data in a local raw data holder. */
		void readPressure(void);

		//-----------------------------------------------------------------------------------------
		// Instance variables
		//-----------------------------------------------------------------------------------------

		/** Raw PTS data holder. */
		PTSRawData rawData;

		uint16_t coefficient[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

			/**
			 * Resets the PTS device to make sure the calibration PROM is loaded
			 * into the internal register.
			 */
			void reset(void);

		//-----------------------------------------------------------------------------------------
		// Private members
		//-----------------------------------------------------------------------------------------
		private:

			//-----------------------------------------------------------------------------------------
			// Methods and structures
			//-----------------------------------------------------------------------------------------

			/**
			 * Reads calibration constants from PTS's PROM.
			 * Stores read values in the local raw data holder.
			 */
			void readCoefficients(void);

			/** Validates PROM coefficients */
			void validateCoefficients();

			/** Writes commands to PTS device. */
			void writeValue(char value);

			/** Reads PTS device. */
			char readValue(char reg);

			/** Combines high & low bytes. */
			short convertMsbLsb(char msb, char lsb);

			//-----------------------------------------------------------------------------------------
			// Instance variables
			//-----------------------------------------------------------------------------------------
			I2C* ptsWire;
};

#endif

