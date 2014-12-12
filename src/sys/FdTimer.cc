/*
 *	FdTimer.cpp
 *  Created on: 11.12.2014
 *	Author: zollder
 *	Inspired by: http://www.2net.co.uk/tutorial/periodic_threads#ref_1
 */

#include "FdTimer.h"

//---------------------------------------------------------------------------------------------
// Periodic FdTimer class implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	FdTimer::FdTimer(double interval)
	{
		printf("Creating and initializing FdTimer ...\n");

		// convert interval to seconds and nanoseconds and initialize corresponding members
		setInterval(interval);

		// initialize notification structure and create timer
		createTimer();
	}

	//-----------------------------------------------------------------------------------------
	// Destructor.
	//-----------------------------------------------------------------------------------------
	FdTimer::~FdTimer()
	{
		printf("Destroying FdTimer ...\n");
	}

	//-----------------------------------------------------------------------------------------
	/** Converts specified time interval into seconds and nanoseconds. */
	//-----------------------------------------------------------------------------------------
	void FdTimer::setInterval(double interval)
	{
		seconds = floor(interval);
		nanoseconds = (interval - seconds)*pow(10,9);
	}

	//-----------------------------------------------------------------------------------------
	/** Creates timer object.
	 *  Returns and saves a timer file descriptor reference (timerRef). */
	//-----------------------------------------------------------------------------------------
	void FdTimer::createTimer()
	{
		// CLOCK_MONOTONIC - not affected by discontinuous changes in the system clock
		// TFD_CLOEXEC - sets the close-on-exec flag (see timerfd doc for details)
		timerRef = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
		if (timerRef < 0)
		{
			printf("Timer creation error \n");
			exit(EXIT_FAILURE);
		}

		// initialize timer info holder
		info.descriptor = timerRef;
		info.missedEvents = 0;

		printf("Timer created successfully \n");
	}

	//-----------------------------------------------------------------------------------------
	/** Starts timer and updates its running status. */
	//-----------------------------------------------------------------------------------------
	int FdTimer::start()
	{
		// (re)initializes timer structure
		reset();

		// start the timer and running status accordingly
		int result = timerfd_settime (timerRef, 0, &timer, NULL);
		if (result != 0)
		{
			printf("Error creating timer.\n");
			exit(EXIT_FAILURE);
		}
		else
			printf("Timer started.\n");

		return result;
	}

	//-----------------------------------------------------------------------------------------
	/** Stops timer by nullifying its timer values and updating active system timer. */
	//-----------------------------------------------------------------------------------------
	int FdTimer::stop()
	{
		timer.it_value.tv_sec = 0;
		timer.it_value.tv_nsec = 0;
		timer.it_interval.tv_sec = 0;
		timer.it_interval.tv_nsec = 0;

		int result = timerfd_settime (timerRef, 0, &timer, NULL);
		if (result != 0)
			printf("Error stopping timer.\n");
		else
			printf("Timer stopped.\n");

		return result;
	}

	//-----------------------------------------------------------------------------------------
	/** (Re)Initializes the guts of the timer structure.*/
	//-----------------------------------------------------------------------------------------
	void FdTimer::reset()
	{
		// periodic timer: will go off in 1 sec and then again every sec+nanosec
		timer.it_value.tv_sec = 1;
		timer.it_value.tv_nsec = 0;
		timer.it_interval.tv_sec = getSeconds();
		timer.it_interval.tv_nsec = getNanoseconds();
	}

	//-----------------------------------------------------------------------------------------
	/** Returns seconds portion (converted) of the timer interval.*/
	//-----------------------------------------------------------------------------------------
	long FdTimer::getSeconds()
	{
		return seconds;
	}

	//-----------------------------------------------------------------------------------------
	/** Returns nanoseconds portion (converted) of the timer interval.*/
	//-----------------------------------------------------------------------------------------
	long FdTimer::getNanoseconds()
	{
		return nanoseconds;
	}
