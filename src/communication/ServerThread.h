/*
 * ServerThread.h
 * Author: AUV Capstone
 */

#include "../sys/BaseThread.h"
#include "../sys/SocketServer.h"
#include <signal.h>
#include "../commons/Config.h"

#ifndef SRC_COMMUNICATION_SERVERTHREAD_H_
#define SRC_COMMUNICATION_SERVERTHREAD_H_


class ServerThread: public BaseThread
{

public:

	ServerThread(SocketServer* server);
	~ServerThread();
	void* run();				// overrides BaseThread's run() method

	int stop();
	int kill();

private:
	SocketServer* socketServer;
};

#endif /* SRC_COMMUNICATION_SERVERTHREAD_H_ */
