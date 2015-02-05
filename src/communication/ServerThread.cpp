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
	printf("[KPI::THREAD] START\n");
	server->start();
	server->run();

	printf("[KPI::THREAD] END\n");

	return NULL;
}

int ServerThread::stop()
{
	printf("[KPI::THREAD] STOP\n");
	server->stop();
	return pthread_cancel( SERVER_THREAD_ID );
}

int ServerThread::kill()
{
	printf("[KPI::THREAD] KILL\n");
	return pthread_kill( SERVER_THREAD_ID , SIGQUIT );
}


