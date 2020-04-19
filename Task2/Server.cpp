#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"
#include "pch.h"
#include "Server.h"
#include "Exceptions.h"


int Server::Main() {
	SOCKET serverSocket, acceptSocket;
	int port = 55555;
	//Find the DLL WinSock File

	this->DllFinder();

	//Create the socket
	
	serverSocket = this->SocketCreator();

	//Create the service address
	this->CreateServiceAddress(port, serverSocket);

	//Listen for client
	this->ListenForClient(serverSocket);


	SERVICEPARAMETERS serviceParams;
	try {


		acceptSocket = accept(serverSocket, NULL, NULL);
		if (acceptSocket == INVALID_SOCKET) {
			//need to do error handling
			cout << stderr << " accept failed: " << WSAGetLastError() << endl;
			throw FailedToAcceptViaSocketException();
			return -1;
		}
		else {
			serviceParams.socket = (LPVOID)acceptSocket;
			serviceParams.instance = this;
			serviceParams.instanceSizeOf = sizeof(Server);

			cout << "Server: Client Connected!" << endl;
			CreateThread(NULL, 0, this->ServerThreadSender, &serviceParams, 0, 0);
		}
	}
	catch (FailedToAcceptViaSocketException& e) {
		PrintException<FailedToAcceptViaSocketException>(e);
	}
	int byteCount = 0;
	while (true) {
		// lock whilst threads are active
		Sleep(1000);
		SocketReceiver<SERVICEPARAMETERS>(serviceParams, (SOCKET)acceptSocket, byteCount);
	}

	system("pause");
	WSACleanup();

	return 0;
}

DWORD WINAPI Server::ServerThreadSender(void* param) {
	SERVICEPARAMETERS* params = (SERVICEPARAMETERS*)param;
	SOCKET socket = (SOCKET)params->socket;
	Server server;
	int byteCount = 0;
	try {
		while (true) {
			Sleep(500);
			byteCount = send(socket, (char *)&server, sizeof(Server), 0);
			if (byteCount == SOCKET_ERROR) {
				cout << "Server send error " << WSAGetLastError() << endl;
				closesocket(socket);
				throw FailedToSendException();
			}
			else {
				cout << "Please enter message:" << endl;
				cin >> server.message;
				if (server.message == "QUIT") {
					exit(0);
				}
			}
		}
		closesocket(socket);
		return 0;
	}
	catch (FailedToSendException& e) {
		PrintException<FailedToSendException>(e);
	}
	catch (exception& e) {
		PrintException<exception>(e);
	}
}

void Server::ListenForClient(SOCKET serverSocket) {
	try {
		if (listen(serverSocket, 1) == SOCKET_ERROR) {
			cout << "listen(): Error listening on socket " << WSAGetLastError() << endl;
			throw UnableToListenOnSocketException();
		}
		else {
			cout << "Waiting for a client to connect..." << endl;
		}
	}
	catch (UnableToListenOnSocketException& e) {
		PrintException<UnableToListenOnSocketException>(e);
	}
	catch (exception& e) {
		PrintException<exception>(e);
	}
}

int Server::CreateServiceAddress(int port, SOCKET serverSocket) {
	try {
		sockaddr_in service = this->ServiceCreate(port);
		if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
			//need to do error handling
			cout << "bind() failed: " << WSAGetLastError() << endl;
			closesocket(serverSocket);
			WSACleanup();
			throw FailedToBindException();
			return 0;
		}
		else {
			//cout << "bind() is OK!" << endl;
			return 1;
		}
	}
	catch (FailedToBindException& e) {
		PrintException<FailedToBindException>(e);
	}
	catch (exception& e) {
		PrintException<exception>(e);
	}
}