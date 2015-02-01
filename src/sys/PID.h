/*
 *	PID.h
 *  Created on: 25.01.2015
 *	Author: zollder
 *	Inspired by: https://github.com/vjaunet/QUADCOPTER
 */

#include <stdio.h>

using namespace std;

#ifndef pid_h
#define pid_h

/** Generic PID controller Interface. */
class PID
{
	public:

		/** Constructors. */
		PID();
		PID(float p, float i, float d);

		/** Destructor. */
		~PID();

		/** resets PID variables. */
		void reset();

		/** sets PID coefficients. */
		void setPidCoefficients(float p, float i, float d);

		/** sets boundary values */
		void setBoundaries(float min, float max);

		/** calculates and returns controller output. */
		float calculate(float setpoint, float input, float dt);

	private:

		/* PID controller coefficients */
		float Kp = 0;
		float Ki = 0;
		float Kd = 0;

		/* PID controller variables */
		float error = 0;
		float integError = 0;
		float derivError = 0;
		float input = 0;
		float lastInput = 0;
		float output = 0;

		/* boundary values */
		float min = 0;
		float max = 70;
};

#endif
