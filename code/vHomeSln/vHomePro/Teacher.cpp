#include "Teacher.h"


CTeacher::CTeacher(void)
{
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

void CTeacher::SetSchool(string strSchool)
{
	m_strSchool = strSchool;
}
void CTeacher::SetArticle(string strArticle)
{
	m_strArticle = strArticle;
}
void CTeacher::SetArticleId(int nArticleId)
{
	m_nArticleId = nArticleId;
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