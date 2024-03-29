/*
struct SimpleClient {
	

	bool connect();
	
	void send(const char* msg, int length);
	
private:
	char* ip_addr {nullptr};
	int port 	{0};
};

*/

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#include "simple-client.h"
#include <vector>

template <typename Key, typename Value>
struct Pair {
	Key k;
	Value v;
};


using SockPair = Pair<SimpleClient*, SOCKET*>;

static WSADATA wsaData;

static std::vector<SockPair> _internalSockets;




SimpleClient::SimpleClient(const char* ip, const char* port) : ip_addr { ip }, port {port} {		
    int iResult;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return;
    }
}

bool SimpleClient::connect() {
	int iResult;
	
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(ip_addr, 	port, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        return false;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL; ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            return false;
        }

        // Connect to server.
        iResult = ::connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        return false;
    }
	
	_internalSockets.push_back(SockPair{this, &ConnectSocket});
	
	printf("New socket added: %d\n", _internalSockets.size());
	
	return true;
}

void SimpleClient::send(const char* msg, int length) {
	
}

SimpleClient::~SimpleClient() {
	for (auto& p : _internalSockets) {
		if (p.k == this && p.v != nullptr)
			closesocket(*p.v);
	}
	
	WSACleanup();
}