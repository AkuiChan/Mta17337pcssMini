//Server

#pragma comment(lib, "ws2_32.lib")						//Tells the linker to add ws2_32.lib to the list of additional library dependencies.
#define _WINSOCK_DEPRECATED_NO_WARNINGS					//Disables deprecated API warnings.
#include <WinSock2.h>									//Includes Winsock 2 header files to enable use of the Winsock API.
#include <iostream>										//Allows the system to read and write to the standard input/output streams.

using namespace std;									//Allows std namespace qialifiers to be omitted in the code; e.g. so "std::cout" can be written as just "cout".

SOCKET Connections[100];								//An array that can store up to 100 sockets.
int ConnectionCounter = 0;								//An integer used to count the number of clients connected to the server.

//Function called as a secondary thread; handles clients connected to the server.
void ClientHandlerThread(int index) {
	char buffer[256];
	while (true) {
		int iResult = recv(Connections[index], buffer, sizeof(buffer), NULL);
		if (iResult == SOCKET_ERROR)
			continue;
		for (int i = 0; i < ConnectionCounter; i++) {
			if (i == index)
				continue;
			send(Connections[i], buffer, sizeof(buffer), NULL);
		}
	}
}

int main() {
	WSAData wsaData;									//Structure that contains data about Windows Sockets.
	WORD DllVersion = MAKEWORD(2, 2);					//Creates a 2 byte WORD value for use as a return value from a window procedure.

	//Function that takes a WORD value and a data structure pointer. Returns 0 if successful. Returns an error code if not.
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	//Setting socket address values.
	SOCKADDR_IN addr;									//Specifies endpoint address to which to connect a socket.
	int addrlen = sizeof(addr);							//Interger value that holds the length of "addr".
	addr.sin_addr.s_addr = inet_addr("192.168.38.182");	//Specifies the IP address of the socket.
	addr.sin_port = htons(17337);						//Specifies the port of the socket.
	addr.sin_family = AF_INET;							//Specifies the internet protocol family.

	//Sets up a socket that listens for clients.
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);//Sets up socket.
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));		//Associates address with the socket.
	listen(sListen, SOMAXCONN);							//Places the socket in a state of listening for incoming connections.

	//Prints text to the console. First thing that the user sees on-screen.
	cout << "Server is online" << endl;

	//Looped function used to accept new connections. Can take up to 100 clients.
	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);
		if (newConnection == 0) {
			cout << "Failed to accept the client's connection" << endl;
		}
		else {
			cout << "Client number " << i + 1 << " connected!" << endl;
			char MOTD[256] = "Welcome! This is the message of the day";
			send(newConnection, MOTD, sizeof(MOTD), NULL);
			Connections[i] = newConnection;
			ConnectionCounter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL);
		}
	}

	system("pause");

	return 0;
}