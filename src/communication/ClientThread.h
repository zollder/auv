/*
 * ClientThread.h
 *
 *      Author: CAPSTONE Project AUV
 */
#include "../sys/BaseThread.h"
#include "../sys/SocketClient.h"
#include <signal.h>

#ifndef SRC_COMMUNICATION_CLIENTTHREAD_H_
#define SRC_COMMUNICATION_CLIENTTHREAD_H_

#define CLIENT_THREAD_ID 10

class ClientThread : public BaseThread
{

public:
	ClientThread();
	ClientThread(int, char *);

	~ClientThread();

	// overrides BaseThread's run() method
	void* run();

	void init(int , char *);


	int stop();
	int kill();

	bool flag;

private:
	SocketClient* client;

};
#endif /* SRC_COMMUNICATION_CLIENTTHREAD_H_ */
