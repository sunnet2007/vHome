#pragma once
#include <string>
using namespace std;

// ��Ӧ��Ϣʵ����
class CResMsg
{
public:
	CResMsg(void);
	~CResMsg(void);

public:
	void SetStatus(int nStatus);
	void SetInfo(const string& strInfo);

	int GetStatus() const;
	string GetInfo() const;

private:
	int m_nStatus;		// ��Ӧ��
	string m_strInfo;	// ��Ӧ��Ϣ
};

