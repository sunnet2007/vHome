#pragma once
#include <string>
#include "User.h"
#include "RegisterInfo.h"
#include "AppConfig.h"
#include "ResMsg.h"

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
};

