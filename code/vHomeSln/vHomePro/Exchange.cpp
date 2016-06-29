#include "Exchange.h"


CExchange::CExchange(void)
{
	m_nId = 0; // ������id
	m_strName = "";	// ����������
	m_strInfo = "";	// ���������
	m_strImage = "";	// ������ͼƬ
	m_nNum = 0;			// �μ�����
	//m_strAuth = "";		// ��������֤ 0:���ڰ� 1:v�� 2:�����
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