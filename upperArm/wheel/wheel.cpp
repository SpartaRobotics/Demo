/*
    Example for moving in continous rotation mode on the Dynamixel UMX-AT series servos
    
	Serial:
	USB  UART: "/dev/ttyUSB0"

	*Rotates Clockwise
		UMX.turn(ID, RIGHT, 0->1020) : Limit is 1020
		
	*Rotates Counter-Clockwise
		UMX.turn(ID, LEFT,  0->900) : Limit is 900 (Anything above this for CCW may not work)
	
	*Halts at current position
		UMX.turn(ID, OFF, 0) : Stops the servo
*/
#include<iostream>
#include "UsbMX.h"

#define ID 1        // ID for singl servo
#define SEC 1000000 // 1 Second in micro second units for delay
#define MSEC 1000	// 1 milli second in micro second units for delay
#define BAUD B57600
#define ROTATION 3500000

using namespace std;

int main()
{
    UsbMX control;

	control.begin("/dev/ttyUSB0", BAUD);
    
    control.setEndless(ID, ON);
	usleep(SEC);
    
    for(int i = 0; i < 1; i++)
    {
		control.turn(ID, RIGHT, 1020);
		usleep(int(ROTATION));
		//control.turn(ID, LEFT, 1020);
		//usleep(ROTATION);
		control.turn(ID, OFF, OFF); // Either clockwise or counter clockwise as long as speed is 0
		usleep(2*SEC);
    }    
    
    control.disconnect();
    return 0;
}

