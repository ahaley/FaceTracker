#include "Windows.h"

class ServoController
{
public:
	ServoController();

	bool Connect(int port);
	void Disconnect();
	bool SendMessage(char * lpBuf, DWORD dwToWrite);

private:
	HANDLE hComm;
};