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
	int		iPos = m_pDataEnd - m_pCircularBuf;	// 시작부터 데이터까지
	int		iSpare = m_pEndbuf - m_pDataStart;

	if (iSpare >= iSize)		// 여유공간이 있을 때
	{
		memcpy(m_pCircularBuf + iPos, pBuf, iSize);
		m_pDataEnd += iSize;
		// what if?
		//memcpy(m_pDataEnd, pBuf, iSize);
		//m_pDataEnd += iSize;
	}
	else						// 여유공간이 없을 때
	{
		memcpy(m_pCircularBuf + iPos, pBuf, iSize);
		memcpy(m_pCircularBuf, (PCHAR)pBuf + iSpare, iSize - iSpare);
		m_pDataEnd = m_pCircularBuf + iSize - iSpare;
	}
	m_iDataSize += iSize;
	return m_iDataSize;

}

int CCircularBuf::ReadBuf(PVOID pBuf)
{
	int		iSize;
	int		iPos = m_pDataStart - m_pCircularBuf;

	memcpy((void*)&iSize, m_pCircularBuf + iPos, 4);

	if (iSize > m_iDataSize || m_iDataSize == 0)	// Data 다 들어왔는지??
		return -1;
	if (iSize < 0 || iSize > BUFSIZE)
		return -1;

	return CircleMem(pBuf, iPos, iSize);
}

int CCircularBuf::ReadBuf(PVOID pBuf, int iSize)
{
	if (iSize > m_iDataSize || m_iDataSize == 0)
		return -1;
	int iPos = m_pDataStart - m_pCircularBuf;

	return CircleMem(pBuf, iPos, iSize);
}


int CCircularBuf::CircleMem(PVOID pBuf, int iPos, int iSize)
{
	int		iSpare = m_pEndbuf - m_pDataStart;

	if (iSpare >= iSize)
	{
		memcpy(pBuf, m_pCircularBuf + iPos, iSize);
		m_pDataStart += iSize;
	}
	else
	{
		memcpy(pBuf, m_pCircularBuf + iPos, iSize);
		memcpy((PCHAR)pBuf + iSpare,m_pCircularBuf, iSize-iSpare);
		m_pDataStart = m_pCircularBuf + iSpare - iSpare;
	}
	m_iDataSize -= iSize;
	return m_iDataSize;
}

int CCircularBuf::GetDatasize()
{
	return 0;
}

int CCircularBuf::GetBufsize()
{
	return 0;
}



