#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"
#include "ServoController.h"
#include "FaceDetector.h"

int PromptForPort();

int main()
{
	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
	if (!capture) {
		fprintf(stderr, "ERROR: capture is NULL.\n");
		getchar();
		return -1;
	}

	ServoController controller;

	if (!controller.Connect(PromptForPort())) {
		fprintf(stderr, "ERROR: Could not connect to servo controller.\n");
		cvReleaseCapture(&capture);
		getchar();
		return -1;
	}

	FaceDetector faceDetector;

	if (!faceDetector.Initialize()) {
		cvReleaseCapture(&capture);
		controller.Disconnect();
		return -1;
	}
	
	cvNamedWindow("mywindow", CV_WINDOW_AUTOSIZE);
	cvResizeWindow("mywindow", 640, 480);
	
	cv::Mat frame;

	while (1) {
		frame = cvQueryFrame(capture);

		if (frame.empty()) {
			fprintf(stderr, "ERROR, frame is NULL.\n");
			getchar();
			break;
		}

		faceDetector.DetectAndDraw(frame);

		controller.ZeroInOnFace(frame, faceDetector.CenterLastFace);

		cv::imshow("mywindow", frame);

		int c = (cv::waitKey(10) & 255);
		if (c == 27) break;
	}

	cvReleaseCapture(&capture);
	cvDestroyWindow("mywindow");
	return 0;
}



int PromptForPort()
{
	printf("Enter the COM port of the Servo Controller: ");
	int port;
	scanf("%d", &port);
	return port;
}