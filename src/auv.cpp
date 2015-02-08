//============================================================================
// Name        : auv.cpp
// Author      : zollder
//============================================================================



#include "communication/ServerThread.h"

using namespace std;

#define ESC 27

int main(int argc, char *argv[])
{
	printf("[KPI::MAIN] START\n");
	ServerThread* serverThread = new ServerThread( 5000, 2);
	ClientThread* clientThread = new ClientThread();

	FdTimer *delayTimer;
	delayTimer = new FdTimer(10, 3);


	serverThread->start();

	delayTimer->startSingle();
	delayTimer->waitTimerEvent();

	clientThread->start();

    char key;
    do
    {

        key = getchar();
        //clientThread->recv();

    }while(key != ESC);

    printf("[KPI::SERVER]::Escape Character Triggered\n");

    if( serverThread->stop() != 0 )
    {
    	printf("[KPI::MAIN] failed stop\n");

    	serverThread->kill();
    }
    //serverThread->join();

    delete serverThread;
    delete clientThread;
    delete delayTimer;

    printf("[KPI::MAIN] END\n");



    return EXIT_SUCCESS;
}
