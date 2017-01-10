#pragma once
#include <windows.h>
#define BUFSIZE 512

class CCircularBuf
{
private:
	PCHAR			m_pCircularBuf;			// 데이터 저장버퍼
	// Pos
	PCHAR			m_pDataStart;			// 데이터 시작 위치
	PCHAR			m_pDataEnd;				// 데이터 끝 위치
	PCHAR			m_pEndbuf;				// 버퍼 끝 위치
	// Size
	int				m_iBufSize;				// 버퍼사이즈
	int				m_iDataSize;			// 버퍼에 존재하는 데이터 사이즈

private:
	int				CircleMem(PVOID pBuf, int iPos, int iSize);
	int				GetDatasize();			// 버퍼에 존재하는 크기리턴
	int				GetBufsize();			// 버퍼에 저장 가능한 크기 리턴

public:
	int				WriteBuf(PVOID pBuf, int iSize);		// 버퍼에 데이터 저장함수
	int				ReadBuf(PVOID pBuf, int iSize);			// 버퍼에 데이터 로드 함수(size)
	int				ReadBuf(PVOID pBuf);					// 버퍼 데이터 로드함수

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

