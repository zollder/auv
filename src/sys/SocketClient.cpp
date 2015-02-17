/*
 * SocketClient.cpp
 *
 *      Author: CAPSTONE Project AUV
 */

#include "SocketClient.h"

//-----------------------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------------------
SocketClient::SocketClient()
{
	init( 5000, "127.0.0.1" );

}

SocketClient::SocketClient(int port, char *ip )
{
	init( port , ip);

}
//-----------------------------------------------------------------------------------------
// initialization of variables
//-----------------------------------------------------------------------------------------
void SocketClient::init(int port , char *ip)
{
	portNumber = port;
	ipServer = ip;

	connfd = -1;
	numBytes = 0;

	//Syslog initialize
	log = new Logger("Socket Client [KPI]");

	//initialize and allocate memory
	memset(&server_addr, '0', sizeof(server_addr));
	memset(recBuff, '0',sizeof(recBuff));

}
//-----------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------
SocketClient::~SocketClient()
{
	if ( close(connfd) < 0)
		log->error("[ERROR] Failed to Close Socket Client");
	else
		log->info("[INFO] Socket Client Closed");

	connfd = -1;

	delete log;

}
//-----------------------------------------------------------------------------------------
// Class Execution
//-----------------------------------------------------------------------------------------
void SocketClient::start()
{

	//create socket inside the kernel and return socket descriptor
	connfd = socket(AF_INET, SOCK_STREAM, 0);

	if (connfd < 0)
	{
		log->error("[ERROR] Failed to Open Socket");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNumber);

	if( inet_pton(AF_INET, ipServer , &server_addr.sin_addr) <= 0 )
	{
		log->error("[ERROR] Unable to open inet_pton");
		exit(EXIT_FAILURE);
	}

}

void SocketClient::recvMsg()
{
	//Attempt to connect onto the server
	if( connect( connfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
	{
		log->error("[ERROR] Failed to Connect to Server");
		//should not terminate program but continuously retry versus exit(EXIT_FAILURE);
	}
	else
	{
		while ( ( numBytes = recv( connfd, recBuff, sizeof( recBuff ) , MSG_WAITALL ) ) > 0)
		{
			recBuff[numBytes] = 0;

	//Not necessary
	//		if(numBytes < 0)
	//			log->error("[ERROR] Read Error");

			for( int  x = 0; x < 15 ; x++ )
				printf("%.2f\t", recBuff[x]);
			printf("\n");

		}
	}

	//Cleanup
	close(connfd);
	connfd= -1;

}
