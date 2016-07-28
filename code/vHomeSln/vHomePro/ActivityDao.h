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
	int GetAllActivity(int nPage,  int nPageNum, vector<CActivity>& vecActivity, CResMsg& msg);  // 获取所有活动
	int GetMyActivity(int nPage, int nPageNum, vector<CActivity>& vecActivity, const string& strToken, CResMsg& msg);	// 获取我的活动
	int GetActivityInfo(const string& strToken, CActivity& activity, CResMsg& msg);	// 查看活动详情
	int ApplyActivity(const string& strToken, int nId, CResMsg& msg);	// 报名活动
};

