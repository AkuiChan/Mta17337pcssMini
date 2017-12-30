//Client

#define _WINSOCK_DEPRECATED_NO_WARNINGS					//Disables API warnings
#pragma comment(lib, "ws2_32.lib")						//Linker to library

#include <WinSock2.h>									//Winsock functions, structures and definitions
#include <iostream>										//Input/output operations - slower than stdio.h, but more safe

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);		//Retrieves a handle for the standard output. This will be used late to change text colors
SOCKET Connection;										//Create a socket object

char buffer[256];										//Declares a buffer of chars
	
char *name = new char[15];								//Buffer for name
char message[200];										//Buffer for message

void ClientThread() {									//Checks for messages from the server
	//First message from the server
	SetConsoleTextAttribute(hConsole, 10);				//Green Color
	recv(Connection, buffer, sizeof(buffer), NULL);		//receive the first message from server
	cout << "\r" << buffer << endl;						//Print out the message
	
	//Recieves chat messages from the server
	while (true) {
		recv(Connection, buffer, sizeof(buffer), NULL);	//Receives messages from the server
		SetConsoleTextAttribute(hConsole, 11);			//Cyan color
		cout << "\r" << buffer << endl;					//Prints out the message
		SetConsoleTextAttribute(hConsole, 14);			//Yellow Color
		cout << "Me" << ": ";							//If the client is stuck in a cin, this will be used to re-cout Me
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
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");		//IP address
	addr.sin_port = htons(17337);						//Port
	addr.sin_family = AF_INET;							//IPv4

	//Connect client socket to server
	Connection = socket(AF_INET, SOCK_STREAM, NULL);	//Initialize the SOCKET. SOCK_STREAM = TCP. 
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) {	//If no server is found
		MessageBoxA(NULL, "Failed to connect", "ERROR!", MB_OK | MB_ICONERROR); //Shows a window error message
		return 0;							
	}

	//Name of Client
	SetConsoleTextAttribute(hConsole, 10);				//Green color
	cout << "______________________________________\n\n Name Selection for MTA17337's chat\n______________________________________\nName: "; //Asks the user to input the name
	SetConsoleTextAttribute(hConsole, 14);				//Yellow color
	cin >> name;										//Input name into variable "name"
	system("cls");										//Clears the text in console

	//Multithreading
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Creates a thread which runs the ClientThread function

	//Input from Client
	while (true) {
		Sleep(15);
		SetConsoleTextAttribute(hConsole, 14);			//Yellow Color
		cout << "\rMe" << ": ";							//Adds name before message
		
		cin.getline(message, sizeof(message));			//Enables input from client (Message)
		
		memset(buffer, 0, sizeof(buffer));				//Clears the values in the char array (buffer)
		strcat_s(buffer, name);							//Sets values from name into buffer
		strcat_s(buffer, ": ");							//Sets a colon value between name and the message
		strcat_s(buffer, message);						//Sets the message after the colon

		if (message[0] == NULL) {						//Checks if there is a whitespace in the first letter. This is used to bypass a bug about sending a message when the client enters a name
			continue;									//Resets the loop
		}

		send(Connection, buffer, sizeof(buffer), NULL);	//Sends the message to the server
		Sleep(10);										//Sleeps for 10ms
	}
	return 0;
}