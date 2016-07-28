#pragma once
#include <string>
#include <vector>
#include "ResMsg.h"
#include "Teacher.h"
#include "Article.h"
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
	int GetStarTeacher(int nPage, int nPageNum, const string& strToken, vector<CTeacher>& vecTeacher, CResMsg& msg); 

	// ��ȡ��ʦ������������Ϣ
	int GetAllArtOfTeacher(int nPage, int nPageNum, const string& strToken, vector<CArticle>& vecArt, CResMsg& msg);

	// ��ȡ��������ǰ�������Ϣ
	int GetArtCat(const string& strToken, string& strExchange, vector<CCategory>& vecCtgy, CResMsg& msg);

	// ��������
	int ReleaseArt(const string& strToken, const CArticle& art, CResMsg& msg);

	// ��ʾ��ʦ��Ϣ
	int GetTeacherInfo(CTeacher& teacher, vector<CSpecialty>& vecSpecialty,const string strToken, CResMsg& msg);

	// �޸Ľ�ʦ��Ϣ
	int ModifyTeacherInfo(const CTeacher& teacher,const string strToken, CResMsg& msg);
	// ��ȡ��ʦ����
	int GetTeacherArts(int nPage, int nPageNum , int tId,const string& strToken, vector<CArticle>& vecArt,vector<CCategory>& vecCtgy, CResMsg& msg ,int nCtgyId = 0);

	// ��������ͼƬ
	int UploadArtImg(const string& strToken, int nArtId, const string& strImg, string& strImgUrl, CResMsg& msg);

	// ������ʦͷ��
	int UploadTeacherImg(const string& strToken, const string& strImg, CResMsg& msg);
};

