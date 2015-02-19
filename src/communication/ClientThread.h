/*
 * ClientThread.h
 *
 *      Author: CAPSTONE Project AUV
 */

#include <signal.h>

#include "../sys/BaseThread.h"
#include "../sys/SocketClient.h"
#include "../sys/FdTimer.h"
#include "../commons/Config.h"

#ifndef SRC_COMMUNICATION_CLIENTTHREAD_H_
#define SRC_COMMUNICATION_CLIENTTHREAD_H_

class ClientThread : public BaseThread
{
	public:

		ClientThread(SocketClient* client, float interval);
		~ClientThread();

		// overrides BaseThread's run() method
		void* run();

		int stop();
		int kill();

		bool flag;

	private:

		FdTimer* timer;
		SocketClient* socketClient;
};
#endif /* SRC_COMMUNICATION_CLIENTTHREAD_H_ */
