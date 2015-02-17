/*
 *	PWM.h
 *  Created on: 30.11.2014
 *	Author: zollder
 */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iostream>

#include "../commons/Config.h"

using namespace std;

#ifndef pwm_h
#define pwm_h

/**
 * PWM Interface.
 * Represents a wrapper class around Beaglebone's PWM implementation.
 * It is assumed that overlays are properly configured and loaded at boot time.
 * INFO:
 * 		BeagleBone has 3 PWM modules with 2 channels each.
 * 		In the current implementation PWM pins (channels) are configured as follows:
 * 		- ehrpwm0 module: P9_29 & P9_31
 * 		- ehrpwm1 module: P9_14 & P9_16
 * 		- ehrpwm2 module: P8_13 & P8_19
 *
 * 		module IDs (names) to the channel IDs (pins) mappings:
 * 		- 1 (ehrpwm0) maps to 11(P9_29), 12(P9_31)
 * 		- 2 (ehrpwm1) maps to 21(P9_14), 22(P9_16)
 * 		- 3 (ehrpwm2) maps to 31(P8_13), 32(P8_19)
 */
class PWM
{
	public:

		/** Constructor. */
		PWM();
		~PWM();

		/** Initializes PWMs.
		 *  Stops and sets default values for selected/all 6 PWM channels. */
		void initialize();
		void initialize(int);
		void initialize(int, int);

		/** Sets/returns PWM period (frequency) for specified PWM module.
		 *  notes:
		 *  - same period is set for both module channels.
		 *  - period can be retrieved by module or channel ID in HZ or ns.
		 */
		void setPeriod(int moduleId, int value);
		int getPeriodHz(int channelId);
		int getPeriodNs(int channelId);

		/** Sets/returns duty cycle (in %) for specified channel ID. */
		void setDuty(int channelId, int value);
		int getDuty(int channelId);

		/** Sets/returns polarity value (0 or 1) for specified channel ID. */
		void setPolarity(int channelId, int value);
		int getPolarity(int channelId);

		/** Activates/disables (1/0) specified channel. */
		void start(int channelId);
		void stop(int channelId);

		/** Activates/disables all (6) channels. */
		void startAll();
		void stopAll();

		/** Calculates and sets forward/reverse duty cycle ranges. */
		void setForwardRange(int min, int max);
		void setReverseRange(int min, int max);

		/** Returns forward/reverse duty cycle range. */
		int getForwardRange();
		int getReverseRange();

	private:

		void resetDuty();
		void resetPolarity();

		string getPinByChannelId(int channelId);

		int writeRawValue(string channel, string target, int value);
		int readRawValue(string channel, string target);

		// duty cycle ranges (initialized to defaults)
		int forwardRange = FORWARD_MAX - FORWARD_MIN;
		int reverseRange = REVERSE_MIN - REVERSE_MAX;
};


#endif
