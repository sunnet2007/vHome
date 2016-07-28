#pragma once
#include <string>
using namespace std;

// 找回密码实体
class CForgetPwd
{
public:
	CForgetPwd(void);
	~CForgetPwd(void);

public:
	void SetPhone(const string& strPhone);
	void SetYzm(const string& strYzm);
	void SetPasswd(const string& strPasswd);
	void SetRepasswd(const string& strRepasswd);

	string GetPhone() const;
	string GetYzm() const;
	string GetPasswd() const;
	string GetRepasswd() const;

private:
	string m_strPhone;		// 手机号
	string m_strYzm;		// 验证码
	string m_strPasswd;		// 设置密码
	string m_strRepasswd;	// 重复密码
};

