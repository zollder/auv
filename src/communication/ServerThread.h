/*
 * ServerThread.h
 *
 *      Author: AUV Capstone
 */

#include "../sys/BaseThread.h"
#include "../sys/SocketServer.h"
#include <signal.h>

#ifndef SRC_COMMUNICATION_SERVERTHREAD_H_
#define SRC_COMMUNICATION_SERVERTHREAD_H_

#define SERVER_THREAD_ID 8

class ServerThread: public BaseThread
{

public:
	ServerThread();
	ServerThread(int,int);
	ServerThread(int,int, DataService* );

	~ServerThread();

	void* run();				// overrides BaseThread's run() method

	int stop();
	int kill();

private:
		SocketServer* server;

};



#endif /* SRC_COMMUNICATION_SERVERTHREAD_H_ */
