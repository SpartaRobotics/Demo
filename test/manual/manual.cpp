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

#define SERVO_MIN 120
#define SERVO_MAX 490

int getkey();
int map ( int x, int in_min, int in_max, int out_min, int out_max);

int main(int argc, const char *argv[])
{
	cv::Mat frame = cv::Mat(600, 500, CV_8UC3);
	char c;
	int err;
	int count = 0;
	int speed = 0;
	int threshold_id1 = 2047;
	int threshold_id2 = 1844;
	int threshold_id3 = 2210;
	int threshold_id4 = 2047;
	int threshold_id5 = 1940;
	int threshold_id6 = 2047;
	int threshold_clw = 120;

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

	//control.setEndless(1, ON);  // Sets the servo to "Wheel" mode
	//control.setEndless(2, OFF); // Sets the servo to "Servo" mode
	control.setEndless(3, OFF); // Sets the servo to "Servo" mode
	control.setEndless(4, OFF); // Sets the servo to "Servo" mode
	control.setEndless(5, OFF); // Sets the servo to "Servo" mode
	control.setEndless(6, OFF); // Sets the servo to "Servo" mode

	// Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
	cvui::init(WINDOW_NAME);

	while (true) {
		// Fill the frame with a nice color
		frame = cv::Scalar(49, 52, 49);


		if (cvui::button(frame, 410, 20, "RESET")) {
			threshold_id1 = 2047;
			threshold_id2 = 1844;
			threshold_id3 = 2210;
			threshold_id4 = 2047;
			threshold_id5 = 1940;
			threshold_id6 = 2047;
		}
		
		if (cvui::button(frame, 20, 20, "QUIT")) {
			break;
		}
		
		if (cvui::button(frame, 80, 505, "OPEN")) {
			threshold_clw = 120;
		}
		
		if (cvui::button(frame, 155, 505, "CLOSE")) {
			threshold_clw = 490;
		}

		cvui::counter(frame, 100, 97, &threshold_id1);
		cvui::counter(frame, 100, 157, &threshold_id2);
		cvui::counter(frame, 100, 227, &threshold_id3);
		cvui::counter(frame, 100, 297, &threshold_id4);
		cvui::counter(frame, 100, 367, &threshold_id5);
		cvui::counter(frame, 100, 437, &threshold_id6);
		
		cvui::trackbar(frame, 250, 80, 165, &threshold_id1, 553, 2969);
		cvui::trackbar(frame, 250, 150, 165, &threshold_id2, 0, 3000);
		cvui::trackbar(frame, 250, 220, 165, &threshold_id3, 1036, 3553);
		cvui::trackbar(frame, 250, 290, 165, &threshold_id4, 0, 4095);
		cvui::trackbar(frame, 250, 360, 165, &threshold_id5, 705, 3185);
		cvui::trackbar(frame, 250, 430, 165, &threshold_id6, 720, 3209);
		cvui::trackbar(frame, 250, 500, 165, &threshold_clw, 120, 490);

		cvui::printf(frame, 140, 20, 0.6, 0xc1c1c1, "SPARTA RM INTERFACE");
		cvui::printf(frame, 20, 100, 0.5, 0xc1c1c1, "ID1: ");
		cvui::printf(frame, 20, 160, 0.5, 0xc1c1c1, "ID2: ");
		cvui::printf(frame, 20, 230, 0.5, 0xc1c1c1, "ID3: ");
		cvui::printf(frame, 20, 300, 0.5, 0xc1c1c1, "ID4: ");
		cvui::printf(frame, 20, 370, 0.5, 0xc1c1c1, "ID5: ");
		cvui::printf(frame, 20, 440, 0.5, 0xc1c1c1, "ID6: ");
		cvui::printf(frame, 20, 510, 0.5, 0xc1c1c1, "CLW: ");

		//control.moveSpeed(1, threshold_id1, 32);
		control.moveSpeed(2, threshold_id2, 32);
		control.moveSpeed(3, threshold_id3, 32);
		control.moveSpeed(4, threshold_id4, 32);
		control.moveSpeed(5, threshold_id5, 32);
		control.moveSpeed(6, threshold_id6, 32);
		
		pca9685->setPWM(0,0,threshold_clw);
        pca9685->setPWM(1,0,threshold_clw);
            
		// Update cvui stuff and show everything on the screen
		cvui::imshow(WINDOW_NAME, frame);

		// Check if ESC key was pressed
		c = cv::waitKey(33);
		if( c == 'q' || c == 27) break;
	}

	pca9685->setPWM(0,0,SERVO_MIN);
    pca9685->setPWM(1,0,SERVO_MIN);
    sleep(1);
    pca9685->closePCA9685();
    
	control.disconnect();

	return 0;
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
