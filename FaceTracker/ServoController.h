#include "Windows.h"

#include "opencv2/highgui/highgui.hpp"

class ServoController
{
public:
	ServoController();

	bool Connect(int port);
	void Disconnect();

	void IncreaseTilt();
	void DecreaseTilt();
	void IncreasePan();
	void DecreasePan();

	void ZeroInOnFace(cv::Mat& frame, cv::Point& faceCenter);

	bool SendByte(char byte);
	bool SendMessage(char * lpBuf, DWORD dwToWrite);

private:

	void TransmitTilt();
	void TransmitPan();

	HANDLE hComm;

	bool connected;
};