/*
 * SocketClient.cpp
 *
 *      Author: CAPSTONE Project AUV
 */

#include "SocketClient.h"

//-----------------------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------------------
SocketClient::SocketClient(DataService* service, int timerId, int port, char *ip)
{
	init(service, timerId, port, ip);
}
//-----------------------------------------------------------------------------------------
// initialization of variables
//-----------------------------------------------------------------------------------------
void SocketClient::init(DataService* service, int timerId, int port , char *ip)
{
	// initialize single-shot timer for retry logic
	retryTimer = new FdTimer(timerId, CLIENT_RETRY_INTERVAL);

	dataService = service;

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
	if (close(connfd) < 0)
		log->error("[ERROR] Failed to Close Socket Client");
	else
		log->info("[INFO] Socket Client Closed");

	connfd = -1;

	delete log;
	delete retryTimer;
}

//-----------------------------------------------------------------------------------------
/** Starts (creates) a socket inside the kernel and returns socket descriptor.
 * 	Implements retry logic with configurable interval and # of attempts. */
//-----------------------------------------------------------------------------------------
void SocketClient::start()
{
	bool retry = true;
	int counter = RETRY_COUNTER;

	while (retry)
	{
		//create socket inside the kernel and return socket descriptor
		connfd = socket(AF_INET, SOCK_STREAM, 0);

		if (connfd >= 0)
			retry = false;
		else if (connfd < 0 and counter >= 0)
		{
			counter--;
			printf("Failed to open a socket (attempt %d)\n", RETRY_COUNTER - counter);
			retryTimer->startSingle();
			retryTimer->waitTimerEvent();
		}
		else
		{
			log->error("[ERROR] Failed to Open Socket");
			exit(EXIT_FAILURE);
		}
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNumber);

	if (inet_pton(AF_INET, ipServer, &server_addr.sin_addr) <= 0)
	{
		log->error("[ERROR] Unable to open inet_pton");
		exit(EXIT_FAILURE);
	}
}

//-----------------------------------------------------------------------------------------
/** Connects to a remote server.
 *  Reads a message (array of floats) from the server.
 *  Closes the connection upon successful read. */
//-----------------------------------------------------------------------------------------
void SocketClient::recvMsg()
{
	//Attempt to connect to the server
	int result = connect(connfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if(result < 0)
		log->error("[ERROR] Failed to Connect to Server");
		//should not terminate program but continuously retry versus exit(EXIT_FAILURE);
	else
		while ((numBytes = recv(connfd, recBuff, sizeof(recBuff), MSG_WAITALL)) > 0)
		{
			dataService->saveData(recBuff);
		}

	//Cleanup
	close(connfd);
	connfd= -1;
}
