#pragma once
#include <iostream>
#include "Comms.h"

using namespace std;

class Server : public Comms {
public:
	Server() {
	};
	~Server() {
	};
	int Main();
	Server * serverInstance;
private:
};
