#pragma once
#include <string>
#include <vector>
#include "ResMsg.h"
#include "Teacher.h"
using namespace std;

// ��ʦ��Ϣ������
class CTeacherDao
{
public:
	CTeacherDao(void);
	~CTeacherDao(void);

public:
	// ��ע��ʦ
	int AddAttention(int nTid, const string& strToken, CResMsg& msg);
	// ȡ����ע
	int DelAttention(int nTid, const string& strToken, CResMsg& msg);

	// ��ȡ��ע�Ľ�ʦ
	int GetCareTeacher(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg);

	// ��ȡ��ʦ�б�(����ǰ��)
	int GetTopTeacher(const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg);

	// ��ȡ��ʦ�б�(ǰ������)
	int GetStarTeacher(int nPage, const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg); 

};

