/*
 *	LSM303.h
 *	Created on: 04.01.2015
 *	Author: zollder
 *	https://github.com/vic320/Arduino-MS5803-14BA/blob/master/MS5803.cpp
 */

#include "../sys/I2C.h"
#include "../commons/Vector.h"
#include "../sys/FdTimer.h"

#include <stdio.h>

using namespace std;

#ifndef ms5803_h
#define ms5803_h

/* DMU's single-shot timer ID */
#define DMU_SH_TIMER 6
#define DMU_READ_TIMER_DELAY 0.009	//9ms

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

/** conversion precision constants (pressure & temperature respectively) */
#define ADC_PRESSURE	0x48
#define ADC_TEMPRATURE	0x58

/** MS5803 pressure & temperature sensor (PTS) I2C wrapper interface. */
class MS5803
{
	struct PTSRawData
	{
	    // pressure and temperature data
		uint32_t pressure;		// AdcPressure
		uint32_t temperature;	// AdcTemperature

	    // calibration coefficients
	    uint16_t pSensitivity;	// C1: pressure sensitivity (SENS)
	    uint16_t pOffset;		// C2: pressure Offset (OFF)
	    uint16_t tSensitivity;	// C3: temperature coefficient of pressure sensitivity (TCS)
	    uint16_t tOffset;    	// C4: temperature coefficient of pressure offset (TCO)
	    uint16_t tReference;	// C5: reference temperature (Tref)
	    uint16_t tTemperature;	// C6: temperature coefficient of the temperature (TEMPSENS)
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
		 * Resets the PTS device to make sure the calibration PROM is loaded
		 * into the internal register.
		 */
		void reset(void);

		/**
		 * Reads calibration coefficients from PTS's PROM.
		 * Stores read values in the local raw data holder.
		 */
		void readCoefficients(void);

		/** Validates PROM coefficients */
		void validateCoefficients();

		/** Reads raw sensor data (temperature& pressure)
		 *  and saves it in a local raw data holder. */
		void readRawData()
		{
			readTemperature();
			readPressure();
		}

		/** Reads temperature sensor and stores the data in a local raw data holder. */
		void readTemperature(void);

		/** Reads pressure sensor and stores the data in a local raw data holder. */
		void readPressure(void);

		//-----------------------------------------------------------------------------------------
		// Instance variables
		//-----------------------------------------------------------------------------------------

		/** Raw PTS data holder. */
		PTSRawData rawData;

		//-----------------------------------------------------------------------------------------
		// Private members
		//-----------------------------------------------------------------------------------------
		private:

			//-----------------------------------------------------------------------------------------
			// Methods and structures
			//-----------------------------------------------------------------------------------------

			/** Writes commands to PTS device. */
			void writeValue(char value);

			//-----------------------------------------------------------------------------------------
			// Instance variables
			//-----------------------------------------------------------------------------------------
			I2C* ptsWire;
			FdTimer* delayTimer;

			/** MS5803 coefficients CRC4 value holder. */
			uint16_t sensorCRC4 = 0;
};

#endif

