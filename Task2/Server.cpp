#include "StdAfx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"
#include "Server.h"


struct PARAMETERS
{
	LPVOID socket;
	Server * serverInstance;
};

// needs to move to abstracted class
DWORD WINAPI ServerThreaSender(void* param) {
	PARAMETERS* params = (PARAMETERS*)param;
	SOCKET socket = (SOCKET)params->socket;
	int byteCount;
	while (1) {
		Sleep(1000);
		if (socket != INVALID_SOCKET) {
			byteCount = send(socket, (char *)params->serverInstance, sizeof(Server), 0);
			if (byteCount == SOCKET_ERROR) {
				cout << "Server send error " << WSAGetLastError() << endl;
				closesocket(socket);
				return -1;
			}
			else {
				cin >> params->serverInstance->message;
			}
		}
	}
	closesocket(socket);
	return 0;
}

DWORD WINAPI ServerThreadReciever(void* param) {
	PARAMETERS* params = (PARAMETERS*)param;
	SOCKET socket = (SOCKET)params->socket;
	int byteCount;
	while (1) {
		Sleep(1000);
		if (socket != INVALID_SOCKET) {
			byteCount = recv(socket, (char *)params->serverInstance, sizeof(Server), 0);
			if (byteCount == SOCKET_ERROR) {
				cout << "Server send error " << WSAGetLastError() << endl;
				closesocket(socket);
				return -1;
			}
			else {
				cout << "Last Message:  " << params->serverInstance->message << endl;
			}
		}
	}
	closesocket(socket);
	return 0;
}
//

int Server::Main() {
	SOCKADDR_STORAGE from;
	int retval, fromlen, socket_type;
	char servstr[NI_MAXSERV], hoststr[NI_MAXHOST];


	SOCKET serverSocket, acceptSocket;
	int port = 55555;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0) {
		//need to do error handling
		cout << "ERROR: The Winsock dll not found!" << endl;
		return 0;
	}
	else {
		//cout << "The Winsock dll found!" << endl;
		//cout << "The status: " << wsaData.szSystemStatus << endl;
	}

	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		//need to do error handling
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else {
		//cout << "socket() is OK!" << endl;
	}


	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		//need to do error handling
		cout << "bind() failed: " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else {
		//cout << "bind() is OK!" << endl;
	}

	if (listen(serverSocket, 1) == SOCKET_ERROR)
		cout << "listen(): Error listening on socket " << WSAGetLastError() << endl;
	else
		cout << "Waiting for a client to connect..." << endl;
	Server server;
	PARAMETERS params;

	DWORD threadId;
	acceptSocket = accept(serverSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET) {
		//need to do error handling
		cout << stderr << " accept failed: " << WSAGetLastError() << endl;
		return -1;
	}
	else {
		params.socket = (LPVOID)acceptSocket;
		params.serverInstance = &server;

		cout << "Server: Client Connected!" << endl;
		CreateThread(NULL, 0, ServerThreaSender, &params, 0, &threadId);
		CreateThread(NULL, 0, ServerThreadReciever, &params, 0, &threadId);
	}

	while (true) {
		// lock whilst threads are active
	}

	system("pause");
	WSACleanup();


	return 0;
}
