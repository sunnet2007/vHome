#include "RegisterInfo.h"


CRegisterInfo::CRegisterInfo(void)
{
	m_strPhone = "";//手机号
	m_strYzm = ""; // 验证码
	m_strPasswd = ""; // 密码
	m_strRepasswd = "";// 确认密码
	m_nType = 0;// 注册用户类型 1 用户 2 教师
}


CRegisterInfo::~CRegisterInfo(void)
{
}


void CRegisterInfo::SetPhone(const string& strPhone)
{
	m_strPhone = strPhone;
}
void CRegisterInfo::SetYzm(const string& strYzm)
{
	m_strYzm = strYzm;
}
void CRegisterInfo::SetPasswd(const string& strPasswd)
{
	m_strPasswd = strPasswd;
}
void CRegisterInfo::SetRepasswd(const string& strRepasswd)
{
	m_strRepasswd = strRepasswd;
}
void CRegisterInfo::SetType(int nType)
{
	m_nType = nType;
}


string CRegisterInfo::GetPhone()
{
	return m_strPhone;
}
string CRegisterInfo::GetYzm()
{
	return m_strYzm;
}
string CRegisterInfo::GetPasswd()
{
	return m_strPasswd;
}
string CRegisterInfo::GetRepasswd()
{
	return m_strRepasswd;
}
int CRegisterInfo::GetType()
{
	return m_nType;
}
