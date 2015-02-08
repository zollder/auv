/*
 * SocketClient.cpp
 *
 *      Author: AUV Capstone
 */

#include "SocketClient.h"

//-----------------------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------------------
SocketClient::SocketClient()
{
	portNumber = 5000;
	ipServer = "127.0.0.1";
}

SocketClient::SocketClient(int port, char *ip )
{
	portNumber = port;
	ipServer = ip;

}
//-----------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------
SocketClient::~SocketClient()
{
	delete log;

}

void SocketClient::start()
{
	log = new Logger("Socket Client [KPI]");

	connfd = -1;
	memset(recvBuff, '0',sizeof(recvBuff));

	//create socket inside the kernel and return socket descriptor
	connfd = socket(AF_INET, SOCK_STREAM, 0);

	if (connfd < 0)
	{
		log->error("[ERROR] Failed to Open Socket");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, '0', sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNumber);

	if( inet_pton(AF_INET, ipServer , &server_addr.sin_addr) <=0 )
	{
		log->error("[ERROR] inet_pton to Open");
		exit(EXIT_FAILURE);
	}

}

void SocketClient::recvMsg()
{
	numBytes = 0;

	if( connect( connfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		log->error("[ERROR] Failed to Connect to Server");
		exit(EXIT_FAILURE);
	}

	while ( (numBytes = recv( connfd, recvBuff, sizeof(recvBuff)-1 , MSG_WAITALL ) ) > 0)
	{
		recvBuff[numBytes] = 0;
		if(fputs(recvBuff, stdout) == EOF)
			log->error("[ERROR] Fputs Failed");
		else if(numBytes < 0)
			    	log->error("[ERROR] Read Error");
		else
		{
			printf("Here is the message: %s\n",recvBuff );
			printf("Amount of Bytes received %d\n",numBytes );
		}

	}
}
