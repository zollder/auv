#ifndef SRC_SYS_SOCKETSERVER_H_
#define SRC_SYS_SOCKETSERVER_H_

#include <stdio.h>
#include <stdlib.h>		//EXIT FUNCTION
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>		//memset, strlen
#include <unistd.h>		//close
#include "../data/DataService.h"

#include "Logger.h"

/**
 * The server program
 * The Server is used to send that corresponding Data back to the client through a socket connection.
 **/
//-----------------------------------------------------------------------------------------------------
class SocketServer {

public:

	SocketServer();					//Default Server config
	SocketServer(int, int);			//Standard Server config( portNumber, maxConnection )
	SocketServer(int, int, DataService *);			//Standard Server config( portNumber, maxConnection )

	~SocketServer();

	void run();						//start listening to port
	void start();					//initializes the server
	void stop();					//close Socket Server Process

private:

	int sockfd;						//server socket descriptor
	int connfd;						//client connection descriptor
	int portNumber;					//listening port number
	int maxUser;					//maximum number of users connected
	int client_len;					//length of the client IP

	struct sockaddr_in server_addr;	//server address
	struct sockaddr_in client_addr;	//client address

	Logger* log;
	DataService* dataService;
};

/*
syslog Log Levels
 * #define LOG_EMERG
 * #define LOG_ALERT
 * #define LOG_CRIT
 * #define LOG_ERR
 * #define LOG_WARNING
 * #define LOG_NOTICE
 * #define LOG_INFO
 * #define LOG_DEBUG
*/

#endif /* SRC_SYS_SocketServer_H_ */
