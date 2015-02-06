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

	serverThread->start();

    char key;
    do
    {

        key = getchar();

    }while(key != ESC);

    printf("[KPI::SERVER]::Escape Character Triggered\n");

    if( serverThread->stop() != 0 )
    {
    	printf("[KPI::MAIN] failed stop\n");

    	serverThread->kill();
    }
    //serverThread->join();

    delete serverThread;
    printf("[KPI::MAIN] END\n");

    return EXIT_SUCCESS;
}
