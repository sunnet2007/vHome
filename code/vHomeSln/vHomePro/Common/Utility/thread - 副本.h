#pragma once

#if (defined(_WIN32) || defined(_WIN64))
#include <Windows.h>
typedef  HANDLE THANDLE;
#else
#include <linux/unistd.h>
typedef  pthread_t THANDLE;
#endif

/*
*�̵߳ġ�ӵ�С���̳д���
*/
class CThread;
class Runnable 
{

public:
    virtual ~Runnable()
	{
    }
    /**
     * ������ں���
     */
    virtual void run(CThread *thread, void *arg) = 0;
};

class CThread 
{
public:
	enum
	{
		thread_null = 0,
		thread_start,
		thread_stop,
		thread_release
	};

    CThread() 
	{
		status = thread_null;
		hThread = NULL;
		args = NULL;
		id = 0;
    }
	CThread( int threadid )
	{
		status = thread_null;
		hThread = NULL;
		args = NULL;
		id = threadid;
	}

     //��ʼ����
    bool start(Runnable *r, void *a) 
	{
		if ( thread_start == status )
		{
			return true;
		}
		else if ( thread_release == status  )
		{
			return false;
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

    //ֹͣ
	void Stop( )
	{
		if ( status != thread_start )
		{
			return;
		}
		status = thread_stop;
	}

	//�ͷ��ڴ�
	void Release( )
	{
		if ( status == thread_null || status == thread_release )
		{
			return;
		}
		status = thread_release;
	}

    // �ȴ��߳��˳�
    void wait_exit() 
	{
		Stop();
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
	bool isRunning()
	{
		return (status==thread_start)?true:false;
	}


    //�̵߳Ļص�����
    static void *thread_proc(void *arg)
	{
        CThread *thread = (CThread*) arg;
		thread->status = thread_start;

        if (thread->getRunnable()) 
		{
			while( thread->isRunning() )
			{
				thread->getRunnable()->run(thread, thread->getArgs());
			}
        }

		if ( thread_release == thread->status )
		{
			delete thread;
		}
		
        return (void*)NULL;
    }
    
private:
	THANDLE	 hThread;
    Runnable *runnable;
	int  id;
    void *args;
	int  status;
};

