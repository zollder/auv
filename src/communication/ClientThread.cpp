/*
 * ClientThread.cpp
 *
 *     Author: CAPSTONE Project AUV
 */

#include "ClientThread.h"

//-----------------------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------------------
ClientThread::ClientThread()
{
	init( 5000, "127.0.0.1" );

}

ClientThread::ClientThread( int port , char *ip)
{
	init( port , ip );

}
//-----------------------------------------------------------------------------------------
// initialization of variables
//-----------------------------------------------------------------------------------------
void::ClientThread::init( int port , char *ip )
{
	flag=false;
	setThreadId( CLIENT_THREAD_ID );
	client = new SocketClient( port, ip );

}
//-----------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------
ClientThread::~ClientThread()
{
	flag=false;

    if( stop() != 0 )
    {
    	syslog(LOG_NOTICE,"[KPI::CLIENT THREAD] failed stop");
    	kill();
    }

	delete client;
}
//-----------------------------------------------------------------------------------------
// Cleanup
//-----------------------------------------------------------------------------------------
int ClientThread::stop()
{
	syslog(LOG_NOTICE,"[KPI::CLIENT THREAD] STOP");
	return pthread_cancel( CLIENT_THREAD_ID );

}

int ClientThread::kill()
{
	syslog(LOG_NOTICE,"[KPI::CLIENT THREAD] KILL");
	return pthread_kill( CLIENT_THREAD_ID , SIGQUIT );

}
//-----------------------------------------------------------------------------------------
// Overrides BaseThread's run() method
//-----------------------------------------------------------------------------------------
void* ClientThread::run()
{
	syslog(LOG_NOTICE,"[KPI::THREAD] START");

	flag=true;

	while(flag)
	{
		client->start();
		client->recvMsg();
		sleep(3);
	}

	return NULL;

}

