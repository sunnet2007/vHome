#include "ActivityDao.h"
#include "./Common/Network/HttpClient.h"
#include "./Common/Jsoncpp/json/json.h"
//#include "./Common/Jsoncpp/include/json/json.h"
#include "./Common/Utility/StringFuns.h"
#include <tchar.h>
#include "StringUtil.h"
#include "AppConfig.h"

#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

CActivityDao::CActivityDao(void)
{

}


CActivityDao::~CActivityDao(void)
{
}

// 获取所有活动
int CActivityDao::GetAllActivity(int nPage, int nPageNum, vector<CActivity>& vecActivity, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("pagenum", nPageNum);
	client.SetReqArg("page", nPage);

	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::NOS_ACTIVITY);
	if (nRes != 0)
	{
		msg.SetInfo("网络错误");
		msg.SetStatus(-1);
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetInfo("网络错误");
		msg.SetStatus(-1);
		return 1;
	}

	string strBody;
	client.GetResBody(strBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strBody, root))
	{
		string strStatus = root["status"].asString();
		int nStatus = atoi(strStatus.c_str());
		string strInfo = root["info"].asString();
		msg.SetInfo(strInfo);
		msg.SetStatus(nStatus);

		if (nStatus != 1)
		{
			return 1;
		}

		Json::Value data = root["data"];
		int nSize = data.size();
		for (int i = 0; i < nSize; i++)
		{
			string strId = data[i]["activityid"].asString();
			int nId = atoi(strId.c_str());
			string strTitle = data[i]["title"].asString();
			string strContent = data[i]["content"].asString();
			int nNum = data[i]["num"].asInt();
			string strStartTime = data[i]["starttime"].asString();
			string strEndTime = data[i]["endtime"].asString();
			time_t tStart = atol(strStartTime.c_str());
			time_t tEnd = atol(strEndTime.c_str());
			string strImg = data[i]["imgthumb"].asString();
			
			CActivity activity;
			activity.SetId(nId);
			activity.SetTitle(strTitle);
			activity.SetContent(strContent);
			activity.SetNum(nNum);
			activity.SetStart(tStart);
			activity.SetEnd(tEnd);

			int nPos = strImg.find_last_of("/");
			string strFileName(strImg.substr(nPos+1));

			activity.SetImg(strFileName);

			vecActivity.push_back(activity);

			// 下载远程图像到本地
			wstring wstrIcon = s2ws(strImg);
			string strDest = "./UIFile/" + strFileName;
			wstring wstrDest = s2ws(strDest);
			URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
		}
		return 0;
	}

	msg.SetInfo("网络错误");
	msg.SetStatus(-1);
	return 1;
}


// 获取我的活动
int CActivityDao::GetMyActivity(int nPage, int nPageNum, vector<CActivity>& vecActivity, const string& strToken, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("page", nPage);
	client.SetReqArg("token", strToken);

	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::MY_ACTIVITY);
	if (nRes != 0)
	{
		msg.SetInfo("网络错误");
		msg.SetStatus(-1);
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetInfo("网络错误");
		msg.SetStatus(-1);
		return 1;
	}

	string strBody;
	client.GetResBody(strBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strBody, root))
	{
		string strStatus = root["status"].asString();
		int nStatus = atoi(strStatus.c_str());
		string strInfo = root["info"].asString();
		msg.SetInfo(strInfo);
		msg.SetStatus(nStatus);

		if (nStatus != 1)
		{
			return 1;
		}

		Json::Value data = root["data"];
		int nSize = data.size();
		for (int i = 0; i < nSize; i++)
		{
			string strId = data[i]["activityid"].asString();
			int nId = atoi(strId.c_str());
			string strTitle = data[i]["title"].asString();
			string strContent = data[i]["content"].asString();
			int nNum = data[i]["num"].asInt();
			string strStartTime = data[i]["starttime"].asString();
			string strEndTime = data[i]["endtime"].asString();
			time_t tStart = atol(strStartTime.c_str());
			time_t tEnd = atol(strEndTime.c_str());
			string strImg = data[i]["imgthumb"].asString();

			CActivity activity;
			activity.SetId(nId);
			activity.SetTitle(strTitle);
			activity.SetContent(strContent);
			activity.SetNum(nNum);
			activity.SetStart(tStart);
			activity.SetEnd(tEnd);

			int nPos = strImg.find_last_of("/");
			string strFileName(strImg.substr(nPos+1));
			activity.SetImg(strFileName);

			vecActivity.push_back(activity);

			// 下载远程图像到本地
			wstring wstrIcon = s2ws(strImg);
			string strDest = "./UIFile/" + strFileName;
			wstring wstrDest = s2ws(strDest);
			URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
		}
		return 0;
	}

	msg.SetInfo("网络错误");
	msg.SetStatus(-1);
	return 1;
}

// 查看活动详情
int CActivityDao::GetActivityInfo(const string& strToken, CActivity& activity, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("aid", activity.GetId());
	client.SetReqArg("token", strToken);

	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::NOS_ACTIVITY_INFO);
	if (nRes != 0)
	{
		msg.SetInfo("网络错误");
		msg.SetStatus(-1);
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetInfo("网络错误");
		msg.SetStatus(-1);
		return 1;
	}

	string strBody;
	client.GetResBody(strBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strBody, root))
	{
		string strStatus = root["status"].asString();
		int nStatus = atoi(strStatus.c_str());
		string strInfo = root["info"].asString();
		msg.SetInfo(strInfo);
		msg.SetStatus(nStatus);

		if (nStatus != 1)
		{
			msg.SetStatus(nStatus);
			msg.SetInfo(strInfo);
			return 1;
		}

		Json::Value data = root["data"];

		string strTitle = data["title"].asString();
		string strContent = data["content"].asCString();
		int nNum = data["num"].asInt();
		string strStartTime = data["starttime"].asString();
		string strEndTime = data["endtime"].asString();
		time_t tStart = atol(strStartTime.c_str());
		time_t tEnd = atol(strEndTime.c_str());
		string strImg = data["imgthumb"].asString();
		int nMyStatus = data["mystatus"].asInt();

		activity.SetTitle(strTitle);
		activity.SetContent(strContent);
		activity.SetNum(nNum);
		activity.SetStart(tStart);
		activity.SetEnd(tEnd);

		int nPos = strImg.find_last_of("/");
		string strFileName(strImg.substr(nPos+1));
		activity.SetImg(strFileName);
		activity.SetMyStatus(nMyStatus);

		// 下载远程图像到本地
		wstring wstrIcon = s2ws(strImg);
		string strDest = "./UIFile/" + strFileName;
		wstring wstrDest = s2ws(strDest);
		URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);

		return 0;
	}

	msg.SetInfo("网络错误");
	msg.SetStatus(-1);
	return 1;
}

// 报名活动
int CActivityDao::ApplyActivity(const string& strToken, int nId, CResMsg& msg)
{
	CHttpClient client;

	client.SetReqArg("token", strToken);
	client.SetReqArg("activityid", nId);

	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::ENROLL_ACTIVITY);
	if (nRes != 0)
	{
		msg.SetStatus(-1);
		msg.SetInfo("网络错误");
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetStatus(-1);
		msg.SetInfo("网络错误");
		return 1;
	}

	string strBody;
	client.GetResBody(strBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strBody, root))
	{
		string strStatus = root["status"].asString();
		int nStatus = atoi(strStatus.c_str());
		string strInfo = root["info"].asString();
		if (nStatus != 1) // 失败
		{
			msg.SetInfo(strInfo);
			msg.SetStatus(nStatus);
			return 1;
		}
		return 0;
	}
	msg.SetStatus(-1);
	msg.SetInfo("网络错误");
	return 1;
}