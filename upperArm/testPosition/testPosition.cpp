
#include<iostream>
#include "UsbMX.h"

#define SEC 1000000 // 1 Second in micro second units for delay
#define MSEC 1000	// 1 milli second in micro second units for delay
#define BAUD B1000000

using namespace std;

void servoTestLimit(UsbMX& control, const unsigned char ID, int low_limit, int high_limit);
void testCase1();
void readAllServos(UsbMX& control);

int main()
{
    UsbMX controller;

	controller.begin("/dev/ttyUSB0", B57600);

	readAllServos(controller);
	

    controller.disconnect();
    return 0;
}

void readAllServos(UsbMX& control)
{
	for(int i = 0; i < 5; i++)
	{
		cout << "Position 3: " << control.readPosition(3) << endl;
		cout << "Position 4: " << control.readPosition(4) << endl;
		cout << "Position 5: " << control.readPosition(5) << endl;
		cout << "Position 6: " << control.readPosition(6) << endl;
		cout << endl;
	}
}

void servoTestLimit(UsbMX& control, const unsigned char ID, int low_limit, int high_limit)
{
	control.setEndless(ID, OFF); // Sets the servo to "Servo" mode

	for(int i = 0; i < 3; i++)
	{
		control.move(ID,low_limit);
		usleep(2*SEC);
		control.move(ID,high_limit);
		usleep(2*SEC);
	}
	control.move(ID,low_limit);
	usleep(2*SEC);
}

/*void testCase1()
{
	//servoTestLimit(controller, 3, 0, 1800);
	//servoTestLimit(controller, 4, 0, 4095);
	//servoTestLimit(controller, 5, 3078, 4095);
	//servoTestLimit(controller, 6, 1800, 4095);
	controller.moveSpeed(4,0,64);
	usleep(1000);
	controller.moveSpeed(5,3078,64);
	usleep(1000);
	controller.moveSpeed(6,1800,64);
	usleep(2*SEC);
	controller.moveSpeed(4,2048,64);
	usleep(1000);
	controller.moveSpeed(5,4095,64);
	usleep(1000);
	controller.moveSpeed(6,4095,64);
	usleep(2*SEC);
	controller.moveSpeed(4,0,64);
	usleep(1000);
	controller.moveSpeed(5,3078,64);
	usleep(1000);
	controller.moveSpeed(6,1800,64);
	usleep(2*SEC);
}*/
