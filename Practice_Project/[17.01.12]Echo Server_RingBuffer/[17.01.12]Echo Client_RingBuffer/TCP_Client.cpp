#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <ctime>

using namespace std;

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 1024

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
void main()
{
	srand((unsigned)time(NULL));

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(1);

	// socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) { cout << "socket is invalid" << endl; }

	// connect
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(SERVERIP);
	server_addr.sin_port = htons(SERVERPORT);
	if (connect(sock, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) { cout << "connect() Error" << endl; exit(1); }

	// 통신 변수
	char buf[BUFSIZE]{};
	DWORD dw_send_Time{};

	int retval{};
	int iCount{};
	while (true)
	{
		// send
		if (GetTickCount() - dw_send_Time > rand() % 3000 + 500)
		{
			dw_send_Time = GetTickCount();

			PCharInfo CharInfo;
			// Header - WORD(2Byte) + Enum(4Byte)
			CharInfo.m_Size = sizeof(CharInfo);
			CharInfo.m_PacketType = EPacketType::ECharInfo;
			// Data
			CharInfo.m_iCharSelect = (ECharType)(rand() % 7);
			CharInfo.m_iPlayerNum = ++iCount;
			PacketEncode(buf, CharInfo);
			// 수동 분산화.
			// Send.1
			retval = send(sock, buf, 7, 0);
			if (retval == SOCKET_ERROR) { cout << "send()" << endl; }
			cout << "[ Send ] " << retval << " byte" << endl;

			// Send.2
			retval = send(sock, buf + 7 , 5, 0);
			if (retval == SOCKET_ERROR) { cout << "send()" << endl; }
			cout << "[ Send ] " << retval << " byte" << endl;

			// Send.3
			retval = send(sock, buf + 12, 4, 0);
			if (retval == SOCKET_ERROR) { cout << "send()" << endl; }
			cout << "[ Send ] " << retval << " byte" << endl;
		}
	}
	closesocket(sock);

	WSACleanup();
}