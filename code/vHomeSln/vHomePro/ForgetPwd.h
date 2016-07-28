#pragma once
#include <string>
using namespace std;

// �һ�����ʵ��
class CForgetPwd
{
public:
	CForgetPwd(void);
	~CForgetPwd(void);

public:
	void SetPhone(const string& strPhone);
	void SetYzm(const string& strYzm);
	void SetPasswd(const string& strPasswd);
	void SetRepasswd(const string& strRepasswd);

	string GetPhone() const;
	string GetYzm() const;
	string GetPasswd() const;
	string GetRepasswd() const;

private:
	string m_strPhone;		// �ֻ���
	string m_strYzm;		// ��֤��
	string m_strPasswd;		// ��������
	string m_strRepasswd;	// �ظ�����
};

