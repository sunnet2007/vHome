#include "TeacherDao.h"
#include "./Common/Network/HttpClient.h"
#include "./Common/Jsoncpp/json/json.h"
#include "./Common/Utility/StringFuns.h"
#include <tchar.h>
#include "StringUtil.h"
#include "AppConfig.h"
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

CTeacherDao::CTeacherDao(void)
{
}


CTeacherDao::~CTeacherDao(void)
{
}

// ¹Ø×¢½²Ê¦
int CTeacherDao::AddAttention(int nTid, const string& strToken, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("tid", nTid);
	client.SetReqArg("token", strToken);
	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::ADDATTENTION);
	if (nRes != 0)
	{
		msg.SetInfo("ÍøÂç´íÎó");
		msg.SetStatus(-1);
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetInfo("ÍøÂç´íÎó");
		msg.SetStatus(-1);
		return 1;
	}

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		int nStatus = atoi(strStatus.c_str());
		string strInfo = root["info"].asString();
		msg.SetInfo(strInfo);
		msg.SetStatus(nStatus);
		return 0;
	}

	msg.SetInfo("ÍøÂç´íÎó");
	msg.SetStatus(-1);
	return 1;
}

// »ñÈ¡¹Ø×¢µÄ½²Ê¦
int CTeacherDao::GetCareTeacher(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("token", strToken);
	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::USER_TEACHER_INFO);
	if (nRes != 0)
	{
		msg.SetStatus(-1);
		msg.SetInfo("ÍøÂç´íÎó");
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetStatus(-1);
		msg.SetInfo("ÍøÂç´íÎó");
		return 1;
	}

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStaus = root["status"].asString();
		int nStatus = atoi(strStaus.c_str());
		string strInfo = root["info"].asString();
		
		msg.SetStatus(nStatus);
		msg.SetInfo(strInfo);

		Json::Value data = root["data"];
		Json::Value list = data["list"];
		int nSize = list.size();
		for (int i = 0; i < nSize; i++)
		{
			int nNum = list[i]["num"].asInt();
			string strArticle = list[i]["article"].asString();
			//string strArticleId = list[i]["articleid"].asString();
			//int nArticleId = atoi(strArticleId.c_str());
			string strUid = list[i]["userid"].asString();
			int nUid = atoi(strUid.c_str());
			string strName = list[i]["name"].asString();
			string strImg = list[i]["imgthumb"].asString();
			string strStar = list[i]["star"].asString();
			int nStar = atoi(strStar.c_str());
			string strSchool = list[i]["school"].asString();

			Json::Value desc = list[i]["description"].size();
			string strDesc = list[i]["description"].asString();

			CTeacher teacher;
			teacher.SetNum(nNum);
			teacher.SetArticle(strArticle);
			//teacher.SetArticleId(nArticleId);
			teacher.SetUid(nUid);
			teacher.SetName(strName);
			teacher.SetImgThumb(strImg);
			teacher.SetStar(nStar);
			teacher.SetSchool(strSchool);
			teacher.SetDesc(strDesc);

			vecTeacher.push_back(teacher);

			wstring wstrIcon = s2ws(teacher.GetImgThumb());
			string strDest = "./UIFile/teacherimage_" + strUid + ".png";
			wstring wstrDest = s2ws(strDest);
			URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
		}
		return 0;
	}
	msg.SetStatus(-1);
	msg.SetInfo("ÍøÂç´íÎó");
	return 1;
}

// È¡Ïû¹Ø×¢
int CTeacherDao::DelAttention(int nTid, const string& strToken, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("tid", nTid);
	client.SetReqArg("token", strToken);
	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::DELATTENTION);
	if (nRes != 0)
	{
		msg.SetStatus(-1);
		msg.SetInfo("ÍøÂç´íÎó");
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetStatus(-1);
		msg.SetInfo("ÍøÂç´íÎó");
		return 1;
	}

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		int nStatus = atoi(strStatus.c_str());
		string strInfo = root["info"].asString();
		msg.SetStatus(nStatus);
		msg.SetInfo(strInfo);
		return 0;
	}
	msg.SetStatus(-1);
	msg.SetInfo("ÍøÂç´íÎó");
	return 1;
}

// »ñÈ¡ÃûÊ¦ÁÐ±í(ÅÅÃûÇ°Èý)
int CTeacherDao::GetTopTeacher(const string& strToken,vector<CTeacher>& vecTeacher, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("token", strToken);
	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::NOS_TOPTEACHER);
	if (nRes != 0)
	{
		msg.SetStatus(-1);
		msg.SetInfo("ÍøÂç´íÎó");
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetStatus(-1);
		msg.SetInfo("ÍøÂç´íÎó");
		return 1;
	}

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		string strInfo = root["info"].asString();
		Json::Value data = root["data"];
		int nSize = data.size();
		for (int i = 0; i < nSize; i++)
		{
			string strName = data[i]["username"].asString();
			string strDesc = data[i]["description"].asString();
			string strUid = data[i]["tid"].asString();
			int nUid = atoi(strUid.c_str());
			string strStar = data[i]["star"].asString();
			int nStar = atoi(strStar.c_str());
			string strExName = data[i]["ex_name"].asString();
			string strNum = data[i]["num"].asString();
			int nNum = atoi(strNum.c_str());
			string strImg = data[i]["imgthumb"].asString();
			int nIfAttrntion = data[i]["ifattention"].asInt();

			CTeacher teacher;
			teacher.SetName(strName);
			teacher.SetDesc(strDesc);
			teacher.SetUid(nUid);
			teacher.SetStar(nStar);
			teacher.SetExName(strExName);
			teacher.SetNum(nNum);
			teacher.SetImgThumb(strImg);
			teacher.SetIfAttention(nIfAttrntion);
			vecTeacher.push_back(teacher);


			wstring wstrIcon = s2ws(teacher.GetImgThumb());
			string strDest = "./UIFile/teacherimage_" + strUid + ".png";
			wstring wstrDest = s2ws(strDest);
			URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
		}
		return 0;
	}

	msg.SetStatus(-1);
	msg.SetInfo("ÍøÂç´íÎó");
	return 1;
}

// »ñÈ¡ÃûÊ¦ÁÐ±í(Ç°Èý³ýÍâ)
int CTeacherDao::GetStarTeacher(int nPage, const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("page", nPage);
	client.SetReqArg("token", strToken);

	client.SetReqCmd("POST");
	int nRes = client.AccessUrl(CAppConfig::NOS_C_STARTEACHER);
	if (nRes != 0)
	{
		msg.SetStatus(-1);
		msg.SetInfo("ÍøÂç´íÎó");
		return 1;
	}

	int nStatusCode = client.GetResStatusCode();
	if (nStatusCode != 200)
	{
		msg.SetStatus(-1);
		msg.SetInfo("ÍøÂç´íÎó");
		return 1;
	}

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		string strInfo = root["info"].asString();
		Json::Value data = root["data"];
		int nSize = data.size();
		for (int i = 0; i < nSize; i++)
		{
			string strName = data[i]["username"].asString();
			string strDesc = data[i]["description"].asString();
			string strUid = data[i]["userid"].asString();
			int nUid = atoi(strUid.c_str());
			string strStar = data[i]["star"].asString();
			int nStar = atoi(strStar.c_str());
			string strExName = data[i]["ex_name"].asString();
			string strNum = data[i]["num"].asString();
			int nNum = atoi(strNum.c_str());
			string strImg = data[i]["imgthumb"].asString();
			int nIfAttrntion = data[i]["ifattention"].asInt();

			CTeacher teacher;
			teacher.SetName(strName);
			teacher.SetDesc(strDesc);
			teacher.SetUid(nUid);
			teacher.SetStar(nStar);
			teacher.SetExName(strExName);
			teacher.SetNum(nNum);
			teacher.SetImgThumb(strImg);
			teacher.SetIfAttention(nIfAttrntion);
			vecTeacher.push_back(teacher);


			wstring wstrIcon = s2ws(teacher.GetImgThumb());
			string strDest = "./UIFile/teacherimage_" + strUid + ".png";
			wstring wstrDest = s2ws(strDest);
			URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
		}
		return 0;
	}

	msg.SetStatus(-1);
	msg.SetInfo("ÍøÂç´íÎó");
	return 1;
}