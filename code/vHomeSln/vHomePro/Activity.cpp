#include "Activity.h"


CActivity::CActivity(void)
{
	m_nId			=	0;	// �ID
	m_strTitle		=	"";	// �����
	m_strContent	=	"";	// ��ȡ����
	m_nNum			=	0;	// �μ�����
	time(&m_tStart)		;	// ��ʼʱ��
	time(&m_tEnd)		;	// ����ʱ��
	m_nInNum		=	0;	// ���������
	m_nMyStatus		=	0;	// ��¼״̬
	m_strImg		=	"";	// �ͼƬ
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