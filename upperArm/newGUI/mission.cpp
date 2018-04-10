/*
This is an extremely simple demo application to showcase the
basic structure, features and use of cvui.

Code licensed under the MIT license, check LICENSE file.
*/

#include <opencv2/opencv.hpp>

// One (and only one) of your C++ files must define CVUI_IMPLEMENTATION
// before the inclusion of cvui.h to ensure its implementaiton is compiled.
#define CVUI_IMPLEMENTATION
#include "cvui.h"

#define WINDOW_NAME "RM CONTROL"

int threshold_id2 = 2047;
int threshold_id3 = 2047;
int threshold_id4 = 2047;
int threshold_id5 = 2047;
int threshold_id6 = 2047;

int main(int argc, const char *argv[])
{
	cv::Mat frame = cv::Mat(500, 500, CV_8UC3);
	int count = 0;
	char c;

	// Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
	cvui::init(WINDOW_NAME);

	while (true) {
		// Fill the frame with a nice color
		frame = cv::Scalar(49, 52, 49);


		if (cvui::button(frame, 410, 20, "RESET")) {
			count++;
		}
		
		if (cvui::button(frame, 20, 20, "QUIT")) {
			break;
		}
		
		if (cvui::button(frame, 90, 95, "CW")) {

		}
		
		if (cvui::button(frame, 145, 95, "STOP")) {

		}
		
		if (cvui::button(frame, 215, 95, "CCW")) {

		}
		
		cvui::counter(frame, 100, 157, &threshold_id2);
		cvui::counter(frame, 100, 227, &threshold_id3);
		cvui::counter(frame, 100, 297, &threshold_id4);
		cvui::counter(frame, 100, 367, &threshold_id5);
		cvui::counter(frame, 100, 437, &threshold_id6);
		
		cvui::trackbar(frame, 250, 150, 165, &threshold_id2, 0, 4095);
		cvui::trackbar(frame, 250, 220, 165, &threshold_id3, 0, 4095);
		cvui::trackbar(frame, 250, 290, 165, &threshold_id4, 0, 4095);
		cvui::trackbar(frame, 250, 360, 165, &threshold_id5, 705, 3185);
		cvui::trackbar(frame, 250, 430, 165, &threshold_id6, 720, 3209);

		// Sometimes you want to show text that is not that simple, e.g. strings + numbers.
		// You can use cvui::printf for that. It accepts a variable number of parameter, pretty
		// much like printf does.
		// Let's show how many times the button has been clicked.
		cvui::printf(frame, 140, 20, 0.6, 0xc1c1c1, "SPARTA RM INTERFACE");
		
		//IDs
		cvui::printf(frame, 20, 100, 0.5, 0xc1c1c1, "ID1: ");
		cvui::printf(frame, 20, 160, 0.5, 0xc1c1c1, "ID2: ");
		cvui::printf(frame, 20, 230, 0.5, 0xc1c1c1, "ID3: ");
		cvui::printf(frame, 20, 300, 0.5, 0xc1c1c1, "ID4: ");
		cvui::printf(frame, 20, 370, 0.5, 0xc1c1c1, "ID5: ");
		cvui::printf(frame, 20, 440, 0.5, 0xc1c1c1, "ID6: ");

		// Update cvui stuff and show everything on the screen
		cvui::imshow(WINDOW_NAME, frame);

		// Check if ESC key was pressed
		c = cv::waitKey(33);
		if( c == 'q' || c == 27) break;
	}

	return 0;
}
