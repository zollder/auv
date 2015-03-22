//============================================================================
// Name        : auv.cpp
// Author      : Main Program Execution Capstone AUV
//============================================================================

#include "communication/SocketClient.h"
#include "communication/SocketServer.h"
#include "communication/ServerThread.h"
#include "communication/ClientThread.h"

#include "controller/MasterController.h"
#include "dmu/DMU.h"
#include "data/DataService.h"
#include "data/SensorData.h"
#include "data/DesiredData.h"
#include "data/CameraData.h"

using namespace std;

int main(int argc, char *argv[])
{
	syslog(LOG_NOTICE,"[KPI::MAIN] START");

	SensorData* sensorData = new SensorData();
	DesiredData* targetData = new DesiredData();
	CameraData* camData = new CameraData();
	DataService* dataService = new DataService(sensorData, targetData, camData);

	SocketServer* socketServer = new SocketServer(dataService, PORT, CLIENTS);
	SocketClient* clientFront = new SocketClient(dataService, RETRY_TIMER_ID, PORT, U3_FRONT_IP);
	SocketClient* clientBottom = new SocketClient(dataService, RETRY_TIMER_ID, PORT, U3_BOTTOM_IP);

	ServerThread* socketServerThread = new ServerThread(socketServer, SERVER_THREAD_ID);
	ClientThread* frontClientThread = new ClientThread(clientFront, CLIENT_U3_FRONT_ID, CLIENT_U3_FRONT_INTERVAL);
	ClientThread* bottomClientThread = new ClientThread(clientBottom, CLIENT_U3_BOTTOM_ID, CLIENT_U3_BOTTOM_INTERVAL);

//	MasterController* masterController = new MasterController(dataService);

	socketServerThread->start();
	frontClientThread->start();
	bottomClientThread->start();
//	masterController->start();

	socketServerThread->join();
	frontClientThread->join();
	bottomClientThread->join();
//	masterController->join();

	delete bottomClientThread;
	delete frontClientThread;
	delete socketServerThread;
	delete clientBottom;
	delete clientFront;
	delete socketServer;
//	delete masterController;
	delete dataService;
	delete camData;
	delete targetData;
	delete sensorData;

    syslog(LOG_NOTICE,"[KPI::MAIN] END");
    return EXIT_SUCCESS;
}
