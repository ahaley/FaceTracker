#include <stdio.h>
#include "ServoController.h"


ServoController::ServoController()
{
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

	SendMessage("hello,world", 10);

	if (hComm == INVALID_HANDLE_VALUE) {
		return false;
	}


	return true;
}

void ServoController::Disconnect()
{
	CloseHandle(hComm);
}


bool ServoController::SendMessage(char * lpBuf, DWORD dwToWrite)
{
	OVERLAPPED osWrite = {0};
	DWORD dwWritten;
	DWORD dwRes;
	bool fRes;

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
