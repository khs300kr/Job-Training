#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>

using namespace std;

#define SERVERPORT 9000
#define BUFSIZE 128
//
// Packet
enum ECharType
{
	// Tanker
	Rine,
	Diva,
	// Dealer
	Genji,
	Hanzo,
	Para,
	// Healer
	Lucio,
	Mersi,
};

enum EPacketType
{
	ECharInfo,
	ECharMove,
};

struct PacketHeader
{
	WORD				m_Size;
	EPacketType			m_PacketType;
};

struct PCharInfo : public PacketHeader
{
	ECharType			m_iCharSelect;
	int					m_iPlayerNum;
};

// PacketUtil
template<class T>
void PacketEncode(char* buf, T& data)
{
	memcpy(buf, &data, sizeof(T));
}
template<class T>
void PacketDecode(char*buf, T& data)
{
	memcpy(&data, buf, sizeof(T));
}
//
int recvn(SOCKET S, char *buf, int len, int flags)
{
	int received{};
	char* ptr = buf;
	int left = len;	// BUFSIZE

	while (left > 0)
	{
		received = recv(S, ptr, left, flags);
		if (received == SOCKET_ERROR) return SOCKET_ERROR;
		else if (received == 0) break;
		cout << "[Received] : " << received << endl;
		left -= received;
		ptr += received;
	}
	return (len - left);
}

void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(1);

	// socket
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);			// Ipv4 + TCP
	if (listen_sock == INVALID_SOCKET) { cout << "listen_sock is invalid" << endl; }

	// bind (소켓 + 주소구조체)
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));		// same as memset(&serveraddr,0,sizeof(serveraddr))
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVERPORT);
	if (bind(listen_sock, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) { cout << "bind() Error" << endl; }

	// listen
	if (listen(listen_sock, SOMAXCONN) == SOCKET_ERROR) { cout << "listen() Error" << endl; }

	// 통신 변수
	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	char buf[BUFSIZE + 1]{};
	int addrlen{};
	
	int retval{};
	while (true)	// accept loop
	{ 
		addrlen = sizeof(client_addr);
		client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &addrlen);
		if (client_sock == INVALID_SOCKET) { cout << "accept() Error" << endl; break; }
		cout << " [ Accept - IP : " << inet_ntoa(client_addr.sin_addr) << ", Port : " << ntohs(client_addr.sin_port) << " ] " << endl;

		while (true)	// communication loop
		{h
			// recv
			retval = recvn(client_sock, buf, 16, 0);
			cout << "[len] 받은 데이터 : " << retval << endl;
			if (retval == SOCKET_ERROR) { cout << "recvn()" << endl; break; }
			else if (retval == 0) break; // client Quit

			// Data Process Logic
			PCharInfo CharInfo;
			PacketDecode(buf, CharInfo);
			switch (CharInfo.m_iCharSelect)
			{
				case Rine:  cout << "[" << CharInfo.m_iPlayerNum << "] " << "Rine" << endl; break;
				case Diva:  cout << "[" << CharInfo.m_iPlayerNum << "] " << "Diva" << endl; break;
				case Genji: cout << "[" << CharInfo.m_iPlayerNum << "] " << "Genji" << endl; break;
				case Hanzo: cout << "[" << CharInfo.m_iPlayerNum << "] " << "Hanzo" << endl; break;
				case Para:  cout << "[" << CharInfo.m_iPlayerNum << "] " << "Para" << endl; break;
				case Lucio: cout << "[" << CharInfo.m_iPlayerNum << "] " << "Lucio" << endl; break;
				case Mersi: cout << "[" << CharInfo.m_iPlayerNum << "] " << "Mersi" << endl; break;
			}
		}

		closesocket(client_sock);
		cout << " [ Quit - IP : " << inet_ntoa(client_addr.sin_addr) << ", Port : " << ntohs(client_addr.sin_port) << " ] " << endl;
	}
	closesocket(listen_sock);

	WSACleanup();
}