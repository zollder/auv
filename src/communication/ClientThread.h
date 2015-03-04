/*
 * ClientThread.h
 *
 *      Author: CAPSTONE Project AUV
 */

#include <signal.h>

#include "../sys/BaseThread.h"
#include "SocketClient.h"
#include "../sys/FdTimer.h"
#include "../commons/Config.h"

#ifndef clientthread_h
#define clientthread_h

class ClientThread : public BaseThread
{
	public:

		ClientThread(SocketClient* client, int id, float interval);
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
#endif
