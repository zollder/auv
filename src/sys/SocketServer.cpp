/*
 * SocketServer.cpp
 *
 *      Author: AUV Capstone
 */
#include "SocketServer.h"

#include <time.h> 		//@TEST, can remove after

SocketServer::SocketServer()
{
	portNumber = 5000;
	maxUser = 2;

}

SocketServer::SocketServer( int port, int max )
{
	portNumber = port;
	maxUser = max;

}

SocketServer::SocketServer( int port, int max , DataService *data)
{
	portNumber = port;
	maxUser = max;
	dataService = data;

}

SocketServer::~SocketServer()
{
	stop();
}

void SocketServer::start()
{
	// Initialize System Log
	log = new Logger("Socket Server [KPI]");

	connfd = -1;
	sockfd = -1;

	//create socket inside the kernel and return socket descriptor
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		log->error("[ERROR] Failed to Open Socket");
		exit(EXIT_FAILURE);
	}

	//Initialize socket structures
	memset(&server_addr, '0', sizeof(server_addr));
	memset(&client_addr, '0', sizeof(client_addr));

	//Initialize buffer structures
	memset(sendBuff, '0', sizeof(sendBuff));

	//initialize server structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(portNumber);

	client_len = sizeof(client_addr);

	//bind host address
	if ( bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr) ) < 0)
	{
		log->error("[ERROR] Failed to Bind Socket");
		exit(EXIT_FAILURE);
	}

	log->info("[INFO] Socket Server initialized");
}

void SocketServer::stop()
{
	// Closing Active Session
	if (connfd < 0)
	{
		log->notice("[NOTICE] Session already Closed");
	}
	else
	{
		if ( shutdown(connfd, SHUT_RDWR) < 0)
			log->error("[DEBUG] Invalid session descriptor");
		else
			log->info("[INFO] Socket Session Closed");

		connfd = -1;
	}

	// Closing Server Socket
	if (sockfd < 0)
	{
		log->notice("[NOTICE] Socket Server already closed");
	}
	else
	{
		if ( close(sockfd) < 0)
			log->error("[ERROR] Failed to Close Socket Server");
		else
			log->info("[INFO] Socket Server Closed");

		sockfd = -1;
	}

	delete log;
}
void SocketServer::run()
{
	// Start Listening for clients
	listen( sockfd, maxUser );
	log->info("[INFO] Listening");

	while( sockfd > 1)
	{
		//wait for clients
		connfd = accept(sockfd, (struct sockaddr *) &client_addr, (socklen_t *)&client_len);
		//connfd = accept(sockfd, (struct sockaddr *) NULL, NULL);

		if( connfd < 1)
		{
			log->error( "[ERROR] Failed to Accept Connection");
			close( connfd);
		}
		else
		{
			//time_t ticks = time(NULL);
			//snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));


			//(send(connfd, dataService->getData(), strlen(sendBuff),0 ) )
			//sizeof( dataService->getData() )

			if( (send(connfd, dataService->getData(), sizeof( dataService->getData() ) ,0 ) ) < 0 )
				log->error( "[ERROR] Failed to Send Buffer to Socket");

			close( connfd);
		}

		log->info( "[DEBUG] End Send");
	}


	log->info( "[INFO] End Run");
}
