#include "ExchangeLogic.h"
#include "ExchangeDao.h"

CExchangeLogic::CExchangeLogic(void)
{
}


CExchangeLogic::~CExchangeLogic(void)
{
}

// 所有交易所
int CExchangeLogic::GetAllExchanges(const string& strToken, vector<CExchange>& vecExchange, string& strMsg)
{
	CExchangeDao exchangeDao;
	vector<CBanner> vecBanner;
	CResMsg msg;
	int nRes = exchangeDao.GetAllExchanges(strToken,vecExchange, vecBanner, msg);
	if (nRes != 0) // 失败
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

// 根据id获取交易所
int CExchangeLogic::GetExchange(CExchange& exchange, string& strMsg)
{
	CExchangeDao exchangeDao;
	CResMsg msg;
	int nRes = exchangeDao.GetExchangeInfoById(exchange, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return nRes;
}

// 获取交易所下的所有讲师
int CExchangeLogic::GetAllTeacherOfExc(vector<CTeacher>& vecTeacher,int nExid, const string& strToken, string& strMsg)
{
	CExchangeDao exchangeDao;
	CResMsg msg;
	//vector<CTeacher> vecTeacher;
	int nRes = exchangeDao.GetTeacherListByExid(vecTeacher, nExid, strToken, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return nRes;
}

// 讲师获取所有交易所
int CExchangeLogic::GetExchanges(vector<CExchange>& vecExchange, string& strMsg)
{
	CExchangeDao exchangeDao;
	CResMsg msg;
	int nRes = exchangeDao.GetExchanges(vecExchange, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}