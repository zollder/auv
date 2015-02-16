/*
 *	PWM.cpp
 *  Created on: 30.11.2014
 *	Author: zollder
 */
#include "PWM.h"

	//-----------------------------------------------------------------------------------------
	/** Constructor. */
	//-----------------------------------------------------------------------------------------
	PWM::PWM()
	{
		printf("Constructing PWM ...\n");
	}

	//-----------------------------------------------------------------------------------------
	/** Destructor. */
	//-----------------------------------------------------------------------------------------
	PWM::~PWM()
	{
		printf("Destroying PWM ...\n");
	}

	//-----------------------------------------------------------------------------------------
	/** Initializes all modules. */
	//-----------------------------------------------------------------------------------------
	void PWM::initialize()
	{
		this->stopAll();
		this->setPeriod(1, DEFAULT_PERIOD_HZ);
		this->setPeriod(2, DEFAULT_PERIOD_HZ);
		this->setPeriod(3, DEFAULT_PERIOD_HZ);
		this->resetDuty();
		this->resetPolarity();
	}

	//-----------------------------------------------------------------------------------------
	/** Initializes selected module. */
	//-----------------------------------------------------------------------------------------
	void PWM::initialize(int moduleId)
	{
		this->setPeriod(moduleId, DEFAULT_PERIOD_HZ);
	}

	//-----------------------------------------------------------------------------------------
	/** Initializes selected modules. Use initialize() to initialize all 3 modules. */
	//-----------------------------------------------------------------------------------------
	void PWM::initialize(int id1, int id2)
	{
		this->setPeriod(id1, DEFAULT_PERIOD_HZ);
		this->setPeriod(id2, DEFAULT_PERIOD_HZ);
	}

	//-----------------------------------------------------------------------------------------
	/** Sets period (frequency) value in Hz.
	 *  Converts (Hz) to (ns) and writes the value to the corresponding file. */
	//-----------------------------------------------------------------------------------------
	void PWM::setPeriod(int moduleId, int value)
	{
		if (value < 0 or value > 1000)
		{
			printf("Supported frequencies: 0-1KHz.\n");
			exit(1);
		}

		// convert from HZ to nanoseconds, if not zero
		int convertedValue = 0;
		if (value != 0)
			convertedValue = CONVERSION_CONST/value;

		// stop channel first and set period after
		switch (moduleId)
		{
			case 1:
				writeRawValue(P9_29, RUN, 0);
				writeRawValue(P9_31, RUN, 0);
				writeRawValue(P9_29, PERIOD, convertedValue);
				writeRawValue(P9_31, PERIOD, convertedValue);
				break;
			case 2:
				writeRawValue(P9_14, RUN, 0);
				writeRawValue(P9_16, RUN, 0);
				writeRawValue(P9_14, PERIOD, convertedValue);
				writeRawValue(P9_16, PERIOD, convertedValue);
				break;
			case 3:
				writeRawValue(P8_13, RUN, 0);
				writeRawValue(P8_19, RUN, 0);
				writeRawValue(P8_13, PERIOD, convertedValue);
				writeRawValue(P8_19, PERIOD, convertedValue);
				break;
			default:
				printf("Unsupported module id: %d.\n", moduleId);
		};

	}

	//-----------------------------------------------------------------------------------------
	/** Returns period (frequency) value in Hz for specified module ID. */
	//-----------------------------------------------------------------------------------------
	int PWM::getPeriodHz(int channelId)
	{
		string channelString = getPinByChannelId(channelId);
		int rawValue = readRawValue(channelString.c_str(), PERIOD);

		// convert to HZ
		if (rawValue < 0)
		{
			printf("Error reading period, moduleId:%d.\n", channelId);
			return rawValue;
		}

		return CONVERSION_CONST/rawValue;

	}

	//-----------------------------------------------------------------------------------------
	/** Returns period (frequency) value in nanoseconds for specified module ID. */
	//-----------------------------------------------------------------------------------------
	int PWM::getPeriodNs(int channelId)
	{
		return readRawValue(getPinByChannelId(channelId), PERIOD);
	}

	//-----------------------------------------------------------------------------------------
	/** Sets duty cycle value in %.
	 *  Converts (%) to (ns) and writes the value to the corresponding file. */
	//-----------------------------------------------------------------------------------------
	void PWM::setDuty(int channelId, int value)
	{
		// convert from percents to nanoseconds, if not zero
		int dutyValue = 0;
		if (value >= 0 and value <= 100)
			dutyValue = (getPeriodNs(channelId)/100) * value;
		else
			printf("Invalid duty value: %d.\n", value);

		writeRawValue(getPinByChannelId(channelId), DUTY, dutyValue);
	}

	//-----------------------------------------------------------------------------------------
	/** Returns a duty cycle value in % for specified channel ID. */
	//-----------------------------------------------------------------------------------------
	int PWM::getDuty(int channelId)
	{
		int dutyValue = readRawValue(getPinByChannelId(channelId), DUTY);
		int periodValue = getPeriodNs(channelId);

		return (dutyValue/periodValue) * 100;
	}

	//-----------------------------------------------------------------------------------------
	/** Returns a duty cycle value in % for specified channel ID. */
	//-----------------------------------------------------------------------------------------
	void PWM::resetDuty()
	{
		int status[6];
		status[0] = writeRawValue(P9_29, DUTY, 0);
		status[1] = writeRawValue(P9_31, DUTY, 0);
		status[2] = writeRawValue(P9_14, DUTY, 0);
		status[3] = writeRawValue(P9_16, DUTY, 0);
		status[4] = writeRawValue(P8_13, DUTY, 0);
		status[5] = writeRawValue(P8_19, DUTY, 0);

		for (int i = 0; i < 6; i++)
		{
			if (status[i] < 0)
				printf("ResetDuty: error resetting channel's duty cycle. Index:%d\n", i);
		}
	}

	//-----------------------------------------------------------------------------------------
	/** Sets PWM polarity for specified channel ID. */
	//-----------------------------------------------------------------------------------------
	void PWM::setPolarity(int channelId, int value)
	{
		int status = writeRawValue(getPinByChannelId(channelId), POLARITY, value);
		if (status < 0)
			printf("Error setting polarity. Channel ID:%d\n", channelId);
	}

	//-----------------------------------------------------------------------------------------
	/** Returns PWM polarity for specified channel ID. */
	//-----------------------------------------------------------------------------------------
	int PWM::getPolarity(int channelId)
	{
		int polarity = readRawValue(getPinByChannelId(channelId), POLARITY);
		if (polarity < 0)
			printf("Error reading polarity. Channel ID:%d\n", channelId);

		return polarity;
	}

	//-----------------------------------------------------------------------------------------
	/** Returns a duty cycle value in % for specified channel ID. */
	//-----------------------------------------------------------------------------------------
	void PWM::resetPolarity()
	{
		int status[6];
		status[0] = writeRawValue(P9_29, POLARITY, 1);
		status[1] = writeRawValue(P9_31, POLARITY, 1);
		status[2] = writeRawValue(P9_14, POLARITY, 1);
		status[3] = writeRawValue(P9_16, POLARITY, 1);
		status[4] = writeRawValue(P8_13, POLARITY, 1);
		status[5] = writeRawValue(P8_19, POLARITY, 1);

		for (int i = 0; i < 6; i++)
		{
			if (status[i] < 0)
				printf("ResetPolarity: error resetting channel's polarity. Index:%d\n", i);
		}
	}

	//-----------------------------------------------------------------------------------------
	/** Activates/starts the specified channel. */
	//-----------------------------------------------------------------------------------------
	void PWM::start(int channelId)
	{
		int status = writeRawValue(getPinByChannelId(channelId), RUN, 1);
		if (status < 0)
			printf("Error starting the channel. Channel ID:%d\n", channelId);
	}

	//-----------------------------------------------------------------------------------------
	/** Activates/starts all 6 PWM channels. */
	//-----------------------------------------------------------------------------------------
	void PWM::startAll()
	{
		int status[6];
		status[0] = writeRawValue(P9_29, RUN, 1);
		status[1] = writeRawValue(P9_31, RUN, 1);
		status[2] = writeRawValue(P9_14, RUN, 1);
		status[3] = writeRawValue(P9_16, RUN, 1);
		status[4] = writeRawValue(P8_13, RUN, 1);
		status[5] = writeRawValue(P8_19, RUN, 1);

		for (int i = 0; i < 6; i++)
		{
			if (status[i] < 0)
				printf("StartAll: error starting the channel. Index:%d\n", i);
		}
	}

	//-----------------------------------------------------------------------------------------
	/** Deactivates/stops the specified channel. */
	//-----------------------------------------------------------------------------------------
	void PWM::stop(int channelId)
	{
		int status = writeRawValue(getPinByChannelId(channelId), RUN, 0);
		if (status < 0)
			printf("Error stopping the channel. Channel ID:%d\n", channelId);
	}

	//-----------------------------------------------------------------------------------------
	/** Activates/starts all 6 PWM channels. */
	//-----------------------------------------------------------------------------------------
	void PWM::stopAll()
	{
		int status[6];
		status[0] = writeRawValue(P9_29, RUN, 0);
		status[1] = writeRawValue(P9_31, RUN, 0);
		status[2] = writeRawValue(P9_14, RUN, 0);
		status[3] = writeRawValue(P9_16, RUN, 0);
		status[4] = writeRawValue(P8_13, RUN, 0);
		status[5] = writeRawValue(P8_19, RUN, 0);

		for (int i = 0; i < 6; i++)
		{
			if (status[i] < 0)
				printf("StartAll: error starting the channel. Index:%d\n", i);
		}
	}

	//-----------------------------------------------------------------------------------------
	/** Returns current raw value for the specified channel ID and target parameter (file). */
	//-----------------------------------------------------------------------------------------
	int PWM::readRawValue(string channel, string target)
	{
		int readValue = -1;
		ifstream file;

		file.open((PATH + channel + target).c_str() , ios::in);

		if(file.fail())
		{
			file.close();
			printf("Error check failed on read, channel:%s, target:%s\n", channel.c_str(), target.c_str());
			return readValue;
		}
		else
		{
			file >> readValue;
			file.close();
			printf("%s successfully read, channel:%s, value:%d\n", target.c_str(), channel.c_str(), readValue);
		}

		return readValue;
	}

	//-----------------------------------------------------------------------------------------
	/** Writes a value to target parameter (file) for specified channel ID. */
	//-----------------------------------------------------------------------------------------
	int PWM::writeRawValue(string channel, string target, int value)
	{
		ofstream file;
		file.open((PATH + channel + target).c_str() , ios::in);

		if(file.fail())
		{
			file.close();
			printf("Error check failed on write, channel:%s, target:%s\n", channel.c_str(), target.c_str());
			return -1;
		}

		file << value;
		file.close();
		printf("%s successfully set, channel:%s, value:%d\n", target.c_str(), channel.c_str(), value);

		return 0;
	}

	//-----------------------------------------------------------------------------------------
	/** Returns BBB pin string by channel ID. */
	//-----------------------------------------------------------------------------------------
	string PWM::getPinByChannelId(int channelId)
	{
		switch (channelId)
		{
			case 11:
				return P9_29;
			case 12:
				return P9_31;
			case 21:
				return P9_14;
			case 22:
				return P9_16;
			case 31:
				return P8_13;
			case 32:
				return P8_19;
			default:
				printf("Unsupported channel id: %d.\n", channelId);
		}

		return "unsupported";
	}

