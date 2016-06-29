#include "LoginWnd.h"
#include "StringUtil.h"
#include "AppConfig.h"

string CAppConfig::NOS_FENXI = "";
string CAppConfig::NOS_TEACHERS = ""; 
string CAppConfig::NOS_TEACHER_EXCHANGEINFO = "";
string CAppConfig::NOS_SENDYZM = "";
string CAppConfig::NOS_REG = "";
string CAppConfig::NOS_LOGIN = "";
string CAppConfig::NOS_MODIFYPASSWORD = "";
string CAppConfig::ADDATTENTION = "";
string CAppConfig::USER_TEACHER_INFO = "";
string CAppConfig::DELATTENTION = "";
string CAppConfig::NOS_TOPTEACHER = "";
string CAppConfig::NOS_C_STARTEACHER = "";


int Init()
{
	WORD wV;
	WSADATA ws;
	int er;
	wV = MAKEWORD(1, 1);
	er = WSAStartup(wV, &ws);
	if(er != 0)
	{
		return -1;
	}

	if(LOBYTE(ws.wVersion) != 1 || HIBYTE(ws.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}

	// ÅäÖÃÐÅÏ¢
	Server server;
	CAppConfig::GetServer(server);
	CAppConfig::NOS_FENXI					= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_fenxi";
	CAppConfig::NOS_TEACHERS				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_teachers";
	CAppConfig::NOS_TEACHER_EXCHANGEINFO 	= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_teacher_exchangeinfo";
	CAppConfig::NOS_SENDYZM					= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_sendyzm";
	CAppConfig::NOS_REG						= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_reg";
	CAppConfig::NOS_LOGIN 					= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_login";
	CAppConfig::NOS_MODIFYPASSWORD			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_Modifypassword";
	CAppConfig::ADDATTENTION				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/addAttention";
	CAppConfig::USER_TEACHER_INFO			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/user_teacher_info";
	CAppConfig::DELATTENTION				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/delAttention";
	CAppConfig::NOS_TOPTEACHER				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_topteacher"; 
	CAppConfig::NOS_C_STARTEACHER				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_starteacher"; 
	return 0;
}

int APIENTRY WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	if (-1 == Init())
	{
		return 0;
	}
	CPaintManagerUI::SetInstance(hInstance);

	TCHAR strCurPath[256];
	memset(strCurPath, 0, 256);
	GetCurrentDirectory(256, strCurPath);

	char strPath[300];
	memset(strPath, 0, 300);

	TcharToChar(strCurPath, strPath);

	strcat_s(strPath, "\\UIFile");

	TCHAR tstrPath[300];
	CharToTchar(strPath, tstrPath);

	CPaintManagerUI::SetResourcePath( tstrPath);
	CDuiString p = CPaintManagerUI::GetResourcePath();

	HRESULT hr = ::CoInitialize(NULL);
	if (FAILED(hr))
		return 0;

	CLoginWnd* pLoginWnd = new CLoginWnd;
	if (pLoginWnd == NULL)
		return 0;

	pLoginWnd->Create(NULL, _T("Login Window"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

	pLoginWnd->CenterWindow();
	//pLoginWnd->ShowWindow();
	pLoginWnd->ShowModal();

	CPaintManagerUI::MessageLoop();
	delete pLoginWnd;
	pLoginWnd = NULL;
	::CoUninitialize();
	return 0;
}