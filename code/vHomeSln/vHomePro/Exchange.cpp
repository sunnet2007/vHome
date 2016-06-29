#include "Exchange.h"


CExchange::CExchange(void)
{
	m_nId = 0; // 交易所id
	m_strName = "";	// 交易所名称
	m_strInfo = "";	// 交易所简介
	m_strImage = "";	// 交易所图片
	m_nNum = 0;			// 参加人数
	//m_strAuth = "";		// 交易所认证 0:金融办 1:v家 2:商务办
}


CExchange::~CExchange(void)
{
}

void CExchange::SetId(unsigned int nId)
{
	m_nId = nId;
}
void CExchange::SetName(const string& strName)
{
	m_strName = strName;
}
void CExchange::SetInfo(const string& strInfo)
{
	m_strInfo = strInfo;
}
void CExchange::SetImage(const string strImage)
{
	m_strImage = strImage;
}
void CExchange::SetNum(int nNum)
{
	m_nNum = nNum;
}
void CExchange::SetAuth(vector<string> vecAuth)
{
	m_vecAuth = vecAuth;
}

unsigned int CExchange::GetId() const
{
	return m_nId;
}
string CExchange::GetName() const
{
	return m_strName;
}
string CExchange::GetInfo() const
{
	return m_strInfo;
}
string CExchange::GetImage() const
{
	return m_strImage;
}
int CExchange::GetNum() const
{
	return m_nNum;
}
vector<string> CExchange::GetAuth() const
{
	return m_vecAuth;
}