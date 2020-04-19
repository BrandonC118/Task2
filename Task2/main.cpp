#include "pch.h"
#include "Server.h"
#include "Client.h"
#include "stdafx.h"
#include <windows.h>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS
#define SERVER false
using namespace std;

int main() {
	if (SERVER) {
		Server server;
		server.Main();
	}
	else {
		Client client;
		client.Main();
	}
}

