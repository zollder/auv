/*
 * ClientThread.h
 *
 *      Author: AUV Capstone
 */
#include "../sys/BaseThread.h"
#include "../sys/SocketClient.h"
#include <signal.h>

#ifndef SRC_COMMUNICATION_CLIENTTHREAD_H_
#define SRC_COMMUNICATION_CLIENTTHREAD_H_

#define CLIENT_THREAD_ID 9

class ClientThread : public BaseThread
{

public:
	ClientThread();
	ClientThread(int, char *);

	~ClientThread();

	void* run();				// overrides BaseThread's run() method

	void recv();

	int stop();
	int kill();

private:
	SocketClient* client;

};




#endif /* SRC_COMMUNICATION_CLIENTTHREAD_H_ */
