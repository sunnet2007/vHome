#include "ActivityLogic.h"
#include "ActivityDao.h"

CActivityLogic::CActivityLogic(void)
{
}


CActivityLogic::~CActivityLogic(void)
{
}

int CActivityLogic::GetAllActivity(int nPage,  int nPageNum, vector<CActivity>& vecActivity, string& strMsg)
{
	CActivityDao activityDao;
	CResMsg msg;
	int nRes = activityDao.GetAllActivity(nPage, nPageNum, vecActivity, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}
// 获取我的活动
int CActivityLogic::GetMyActivity(int nPage,  int nPageNum,vector<CActivity>& vecActivity, const string& strToken, string& strMsg)
{
	CActivityDao activityDao;
	CResMsg msg;
	int nRes = activityDao.GetMyActivity(nPage, nPageNum, vecActivity, strToken, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;

}
// 查看活动详情
int CActivityLogic::GetActivityInfo(const string& strToken, CActivity& activity, string& strMsg)
{
	CActivityDao activityDao;
	CResMsg msg;
	int nRes = activityDao.GetActivityInfo(strToken, activity, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}
// 报名活动
int CActivityLogic::ApplyActivity(const string& strToken, int nId, string& strMsg)
{
	CActivityDao activityDao;
	CResMsg msg;
	int nRes = activityDao.ApplyActivity(strToken, nId, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}