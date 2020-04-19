#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"
#include "pch.h"
#include "Client.h"



int Client::Main() {
	SOCKET clientSocket;
	int port = 55555;
	Client client;
	SERVICEPARAMETERS serviceParams;
	this->DllFinder();

	clientSocket = this->SocketCreator();

	sockaddr_in clientService = this->ServiceCreate(port);

	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		cout << "Client: connect() - Failed to connect." << endl;
		WSACleanup();
		return 0;
	}
	else {
		DWORD threadId;
		cout << "Client: connecting to a host" << endl;
		serviceParams.socket = (LPVOID)clientSocket;
		serviceParams.clientInstance = &client;

		CreateThread(NULL, 0, ClientThreadSender<SERVICEPARAMETERS>, &serviceParams, 0, &threadId);
		
	}

	while (true) {
		// lock app whilst threads are active - check for quit message
		SOCKET socket = (SOCKET)clientSocket;
		SocketReceiver(&client, socket);
	}

	system("pause");
	WSACleanup();
	return 0;

}