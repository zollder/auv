#ifndef SRC_SYS_SOCKETSERVER_H_
#define SRC_SYS_SOCKETSERVER_H_

#include <stdio.h>
#include <stdlib.h>		//EXIT FUNCTION
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>		//memset, strlen
#include <unistd.h>		//close

/** Dependencies */
#include "../commons/Config.h"
#include "../data/DataService.h"
#include "../sys/Logger.h"

/**
 * The server program
 * The Server is used to send corresponding Data back to the client through a socket connection.
 **/
//-----------------------------------------------------------------------------------------------------
class SocketServer
{
	public:

		SocketServer(DataService*);		//Default Server Config
		SocketServer(DataService*, int, int);	//DataService Server config( portNumber, maxConnection , Dataservice)

		~SocketServer();

		void run();							//start listening to port
		void start();						//initializes the server
		void stop();						//close Socket Server Process
		void init(DataService*, int, int);				//Initialize variables

	private:

		int sockfd = 0;						//server socket descriptor
		int connfd = 0;						//client connection descriptor
		int portNumber = 0;					//listening port number
		int maxUser = 0;					//maximum number of users connected
		int client_len = 0;					//length of the client IP

		struct sockaddr_in server_addr;	//server address
		struct sockaddr_in client_addr;	//client address

		Logger* logger;
		DataService* dataService;
};

#endif
