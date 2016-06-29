#pragma  once
#include <map>
#include <assert.h>
#include "../Utility/Lock.h"
#include "../Utility/thread.h"


#define THREAD_POOL_SHUTDOWN ((OVERLAPPED*) ((__int64) -1))


//»ŒŒÒ¿‡
class  Worker
{
public:
	virtual BOOL Initialize( void * /*pvParam*/) throw()
	{
		return TRUE;
	}

	virtual void Execute(
			void *pvWorkerParam,
			OVERLAPPED *pOverlapped)
	{

	}

	virtual void Terminate( void* /*pvParam*/) throw()
	{
	}
}; 


class  CThreadPool:public Runnable
{
public:

	CThreadPool();
	virtual ~CThreadPool();


	DWORD	Initialize(void *pvWorkerParam=NULL,int nNumThreads=0);
	void	Shutdown( DWORD dwMaxWait=60000);
	BOOL	QueueRequest(  Worker * pWorker );
	HANDLE	GetQueueHandle();
	int		GetNumThreads();
	int		GetWorksCount()
	{
		LONG iWorks = 0;
		InterlockedExchangeAdd(&iWorks,m_iWorks);
		return iWorks;
	}

protected:
	 virtual void run(CThread *thread, void *arg);
	 DWORD CreatePool(int nNumThreads);
	 DWORD DestoryPool(DWORD dwMaxWait);
private:
	HANDLE m_hEvent;
	HANDLE m_hRequestQueue;
	int  m_poolSize;
	LONG m_iWorks;
	void *m_pvWorkerParam;
}; // class CThreadPool

