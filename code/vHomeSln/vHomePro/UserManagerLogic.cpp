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


// 发送验证码
// 返回值1成功，非1失败
int CUserManagerLogic::SendCode(const string &strPhone, string& strInfo)
{
	// 手机号格式检查
	regex pattern("^(0|86|17951)?(13[0-9]|15[012356789]|17[678]|18[0-9]|14[57])[0-9]{8}$");//^1[3|4|5|7|8]\\d{9}$
	if (!regex_match(strPhone, pattern))
	{
		return -1;
	}
	

	CUserDao userDao;
	CResMsg msg;
	return userDao.SendCode(strPhone,msg);
}

// 注册
// 返回值1成功，非1失败
int CUserManagerLogic::Register(CRegisterInfo& regInfo, string& strInfo)
{
	// 手机号格式检查
	regex pattern("^(0|86|17951)?(13[0-9]|15[012356789]|17[678]|18[0-9]|14[57])[0-9]{8}$");
	if (!regex_match(regInfo.GetPhone(), pattern))
	{
		strInfo = "请输入正确的手机号";
		return -1;
	}
	// 密码格式检查
	string strPass = regInfo.GetPasswd();
	if (strPass.length() <6 || strPass.length() > 12)
	{
		strInfo = "密码格式不对";
		return -1;
	}

	// 密码加密
	if (regInfo.GetPasswd() != regInfo.GetRepasswd()) // 密码不一致
	{
		strInfo = "密码不一致,请重新输入";
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

// 登录
int CUserManagerLogic::Login(CUser& user,string& strInfo)
{
	// 手机号格式检查
	regex pattern("^(0|86|17951)?(13[0-9]|15[012356789]|17[678]|18[0-9]|14[57])[0-9]{8}$");
	if (!regex_match(user.GetPhone(), pattern))
	{
		strInfo = "请输入正确的手机号";
		return -1;
	}
	// 密码格式检查
	string strPass = user.GetPassword();
	if (strPass.length() <6 || strPass.length() > 12)
	{
		strInfo = "密码格式不对";
		return -1;
	}
	// 密码加密
	user.SetPassword(md5(CBase64::Encode(md5(user.GetPassword()) + "yykjAwdx")));

	CUserDao userDao;
	CResMsg msg;
	return userDao.Login(user, msg);
}