#pragma once
#include "HttpMsg.h"
#include "../Pool/ThreadPool.h"
#include "../Utility/thread.h"
#include "../Utility/Log.h"
#include "Sock.h"


class  CHttpSever:public CThreadPool
{
public:
	CHttpSever(void);
	~CHttpSever(void);

	int  Start( int port,int nNumThreads=1000);
	void Stop( );
	virtual void run(CThread *thread, void *arg);
	virtual Worker* CreateWorker( int sock,const CNetAddr &addr )=0;

protected:
	int _sock;
	CThread m_recvThread;
};


