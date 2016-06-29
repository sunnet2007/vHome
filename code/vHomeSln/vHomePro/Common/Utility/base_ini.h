#pragma once
#include <Windows.h>
#include "base_string.h"

class  CBaseIni
{
public:
	CBaseIni();
	CBaseIni( LPCTSTR lpszFileName );
public:
	~CBaseIni(void);
public:
	void   Ini( LPCTSTR lpszFileName );
	bool   WriteStr( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszString/*, LPCTSTR lpszFileName = NULL*/ );
	bool   WriteStruct( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, void * pStruct, unsigned int nSizeStruct );
	bool   WriteInt( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int nValue );
	bool   WriteInt64( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, __int64 nValue );

	unsigned int ReadStr( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpDefault, CBaseString & strValue/*, LPCTSTR lpszFileName = NULL*/ );
	int  ReadInt( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int nDefault/*, LPCTSTR lpszFileName = NULL*/ );
	__int64  ReadInt64( LPCTSTR lpszAppName, LPCTSTR lpszkeyName, __int64 nDefault );
	bool   ReadStruct( LPCTSTR lpszAppName, LPCTSTR lpszKeyName, void * pStruct, unsigned int nSizeStruct );

	int    GetAllSectionNames( TCHAR * pBuf, int nBuflen );
public:
	CBaseString m_strFileName;
};
