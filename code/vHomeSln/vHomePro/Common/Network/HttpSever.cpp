#include "StdAfx.h"
#include "HttpSever.h"
#include "..\Utility\Log.h"
#include "Sock.h"

#define MAX_REQUEST_COUNT 10000  //最大请求数

CHttpSever::CHttpSever(void)
{
	_sock = 0;
}


CHttpSever::~CHttpSever(void)
{
}

int CHttpSever::Start( int port,int nNumThreads)
{
	Stop();
	if ( S_OK != Initialize( this,nNumThreads ) )
	{
		return -1;
	}

	_sock = CTcpSock::Create();
	if ( _sock < 0 )
	{
		LOG.LogDebug("## create %d err=%d",_sock,GetLastError() );
		return -1;
	}

	if ( CTcpSock::Listen( _sock,port ) < 0 )
	{
		LOG.LogDebug("## Listen %d err=%d",_sock,GetLastError() );
		return -1;
	}
	
	m_recvThread.start( this,NULL );

	return 0;
}
void CHttpSever::Stop( )
{
	if ( _sock > 0 )
	{
		CTcpSock::Close( _sock );
	}
	m_recvThread.wait_exit();
	Shutdown();
}
void CHttpSever::run(CThread *thread, void *arg)
{
	if ( thread == &m_recvThread )
	{
		while(1)
		{
			CNetAddr addr;
			int s = CTcpSock::Accept( _sock,addr );

			LOG.LogDebug("accept sock=%d,addr=%s",s,addr.toAddrString().c_str() );
			if (  s >= 0)
			{
				int iWorks = GetWorksCount();
				if ( iWorks > MAX_REQUEST_COUNT )
				{
					//request too much.
					CTcpSock::Close( s );
					Sleep(1);
					continue;
				}

				Worker * pClient = CreateWorker(s,addr);
				if ( pClient )
				{
					QueueRequest( pClient );
				}
				else
				{
					LOG.LogError( "create worker fail" );
				}
				
			}
			else
			{
				int err = GetLastError();
				LOG.LogDebug("accept sock err=%d",err);
			}
		}
	}
	else
	{
		__super::run( thread,arg);
	}

}
