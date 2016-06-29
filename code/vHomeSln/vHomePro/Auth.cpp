#include "Auth.h"


CAuth::CAuth(void)
{
	m_nId = 0;
	m_strName = "";
}


CAuth::~CAuth(void)
{
}

void CAuth::SetId(int nId)
{
	m_nId = nId;
}
void CAuth::SetName(string strName)
{
	m_strName = strName;
}

int CAuth::GetId()
{
	return m_nId;
}
string CAuth::GetName()
{
	return m_strName;
}