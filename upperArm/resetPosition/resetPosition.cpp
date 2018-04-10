
#include<iostream>
#include "UsbMX.h"

#define SEC 1000000 // 1 Second in micro second units for delay
#define MSEC 1000	// 1 milli second in micro second units for delay
#define BAUD B57600
#define ID 1

using namespace std;

int main()
{
    UsbMX control;

	control.begin("/dev/ttyUSB0", B57600);
	control.setBD(3, 1000000);
	control.setBD(4, 1000000);
	control.setBD(5, 1000000);
	control.setBD(6, 1000000);
	/*control.setEndless(1, OFF); // Sets the servo to "Servo" mode
	usleep(SEC);
	control.setID(1,2);

	control.moveSpeed(2,0,128);
	usleep(2*SEC);
	control.moveSpeed(2,2048,128);
	usleep(2*SEC);
	*/
/*
	control.setEndless(3, OFF); // Sets the servo to "Servo" mode
	control.setEndless(4, OFF); // Sets the servo to "Servo" mode
	control.setEndless(5, OFF); // Sets the servo to "Servo" mode
	control.setEndless(6, OFF); // Sets the servo to "Servo" mode
	usleep(SEC);

	control.moveSpeed(3,2048,128);
	usleep(2*SEC);
	control.moveSpeed(4,2048,128);
	usleep(2*SEC);
	control.moveSpeed(5,2048,128);
	usleep(2*SEC);
	control.moveSpeed(6,2048,128);
	usleep(2*SEC);
*/


    control.disconnect();
    return 0;
}

