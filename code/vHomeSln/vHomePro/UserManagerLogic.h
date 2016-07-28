#pragma once
#include <string>
#include <vector>
#include "User.h"
#include "RegisterInfo.h"
#include "ForgetPwd.h"
using namespace std;

// �û������߼���
class CUserManagerLogic
{
public:
	CUserManagerLogic(void);
	~CUserManagerLogic(void);
public:
	int SendCode(const string &strPhone, string& strInfo); // ������֤��
	int Register(CRegisterInfo& regInfo, string& strInfo);// ע��
	int Login(CUser& user,string& strInfo);

	int GetPwd(CForgetPwd& forget, string& strMsg);
	int SendZhYzm(const string& strPhone, string& strMsg);
	int UploadUserImage(const string& strToken, const string& strImg, string& strMsg);
	// ��ʾ�û���Ϣ
	int GetUserInfo(CUser& user, string& strMsg);
	// �޸��û���Ϣ
	int ModifyUserInfo(const CUser& user,  string& strMsg);

	// ���ؼ�ס������Ϣ
	int LoadRememberInfo(vector<Remember> &vecRemember);
	// �����ס������Ϣ
	int SaveRememberInfo(vector<Remember> &vecRemember);
};

