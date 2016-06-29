#pragma once
#include <string>
#include "User.h"
#include "RegisterInfo.h"
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
};

