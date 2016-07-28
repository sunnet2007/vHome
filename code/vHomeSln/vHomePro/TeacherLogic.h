#pragma once
#include <string>
#include <vector>
#include "ResMsg.h"
#include "Teacher.h"
#include "Article.h"
using namespace std;


// ��ʦҵ���߼���
class CTeacherLogic
{
public:
	CTeacherLogic(void);
	~CTeacherLogic(void);

public:
	// ��ע��ʦ
	int AddAttention(int nTid, const string& strToken, string& strMsg);

	// ȡ����ע
	int DelAttention(int nTid, const string& strToken, string& strMsg);

	// ��ȡ��ע�Ľ�ʦ
	int GetCareTeacher(const string& strToken, vector<CTeacher>& vecTeacher, string& strMsg);

	// ��ȡ�Ƽ���ʦ
	int GetTeachers(int nPage, int nPageNum, const string& strToken, vector<CTeacher>& vecTeacher, string& strMsg );

	// ��ȡ��ʦ�б�(����ǰ��)
	int GetTopTeacher(const string& strToken, vector<CTeacher>& vecTeacher, string& strMsg);

	// ��ȡ��ʦ�б�(ǰ������)
	int GetStarTeacher(int nPage,  int nPageNum, const string& strToken, vector<CTeacher>& vecTeacher, string& strMsg); 

	// ��ʦ��ȡ�Լ�����������
	int GetAllArtOfTeacher(int nPage, int nPageNum, const string& strToken, vector<CArticle>& vecArt, string& strMsg);

	// ��ȡ��������ǰ�������Ϣ
	int GetArtCat(const string& strToken, string& strExchange, vector<CCategory>& vecCtgy, string& strMsg);

	// ��������
	int ReleaseArt(const string& strToken, CArticle& art, string strMsg);

	// ��ʾ��ʦ��Ϣ
	int GetTeacherInfo(CTeacher& teacher, vector<CSpecialty>& vecSpecialty,const string strToken, string strMsg);

	// �޸Ľ�ʦ��Ϣ
	int ModifyTeacherInfo(const CTeacher& teacher,const string strToken, string strMsg);
	// ��ȡ��ʦ����������
	int GetTeacherArts(int nPage, int nPageNum , int tId,const string& strToken, vector<CArticle>& vecArt,vector<CCategory>& vecCtgy, string& strMsg, int nCtgyId = 0);

	int UploadTeacherImage(const string& strToken, const string& strImg, string& strMsg);
};

