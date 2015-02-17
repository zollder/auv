/*
 * Logger.h
 *
 *  Author: CAPSTONE Project AUV
 */

#ifndef SRC_SYS_LOGGER_H_
#define SRC_SYS_LOGGER_H_

#include <syslog.h>

class Logger {

public:

	Logger( char * );
	~Logger();

	void error( char *);
	void info( char * );
	void notice( char * );

private:


};



#endif /* SRC_SYS_LOGGER_H_ */
