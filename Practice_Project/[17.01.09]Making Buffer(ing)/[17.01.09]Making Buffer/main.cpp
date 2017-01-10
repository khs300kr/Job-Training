#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <string>
#include "CircularBuf.h"

#define BUFFER_SIZE 512
#define SERVERPORT 8000

using namespace std;

enum PacketID
{
	StringData,
	Two,
	Three,
};

struct PacketHeader
{
	PacketID	m_ID;
	u_short		m_size;
};

struct TempPacket : public PacketHeader
{
	string str_temp;
};


void main()
{
	// Winsock Init
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(1);

	// Socket
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) cout << "listen" << endl;

	// Bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	if (bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr) == SOCKET_ERROR)) cout << "bind" << endl;

	// Listen
	if (listen(listen_sock, SOMAXCONN) == SOCKET_ERROR) cout << "listen" << endl;

	// Data Virables
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	CCircularBuf buf;


	while (true)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) cout << "accept" << endl;
		
		// print client info
		cout << "[TCP Server]Client IP : " << inet_ntoa(clientaddr.sin_addr) << endl;
		cout << "[TCP Server]Client Port : " << ntohs(clientaddr.sin_port) << endl;
		//

		while (true)
		{
			recv(client_sock,)
		}

	}

	closesocket(listen_sock);
	WSACleanup();
}


