#include "ExchangeLogic.h"
#include "ExchangeDao.h"

CExchangeLogic::CExchangeLogic(void)
{
}


CExchangeLogic::~CExchangeLogic(void)
{
}

// ���н�����
int CExchangeLogic::GetAllExchanges(vector<CExchange>& vecExchange, CResMsg& msg)
{
	CExchangeDao exchangeDao;
	vector<CBanner> vecBanner;
	int nRes = exchangeDao.GetAllExchanges(vecExchange, vecBanner, msg);
	return nRes;
	if (nRes != 1) // ʧ��
	{

	}
}

// ����id��ȡ������
int CExchangeLogic::GetExchange(CExchange& exchange, CResMsg& msg)
{
	CExchangeDao exchangeDao;
	int nRes = exchangeDao.GetExchangeInfoById(exchange, msg);
	return nRes;
}

// ��ȡ�������µ����н�ʦ
int CExchangeLogic::GetAllTeacherOfExc(vector<CTeacher>& vecTeacher,int nExid, const string& strToken, CResMsg& msg)
{
	CExchangeDao exchangeDao;
	//vector<CTeacher> vecTeacher;
	int nRes = exchangeDao.GetTeacherListByExid(vecTeacher, nExid, strToken, msg);
	return nRes;
}