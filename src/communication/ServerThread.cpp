/*
 * ServerThread.cpp
 *
 *      Author: AUV Capsone
 */


#include "ServerThread.h"

//-----------------------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------------------
ServerThread::ServerThread(SocketServer* server)
{
	setThreadId( SOCKET_SERVER_THREAD_ID );
	socketServer = server;
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
	socketServer->start();
	socketServer->run();

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


