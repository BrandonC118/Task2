#pragma once
#include "Comms.h"


using namespace std;

class Client : public Comms {
public:
	Client() {
		clientNameFrom = "Server";
	};
	~Client() {
	}
	struct SERVICEPARAMETERS {
		LPVOID socket;
		Client * instance;
		int instanceSizeOf;
	};
	int Main();
	static DWORD WINAPI ClientThreadSender(void* param);
private:
};


