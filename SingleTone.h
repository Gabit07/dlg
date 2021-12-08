
#ifndef SingleTone_H_
#define SingleTone_H_

#include "common/CommonDefine.h"

template < class T >
class CSingleTone
{
public:
	static T * Create();
	static T * GetInstance();
	static void Destroy();

protected:
	CSingleTone();
	virtual ~CSingleTone();

protected:
	static T * m_pInstance;
	static UINT m_nRef;
};

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
template < class T >
T * CSingleTone<T>::m_pInstance = NULL;

template < class T >
UINT CSingleTone<T>::m_nRef = NULL;

template < class T >
CSingleTone<T>::CSingleTone()
{
}

template < class T >
CSingleTone<T>::~CSingleTone()
{
}

template < class T >
T * CSingleTone<T>::Create()
{
	m_nRef++;
	if ( !m_pInstance ) 
		m_pInstance =  new T;
	return GetInstance();
}

template < class T >
T * CSingleTone<T>::GetInstance()
{
	return m_pInstance;
}

template < class T >
void CSingleTone<T>::Destroy()
{
	if ( !--m_nRef ) 
		SAFE_DELETE(m_pInstance);
}

#endif // SingleTone_H_