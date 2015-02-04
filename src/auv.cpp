//============================================================================
// Name        : auv.cpp
// Author      : zollder
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "SocketServer.h"


#define ESC 27
bool flag=true;

using namespace std;


int main(int argc, char *argv[])
{
	printf("[KPI::MAIN] START\n");

    char key;
    do
    {

        key = getchar();

    }while(key != ESC);
    printf("[KPI::SERVER]::Escape Character Triggered\n");


    printf("[KPI::MAIN] END\n");

    return EXIT_SUCCESS;
}

