#include "StdAfx.h"
#include "NTService.h"
#include <assert.h>

#define FS_CTRL_INTERVAL	5000

CALLBACK_NTSERVICE CNTService::m_pStartProc=NULL;
CALLBACK_NTSERVICE CNTService::m_pEndProc=NULL;
HANDLE CNTService::m_hEvtTerminateSys=NULL;
SERVICE_STATUS_HANDLE  CNTService::m_hSvcStatus=NULL;
bool CNTService::m_bRunAsConsoleMode=false;
SERVICE_STATUS  CNTService::ss;
TCHAR CNTService::m_szServiceName[64]={0};



CNTService::CNTService(void)
{

}


CNTService::~CNTService(void)
{
}

//
// Main entry point for this service module. This is where
// all of the action starts. If the SCM Can't be contacted,
// the process is run as a console-mode applciation.
//
bool CNTService::StartNTService(	LPCTSTR szServiceName, 
								CALLBACK_NTSERVICE _startProc, 
								CALLBACK_NTSERVICE _endProc, 
								int _argc,TCHAR*_argv[] )
{
	assert ( szServiceName );
	//ASSERT ( _startProc && _endProc );
	m_pStartProc = _startProc;
	m_pEndProc   = _endProc;

	CreateTerminateEvent ();

	_tcsncpy( m_szServiceName,szServiceName,64 );

	/*
	* Service table entry that specifies Displays for the
	* services in this module.
	*/
	SERVICE_TABLE_ENTRY ste[] = { (TCHAR*)szServiceName, (LPSERVICE_MAIN_FUNCTION)SrvMain, NULL, NULL	};

	if ( ::StartServiceCtrlDispatcher ( ste ) == FALSE )
	{
		DWORD dwErr = ::GetLastError();
		if ( dwErr == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT )
		{
			m_bRunAsConsoleMode = TRUE;
			SrvMain ( _argc, _argv );
		}
		else
		{
			/* Some other error has occurred. */
			return false;
		}
	}

	return true;
}
//
// 服务主函数入口
//
void CNTService::SrvMain(DWORD argc, TCHAR *argv[])
{
	if ( IsRunAsService() )
	{
		m_hSvcStatus = ::RegisterServiceCtrlHandler ( m_szServiceName, CtrlHandler );
		if(m_hSvcStatus == 0)
		{
			return;
		}
	}
	UpdateServiceStatus ( m_hSvcStatus, SERVICE_START_PENDING, NO_ERROR, FS_CTRL_INTERVAL );

	__try
	{
		if( !m_pStartProc || !m_pStartProc() )
		{
			__leave;
		}
		UpdateServiceStatus ( m_hSvcStatus, SERVICE_RUNNING, NO_ERROR, FS_CTRL_INTERVAL );

		// 等待系统结束
		WaitForSingleObject ( m_hEvtTerminateSys, INFINITE);
		::SetEvent ( m_hEvtTerminateSys );
	}
	__finally
	{
		/* Release resources */
		UpdateServiceStatus ( m_hSvcStatus, SERVICE_STOP_PENDING, ::GetLastError(), FS_CTRL_INTERVAL );

		if ( m_pEndProc ) m_pEndProc();

		if ( m_hEvtTerminateSys && m_hEvtTerminateSys!=INVALID_HANDLE_VALUE )
		{
			::CloseHandle ( m_hEvtTerminateSys );
			m_hEvtTerminateSys = NULL;
		}

		/* Elvis has left the building */
		UpdateServiceStatus ( m_hSvcStatus, SERVICE_STOPPED, NO_ERROR, FS_CTRL_INTERVAL );
	}
}

bool CNTService::InstallNTService( LPCTSTR szServiceName)
{
	if (IsServiceInstalled(szServiceName))
		return TRUE;

	//打开服务控制管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		return FALSE;
	}

	// Get the executable file path
	TCHAR szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);

	//创建服务
	SC_HANDLE hService = ::CreateService(
		hSCM, szServiceName, szServiceName,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, _T(""), NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		return FALSE;
	}

	SERVICE_STATUS tmp_ss;

	if(StartService(hService, NULL, NULL))
	{
		Sleep(30);
		while(QueryServiceStatus(hService, &tmp_ss))
		{
			if(tmp_ss.dwCurrentState == SERVICE_START_PENDING)
				Sleep(30);
			else 
				break;
		}

		if(tmp_ss.dwCurrentState == SERVICE_RUNNING)
		{

		}
		else
		{

		}
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return TRUE;
}

bool CNTService::UninstallNTService( LPCTSTR szServiceName )
{
	if (!IsServiceInstalled(szServiceName) )
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		return FALSE;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	//删除服务
	BOOL bDelete = ::DeleteService(hService);

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	if (bDelete)
		return TRUE;

	return FALSE;
}

//
// 主动停止服务
//
void CNTService::StopNTService()
{
	UpdateServiceStatus ( m_hSvcStatus, SERVICE_STOP_PENDING, NO_ERROR, FS_CTRL_INTERVAL );
	::SetEvent ( m_hEvtTerminateSys );
}

bool CNTService::IsRunAsService()
{
	return (!m_bRunAsConsoleMode);
}

bool CNTService::IsServiceInstalled( LPCTSTR szServiceName)
{
	BOOL bResult = FALSE;

	//打开服务控制管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		//打开服务
		SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}






/********************************************************************************
* Function Type	:	Private
* Parameter		:	dwControl		-	控制代码
* Return Value	:	None
* Description	:	Service control handler function.
*********************************************************************************/
void WINAPI CNTService::CtrlHandler ( DWORD dwControl )
{
	switch ( dwControl )
	{
		// SCM request stop server
	case SERVICE_CONTROL_SHUTDOWN:
	case SERVICE_CONTROL_STOP:
		{
			StopNTService ();
			break;
		}
		// Returns the current state of the service to the SCM.
	case SERVICE_CONTROL_INTERROGATE:
		{
			if ( m_hSvcStatus != 0 )
				::SetServiceStatus ( m_hSvcStatus, &ss);
			break;
		}
	}
}

/********************************************************************************
* Function Type	:	Private
* Parameter		:	hStatus		-	服务状态句柄
*					dwStatus	-	状态
*					dwErrorCode	-	错误代码
*					
* Return Value	:	None
* Description	:	Sends the current service status to the SCM. Also updates
*					the global service status structure.
*********************************************************************************/
bool CNTService::UpdateServiceStatus (
		SERVICE_STATUS_HANDLE hStatus,
		DWORD dwStatus,
		DWORD dwErrorCode,
		DWORD dwWaitHint )
{
	if ( !IsRunAsService() ) return TRUE;
	if ( hStatus == 0 ) return FALSE;

	static DWORD dwCheckpoint = 1;
	DWORD  dwControls = SERVICE_ACCEPT_STOP;

	ZeroMemory ( &ss, sizeof(ss) );
	ss.dwServiceType = SERVICE_WIN32;
	ss.dwCurrentState = dwStatus;
	ss.dwWaitHint = dwWaitHint;

	if(dwErrorCode)
	{
		ss.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
		ss.dwServiceSpecificExitCode = dwErrorCode;
	}
	// special cases that depend on the new state
	switch ( dwStatus )
	{
	case SERVICE_START_PENDING:
		dwControls = 0;
		break;
	case SERVICE_RUNNING:
	case SERVICE_STOPPED:
		dwCheckpoint = 0;
		break;
	}
	ss.dwCheckPoint = dwCheckpoint ++;
	ss.dwControlsAccepted = dwControls;
	return ::SetServiceStatus ( hStatus, &ss );
}


//////////////////////////////////////////////////////////////////////////

//
// 创建一个系统终止的事件，当系统要终止时该事件将被置有信号状态
//
bool CNTService::CreateTerminateEvent()
{
	if ( m_hEvtTerminateSys && m_hEvtTerminateSys != INVALID_HANDLE_VALUE )
		return TRUE;

	m_hEvtTerminateSys = ::CreateEvent ( NULL, TRUE, FALSE, NULL );
	if ( m_hEvtTerminateSys == NULL )
	{
		return FALSE;
	}

	return TRUE;
}

//
// 服务退出前调用这个函数
//
void CNTService::UnInitService()
{
	if ( m_hEvtTerminateSys && m_hEvtTerminateSys!=INVALID_HANDLE_VALUE )
	{
		::CloseHandle ( m_hEvtTerminateSys );
		m_hEvtTerminateSys = NULL;
	}
}
