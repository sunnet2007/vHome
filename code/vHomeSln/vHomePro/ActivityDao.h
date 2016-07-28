#pragma once
#include <vector>
#include "Activity.h"
#include "ResMsg.h"
using namespace std;

class CActivityDao
{
public:
	CActivityDao(void);
	~CActivityDao(void);

public:
	int GetAllActivity(int nPage,  int nPageNum, vector<CActivity>& vecActivity, CResMsg& msg);  // ��ȡ���л
	int GetMyActivity(int nPage, int nPageNum, vector<CActivity>& vecActivity, const string& strToken, CResMsg& msg);	// ��ȡ�ҵĻ
	int GetActivityInfo(const string& strToken, CActivity& activity, CResMsg& msg);	// �鿴�����
	int ApplyActivity(const string& strToken, int nId, CResMsg& msg);	// �����
};

