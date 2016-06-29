#pragma once
#include <string>
#include <vector>
#include "Auth.h"

using namespace std;

// ��������Ϣʵ����
class CExchange
{
public:
	CExchange(void);
	~CExchange(void);

public:
	void SetId(unsigned int nId);
	void SetName(const string& strName);
	void SetInfo(const string& strInfo);
	void SetImage(const string strImage);
	void SetNum(int nNum);
	void SetAuth(vector<string> strAuth);

	unsigned int GetId() const;
	string GetName() const;
	string GetInfo() const;
	string GetImage() const;
	int GetNum() const;
	vector<string> GetAuth() const;

protected:
	unsigned int m_nId; // ������id
	int m_nNum;			// �μ�����
	string m_strName;				// ����������
	string m_strInfo;				// ���������
	string m_strImage;				// ������ͼƬ
	vector<string> m_vecAuth;		// ��������֤ 
};

