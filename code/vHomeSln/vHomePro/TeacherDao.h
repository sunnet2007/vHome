#pragma once
#include <string>
#include <vector>
#include "ResMsg.h"
#include "Teacher.h"
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
	int GetStarTeacher(int nPage, const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg); 

};

