#include "cv.h"
#include "highgui.h"

class FaceDetector
{
public:
	FaceDetector();
	bool Initialize();
	void DetectAndDraw(IplImage* image);

private:
	CvMemStorage* storage;
	CvHaarClassifierCascade* classifier;
};
