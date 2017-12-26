//Client
#define _WINSOCK_DEPRECATED_NO_WARNINGS					//Disables API warnings
#pragma comment(lib, "ws2_32.lib")						//Linker to library

#include <WinSock2.h>									//Winsock functions, structures and definitions
#include <iostream>										//Input/output operations - slower than stdio.h, but more safe

using namespace std;

SOCKET Connection;

char buffer[256];										//Declares a buffer of chars

char *name = new char[15];
char *message = new char[200];

void ClientThread() {									//Checks for messages from the server
	while (true) {
		recv(Connection, buffer, sizeof(buffer), NULL);	//Receives messages from the server
		cout << buffer << endl;							//Prints out the message
	}
}

int main() {
	//Winsock startup
	WSAData wsaData;									//The structure contains info about Winsock implementation
	WORD DllVersion = MAKEWORD(2, 2);					//MAKEWORD is request for version 2.2 of Winsock
	if (WSAStartup(DllVersion, &wsaData) != 0) {		//Checks for errors in using WS2_32.dll
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR); //Shows a window error message
		exit(1);										//Exits the client
	}

	//Create Socket
	SOCKADDR_IN addr;									//Specifies a transport address and port for the AF_INET address family
	int sizeofaddr = sizeof(addr);						//Size of the structure addr
	addr.sin_addr.s_addr = inet_addr("192.168.38.182");	//IP address
	addr.sin_port = htons(17337);						//Port
	addr.sin_family = AF_INET;							//IPv4

	//Connect client socket to server
	Connection = socket(AF_INET, SOCK_STREAM, NULL);	//Initialize the SOCKET. SOCK_STREAM = TCP. 
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) {	//If no server is found
		MessageBoxA(NULL, "Failed to connect", "ERROR!", MB_OK | MB_ICONERROR); //Shows a window error message
		return 0;							
	}

	//Name of Client
	cout << "What's your name?" << endl;
	cin >> name;

	//Multithreading
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Creates a thread which runs the ClientThread function

	//Input from Client
	while (true) {
		cin.getline(buffer, sizeof(buffer));			//Enables input from client

		strcat_s(buffer, " wrote ");
		strcat_s(buffer, name);

		send(Connection, buffer, sizeof(buffer), NULL);	//Sends the message to the server
		Sleep(10);										//Sleeps for 10ms
	}
	return 0;
}