#include "ForgetPwd.h"


CForgetPwd::CForgetPwd(void)
{
}


CForgetPwd::~CForgetPwd(void)
{
}

void CForgetPwd::SetPhone(const string& strPhone)
{
	m_strPhone = strPhone;
}
void CForgetPwd::SetYzm(const string& strYzm)
{
	m_strYzm = strYzm;
}
void CForgetPwd::SetPasswd(const string& strPasswd)
{
	m_strPasswd = strPasswd;
}
void CForgetPwd::SetRepasswd(const string& strRepasswd)
{
	m_strRepasswd = strRepasswd;
}

string CForgetPwd::GetPhone() const
{
	return m_strPhone;
}
string CForgetPwd::GetYzm() const
{
	return m_strYzm;
}
string CForgetPwd::GetPasswd() const
{
	return m_strPasswd;
}
string CForgetPwd::GetRepasswd() const
{
	return m_strRepasswd;
}