/*
 *	LSM303.h
 *  Created on: 10.11.2014
 *	Author: zollder
 */

#ifndef LSM303_H_
#define LSM303_H_

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

//-----------------------------------------TODO: revise & cleanup definitions
#define LSM303_HP_FILTER_RESET_A 0x25 // DLH, DLM only
#define LSM303_REFERENCE_A       0x26
#define LSM303_STATUS_REG_A      0x27

#define LSM303_FIFO_CTRL_REG_A   0x2E // DLHC only
#define LSM303_FIFO_SRC_REG_A    0x2F // DLHC only
#define LSM303_INT1_CFG_A        0x30
#define LSM303_INT1_SRC_A        0x31
#define LSM303_INT1_THS_A        0x32
#define LSM303_INT1_DURATION_A   0x33
#define LSM303_INT2_CFG_A        0x34
#define LSM303_INT2_SRC_A        0x35
#define LSM303_INT2_THS_A        0x36
#define LSM303_INT2_DURATION_A   0x37

#define LSM303_CLICK_CFG_A       0x38 // DLHC only
#define LSM303_CLICK_SRC_A       0x39 // DLHC only
#define LSM303_CLICK_THS_A       0x3A // DLHC only
#define LSM303_TIME_LIMIT_A      0x3B // DLHC only
#define LSM303_TIME_LATENCY_A    0x3C // DLHC only
#define LSM303_TIME_WINDOW_A     0x3D // DLHC only
#define LSM303_CRA_REG_M         0x00
#define LSM303_CRB_REG_M         0x01
#define LSM303_MR_REG_M          0x02

#define LSM303_SR_REG_M          0x09
#define LSM303_IRA_REG_M         0x0A
#define LSM303_IRB_REG_M         0x0B
#define LSM303_IRC_REG_M         0x0C

#define LSM303_WHO_AM_I_M        0x0F // DLM only
#define LSM303_TEMP_OUT_H_M      0x31 // DLHC only
#define LSM303_TEMP_OUT_L_M      0x32 // DLHC only

class LSM303
{
	public:

		/** Raw data holder structure. */
		typedef struct RawData
		{
			float x, y, z;
		} RawData;

		/** Accelerometer's raw data holder. */
		RawData accelData;

		/** Magnetometer's raw data holder. */
		RawData magnetData;

		// HEX  = BIN          RANGE    GAIN X/Y/Z        GAIN Z
		//                               DLH (DLM/DLHC)    DLH (DLM/DLHC)
		// 0x20 = 0b00100000   ±1.3     1055 (1100)        950 (980) (default)
		// 0x40 = 0b01000000   ±1.9      795  (855)        710 (760)
		// 0x60 = 0b01100000   ±2.5      635  (670)        570 (600)
		// 0x80 = 0b10000000   ±4.0      430  (450)        385 (400)
		// 0xA0 = 0b10100000   ±4.7      375  (400)        335 (355)
		// 0xC0 = 0b11000000   ±5.6      320  (330)        285 (295)
		// 0xE0 = 0b11100000   ±8.1      230  (230)        205 (205)
		enum magGain
		{
			magGain_13 = 0x20,
			magGain_19 = 0x40,
			magGain_25 = 0x60,
			magGain_40 = 0x80,
			magGain_47 = 0xA0,
			magGain_56 = 0xC0,
			magGain_81 = 0xE0
		};

		/** Turns on and configures LSM303's accelerometer and magnetometer. */
		void enable(void);

		/** Writes an accelerometer register. */
		void writeAccelerometerRegister(char reg, char regValue);

		/** Reads accelerometer register. */
		char readAccelerometerRegister(char regAddress);

		/** Writes magnetometer register. */
		void writeMagnetometerRegister(char reg, char regValue);

		/** Reads magnetometer register. */
		char readMagnetometerRegister(char regAddress);

		/** Reads accelerometer registers and stores the values in a RawData structure. */
		void readAccelerometerData(void);

		/** Reads magnetometer registers and stores the values in a RawData structure. */
		void readMagnetometerData(void);

		/**
		 * Combines high & low bytes.
		 * There is no need to discard 4 lower bits as LSM303D has 16-bit resolution.
		 */
		short convertMsbLsb(char msb, char lsb);

};

#endif /* LSM303_H_ */
