#include "ExchangeLogic.h"
#include "ExchangeDao.h"

CExchangeLogic::CExchangeLogic(void)
{
}


CExchangeLogic::~CExchangeLogic(void)
{
}

// 所有交易所
int CExchangeLogic::GetAllExchanges(vector<CExchange>& vecExchange, CResMsg& msg)
{
	CExchangeDao exchangeDao;
	vector<CBanner> vecBanner;
	int nRes = exchangeDao.GetAllExchanges(vecExchange, vecBanner, msg);
	return nRes;
	if (nRes != 1) // 失败
	{

	}
}

// 根据id获取交易所
int CExchangeLogic::GetExchange(CExchange& exchange, CResMsg& msg)
{
	CExchangeDao exchangeDao;
	int nRes = exchangeDao.GetExchangeInfoById(exchange, msg);
	return nRes;
}

// 获取交易所下的所有讲师
int CExchangeLogic::GetAllTeacherOfExc(vector<CTeacher>& vecTeacher,int nExid, const string& strToken, CResMsg& msg)
{
	CExchangeDao exchangeDao;
	//vector<CTeacher> vecTeacher;
	int nRes = exchangeDao.GetTeacherListByExid(vecTeacher, nExid, strToken, msg);
	return nRes;
}