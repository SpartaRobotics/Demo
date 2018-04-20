// Testing Manual Control!

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "UsbMX.h"
#include "JHPWMPCA9685.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>

// One (and only one) of your C++ files must define CVUI_IMPLEMENTATION
// before the inclusion of cvui.h to ensure its implementaiton is compiled.
#define CVUI_IMPLEMENTATION
#include "cvui.h"

#define WINDOW_NAME "RM CONTROL"
#define SEC 1000000 // 1 Second in micro second units for delay
#define MSEC 1000	// 1 milli second in micro second units for delay

#define BAUD_RATE B115200

#define CLW_OPN 120 // Open Claw
#define CLW_CLS 490 // Close Claw

#define ID1_MIN 0
#define ID2_MIN 1100
#define ID3_MIN 1800
#define ID4_MIN 0
#define ID5_MIN 600
#define ID6_MIN 600

#define ID1_MAX 2100
#define ID2_MAX 3500
#define ID3_MAX 2800
#define ID4_MAX 3100
#define ID5_MAX 3250
#define ID6_MAX 3200

#define ID1_RST 280
#define ID2_RST 2400
#define ID3_RST 2250
#define ID4_RST 1970
#define ID5_RST 1930
#define ID6_RST 1920

#define DSPEED 5 // Default Speed

using namespace std;
using namespace cv;

VideoCapture clawCam;
Mat frame;
char quit;

void displayGUI(UsbMX *control, int *servoThres, int *servoCon, int *servoReadPos, int *moveStatus);
int getkey();
int map ( int x, int in_min, int in_max, int out_min, int out_max);

int main(int argc, const char *argv[])
{
	frame = cv::Mat(800, 1200, CV_8UC3);
	char c;
	int err;
	int count = 0;
	int speed = 0;
	
	// Position values for each servo
	int servoThres[7] = {ID1_RST,ID2_RST,ID3_RST,ID4_RST,
					     ID5_RST,ID6_RST,CLW_OPN};
	
	// Position for executing concurrent arm configuration
	int servoCon[6] = {ID1_RST,ID2_RST,ID3_RST,ID4_RST,
					   ID5_RST,ID6_RST};
	
	// Read positions from servos
	int servoReadPos[6];
	
	// Indicates if the servos will move
	int moveStatus = 0;
	
	//VideoCapture clawCam;
	clawCam.open(0);
	Mat clawFrame;
	
	VideoCapture baseCam;/*
	baseCam.open(1);
	Mat baseFrame;
	*/
	Mat currentFrame;
	
	PCA9685 *pca9685 = new PCA9685() ;
    err = pca9685->openPCA9685();
    if (err < 0){
        printf("Error: %d", pca9685->error);
    } else {
        pca9685->setAllPWM(0,0) ;
        pca9685->reset() ;
        pca9685->setPWMFrequency(60);
    }

	UsbMX control;
	control.begin("/dev/ttyUSB0", BAUD_RATE);

	// Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
	cvui::init(WINDOW_NAME);

	while (true) {
		
		displayGUI(&control, servoThres, servoCon, servoReadPos, &moveStatus);
		
		servoReadPos[0] = control.readPosition(1);
		servoReadPos[1] = control.readPosition(2);
		servoReadPos[2] = control.readPosition(3);
		servoReadPos[3] = control.readPosition(4);
		servoReadPos[4] = control.readPosition(5);
		servoReadPos[5] = control.readPosition(6);
		
		if(moveStatus)
		{		   
		control.moveSpeed(1, servoThres[0], DSPEED);
		control.moveSpeed(2, servoThres[1], DSPEED);
		control.moveSpeed(3, servoThres[2], DSPEED);
		control.moveSpeed(4, servoThres[3], DSPEED);
		control.moveSpeed(5, servoThres[4], DSPEED);
		control.moveSpeed(6, servoThres[5], DSPEED);
		}
		
		pca9685->setPWM(0,0,servoThres[6]);
        pca9685->setPWM(1,0,servoThres[6]);
        /*
        clawCam.read(currentFrame);
		if(currentFrame.empty()) break;
		imshow("VID1", currentFrame);
		*/
		/*
		baseCam.read(baseFrame);
		if(baseFrame.empty()) break;
		imshow("VID2", baseFrame);
		*/
		// Check if ESC key was pressed
		c = cv::waitKey(33);
		if( c == 'q' || c == 27 || quit == 'q') break;
	}

	pca9685->setPWM(0,0,CLW_OPN);
    pca9685->setPWM(1,0,CLW_OPN);
    sleep(1);
    pca9685->closePCA9685();
    
	control.disconnect();

	return 0;
}

void displayGUI(UsbMX *control, int *servoThres, int *servoCon, int *servoReadPos, int *moveStatus)
{
	
	
	// Fill the frame with a nice color
	frame = cv::Scalar(49, 52, 49);
	
	if (cvui::button(frame, 410, 20, "RESET")) {
		servoThres[0] = ID1_RST;
		servoThres[1] = ID2_RST;
		servoThres[2] = ID3_RST;
		servoThres[3] = ID4_RST;
		servoThres[4] = ID5_RST;
		servoThres[5] = ID6_RST;
	}
	
	if (cvui::button(frame, 80, 505, "OPEN")) {
		servoThres[6] = CLW_OPN;
	}
	
	if (cvui::button(frame, 155, 505, "CLOSE")) {
		servoThres[6] = CLW_CLS;
	}
	
	if (cvui::button(frame, 20, 20, "QUIT")) {
		quit = 'q';
	}
	
	if (cvui::button(frame, 100, 550, "BASE CAM")) {
		clawCam.release();
		clawCam.open(1);
	}
	
	if (cvui::button(frame, 100, 600, "CLAW CAM")) {
		//clawCam.open(0);
	}

	if (cvui::button(frame, 880, 510, "CONCURRENT MOVE")) {
		servoThres[0] = servoCon[0];
		servoThres[1] = servoCon[1];
		servoThres[2] = servoCon[2];
		servoThres[3] = servoCon[3];
		servoThres[4] = servoCon[4];
		servoThres[5] = servoCon[5];
	}

	if (cvui::button(frame, 100, 700, "KILL MODE")) {
		//Kill Mode!
	}
	
	if (cvui::button(frame, 300, 600, "GO TO READ POSITIONS")) {
		servoThres[0] = servoReadPos[0];
		servoThres[1] = servoReadPos[1];
		servoThres[2] = servoReadPos[2];
		servoThres[3] = servoReadPos[3];
		servoThres[4] = servoReadPos[4];
		servoThres[5] = servoReadPos[5];
	}
	
	if (cvui::button(frame, 600, 600, "LOOSEN SERVOS")) {
		*moveStatus = 0;
		control->setEndless(1, ON);
		control->setEndless(2, ON);
		control->setEndless(3, ON);
		control->setEndless(4, ON);
		control->setEndless(5, ON);
		control->setEndless(6, ON);
	}
	
	if (cvui::button(frame, 600, 650, "TIGHTEN SERVOS")) {
		*moveStatus = 1;
		control->setEndless(1, OFF);
		control->setEndless(2, OFF);
		control->setEndless(3, OFF);
		control->setEndless(4, OFF);
		control->setEndless(5, OFF);
		control->setEndless(6, OFF);
		
		servoThres[0] = servoReadPos[0];
		servoThres[1] = servoReadPos[1];
		servoThres[2] = servoReadPos[2];
		servoThres[3] = servoReadPos[3];
		servoThres[4] = servoReadPos[4];
		servoThres[5] = servoReadPos[5];
	}
	

	cvui::counter(frame, 180, 97, &servoThres[0]);
	cvui::counter(frame, 180, 157, &servoThres[1]);
	cvui::counter(frame, 180, 227, &servoThres[2]);
	cvui::counter(frame, 180, 297, &servoThres[3]);
	cvui::counter(frame, 180, 367, &servoThres[4]);
	cvui::counter(frame, 180, 437, &servoThres[5]);
	
	cvui::trackbar(frame, 300, 80, 400, &servoThres[0], 0, 2100);
	cvui::trackbar(frame, 300, 150, 400, &servoThres[1], 1300, 4000);
	cvui::trackbar(frame, 300, 220, 400, &servoThres[2], 600, 4000);
	cvui::trackbar(frame, 300, 290, 400, &servoThres[3], 0, 4095);
	cvui::trackbar(frame, 300, 360, 400, &servoThres[4], 640, 3142);
	cvui::trackbar(frame, 300, 430, 400, &servoThres[5], 665, 3140);
	cvui::trackbar(frame, 300, 500, 400, &servoThres[6], 120, 490);
	
	cvui::trackbar(frame, 750, 80, 400, &servoCon[0], 0, 2100);
	cvui::trackbar(frame, 750, 150, 400, &servoCon[1], 1300, 4000);
	cvui::trackbar(frame, 750, 220, 400, &servoCon[2], 600, 4000);
	cvui::trackbar(frame, 750, 290, 400, &servoCon[3], 0, 4095);
	cvui::trackbar(frame, 750, 360, 400, &servoCon[4], 640, 3142);
	cvui::trackbar(frame, 750, 430, 400, &servoCon[5], 665, 3140);

	cvui::printf(frame, 140, 20, 0.6, 0xc1c1c1, "SPARTA RM INTERFACE");
	cvui::printf(frame, 730, 20, 0.6, 0xc1c1c1, "CONCURRENT SERVO MOVEMENT");
	cvui::printf(frame, 110, 50, 0.4, 0xc1c1c1, "CURRENT");
	cvui::printf(frame, 110, 70, 0.4, 0xc1c1c1, "POSITION");
	cvui::printf(frame, 20, 90, 0.5, 0xc1c1c1, "SERVO 1:   %d", servoReadPos[0]);
	cvui::printf(frame, 20, 160, 0.5, 0xc1c1c1, "SERVO 2:   %d", servoReadPos[1]);
	cvui::printf(frame, 20, 230, 0.5, 0xc1c1c1, "SERVO 3:   %d", servoReadPos[2]);
	cvui::printf(frame, 20, 300, 0.5, 0xc1c1c1, "SERVO 4:   %d", servoReadPos[3]);
	cvui::printf(frame, 20, 370, 0.5, 0xc1c1c1, "SERVO 5:   %d", servoReadPos[4]);
	cvui::printf(frame, 20, 440, 0.5, 0xc1c1c1, "SERVO 6:   %d", servoReadPos[5]);
	cvui::printf(frame, 20, 510, 0.5, 0xc1c1c1, "CLAW: ");
	
	//cvui::printf(frame, 20, 150, 0.5, 0xc1c1c1, "%d", rand() % (4095) );
	
	// Update cvui stuff and show everything on the screen
	cvui::imshow(WINDOW_NAME, frame);
}

int getkey() {
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    character = fgetc(stdin);
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}

int map ( int x, int in_min, int in_max, int out_min, int out_max) {
    int toReturn =  (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min ;
    
    return toReturn ;
}
