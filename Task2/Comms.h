#pragma once
#include <string>
#include <iostream>
#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

class Comms {
public:
	string hostName = "";
	string clientName = "";
	string message;
	int DllFinder() {
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
		};
	};
	template<typename T>
	int SocketReceiver(T instance, SOCKET socket);
	int SocketCreator() {
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
	sockaddr_in ServiceCreate(int port){
		sockaddr_in service;
		service.sin_family = AF_INET;
		InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
		service.sin_port = htons(port);
		return service;
	}
private:
};

template <typename T>
int Comms::SocketReceiver(T instance, SOCKET socket) {
	int byteCount;
	while (1) {
		Sleep(1000);
		if (socket != INVALID_SOCKET) {
			byteCount = recv(socket, (char *)&instance, sizeof(instance), 0);
			if (byteCount == SOCKET_ERROR) {
				cout << "send error " << WSAGetLastError() << endl;
				closesocket(socket);
				return -1;
			}
			else {
				cout << "-----" << &instance  << "-----" << endl;
				cout << "Last Message from " << &instance << ":  " << &instance->message << endl;
			}
		}
	}
	closesocket(socket);
	return 0;
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