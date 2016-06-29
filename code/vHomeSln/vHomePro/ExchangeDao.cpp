#include "ExchangeDao.h"
#include "./Common/Network/HttpClient.h"
#include "./Common/Jsoncpp/json/json.h"
#include "./Common/Utility/StringFuns.h"
#include <tchar.h>
#include "StringUtil.h"
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")


CExchangeDao::CExchangeDao(void)
{
	//Server server;
	//CAppConfig::GetServer(server);
	//m_strURL = server.strProtocol + "://" + server.strHost + ":" + server.strPort;
}


CExchangeDao::~CExchangeDao(void)
{
}

// 获取所有交易所信息
int CExchangeDao::GetAllExchanges(vector<CExchange>& vecExchange, vector<CBanner>& vecBanner,CResMsg& msg)
{
	CHttpClient client;
	client.SetReqCmd("POST");

	//string strUrl = m_strURL + "/index.php/app/nos_fenxi";
	int nRes = client.AccessUrl(CAppConfig::NOS_FENXI);
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

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;

	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		int nStatus = atoi(strStatus.c_str());
		msg.SetStatus(nStatus);
		msg.SetInfo(root["info"].asString());

		Json::Value data = root["data"];
		Json::Value lstBanner = data["banner"];
		Json::Value lstExchange = data["exchange"];
		
		// 广告列表
		int nBannerSize = lstBanner.size();
		for (int i = 0; i < nBannerSize; i++)
		{
			string strLink = lstBanner[i]["link"].asString();
			string strImgThumb = lstBanner[i]["imgthumb"].asString();

			CBanner banner;
			banner.SetLink(strLink);
			banner.SetImgThumb(strImgThumb);
			vecBanner.push_back(banner);
		}

		// 交易所列表
		int nExSize = lstExchange.size();
		for (int i = 0; i < nExSize; i++)
		{
			string strId = lstExchange[i]["id"].asString();
			int nId = atoi(strId.c_str());

			string strName = lstExchange[i]["ex_name"].asString();
			string strExInfo = lstExchange[i]["exc_information"].asString();
			string strImage = lstExchange[i]["excimg"].asString();
			int nNum = lstExchange[i]["num"].asInt();

			int nCount =  lstExchange[i]["rz"].size();
			vector<string> vecAuth;
			//string strAuth = lstExchange[i]["rz"][0].asString();
			for (int j = 0; j < nCount; j++)
			{
				string strAuth = lstExchange[i]["rz"][j].asString();
				vecAuth.push_back(strAuth);
			}

			CExchange ex;
			ex.SetId(nId);
			ex.SetName(strName);
			ex.SetImage(strImage);
			ex.SetAuth(vecAuth);
			ex.SetInfo(strExInfo);
			ex.SetNum(nNum);

			vecExchange.push_back(ex);

			// 下载远程图像到本地
			wstring wstrIcon = s2ws(ex.GetImage());
			string strDest = "./UIFile/eximage_" + strId + ".png";
			wstring wstrDest = s2ws(strDest);
			URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
		}
		return 0;

	}
	msg.SetStatus(-1);
	msg.SetInfo("网络错误");
	return  1;
}

// 根据交易所ID，获取交易所所有讲师信息
int CExchangeDao::GetTeacherListByExid(vector<CTeacher>& vecTeacher,int nExid, const string& strToken, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("exid", nExid);
	client.SetReqArg("token", strToken);
	client.SetReqCmd("POST");

	//string strUrl = m_strURL + "/index.php/app/nos_teachers";
	int nRes = client.AccessUrl(CAppConfig::NOS_TEACHERS);
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

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		int nStatus = atoi(strStatus.c_str());
		msg.SetStatus(nStatus);
		msg.SetInfo(root["info"].asString());

		Json::Value data;
		data = root["data"];
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
			int nNum = data[i]["num"].asInt();
			string strImgThumb = data[i]["imgthumb"].asString();
			int nIfattention = data[i]["ifattention"].asInt();

			CTeacher teacher;
			teacher.SetName(strName);
			teacher.SetDesc(strDesc);
			teacher.SetUid(nUid);
			teacher.SetStar(nStar);
			teacher.SetExName(strExName);
			teacher.SetNum(nNum);
			teacher.SetImgThumb(strImgThumb);
			teacher.SetIfAttention(nIfattention);

			vecTeacher.push_back(teacher);

			// 下载远程图像到本地
			wstring wstrIcon = s2ws(teacher.GetImgThumb());
			string strDest = "./UIFile/teacherimage_" + strUid + ".png";
			wstring wstrDest = s2ws(strDest);
			URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);
		}
		return 0;
	}
	msg.SetStatus(-1);
	msg.SetInfo("网络错误");
	return 1;
}

// 根据交易所ID获取交易所信息
int CExchangeDao::GetExchangeInfoById(CExchange& exchange, CResMsg& msg)
{
	CHttpClient client;
	client.SetReqArg("excid", exchange.GetId());
	client.SetReqCmd("POST");
	
	//http://115.28.134.125/index.php/app/nos_teacher_exchangeinfo
	//string strUrl = m_strURL + "/index.php/app/nos_teacher_exchangeinfo";
	int nRes = client.AccessUrl(CAppConfig::NOS_TEACHER_EXCHANGEINFO);
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

	string strResBody;
	client.GetResBody(strResBody);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strResBody, root))
	{
		string strStatus = root["status"].asString();
		int nStatus = atoi(strStatus.c_str());
		msg.SetStatus(nStatus);
		msg.SetInfo(root["info"].asString());

		Json::Value data  = root["data"];
		string strId = data["id"].asString();
		int nId = atoi(strId.c_str());
		string strName = data["ex_name"].asString();
		string strExInfo = data["exc_information"].asString();
		string strImg = data["excimg"].asString();

		Json::Value auth = data["exchcft"];
		int nSize = auth.size();
		vector<string> vecAuth;
		for (int i = 0; i < nSize; i++)
		{
			string strAuth = auth[i].asString();
			vecAuth.push_back(strAuth);
		}

		exchange.SetName(strName);
		exchange.SetInfo(strExInfo);
		exchange.SetImage(strImg);
		exchange.SetAuth(vecAuth);

		// 下载远程图像到本地
		wstring wstrIcon = s2ws(exchange.GetImage());
		string strDest = "./UIFile/eximage_" + strId + ".png";
		wstring wstrDest = s2ws(strDest);
		URLDownloadToFile(0, wstrIcon.c_str(), wstrDest.c_str(),0,0);

		return 0;
	}
	msg.SetStatus(-1);
	msg.SetInfo("网络错误");
	return 1;

}