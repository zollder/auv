/*
 * SocketClient.h
 *
 *      Author: AUV Capstone Project
 */

#ifndef SRC_SYS_SOCKETCLIENT_H_
#define SRC_SYS_SOCKETCLIENT_H_


#include <stdio.h>
#include <stdlib.h>		//EXIT FUNCTION
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <syslog.h>
#include <string.h>		//memset, strlen
#include <unistd.h>		//close

#include "Logger.h"

/**
 * The Client program
 * The Client is used to receive the corresponding Data back to from the server through a socket connection.
 **/

class SocketClient {

public:

	SocketClient();					//Default Server config
	SocketClient(int, char *);		//Standard Server config( portNumber, IP Address )

	~SocketClient();

	void recvMsg();					//Receive Data from Server
	void start();			//initializes the client


private:

	int connfd;						//client connection descriptor
	int portNumber;					//listening port number
	char recvBuff[1024];
	int numBytes;
	char *ipServer;

	struct sockaddr_in server_addr;	//server address



	Logger *log;
};



#endif /* SRC_SYS_SOCKETCLIENT_H_ */
