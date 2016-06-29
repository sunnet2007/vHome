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
	int GetAllExchanges(vector<CExchange>& vecExchange, CResMsg& msg);

	// 获取交易所信息
	int GetExchange(CExchange& exchange, CResMsg& msg);

	// 获取交易所下的所有讲师信息
	int GetAllTeacherOfExc(vector<CTeacher>& vecTeacher,int nExid, const string& strToken, CResMsg& msg);
};


