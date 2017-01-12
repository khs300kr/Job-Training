#pragma once

template<class T>
class TCircularQue
{
private:
	T*		m_pBuf;			// 버퍼
	int		m_iFront;		// 시작 위치
	int		m_iRear;		// 끝 위치
	int		m_iRemain;		// 남은공간
	int		m_iSize;		// 버퍼크기

public:
	TCircularQue();
	TCircularQue(int _iSize);
	virtual ~TCircularQue();

public:
	int		Enqueue(T* p);
	int		Dequeue(T* p);

public:
	int		GetRemain(void) { return m_iRemain; }
};

template<class T>
inline TCircularQue<T>::TCircularQue()
{
	m_pBuf		= NULL;
	m_iFront	= 0;
	m_iRear		= 0;
	m_iRemain	= 0;
	m_iSize		= 0;
}

template<class T>
inline TCircularQue<T>::TCircularQue(int _iSize)
{
	m_iFront	= 0;
	m_iRear		= 0;
	m_iRemain	= _iSize;
	m_iSize		= _iSize;
	m_pBuf = new T[_iSize + 1];
	memset(m_pBuf, 0, sizeof(int)*(m_iSize + 1));
}

template<class T>
inline TCircularQue<T>::~TCircularQue()
{
	if (m_pBuf)
	{
		delete[] m_pBuf;
		m_pBuf = NULL;
	}
}

template<class T>
inline int TCircularQue<T>::Enqueue(T * p)
{
	if (m_iRemain == 0)	// Is_Full
	{
		cout << "FULL" << endl;
		return -1;
	}
	memcpy(&m_pBuf[m_iRear], p, sizeof(T));

	--m_iRemain;
	++m_iRear;
	m_iRear %= m_iSize;
	return 0;
}

template<class T>
inline int TCircularQue<T>::Dequeue(T * p)
{
	if (m_iRemain == m_iSize)	// Is_Empty
		return -1;
	memcpy(p, &m_pBuf[m_iFront], sizeof(T));

	++m_iRemain;
	++m_iFront;
	m_iFront %= m_iSize;
	return 0;
}
