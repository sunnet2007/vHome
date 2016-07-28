#pragma once
#include <string>
#include <vector>
#include "ResMsg.h"
#include "Teacher.h"
#include "Article.h"
using namespace std;


// 讲师业务逻辑类
class CTeacherLogic
{
public:
	CTeacherLogic(void);
	~CTeacherLogic(void);

public:
	// 关注讲师
	int AddAttention(int nTid, const string& strToken, string& strMsg);

	// 取消关注
	int DelAttention(int nTid, const string& strToken, string& strMsg);

	// 获取关注的讲师
	int GetCareTeacher(const string& strToken, vector<CTeacher>& vecTeacher, string& strMsg);

	// 获取推荐讲师
	int GetTeachers(int nPage, int nPageNum, const string& strToken, vector<CTeacher>& vecTeacher, string& strMsg );

	// 获取名师列表(排名前三)
	int GetTopTeacher(const string& strToken, vector<CTeacher>& vecTeacher, string& strMsg);

	// 获取名师列表(前三除外)
	int GetStarTeacher(int nPage,  int nPageNum, const string& strToken, vector<CTeacher>& vecTeacher, string& strMsg); 

	// 讲师获取自己发布的文章
	int GetAllArtOfTeacher(int nPage, int nPageNum, const string& strToken, vector<CArticle>& vecArt, string& strMsg);

	// 获取发布文章前的相关信息
	int GetArtCat(const string& strToken, string& strExchange, vector<CCategory>& vecCtgy, string& strMsg);

	// 发布文章
	int ReleaseArt(const string& strToken, CArticle& art, string strMsg);

	// 显示讲师信息
	int GetTeacherInfo(CTeacher& teacher, vector<CSpecialty>& vecSpecialty,const string strToken, string strMsg);

	// 修改讲师信息
	int ModifyTeacherInfo(const CTeacher& teacher,const string strToken, string strMsg);
	// 获取讲师发布的文章
	int GetTeacherArts(int nPage, int nPageNum , int tId,const string& strToken, vector<CArticle>& vecArt,vector<CCategory>& vecCtgy, string& strMsg, int nCtgyId = 0);

	int UploadTeacherImage(const string& strToken, const string& strImg, string& strMsg);
};

