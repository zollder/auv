//============================================================================
// Name        : auv.cpp
// Author      : zollder
// Copyright   :
// Description :
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


#define ESC 27
bool flag=true;

static void* server_run();
int client_run( char* , unsigned short int);


using namespace std;


int main(int argc, char *argv[]) {

    unsigned short int port = 5000;


    pthread_t thread1;
    pthread_t t2;

    const char *message1 = "Server Thread";

    //Server Start
    int server = pthread_create( &thread1, NULL, server_run, (void*) message1 );
    if (server == 0) {
        printf("[KPI::SERVER]:Thread started\n");
    }
    else
    {
        printf("[KPI::SERVER]:Thread Did not start\n");
        return EXIT_FAILURE;
    }


    //Client Start
    if(argc == 2)
    {
        client_run( argv[1], atoi(argv[2]) );
        //printf("\nUsage: %s <ip of server> <port of server\n",argv[0]);
        //return EXIT_FAILURE;
    }
    else
    {
        client_run( address, port );
    }



    char key;
    do
    {

        key = getchar();

    }while(key != ESC);
    printf("[KPI::SERVER]::Escape Character Triggered\n");

    server = pthread_join(thread1, NULL);

    return EXIT_SUCCESS;


}

static void* server_run()
{

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));




    listen(listenfd, 10);

    while (flag) {
        connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff));

        close(connfd);
        sleep(1);
    }

    close(listenfd);

}


int client_run( char* address, unsigned short int port )
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;



    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return EXIT_FAILURE;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return EXIT_FAILURE;
    }

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return EXIT_FAILURE;
    }

    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    }

    if(n < 0)
    {
        printf("\n Read error \n");
    }

    return EXIT_SUCCESS;
}
