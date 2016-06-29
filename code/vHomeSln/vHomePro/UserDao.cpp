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

// ������֤��
// ����ֵ1�ɹ�����1ʧ��
int CUserDao::SendCode(const string &strPhone,  CResMsg& msg )
{
	CHttpClient client;

	// ������ʽ
	client.SetReqArg("phone", strPhone);
	// ����ʽ
	client.SetReqCmd("POST");
	// ����·��
	//string strUrl = m_strURL + "/index.php/app/nos_sendyzm";
	if (-1 == client.AccessUrl(CAppConfig::NOS_SENDYZM))
	{
		msg.SetInfo("�������");
		msg.SetStatus(-1);
		return 1;
	}
	// ��Ӧ״̬��
	int nStatusCode= client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetInfo("�������");
		msg.SetStatus(-1);
		return 1;
	}

	// ��ȡ��Ӧ����
	string strResBody;
	client.GetResBody(strResBody);

	// ������Ӧ����
	Json::Reader reader;
	Json::Value root;

	if (reader.parse(strResBody, root))
	{
		// ״̬
		string strStatus  = root["status"].asString();
		int nStatus = atoi(strStatus.c_str());

		string strInfo = root["info"].asString();

//		string strData = root["data"].asString();
		msg.SetStatus(nStatus);
		msg.SetInfo(strInfo);
		return 0;
	}
	msg.SetInfo("�������");
	msg.SetStatus(-1);
	return 1;
	return 11;
}

// ע��
// ����ֵ1�ɹ�����1ʧ��
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
		msg.SetInfo("�������");
		msg.SetStatus(-1);
		return -1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetInfo("�������");
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

	msg.SetInfo("�������");
	msg.SetStatus(-1);
	return -1;
}

// ��¼
int CUserDao::Login(CUser& user, CResMsg& msg)
{
	CHttpClient client;
	// ��������
	client.SetReqArg("phone", user.GetPhone());
	client.SetReqArg("userpwd", user.GetPassword());

	//POST����
	client.SetReqCmd("POST");

	//string strUrl = m_strURL + "/index.php/app/nos_login";
	int nRes = client.AccessUrl(CAppConfig::NOS_LOGIN);
	if (nRes == -1)
	{
		msg.SetInfo("�������");
		msg.SetStatus(-1);
		return -1;
	}
	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetInfo("�������");
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
		if (nStatus == 1) // ��¼�ɹ�
		{
			string strUid = root["data"]["uid"].asString();			
			string strNicName = root["data"]["username"].asString();			
			string strToken = root["data"]["token"].asString();			
			string strIcon = root["data"]["imgthumb"].asString();

			user.SetUid(atoi(strUid.c_str()));
			user.SetNicName(strNicName);
			user.SetToken(strToken);
			user.SetIcon(strIcon);

			// ����Զ��ͼ�񵽱���
			wstring wstrIcon = s2ws(user.GetIcon());
			string strDest = "./UIFile/icon_" + strUid + ".png";
			wstring wstrDest = s2ws(strDest);
			URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
			return 0;
		}
	}
	msg.SetInfo("�������");
	msg.SetStatus(-1);
	return -1;
}

// �޸�����
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

// ע��
int CUserDao::Logout(CUser& user, CResMsg& msg)
{
	return 0;
}