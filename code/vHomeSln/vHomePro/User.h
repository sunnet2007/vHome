#pragma once
#include <string>
using namespace std;
// 用户信息实体类
class CUser
{
public:
	CUser(void);
	~CUser(void);

public:
	void SetUid(unsigned int nUid);
	void SetNicName(const string& strNicName);
	void SetPhone(const string& strPhone);
	void SetToken(const string& strToken);
	void SetIcon(const string& strIcon);
	void SetPassword(const string& strPassword);

	unsigned int GetUid();
	string GetNicName();
	string GetPhone();
	string GetToken();
	string GetIcon();
	string GetPassword();

protected:
	unsigned int m_nUid; // 用户id
	string m_strNicName;    // 昵称 
	string m_strPhone;   // 手机
	string m_strToken;   // 登录令牌
	string m_strIcon;    // 用户头像
	string m_strPassword; // 登录密码，加密为 md5(base64_encode(md5()));
};

