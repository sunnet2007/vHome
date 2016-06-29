#include "StdAfx.h"
#include "base_ini.h"


CBaseIni::CBaseIni(void)
{
}
CBaseIni::CBaseIni( LPCTSTR lpszFileName )
{
	m_strFileName = lpszFileName;
}
CBaseIni::~CBaseIni(void)
{
}
void   CBaseIni::Ini( LPCTSTR lpszFileName )
{
	m_strFileName = lpszFileName;
}
bool   CBaseIni::WriteStr( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszString/*, LPCTSTR lpszFileName = NULL*/ )
{
	return WritePrivateProfileString( lpszAppName, lpszKeyName, lpszString, m_strFileName.c_str() ) == TRUE;
}
bool CBaseIni::WriteStruct( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, void * pStruct, unsigned int nSizeStruct )
{
	return WritePrivateProfileStruct( lpszAppName, lpszKeyName, pStruct, nSizeStruct, m_strFileName.c_str() ) == TRUE;
}
bool   CBaseIni::WriteInt( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int nValue )
{
	TCHAR buf[32];
	_stprintf_s( buf, 32, _T("%d"), nValue );
	return WritePrivateProfileString( lpszAppName, lpszKeyName, buf, m_strFileName.c_str() ) == TRUE;
}
bool   CBaseIni::WriteInt64( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, __int64 nValue )
{
	TCHAR buf[48]={0};
	_stprintf_s( buf, 48, _T("%I64d"), nValue );
	return WritePrivateProfileString( lpszAppName, lpszKeyName, buf, m_strFileName.c_str() ) == TRUE;
}
unsigned int CBaseIni::ReadStr( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpDefault, CBaseString & strValue/*, LPCTSTR lpszFileName = NULL*/ )
{
	TCHAR buf[256];
	unsigned int nSize =  GetPrivateProfileString( lpszAppName, lpszKeyName, lpDefault, buf, 256, m_strFileName.c_str() );
	if( nSize > 0 )
	{
		strValue = buf;
	}
	return nSize;
}
int  CBaseIni::ReadInt( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int nDefault/*, LPCTSTR lpszFileName = NULL*/ )
{
	return GetPrivateProfileInt( lpszAppName, lpszKeyName, nDefault, m_strFileName.c_str() );
}
__int64  CBaseIni::ReadInt64( LPCTSTR lpszAppName, LPCTSTR lpszkeyName, __int64 nDefault )
{
	TCHAR buf[48];
	int nSize = GetPrivateProfileString( lpszAppName, lpszkeyName, _T("0"), buf, 48, m_strFileName.c_str() );
	return _tstoi64( buf );
}
bool CBaseIni::ReadStruct( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, void * pStruct, unsigned int nSizeStruct )
{
	return GetPrivateProfileStruct( lpszAppName, lpszKeyName, pStruct, nSizeStruct, m_strFileName.c_str() )==TRUE;
}
int  CBaseIni::GetAllSectionNames( TCHAR * pBuf, int nBuflen )
{
	return GetPrivateProfileSectionNames( pBuf, nBuflen, m_strFileName.c_str() );
}

