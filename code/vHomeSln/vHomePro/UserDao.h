#pragma once
#include <string>
#include "User.h"
#include "RegisterInfo.h"
#include "AppConfig.h"
#include "ResMsg.h"
#include "ForgetPwd.h"
#include <vector>

using namespace std;

class CUserDao
{
public:
	CUserDao(void);
	~CUserDao(void);

public:
	int SendCode(const string &strPhone, CResMsg& msg); // ������֤��
	int Register(CRegisterInfo& regInfo, CResMsg& msg);// ע��
	int Login(CUser& user, CResMsg& msg);
	int ModifyPwd(CUser& user, const string& strNewPwd, CResMsg& msg);
	int Logout(CUser& user, CResMsg& msg);
	int GetPwd(const CForgetPwd& forget, CResMsg& msg);
	int SendZhYzm(const string& strPhone, CResMsg& msg);
	int UploadUserImage(const string& strToken, const string& strImg, CResMsg& msg);
	// ��ʾ�û���Ϣ
	int GetUserInfo(CUser& user, CResMsg& msg);
	// �޸��û���Ϣ
	int ModifyUserInfo(const CUser& user,  CResMsg& msg);

	// ���ؼ�ס������Ϣ
	int LoadRememberInfo(vector<Remember> &vecRemInfo, string& strInfo);
	// �����ס������Ϣ
	int SaveRememberInfo(vector<Remember> vecRemInfo);
};

