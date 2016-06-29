#include "StdAfx.h"
#include "ThreadPool.h"
#include "../Utility/Log.h"


CThreadPool::CThreadPool():
	m_hRequestQueue(NULL),
	m_pvWorkerParam(NULL),
	m_hEvent(NULL),
	m_iWorks(0),
	m_poolSize(0)
{

}
CThreadPool::~CThreadPool()
{
	Shutdown();
}

DWORD CThreadPool::Initialize(void *pvWorkerParam,int nNumThreads)
{
	if (m_hRequestQueue)   // Already initialized
		return ERROR_ALREADY_INITIALIZED;

	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!m_hEvent)
	{
		return GetLastError();
	}

	m_hRequestQueue = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, nNumThreads);
	if (m_hRequestQueue == NULL)
	{
		CloseHandle(m_hEvent);
		return GetLastError();
	}
	m_pvWorkerParam = pvWorkerParam;

	DWORD hr = CreatePool(nNumThreads);
	if (hr != S_OK)
	{
		CloseHandle(m_hRequestQueue);
		m_hRequestQueue = NULL;
		CloseHandle(m_hEvent);
		return hr;
	}

	return S_OK;
}

void CThreadPool::Shutdown( DWORD dwMaxWait)
{
	if (!m_hRequestQueue)   // Not initialized
		return;


	DWORD hr = DestoryPool(dwMaxWait);

	CloseHandle(m_hRequestQueue);
	m_hRequestQueue = NULL;

	CloseHandle(m_hEvent);

}

HANDLE CThreadPool::GetQueueHandle()
{
	return m_hRequestQueue;
}

BOOL CThreadPool::QueueRequest(  Worker * pWorker )
{
	if( !m_hRequestQueue )
	{
		return FALSE;
	}
	//LOG.LogDebug("QueueRequest 111");
	if (!PostQueuedCompletionStatus(m_hRequestQueue, 0, (ULONG_PTR) pWorker, NULL))
		return FALSE;
	InterlockedExchangeAdd(&m_iWorks,1);
	//LOG.LogDebug("QueueRequest 222");
	return TRUE;
}


void CThreadPool::run(CThread *thread, void *arg)
{
	DWORD dwBytesTransfered;
	ULONG_PTR dwCompletionKey;
	OVERLAPPED* pOverlapped;
	SetEvent( m_hEvent );
	//LOG.LogDebug("ThreadPool 000");
	while( GetQueuedCompletionStatus(m_hRequestQueue, &dwBytesTransfered, &dwCompletionKey, &pOverlapped, INFINITE) )
	{
		//LOG.LogDebug("ThreadPool  1111");

		if (pOverlapped == THREAD_POOL_SHUTDOWN) // Shut down
		{
			//LOG.LogDebug("ThreadPool  2222");
			SetEvent( m_hEvent );
			break;
		}
		else										// Do work
		{
			try
			{
				//LOG.LogDebug("ThreadPool  3333");

				Worker *pWorker = (Worker*) dwCompletionKey;

				if ( pWorker && pWorker->Initialize(m_pvWorkerParam) )
				{
					pWorker->Execute(m_pvWorkerParam, pOverlapped);
					pWorker->Terminate( m_pvWorkerParam );
					InterlockedExchangeAdd(&m_iWorks,-1);
					dwCompletionKey = 0;
				}
			}
			catch( ... )
			{
				LOG.LogDebug("ThreadPool  Bad Work");
			}

		}
	}

	delete thread;
}

DWORD CThreadPool::DestoryPool(DWORD dwMaxWait)
{
	for (int i = 0; i < m_poolSize; i++)
	{
		ResetEvent(m_hEvent);
		PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, THREAD_POOL_SHUTDOWN);
		DWORD dwRet = WaitForSingleObject(m_hEvent, dwMaxWait);
	}
	return S_OK;
}
DWORD CThreadPool::CreatePool(int nNumThreads)
{
	if (!m_hRequestQueue)   // Not initialized
		return E_FAIL;

	if (nNumThreads == 0)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		nNumThreads = 2 * si.dwNumberOfProcessors + 2;
	}

	for (int i = 0; i < nNumThreads; i++)
	{
		ResetEvent(m_hEvent);
		CThread *pThread = new CThread;
		pThread->start( this,NULL);
		DWORD dwRet = WaitForSingleObject(m_hEvent, 5000);
	}

	return S_OK;
}

