#include <iostream>
#include <map>
#include <Windows.h>
#include <ctime>
#include "TCircularQue.h"

using namespace std;
//
enum PacketID
{
	Player_Move,
};

struct PacketHeader
{
	PacketID	m_type;
	int			m_iSize;
};

struct PlayerMove : PacketHeader
{
	int x;
	int y;
};
//
struct TempMsg
{
	UINT socket;
	PlayerMove Player_msg;
};
// Player Class
class CPlayer
{
private:
	int x, y;
public:
	CPlayer() {};
	CPlayer(int _x, int _y)
	: x(_x),y(_y) {};
	~CPlayer() {};
	void SetX(int _x) { x += _x; }
	void SetY(int _y) { y += _y; }
	int GetX() {return x; }
	int GetY() {return y; }

};


void main()
{
	srand((unsigned)time(NULL));
	TCircularQue<TempMsg>* pCirQue = new TCircularQue<TempMsg>(1024);
	map<int,CPlayer> mPlayer;

	DWORD dw_recv_Time{};
	DWORD dw_send_Time{};

	while (true)
	{	
		// recv
		if (GetTickCount() - dw_recv_Time > 1000)
		{
			dw_recv_Time = GetTickCount();

			TempMsg recv_Msg;
			if (pCirQue->Dequeue(&recv_Msg) == 0)
			{
				mPlayer.insert(pair<int, CPlayer>
					(recv_Msg.socket, CPlayer(recv_Msg.Player_msg.x, recv_Msg.Player_msg.y)));

				mPlayer.at(recv_Msg.socket).SetX(rand() % 5);
				mPlayer.at(recv_Msg.socket).SetY(rand() % 5);
				
				cout << "[" << recv_Msg.socket << "]";
				cout << mPlayer.at(recv_Msg.socket).GetX() << ",";
				cout << mPlayer.at(recv_Msg.socket).GetY();
				cout << "--------" << pCirQue->GetRemain() << endl;				
			}
		}

		// send
		if (GetTickCount() - dw_send_Time > rand()% 3000)
		{
			dw_send_Time = GetTickCount();

			TempMsg send_Msg;
			send_Msg.socket = rand() % 10 + 1;
			send_Msg.Player_msg.m_type = PacketID::Player_Move;
			send_Msg.Player_msg.m_iSize = sizeof(TempMsg);
			send_Msg.Player_msg.x = 1;
			send_Msg.Player_msg.y = 1;

			pCirQue->Enqueue(&send_Msg);
			//cout << "Enqueue" << endl;
		}
	}


	delete pCirQue;
}