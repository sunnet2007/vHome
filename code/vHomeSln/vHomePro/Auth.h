#pragma once
#include <string>
using namespace std;

// ��֤��Ϣ
class CAuth
{
public:
	CAuth(void);
	~CAuth(void);

public:
	void SetId(int nId);
	void SetName(string strName);

	int GetId();
	string GetName();

private:
	int m_nId;
	string m_strName;
};

