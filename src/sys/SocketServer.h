#ifndef SOCKETSERVER_H_
#define SOCKETSERVER_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <syslog.h>


/**
 * The server program
 * The Server is used to send that corresponding Data back to the client through a socket connection.
 **/
//-----------------------------------------------------------------------------------------------------
class SocketServer {

public:

	SocketServer();					//Default Server config
	SocketServer(int, int);			//Standard Server config( portNumber, maxConnection )

	~SocketServer();

	void run();						//start listening to port

private:

	int sockfd;						//server socket descriptor
	int connfd;						//client connection descriptor
	int portNumber;					//listening port number
	int maxUser;					//maximum number of users connected
	int client_len;					//length of the client IP

	struct sockaddr_in server_addr;	//server address
	struct sockaddr_in client_addr;	//client address

	char sendBuff[256];

	void start();					//initializes the server
	void stop();					//close Socket Server Process


	const char *node = "Socket Server [KPI]";	//syslog tag for writer
	void logInit();					//syslog start
	void logClose();				//syslog stop

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

#endif /* SocketServer_H_ */
