//============================================================================
// Name        : auv.cpp
// Author      : zollder
//============================================================================

<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>


#define ESC 27
bool flag=true;

static void* server_run();
int client_run( char* , unsigned short int);


using namespace std;


int main(int argc, char *argv[]) {

    unsigned short int port = 5000;


    pthread_t thread1;
    pthread_t t2;

    const char *message1 = "Server Thread";

    //Server Start
    int server = pthread_create( &thread1, NULL, server_run, (void*) message1 );
    if (server == 0) {
        printf("[KPI::SERVER]:Thread started\n");
    }
    else
    {
        printf("[KPI::SERVER]:Thread Did not start\n");
        return EXIT_FAILURE;
    }


    //Client Start
    if(argc == 2)
    {
        client_run( argv[1], atoi(argv[2]) );
        //printf("\nUsage: %s <ip of server> <port of server\n",argv[0]);
        //return EXIT_FAILURE;
    }
    else
    {
        client_run( address, port );
    }



    char key;
    do
    {

        key = getchar();

    }while(key != ESC);
    printf("[KPI::SERVER]::Escape Character Triggered\n");

    server = pthread_join(thread1, NULL);

    return EXIT_SUCCESS;


}

static void* server_run()
{

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));




    listen(listenfd, 10);

    while (flag) {
        connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff));

        close(connfd);
        sleep(1);
    }

    close(listenfd);

}


int client_run( char* address, unsigned short int port )
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;



    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return EXIT_FAILURE;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return EXIT_FAILURE;
    }

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return EXIT_FAILURE;
    }

    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    }

    if(n < 0)
    {
        printf("\n Read error \n");
    }

    return EXIT_SUCCESS;
=======
#include "imu/ImuThread.h"
#include "dmu/DmuThread.h"
#include "controller/AltitudeController.h"
#include "controller/HeadingController.h"
#include "controller/HorizontalMotion.h"
#include "dmu/DMU.h"

#define MAX 64

int readButton(int);

int main()
{
	printf("Main thread started!\n");

	readButton(76);

	SensorData* sensorData = new SensorData();
//	DesiredData* targetData = new DesiredData();

	ImuThread* imuThread = new ImuThread(sensorData);
	DmuThread* dmuThread = new DmuThread(sensorData);
//	VerticalMotion* verticalMotionThread = new VerticalMotion(sensorData, targetData);
//	HeadingMotion* headingMotionThread = new HeadingMotion(sensorData, targetData);
//	HorizontalMotion* horizontalMotionThread = new HorizontalMotion(sensorData, targetData);
//
	imuThread->start();
	dmuThread->start();
//	verticalMotionThread->start();
//	headingMotionThread->start();
//	horizontalMotionThread->start();
//
	imuThread->join();
	dmuThread->join();
//	verticalMotionThread->join();
//	headingMotionThread->join();
//	horizontalMotionThread->join();
//
//	delete horizontalMotionThread;
//	delete headingMotionThread;
//	delete verticalMotionThread;
	delete dmuThread;
	delete imuThread;
//	delete targetData;
//	delete sensorData;

//	DMU dmu;
//	dmu.enable();
//	for (int i = 0; i < 20; i++)
//	{
//		dmu.readSensor();
//		dmu.calculateTemperature();
//		dmu.calculatePressure();
//		printf("Actual temperature: %0.2f\n", dmu.temperature);
//		printf("Actual pressure: %0.2f\n", dmu.pressure);
//	}

//	IMU imu;
//	imu.execute(1);

//	PWM pwm;
//	pwm.initialize();
//	pwm.getPeriodHz(11);
//	pwm.getDuty(11);
//
//	pwm.setPeriod(1, 500);
//
//	pwm.setPolarity(11, 0);
//	pwm.setPolarity(12, 0);
//
//	pwm.setDuty(11, 0);
//	pwm.setDuty(12, 0);
//
//	pwm.start(11);
//	pwm.start(12);
//	printf("duty1: %d, duty2: %d\n", 0, 0);
//
//	sleep(10);
//
//	pwm.setDuty(11, 15);
//	pwm.setDuty(12, 5);
//	printf("duty1: %d, duty2: %d\n", 15, 5);
//
//	sleep(10);
//
//	pwm.setDuty(11, 30);
//	pwm.setDuty(12, 10);
//	printf("duty1: %d, duty2: %d\n", 30, 10);
//
//	sleep(10);
//
//	pwm.setDuty(11, 0);
//	pwm.setDuty(12, 50);
//	printf("duty1: %d, duty2: %d\n", 0, 50);
//
//	sleep(10);
//
//	pwm.setDuty(11, 100);
//	pwm.setDuty(12, 0);
//	printf("duty1: %d, duty2: %d\n", 100, 0);
//
//	pwm.getDuty(11);
//	pwm.getDuty(12);
//
//	pwm.getPeriod(1);
>>>>>>> master
}

int readButton(int inputPin)
{
	cout << "GPIO input pin:" << inputPin << " start" << endl;
	FILE *buttonHandle = NULL;
	char setValue[4], GPIOString[4], GPIOValue[MAX], GPIODirection[MAX];
	sprintf(GPIOString, "%d", inputPin);
	sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", inputPin);
	sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", inputPin);

	// // export the pin
	// buttonHandle = fopen("/sys/class/gpio/export", "ab");
	// if (buttonHandle == NULL)
	// {
	// 	printf("Can't export GPIO pin.\n");
	// 	return 1;
	// }

	// strcpy(setValue, GPIOString);
	// fwrite(&setValue, sizeof(char), 3, buttonHandle);
	// fclose(buttonHandle);

	// set pin direction
//	buttonHandle = fopen(GPIODirection, "rb+");
//	if (buttonHandle == NULL)
//	{
//		printf("Can't open value handle.\n");
//		return 1;
//	}
//
//	fread(&setValue, sizeof(char), 1, buttonHandle);
//	fclose(buttonHandle);
//	printf("Value read: %d\n", setValue[0]);

	// get pin value
	buttonHandle = fopen(GPIOValue, "rb+");
	if (buttonHandle == NULL)
	{
		printf("Can't open value handle.\n");
		return 1;
	}

	fread(&setValue, sizeof(char), 1, buttonHandle);
	fclose(buttonHandle);
	cout << "Pin value: " << setValue[0] << endl;
//	printf("Value read: %d\n", setValue[0]);


//	unsigned int value = LOW;
//	do
//	{
//		gpio_get_value(ButtonGPIO, &value);
//		cout << ".";
//		usleep(1000);      // sleep for one millisecond
//	}
//	while (value!=HIGH);
//
//	cout << endl <<  "Button was just pressed!" << endl;

	return 0;
}
