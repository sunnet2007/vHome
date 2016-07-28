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
	// ��ȡ���н�����
	int GetAllExchanges(const string& strToken, vector<CExchange>& vecExchange, string& strMsg);

	// ��ȡ��������Ϣ
	int GetExchange(CExchange& exchange, string& strMsg);

	// ��ȡ�������µ����н�ʦ��Ϣ
	int GetAllTeacherOfExc(vector<CTeacher>& vecTeacher,int nExid, const string& strToken, string& strMsg);

	// ��ʦ��ȡ���н�����
	int GetExchanges(vector<CExchange>& vecExchange, string& strMsg);
};


