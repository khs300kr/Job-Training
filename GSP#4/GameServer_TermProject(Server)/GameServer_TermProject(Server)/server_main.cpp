#define DEBUG							// DEBUG
#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <winsock.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <unordered_set>				// order�� �ʿ������ ���� ���
#include <mutex>
#include "protocol.h"

enum OPTYPE { OP_SEND, OP_RECV };

struct OverlappedEx
{
	// WSAOVERLAPPED ����ü ���� 2����.
	// 1. �񵿱� ������� ���� ������ �ü���� �����Ѵ�.
	// 2. �ü���� �񵿱� ����� ����� �������α׷��� �˷��� �� ����Ѵ�.
	WSAOVERLAPPED	m_Over;
	// WSABUF (1. ���� 2. ���� �����ּ�) => Scatter/Gather
	WSABUF			m_Wsabuf;
	// IOCP send/recv ����
	unsigned char	m_IOCP_buf[MAX_BUFF_SIZE];
	// Send(?) Recv(?)
	OPTYPE			m_Event_type;
	
};

struct CLIENT
{
	int				m_iX;
	int				m_iY;
	bool			m_bConnect;
	SOCKET			m_client_socket;
	OverlappedEx	m_recv_over;
	// recv�� ���� ����.
	unsigned char	packet_buf[MAX_PACKET_SIZE];
	// ������ ���� ������.
	int prev_packet_data;	// ���� �� ������
	int curr_packet_size;	// ���� ��
	// �þ߱���
	std::unordered_set <int> view_list;
	std::mutex vl_lock;
};

HANDLE g_Hiocp;
SOCKET g_ServerSocket;
CLIENT g_Clients[MAX_USER];		// ����

void error_display(char *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::cout << msg;
	std::wcout << L"����" << lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
	while (true);
}

bool Is_Close(int from, int to)
{
	//return (g_Clients[from].m_iX - g_Clients[to].m_iX) 
	//	* (g_Clients[from].m_iX - g_Clients[to].m_iX)
	//	+ (g_Clients[from].m_iY - g_Clients[to].m_iY)
	//	* (g_Clients[from].m_iY - g_Clients[to].m_iY) <= VIEW_RADIUS * VIEW_RADIUS; 
	return ((abs(g_Clients[from].m_iX - g_Clients[to].m_iX) < 7) &&
		(abs(g_Clients[from].m_iY - g_Clients[to].m_iY) < 7));
}

void Init_Server()
{
	// �����ʱ�ȭ
	std::wcout.imbue(std::locale("korean"));
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	// IOCP ����
	g_Hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);

	// socket()
	g_ServerSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	// bind()
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(MY_SERVER_PORT);
	ServerAddr.sin_addr.s_addr = INADDR_ANY;
	::bind(g_ServerSocket, reinterpret_cast<sockaddr *>(&ServerAddr), sizeof(ServerAddr));

	listen(g_ServerSocket, 5);
	for (int i = 0; i < MAX_USER; ++i)
	{
		g_Clients[i].m_bConnect = false;
	}

}

void Send_Packet(int client, void* packet)
{
	int packet_size = reinterpret_cast<unsigned char *>(packet)[0];
	int pcket_type = reinterpret_cast<unsigned char *>(packet)[1];
	OverlappedEx *over = new OverlappedEx;
	over->m_Event_type = OP_SEND;
	memcpy(over->m_IOCP_buf, packet, packet_size);
	ZeroMemory(&over->m_Over, sizeof(over->m_Over));
	over->m_Wsabuf.buf = reinterpret_cast<CHAR *>(over->m_IOCP_buf);
	over->m_Wsabuf.len = packet_size;

	int ret = WSASend(g_Clients[client].m_client_socket, &over->m_Wsabuf, 1, NULL, 0,
		&over->m_Over, NULL);
	if (ret != 0)
	{
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			error_display("Error in SendPacket:", err_no);
	}
	std::cout << "Send Packet [" << pcket_type << "] To Client : " << client << std::endl;
}

void SendPutPlayerPacket(int client, int object)
{
	sc_packet_put_player packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_PLAYER;
	packet.x = g_Clients[object].m_iX;
	packet.y = g_Clients[object].m_iY;

	Send_Packet(client, &packet);
}

void SendPositionPacket(int client, int object)
{
	sc_packet_pos packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_POS;
	packet.x = g_Clients[object].m_iX;
	packet.y = g_Clients[object].m_iY;

	Send_Packet(client, &packet);
}

void SendRemovePlayerPacket(int client, int object)
{
	sc_packet_pos packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_REMOVE_PLAYER;

	Send_Packet(client, &packet);
}


void Accept_Thread()
{
	/*
	<Accept Thread>
	- ���� ������ ���� Ŭ���̾�Ʈ�� IOCP�� �ѱ�� ����
	- ���� ������ ���鼭,
	�� Accept() ȣ��
	�� �� Ŭ���̾�Ʈ ���� -> Ŭ���̾�Ʈ ���� ����ü ����
	�� IOCP�� ���� ��� (send/recv�� IOCP�� ���� ����)
	�� WSARecv() ȣ�� (Overlapped I/O recv ���¸� �׻� ����)
	*/
	while (true)
	{
		SOCKADDR_IN ClientAddr;
		ZeroMemory(&ClientAddr, sizeof(SOCKADDR_IN));
		ClientAddr.sin_family = AF_INET;
		ClientAddr.sin_port = htons(MY_SERVER_PORT);
		ClientAddr.sin_addr.s_addr = INADDR_ANY;
		int addr_size = sizeof(ClientAddr);
		SOCKET client_sock = WSAAccept(g_ServerSocket,
			reinterpret_cast<sockaddr *>(&ClientAddr), &addr_size, NULL, NULL);
#ifdef DEBUG
		std::cout << "[TCP����] Ŭ���̾�Ʈ ���� : IP =" << inet_ntoa(ClientAddr.sin_addr) << ", ��Ʈ ��ȣ = " <<
			ntohs(ClientAddr.sin_port) << "\n";
#endif
		// Accet loop ó��.
		int new_id{ -1 };
		for (int i = 0; i < MAX_USER; ++i)
			if (g_Clients[i].m_bConnect == false) { new_id = i; break; }
		if (new_id == -1) { std::cout << "MAX USER : " << MAX_USER << "�� ���� OVERFLOW\n"; closesocket(client_sock); continue; }

		g_Clients[new_id].m_bConnect = true;
		g_Clients[new_id].m_client_socket = client_sock;
		g_Clients[new_id].curr_packet_size = 0;
		g_Clients[new_id].prev_packet_data = 0;
		ZeroMemory(&g_Clients[new_id].m_recv_over, sizeof(g_Clients[new_id].m_recv_over));
		g_Clients[new_id].m_recv_over.m_Event_type = OP_RECV;
		// WSABUF <- IOCP ����
		g_Clients[new_id].m_recv_over.m_Wsabuf.buf =
			reinterpret_cast<CHAR *>(g_Clients[new_id].m_recv_over.m_IOCP_buf);
		g_Clients[new_id].m_recv_over.m_Wsabuf.len = sizeof(g_Clients[new_id].m_recv_over.m_IOCP_buf);
		// �ʱ���ġ
		g_Clients[new_id].m_iX = 4;
		g_Clients[new_id].m_iY = 4;

		// �񵿱� ����� ����
		DWORD recv_flag = 0;
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_sock), g_Hiocp, new_id, 0);
		WSARecv(client_sock, &g_Clients[new_id].m_recv_over.m_Wsabuf, 1,
			NULL, &recv_flag, &g_Clients[new_id].m_recv_over.m_Over, NULL);

		// ��ġ �ϱ�.
		SendPutPlayerPacket(new_id, new_id);

		std::unordered_set<int> local_my_view_list;

		for (int i = 0; i < MAX_USER; ++i)
		{
			if (g_Clients[i].m_bConnect == true)
			{
				if (i != new_id)
				{
					if(Is_Close(i,new_id) == true){
					SendPutPlayerPacket(new_id, i);
					local_my_view_list.insert(i);
					SendPutPlayerPacket(i, new_id);
																		// ������ �ִ´�. (�þ߸���Ʈ)
					g_Clients[new_id].view_list.insert(i);

					// �����׸��� �ƴ϶� �������׵� �־�����Ѵ�.
					g_Clients[i].vl_lock.lock();						//lock
					g_Clients[i].view_list.insert(new_id);			
					g_Clients[i].vl_lock.unlock();						//unlock
					// �̷����ϸ� ó���� ��ó���ִ� ģ���鸸 ���δ�.
					}
				}
			}
			g_Clients[new_id].vl_lock.lock();					// lock
			for (auto p : local_my_view_list) g_Clients[new_id].view_list.insert(p);
			g_Clients[new_id].vl_lock.unlock();					//unlock
		}// for loop
	}
}

void ProcessPacket(int id, unsigned char packet[])
{
	switch (packet[1])
	{
	case CS_UP: if (g_Clients[id].m_iY > 0) g_Clients[id].m_iY--; break;
	case CS_DOWN: if (g_Clients[id].m_iY < BOARD_HEIGHT - 1) g_Clients[id].m_iY++; break;
	case CS_LEFT: if (g_Clients[id].m_iX > 0) g_Clients[id].m_iX--; break;
	case CS_RIGHT: if (g_Clients[id].m_iX < BOARD_WIDTH - 1) g_Clients[id].m_iX++; break;
	default: std::cout << "Unknown Packet Type from Client : " << id << std::endl;
		while (true);
	}

	SendPositionPacket(id, id);

	// <�þ߱���>
	// �̵��� �����ϱ� �� ����Ʈ�� �ϳ� ����.
	std::unordered_set<int> new_view_list;
	for (int i = 0; i < MAX_USER; ++i)
		if (g_Clients[i].m_bConnect == true)
			if(i != id)	
				if(Is_Close(id,i) == true) new_view_list.insert(i);	// �ڱ� �ڽ��� �����ϰ�.

	std::unordered_set<int> vlc;
	g_Clients[id].vl_lock.lock();
	vlc = g_Clients[id].view_list;	// ���� �ɰ� ī���ؾ��Ѵ�.
	g_Clients[id].vl_lock.unlock();

	for (auto target : new_view_list)
		if (vlc.count(target) == 0) {			// view_list�ȿ� new_view_list�� ��
			// ���� �߰��Ǵ� ��ü
			SendPutPlayerPacket(id, target);
			vlc.insert(target);
			g_Clients[target].vl_lock.lock();		// lock
			if (g_Clients[target].view_list.count(id) != 0)
			{
				g_Clients[target].vl_lock.unlock();
				SendPositionPacket(target, id);
			}
			else
			{
				g_Clients[target].view_list.insert(id);
				g_Clients[target].vl_lock.unlock();		// unlock
				SendPutPlayerPacket(target, id);
			}
		}
		else{
		// �������� �����ϴ� ��ü (�����ְ�, ���濡�Ե� �ְ�) - �����̴°� ���ϱ� ������ �� �ʿ䰡 ����, ������ �˾ƾ��Ѵ�.
		g_Clients[target].vl_lock.lock();		// lock

		if (g_Clients[target].view_list.count(id) != 0)
		{
			g_Clients[target].vl_lock.unlock();		// unlock
			SendPositionPacket(target, id);
		}
		else {
			g_Clients[target].view_list.insert(id);
			g_Clients[target].vl_lock.unlock();		// unlock
			SendPutPlayerPacket(target, id); }
		}

		// �þ߿��� �־��� ��ü
		std::unordered_set<int> faraway_id_list;
		for(auto target: vlc)	// ������ �ִµ� ������ ���� ��
			if (new_view_list.count(target) == 0)
			{
				SendRemovePlayerPacket(id, target);			// ������ ������
				faraway_id_list.insert(target);

				g_Clients[target].vl_lock.lock();		// lock
				if (g_Clients[target].view_list.count(id) != 0)	// ���濡 ���� �ֳ� ���� Ȯ��.
				{
					g_Clients[target].view_list.erase(id);
					g_Clients[target].vl_lock.unlock();		// unlock
					SendRemovePlayerPacket(target, id);			// ���浵 ���� ������
				}
				else g_Clients[target].vl_lock.unlock();		// unlock

			}

		g_Clients[id].vl_lock.lock();	// lock
		for (auto p : vlc)
			g_Clients[id].view_list.insert(p);
		for (auto d : faraway_id_list) 
			g_Clients[id].view_list.erase(d);
		g_Clients[id].vl_lock.unlock();	// unlock
}

void DisconnectClient(int id)
{
	closesocket(g_Clients[id].m_client_socket);
	g_Clients[id].m_bConnect = false;

	std::unordered_set<int> lvl;	// local view list
	g_Clients[id].vl_lock.lock();
	lvl = g_Clients[id].view_list;
	g_Clients[id].vl_lock.unlock();

	// <�þ߱���>
	for (auto target : lvl) {
		g_Clients[target].vl_lock.lock();
		if (g_Clients[target].view_list.count(id) != 0)
		{
			g_Clients[target].view_list.erase(id);
			g_Clients[target].vl_lock.unlock();
			SendRemovePlayerPacket(target, id);
		}
		else g_Clients[target].vl_lock.unlock();
	}

	g_Clients[id].vl_lock.lock();
	g_Clients[id].view_list.clear();
	g_Clients[id].vl_lock.unlock();

}

void Worker_Thread()
{
	while (true)
	{
		// �񵿱� ����� �Ϸ� ��ٸ���. 
		DWORD io_size;					// �񵿱� ����� �۾��� ����Ʈ
		unsigned long long id;			// Ŭ���̾�Ʈ id
		OverlappedEx *over;				// ������ ����ü �ּ�
										// IOCP �� ����¿Ϸ� ��Ŷ�� ���� �� ���� ����Ѵ�. ����� �Ϸ� ��Ŷ�� IOCP�� ������
										// �ü���� ���� ���� �۾��� ������ ������ üũ�Ѵ�. �� ������ �ھ� �������� ������ ��� ���� �����带
										// �ƿ��� ����� �Ϸ� ��Ŷ�� ó���Ѵ�.
										// (<1> IOCP�ڵ� <2> �񵿱� ����� �۾��� ����Ʈ ���� <3> Ŭ���̾�Ʈ_id <4> OVERLAPPED ����ü�� �ּ� ����
		BOOL ret = GetQueuedCompletionStatus(g_Hiocp, &io_size, &id,
			reinterpret_cast<LPWSAOVERLAPPED *>(&over), INFINITE);

		// Error ó��
		std::cout << "GQCS :";
		if (ret == FALSE)
		{
			int err_no = WSAGetLastError();
			if (err_no == 64)
				DisconnectClient(id);
			else error_display("GQCS : ", WSAGetLastError());
		}
		if (io_size == 0)
		{
			DisconnectClient(id);
			continue;
		}

		// Send, Recv ó��
		if (over->m_Event_type == OP_RECV)
		{
			std::cout << "RECV from Client :" << id;
			std::cout << "  IO_SIZE : " << io_size << std::endl;
			unsigned char *buf = g_Clients[id].m_recv_over.m_IOCP_buf;
			unsigned cu_size = g_Clients[id].curr_packet_size;
			unsigned pr_size = g_Clients[id].prev_packet_data;

			while (io_size > 0)
			{
				if (cu_size == 0) cu_size = buf[0];		// ��Ŷ ����� 0 �̸�, �ٷ� ���� ó���ϴ� ��Ŷ�� ó���� ������ �Ǵ� accept �ϰ� ó�� �����͸� �޴´�.
														// io_size�� ��� 1�̴ϱ�, Ŭ���̾�Ʈ���� ����� �ǹ��ִ� �������̴�.

				if (io_size + pr_size >= cu_size)		// ��Ŷ�� �ϼ� �� �� �ִ�.
				{
					unsigned char packet[MAX_PACKET_SIZE];
					memcpy(packet, g_Clients[id].packet_buf, pr_size);
					memcpy(packet + pr_size, buf, cu_size - pr_size);
					ProcessPacket(static_cast<int>(id), packet);
					io_size -= cu_size - pr_size;
					buf += cu_size - pr_size;
					cu_size = 0; pr_size = 0;
				}
				else									// ��Ŷ�� �ϼ� ��ų �� ����.
				{
					memcpy(g_Clients[id].packet_buf + pr_size, buf, io_size);
					pr_size += io_size;
					io_size = 0;
				}
			}
			g_Clients[id].curr_packet_size = cu_size;
			g_Clients[id].prev_packet_data = pr_size;

			DWORD recv_flag = 0;
			WSARecv(g_Clients[id].m_client_socket,
				&g_Clients[id].m_recv_over.m_Wsabuf, 1,
				NULL, &recv_flag, &g_Clients[id].m_recv_over.m_Over, NULL);
		}//OP_RECV

		else if (over->m_Event_type == OP_SEND)
		{
			if (over->m_Wsabuf.len != io_size)
			{
				std::cout << "Send Incomplete Error!\n";
				closesocket(g_Clients[id].m_client_socket);
				g_Clients[id].m_bConnect = false;
			}
			delete over;
		}//OP_SEND

		else
		{
			std::cout << "Unknown GQCS event!\n";
			while (true);
		}//UNKNOWN_GQCS

	}//Worker_Loop
}

void Close_Server()
{
	closesocket(g_ServerSocket);
	CloseHandle(g_Hiocp);
	WSACleanup();
}

int main()
{
	Init_Server();

	// �۾��� ������ ����.
	std::vector<std::thread *> vWorker_threads;
	for (int i = 0; i < 6; ++i)			// �ھ�4 * 1.5 = 6
		vWorker_threads.push_back(new std::thread{ Worker_Thread });

	std::thread accept_thread{ Accept_Thread };
	accept_thread.join();

	for (auto d : vWorker_threads)
	{
		d->join();
		delete d;
	}
	Close_Server();
}
