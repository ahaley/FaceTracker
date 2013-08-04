#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class FaceDetector
{
public:
	FaceDetector();
	bool Initialize();
	void DetectAndDraw(cv::Mat image);

	cv::Point CenterLastFace;

private:
	cv::CascadeClassifier classifier;
};
