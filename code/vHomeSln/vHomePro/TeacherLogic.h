#pragma once
#include <string>
#include <vector>
#include "ResMsg.h"
#include "Teacher.h"
using namespace std;


// ��ʦҵ���߼���
class CTeacherLogic
{
public:
	CTeacherLogic(void);
	~CTeacherLogic(void);

public:
	// ��ע��ʦ
	int AddAttention(int nTid, const string& strToken, CResMsg& msg);

	// ȡ����ע
	int DelAttention(int nTid, const string& strToken, CResMsg& msg);

	// ��ȡ��ע�Ľ�ʦ
	int GetCareTeacher(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg);

	// ��ȡ�Ƽ���ʦ
	int GetTeachers(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg );

	// ��ȡ��ʦ�б�(����ǰ��)
	int GetTopTeacher(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg);

	// ��ȡ��ʦ�б�(ǰ������)
	int GetStarTeacher(int nPage, const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg); 
};

