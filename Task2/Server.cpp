#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"
#include "pch.h"
#include "Server.h"


int Server::Main() {
	SOCKET serverSocket, acceptSocket;
	int port = 55555;

	this->DllFinder();

	serverSocket = this->SocketCreator();

	this->CreateServiceAddress(port, serverSocket);

	this->ListenForClient(serverSocket);
	Server server;
	SERVICEPARAMETERS serviceParams;

	acceptSocket = accept(serverSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET) {
		//need to do error handling
		cout << stderr << " accept failed: " << WSAGetLastError() << endl;
		return -1;
	}
	else {
		DWORD tID;
		serviceParams.socket = (LPVOID)acceptSocket;
		serviceParams.serverInstance = &server;

		cout << "Server: Client Connected!" << endl;
		CreateThread(NULL, 0, ServerThreadSender<SERVICEPARAMETERS>, &serviceParams, 0, &tID);
	}

	while (true) {
		// lock whilst threads are active
		SOCKET socket = (SOCKET)acceptSocket;
		SocketReceiver(&server, socket);
	}

	system("pause");
	WSACleanup();


	return 0;
}