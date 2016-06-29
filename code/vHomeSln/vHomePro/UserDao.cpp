#include "UserDao.h"
#include "./Common/Network/HttpClient.h"
#include "./Common/Jsoncpp/json/json.h"
#include "./Common/Utility/StringFuns.h"
#include <tchar.h>
#include "StringUtil.h"
#include "AppConfig.h"

#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")


CUserDao::CUserDao(void)
{


}


CUserDao::~CUserDao(void)
{
}

// 发送验证码
// 返回值1成功，非1失败
int CUserDao::SendCode(const string &strPhone,  CResMsg& msg )
{
	CHttpClient client;

	// 参数格式
	client.SetReqArg("phone", strPhone);
	// 请求方式
	client.SetReqCmd("POST");
	// 请求路径
	//string strUrl = m_strURL + "/index.php/app/nos_sendyzm";
	if (-1 == client.AccessUrl(CAppConfig::NOS_SENDYZM))
	{
		msg.SetInfo("网络错误");
		msg.SetStatus(-1);
		return 1;
	}
	// 响应状态码
	int nStatusCode= client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetInfo("网络错误");
		msg.SetStatus(-1);
		return 1;
	}

	// 获取相应数据
	string strResBody;
	client.GetResBody(strResBody);

	// 解析响应数据
	Json::Reader reader;
	Json::Value root;

	if (reader.parse(strResBody, root))
	{
		// 状态
		string strStatus  = root["status"].asString();
		int nStatus = atoi(strStatus.c_str());

		string strInfo = root["info"].asString();

//		string strData = root["data"].asString();
		msg.SetStatus(nStatus);
		msg.SetInfo(strInfo);
		return 0;
	}
	msg.SetInfo("网络错误");
	msg.SetStatus(-1);
	return 1;
	return 11;
}

// 注册
// 返回值1成功，非1失败
int CUserDao::Register(CRegisterInfo& regInfo, CResMsg& msg)
{
	CHttpClient client;

	client.SetReqArg("phone", regInfo.GetPhone());
	client.SetReqArg("yzm", regInfo.GetYzm());
	client.SetReqArg("passwd", regInfo.GetPasswd());
	client.SetReqArg("repasswd", regInfo.GetRepasswd());
	client.SetReqArg("type", 1);

	client.SetReqCmd("POST");

	//string strUrl = m_strURL + "/index.php/app/nos_reg";
	if (-1 == client.AccessUrl(CAppConfig::NOS_REG))
	{
		msg.SetInfo("网络错误");
		msg.SetStatus(-1);
		return -1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetInfo("网络错误");
		msg.SetStatus(-1);
		return -1;
	}

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		msg.SetStatus(atoi(strStatus.c_str()));
		string strInfo = root["info"].asString();
		msg.SetInfo(strInfo);
		return 0;
	}

	msg.SetInfo("网络错误");
	msg.SetStatus(-1);
	return -1;
}

// 登录
int CUserDao::Login(CUser& user, CResMsg& msg)
{
	CHttpClient client;
	// 请求数据
	client.SetReqArg("phone", user.GetPhone());
	client.SetReqArg("userpwd", user.GetPassword());

	//POST请求
	client.SetReqCmd("POST");

	//string strUrl = m_strURL + "/index.php/app/nos_login";
	int nRes = client.AccessUrl(CAppConfig::NOS_LOGIN);
	if (nRes == -1)
	{
		msg.SetInfo("网络错误");
		msg.SetStatus(-1);
		return -1;
	}
	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetInfo("网络错误");
		msg.SetStatus(-1);
		return -1;
	}


	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		int nStatus = atoi(strStatus.c_str());
		string strInfo = root["info"].asString();
		msg.SetStatus(nStatus);
		msg.SetInfo(strInfo);
		if (nStatus == 1) // 登录成功
		{
			string strUid = root["data"]["uid"].asString();			
			string strNicName = root["data"]["username"].asString();			
			string strToken = root["data"]["token"].asString();			
			string strIcon = root["data"]["imgthumb"].asString();

			user.SetUid(atoi(strUid.c_str()));
			user.SetNicName(strNicName);
			user.SetToken(strToken);
			user.SetIcon(strIcon);

			// 下载远程图像到本地
			wstring wstrIcon = s2ws(user.GetIcon());
			string strDest = "./UIFile/icon_" + strUid + ".png";
			wstring wstrDest = s2ws(strDest);
			URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
			return 0;
		}
	}
	msg.SetInfo("网络错误");
	msg.SetStatus(-1);
	return -1;
}

// 修改密码
int CUserDao::ModifyPwd(CUser& user, const string& strNewPwd, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqCmd("POST");

	client.SetReqArg("token", user.GetToken());
	client.SetReqArg("oldpassword", user.GetPassword());
	client.SetReqArg("newpassword", strNewPwd);

	//string strUrl = m_strURL + "/index.php/app/nos_Modifypassword";
	client.AccessUrl(CAppConfig::NOS_MODIFYPASSWORD);

	int nStatus = client.GetResStatusCode();
	if (nStatus != 200)
		return -1;
	return 0;
}

// 注销
int CUserDao::Logout(CUser& user, CResMsg& msg)
{
	return 0;
}