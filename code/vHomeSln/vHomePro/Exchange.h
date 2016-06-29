#pragma once
#include <string>
#include <vector>
#include "Auth.h"

using namespace std;

// 交易所信息实体类
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
	unsigned int m_nId; // 交易所id
	int m_nNum;			// 参加人数
	string m_strName;				// 交易所名称
	string m_strInfo;				// 交易所简介
	string m_strImage;				// 交易所图片
	vector<string> m_vecAuth;		// 交易所认证 
};

