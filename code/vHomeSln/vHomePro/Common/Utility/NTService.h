#pragma once
#include <winsvc.h>


// NT 服务回调函数，启动服务和终止服务时要调用的函数
typedef bool (*CALLBACK_NTSERVICE)(void);			

class  CNTService
{
public:
	CNTService(void);
	~CNTService(void);

	bool	StartNTService(	LPCTSTR szServiceName, 
							CALLBACK_NTSERVICE _startProc, 
							CALLBACK_NTSERVICE _endProc, 
							int _argc,TCHAR*_argv[] );
	bool	InstallNTService( LPCTSTR szServiceName );
	bool	UninstallNTService( LPCTSTR szServiceName );
	bool	IsServiceInstalled(LPCTSTR szServiceName);


	static bool	UpdateServiceStatus(
							SERVICE_STATUS_HANDLE hStatus,
							DWORD dwStatus,
							DWORD dwErrorCode,
							DWORD dwWaitHint );
		
	static void	StopNTService();
	static bool	IsRunAsService();
	


	static void WINAPI CtrlHandler ( DWORD dwControl );
	static void WINAPI SrvMain(DWORD argc, TCHAR *argv[]);

private:
	bool CreateTerminateEvent();
	void UnInitService();

	// 回调函数
	static CALLBACK_NTSERVICE m_pStartProc;
	static CALLBACK_NTSERVICE m_pEndProc;
	static HANDLE m_hEvtTerminateSys;
	static SERVICE_STATUS_HANDLE  m_hSvcStatus;
	static bool m_bRunAsConsoleMode;
	static SERVICE_STATUS  ss;
	static TCHAR m_szServiceName[64];


};
