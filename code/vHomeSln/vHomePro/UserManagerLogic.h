#pragma once
#include <string>
#include <vector>
#include "User.h"
#include "RegisterInfo.h"
#include "ForgetPwd.h"
using namespace std;

// 用户管理逻辑类
class CUserManagerLogic
{
public:
	CUserManagerLogic(void);
	~CUserManagerLogic(void);
public:
	int SendCode(const string &strPhone, string& strInfo); // 发送验证码
	int Register(CRegisterInfo& regInfo, string& strInfo);// 注册
	int Login(CUser& user,string& strInfo);

	int GetPwd(CForgetPwd& forget, string& strMsg);
	int SendZhYzm(const string& strPhone, string& strMsg);
	int UploadUserImage(const string& strToken, const string& strImg, string& strMsg);
	// 显示用户信息
	int GetUserInfo(CUser& user, string& strMsg);
	// 修改用户信息
	int ModifyUserInfo(const CUser& user,  string& strMsg);

	// 加载记住密码信息
	int LoadRememberInfo(vector<Remember> &vecRemember);
	// 保存记住密码信息
	int SaveRememberInfo(vector<Remember> &vecRemember);
};

