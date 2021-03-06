
#include<iostream>
#include "UsbMX.h"

#define SEC 1000000 // 1 Second in micro second units for delay
#define MSEC 1000	// 1 milli second in micro second units for delay
#define NEW_BAUD B57600
#define OLD_BAUD B1000000
#define ID 2

using namespace std;

void changeBaud();
void test();

int main()
{
    //changeBaud();
	test();
}

void changeBaud()
{
	UsbMX control;
	control.begin("/dev/ttyUSB0", OLD_BAUD);
	control.setBD(ID, 57600);
	usleep(SEC);
	control.disconnect();
}

void test()
{
	UsbMX control;
	control.begin("/dev/ttyUSB0", B1000000);

	control.setEndless(ID, OFF);

	control.move(ID, 1024);
	usleep(2*SEC);
	//cout << "POSITION " << ID << ": " << control.readPosition(ID) << endl;
	control.move(ID, 2056);
	usleep(2*SEC);
	//cout << "POSITION " << ID << ": " << control.readPosition(ID) << endl;
	control.move(ID, 4095);
	usleep(2*SEC);
	//cout << "POSITION " << ID << ": " << control.readPosition(ID) << endl;
	control.move(ID, 2056);
	usleep(2*SEC);
	//cout << "POSITION " << ID << ": " << control.readPosition(ID) << endl;
	control.move(ID, 0);
	usleep(2*SEC);
	//cout << "POSITION " << ID << ": " << control.readPosition(ID) << endl;

	control.disconnect();
}
