#ifndef _LLOCK_
#define _LLOCK_

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

//��������

class CLock
{
public:

	inline CLock()
	{
		m_bInit=true;
#ifdef _WIN32
		::InitializeCriticalSection(&m_lock);
#else
		::pthread_mutex_init(&m_lock,NULL);
#endif
	}

	inline ~CLock()
	{
		if(m_bInit)
		{
			m_bInit=false;
#ifdef _WIN32
			::DeleteCriticalSection(&m_lock);
#else
			::pthread_mutex_destroy(&m_lock);
#endif
		}
	}
	//���������뻥������
	inline void lock()
	{
		if(m_bInit)
		{
#ifdef _WIN32
			::EnterCriticalSection(&m_lock);
#else
			::pthread_mutex_lock(&m_lock);
#endif
		}
	}
	//�������뿪��������
	inline void unlock()
	{
		if(m_bInit)
		{
#ifdef _WIN32
			::LeaveCriticalSection(&m_lock);
#else
			::pthread_mutex_unlock(&m_lock);
#endif
		}
	}

private:
	bool volatile  m_bInit;
#ifdef _WIN32
	CRITICAL_SECTION m_lock;
#else
	pthread_mutex_t  m_lock;
#endif
};

//�Զ������ࣨ������ʽʹ�ã�

class CAutoLock
{
public:
	inline CAutoLock(CLock & lock):m_lock(lock)
	{
		m_lock.lock();
	}
	inline ~CAutoLock()
	{
		m_lock.unlock();
	}
private:
	CLock & m_lock;
};

//��д��
class CRWLock
{
public:
	CRWLock()
		:read_cnt(0)
	{
	}

	inline void readLock()
	{
		read_mtx.lock();
		if (++read_cnt == 1)
		{
			write_mtx.lock();
		}
		read_mtx.unlock();
	}

	inline void writeLock()
	{
		write_mtx.lock();
	}

	inline void readUnLock()
	{
		read_mtx.lock();
		if (--read_cnt == 0)
		{
			write_mtx.unlock();
		}
		read_mtx.unlock();
	}

	inline void writeUnLock()
	{
		write_mtx.unlock();
	}

private:
	CLock read_mtx;
	CLock write_mtx;
	int   read_cnt; //������
};

class CAutoRWLock
{
public:
	inline CAutoRWLock(CRWLock & lock,bool isWrite=false):m_lock(lock)
	{
		m_isWrite = isWrite; 
		if ( m_isWrite )
		{
			m_lock.writeLock();
		}
		else
		{
			m_lock.readLock();
		}
	}
	inline ~CAutoRWLock()
	{
		if ( m_isWrite )
		{
			m_lock.writeUnLock();
		}
		else
		{
			m_lock.readUnLock();
		}
	}
private:
	CRWLock & m_lock;
	bool m_isWrite;
};
#endif




