#include "User.h"


CUser::CUser(void)
{
	m_nUid			= 0;
	m_strNicName	= "";    // 用户名 
	m_strPhone		= "";   // 手机
	m_strToken		= "";   // 登录令牌
	m_strIcon		= "";    // 用户头像
	m_strPassword	= ""; // 登录密码，加密为 md5(base64_encode(md5()));
	m_nType			= 0;
	m_strName		= "";//用户姓名
	m_strSex		= "";		//性别，0女1男
	m_strEmail		= "";	//邮箱
	m_strQq			= "";		// QQ
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
void CUser::SetType(int nType)
{
	m_nType = nType;
}
void CUser::SetName(const string& strName)
{
	m_strName = strName;
}
void CUser::SetSex(const string& strSex)
{
	m_strSex = strSex;
}
void CUser::SetEmail(const string& strEmail)
{
	m_strEmail = strEmail;
}
void CUser::SetQq(const string& strQq)
{
	m_strQq = strQq;
}

/**********************
GetXxx()
************************/
unsigned int CUser::GetUid() const
{
	return m_nUid;
}
string CUser::GetNicName() const
{
	return m_strNicName;
}
string CUser::GetPhone() const
{
	return m_strPhone;
}
string CUser::GetToken() const
{
	return m_strToken;
}
string CUser::GetIcon() const
{
	return m_strIcon;
}
string CUser::GetPassword() const
{
	return m_strPassword;
}
int CUser::GetType() const
{
	return m_nType;
}

string CUser::GetName() const
{
	return m_strName;
}
string CUser::GetSex() const
{
	return m_strSex;
}
string CUser::GetEmail() const
{
	return m_strEmail;
}
string CUser::GetQq() const
{
	return m_strQq;
}