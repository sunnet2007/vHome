#pragma once
#include "Exchange.h"
#include "Teacher.h"
#include "ResMsg.h"

class CExchangeLogic
{
public:
	CExchangeLogic(void);
	~CExchangeLogic(void);

public:
	// 获取所有交易所
	int GetAllExchanges(const string& strToken, vector<CExchange>& vecExchange, string& strMsg);

	// 获取交易所信息
	int GetExchange(CExchange& exchange, string& strMsg);

	// 获取交易所下的所有讲师信息
	int GetAllTeacherOfExc(vector<CTeacher>& vecTeacher,int nExid, const string& strToken, string& strMsg);

	// 讲师获取所有交易所
	int GetExchanges(vector<CExchange>& vecExchange, string& strMsg);
};


