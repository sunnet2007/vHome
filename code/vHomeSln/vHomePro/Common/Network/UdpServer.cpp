#include "StdAfx.h"
#include "UdpServer.h"
#include "UDPHelper.h"
#include "../Utility/Log.h"

#define MAX_REQUEST_COUNT 10000  //最大请求数
#define UDP_MSGLEN        10240

CUdpServer::CUdpServer()
{
	_sock = 0;
}
CUdpServer::~CUdpServer()
{

}

BOOL CUdpServer::Start( int port,int nNumThreads)
{
	Stop();
	if ( S_OK != Initialize( this,nNumThreads ) )
	{
		return FALSE;
	}

	_sock = CUDPHelper::CreateSock();
	if ( _sock > 0 )
	{
		if ( CUDPHelper::Bind(_sock,port) < 0)
		{
			return FALSE;
		}
		m_recvThread.start( this,NULL);

		return TRUE;
	}

	return FALSE;

}
BOOL  CUdpServer::Stop()
{
	if ( _sock > 0 )
	{
		CUDPHelper::CloseSock( _sock );
	}
	m_recvThread.wait_exit();
	Shutdown();
	return TRUE;
}
void CUdpServer::HandleMessage( UDPMSG &udpMsg )
{
}

void CUdpServer::run(CThread *thread, void *arg)
{
	if ( thread == &m_recvThread )
	{
		while(1)
		{
			int iWorks = GetWorksCount();
			if ( iWorks > MAX_REQUEST_COUNT )
			{
				//request too much.
				Sleep(1);
				continue;
			}
			//LOG.LogDebug("CUdpServer run 111");
			CServerWork * pWork = new CServerWork;
			UDPMSG &udpMsg = pWork->udpMsg;
			if ( pWork && CUDPHelper::RecvFrom(_sock,udpMsg._msg.GetBuffer(UDP_MSGLEN),UDP_MSGLEN,udpMsg._addr) > 0 )
			{
				//LOG.LogDebug("CUdpServer run 222");
				QueueRequest( pWork );
				//LOG.LogDebug("CUdpServer run 333");
			}
		}
	}
	else
	{
		__super::run(thread,arg);
	}
}

