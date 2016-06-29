#include "TeacherLogic.h"
#include "TeacherDao.h"

CTeacherLogic::CTeacherLogic(void)
{
}


CTeacherLogic::~CTeacherLogic(void)
{
}

// ��ע��ʦ
int CTeacherLogic::AddAttention(int nTid, const string& strToken, CResMsg& msg)
{
	CTeacherDao teacherDao;
	int nRes = teacherDao.AddAttention(nTid,strToken,msg);
	return nRes;
}

// ȡ����ע
int CTeacherLogic::DelAttention(int nTid, const string& strToken, CResMsg& msg)
{
	CTeacherDao teacherDao;
	int nRes = teacherDao.DelAttention(nTid, strToken, msg);
	return nRes;
}

// ��ȡ��ע�Ľ�ʦ
int CTeacherLogic::GetCareTeacher(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg)
{
	CTeacherDao teacherDao;
	int nRes = teacherDao.GetCareTeacher(strToken, vecTeacher, msg);
	return nRes;
}

// ��ȡ��ʦ�б�(����ǰ��)
int CTeacherLogic::GetTopTeacher(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg)
{
	CTeacherDao teacherDao;
	int nRes = teacherDao.GetTopTeacher(strToken, vecTeacher, msg);
	return nRes;
}

// ��ȡ��ʦ�б�(ǰ������)
int CTeacherLogic::GetStarTeacher(int nPage, const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg)
{
	CTeacherDao teacherDao;
	int nRes = teacherDao.GetStarTeacher(nPage, strToken, vecTeacher, msg);
	return nRes;
}

// ��ȡ�Ƽ���ʦ
int CTeacherLogic::GetTeachers(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg )
{
	GetTopTeacher(strToken, vecTeacher, msg);
	GetStarTeacher(0, strToken, vecTeacher, msg);
	return 0;
}