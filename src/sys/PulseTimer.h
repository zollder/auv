#include <cstdlib>
#include <iostream>
#include "time.h"
#include "signal.h"
//#include <sys/time.h>
#include <sys/timerfd.h>

// include -lmath linker option at compile time to avoid "undefined reference" error
#include <math.h>

#ifndef pulsetimer_h
#define pulsetimer_h

// pulse from timer (check the pulse's code value upon message receipt)
#define PULSE_FROM_TIMER 1

//-----------------------------------------------------------------------------------------
// PulseTimer interface.
//-----------------------------------------------------------------------------------------
class PulseTimer
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		PulseTimer(double);

		// destructor
		~PulseTimer();

		int initialize();

		// starts timer
		int start();

		// stops timer
		int stop();

		// (re)initializes the guts of the timer structure
		void reset();

		// returns a handle to the current timer instance (itself)
		timer_t getTimerId();

		// returns connection id
		int getConnectionId();

		// returns seconds portion of the time interval
		long getSeconds();

		// returns nanoseconds portion of the time interval
		long getNanoseconds();

		// returns timer running state
		bool isRunning();

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:

		timer_t timerId;
		struct itimerspec timer;
		struct sigevent event;

		long seconds;
		long nanoseconds;

		bool running;

	//-----------------------------------------------------------------------------------------
	// Protected members
	//-----------------------------------------------------------------------------------------
	protected:

		// sets seconds portion of the time interval
		void setInterval(double interval);

		// creates timer
		void createTimer();

		// sets timer's running flag (1=true, 0=false)
		void setRunning(bool);
};

#endif
