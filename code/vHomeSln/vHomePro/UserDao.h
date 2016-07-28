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
	int SendCode(const string &strPhone, CResMsg& msg); // 发送验证码
	int Register(CRegisterInfo& regInfo, CResMsg& msg);// 注册
	int Login(CUser& user, CResMsg& msg);
	int ModifyPwd(CUser& user, const string& strNewPwd, CResMsg& msg);
	int Logout(CUser& user, CResMsg& msg);
	int GetPwd(const CForgetPwd& forget, CResMsg& msg);
	int SendZhYzm(const string& strPhone, CResMsg& msg);
	int UploadUserImage(const string& strToken, const string& strImg, CResMsg& msg);
	// 显示用户信息
	int GetUserInfo(CUser& user, CResMsg& msg);
	// 修改用户信息
	int ModifyUserInfo(const CUser& user,  CResMsg& msg);

	// 加载记住密码信息
	int LoadRememberInfo(vector<Remember> &vecRemInfo, string& strInfo);
	// 保存记住密码信息
	int SaveRememberInfo(vector<Remember> vecRemInfo);
};

