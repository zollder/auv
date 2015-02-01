//============================================================================
// Name        : auv.cpp
// Author      : zollder
//============================================================================

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
