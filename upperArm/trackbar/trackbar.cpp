#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "UsbMX.h"

using namespace std;
using namespace cv;

#define USB 1   	// 1 for USB, 0 for GPIO
#define SEC 1000000 // 1 Second in micro second units for delay
#define MSEC 1000	// 1 milli second in micro second units for delay

UsbMX control;

int min_1 = 0;
int const max_1 = 2;

int min_2 = 180;
int const max_2 = 360;

int min_3 = 106;
int const max_3 = 192;

int min_4 = 180;
int const max_4 = 360;

int min_5 = 110;
int const max_5 = 215;

int min_6 = 113;
int const max_6 = 224;

void Threshold_1(int, void*)
{
	/*if(min_1 == 0){
	
	}else if(min_1 == 1){
	
	}else if(min_1 == 2)

	}*/
}

void Threshold_2(int, void*)
{
	int temp = min_2;
	control.moveDeg(2, int(temp - 180));
}

void Threshold_3(int, void*)
{
	int temp = min_3 - 92;
	control.moveDeg(3, int(temp - 180));
}

void Threshold_4(int, void*)
{
	int temp = min_4;
	control.moveDeg(4, int(temp - 180));
}

void Threshold_5(int, void*)
{
	int temp = min_5 + 105;
	control.moveDeg(5, int(temp - 180));
}

void Threshold_6(int, void*)
{
	int temp = min_6 + 57;
	control.moveDeg(6, int(temp - 180));
}

int main()
{

	control.begin("/dev/ttyUSB0", B1000000);

	control.setEndless(1, ON); // Sets the servo to "Servo" mode
	control.setEndless(2, OFF); // Sets the servo to "Servo" mode
	control.setEndless(3, OFF); // Sets the servo to "Servo" mode
	control.setEndless(4, OFF); // Sets the servo to "Servo" mode
	control.setEndless(5, OFF); // Sets the servo to "Servo" mode
	control.setEndless(6, OFF); // Sets the servo to "Servo" mode

	namedWindow("Window", CV_WINDOW_AUTOSIZE);
	createTrackbar( "Servo 1:", "Window", &min_1, max_1, Threshold_1);
	createTrackbar( "Servo 2:", "Window", &min_2, max_2, Threshold_2);
	createTrackbar( "Servo 3:", "Window", &min_3, max_3, Threshold_3);
	createTrackbar( "Servo 4:", "Window", &min_4, max_4, Threshold_4);
	createTrackbar( "Servo 5:", "Window", &min_5, max_5, Threshold_5);
	createTrackbar( "Servo 6:", "Window", &min_6, max_6, Threshold_6);

	char c;

	control.moveDeg(2, min_2);
	control.moveDeg(3, min_3);
	control.moveDeg(4, min_4);
	control.moveDeg(5, min_5);
	control.moveDeg(6, min_6);

	while(1)
	{

		c = waitKey(33);
		if( c == 'q') break;
	}

	destroyAllWindows();
	control.disconnect();
	return 0;
}
