#pragma once
#include <iostream>
#include "Comms.h"
using namespace std;

class Client : public Comms {
public:
	Client() {
	};
	~Client() {
	};
	int Main();
private:
};
