#include <stdio.h>
#include "ServoController.h"

static char servoTiltPosition = 90;
static char servoPanPosition = 90;
static char tiltChannel = 0;
static char panChannel = 1;

static int servoStepSize = 1;

ServoController::ServoController()
{
	connected = false;
}

bool ServoController::Connect(int port)
{
	char comm_str[6];
	sprintf_s(comm_str, 6, "COM%d", port);

	hComm = CreateFile(comm_str,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0, 
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		0);


	TransmitTilt();
	TransmitPan();

	if (hComm == INVALID_HANDLE_VALUE) {
		return false;
	}

	connected = true;
	return true;
}

void ServoController::Disconnect()
{
	if (connected) {
		CloseHandle(hComm);
		connected = false;
	}
}

void ServoController::TransmitTilt()
{
	SendByte(tiltChannel);
	SendByte(servoTiltPosition);
}

void ServoController::TransmitPan()
{
	SendByte(panChannel);
	SendByte(servoPanPosition);
}

static int count = 0;

void ServoController::IncreaseTilt()
{
	printf("%d: increasing tilt\n", count++);
	servoTiltPosition += servoStepSize;
	TransmitTilt();
}

void ServoController::DecreaseTilt()
{
	printf("%d: decreasing tilt\n", count++);
	servoTiltPosition -= servoStepSize;
	TransmitTilt();
}

void ServoController::IncreasePan()
{
	printf("%d: increasing pan\n", count++);

	servoPanPosition += servoStepSize;
	TransmitPan();
}

void ServoController::DecreasePan()
{
	printf("%d: decreasing pan\n", count++);
	servoPanPosition -= servoStepSize;
	TransmitPan();
}


bool ServoController::SendByte(char byte)
{
	char buffer[2] = { byte, 0 } ;
	return SendMessage(buffer, 1);
}

bool ServoController::SendMessage(char * lpBuf, DWORD dwToWrite)
{
	OVERLAPPED osWrite = {0};
	DWORD dwWritten;
	DWORD dwRes;
	bool fRes;

	if (!connected)
		return false;

	// Create this write operation's OVERLAPPED structure's hEvent.
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osWrite.hEvent == NULL)
		// error creating overlapped event handle
			return false;

	// Issue write.
	if (!WriteFile(hComm, lpBuf, dwToWrite, &dwWritten, &osWrite)) {
		if (GetLastError() != ERROR_IO_PENDING) { 
			// WriteFile failed, but isn't delayed. Report error and abort.
			fRes = false;
		}
		else {
			// Write is pending.
			dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
			switch(dwRes)
			{
				// OVERLAPPED structure's event has been signaled. 
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(hComm, &osWrite, &dwWritten, FALSE))
					fRes = false;
				else
					// Write operation completed successfully.
					fRes = true;
				break;

			default:
				// An error has occurred in WaitForSingleObject.
				// This usually indicates a problem with the
				// OVERLAPPED structure's event handle.
				fRes = false;
				break;
			}
		}
	}
	else
		// WriteFile completed immediately.
		fRes = true;

	CloseHandle(osWrite.hEvent);
	return fRes;
}

void ServoController::ZeroInOnFace(cv::Mat& frame, cv::Point& faceCenter)
{
	static const int slop = 10;
	int centerCol = frame.cols >> 1;
	if (faceCenter.x > centerCol + slop) {
		IncreasePan();
	}
	else if (faceCenter.x < centerCol - slop) {
		DecreasePan();
	}

	int centerRow = frame.rows >> 1;
	if (faceCenter.y > centerRow + slop) {
		DecreaseTilt();
	}
	if (faceCenter.y < centerRow - slop) {
		IncreaseTilt();
	}
}