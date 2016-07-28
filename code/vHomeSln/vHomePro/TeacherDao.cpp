#include "TeacherDao.h"
#include "./Common/Network/HttpClient.h"
#include "./Common/Jsoncpp/json/json.h"
//#include "./Common/Jsoncpp/include/json/json.h"
#include "./Common/Utility/StringFuns.h"
#include "./Common/Utility/base_string.h"
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
		if (nStatus != 1)
		{
			return 1;
		}
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
		if (nStatus != 1)
		{
			return 1;
		}

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
			
			teacher.SetStar(nStar);
			teacher.SetSchool(strSchool);
			teacher.SetDesc(strDesc);
			teacher.SetIfAttention(1);


			int nPos = strImg.find_last_of('/');
			string srtFileName(strImg.substr(nPos+1));
			teacher.SetImgThumb(srtFileName);

			wstring wstrIcon = s2ws(strImg);
			string strDest = "./UIFile/" + srtFileName;
			wstring wstrDest = s2ws(strDest);
			URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);

			vecTeacher.push_back(teacher);
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
			
			teacher.SetIfAttention(nIfAttrntion);


			int nPos = strImg.find_last_of('/');
			string srtFileName(strImg.substr(nPos+1));
			teacher.SetImgThumb(srtFileName);

			wstring wstrIcon = s2ws(strImg);
			string strDest = "./UIFile/" + srtFileName;
			wstring wstrDest = s2ws(strDest);
			URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);

			vecTeacher.push_back(teacher);
		}
		return 0;
	}

	msg.SetStatus(-1);
	msg.SetInfo("ÍøÂç´íÎó");
	return 1;
}

// »ñÈ¡ÃûÊ¦ÁÐ±í(Ç°Èý³ýÍâ)
int CTeacherDao::GetStarTeacher(int nPage, int nPageNum, const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("pagenum", nPageNum);
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
			wstring wstr = s2ws(data[i]["username"].asString());
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
			teacher.SetIfAttention(nIfAttrntion);
			int nPos = strImg.find_last_of('/');
			string srtFileName(strImg.substr(nPos+1));
			teacher.SetImgThumb(srtFileName);

			wstring wstrIcon = s2ws(strImg);
			string strDest = "./UIFile/" + srtFileName;
			wstring wstrDest = s2ws(strDest);
			URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
			
			vecTeacher.push_back(teacher);
		}
		return 0;
	}

	msg.SetStatus(-1);
	msg.SetInfo("ÍøÂç´íÎó");
	return 1;
}

// »ñÈ¡½²Ê¦µÄËùÓÐÎÄÕÂÐÅÏ¢(½²Ê¦×Ô¼º)
int CTeacherDao::GetAllArtOfTeacher(int nPage, int nPageNum, const string& strToken, vector<CArticle>& vecArt, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("token", strToken);
	client.SetReqArg("pagenum",nPageNum);
	client.SetReqArg("page", nPage);

	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::NOS_TEACHER_ALLARTICLE);
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
		if (nStatus != 1)
		{
			msg.SetStatus(nStatus);
			msg.SetInfo(strInfo);
			return 1;
		}
		Json::Value data = root["data"];
		int nSize = data.size();
		for (int i = 0; i < nSize; i++)
		{
			string strId = data[i]["id"].asString();
			int nId = atoi(strId.c_str());

			string strTitle = data[i]["title"].asString();
			string strAbout = data[i]["about"].asString();
			string strHits = data[i]["hits"].asString();
			int nHits = atoi(strHits.c_str());
			string strContent = data[i]["content"].asString();
			string strImgThumb = data[i]["imgthumb"].asString();
			string strTime = data[i]["time"].asString();
			time_t tTime = atol(strTime.c_str());
			string strStatus = data[i]["status"].asString();
			int nArtStatus = atoi(strStatus.c_str());
			string strCtgyid = data[i]["ctgyid"].asString();
			int nCtgyId = atoi(strCtgyid.c_str());
			string strExid = data[i]["exid"].asString();
			int nExid = atoi(strExid.c_str());
			string strExName = data[i]["ex_name"].asString();
			string strCtgy = data[i]["ctgy"].asString();
			string strReason = data[i]["reason"].asString();

			CArticle art;
			art.SetId(nId);
			art.SetTitle(strTitle);
			art.SetAbout(strAbout);
			art.SetHits(nHits);
			art.SetContent(strContent);
			
			art.SetCTime(tTime);
			art.SetStatus(nArtStatus);
			art.SetCtgyId(nCtgyId);
			art.SetCtgy(strCtgy);
			art.SetReason(strReason.c_str());
			
			int nPos = strImgThumb.find_last_of('/');
			string srtFileName(strImgThumb.substr(nPos+1));
			art.SetImgThumb(srtFileName);

			if (strImgThumb != "")
			{
				wstring wstrIcon = s2ws(strImgThumb);
				string strDest = "./UIFile/" + srtFileName;
				wstring wstrDest = s2ws(strDest);
				URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
			}
			
			vecArt.push_back(art);
		}
		return 0;
	}
	msg.SetInfo("ÍøÂç´íÎó");
	msg.SetStatus(-1);
	return 1;
}

// »ñÈ¡½²Ê¦µÄËùÓÐÎÄÕÂÐÅÏ¢(ÓÃ»§)
int CTeacherDao::GetTeacherArts(int nPage, int nPageNum, int tId, const string& strToken, vector<CArticle>& vecArt, vector<CCategory>& vecCtgy, CResMsg& msg, int nCtgyId/* = 0*/)
{
	CHttpClient client;
	client.SetReqArg("teacher_id", tId);
	client.SetReqArg("pagenum",nPageNum);
	client.SetReqArg("page", nPage);
	client.SetReqArg("token", strToken);
	client.SetReqArg("ctgy", nCtgyId);


	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::NOS_C_TEACHERS_INFO);
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
		if (nStatus != 1)
		{
			msg.SetStatus(nStatus);
			msg.SetInfo(strInfo);
			return 1;
		}
		Json::Value data = root["data"];
		Json::Value cat = data["cat"];
		int nSize = cat.size();
		for (int i = 0; i < nSize; i++)
		{
			string strId = cat[i]["id"].asString();
			int nId = atoi(strId.c_str());
			string strCtgyName = cat[i]["ctgname"].asString();
			string strImg = cat[i]["imgthumb"].asString();
			CCategory ctgy;
			ctgy.SetId(nId);
			ctgy.SetName(strCtgyName);
			
			int nPos = strImg.find_last_of('/');
			string srtFileName(strImg.substr(nPos+1));

			ctgy.SetImg(srtFileName);

			
			if (strImg != "")
			{
				wstring wstrIcon = s2ws(strImg);
				string strDest = "./UIFile/" + srtFileName;
				wstring wstrDest = s2ws(strDest);
				URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
			}
			
			vecCtgy.push_back(ctgy);
		}
		Json::Value lstArt = data["list"];
		nSize = lstArt.size();
		for (int i = 0; i < nSize; i++)
		{
			string strId = lstArt[i]["id"].asString();
			int nId = atoi(strId.c_str());
			string strTitle = lstArt[i]["title"].asString();
			string strAbout = lstArt[i]["about"].asString();
			string strImg = lstArt[i]["imgthumb"].asString();

			CArticle art;
			art.SetId(nId);
			art.SetTitle(strTitle);
			art.SetAbout(strAbout);

			int nPos = strImg.find_last_of('/');
			string srtFileName(strImg.substr(nPos+1));
			art.SetImgThumb(srtFileName);

			

			if (strImg != "")
			{
				wstring wstrIcon = s2ws(strImg);
				string strDest = "./UIFile/" + srtFileName;
				wstring wstrDest = s2ws(strDest);
				URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
			}
			
			vecArt.push_back(art);
		}

		return 0;
	}
	msg.SetInfo("ÍøÂç´íÎó");
	msg.SetStatus(-1);
	return 1;
}


// »ñÈ¡·¢²¼ÎÄÕÂÇ°µÄÏà¹ØÐÅÏ¢
int CTeacherDao::GetArtCat(const string& strToken, string& strExchange, vector<CCategory>& vecCtgy, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("token", strToken);
	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::NOS_TEACHER_PRERELEASE);
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
		if (nStatus != 1)
		{
			msg.SetStatus(nStatus);
			msg.SetInfo(strInfo);
			return 1;
		}

		Json::Value data = root["data"];
		strExchange = data["exchange"].asString();
		Json::Value ex_name = data["ex_name"];
		int nSize = ex_name.size();
		for (int i = 0; i < nSize; i++)
		{
			string strExClassId = ex_name[i]["ex_classid"].asString();
			int nExClassId = atoi(strExClassId.c_str());
			string strExClass =  ex_name[i]["ex_class"].asString();

			CCategory ctgy;
			ctgy.SetId(nExClassId);
			ctgy.SetName(strExClass);

			vecCtgy.push_back(ctgy);
		}
		return 0;
	}

	msg.SetInfo("ÍøÂç´íÎó");
	msg.SetStatus(-1);
	return 1;
}

// ·¢²¼ÎÄÕÂ
int CTeacherDao::ReleaseArt(const string& strToken, const CArticle& art, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("token", strToken);

	client.SetReqArg("title", art.GetTitle());
	client.SetReqArg("content", art.GetContent());
	client.SetReqArg("imgthumb", art.GetImgThumb());
	client.SetReqArg("ctgy", art.GetCtgyId());
	client.SetReqArg("exid", art.GetExid());


	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::NOS_TEACHER_RELEASE);
	if (nRes != 0)
	{
		msg.SetInfo("ÍøÂç´íÎó");
		msg.SetStatus(-1);
		return 1;
	}

	//int nStatusCode = client.GetResStatusCode();
	//if (nStatusCode != 200)
	//{
	//	msg.SetInfo("ÍøÂç´íÎó");
	//	msg.SetStatus(-1);
	//	return 1;
	//}

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		int nStatus = atoi(strStatus.c_str());
		string strInfo = root["info"].asString();
		if (nStatus != 1)
		{
			msg.SetStatus(nStatus);
			msg.SetInfo(strInfo);
			return 1;
		}
		return 0;
	}

	msg.SetInfo("ÍøÂç´íÎó");
	msg.SetStatus(-1);
	return 1;
}

// ÏÔÊ¾½²Ê¦ÐÅÏ¢
int CTeacherDao::GetTeacherInfo(CTeacher& teacher,  vector<CSpecialty>& vecSpecialty, const string strToken, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("token", strToken);
	client.SetReqCmd("POST");
	
	int nRes = client.AccessUrl(CAppConfig::NOS_TEACHER_VIEW);
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
	Json::Value  root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		string strInfo = root["info"].asString();
		int nStatus = atoi(strStatus.c_str());
		msg.SetStatus(nStatus);
		msg.SetInfo(strInfo);
		if (nStatus != 1)
			return 1;
		Json::Value data = root["data"];
		string strNicName = data["username"].asString();
		string strName = data["name"].asString();
		string strImg = data["imgthumb"].asString();
		string strEmail = data["email"].asString();
		string strQq = data["qq"].asString();
		string strSchool = data["school"].asString();
		string strSex = data["sex"].asString();
		string strDesc = data["description"].asString();
		string strExid = data["exid"].asString();
		int nExid = atoi(strExid.c_str());

		Json::Value goodat = data["goodat"];
		vector<CSpecialty> vecGoodat;
		int nSize = goodat.size();
		for (int i = 0; i < nSize; i++)
		{
			CSpecialty special;
			string strId = goodat[i]["id"].asString();
			int nId = atoi(strId.c_str());
			special.SetId(nId);
			string strGName = goodat[i]["splname"].asString();
			special.SetName(strGName);
			vecGoodat.push_back(special);
		}
		Json::Value specialty = data["specialty"];
		nSize = specialty.size();
		for (int i = 0; i < nSize; i++)
		{
			CSpecialty special;
			string strId = specialty[i]["id"].asString();
			int nId = atoi(strId.c_str());
			special.SetId(nId);
			string strGName = specialty[i]["splname"].asString();
			special.SetName(strGName);
			vecSpecialty.push_back(special);
		}
		teacher.SetNicName(strNicName);
		teacher.SetName(strName);
		
		teacher.SetEmail(strEmail);
		teacher.SetQq(strQq);
		teacher.SetSchool(strSchool);
		teacher.SetSex(strSex);
		teacher.SetDesc(strDesc);
		teacher.SetExid(nExid);
		teacher.SetGoodat(vecGoodat);
		int nPos = strImg.find_last_of('/');
		string srtFileName(strImg.substr(nPos+1));
		teacher.SetImgThumb(srtFileName);
		if (strImg != "")
		{
			wstring wstrIcon = s2ws(strImg);
			string strDest = "./UIFile/" + srtFileName;
			wstring wstrDest = s2ws(strDest);
			URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
		}

		return 0;
	}
	msg.SetInfo("ÍøÂç´íÎó");
	msg.SetStatus(-1);
	return 1;
}

// ÐÞ¸Ä½²Ê¦ÐÅÏ¢
int CTeacherDao::ModifyTeacherInfo(const CTeacher& teacher,const string strToken, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("token", strToken);
	client.SetReqArg("username", teacher.GetNicName());
	client.SetReqArg("name", teacher.GetName());
	client.SetReqArg("imgthumb", teacher.GetImgThumb());
	string strSex = teacher.GetSex();
	client.SetReqArg("sex", strSex);
	client.SetReqArg("email", teacher.GetEmail());
	client.SetReqArg("qq", teacher.GetQq());
	client.SetReqArg("school", teacher.GetSchool());
	client.SetReqArg("description", teacher.GetDesc());
	client.SetReqArg("goodat", teacher.GetGoodIds());

	client.SetReqCmd("POST");

	int nRes = client.AccessUrl(CAppConfig::NOS_TEACHER_MODI);
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
		string strInfo = root["info"].asString();
		int nStatus = atoi(strStatus.c_str());
		msg.SetStatus(nStatus);
		msg.SetInfo(strInfo);
		if (nStatus != 1)
			return 1;
		return 0;
	}

	msg.SetInfo("ÍøÂç´íÎó");
	msg.SetStatus(-1);
	return 1;
}

// ¸ü»»ÎÄÕÂÍ¼Æ¬
int CTeacherDao::UploadArtImg(const string& strToken, int nArtId, const string& strImg, string& strImgUrl, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("token", strToken);
	client.SetReqArg("image", strImg);
	client.SetReqArg("articleid", nArtId);
	client.SetReqCmd("POST");
	int nRes = client.AccessUrl(CAppConfig::NOS_USERIMAGE);
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
		int nStatus = atoi(strStatus.c_str());
		msg.SetStatus(nStatus);
		msg.SetInfo(strInfo);
		if (nStatus != 1)
			return 1;

		Json::Value data = root["data"];
		strImgUrl  = data["imgthumb"].asString();

		//int nPos = strImg.find_last_of('/');
		//strImgUrl = strImg.substr(nPos+1);

		//wstring wstrIcon = s2ws(strImg);
		//string strDest = "./UIFile/" + strImgUrl;
		//wstring wstrDest = s2ws(strDest);
		//URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);

		return 0;
	}

	msg.SetStatus(-1);
	msg.SetInfo("ÍøÂç´íÎó");
	return 1;

}

// ¸ü»»½²Ê¦Í·Ïñ
int CTeacherDao::UploadTeacherImg(const string& strToken, const string& strImg, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("token", strToken);
	client.SetReqArg("image", strImg);
	client.SetReqCmd("POST");
	int nRes = client.AccessUrl(CAppConfig::NOS_USERIMAGE);
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
		int nStatus = atoi(strStatus.c_str());
		msg.SetStatus(nStatus);
		msg.SetInfo(strInfo);
		if (nStatus == 1)
			return 0;
		return 1;
	}

	msg.SetStatus(-1);
	msg.SetInfo("ÍøÂç´íÎó");
	return 1;
}