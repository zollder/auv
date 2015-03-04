/*
 * ClientThread.cpp
 *
 *     Author: CAPSTONE Project AUV
 */

#include "ClientThread.h"

//-----------------------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------------------
ClientThread::ClientThread(SocketClient* client, int threadId, float interval)
{
	setThreadId(threadId);
	socketClient = client;
	flag = false;

	timer = new FdTimer(getThreadId(), interval);
}

//-----------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------
ClientThread::~ClientThread()
{
	flag = false;

    if(stop() != 0)
    {
    	syslog(LOG_NOTICE,"[KPI::CLIENT THREAD] failed stop");
    	kill();
    }

	delete socketClient;
}

//-----------------------------------------------------------------------------------------
// Overrides BaseThread's run() method
//-----------------------------------------------------------------------------------------
void* ClientThread::run()
{
	syslog(LOG_NOTICE,"[KPI::THREAD] START");

	flag = true;
	timer->start();

	while(flag)
	{
		timer->waitTimerEvent();

		socketClient->start();
		socketClient->recvMsg();
	}

	return NULL;
}

//-----------------------------------------------------------------------------------------
// Cleanup
//-----------------------------------------------------------------------------------------
int ClientThread::stop()
{
	syslog(LOG_NOTICE,"[KPI::CLIENT THREAD] STOP");
	return pthread_cancel(getThreadId());
}

int ClientThread::kill()
{
	syslog(LOG_NOTICE,"[KPI::CLIENT THREAD] KILL");
	return pthread_kill(getThreadId(), SIGQUIT);
}

