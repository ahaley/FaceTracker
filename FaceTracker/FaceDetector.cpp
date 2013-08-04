#include "FaceDetector.h"

const char* cascade_name = "haarcascade_frontalface_alt.xml";

FaceDetector::FaceDetector()
{
}

bool FaceDetector::Initialize()
{
	classifier = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0);

	if (!classifier) {
		fprintf(stderr, "Could not create classifier.");
		getchar();
		return false;
	}

	storage = cvCreateMemStorage(0);


}


void FaceDetector::DetectAndDraw(IplImage* image)
{

}

