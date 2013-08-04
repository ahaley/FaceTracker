#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <stdio.h>
#include "cv.h"
#include "highgui.h"
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

/*
	FaceDetector faceDetector;

	if (!faceDetector.Initialize()) {
		cvReleaseCapture(&capture);
		controller.Disconnect();
		return -1;
	}
	*/
	cvNamedWindow("mywindow", CV_WINDOW_AUTOSIZE);
	
	IplImage *frame, *frameCopy = 0;

	while (1) {
		frame = cvQueryFrame(capture);
		if (!frame) {
			fprintf(stderr, "ERROR, frame is NULL.\n");
			getchar();
			break;
		}

		if (!frameCopy) {
			frameCopy = cvCreateImage(cvSize(frame->width, frame->height),
				IPL_DEPTH_8U, frame->nChannels);
		}

		if (frame->origin == IPL_ORIGIN_TL)
			cvCopy(frame, frameCopy, 0);
		else
			cvFlip(frame, frameCopy, 0);

	//	faceDetector.DetectAndDraw(frameCopy);

		cvShowImage("mywindow", frameCopy);

		int c = (cvWaitKey(10) & 255);
		if (c == 27) break;
		if (c == 32) controller.SendMessage("1", 1);
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