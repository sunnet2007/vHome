#include "RegisterInfo.h"


CRegisterInfo::CRegisterInfo(void)
{
	m_strPhone = "";//�ֻ���
	m_strYzm = ""; // ��֤��
	m_strPasswd = ""; // ����
	m_strRepasswd = "";// ȷ������
	m_nType = 0;// ע���û����� 1 �û� 2 ��ʦ
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
