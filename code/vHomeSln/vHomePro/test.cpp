//#include <windows.h>
//#include <string>
//using namespace std;
//#include "Exchange.h"
//#include "ExchangeLogic.h"
//#include "Banner.h"
//#include "ExchangeDao.h"
//#include "TeacherDao.h"
//
//string CAppConfig::NOS_FENXI = "";
//string CAppConfig::NOS_TEACHERS = ""; 
//string CAppConfig::NOS_TEACHER_EXCHANGEINFO = "";
//string CAppConfig::NOS_SENDYZM = "";
//string CAppConfig::NOS_REG = "";
//string CAppConfig::NOS_LOGIN = "";
//string CAppConfig::NOS_MODIFYPASSWORD = "";
//string CAppConfig::ADDATTENTION = "";
//string CAppConfig::USER_TEACHER_INFO = "";
//string CAppConfig::DELATTENTION = "";
//string CAppConfig::NOS_TOPTEACHER = "";
//string CAppConfig::NOS_STARTEACHER = "";
//
//int Init()
//{
//	WORD wV;
//	WSADATA ws;
//	int er;
//	wV = MAKEWORD(1, 1);
//	er = WSAStartup(wV, &ws);
//	if(er != 0)
//	{
//		return -1;
//	}
//
//	if(LOBYTE(ws.wVersion) != 1 || HIBYTE(ws.wVersion) != 1)
//	{
//		WSACleanup();
//		return -1;
//	}
//	Server server;
//	CAppConfig::GetServer(server);
//	CAppConfig::NOS_FENXI					= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_fenxi";
//	CAppConfig::NOS_TEACHERS				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_teachers";
//	CAppConfig::NOS_TEACHER_EXCHANGEINFO 	= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_teacher_exchangeinfo";
//	CAppConfig::NOS_SENDYZM					= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_sendyzm";
//	CAppConfig::NOS_REG						= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_reg";
//	CAppConfig::NOS_LOGIN 					= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_login";
//	CAppConfig::NOS_MODIFYPASSWORD			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_Modifypassword";
//	CAppConfig::ADDATTENTION				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/addAttention";
//	CAppConfig::USER_TEACHER_INFO			= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/user_teacher_info";
//	CAppConfig::DELATTENTION				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/delAttention";
//	CAppConfig::NOS_TOPTEACHER				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_topteacher"; 
//	CAppConfig::NOS_STARTEACHER				= server.strProtocol + "://" + server.strHost + ":" + server.strPort + "/index.php/app/nos_starteacher"; 
//	return 0;
//}
//
//int APIENTRY WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
//{
//	if (-1 == Init())
//	{
//		return 0;
//	}
//	vector<CTeacher> teachers;
//	string strToken = "2e935ee6c6be25ec62169425e150b414";
//	CResMsg msg;
//	CTeacherDao teacherDao;
//	//teacherDao.GetTopTeacher(strToken,teachers, msg);
//	teacherDao.GetStarTeacher(1,strToken,teachers,msg);
//	return 0;
//}