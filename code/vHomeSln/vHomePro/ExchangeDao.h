#pragma once
#include <vector>
#include <string>
#include "Exchange.h"
#include "Banner.h"
#include "Teacher.h"
#include "AppConfig.h"
#include "ResMsg.h"

using namespace std;
// 交易所信息访问类
class CExchangeDao
{
public:
	CExchangeDao(void);
	~CExchangeDao(void);


public:
	// 获取交易所信息列表
	int GetAllExchanges(const string& strToken, vector<CExchange>& vecExchange, vector<CBanner>& vecBanner, CResMsg& msg);
	// 获取交易所讲师列表
	int GetTeacherListByExid(vector<CTeacher>& vecTeacher,int nExid, const string& strToken, CResMsg& msg);
	// 根据ID获取交易所信息
	int GetExchangeInfoById(CExchange& exchange, CResMsg& msg);

	// 讲师首页交易所信息
	int GetExchanges(vector<CExchange>& vecExchange, CResMsg& msg);


private:
	string m_strURL;
};

