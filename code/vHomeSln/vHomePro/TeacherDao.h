#pragma once
#include <string>
#include <vector>
#include "ResMsg.h"
#include "Teacher.h"
#include "Article.h"
using namespace std;

// 讲师信息访问类
class CTeacherDao
{
public:
	CTeacherDao(void);
	~CTeacherDao(void);

public:
	// 关注讲师
	int AddAttention(int nTid, const string& strToken, CResMsg& msg);
	// 取消关注
	int DelAttention(int nTid, const string& strToken, CResMsg& msg);

	// 获取关注的讲师
	int GetCareTeacher(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg);

	// 获取名师列表(排名前三)
	int GetTopTeacher(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg);

	// 获取名师列表(前三除外)
	int GetStarTeacher(int nPage, int nPageNum, const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg); 

	// 获取讲师的所有文章信息
	int GetAllArtOfTeacher(int nPage, int nPageNum, const string& strToken, vector<CArticle>& vecArt, CResMsg& msg);

	// 获取发布文章前的相关信息
	int GetArtCat(const string& strToken, string& strExchange, vector<CCategory>& vecCtgy, CResMsg& msg);

	// 发布文章
	int ReleaseArt(const string& strToken, const CArticle& art, CResMsg& msg);

	// 显示讲师信息
	int GetTeacherInfo(CTeacher& teacher, vector<CSpecialty>& vecSpecialty,const string strToken, CResMsg& msg);

	// 修改讲师信息
	int ModifyTeacherInfo(const CTeacher& teacher,const string strToken, CResMsg& msg);
	// 获取讲师文章
	int GetTeacherArts(int nPage, int nPageNum , int tId,const string& strToken, vector<CArticle>& vecArt,vector<CCategory>& vecCtgy, CResMsg& msg ,int nCtgyId = 0);

	// 更换文章图片
	int UploadArtImg(const string& strToken, int nArtId, const string& strImg, string& strImgUrl, CResMsg& msg);

	// 更换讲师头像
	int UploadTeacherImg(const string& strToken, const string& strImg, CResMsg& msg);
};

