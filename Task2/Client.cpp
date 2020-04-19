#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"
#include "pch.h"
#include "Client.h"



int Client::Main() {
	SOCKET clientSocket;
	int port = 55555;

	SERVICEPARAMETERS serviceParams;
	this->DllFinder();

	clientSocket = this->SocketCreator();


	sockaddr_in clientService = this->ServiceCreate(port);

	try {
		if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
			cout << "Client: connect() - Failed to connect." << endl;
			WSACleanup();
			throw FailedToConnectToServerException();
		}
		else {
			cout << "Client: connecting to a host" << endl;
			serviceParams.socket = (LPVOID)clientSocket;
			serviceParams.instance = this;
			serviceParams.instanceSizeOf = sizeof(Client);

			CreateThread(NULL, 0, this->ClientThreadSender, &serviceParams, 0, 0);
		}
	}
	catch (FailedToConnectToServerException& e) {
		PrintException<FailedToConnectToServerException>(e);
	}
	catch (exception& e) {
		PrintException<exception>(e);
	}

	int byteCount = 0;
	while (true) {
		// lock app whilst threads are active - check for quit message
		Sleep(1000);
		SocketReceiver<SERVICEPARAMETERS>(serviceParams, (SOCKET)clientSocket, byteCount);
	}

	system("pause");
	WSACleanup();
	return 0;

}

DWORD WINAPI Client::ClientThreadSender(void* param) {
	SERVICEPARAMETERS* params = (SERVICEPARAMETERS*)param;
	SOCKET socket = (SOCKET)params->socket;
	Client client;
	int byteCount = 0;
	try {
		while (true) {
			Sleep(500);
			if (socket != INVALID_SOCKET) {
				byteCount = send(socket, (char *)&client, sizeof(Client), 0);
				if (byteCount == SOCKET_ERROR) {
					cout << "Server send error " << WSAGetLastError() << endl;
					closesocket(socket);
					throw FailedToSendException();
				}
				else {
					cout << "Please enter message:" << endl;
					cin >> client.message;
					if (client.message == "QUIT") {
						exit(0);
					}
				}
			}
		}
		closesocket(socket);
		return 0;
	}
	catch (FailedToSendException& e) {
		PrintException<FailedToSendException>(e);
	}
}