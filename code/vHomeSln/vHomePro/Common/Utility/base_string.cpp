#include "StdAfx.h"
#include "base_string.h"
#include <algorithm> 

#pragma warning(disable:4267) 


CBaseStringA::CBaseStringA(void)
{
}
CBaseStringA::CBaseStringA( const char* lpsz )
{
	m_str = lpsz;
}
CBaseStringA::CBaseStringA( const string & str )
{
	m_str = str;
}
CBaseStringA::CBaseStringA( const CBaseStringA & str )
{
	m_str = str.m_str;
}
CBaseStringA::CBaseStringA( const wchar_t* lpsz )
{
	*this = lpsz;
}
CBaseStringA::CBaseStringA( const wstring & str )
{
	*this = str.c_str();
}
CBaseStringA::CBaseStringA( const CBaseStringW & str )
{
	*this = str.c_str();
}
CBaseStringA::CBaseStringA( const CBaseStringU & str )
{
	*this = str;
}
CBaseStringA::~CBaseStringA(void)
{
}
bool CBaseStringA::operator==( const CBaseStringA & str )
{
	return m_str == str.m_str;
}
bool CBaseStringA::operator==( const string & str )
{
	return m_str == str;
}
bool CBaseStringA::operator==( const char* lpsz )
{
	return m_str == lpsz;
}
bool CBaseStringA::operator!=( const CBaseStringA & str )
{
	return m_str != str.m_str;
}
bool CBaseStringA::operator!=( const string & str )
{
	return m_str != str;
}
bool CBaseStringA::operator!=( const char* lpsz )
{
	return m_str != lpsz;
}

CBaseStringA & CBaseStringA::operator=( const CBaseStringA & str )
{
	this->m_str = str.m_str;
	return *this;
}
CBaseStringA & CBaseStringA::operator=( const string & str )
{
	this->m_str = str;
	return *this;
}
CBaseStringA & CBaseStringA::operator=( const char* lpsz )
{
	this->m_str = lpsz;
	return *this;
}
CBaseStringA & CBaseStringA::operator=( const CBaseStringW & str )
{
	*this = str.c_str();
	return *this;
}
CBaseStringA & CBaseStringA::operator=( const wstring & str )
{
	*this = str.c_str();
	return *this;
}
CBaseStringA & CBaseStringA::operator=( const wchar_t* lpsz )
{
	int ansilen = ::WideCharToMultiByte( CP_ACP, NULL, lpsz, -1, NULL, 0, NULL, NULL );
	if( ansilen > 0 )
	{
		char * p = new char[ansilen+1];
		::WideCharToMultiByte( CP_ACP, NULL, lpsz, -1, p, ansilen+1, NULL, NULL );
		m_str = p;
		delete p;
	}
	return *this;
}
CBaseStringA &CBaseStringA::operator=( const CBaseStringU & str )
{
	int wclen = ::MultiByteToWideChar( CP_UTF8, NULL, str.c_str(), -1, NULL, 0 );
	if( wclen > 0 )
	{
		wchar_t * pW = new wchar_t[wclen+1];
		::MultiByteToWideChar( CP_UTF8, NULL, str.c_str(), -1, pW, wclen+1 );

		int ansilen = ::WideCharToMultiByte( CP_ACP, NULL, pW, -1, NULL, 0, NULL, NULL );
		if( ansilen > 0 )
		{
			char * p = new char[ansilen+1];
			::WideCharToMultiByte( CP_ACP, NULL, pW, -1, p, ansilen+1, NULL, NULL );
			m_str = p;
			delete p;
		}
		delete pW;
	}
	return *this;
}
CBaseStringA & CBaseStringA::operator+=( const CBaseStringA & str )
{
	m_str += str.c_str();
	return *this;
}
CBaseStringA & CBaseStringA::operator+=( const string & str )
{
	m_str += str.c_str();
	return *this;
}
CBaseStringA & CBaseStringA::operator+=( const char* lpsz )
{
	m_str += lpsz;
	return *this;
}
bool CBaseStringA::operator<( const CBaseStringA & str ) const
{
	return m_str < str.m_str;
}
const string & CBaseStringA::GetStr() const
{
	return m_str;
}
const char* CBaseStringA::c_str() const
{
	return m_str.c_str();
}
CBaseStringA::operator LPSTR() const
{
	return (LPSTR)m_str.c_str();
}
int CBaseStringA::Find( const char* lpsz, int pos )
{
	int poss = m_str.find( lpsz, pos );
	return poss;
}
int CBaseStringA::Find( const string & str, int pos )
{
	int poss = m_str.find( str, pos );
	return poss;
}
int CBaseStringA::Find( const CBaseStringA & str, int pos)
{
	int poss = m_str.find( str.m_str, pos );
	return poss;
}
int CBaseStringA::Find( char ch, int pos )
{
	int poss = m_str.find( ch, pos );
	return pos;
}
int CBaseStringA::RFind( const char* lpsz )
{
	int poss = m_str.rfind( lpsz );
	return poss;
}
int CBaseStringA::RFind( const string & str )
{
	int poss = m_str.rfind( str );
	return poss;
}
int CBaseStringA::RFind( const CBaseStringA & str )
{
	int poss = m_str.rfind( str.m_str );
	return poss;
}
int CBaseStringA::RFind( char ch )
{
	int poss = m_str.rfind( ch );
	return poss;
}
CBaseStringA CBaseStringA::substr( int pos, string::size_type _Count ) const
{
	return m_str.substr( pos, _Count );
}
CBaseStringA CBaseStringA::Left(int iLength) const
{
	if( iLength < 0 ) iLength = 0;
	if( iLength > m_str.length() ) iLength = m_str.length();
	return CBaseStringA(m_str.substr(0, iLength));
}

CBaseStringA CBaseStringA::Right(int iLength) const
{
	int iPos = m_str.length() - iLength;
	if( iPos < 0 ) {
		iPos = 0;
		iLength = m_str.length();
	}
	return CBaseStringA(m_str.substr(iPos, iLength));
}
CBaseStringA CBaseStringA::Mid(int iPos, int iLength) const
{
	if( iLength < 0 ) iLength = m_str.length() - iPos;
	if( iPos + iLength > m_str.length() ) iLength = m_str.length() - iPos;
	if( iLength <= 0 ) return CBaseStringA();
	return CBaseStringA(m_str.substr(iPos,iLength));
}
CBaseStringA CBaseStringA::Replace( int pos, int count, const char* lpsz )
{
	return m_str.replace( pos, count, lpsz ).c_str();
}
int CBaseStringA::Replace(LPCSTR pstrFrom, LPCSTR pstrTo)
{
	CBaseStringA sTemp;
	int nCount = 0;
	int iPos = Find(pstrFrom);
	if( iPos < 0 ) return 0;
	int cchFrom = (int) strlen(pstrFrom);
	int cchTo = (int) strlen(pstrTo);
	while( iPos >= 0 ) {
		sTemp = Left(iPos);
		sTemp += pstrTo;
		sTemp += Mid(iPos + cchFrom);
		*this = sTemp; 
		iPos = Find(pstrFrom, iPos + cchTo);
		nCount++;
	}
	return nCount;
}
int CBaseStringA::CompareNoCase( LPCSTR pstr )
{
	return stricmp(m_str.c_str(),pstr );
}
void CBaseStringA::MakeUpper()
{
	transform( m_str.begin(), m_str.end(), m_str.begin(), toupper );
}
void CBaseStringA::MakeLower()
{
	transform( m_str.begin(), m_str.end(), m_str.begin(), tolower );
}
void CBaseStringA::Trim()
{
	TrimLeft();
	TrimRight();
}
void CBaseStringA::TrimLeft()
{
	size_t begin = 0;
	size_t len = m_str.length();
	while( begin < len )
	{
		if( m_str[begin] != ' ' )
			break;
		begin++;
	}
	m_str = m_str.substr( begin );
}
void CBaseStringA::TrimRight()
{
	size_t begin = m_str.length();
	while( begin > 0 )
	{
		if( m_str[begin-1] != ' ' )
			break;
		begin--;
	}
	m_str = m_str.substr( 0, begin );
}
int CBaseStringA::GetLength()
{
	return m_str.length();
}
bool CBaseStringA::IsEmpty()
{
	return m_str.empty();
}
void CBaseStringA::clear()
{
	m_str.clear();
}
int CBaseStringA::Format(const char* lpFormat, ...)
{
	va_list argptr;
	va_start(argptr, lpFormat);
	int bufsize = _vsnprintf(NULL, 0, lpFormat, argptr) + 2;
	char* buf = new char[bufsize];
	int iRet = _vsnprintf(buf, bufsize, lpFormat, argptr);
	m_str = buf;
	delete[] buf;  
	va_end(argptr); 
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
CBaseStringW::CBaseStringW(void)
{
}
CBaseStringW::CBaseStringW( const wchar_t* lpsz )
{
	m_str = lpsz;
}
CBaseStringW::CBaseStringW( const wstring & str )
{
	m_str = str;
}
CBaseStringW::CBaseStringW( const CBaseStringW & str )
{
	m_str = str.m_str;
}
CBaseStringW::CBaseStringW( const char* lpsz )
{
	*this = lpsz;
}
CBaseStringW::CBaseStringW( const string & str )
{
	*this = str.c_str();
}
CBaseStringW::CBaseStringW( const CBaseStringA & str )
{
	*this = str.c_str();
}
CBaseStringW::CBaseStringW( const CBaseStringU & str )
{
	*this = str;
}
CBaseStringW::~CBaseStringW(void)
{
}
bool CBaseStringW::operator==( const CBaseStringW & str )
{
	return m_str == str.m_str;
}
bool CBaseStringW::operator==( const wstring & str )
{
	return m_str == str;
}
bool CBaseStringW::operator==( const wchar_t* lpsz )
{
	return m_str == lpsz;
}
bool CBaseStringW::operator!=( const CBaseStringW & str )
{
	return m_str != str.m_str;
}
bool CBaseStringW::operator!=( const wstring & str )
{
	return m_str != str;
}
bool CBaseStringW::operator!=( const wchar_t* lpsz )
{
	return m_str != lpsz;
}

CBaseStringW & CBaseStringW::operator=( const CBaseStringW & str )
{
	this->m_str = str.m_str;
	return *this;
}
CBaseStringW & CBaseStringW::operator=( const wstring & str )
{
	this->m_str = str;
	return *this;
}
CBaseStringW & CBaseStringW::operator=( const wchar_t* lpsz )
{
	this->m_str = lpsz;
	return *this;
}
CBaseStringW & CBaseStringW::operator=( const CBaseStringA & str )
{
	*this = str.c_str();
	return *this;
}
CBaseStringW & CBaseStringW::operator=( const string & str )
{
	*this = str.c_str();
	return *this;
}
CBaseStringW & CBaseStringW::operator+=( const CBaseStringW & str )
{
	m_str += str.c_str();
	return *this;
}
CBaseStringW & CBaseStringW::operator=( const CBaseStringU & str )
{
	int wclen = ::MultiByteToWideChar( CP_UTF8, NULL, str.c_str(), -1, NULL, 0 );
	//int wclen = ::MultiByteToWideChar( CP_ACP, NULL, str.c_str(), -1, NULL, 0 );
	if( wclen > 0 )
	{
		wchar_t * p = new wchar_t[wclen+1];
		::MultiByteToWideChar( CP_UTF8, NULL, str.c_str(), -1, p, wclen+1 );
		//::MultiByteToWideChar( CP_ACP, NULL, str.c_str(), -1, p, wclen+1 );
		m_str = p;
		delete p;
	}
	return *this;
}
CBaseStringW & CBaseStringW::operator+=( const wstring & str )
{
	m_str += str.c_str();
	return *this;
}
CBaseStringW & CBaseStringW::operator+=( const wchar_t* lpsz )
{
	m_str += lpsz;
	return *this;
}
CBaseStringW & CBaseStringW::operator=( const char* lpsz )
{
	int wclen = ::MultiByteToWideChar( CP_ACP, NULL, lpsz, -1, NULL, 0 );
	if( wclen > 0 )
	{
		wchar_t * p = new wchar_t[wclen+1];
		::MultiByteToWideChar( CP_ACP, NULL, lpsz, -1, p, wclen+1 );
		m_str = p;
		delete p;
	}
	return *this;
}
bool CBaseStringW::operator<( const CBaseStringW & str ) const
{
	return m_str < str.m_str;
}
const wstring & CBaseStringW::GetStr() const
{
	return m_str;
}
const WCHAR* CBaseStringW::c_str() const
{
	return m_str.c_str();
}
CBaseStringW::operator LPWSTR() const
{
	return (LPWSTR)m_str.c_str();
}
int CBaseStringW::Find( const wchar_t* lpsz, int pos )
{
	int poss = m_str.find( lpsz, pos );
	return poss;
}
int CBaseStringW::Find( const wstring & str, int pos )
{
	int poss = m_str.find( str, pos );
	return poss;
}
int CBaseStringW::Find( const CBaseStringW & str, int pos)
{
	int poss = m_str.find( str.m_str, pos );
	return poss;
}
int CBaseStringW::Find( wchar_t ch, int pos )
{
	int poss = m_str.find( ch, pos );
	return poss;
}
int CBaseStringW::RFind( const wchar_t* lpsz )
{
	int poss = m_str.rfind( lpsz );
	return poss;
}
int CBaseStringW::RFind( const wstring & str )
{
	int poss = m_str.rfind( str );
	return poss;
}
int CBaseStringW::RFind( const CBaseStringW & str )
{
	int poss = m_str.rfind( str.m_str );
	return poss;
}
int CBaseStringW::RFind( wchar_t ch )
{
	int poss = m_str.rfind( ch );
	return poss;
}
CBaseStringW CBaseStringW::substr( int pos, wstring::size_type _Count ) const
{
	return m_str.substr( pos, _Count );
}
CBaseStringW CBaseStringW::Left(int iLength) const
{
	if( iLength < 0 ) iLength = 0;
	if( iLength > m_str.length() ) iLength = m_str.length();
	return CBaseStringW(m_str.substr(0, iLength));
}

CBaseStringW CBaseStringW::Right(int iLength) const
{
	int iPos = m_str.length() - iLength;
	if( iPos < 0 ) {
		iPos = 0;
		iLength = m_str.length();
	}
	return CBaseStringW(m_str.substr(iPos, iLength));
}
CBaseStringW CBaseStringW::Mid(int iPos, int iLength) const
{
	if( iLength < 0 ) iLength = m_str.length() - iPos;
	if( iPos + iLength > m_str.length() ) iLength = m_str.length() - iPos;
	if( iLength <= 0 ) return CBaseStringW();
	return CBaseStringW(m_str.substr(iPos,iLength));
}
CBaseStringW CBaseStringW::Replace( int pos, int count, const wchar_t* lpsz )
{
	return m_str.replace( pos, count, lpsz ).c_str();
}
int CBaseStringW::Replace(LPCWSTR pstrFrom, LPCWSTR pstrTo)
{
	CBaseStringW sTemp;
	int nCount = 0;
	int iPos = Find(pstrFrom);
	if( iPos < 0 ) return 0;
	int cchFrom = (int) wcslen(pstrFrom);
	int cchTo = (int) wcslen(pstrTo);
	while( iPos >= 0 ) {
		sTemp = Left(iPos);
		sTemp += pstrTo;
		sTemp += Mid(iPos + cchFrom);
		*this = sTemp;
		iPos = Find(pstrFrom, iPos + cchTo);
		nCount++;
	}
	return nCount;
}
int CBaseStringW::CompareNoCase( LPCWSTR pstr )
{
	return wcsicmp( m_str.c_str(),pstr );
}
void CBaseStringW::MakeUpper()
{
	transform( m_str.begin(), m_str.end(), m_str.begin(), toupper );
}
void CBaseStringW::MakeLower()
{
	transform( m_str.begin(), m_str.end(), m_str.begin(), tolower );
}

void CBaseStringW::Trim()
{
	TrimLeft();
	TrimRight();
}
void CBaseStringW::TrimLeft()
{
	size_t begin = 0;
	size_t len = m_str.length();
	while( begin < len )
	{
		if( m_str[begin] != L' ' )
			break;
		begin++;
	}
	m_str = m_str.substr( begin );
}
void CBaseStringW::TrimRight()
{
	int begin = m_str.length();
	while( begin > 0 )
	{
		if( m_str[begin-1] != L' ' )
			break;
		begin--;
	}
	m_str = m_str.substr( 0, begin );
}
int CBaseStringW::GetLength()
{
	return m_str.length();
}
bool CBaseStringW::IsEmpty()
{
	return m_str.empty();
}
void CBaseStringW::clear()
{
	m_str.clear();
}
int CBaseStringW::Format(const wchar_t* lpFormat, ...)
{
	va_list argptr;
	va_start(argptr, lpFormat);
	int bufsize = _vsnwprintf(NULL, 0, lpFormat, argptr) + 2;
	WCHAR* buf = new WCHAR[bufsize];
	int iRet = _vsnwprintf(buf, bufsize, lpFormat, argptr);
	m_str = buf;
	delete[] buf;  
	va_end(argptr); 
	return iRet;
}

//////////////////////////////////////////////////////////////////////
CBaseStringU::CBaseStringU(void)
{
}
CBaseStringU::CBaseStringU( const char* lpsz )
{
	m_str = lpsz;
}
CBaseStringU::CBaseStringU( const string & str )
{
	*this = str;
}
CBaseStringU::CBaseStringU( const CBaseStringA & str )
{
	*this = str.GetStr();
}
CBaseStringU::CBaseStringU( const CBaseStringU & str )
{
	this->m_str = str.m_str.c_str();
}
CBaseStringU::CBaseStringU( const wchar_t* lpsz )
{
	*this = lpsz;
}
CBaseStringU::CBaseStringU( const wstring & str )
{
	*this = str.c_str();
}
CBaseStringU::CBaseStringU( const CBaseStringW & str )
{
	*this = str.c_str();
}
CBaseStringU::~CBaseStringU(void)
{
}
bool CBaseStringU::operator==( const CBaseStringU & str )
{
	return m_str == str.m_str;
}
bool CBaseStringU::operator!=( const CBaseStringU & str )
{
	return m_str != str.m_str;
}

CBaseStringU & CBaseStringU::operator=( const CBaseStringU & str )
{
	this->m_str = str.m_str;
	return *this;
}
CBaseStringU & CBaseStringU::operator=( const CBaseStringA & str )
{
	*this = str.GetStr();
	return *this;
}
CBaseStringU & CBaseStringU::operator=( const string & str )
{
	int wclen = ::MultiByteToWideChar( CP_ACP, NULL, str.c_str(), -1, NULL, 0 );
	if( wclen > 0 )
	{
		wchar_t * pW = new wchar_t[wclen+1];
		::MultiByteToWideChar( CP_ACP, NULL, str.c_str(), -1, pW, wclen+1 );

		int ansilen = ::WideCharToMultiByte( CP_UTF8, NULL, pW, -1, NULL, 0, NULL, NULL );
		if( ansilen > 0 )
		{
			char * pA = new char[ansilen+1];
			::WideCharToMultiByte( CP_UTF8, NULL, pW, -1, pA, ansilen+1, NULL, NULL );
			m_str = pA;
			delete pA;
		}
		delete pW;
	}
	return *this;
}
CBaseStringU & CBaseStringU::operator=( const char* lpsz )
{
	this->m_str = lpsz;
	return *this;
}
CBaseStringU & CBaseStringU::operator=( const CBaseStringW & str )
{
	*this = str.c_str();
	return *this;
}
CBaseStringU & CBaseStringU::operator=( const wstring & str )
{
	*this = str.c_str();
	return *this;
}
CBaseStringU & CBaseStringU::operator=( const wchar_t* lpsz )
{
	int ansilen = ::WideCharToMultiByte( CP_UTF8, NULL, lpsz, -1, NULL, 0, NULL, NULL );
	if( ansilen > 0 )
	{
		char * p = new char[ansilen+1];
		::WideCharToMultiByte( CP_UTF8, NULL, lpsz, -1, p, ansilen+1, NULL, NULL );
		m_str = p;
		delete p;
	}
	return *this;
}
CBaseStringU & CBaseStringU::operator+=( const CBaseStringU & str )
{
	m_str += str.c_str();
	return *this;
}
CBaseStringU & CBaseStringU::operator+=( const string & str )
{
	CBaseStringU strU(str);
	m_str += strU.GetStr();
	return *this;
}
CBaseStringU & CBaseStringU::operator+=( const char* lpsz )
{
	m_str += lpsz;
	return *this;
}
bool CBaseStringU::operator<( const CBaseStringU & str ) const
{
	return m_str < str.m_str;
}
const string & CBaseStringU::GetStr() const
{
	return m_str;
}
const char* CBaseStringU::c_str() const
{
	return m_str.c_str();
}
int CBaseStringU::Find( const char* lpsz, int pos )
{
	int poss = m_str.find( lpsz, pos );
	return poss;
}
int CBaseStringU::Find( const string & str, int pos )
{
	int poss = m_str.find( str, pos );
	return poss;
}
int CBaseStringU::Find( const CBaseStringU & str, int pos)
{
	int poss = m_str.find( str.m_str, pos );
	return poss;
}
int CBaseStringU::Find( char ch, int pos )
{
	int poss = m_str.find( ch, pos );
	return pos;
}
int CBaseStringU::RFind( const char* lpsz )
{
	int poss = m_str.rfind( lpsz );
	return poss;
}
int CBaseStringU::RFind( const string & str )
{
	int poss = m_str.rfind( str );
	return poss;
}
int CBaseStringU::RFind( const CBaseStringU & str )
{
	int poss = m_str.rfind( str.m_str );
	return poss;
}
int CBaseStringU::RFind( char ch)
{
	int poss = m_str.rfind( ch );
	return poss;
}
CBaseStringU CBaseStringU::substr( int pos, string::size_type _Count ) const
{
	return m_str.substr( pos, _Count );
}
CBaseStringU CBaseStringU::Replace( int pos, int count, const char* lpsz )
{
	return m_str.replace( pos, count, lpsz ).c_str();
}

void CBaseStringU::Trim()
{
	TrimLeft();
	TrimRight();
}
void CBaseStringU::TrimLeft()
{
	size_t begin = 0;
	size_t len = m_str.length();
	while( begin < len )
	{
		if( m_str[begin] != ' ' )
			break;
		begin++;
	}
	m_str = m_str.substr( begin );
}
void CBaseStringU::TrimRight()
{
	int begin = m_str.length();
	while( begin > 0 )
	{
		if( m_str[begin-1] != ' ' )
			break;
		begin--;
	}
	m_str = m_str.substr( 0, begin );
}
void CBaseStringU::clear()
{
	m_str.clear();
}

