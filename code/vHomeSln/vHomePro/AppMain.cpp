#include "LoginWnd.h"
#include "StringUtil.h"
#include "AppConfig.h"
#include "resource.h"

string CAppConfig::NOS_FENXI					= "";
string CAppConfig::NOS_TEACHERS					= ""; 
string CAppConfig::NOS_TEACHER_EXCHANGEINFO		= "";
string CAppConfig::NOS_SENDYZM					= "";
string CAppConfig::NOS_REG						= "";
string CAppConfig::NOS_C_LOGIN					= "";
string CAppConfig::NOS_MODIFYPASSWORD			= "";
string CAppConfig::ADDATTENTION					= "";
string CAppConfig::USER_TEACHER_INFO			= "";
string CAppConfig::DELATTENTION					= "";
string CAppConfig::NOS_TOPTEACHER				= "";
string CAppConfig::NOS_C_STARTEACHER			= "";
string CAppConfig::NOS_SENDZHYZM				= "";
string CAppConfig::NOS_GETPWD					= "";
string CAppConfig::NOS_ACTIVITY					= "";
string CAppConfig::MY_ACTIVITY					= "";
string CAppConfig::NOS_ACTIVITY_INFO			="";
string CAppConfig::NOS_ACTIVITY_CONTENT			="";
string CAppConfig::ENROLL_ACTIVITY				= "";
string CAppConfig::NOS_C_TEACHERS_INFO			= "";
string CAppConfig::NOS_TEACHER_ARTICLEINFO		= "";
string CAppConfig::NOS_TEACHER_ALLARTICLE		= "";
string CAppConfig::NOS_TEACHER_RELEASE			= "";
string CAppConfig::NOS_TEACHER_PRERELEASE		= "";
string CAppConfig::NOS_TEACHER_INDEX			= "";
string CAppConfig::NOS_USERIMAGE				= "";
string CAppConfig::NOS_USERINFO_VIEW			= "";
string CAppConfig::NOS_USERINFO_MODI			= "";
string CAppConfig::NOS_TEACHER_VIEW				= "";
string CAppConfig::NOS_TEACHER_MODI				= "";
string CAppConfig::NOS_TEST						= "";
string CAppConfig::NOS_TEACHER_HEADING			= "";
string CAppConfig::NOS_UPLOADARTICLEIMG			= "";
string CAppConfig::UI_FILE_PATH					= "";
string CAppConfig::NOS_ABOUTUS					= "";

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
	CAppConfig::NOS_C_LOGIN 				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_c_login";
	CAppConfig::NOS_MODIFYPASSWORD			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_Modifypassword";
	CAppConfig::ADDATTENTION				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/addAttention";
	CAppConfig::USER_TEACHER_INFO			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/user_teacher_info";
	CAppConfig::DELATTENTION				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/delAttention";
	CAppConfig::NOS_TOPTEACHER				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_topteacher"; 
	CAppConfig::NOS_C_STARTEACHER			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_starteacher"; 
	CAppConfig::NOS_SENDZHYZM				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_sendzhyzm"; 
	CAppConfig::NOS_GETPWD					= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_getpwd"; 
	CAppConfig::NOS_ACTIVITY				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_activity";
	CAppConfig::MY_ACTIVITY					= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/my_activity";
	CAppConfig::NOS_ACTIVITY_INFO			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_activity_info";
	CAppConfig::NOS_ACTIVITY_CONTENT		= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_activity_content/";
	CAppConfig::ENROLL_ACTIVITY				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/enroll_activity";
	CAppConfig::NOS_C_TEACHERS_INFO			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_c_teachers_info";
	CAppConfig::NOS_TEACHER_ARTICLEINFO		= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_teacher_articleinfo/";
	CAppConfig::NOS_TEACHER_ALLARTICLE		= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_teacher_allarticle";
	CAppConfig::NOS_TEACHER_PRERELEASE		= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_teacher_Prerelease";
	CAppConfig::NOS_TEACHER_RELEASE			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_teacher_release";
	CAppConfig::NOS_TEACHER_INDEX			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_teacher_index";
	CAppConfig::NOS_USERIMAGE				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_userimage";
	CAppConfig::NOS_USERINFO_VIEW			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_userinfo_view";
	CAppConfig::NOS_USERINFO_MODI			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_userinfo_modi";
	CAppConfig::NOS_TEACHER_VIEW			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_teacher_view";
	CAppConfig::NOS_TEACHER_MODI			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_teacher_modi";
	CAppConfig::NOS_TEST					= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_test";
	CAppConfig::NOS_TEACHER_HEADING			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_teacher_headimg";
	CAppConfig::NOS_UPLOADARTICLEIMG		= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_uploadarticleimg";
	CAppConfig::NOS_ABOUTUS					= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_aboutus";

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
	CAppConfig::UI_FILE_PATH = strPath;

	TCHAR tstrPath[300];
	CharToTchar(strPath, tstrPath);

	CPaintManagerUI::SetResourcePath( tstrPath);
	CDuiString p = CPaintManagerUI::GetResourcePath();


	HICON hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	HRESULT hr = ::CoInitialize(NULL);
	if (FAILED(hr))
		return 0;

	CLoginWnd* pLoginWnd = new CLoginWnd;
	if (pLoginWnd == NULL)
		return 0;

	pLoginWnd->Create(NULL, _T("Login Window"), UI_WNDSTYLE_DIALOG/*UI_WNDSTYLE_FRAME*/, WS_EX_WINDOWEDGE);
	::SendMessage(pLoginWnd->GetHWND(),STM_SETICON,IMAGE_ICON,(LPARAM)(UINT)hIcon);
	pLoginWnd->CenterWindow();
	//pLoginWnd->ShowWindow();
	pLoginWnd->ShowModal();

	CPaintManagerUI::MessageLoop();
	delete pLoginWnd;
	pLoginWnd = NULL;
	::CoUninitialize();
	return 0;
}