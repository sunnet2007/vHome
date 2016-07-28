#include "Article.h"

CCategory::CCategory(void)
{
	m_nId = 0;
	m_strName = "";
	m_strImg = "";
}
CCategory::~CCategory(void)
{

}

void CCategory::SetId(int nId)
{
	m_nId = nId;
}
void CCategory::SetName(const string& strName)
{
	m_strName = strName;
}

void CCategory::SetImg(const string& strImg)
{
	m_strImg = strImg;
}

int CCategory::GetId() const
{
	return m_nId;
}
string CCategory::GetName() const
{
	return m_strName;
}

string CCategory::GetImg() const
{
	return m_strImg;
}


CArticle::CArticle(void)
{
	m_nId			= 0;	// 文章id
	m_strAbout		= "";	// 文章简介
	m_nHits			= 0;	// 点赞数
	m_strTitle		= "";	// 文章标题
	m_strContent	= "";	// 文章内容
	m_strImgThumb	= "";	// 文章图片
	m_tCTime		= 0;	// 创建时间
	m_strCtgy		= "";	// 所属分类
	m_nCtgyId		= 0;	// 所属分类id
	m_nStatus		= 0;	// 状态 0未通过 1通过
	m_strReason		= "";	// 未通过原因
	m_nExid			= 0;
	m_strExName		= "";
}


CArticle::~CArticle(void)
{
}

void CArticle::SetId(int nId)
{
	m_nId = nId;
}
void CArticle::SetAbout(const string& strAbout)
{
	m_strAbout = strAbout;
}
void CArticle::SetHits(int nHits)
{
	m_nHits = nHits;
}
void CArticle::SetTitle(const string& strTitle)
{
	m_strTitle = strTitle;
}
void CArticle::SetContent(const string& strContent)
{
	m_strContent = strContent;
}
void CArticle::SetImgThumb(const string& strImgThumb)
{
	m_strImgThumb = strImgThumb;
}
void CArticle::SetCTime(time_t tTime)
{
	m_tCTime = tTime;
}
void CArticle::SetCtgy(const string& strCtgy)
{
	m_strCtgy = strCtgy;
}
void CArticle::SetCtgyId(int nCtgtId)
{
	m_nCtgyId = nCtgtId;
}
void CArticle::SetStatus(int nStatus)
{
	m_nStatus = nStatus;
}
void CArticle::SetReason(const string& strReason)
{
	m_strReason = strReason;
}

void CArticle::SetExid(int nExid)
{
	m_nExid = nExid;
}
void CArticle::SetExName(const string& strExName)
{
	m_strExName = strExName;
}


int CArticle::GetId() const
{
	return m_nId;
}
string CArticle::GetAbout() const
{
	return m_strAbout;
}
int CArticle::GetHits() const
{
	return m_nHits;
}
string CArticle::GetTitle() const
{
	return m_strTitle;
}
string CArticle::GetContent() const
{
	return m_strContent;
}
string CArticle::GetImgThumb() const
{
	return m_strImgThumb;
}
time_t CArticle::GetCTime() const
{
	return m_tCTime;
}
string CArticle::GetCtgy() const
{
	return m_strCtgy;
}
int CArticle::GetCtgyId() const
{
	return m_nCtgyId;
}
int CArticle::GetStatus() const
{
	return m_nStatus;
}
string CArticle::GetReason() const
{
	return m_strReason;
}

int CArticle::GetExid() const
{
	return m_nExid;
}
string CArticle::GetExName() const
{
	return m_strExName;
}