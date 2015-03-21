/*
 * ServerThread.h
 * Author: AUV Capstone
 */

#include <signal.h>

#include "../sys/BaseThread.h"
#include "../commons/Config.h"

#include "SocketServer.h"

#ifndef serverthread_h
#define serverthread_h

class ServerThread: public BaseThread
{
    public:

        ServerThread(SocketServer* server, int id);
        ~ServerThread();
        void* run();                // overrides BaseThread's run() method

        int stop();
        int kill();

    private:

        SocketServer* socketServer;
};

#endif
