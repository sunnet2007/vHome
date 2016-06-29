#pragma once
#include <string>
using namespace std;

class CRegisterInfo
{
public:
	CRegisterInfo(void);
	~CRegisterInfo(void);

public:
	void SetPhone(const string& strPhone);
	void SetYzm(const string& strYzm);
	void SetPasswd(const string& strPasswd);
	void SetRepasswd(const string& strRepasswd);
	void SetType(int nType);


	string GetPhone();
	string GetYzm();
	string GetPasswd();
	string GetRepasswd();
	int GetType();


protected:
	string m_strPhone;// 手机号
	string m_strYzm; // 验证码
	string m_strPasswd; // 密码
	string m_strRepasswd;// 确认密码
	int m_nType;// 注册用户类型 1 用户 2 教师
};

