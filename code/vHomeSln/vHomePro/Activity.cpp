#include "Activity.h"


CActivity::CActivity(void)
{
	m_nId			=	0;	// 活动ID
	m_strTitle		=	"";	// 活动标题
	m_strContent	=	"";	// 获取内容
	m_nNum			=	0;	// 参加人数
	time(&m_tStart)		;	// 开始时间
	time(&m_tEnd)		;	// 结束时间
	m_nInNum		=	0;	// 活动限制人数
	m_nMyStatus		=	0;	// 登录状态
	m_strImg		=	"";	// 活动图片
}


CActivity::~CActivity(void)
{
}

void CActivity::SetId(int nId)
{
	m_nId = nId;
}
void CActivity::SetTitle(const string& strTitle)
{
	m_strTitle = strTitle;
}
void CActivity::SetContent(const string& strContent)
{
	m_strContent = strContent;
}
void CActivity::SetNum(int nNum)
{
	m_nNum = nNum;
}
void CActivity::SetStart(time_t tStart)
{
	m_tStart = tStart;
}
void CActivity::SetEnd(time_t tEnd)
{
	m_tEnd = tEnd;
}
void CActivity::SetInNum(int nInNum)
{
	m_nInNum = nInNum;
}
void CActivity::SetMyStatus(int nStatus)
{
	m_nMyStatus = nStatus;
}
void CActivity::SetImg(const string& strImg)
{
	m_strImg = strImg;
}

int CActivity::GetId()  const
{
	return m_nId;
}
string CActivity::GetTitle() const
{
	return m_strTitle;
}
string CActivity::GetContent() const
{
	return m_strContent;
}
int CActivity::GetNum() const
{
	return m_nNum;
}
time_t CActivity::GetStart() const
{
	return m_tStart;
}
time_t CActivity::GetEnd() const
{
	return m_tEnd;
}
int CActivity::GetInNum() const
{
	return m_nInNum;
}
int CActivity::GetMyStatus() const
{
	return m_nMyStatus;
}

string CActivity::GetImg() const
{
	return m_strImg;
}