
#include<iostream>
#include "UsbMX.h"

#define SEC 1000000 // 1 Second in micro second units for delay
#define MSEC 1000	// 1 milli second in micro second units for delay
#define BAUD B57600
#define ID 2

using namespace std;

int main()
{
    UsbMX control;

	control.begin("/dev/ttyUSB0", BAUD);

	control.setEndless(ID, OFF); // Sets the servo to "Servo" mode

	control.move(ID,1024);
	usleep(2*SEC);

    for(int i = 0; i < 5; i ++)
    {
		cout << "POSITION " << ID << ": " << control.readPosition(ID) << endl;
		usleep(SEC);
    }

    control.disconnect();
    return 0;
}

