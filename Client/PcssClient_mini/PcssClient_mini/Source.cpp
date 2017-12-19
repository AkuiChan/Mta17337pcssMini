#include <winsock2.h>									//winsock functions, structures and definitions
#include <ws2tcpip.h>									//used to retrieve IP addresses
#include <stdio.h>										//input/output operations - faster than iostream, but less safe

#define DEFAULT_PORT "17337"
	
#pragma comment(lib, "Ws2_32.lib")						//linker to library (works like openCV maybe)

WSADATA wsaData;										//the structure contains info about Winsock implementation
int iResult;											//error handling

int main(char **argv) { /////////////// FIND OUT WHAT ARGV IS ///////////////
	//Initialize Winsock -  WSAStartup called to initiate use of WS2_32.dll - MAKEWORD is request for version 2.2 of Winsock - &wsaData address
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	//WSAStartup - 0 is successful
	if (iResult != 0) {									//If WSAStartup has failed
		printf("WSAStartup failed: %d\n", iResult);		//print error
		return 1;
	}

	//CREATE SOCKET//
	struct addrinfo *result = NULL, *ptr = NULL, hints;	//info about client address, host will get info

	ZeroMemory(&hints, sizeof(hints));					//fills hints block with zeroes
	hints.ai_family = AF_UNSPEC;						//unspecified whether it's IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;					//socket type requested to be a stream type
	hints.ai_protocol = IPPROTO_TCP;					//TCP protocol - used mostly for chats, emails etc.

	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);	//getaddrinfo returns integer value
	if (iResult != 0) {									//if error
		printf("getaddrinfo failed %d\n", iResult);		//print error
		WSACleanup();									//terminates use of Ws2_32.lib
		return 1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;				//create socket object
	
	//attempt to connect to the first address returned by the call to getaddrinfo
	ptr = result;

	//create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {				//if error
		printf("Error at socket(): %ld\n", WSAGetLastError());	//print error
		freeaddrinfo(result);							//frees resources result
		WSACleanup();									//terminates use of Ws2_32.lib
		return 1;
	}

	//CONNECT TO A SOCKET//
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);	//tries to connect to server socket
	if (iResult == SOCKET_ERROR) {						//if error
		closesocket(ConnectSocket);						//closes the socket
		ConnectSocket = INVALID_SOCKET;					//set the socket to be invalid
	}

	freeaddrinfo(result);								//frees resources result

	if (ConnectSocket == INVALID_SOCKET) {				//if the socket value is invalid
		printf("Unable to connect to server! \n");		//print error
		WSACleanup();									//terminates use of Ws2_32.lib
		return 1;
	}

	return 0;
}