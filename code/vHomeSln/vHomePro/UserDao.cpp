#include "UserDao.h"
#include "./Common/Network/HttpClient.h"
#include "./Common/Jsoncpp/json/json.h"
#include "./Common/Utility/base_file.h"
//#include "./Common/Jsoncpp/include/json/json.h"
#include "./Common/Utility/StringFuns.h"
#include "./Common/Utility/Base64.h"
#include <tchar.h>
#include "StringUtil.h"
#include "AppConfig.h"
#include "ForgetPwd.h"

#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")
#define FILEPATH "vhome.dat"

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
	client.SetReqArg("type", regInfo.GetType());

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
	//client.SetReqArg("title", "sdfdsfds ");

	//POST����
	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::NOS_C_LOGIN);
	//int nRes = client.AccessUrl(CAppConfig::NOS_TEST);
	if (nRes == -1)
	{
		msg.SetInfo("�������");
		msg.SetStatus(-1);
		return 1;
	}
	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetInfo("�������");
		msg.SetStatus(-1);
		return 1;
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
			string strType = root["data"]["type"].asString();
			int nType = atoi(strType.c_str());
			user.SetUid(atoi(strUid.c_str()));
			user.SetNicName(strNicName);
			user.SetToken(strToken);
			int nPos = strIcon.find_last_of('/');
			string srtFileName(strIcon.substr(nPos+1));
			user.SetIcon(srtFileName);
			user.SetType(nType);

			
			// ����Զ��ͼ�񵽱���
			wstring wstrIcon = s2ws(strIcon);
			string strDest = "./UIFile/" + srtFileName;
			wstring wstrDest = s2ws(strDest);
			HRESULT hr = URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(), 0, 0);
			if (FAILED(hr))
			{
				DWORD dwError = GetLastError();
			}
			return 0;
		}
		return 1;
	}
	msg.SetInfo("�������");
	msg.SetStatus(-1);
	return 1;
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

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
		return -1;
	return 0;
}

// �����һ�������֤��
int CUserDao::SendZhYzm(const string& strPhone, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("phone", strPhone);
	client.SetReqCmd("POST");
	int nRes = client.AccessUrl(CAppConfig::NOS_SENDZHYZM);
	if (nRes != 0)
	{
		msg.SetStatus(-1);
		msg.SetInfo("�������");
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetStatus(-1);
		msg.SetInfo("�������");
		return 1;
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
		return 0;
	}

	msg.SetStatus(-1);
	msg.SetInfo("�������");
	return 1;
}

// �һ�����
int CUserDao::GetPwd(const CForgetPwd& forget, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("phone", forget.GetPhone());
	client.SetReqArg("yzm", forget.GetYzm());
	client.SetReqArg("passwd", forget.GetPasswd());
	client.SetReqArg("repasswd", forget.GetRepasswd());
	client.SetReqCmd("POST");
	int nRes = client.AccessUrl(CAppConfig::NOS_GETPWD);
	if (nRes != 0)
	{
		msg.SetStatus(-1);
		msg.SetInfo("�������");
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetStatus(-1);
		msg.SetInfo("�������");
		return 1;
	}

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		string strInfo = root["info"].asString();
		int nStatus = atoi(strStatus.c_str());
		msg.SetStatus(nStatus);
		msg.SetInfo(strInfo);
		if (nStatus == 1)
			return 0;
		return 1;
	}

	msg.SetStatus(-1);
	msg.SetInfo("�������");
	return 1;
}

// ע��
int CUserDao::Logout(CUser& user, CResMsg& msg)
{
	return 0;
}

// �ϴ��û�ͷ��
int CUserDao::UploadUserImage(const string& strToken, const string& strImg, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("token", strToken);
	client.SetReqArg("image", strImg);
	client.SetReqCmd("POST");
	int nRes = client.AccessUrl(CAppConfig::NOS_USERIMAGE);
	if (nRes != 0)
	{
		msg.SetStatus(-1);
		msg.SetInfo("�������");
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetStatus(-1);
		msg.SetInfo("�������");
		return 1;
	}

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		string strInfo = root["info"].asString();
		int nStatus = atoi(strStatus.c_str());
		msg.SetStatus(nStatus);
		msg.SetInfo(strInfo);
		if (nStatus == 1)
			return 0;
		return 1;
	}

	msg.SetStatus(-1);
	msg.SetInfo("�������");
	return 1;
}

// ��ʾ�û���Ϣ
int CUserDao::GetUserInfo(CUser& user, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("token", user.GetToken());
	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::NOS_USERINFO_VIEW);
	if (nRes != 0)
	{
		msg.SetInfo("�������");
		msg.SetStatus(-1);
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetInfo("�������");
		msg.SetStatus(-1);
		return 1;
	}

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		string strInfo = root["info"].asString();
		int nStatus = atoi(strStatus.c_str());
		msg.SetStatus(nStatus);
		msg.SetInfo(strInfo);
		if (nStatus != 1)
			return 1;
		
		Json::Value data = root["data"];
		string strNicName = data["username"].asString();
		string strName = data["name"].asString();
		string strSex = data["sex"].asString();
		string strEmail = data["email"].asString();
		string strQq = data["qq"].asString();

		user.SetNicName(strNicName);
		user.SetName(strName);
		user.SetSex(strSex);
		user.SetEmail(strEmail);
		user.SetQq(strQq);

		return 0;
	}
	msg.SetInfo("�������");
	msg.SetStatus(-1);
	return 1;
}

// �޸��û���Ϣ
int CUserDao::ModifyUserInfo(const CUser& user, CResMsg& msg)
{
	CHttpClient client;

	client.SetReqCmd("POST");
	client.SetReqArg("token", user.GetToken());
	client.SetReqArg("username", user.GetNicName());
	client.SetReqArg("name", user.GetName());
	client.SetReqArg("email", user.GetEmail());
	client.SetReqArg("flag", "C");
	string strSex = user.GetSex();

	client.SetReqArg("sex", strSex);

	int nRes = client.AccessUrl(CAppConfig::NOS_USERINFO_MODI);
	if (nRes != 0)
	{
		msg.SetInfo("�������");
		msg.SetStatus(-1);
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetInfo("�������");
		msg.SetStatus(-1);
		return 1;
	}

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		string strInfo = root["info"].asString();
		int nStatus = atoi(strStatus.c_str());
		msg.SetStatus(nStatus);
		msg.SetInfo(strInfo);
		if (nStatus != 1)
			return 1;
		return 0;
	}

	msg.SetInfo("�������");
	msg.SetStatus(-1);
	return 1;
}


// ���ؼ�ס������Ϣ
int CUserDao::LoadRememberInfo(vector<Remember> &vecRemInfo, string& strInfo)
{

	FILE* fp = NULL;
	fp = fopen(FILEPATH, "rb");
	if (fp == NULL)
	{
		return -1;
	}

	Remember remInfo;
	while(fread(&remInfo, sizeof(Remember), 1, fp) > 0)
	{
		string strPwd = CBase64::Decode(remInfo.strPwd);
		strcpy(remInfo.strPwd, strPwd.data());
		vecRemInfo.push_back(remInfo);
	}

	fclose(fp);

	return 0;

}

// �����ס����
int CUserDao::SaveRememberInfo(vector<Remember> vecRemInfo)
{
	FILE* fp = NULL;
	fp = fopen(FILEPATH, "wb");
	if (fp == NULL)
	{
		return -1;
	}

	int nSize = vecRemInfo.size();
	for (int i = 0; i < nSize; i++)
	{
		Remember remInfo = vecRemInfo[i];
		string strPwd = CBase64::Encode(remInfo.strPwd);
		strcpy(remInfo.strPwd, strPwd.c_str());
		fwrite(&remInfo, sizeof(Remember), 1, fp);

	}
	fclose(fp);

	return 0;
}