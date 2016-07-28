#include "Teacher.h"



CSpecialty::CSpecialty(void)
{
	m_nId = 0;
	m_strName = "";
}
CSpecialty::~CSpecialty(void)
{

}
void CSpecialty::SetId(int nId)
{
	m_nId = nId;
}
void CSpecialty::SetName(const string& strName)
{
	m_strName = strName;
}

int CSpecialty::GetId() const
{
	return m_nId;
}
string CSpecialty::GetName() const
{
	return m_strName;
}

CTeacher::CTeacher(void)
{
	m_strName		=	"";	// ��ʦ����
	m_strDesc		=	"";	// ��ʦ���
	m_strExName		=	"";	// ����������
	m_strImgThumb	=	"";	// ͷ��
	m_nNum			=	0;	// ��˿����
	m_nUid			=	0;	// ��ʦUid
	m_nStar			=	0;	// ��ʦ�Ǽ�
	m_nIfAttention	=	0;	// �Ƿ��ע	1�ѹ�ע 0δ��ע
	m_strSchool		=	"";	// ��ҵԺУ
	m_strArticle	=	"";	// ��������
	m_nArticleId	=	0;	// ��������ID
	m_strNicName	=	"";	// ��ʦ�ǳ�
	m_strSex		=	"";	// �Ա�
	m_strEmail		=	"";	// ����
	m_strQq			=	"";	// QQ
	m_nExid			=	0;
	m_strGoodatIds	=	"";
}


CTeacher::~CTeacher(void)
{
}

void CTeacher::SetName(const string& strName)
{
	m_strName = strName;
}
void CTeacher::SetDesc(const string& strDesc)
{
	m_strDesc = strDesc;
}
void CTeacher::SetExName(const string& strExName)
{
	m_strExName = strExName;
}
void CTeacher::SetImgThumb(const string& strImgThumb)
{
	m_strImgThumb = strImgThumb;
}
void CTeacher::SetNum(int nNum)
{
	m_nNum = nNum;
}
void CTeacher::SetUid(int nUid)
{
	m_nUid = nUid;
}
void CTeacher::SetStar(int nStar)
{
	m_nStar = nStar;
}
void CTeacher::SetIfAttention(int nIfAttention)
{
	m_nIfAttention = nIfAttention;
}

void CTeacher::SetSchool(const string& strSchool)
{
	m_strSchool = strSchool;
}
void CTeacher::SetArticle(const string& strArticle)
{
	m_strArticle = strArticle;
}
void CTeacher::SetArticleId(int nArticleId)
{
	m_nArticleId = nArticleId;
}
void CTeacher::SetNicName(const string& strNicName)
{
	m_strNicName = strNicName;
}
void CTeacher::SetSex(const string& strSex)
{
	m_strSex = strSex;
}
void CTeacher::SetEmail(const string& strEmail)
{
	m_strEmail = strEmail;
}
void CTeacher::SetQq(const string& strQq)
{
	m_strQq = strQq;
}

void CTeacher::SetExid(int nId)
{
	m_nExid = nId;
}

void CTeacher::SetGoodat(const vector<CSpecialty> vecGoodat)
{
	m_vecGoodat = vecGoodat;
}
void CTeacher::SetGoodatIds(const string& strGoodatIds)
{
	m_strGoodatIds = strGoodatIds;
}


string CTeacher::GetName() const
{
	return m_strName;
}
string CTeacher::GetDesc() const
{
	return m_strDesc;
}
string CTeacher::GetExName() const
{
	return m_strExName;
}
string CTeacher::GetImgThumb() const
{
	return m_strImgThumb;
}
int CTeacher::GetNum() const
{
	return m_nNum;
}
int CTeacher::GetUid() const
{
	return m_nUid;
}
int CTeacher::GetStar() const
{
	return m_nStar;
}
int CTeacher::GetIfAttention() const
{
	return m_nIfAttention;
}

string CTeacher::GetSchool() const
{
	return m_strSchool;
}
string CTeacher::GetArticle() const
{
	return m_strArticle;
}
int CTeacher::GetArticleId() const
{
	return m_nArticleId;
}

string CTeacher::GetNicName() const
{
	return m_strNicName;
}
string CTeacher::GetSex() const
{
	return m_strSex;
}
string CTeacher::GetEmail() const
{
	return m_strEmail;
}
string CTeacher::GetQq() const
{
	return m_strQq;
}
int CTeacher::GetExid() const
{
	return m_nExid;
}

vector<CSpecialty> CTeacher::GetGoodat() const
{
	return m_vecGoodat;
}

string CTeacher::GetGoodIds() const
{
	return m_strGoodatIds;
}