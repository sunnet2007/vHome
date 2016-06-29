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
	int GetAllExchanges(vector<CExchange>& vecExchange, CResMsg& msg);

	// ��ȡ��������Ϣ
	int GetExchange(CExchange& exchange, CResMsg& msg);

	// ��ȡ�������µ����н�ʦ��Ϣ
	int GetAllTeacherOfExc(vector<CTeacher>& vecTeacher,int nExid, const string& strToken, CResMsg& msg);
};


