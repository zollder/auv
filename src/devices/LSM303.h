/*
 *	LSM303.h
 *  Created on: 10.11.2014
 *	Author: zollder
 */

#include "../sys/I2C.h"
#include <stdio.h>

#ifndef LSM303_H_
#define LSM303_H_

// LSM303D uses same address for magnetometer and accelerometer
#define ACC_MAG_ADDRESS 0x1D		// 00011101b
#define I2C_BUS	1

// register addresses
#define CTRL_REG1_A		0x20
#define CTRL_REG2_A		0x21
#define CTRL_REG3_A		0x22
#define CTRL_REG4_A		0x23
#define CTRL_REG5_A		0x24
#define CTRL_REG6_A		0x25
#define CTRL_REG7_A		0x26

#define OUT_X_L_A		0x28
#define OUT_X_H_A		0x29
#define OUT_Y_L_A		0x2A
#define OUT_Y_H_A		0x2B
#define OUT_Z_L_A		0x2C
#define OUT_Z_H_A		0x2D

#define D_OUT_X_L_M		0x08
#define D_OUT_X_H_M		0x09
#define D_OUT_Y_L_M		0x0A
#define D_OUT_Y_H_M		0x0B
#define D_OUT_Z_L_M		0x0C
#define D_OUT_Z_H_M		0x0D

class LSM303
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		/** Constructor. */
		LSM303();

		/** Destructor. */
		~LSM303();

		//-----------------------------------------------------------------------------------------
		// Methods and structures
		//-----------------------------------------------------------------------------------------

		/** Raw data holder structure. */
		typedef struct RawData
		{
			int x, y, z;
		} RawData;

		/** Turns on and configures LSM303's accelerometer and magnetometer. */
		void enable(void);

		/** Reads accelerometer registers and stores the values in a RawData structure. */
		void readAccelerometerData(void);

		/** Reads magnetometer registers and stores the values in a RawData structure. */
		void readMagnetometerData(void);

		//-----------------------------------------------------------------------------------------
		// Instance variables
		//-----------------------------------------------------------------------------------------
		RawData accelRawData;	/** Accelerometer's raw data holder. */
		RawData magnetRawData;	/** Magnetometer's raw data holder. */

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:

		//-----------------------------------------------------------------------------------------
		// Methods and structures
		//-----------------------------------------------------------------------------------------
		/** Writes an accelerometer register. */
		void writeAccelerometerRegister(char reg, char regValue);

		/** Reads accelerometer register. */
		char readAccelerometerRegister(char regAddress);

		/** Writes magnetometer register. */
		void writeMagnetometerRegister(char reg, char regValue);

		/** Reads magnetometer register. */
		char readMagnetometerRegister(char regAddress);

		/**
		 * Combines high & low bytes.
		 * There is no need to discard 4 lower bits as LSM303D has 16-bit resolution.
		 */
		short convertMsbLsb(char msb, char lsb);

		//-----------------------------------------------------------------------------------------
		// Instance variables
		//-----------------------------------------------------------------------------------------
		I2C* accelerometerWire;
		I2C* magnetometerWire;
};

#endif /* LSM303_H_ */
