#include "UserManagerLogic.h"
#include "UserDao.h"
#include "./Common/Utility/Base64.h"
#include "./Common/Utility/Md5.h"
#include <regex>
#include "ResMsg.h"
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
		strInfo = "��������ȷ���ֻ���";
		return -1;
	}
	

	CUserDao userDao;
	CResMsg msg;
	int nRes = userDao.SendCode(strPhone,msg);
	if (nRes != 0)
	{
		strInfo = msg.GetInfo();
		return -1;
	}
	return 0;
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
	int nRes = uesrDao.Register(regInfo, msg);
	if (nRes != 0)
	{
		strInfo = msg.GetInfo();
		return -1;
	}
	return 0;
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
	int nRes = userDao.Login(user, msg);
	if (nRes != 0)
	{
		strInfo = msg.GetInfo();
		return -1;
	}
	return 0;
}

// �һ�����
int CUserManagerLogic::GetPwd(CForgetPwd& forget, string& strMsg)
{
	CUserDao userDao;
	CResMsg msg;

	// �����ʽ���
	string strPass = forget.GetPasswd();
	if (strPass.length() <6 || strPass.length() > 12)
	{
		strMsg = "�����ʽ����";
		return -1;
	}

	// �������
	if (forget.GetPasswd() != forget.GetRepasswd()) // ���벻һ��
	{
		strMsg = "���벻һ��,����������";
		return -1;
	}

	strPass = md5(CBase64::Encode(md5(forget.GetPasswd())+"yykjAwdx"));
	string strRepass = md5(CBase64::Encode(md5(forget.GetRepasswd())+"yykjAwdx"));
	forget.SetPasswd(strPass);
	forget.SetRepasswd(strRepass);

	int nRes = userDao.GetPwd(forget, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	
	return 0;
}

// �����һ�������֤��
int CUserManagerLogic::SendZhYzm(const string& strPhone, string& strMsg)
{
	CUserDao userDao;
	CResMsg msg;
	int nRes = userDao.SendZhYzm(strPhone, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

int CUserManagerLogic::UploadUserImage(const string& strToken,  const string& strImg, string& strMsg)
{
	CUserDao userDao;
	CResMsg msg;
	int nRes = userDao.UploadUserImage(strToken, strImg, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

// ��ʾ�û���Ϣ
int CUserManagerLogic::GetUserInfo(CUser& user, string& strMsg)
{
	CUserDao userDao;
	CResMsg msg;
	int nRes = userDao.GetUserInfo(user, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}
// �޸��û���Ϣ
int CUserManagerLogic::ModifyUserInfo(const CUser& user,  string& strMsg)
{
	CUserDao userDao;
	CResMsg msg;
	int nRes = userDao.ModifyUserInfo(user, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

// ���ؼ�ס������Ϣ
int CUserManagerLogic::LoadRememberInfo(vector<Remember> &vecRemember)
{
	CUserDao userDao;
	string strMsg;
	int nRes = userDao.LoadRememberInfo(vecRemember, strMsg);
	if (nRes != 0)
	{
		return -1;
	}
	return 0;
}

// �����ס������Ϣ
int CUserManagerLogic::SaveRememberInfo(vector<Remember> &vecRemember)
{
	CUserDao userDao;
	
	int nRes = userDao.SaveRememberInfo(vecRemember);
	if (nRes != 0)
	{
		return -1;
	}
	return 0;
}