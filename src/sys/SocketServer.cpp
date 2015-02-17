/*
 * SocketServer.cpp
 *
 *      Author: AUV Capstone
 */
#include "SocketServer.h"

//-----------------------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------------------
SocketServer::SocketServer()
{
	init( 5000 , 2 );

}

SocketServer::SocketServer( int port, int max )
{
	init( port , max );

}

SocketServer::SocketServer( int port, int max , DataService *data)
{
	init( port , max );
	dataService = data;

}
//-----------------------------------------------------------------------------------------
// initialization of variables
//-----------------------------------------------------------------------------------------
void SocketServer::init( int port, int max)
{
	portNumber = port;
	maxUser = max;
	dataService = -1;

	// Initialize System Log
	log = new Logger("Socket Server [KPI]");

	//Initializing Socket data
	connfd = -1;
	sockfd = -1;

	//Initialize socket structures
	memset(&server_addr, '0', sizeof(server_addr));
	memset(&client_addr, '0', sizeof(client_addr));
}
//-----------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------
SocketServer::~SocketServer()
{
	stop();
}
//-----------------------------------------------------------------------------------------
// Class Execution
//-----------------------------------------------------------------------------------------
void SocketServer::start()
{
	if( dataService == -1)
	{
		log->error("[ERROR] No Data Holder passing, Not supported for anything else");
		exit(EXIT_FAILURE);
	}

	//create socket inside the kernel and return socket descriptor
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		log->error("[ERROR] Failed to Open Socket");
		exit(EXIT_FAILURE);
	}

	//initialize server structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(portNumber);

	client_len = sizeof(client_addr);

	//bind host address
	if ( bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr) ) < 0)
	{
		int delay=8;
		while( bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr) ) < 0 && --delay)
		{
			//Max Linux delay is 60 seconds by default.
			//creates TIME_WAIT for making sure all socket connection had chance to send data back or from server.
			sleep(10);
		}

		if( delay == 0)
		{
			log->error("[ERROR] Failed to Bind Socket");
			exit(EXIT_FAILURE);
		}
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
			log->error("[NOTICE] Invalid session descriptor");
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
			log->error("[NOTICE] Failed to Close Socket Server");
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

		if( connfd < 1)
		{
			log->error( "[ERROR] Failed to Accept Connection");
			close( connfd);
		}
		else
		{
			if( (send(connfd, dataService->getData(), dataService->getSize() ,0 ) ) < 0 )
				log->error( "[ERROR] Failed to Send Buffer to Socket");

			close( connfd);
		}

	}

	log->info( "[INFO] End Run");
}
