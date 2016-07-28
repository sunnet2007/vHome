#pragma once
#include "Activity.h"
#include <vector>
#include <string>
using namespace std;

class CActivityLogic
{
public:
	CActivityLogic(void);
	~CActivityLogic(void);

public:
	int GetAllActivity(int nPage,  int nPageNum, vector<CActivity>& vecActivity, string& strMsg);
	int GetMyActivity(int nPage, int nPageNum, vector<CActivity>& vecActivity, const string& strToken, string& strMsg);	// 获取我的活动
	int GetActivityInfo(const string& strToken, CActivity& activity, string& strMsg);	// 查看活动详情
	int ApplyActivity(const string& strToken, int nId, string& strMsg);
};

