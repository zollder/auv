/*
 * LSM303.cpp
 *
 *  Created on: 30.09.2013
 *      Author: tuuzdu
 */

#include "LSM303.h"
#include "../sys/I2C.h"
//#include <stdio.h>

// for LSM303D magnetometer and accelerometer addresses are equal
#define LSM303_ACC_ADDRESS (0x1D >> 1)	// 00011101b
#define LSM303_MAG_ADDRESS (0x1D >> 1)	// 00011101b

#define BUS	1

// Initialize I2Cs ()
I2C Wire_Acc = I2C(BUS, LSM303_ACC_ADDRESS);
I2C Wire_Mag = I2C(BUS, LSM303_MAG_ADDRESS);

// Turns on the LSM303's accelerometer and magnetometer and places them in normal mode.
void LSM303::enableDefault(void) {
	/*
	 * CTRL1 register (accelerometer)
	 * 0-3: 0001 - 3.125Hz
	 * 4: 0 - continuous update
	 * 5: 1 - Z-axis enabled
	 * 6: 1 - Y-axis enabled
	 * 7: 1 - X-axis enabled
	 * result: 00010111 => 0x17
	 */
	writeAccReg(LSM303_CTRL_REG1_A, 0x17);

	/*
	 * CTRL2 register (accelerometer)
	 * 0-1: 00 - anti-alias filter (773Hz)
	 * 2-4: 011 - ±8g full-scale acceleration
	 * 5: 0 - reserved
	 * 6: 0 - self-test disabled
	 * 7: 0 - 4-wire serial interface
	 * result: 00011000 => 0x18
	 */
	writeAccReg(LSM303_CTRL_REG2_A, 0x18);

	/*
	 * CTRL5 register (magnetometer)
	 * 0: 0 - temperature sensor disabled
	 * 1-2: 11 - high resolution
	 * 3-5: 000 - data rate 3.125Hz
	 * 6-7: 00 - interrupt requests
	 * result: 01100000 => 0x60
	 */
	writeMagReg(LSM303_CTRL_REG5_A, 0x60);

	/*
	 * CTRL6 register (magnetometer)
	 * 0: 0 - reserved
	 * 1-2: 10 - ±8 gauss full-scale
	 * 3-7: 00000 - reserved
	 * result: 01000000 => 0x40
	 */
	writeMagReg(LSM303_CTRL_REG6_A, 0x40);

	/*
	 * CTRL7 register (magnetometer)
	 * 0-1: 00 - high-pass filtermode - normal mode (resets axis registers)
	 * 2: 0 - bypass acceleration data filter
	 * 3: 0 - temperature sensor only mode OFF
	 * 4: 0 - reserved
	 * 5: 0 - magnetic data low-power mode OFF
	 * 6-7: 00 - continuous-conversion mode ON
	 * result: 00000000 => 0x00
	 */
	writeMagReg(LSM303_REFERENCE_A, 0x00);
}

// Writes an accelerometer register
void LSM303::writeAccReg(char reg, char value) {
	Wire_Acc.writeI2CDeviceByte(reg, value);
}

// Writes a magnetometer register
void LSM303::writeMagReg(char reg, char value) {
	Wire_Mag.writeI2CDeviceByte(reg, value);
}

// Reads an accelerometer register
char LSM303::readAccReg(char reg) {
	return Wire_Acc.readI2CDeviceByte(reg);
}

// Reads a magnetometer register
char LSM303::readMagReg(char reg) {
	return Wire_Mag.readI2CDeviceByte(reg);
}

// Reads the 3 accelerometer channels and stores them in vector a
void LSM303::readAcc(void) {
	// assert the MSB of the address to get the accelerometer
	// to do slave-transmit subaddress updating.
	Wire_Acc.readI2CDeviceMultipleByte(LSM303_OUT_X_L_A | (1 << 7), 6);

	char xla = Wire_Acc.dataBuffer[0];
	char xha = Wire_Acc.dataBuffer[1];
	char yla = Wire_Acc.dataBuffer[2];
	char yha = Wire_Acc.dataBuffer[3];
	char zla = Wire_Acc.dataBuffer[4];
	char zha = Wire_Acc.dataBuffer[5];

	// combine high and low bytes, then shift right to discard lowest 4 bits (which are meaningless)
	// GCC performs an arithmetic right shift for signed negative numbers, but this code will not work
	// if you port it to a compiler that does a logical right shift instead.

	a.x = (float) convertMsbLsb(xha, xla, 4);
	a.y = (float) convertMsbLsb(yha, yla, 4);
	a.z = (float) convertMsbLsb(zha, zla, 4);
//  printf ("aroll: %.2f \tapitch: %.2f  \tayaw: %.2f\n", a.x, a.y, a.z);
}

// Reads the 3 magnetometer channels and stores them in vector m
void LSM303::readMag(void) {
	Wire_Mag.readI2CDeviceMultipleByte(LSM303_OUT_X_H_M, 6);

	char xhm = Wire_Mag.dataBuffer[0];
	char xlm = Wire_Mag.dataBuffer[1];
	char zhm = Wire_Mag.dataBuffer[2];
	char zlm = Wire_Mag.dataBuffer[3];
	char yhm = Wire_Mag.dataBuffer[4];
	char ylm = Wire_Mag.dataBuffer[5];
	// combine high and low bytes
	m.x = (float) convertMsbLsb(xhm, xlm, 0);
	m.y = (float) convertMsbLsb(yhm, ylm, 0);
	m.z = (float) convertMsbLsb(zhm, zlm, 0);
//  printf ("mroll: %.2f \tmpitch: %.2f \tmyaw: %.2f\n", m.x, m.y, m.z);
}

short LSM303::convertMsbLsb(char msb, char lsb, char discard) {
	short temp;
	temp = (msb << 8 | lsb);
	temp = temp >> discard;
	return temp;
}
