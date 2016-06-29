#include "User.h"


CUser::CUser(void)
{
	m_nUid			= 0;
	m_strNicName	= "";    // 用户名 
	m_strPhone		= "";   // 手机
	m_strToken		= "";   // 登录令牌
	m_strIcon		= "";    // 用户头像
	m_strPassword	=""; // 登录密码，加密为 md5(base64_encode(md5()));
}

CUser::~CUser(void)
{
}
/**********************
SetXxx()
************************/
void CUser::SetUid(unsigned int nUid)
{
	m_nUid = nUid;
}
void CUser::SetNicName(const string& strNicName)
{
	m_strNicName = strNicName;
}
void CUser::SetPhone(const string& strPhone)
{
	m_strPhone = strPhone;
}
void CUser::SetToken(const string& strToken)
{
	m_strToken = strToken;
}
void CUser::SetIcon(const string& strIcon)
{
	m_strIcon = strIcon;
}
void CUser::SetPassword(const string& strPassword)
{
	m_strPassword = strPassword;
}

/**********************
GetXxx()
************************/
unsigned int CUser::GetUid()
{
	return m_nUid;
}
string CUser::GetNicName()
{
	return m_strNicName;
}
string CUser::GetPhone()
{
	return m_strPhone;
}
string CUser::GetToken()
{
	return m_strToken;
}
string CUser::GetIcon()
{
	return m_strIcon;
}
string CUser::GetPassword()
{
	return m_strPassword;
}
