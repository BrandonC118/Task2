#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"
#include "pch.h"
#include "Client.h"

struct PARAMETERS
{
	LPVOID socket;
	Client * clientInstance;
};

// needs to move the to abstracted class
DWORD WINAPI ClientThreaSender(void* param) {
	PARAMETERS* params = (PARAMETERS*)param;
	SOCKET socket = (SOCKET)params->socket;
	int byteCount;
	while (1) {
		Sleep(1000);
		if (socket != INVALID_SOCKET) {
			byteCount = send(socket, (char *)params->clientInstance, sizeof(Client), 0);
			if (byteCount == SOCKET_ERROR) {
				cout << "Server send error " << WSAGetLastError() << endl;
				closesocket(socket);
				return -1;
			}
			else {
				cin >> params->clientInstance->message;
			}
		}
	}
	closesocket(socket);
	return 0;
}

DWORD WINAPI ClientThreadReciever(void* param) {
	PARAMETERS* params = (PARAMETERS*)param;
	SOCKET socket = (SOCKET)params->socket;
	int byteCount;
	while (1) {
		Sleep(1000);
		if (socket != INVALID_SOCKET) {
			byteCount = recv(socket, (char *)params->clientInstance, sizeof(Client), 0);
			if (byteCount == SOCKET_ERROR) {
				cout << "Server send error " << WSAGetLastError() << endl;
				closesocket(socket);
				return -1;
			}
			else {
				cout << "Last Message:  " << params->clientInstance->message << endl;
			}
		}
	}
	closesocket(socket);
	return 0;
}




int Client::Main() {
	SOCKET clientSocket;
	int port = 55555;
	WSADATA wsaData;
	Client client;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0) {
		cout << "The Winsock dll not found!" << endl;
		return 0;
	}
	else {
		cout << "The Winsock dll found!" << endl;
		cout << "The status: " << wsaData.szSystemStatus << endl;
	}

	clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else {
		cout << "socket() is OK!" << endl;
	}

	DWORD threadId;
	sockaddr_in clientService;
	PARAMETERS params;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		cout << "Client: connect() - Failed to connect." << endl;
		WSACleanup();
		return 0;
	}
	else {
		cout << "Client: connecting to a host" << endl;
		params.socket = (LPVOID)clientSocket;
		params.clientInstance = &client;

		CreateThread(NULL, 0, ClientThreadReciever, &params, 0, &threadId);
		CreateThread(NULL, 0, ClientThreaSender, &params, 0, &threadId);
	}

	while (true) {
		// lock app whilst threads are active - check for quit message
	}

	system("pause");
	WSACleanup();
	return 0;

}


