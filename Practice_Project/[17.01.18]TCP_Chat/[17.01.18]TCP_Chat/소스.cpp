#pragma comment(lib,"Ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h> // getaddrinfo()
#include <thread>	  // C+11 multithread
#include <string>
#include <vector>

#define IP_ADDR "127.0.0.1"		// loop back
#define SERVER_PORT "8000"
#define BUFSIZE 512
#define MAX_CLIENT 1000			// 동접 1000
#define NOTYET_CONNECT -1

using namespace std;

typedef struct tagClientType
{
	int		m_id;
	SOCKET	m_socket;
	tagClientType()
		: m_id(-1), m_socket(INVALID_SOCKET) {}
	~tagClientType() {}

}CLIENT_TYPE;

typedef struct tagClientAddr
{
	SOCKADDR_IN m_addr;
	int			m_iLen;
	tagClientAddr() 
	{ 	m_iLen = sizeof(m_addr); }
}CLIENT_ADDR;


void process_client(CLIENT_TYPE& _client, vector<CLIENT_TYPE>& vClient, thread& thread)
{
	string strMsg{};
	char buf[BUFSIZE]{};

	while (true)
	{

	}
}

void main()
{
	// Winsock Initializing
	cout << "Initializing Winsock..." << endl;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { cout << "< Winsock Intializing failed >" << endl; exit(1);}

	// Setup hints
	cout << "Setting up Server ( 주소 설정, 서버소켓 생성 )..." << endl;
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;		// The socket address will be used in a call to the bind function.(msdn)
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;

	// Create server socket
	addrinfo*	server;
	getaddrinfo(IP_ADDR, SERVER_PORT, &hints, &server);
	SOCKET server_socket = socket(server->ai_family, server->ai_socktype, 0);
	if (server_socket == INVALID_SOCKET) { cout << "server_socket is invalid" << endl; exit(1); }

	// Setup socket options
	bool optval = true;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));	// 소켓코드가 처리하는 옵션 - (프로토콜 독립적 성격)
	setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));	// 프로토콜구현 코드가 처리하는 옵션 - Reduce response time but increase traffic

	cout << "Binding socket..." << endl;
	bind(server_socket, &server->ai_addr, (int)server->ai_addrlen);

	cout << "Listening..." << endl;
	listen(server_socket, SOMAXCONN);
	
	// 통신변수
	vector<CLIENT_TYPE> client(MAX_CLIENT);		// key-value? trying map?
	thread client_thread[MAX_CLIENT];
	CLIENT_ADDR client_addr;
	int		iIndex{};
	int		iClient_Number{};
	string	strMsg{};

	while (true)
	{
		SOCKET sTemp = accept(server_socket, (SOCKADDR*)&client_addr.m_addr, &client_addr.m_iLen);
		if (sTemp == INVALID_SOCKET) continue;
		cout << " [ Accept - IP : " << inet_ntoa(client_addr.m_addr.sin_addr) << ", Port : " << ntohs(client_addr.m_addr.sin_port) << " ] " << endl;

		// 다음 클라이언트를 위해 템프 id 생성?
		iIndex = NOTYET_CONNECT;
		for (int i = 0; i < MAX_CLIENT; ++i)
		{
			if (client[i].m_socket == INVALID_SOCKET && iIndex == NOTYET_CONNECT)
			{
				client[i].m_socket = sTemp;
				client[i].m_id = i;
				iIndex = i;
			}
			
			if (client[i].m_socket != INVALID_SOCKET)
				++iClient_Number;							// 동접수?
		}
	


		if (iIndex == NOTYET_CONNECT)
		{
			strMsg = "Server is Full";
			send(sTemp, strMsg.c_str(), strMsg.size(), 0);
			cout << strMsg << endl;
		}
		else
		{
			// id를 클라이언트에게 보낸다?
			cout << "Client[" << client[iIndex].m_id << "]" << "접속" << endl;
			strMsg = to_string(client[iIndex].m_id);
			send(client[iIndex].m_socket, strMsg.c_str(), strMsg.size(), 0);

			// 해당 클라이언트를 위한 스레드 프로세스를 만든다.
			client_thread[iIndex] = thread(
				process_client, 
				std::ref(client[iIndex]),
				std::ref(client),
				std::ref(client_thread[iIndex]));		
		}
	} // end while

	// 서버(listening)소켓 종료
	closesocket(server_socket);

	// 스레드 , 클라이언트 소켓 종료
	for (int i = 0; i < iClient_Number; ++i)
	{
		client_thread[i].detach();		// 프로세스와 스레드 분리.
		closesocket(client[i].m_socket);
	}

	WSACleanup();
}


