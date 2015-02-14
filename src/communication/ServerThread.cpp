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
	setThreadId( SERVER_THREAD_ID );
	server = new SocketServer();

}

ServerThread::ServerThread(int port, int max)
{
	setThreadId( SERVER_THREAD_ID );
	server = new SocketServer(port , max);

}
ServerThread::ServerThread(int port, int max, DataService* dataService)
{
	setThreadId( SERVER_THREAD_ID );
	server = new SocketServer(port , max , dataService);

}
//-----------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------
ServerThread::~ServerThread()
{
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
	server->stop();
	return pthread_cancel( SERVER_THREAD_ID );
}

int ServerThread::kill()
{
	syslog(LOG_NOTICE,"[KPI::THREAD] KILL");
	return pthread_kill( SERVER_THREAD_ID , SIGQUIT );
}


