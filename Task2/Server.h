#pragma once
#include "StdAfx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Comms.h"

using namespace std;

class Server : public Comms {
public:
	Server() {
		clientNameFrom = "Client";
	};
	~Server() {

	};
	int Main();
	struct SERVICEPARAMETERS
	{
		LPVOID socket;
		Server* instance;
		int instanceSizeOf;
	};
	int CreateServiceAddress(int port, SOCKET serverSocket);
	void ListenForClient(SOCKET serverSocket);
	static DWORD WINAPI ServerThreadSender(void* param);
private:
};




