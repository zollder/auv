/*
 *	FdTimer.h
 *  Created on: 11.12.2014
 *	Author: zollder
 *	Inspired by: http://www.2net.co.uk/tutorial/periodic_threads#ref_1
 */

#include <cstdlib>
#include <iostream>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <unistd.h>

// include -lmath linker option at compile time to avoid "undefined reference" error
#include <math.h>

#ifndef fdtimer_h
#define fdtimer_h

//-----------------------------------------------------------------------------------------
// FdTimer interface.
//-----------------------------------------------------------------------------------------
class FdTimer
{
	// periodic timer info holder
	struct TimerInfo
	{
		int descriptor;
		unsigned long long missedEvents;
	};

	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:


		// constructor
		FdTimer(int, double);

		// destructor
		~FdTimer();

		// starts timer
		int start();

		// stops timer
		int stop();

		void waitTimerEvent();

		// (re)initializes the guts of the timer structure
		void reset();

		// returns seconds portion of the time interval
		long getSeconds();

		// returns nanoseconds portion of the time interval
		long getNanoseconds();

		// returns timer info data holder
		TimerInfo getTimerInfo();

		int getTimerId();

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:

		int timerId;

		TimerInfo timerInfo;
		int result;
		int timerRef;
		struct itimerspec timer;

		long seconds;
		long nanoseconds;

	//-----------------------------------------------------------------------------------------
	// Protected members
	//-----------------------------------------------------------------------------------------
	protected:

		// sets seconds and nanoseconds portions of the time interval
		void setInterval(double interval);

		// creates timer
		void createTimer();
};

#endif
