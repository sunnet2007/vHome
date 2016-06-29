#include "UserManagerLogic.h"
#include "UserDao.h"
#include "./Common/Utility/Base64.h"
#include "./Common/Utility/Md5.h"
#include <regex>
CUserManagerLogic::CUserManagerLogic(void)
{
}


CUserManagerLogic::~CUserManagerLogic(void)
{
}


// ������֤��
// ����ֵ1�ɹ�����1ʧ��
int CUserManagerLogic::SendCode(const string &strPhone, string& strInfo)
{
	// �ֻ��Ÿ�ʽ���
	regex pattern("^(0|86|17951)?(13[0-9]|15[012356789]|17[678]|18[0-9]|14[57])[0-9]{8}$");//^1[3|4|5|7|8]\\d{9}$
	if (!regex_match(strPhone, pattern))
	{
		return -1;
	}
	

	CUserDao userDao;
	CResMsg msg;
	return userDao.SendCode(strPhone,msg);
}

// ע��
// ����ֵ1�ɹ�����1ʧ��
int CUserManagerLogic::Register(CRegisterInfo& regInfo, string& strInfo)
{
	// �ֻ��Ÿ�ʽ���
	regex pattern("^(0|86|17951)?(13[0-9]|15[012356789]|17[678]|18[0-9]|14[57])[0-9]{8}$");
	if (!regex_match(regInfo.GetPhone(), pattern))
	{
		strInfo = "��������ȷ���ֻ���";
		return -1;
	}
	// �����ʽ���
	string strPass = regInfo.GetPasswd();
	if (strPass.length() <6 || strPass.length() > 12)
	{
		strInfo = "�����ʽ����";
		return -1;
	}

	// �������
	if (regInfo.GetPasswd() != regInfo.GetRepasswd()) // ���벻һ��
	{
		strInfo = "���벻һ��,����������";
		return -1;
	}

	strPass = md5(CBase64::Encode(md5(regInfo.GetPasswd())+"yykjAwdx"));
	string strRepass = md5(CBase64::Encode(md5(regInfo.GetRepasswd())+"yykjAwdx"));
	regInfo.SetPasswd(strPass);
	regInfo.SetRepasswd(strRepass);

	CUserDao uesrDao;
	CResMsg msg;
	return uesrDao.Register(regInfo, msg);
}

// ��¼
int CUserManagerLogic::Login(CUser& user,string& strInfo)
{
	// �ֻ��Ÿ�ʽ���
	regex pattern("^(0|86|17951)?(13[0-9]|15[012356789]|17[678]|18[0-9]|14[57])[0-9]{8}$");
	if (!regex_match(user.GetPhone(), pattern))
	{
		strInfo = "��������ȷ���ֻ���";
		return -1;
	}
	// �����ʽ���
	string strPass = user.GetPassword();
	if (strPass.length() <6 || strPass.length() > 12)
	{
		strInfo = "�����ʽ����";
		return -1;
	}
	// �������
	user.SetPassword(md5(CBase64::Encode(md5(user.GetPassword()) + "yykjAwdx")));

	CUserDao userDao;
	CResMsg msg;
	return userDao.Login(user, msg);
}