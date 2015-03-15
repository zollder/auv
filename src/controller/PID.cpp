/*
 *	PID.cpp
 *  Created on: 25.01.2015
 *	Author: zollder
 */
#include "PID.h"

	//-----------------------------------------------------------------------------------------
	/** Constructor. */
	//-----------------------------------------------------------------------------------------
	PID::PID()
	{
		printf("Constructing PID ...\n");
	}

	//-----------------------------------------------------------------------------------------
	/** Overloaded constructor.
	 *  Initializes controller coefficients*/
	//-----------------------------------------------------------------------------------------
	PID::PID(float p, float i, float d)
	{
		printf("Constructing PID ...\n");
		Kp = p;
		Ki = i;
		Kd = d;
	}

	//-----------------------------------------------------------------------------------------
	/** Destructor. */
	//-----------------------------------------------------------------------------------------
	PID::~PID()
	{
		printf("Destroying PID ...\n");
	}


	//-----------------------------------------------------------------------------------------
	/** Resets PID variables. */
	//-----------------------------------------------------------------------------------------
	void PID::reset()
	{
		error = 0;
		integError = 0;
		derivError = 0;
		input = 0;
		lastInput = 0;
		output = 0;
	}

	//-----------------------------------------------------------------------------------------
	/** Sets PID coefficients. */
	//-----------------------------------------------------------------------------------------
	void PID::setPidCoefficients(float p, float i, float d)
	{
		Kp = p;
		Ki = i;
		Kd = d;
	}

	//-----------------------------------------------------------------------------------------
	/** Sets PID coefficients. */
	//-----------------------------------------------------------------------------------------
	void PID::setLimits(int lower, int upper)
	{
		min = lower;
		max = upper;
	}

	//-----------------------------------------------------------------------------------------
	/** Calculates PID output based on PID input and current desired value (setpoint).
	 *  Updates cumulative PID variables.
	 *  Handles "derivative kick" effect (http://www.controlguru.com/wp/p76.html)
	 *  Returns calculated control signal value.
	 */
	//-----------------------------------------------------------------------------------------
	int PID::calculate(float setpoint, float input, float dt)
	{
		// calculate current controller error (proportional term)
		error = setpoint - input;

		// find integral term
		integError = integError + (Ki * error * dt);

		// find derivative term, use derivative on measurement to avoid derivative kick
		derivError = -Kd * (input - lastInput) / dt;
		lastInput = input;

		// calculate control signal and verify the boundaries
		output = (Kp * error) + integError + derivError;
		if (output > max)
		{
			output = max;
			integError = 0;
		}
		else if (output < min)
		{
			output = min;
			integError = 0;
		}

		printf("Duty: %5d, E: %5.0f, IE: %5.0f, DE: %5.0f\n", (int) output, error, integError, derivError);

		return (int) output;
	}

