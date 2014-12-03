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
		printf("Destroying I2C ...\n");
	}

	//-----------------------------------------------------------------------------------------
	/** Initializes all modules. */
	//-----------------------------------------------------------------------------------------
	void PWM::initialize()
	{
//		this->stopAll();
		this->setPeriod(1, 0);
		this->setPeriod(2, 0);
		this->setPeriod(3, 0);
//		this->resetDuty();
//		this->resetPolarity();
	}

	//-----------------------------------------------------------------------------------------
	/** Initializes selected module. */
	//-----------------------------------------------------------------------------------------
	void PWM::initialize(int moduleId)
	{
		this->setPeriod(moduleId, 0);
	}

	//-----------------------------------------------------------------------------------------
	/** Initializes selected modules. Use initialize() to initialize all 3 modules. */
	//-----------------------------------------------------------------------------------------
	void PWM::initialize(int id1, int id2)
	{
		this->setPeriod(id1, 0);
		this->setPeriod(id2, 0);
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

		// convert to nanoseconds, if not zero
		int convertedValue = 0;
		if (value != 0)
			convertedValue = 1000000000/value;

		switch (moduleId)
		{
			case 1:
				writeRawValue(P9_29, PERIOD, convertedValue);
				writeRawValue(P9_31, PERIOD, convertedValue);
				break;
			case 2:
				writeRawValue(P9_14, PERIOD, convertedValue);
				writeRawValue(P9_16, PERIOD, convertedValue);
				break;
			case 3:
				writeRawValue(P8_13, PERIOD, convertedValue);
				writeRawValue(P8_19, PERIOD, convertedValue);
				break;
			default:
				printf("Unsupported module id: %d.\n", moduleId);
		};

	}

	int PWM::getPeriod(int moduleId)
	{
		int rawValue = 0;
		switch (moduleId)
		{
			// read one channel per module, as it is assumed that both share the same period
			case 1:
				rawValue = readRawValue(P9_29, PERIOD);
				break;
			case 2:
				rawValue = readRawValue(P9_14, PERIOD);
				break;
			case 3:
				rawValue = readRawValue(P8_13, PERIOD);
				break;
			default:
				printf("Unsupported module id: %d.\n", moduleId);
		};

		// convert to HZ
		if (rawValue >= 0)
			return 1000000000/rawValue;
		else
			printf("Error reading period, moduleId:%d.\n", moduleId);

		return -1;
	}

	void PWM::setDuty(int moduleId, int value)
	{

	}

	int PWM::getDuty(int moduleId)
	{
		return -1;
	}

	void PWM::setPolarity(int channelId, int value)
	{

	}

	int PWM::getPolarity(int channelId)
	{
		return -1;
	}

	void PWM::start(int channelId)
	{

	}

	void PWM::startAll()
	{

	}

	void PWM::stop(int channelId)
	{

	}

	int PWM::readRawValue(string channel, string target)
	{
		int readValue = -1;
		ifstream file;

		file.open(PATH + channel + target, ios::in);

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
			printf("Period successfully read, channel:%s, value:%d\n", channel.c_str(), readValue);
		}

		return readValue;
	}

	void PWM::writeRawValue(string channel, string target, int value)
	{
		ofstream file;
		file.open(PATH + channel + target, ios::out);

		if(file.fail())
		{
			file.close();
			printf("Error check failed on write, channel:%s, target:%s\n", channel.c_str(), target.c_str());
			return;
		}
		else
		{
			file << value;
			file.close();
			printf("Period successfully set, channel:%s, value:%d\n", channel.c_str(), value);
		}
	}


