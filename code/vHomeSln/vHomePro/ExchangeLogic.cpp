#include "ExchangeLogic.h"
#include "ExchangeDao.h"

CExchangeLogic::CExchangeLogic(void)
{
}


CExchangeLogic::~CExchangeLogic(void)
{
}

// ���н�����
int CExchangeLogic::GetAllExchanges(const string& strToken, vector<CExchange>& vecExchange, string& strMsg)
{
	CExchangeDao exchangeDao;
	vector<CBanner> vecBanner;
	CResMsg msg;
	int nRes = exchangeDao.GetAllExchanges(strToken,vecExchange, vecBanner, msg);
	if (nRes != 0) // ʧ��
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

// ����id��ȡ������
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

// ��ȡ�������µ����н�ʦ
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

// ��ʦ��ȡ���н�����
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