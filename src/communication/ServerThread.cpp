/*
 * ServerThread.cpp
 *
 *      Author: AUV Capsone
 */


#include "ServerThread.h"

//-----------------------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------------------
ServerThread::ServerThread()
{
	init ( 5000 , 2 );
}

ServerThread::ServerThread(int port, int max)
{
	init( port , max );
}
ServerThread::ServerThread(int port, int max, DataService* dataService)
{
	setThreadId( SERVER_THREAD_ID );
	server = new SocketServer(port , max , dataService);

}
//-----------------------------------------------------------------------------------------
// initialization of variables
//-----------------------------------------------------------------------------------------
void ServerThread::init( int port , int max )
{
	setThreadId( SERVER_THREAD_ID );
	server = new SocketServer( port, max );

}
//-----------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------
ServerThread::~ServerThread()
{
	if( stop() != 0 )
	{
	    	syslog(LOG_NOTICE,"[KPI::CLIENT THREAD] failed stop");
	    	kill();
	}
	delete server;
}
//-----------------------------------------------------------------------------------------
// Overrides BaseThread's run() method
//-----------------------------------------------------------------------------------------
void* ServerThread::run()
{
	syslog(LOG_NOTICE,"[KPI::THREAD] START");
	server->start();
	server->run();

	syslog(LOG_NOTICE,"[KPI::THREAD] END");

	return NULL;
}

int ServerThread::stop()
{
	syslog(LOG_NOTICE,"[KPI::THREAD] STOP");
	return pthread_cancel( SERVER_THREAD_ID );
}

int ServerThread::kill()
{
	syslog(LOG_NOTICE,"[KPI::THREAD] KILL");
	return pthread_kill( SERVER_THREAD_ID , SIGQUIT );
}


