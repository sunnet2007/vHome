#pragma once
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include "../Pool/ThreadPool.h"
#include "../Utility/thread.h"
#include "../Utility/Buffer.h"


class UDPMSG
{
public:
	CBuffer _msg;
	sockaddr_in _addr;
};

class  CUdpServer:public CThreadPool
{
public:
	CUdpServer();
	~CUdpServer();

	BOOL   Start( int port ,int nNumThreads=1000);
	BOOL   Stop();
	virtual void HandleMessage( UDPMSG &udpMsg );

	void run(CThread *thread, void *arg);
public:
	int _sock;
	CThread m_recvThread;
};
class CServerWork:public Worker
{
public:
	virtual BOOL Initialize( void * /*pvParam*/)
	{
		return TRUE;
	};
	virtual void Execute(void *pvWorkerParam,OVERLAPPED *pOverlapped)
	{
		CUdpServer *pSever = (CUdpServer*)pvWorkerParam;
		pSever->HandleMessage(  udpMsg );
	};
	virtual void Terminate( void* /*pvParam*/)
	{ 
		udpMsg._msg.ReleaseBuffer();
		delete this;
	};

public:
	UDPMSG udpMsg;
};




