#pragma once
#include <string>
using namespace std;

class CRegisterInfo
{
public:
	CRegisterInfo(void);
	~CRegisterInfo(void);

public:
	void SetPhone(const string& strPhone);
	void SetYzm(const string& strYzm);
	void SetPasswd(const string& strPasswd);
	void SetRepasswd(const string& strRepasswd);
	void SetType(int nType);


	string GetPhone();
	string GetYzm();
	string GetPasswd();
	string GetRepasswd();
	int GetType();


protected:
	string m_strPhone;// �ֻ���
	string m_strYzm; // ��֤��
	string m_strPasswd; // ����
	string m_strRepasswd;// ȷ������
	int m_nType;// ע���û����� 1 �û� 2 ��ʦ
};

