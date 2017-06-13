// MLock.cpp : πÍß@¿…
//

#include "stdafx.h"
#include "Lock.h"


using namespace AutoLib;

MLock::MLock()
{
	m_pLocker=NULL;
	::InitializeCriticalSection(&m_csLock);
}

MLock::~MLock()
{
	::DeleteCriticalSection(&m_csLock);
}
bool MLock::Lock(CObject *pLocker)
{
	bool ret;
	::EnterCriticalSection(&m_csLock);
	if (m_pLocker==pLocker || m_pLocker==NULL)
	{
		m_pLocker=pLocker;
		ret=true;
	}else{
		ret=false;
	}
	::LeaveCriticalSection(&m_csLock);
	return ret;
}
void MLock::UnLock(CObject *pLocker)
{
	::EnterCriticalSection(&m_csLock);
	if (m_pLocker==pLocker)
	{		 
		m_pLocker=NULL;
	}
	::LeaveCriticalSection(&m_csLock);
}
CObject* MLock::GetLocker()
{
	return m_pLocker;
}
