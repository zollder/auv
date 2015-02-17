/*
 * SocketClient.h
 *
 *      Author: CAPSTONE Project AUV
 */

#ifndef SRC_SYS_SOCKETCLIENT_H_
#define SRC_SYS_SOCKETCLIENT_H_


#include <stdio.h>		//printf
#include <stdlib.h>		//EXIT FUNCTION
#include <sys/types.h>	//Socket Class
#include <sys/socket.h>	//Socket Class
#include <netinet/in.h>	//Socket Class
#include <string.h>		//memset, strlen
#include <unistd.h>		//close
#include <arpa/inet.h>	//inet_pton functions
#include "Logger.h"		//syslog

/**
 * The Socket Client
 * The Client is used to Pull message from server to the corresponding Data using a socket connection.
 **/

class SocketClient {

public:

	SocketClient();					//Default Server config
	SocketClient(int, char *);		//Standard Server config( portNumber, IP Address )

	~SocketClient();

	void recvMsg();					//Pull Rx Data from Server
	void start();					//initializes the client
	void init( int , char *);		//Initialize Class


private:

	int connfd;						//Client connection descriptor
	int portNumber;					//port number used for Connecting to server
	float recBuff[15];				//Store DataHolder info
	int numBytes;					// Bytes message received.
	char *ipServer;					//Destination IP

	struct sockaddr_in server_addr;	//Socket server holder
	Logger *log;					//syslog wrapper
};



#endif /* SRC_SYS_SOCKETCLIENT_H_ */
