#include <iostream>
#include <Windows.h>
using namespace std;

enum EPacketType
{
	ECharInfo,
	ECharMove,
};

struct CharInf
{
	int		iId;
	int		iHp;
	int		iMp;
};

int PacketEncode(void* pOut, int iPacketType, void* pMsg, int iLenMsg,
	int iLenPacketHeader = 2, int iLenpacketType = 4)
{
	unsigned char* pDst = (unsigned char*)pOut;				// 
	WORD sSize = iLenPacketHeader + iLenpacketType + iLenMsg + 55000; // 2 + 4 + 12 
	memcpy(pDst, &sSize, iLenPacketHeader);
	memcpy(pDst + iLenPacketHeader, &iPacketType, iLenpacketType);	// 패킷 Type 넣기 ( 4byte )
	memcpy(pDst + iLenPacketHeader + iLenpacketType, pMsg, iLenMsg);

	return sSize;
}

int PacketDecode(int* iPacketType, void* pMsg, int* iLenMsg, void* pIn,
	int iLenPacketHeader = 2, int iLenPacketType = 4)
{
	unsigned char* pSrc = (unsigned char*)pIn;
	WORD sSize = 
}

void main()
{
	CharInf TestChar;
	TestChar.iId = 1;
	TestChar.iHp = 50;
	TestChar.iMp = 50;

	// Encode
	int		Send_Type = ECharInfo;
	char	send_buf[256]{};
	cout << PacketEncode(send_buf, Send_Type, &TestChar, sizeof(TestChar)) << endl;
	// Send
	char	recv_buf[256]{};
	memcpy(recv_buf, send_buf, 256);
	// Recv
	int		Recv_Type{};
	int		Recv_Len{};
	cout << PacketDecode(&Recv_Type, recv_buf, &Recv_Len, recv_buf) << endl;



}