#pragma once
#include <string>
#include "User.h"
#include "RegisterInfo.h"
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
};

