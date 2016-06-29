#pragma once

#if (defined(_WIN32) || defined(_WIN64))
#include <Windows.h>
typedef  HANDLE THANDLE;
#else
#include <linux/unistd.h>
typedef  pthread_t THANDLE;
#endif

/*
*线程的“拥有”类继承此类
*/
class CThread;
class Runnable 
{

public:
    virtual ~Runnable()
	{
    }
    /**
     * 运行入口函数
     */
    virtual void run(CThread *thread, void *arg) = 0;
};

class CThread 
{

public:
    CThread() 
	{
		hThread = NULL;
		args = NULL;
		id = 0;
    }
	CThread( int threadid )
	{
		hThread = NULL;
		args = NULL;
		id = threadid;
	}

     //开始运行
    bool start(Runnable *r, void *a) 
	{
		if ( hThread != NULL )
		{
			return true;
		}
		
        runnable = r;
        args = a;
#if (defined(_WIN32) || defined(_WIN64))
		hThread = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)CThread::thread_proc, this, 0, NULL);
#else
		return 0 == pthread_create(&hThread, NULL, CThread::thread_proc, this);
#endif 
		return true;
    }

   
    // 等待线程退出
    void wait_exit() 
	{
#if (defined(_WIN32) || defined(_WIN64))
		if ( hThread )
		{
			WaitForSingleObject(hThread, INFINITE);  
			CloseHandle( hThread );
			hThread = NULL;
		}
#else
		if (hThread) 
		{
			pthread_join(hThread, NULL);
			hThread = NULL;
		}
#endif 
    }

	Runnable*getRunnable()
	{
		return runnable;
	}
    void *getArgs() 
	{
        return args;
    }
    int getThreadId()
	{
		return id;
	}


    //线程的回调函数
    static void *thread_proc(void *arg)
	{
        CThread *thread = (CThread*) arg;

        if (thread->getRunnable()) {
            thread->getRunnable()->run(thread, thread->getArgs());
        }

        return (void*)NULL;
    }
    
private:
	THANDLE	 hThread;
    Runnable *runnable;
	int  id;
    void *args;
};

