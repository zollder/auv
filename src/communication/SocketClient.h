/*
 * SocketClient.h
 *
 *      Author: CAPSTONE Project AUV
 */

#include "../data/DataService.h"
#include "../sys/FdTimer.h"
#include "../commons/Config.h"

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
#include "../sys/Logger.h"		//syslog

/**
 * SocketClient interface.
 * Is used to pull messages from the server and save their content in the corresponding data holder.
 */
class SocketClient {

public:

	// Constructor
	SocketClient(DataService* dataService, int timerId, int serverPort, char* serverIp);

	~SocketClient();

	void recvMsg();					//Pull Rx Data from Server
	void start();					//initializes the client
	void init(DataService* service, int timerId, int serverPort, char* serverIp);

private:

	DataService* dataService;
	FdTimer* retryTimer;

	int connfd;						//Client connection descriptor
	int portNumber;					//port number used for Connecting to server
	float recBuff[15];				//Store DataHolder info
	int numBytes;					// Bytes message received.
	char *ipServer;					//Destination IP

	struct sockaddr_in server_addr;	//Socket server holder
	Logger *log;					//syslog wrapper
};

#endif /* SRC_SYS_SOCKETCLIENT_H_ */
