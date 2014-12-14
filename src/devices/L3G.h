/*
 *	LSM303.h
 *	Author: zollder
 */

#include "../sys/I2C.h"
#include "../commons/Vector.h"

#include <stdio.h>

using namespace std;

#ifndef L3G_h
#define L3G_h

// read/write bit is set by ioCtr in I2C.cpp
#define GYRO_ADDRESS 0x6B
#define I2C_BUS	1

// register addresses

#define L3G_WHO_AM_I      0x0F

#define L3G_CTRL_REG1     0x20
#define L3G_CTRL_REG2     0x21
#define L3G_CTRL_REG3     0x22
#define L3G_CTRL_REG4     0x23
#define L3G_CTRL_REG5     0x24
#define L3G_REFERENCE     0x25
#define L3G_OUT_TEMP      0x26
#define L3G_STATUS_REG    0x27

#define L3G_OUT_X_L       0x28
#define L3G_OUT_X_H       0x29
#define L3G_OUT_Y_L       0x2A
#define L3G_OUT_Y_H       0x2B
#define L3G_OUT_Z_L       0x2C
#define L3G_OUT_Z_H       0x2D

class L3G
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		/** Constructor. */
		L3G();

		/** Destructor. */
		~L3G();

		//-----------------------------------------------------------------------------------------
		// Methods and structures
		//-----------------------------------------------------------------------------------------

		/** Turns on and configures L3G gyroscope. */
		void enable(void);

		/** Reads gyroscope registers and stores the values in a RawData structure. */
		void readGyroscopeData(void);

		//-----------------------------------------------------------------------------------------
		// Instance variables
		//-----------------------------------------------------------------------------------------

		Vector<int> gyroRawData;	/** Gyroscope's raw data holder (angular velocity data). */

		//-----------------------------------------------------------------------------------------
		// Private members
		//-----------------------------------------------------------------------------------------
		private:

			//-----------------------------------------------------------------------------------------
			// Methods and structures
			//-----------------------------------------------------------------------------------------

			/** Writes gyroscope register. */
			void writeRegister(char reg, char value);

			/** Reads gyroscope register. */
			char readRegister(char reg);

			/** Combines high & low bytes. */
			short convertMsbLsb(char msb, char lsb);

			//-----------------------------------------------------------------------------------------
			// Instance variables
			//-----------------------------------------------------------------------------------------
			I2C* gyroscopeWire;
};

#endif

