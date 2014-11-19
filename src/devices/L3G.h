#include "../sys/I2C.h"
#include <stdio.h>

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

#define L3G_FIFO_CTRL_REG 0x2E
#define L3G_FIFO_SRC_REG  0x2F

#define L3G_INT1_CFG      0x30
#define L3G_INT1_SRC      0x31
#define L3G_INT1_THS_XH   0x32
#define L3G_INT1_THS_XL   0x33
#define L3G_INT1_THS_YH   0x34
#define L3G_INT1_THS_YL   0x35
#define L3G_INT1_THS_ZH   0x36
#define L3G_INT1_THS_ZL   0x37
#define L3G_INT1_DURATION 0x38

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

		/** Raw data holder structure. */
		typedef struct RawData
		{
			float x, y, z;
		} RawData;

		/** Turns on and configures L3G gyroscope. */
		void enable(void);

		/** Reads gyroscope registers and stores the values in a RawData structure. */
		void readGyroscopeData(void);

		//-----------------------------------------------------------------------------------------
		// Instance variables
		//-----------------------------------------------------------------------------------------

		RawData gyroRawData;	/** Gyroscope's raw data holder (angular velocity data). */

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

