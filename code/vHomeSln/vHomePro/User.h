#pragma once
#include <string>
using namespace std;
// 用户信息实体类

struct Remember
{
	char strPhone[16];	// 账号
	char strPwd[32];	// 密码
	bool bRemember;		// 是否记住密码
	bool bAuto;			// 是否自动登录
	//bool bLogin;		// 是否记住密码
};

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
	void SetType(int nType);
	void SetName(const string& strName);
	void SetSex(const string& strSex);
	void SetEmail(const string& strEmail);
	void SetQq(const string& strQq);

	unsigned int GetUid() const;
	string GetNicName() const;
	string GetPhone() const;
	string GetToken() const;
	string GetIcon() const;
	string GetPassword() const;
	int GetType() const;
	string GetName() const;
	string GetSex() const;
	string GetEmail() const;
	string GetQq() const;

protected:
	unsigned int m_nUid; // 用户id
	string m_strNicName;    // 昵称 
	string m_strPhone;   // 手机
	string m_strToken;   // 登录令牌
	string m_strIcon;    // 用户头像
	string m_strPassword; // 登录密码，加密为 md5(base64_encode(md5()));
	int m_nType;		// 用户类型0学员，1 讲师
	string m_strName;//用户姓名
	string m_strSex;		//性别，0女1男
	string m_strEmail;	//邮箱
	string m_strQq;		// QQ
};

