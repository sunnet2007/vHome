#include "TeacherLogic.h"
#include "TeacherDao.h"

CTeacherLogic::CTeacherLogic(void)
{
}


CTeacherLogic::~CTeacherLogic(void)
{
}

// 关注讲师
int CTeacherLogic::AddAttention(int nTid, const string& strToken, CResMsg& msg)
{
	CTeacherDao teacherDao;
	int nRes = teacherDao.AddAttention(nTid,strToken,msg);
	return nRes;
}

// 取消关注
int CTeacherLogic::DelAttention(int nTid, const string& strToken, CResMsg& msg)
{
	CTeacherDao teacherDao;
	int nRes = teacherDao.DelAttention(nTid, strToken, msg);
	return nRes;
}

// 获取关注的讲师
int CTeacherLogic::GetCareTeacher(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg)
{
	CTeacherDao teacherDao;
	int nRes = teacherDao.GetCareTeacher(strToken, vecTeacher, msg);
	return nRes;
}

// 获取名师列表(排名前三)
int CTeacherLogic::GetTopTeacher(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg)
{
	CTeacherDao teacherDao;
	int nRes = teacherDao.GetTopTeacher(strToken, vecTeacher, msg);
	return nRes;
}

// 获取名师列表(前三除外)
int CTeacherLogic::GetStarTeacher(int nPage, const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg)
{
	CTeacherDao teacherDao;
	int nRes = teacherDao.GetStarTeacher(nPage, strToken, vecTeacher, msg);
	return nRes;
}

// 获取推荐讲师
int CTeacherLogic::GetTeachers(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg )
{
	GetTopTeacher(strToken, vecTeacher, msg);
	GetStarTeacher(0, strToken, vecTeacher, msg);
	return 0;
}