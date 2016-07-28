#pragma once
#include <vector>
#include <string>
#include "Exchange.h"
#include "Banner.h"
#include "Teacher.h"
#include "AppConfig.h"
#include "ResMsg.h"

using namespace std;
// ��������Ϣ������
class CExchangeDao
{
public:
	CExchangeDao(void);
	~CExchangeDao(void);


public:
	// ��ȡ��������Ϣ�б�
	int GetAllExchanges(const string& strToken, vector<CExchange>& vecExchange, vector<CBanner>& vecBanner, CResMsg& msg);
	// ��ȡ��������ʦ�б�
	int GetTeacherListByExid(vector<CTeacher>& vecTeacher,int nExid, const string& strToken, CResMsg& msg);
	// ����ID��ȡ��������Ϣ
	int GetExchangeInfoById(CExchange& exchange, CResMsg& msg);

	// ��ʦ��ҳ��������Ϣ
	int GetExchanges(vector<CExchange>& vecExchange, CResMsg& msg);


private:
	string m_strURL;
};

