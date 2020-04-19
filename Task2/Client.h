#pragma once
#include "Comms.h"


using namespace std;

class Client : public Comms {
public:
	Client() {
	};
	~Client() {
	}
	struct SERVICEPARAMETERS {
		LPVOID socket;
		Client * clientInstance;
	};
	int Main();
private:
};

template <typename T>
DWORD WINAPI ClientThreadSender(void* param) {
	T* params = (T*)param;
	SOCKET socket = (SOCKET)params->socket;

	int byteCount;
	while (1) {
		Sleep(1000);
		if (socket != INVALID_SOCKET) {
			byteCount = send(socket, (char *)&params, sizeof(Client), 0);
			if (byteCount == SOCKET_ERROR) {
				cout << "Server send error " << WSAGetLastError() << endl;
				closesocket(socket);
				return -1;
			}
			else {
				cout << "----- client sending :) -----" << endl;
				cin >> params->clientInstance->message;
			}
		}
	}
	closesocket(socket);
	return 0;
}
