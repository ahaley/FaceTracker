#include "FaceDetector.h"

#include "cv.h"

using cv::Rect;

const char* cascade_name = "haarcascade_frontalface_alt.xml";

FaceDetector::FaceDetector()
{
}

bool FaceDetector::Initialize()
{
	if (!classifier.load(cascade_name)) {
		fprintf(stderr, "Could not create classifier.");
		getchar();
		return false;
	}

	storage = cvCreateMemStorage(0);
}


void FaceDetector::DetectAndDraw(cv::Mat image)
{
	cv::Mat image_gray;

	cv::cvtColor(image, image_gray, CV_BGR2GRAY);
	cv::equalizeHist(image_gray, image_gray);


	std::vector<Rect> faces;

	classifier.detectMultiScale(image_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

	for (int i = 0; i < faces.size(); i++) {
		
		//cv::Point center(
		//	faces[i].x + faces[i].width * 0.5,
		//	faces[i].y + faces[i].height * 0.5);

		cv::rectangle(image,
			cv::Rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height),
			cv::Scalar(255, 0, 0)
			);
	}


}

