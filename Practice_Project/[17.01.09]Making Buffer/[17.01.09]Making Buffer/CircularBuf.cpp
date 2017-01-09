#include "CircularBuf.h"


CCircularBuf::CCircularBuf()
{
	// Size Init
	m_iBufSize = BUFSIZE * 2;
	m_iDataSize = 0;
	// Buf Init
	m_pCircularBuf = new char[BUFSIZE * 2 + 1];
	// Pos Init
	m_pDataStart = m_pDataEnd = m_pCircularBuf;
	m_pEndbuf = &m_pCircularBuf[BUFSIZE * 2];

}

CCircularBuf::~CCircularBuf()
{
	delete[] m_pCircularBuf;
}

int CCircularBuf::WriteBuf(PVOID pBuf, int iSize)
{
	int		iPos = m_pDataEnd - m_pCircularBuf;
	int		iSpare = m_pEndbuf - m_pDataStart;

	if (iSpare >= iSize)		// 선형구조
	{
		memcpy(m_pCircularBuf + iPos, pBuf, iSize);
		m_pDataEnd += iSize;
	}
	else						// 넘을때
	{
		
	}

	return 0;
}

int CCircularBuf::CircleMem(PVOID pBuf, int nPos, int nSize)
{
	return 0;
}

int CCircularBuf::GetDatasize()
{
	return 0;
}

int CCircularBuf::GetBufsize()
{
	return 0;
}


int CCircularBuf::ReadBuf(PVOID pBuf, int nSize)
{
	return 0;
}

int CCircularBuf::ReadBuf(PVOID pBuf)
{
	return 0;
}

