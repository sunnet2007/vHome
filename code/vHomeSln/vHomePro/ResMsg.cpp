#include "ResMsg.h"


CResMsg::CResMsg(void)
{
	m_nStatus = 0;
	m_strInfo = "";
}

CResMsg::~CResMsg(void)
{
}

void CResMsg::SetStatus(int nStatus)
{
	m_nStatus = nStatus;
}
void CResMsg::SetInfo(const string& strInfo)
{
	m_strInfo = strInfo;
}

int CResMsg::GetStatus() const
{
	return m_nStatus;
}
string CResMsg::GetInfo() const
{
	return m_strInfo;
}