#pragma once
#include <string>
using namespace std;
// �û���Ϣʵ����
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
	unsigned int m_nUid; // �û�id
	string m_strNicName;    // �ǳ� 
	string m_strPhone;   // �ֻ�
	string m_strToken;   // ��¼����
	string m_strIcon;    // �û�ͷ��
	string m_strPassword; // ��¼���룬����Ϊ md5(base64_encode(md5()));
};

