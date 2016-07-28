#pragma once
#include <string>
using namespace std;
// �û���Ϣʵ����

struct Remember
{
	char strPhone[16];	// �˺�
	char strPwd[32];	// ����
	bool bRemember;		// �Ƿ��ס����
	bool bAuto;			// �Ƿ��Զ���¼
	//bool bLogin;		// �Ƿ��ס����
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
	unsigned int m_nUid; // �û�id
	string m_strNicName;    // �ǳ� 
	string m_strPhone;   // �ֻ�
	string m_strToken;   // ��¼����
	string m_strIcon;    // �û�ͷ��
	string m_strPassword; // ��¼���룬����Ϊ md5(base64_encode(md5()));
	int m_nType;		// �û�����0ѧԱ��1 ��ʦ
	string m_strName;//�û�����
	string m_strSex;		//�Ա�0Ů1��
	string m_strEmail;	//����
	string m_strQq;		// QQ
};

