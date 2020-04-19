#pragma once
#include "StdAfx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Comms.h"

using namespace std;

class Server : public Comms {
public:
	Server() {

	};
	~Server() {

	};
	int Main();
	struct SERVICEPARAMETERS
	{
		LPVOID socket;
		Server* serverInstance;
	};
	int CreateServiceAddress(int port, SOCKET serverSocket) {
		sockaddr_in service = this->ServiceCreate(port);
		if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
			//need to do error handling
			cout << "bind() failed: " << WSAGetLastError() << endl;
			closesocket(serverSocket);
			WSACleanup();
			return 0;
		}
		else {
			//cout << "bind() is OK!" << endl;
			return 1;
		}
	}
	void ListenForClient(SOCKET serverSocket) {
		if (listen(serverSocket, 1) == SOCKET_ERROR)
			cout << "listen(): Error listening on socket " << WSAGetLastError() << endl;
		else
			cout << "Waiting for a client to connect..." << endl;
	}
private:
};

template <typename P>
DWORD WINAPI ServerThreadSender(void* param) {
	P* params = (P*)param;
	SOCKET socket = (SOCKET)params->socket;
	int byteCount;
	while (1) {
		Sleep(500);
		if (socket != INVALID_SOCKET) {
			byteCount = send(socket, (char *)params->serverInstance, sizeof(Server), 0);
			if (byteCount == SOCKET_ERROR) {
				cout << "Server send error " << WSAGetLastError() << endl;
				closesocket(socket);
				return -1;
			}
			else {
				cout << "-----  server sending :) -----" << endl;
				cin >> params->serverInstance->message;
			}
		}
	}
	closesocket(socket);
	return 0;
}


