#pragma once
#include <string>
using namespace std;

// 响应信息实体类
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
	int m_nStatus;		// 响应码
	string m_strInfo;	// 响应消息
};

