//Server

#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

using namespace std;

SOCKET Connections[100];
int ConnectionCounter = 0;

void ClientHandlerThread(int index) {
	int bufferlength;
	while (true) {
		recv(Connections[index], (char*)&bufferlength, sizeof(int), NULL);
		char * buffer = new char[bufferlength];
		recv(Connections[index], buffer, bufferlength, NULL);
		for (int i = 0; i < ConnectionCounter; i++) {
			if (i == index)
				continue;
			send(Connections[i], (char*)&bufferlength, sizeof(int), NULL);
			send(Connections[i], buffer, bufferlength, NULL);
		}
		delete[] buffer;

	}
}

int main() {

	//Winsock startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 2);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("100.72.27.76");
	addr.sin_port = htons(17337);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);

		if (newConnection == 0) {
			cout << "Failed to accept the client's connection" << endl;
		}

		else {
			cout << "Client number " << i + 1 << " connected!" << endl;
			string MOTD = "Welcome! This is the message of the day"; //MAAAAAAA
			int MOTDLength = MOTD.size();
			send(newConnection, (char*)&MOTDLength, sizeof(int), NULL);
			send(newConnection, MOTD.c_str(), MOTDLength, NULL);
			Connections[i] = newConnection;
			ConnectionCounter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL);
		}
	}

	system("pause");

	return 0;
}