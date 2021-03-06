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

#define CLW_OPN 120 // Open Claw
#define CLW_CLS 490 // Close Claw

#define ID1_MIN 0
#define ID2_MIN 0 // NOT READY!
#define ID3_MIN 1000
#define ID4_MIN 0
#define ID5_MIN 600
#define ID6_MIN 759

#define ID1_MAX 2100
#define ID2_MAX 0 // NOT READY!
#define ID3_MAX 2700
#define ID4_MAX 4095
#define ID5_MAX 3142
#define ID6_MAX 3140

#define ID1_RST 1230
#define ID2_RST 0	 // NOT READY!
#define ID3_RST 1232
#define ID4_RST 4041
#define ID5_RST 1900
#define ID6_RST 1924

using namespace std;
using namespace cv;

VideoCapture clawCam;
Mat frame;
char quit;

void displayGUI(int *threshold_id1, int *threshold_id2, int *threshold_id3,
				int *threshold_id4, int *threshold_id5, int *threshold_id6,
				int *threshold_clw);
int getkey();
int map ( int x, int in_min, int in_max, int out_min, int out_max);

int main(int argc, const char *argv[])
{
	frame = cv::Mat(700, 500, CV_8UC3);
	char c;
	int err;
	int count = 0;
	int speed = 0;
	int threshold_id1 = ID1_RST;
	int threshold_id2 = ID2_RST;
	int threshold_id3 = ID3_RST;
	int threshold_id4 = ID4_RST;
	int threshold_id5 = ID5_RST;
	int threshold_id6 = ID6_RST;
	int threshold_clw = CLW_OPN;
	
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
	control.begin("/dev/ttyUSB0", B1000000);

	//control.setEndless(1, OFF); // Do not set mode for Multi-Turn
	//control.setEndless(2, OFF); // Do not set mode for Multi-Turn
	control.setEndless(3, OFF); // Sets the servo to "Servo" mode
	control.setEndless(4, OFF); // Sets the servo to "Servo" mode
	control.setEndless(5, OFF); // Sets the servo to "Servo" mode
	control.setEndless(6, OFF); // Sets the servo to "Servo" mode

	// Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
	cvui::init(WINDOW_NAME);

	while (true) {
		
		displayGUI(&threshold_id1, &threshold_id2, &threshold_id3, 
			&threshold_id4, &threshold_id5, &threshold_id6, &threshold_clw);
				   
		control.moveSpeed(1, threshold_id1, 32);
		control.moveSpeed(2, threshold_id2, 32);
		control.moveSpeed(3, threshold_id3, 32);
		control.moveSpeed(4, threshold_id4, 32);
		control.moveSpeed(5, threshold_id5, 32);
		control.moveSpeed(6, threshold_id6, 32);
		
		pca9685->setPWM(0,0,threshold_clw);
        pca9685->setPWM(1,0,threshold_clw);
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

void displayGUI(int *threshold_id1, int *threshold_id2, int *threshold_id3,
				int *threshold_id4, int *threshold_id5, int *threshold_id6,
				int *threshold_clw)
{
	// Fill the frame with a nice color
	frame = cv::Scalar(49, 52, 49);
	
	if (cvui::button(frame, 410, 20, "RESET")) {
		*threshold_id1 = ID1_RST;
		*threshold_id2 = ID2_RST;
		*threshold_id3 = ID3_RST;
		*threshold_id4 = ID4_RST;
		*threshold_id5 = ID5_RST;
		*threshold_id6 = ID6_RST;
	}
	
	if (cvui::button(frame, 80, 505, "OPEN")) {
		*threshold_clw = CLW_OPN;
	}
	
	if (cvui::button(frame, 155, 505, "CLOSE")) {
		*threshold_clw = CLW_CLS;
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

	if (cvui::button(frame, 300, 600, "KILL MODE")) {
		//Kill Mode!
	}


	cvui::counter(frame, 100, 97, threshold_id1);
	cvui::counter(frame, 100, 157, threshold_id2);
	cvui::counter(frame, 100, 227, threshold_id3);
	cvui::counter(frame, 100, 297, threshold_id4);
	cvui::counter(frame, 100, 367, threshold_id5);
	cvui::counter(frame, 100, 437, threshold_id6);
	
	cvui::trackbar(frame, 250, 80, 165, threshold_id1, 800, 1600);
	cvui::trackbar(frame, 250, 150, 165, threshold_id2, 0, 3000);
	cvui::trackbar(frame, 250, 220, 165, threshold_id3, 1000, 2700);
	cvui::trackbar(frame, 250, 290, 165, threshold_id4, 0, 4095);
	cvui::trackbar(frame, 250, 360, 165, threshold_id5, 600, 3142);
	cvui::trackbar(frame, 250, 430, 165, threshold_id6, 759, 3140);
	cvui::trackbar(frame, 250, 500, 165, threshold_clw, 120, 490);

	cvui::printf(frame, 140, 20, 0.6, 0xc1c1c1, "SPARTA RM INTERFACE");
	cvui::printf(frame, 20, 100, 0.5, 0xc1c1c1, "ID1: ");
	cvui::printf(frame, 20, 160, 0.5, 0xc1c1c1, "ID2: ");
	cvui::printf(frame, 20, 230, 0.5, 0xc1c1c1, "ID3: ");
	cvui::printf(frame, 20, 300, 0.5, 0xc1c1c1, "ID4: ");
	cvui::printf(frame, 20, 370, 0.5, 0xc1c1c1, "ID5: ");
	cvui::printf(frame, 20, 440, 0.5, 0xc1c1c1, "ID6: ");
	cvui::printf(frame, 20, 510, 0.5, 0xc1c1c1, "CLW: ");
	
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
