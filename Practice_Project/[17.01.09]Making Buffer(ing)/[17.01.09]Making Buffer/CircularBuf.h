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
	int				CircleMem(PVOID pBuf, int iPos, int iSize);
	int				GetDatasize();			// ���ۿ� �����ϴ� ũ�⸮��
	int				GetBufsize();			// ���ۿ� ���� ������ ũ�� ����

public:
	int				WriteBuf(PVOID pBuf, int iSize);		// ���ۿ� ������ �����Լ�
	int				ReadBuf(PVOID pBuf, int iSize);			// ���ۿ� ������ �ε� �Լ�(size)
	int				ReadBuf(PVOID pBuf);					// ���� ������ �ε��Լ�

public:
	CCircularBuf();
	virtual ~CCircularBuf();
	CCircularBuf& operator=(const CCircularBuf& p)
	{
		delete[] m_pCircularBuf;
		m_pCircularBuf = new char[BUFSIZE * 2 + 1];
		memcpy(m_pCircularBuf, p.m_pCircularBuf,sizeof(p.m_pCircularBuf));
		m_iDataSize = p.m_iDataSize;
		return *this;
	}
};

