#pragma once
#include <windows.h>
#define BUFSIZE 512

class CCircularBuf
{
private:
	PCHAR			m_pCircularBuf;			// ������ �������
	// Pos
	PCHAR			m_pDataStart;			// ������ ���� ��ġ
	PCHAR			m_pDataEnd;				// ������ �� ��ġ
	PCHAR			m_pEndbuf;				// ���� �� ��ġ
	// Size
	int				m_iBufSize;				// ���ۻ�����
	int				m_iDataSize;			// ���ۿ� �����ϴ� ������ ������

private:
	int				CircleMem(PVOID pBuf, int nPos, int nSize);
	int				GetDatasize();			// ���ۿ� �����ϴ� ũ�⸮��
	int				GetBufsize();			// ���ۿ� ���� ������ ũ�� ����

public:
	int				WriteBuf(PVOID pBuf, int nSize);		// ���ۿ� ������ �����Լ�
	int				ReadBuf(PVOID pBuf, int nSize);			// ���ۿ� ������ �ε� �Լ�(size)
	int				ReadBuf(PVOID pBuf);					// ���� ������ �ε��Լ�

public:
	CCircularBuf();
	virtual ~CCircularBuf();
};

