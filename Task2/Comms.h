#pragma once
#include <string>
#include <iostream>
#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Exceptions.h"

using namespace std;

class Comms {
public:
	string hostName = "";
	string clientNameFrom = "";
	string message;
	int DllFinder() {
		try {
			WSADATA wsaData;
			int wsaerr;
			WORD wVersionRequested = MAKEWORD(2, 2);
			wsaerr = WSAStartup(wVersionRequested, &wsaData);
			if (wsaerr != 0) {
				cout << "The Winsock dll not found!" << endl;
				throw DDLNotfoundException();
			}
			else {
				cout << "The Winsock dll found!" << endl;
				cout << "The status: " << wsaData.szSystemStatus << endl;
				return 1;
			};
		}
		catch (DDLNotfoundException& e) {
			PrintException(e);
		}
		catch (exception& e) {
			PrintException(e);
		}
	}
	template<typename T>
	void SocketReceiver(T instance, SOCKET socket, int byteCount);
	int SocketCreator() {
		try {
			SOCKET instanceSocket = INVALID_SOCKET;
			try {
				instanceSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			}
			catch (exception& e) {
				throw e;
			}
			if (instanceSocket == INVALID_SOCKET) {
				cout << "Error at socket(): " << WSAGetLastError() << endl;
				WSACleanup();
				throw InvalidSocketException();
			}
			else {
				cout << "socket() is OK!" << endl;
				return instanceSocket;
			}
		}
		catch (InvalidSocketException& e) {
			PrintException<InvalidSocketException>(e);
		}
		catch (exception& e) {
			PrintException<exception>(e);
		}

	}
	sockaddr_in ServiceCreate(int port){
			sockaddr_in service;
			service.sin_family = AF_INET;
			InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
			service.sin_port = htons(port);
			return service;
	}
	template <typename T>
	static void PrintException(T e) {
		cout << "Error : " << e.what() << endl;
	}
private:
};

template <typename T>
inline void Comms::SocketReceiver(T params, SOCKET socket, int byteCount ) {
	try {
		if (socket != INVALID_SOCKET) {
			byteCount = recv(socket, (char *)params.instance, params.instanceSizeOf, 0);
			if (byteCount == SOCKET_ERROR) {
				cout << "send error " << WSAGetLastError() << endl;
				closesocket(socket);
				throw FailedToRecieveException();
			}
			else {
				if (params.instance->message != "") {
					cout << "Last Message from " << clientNameFrom << ":  " << params.instance->message << endl;
				}
			}
		}
		else {
			throw InvalidSocketException();
		}
	}
	catch (InvalidSocketException& e) {
		PrintException<InvalidSocketException>(e);

	}
	catch (FailedToRecieveException& e) {
		PrintException<FailedToRecieveException>(e);
		exit(0);
	}

}	
/*
 int Comms::DllFinder() {
	WSADATA wsaData;
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
		return 1;
	}
}*/
/*
int Comms::SocketCreator() {
	SOCKET instanceSocket = INVALID_SOCKET;
	instanceSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (instanceSocket == INVALID_SOCKET) {
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else {
		cout << "socket() is OK!" << endl;
		return instanceSocket;
	}
}
*/