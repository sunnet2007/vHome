#pragma warning(disable : 4996)

#include "stdafx.h"
#include <stdlib.h>
#include <assert.h>
#include "base_FindFile.h"


CBase_FindFile::CBase_FindFile()
{
#ifdef WIN32
	m_hFind = NULL;
#else
	m_hFind = -1;
#endif
	m_chDirSeparator = _T('\\');
	m_bFound = false;
}

CBase_FindFile::~CBase_FindFile()
{
	Close();
}

unsigned __int64 CBase_FindFile::GetFileSize() const
{
	
#ifdef WIN32
	assert(NULL != m_hFind);
	if (m_bFound)
	{
		unsigned __int64 nFileSize = 0;
		nFileSize = m_fd.nFileSizeHigh;
		nFileSize = nFileSize<<32;
		nFileSize += m_fd.nFileSizeLow;

		return nFileSize;
	}
#else
	assert(-1 != m_hFind);
	if (m_bFound)
	{
		unsigned __int64 nFileSize = 0;
		nFileSize = m_fd.nFileSizeHigh;
		nFileSize = nFileSize<<32 + m_fd.nFileSizeLow;

		return nFileSize;
	}
	
#endif
	return 0;
}

bool CBase_FindFile::GetFileName(TCHAR* lpstrFileName, unsigned int cchLength)const
{
	
#ifdef WIN32
	assert(NULL != m_hFind);
	if(_tcslen(m_fd.cFileName) >= cchLength)
		return false;

	return (m_bFound && (_tcscpy(lpstrFileName, m_fd.cFileName) != NULL));
#else
	assert(-1 != m_hFind);
	if(_tcslen(m_fd.name) >= cchLength)
		return false;

	return (m_bFound && (_tcscpy(lpstrFileName, m_fd.name) != NULL));
#endif

}

bool CBase_FindFile::GetFilePath(TCHAR* lpstrFilePath, unsigned int cchLength)const
{
#ifdef WIN32
	assert(NULL != m_hFind);
#else
	assert(-1 != m_hFind);
#endif

	size_t Len = _tcslen(m_lpszRoot);
	if(0 == Len)return false;

	bool bAddSep = (m_lpszRoot[Len -1] != _T('\\') && m_lpszRoot[Len -1] != _T('/'));
	if((_tcslen(m_lpszRoot) + (bAddSep ?  1 : 0)) >= cchLength)
		return false;

	bool bRet =(_tcscpy(lpstrFilePath, m_lpszRoot) != NULL);

	if (bRet)
	{
		if (bAddSep)
		{
			TCHAR szSeparator[2] = {m_chDirSeparator, 0};
			bRet = (_tcscat(lpstrFilePath, szSeparator) != NULL);
		}
		if (bRet)
		{
#ifdef WIN32
			bRet = (_tcscat(lpstrFilePath, m_fd.cFileName) != NULL);
#else
			bRet = (_tcscat(lpstrFilePath, m_fd.name) != NULL);
#endif
		}
	}

	return bRet;
}

bool CBase_FindFile::GetFileTitle(TCHAR* lpstrFileTitle, unsigned int cchLength)const
{
#ifdef WIN32
	assert(NULL != m_hFind);
#else
	assert(-1 != m_hFind);
#endif

	TCHAR szBuffer[CUSTOM_PATH] = { 0 };
	if(!GetFileName(szBuffer, CUSTOM_PATH))
		return false;

	TCHAR szName[CUSTOM_NAME] = { 0 };
	
	_tsplitpath(szBuffer, NULL, NULL, szName, NULL);

	if(_tcslen(szName) >= cchLength)
		return false;

	return(_tcscpy(lpstrFileTitle, szName) != NULL);
}

bool CBase_FindFile::GetFileURL(TCHAR* lpstrFileURL, unsigned int cchLength)const
{
#ifdef WIN32
	assert(NULL != m_hFind);
#else
	assert(-1 != m_hFind);
#endif

	TCHAR szBuffer[CUSTOM_PATH] = { 0 };
	if(!GetFilePath(szBuffer, CUSTOM_PATH))
		return false;

	const TCHAR* lpstrFileURLPrefix = _T("file://");
	if(_tcslen(szBuffer) + _tcslen(lpstrFileURLPrefix) >= cchLength)
		return false;

	if(_tcscpy(lpstrFileURL, lpstrFileURLPrefix) == NULL)
		return false;

	return (_tcscat(lpstrFileURL, szBuffer) != NULL);
}

bool CBase_FindFile::GetRoot(TCHAR* lpstrRoot, unsigned int cchLength)const
{
#ifdef WIN32
	assert(NULL != m_hFind);
#else
	assert(-1 != m_hFind);
#endif

	if(_tcslen(m_lpszRoot) >= cchLength)
		return false;

	return (_tcscpy(lpstrRoot, m_lpszRoot) != NULL);
}


bool CBase_FindFile::GetLastWriteTime(CUSTOM_TIME& TimeStamp)const
{
#ifdef WIN32
	assert(NULL != m_hFind);
#else
	assert(-1 != m_hFind);
#endif

	if(!m_bFound)
		return false;

#ifdef WIN32
	FILETIME tmpFileTime = { 0 };
	SYSTEMTIME tmpSysTime = { 0 };
	if(!::FileTimeToLocalFileTime(&m_fd.ftLastWriteTime, &tmpFileTime))
		return false;
	if(!::FileTimeToSystemTime(&tmpFileTime, &tmpSysTime))
		return false;

	TimeStamp.year = tmpSysTime.wYear;
	TimeStamp.month = tmpSysTime.wMonth;
	TimeStamp.day = tmpSysTime.wDay;
	TimeStamp.hour = tmpSysTime.wHour;
	TimeStamp.minute = tmpSysTime.wMinute;
	TimeStamp.second = tmpSysTime.wSecond;
#else
	tm *tmptm = localtime(&m_fd.time_write);
	TimeStamp.year = tmptm->tm_year + 1900;
	TimeStamp.month = tmptm->tm_mon +1 ;
	TimeStamp.day = tmptm->tm_mday;
	TimeStamp.hour = tmptm->tm_hour;
	TimeStamp.minute = tmptm->tm_min;
	TimeStamp.second = tmptm->tm_sec;
#endif
	
	return true;
}

bool CBase_FindFile::GetLastAccessTime(CUSTOM_TIME& TimeStamp)const
{
#ifdef WIN32
	assert(NULL != m_hFind);
#else
	assert(-1 != m_hFind);
#endif

	if(!m_bFound)
		return false;

#ifdef WIN32
	FILETIME tmpFileTime = { 0 };
	SYSTEMTIME tmpSysTime = { 0 };
	if(!::FileTimeToLocalFileTime(&m_fd.ftLastAccessTime, &tmpFileTime))
		return false;
	if(!::FileTimeToSystemTime(&tmpFileTime, &tmpSysTime))
		return false;

	TimeStamp.year = tmpSysTime.wYear;
	TimeStamp.month = tmpSysTime.wMonth;
	TimeStamp.day = tmpSysTime.wDay;
	TimeStamp.hour = tmpSysTime.wHour;
	TimeStamp.minute = tmpSysTime.wMinute;
	TimeStamp.second = tmpSysTime.wSecond;
#else
	tm *tmptm = localtime(&m_fd.time_access);
	TimeStamp.year = tmptm->tm_year + 1900;
	TimeStamp.month = tmptm->tm_mon + 1;
	TimeStamp.day = tmptm->tm_mday;
	TimeStamp.hour = tmptm->tm_hour;
	TimeStamp.minute = tmptm->tm_min;
	TimeStamp.second = tmptm->tm_sec;
#endif

	return true;
}

bool CBase_FindFile::GetCreationTime(CUSTOM_TIME& TimeStamp) const
{
#ifdef WIN32
	assert(NULL != m_hFind);
#else
	assert(-1 != m_hFind);
#endif

	if(!m_bFound)
		return false;

#ifdef WIN32
	FILETIME tmpFileTime = { 0 };
	SYSTEMTIME tmpSysTime = { 0 };
	if(!::FileTimeToLocalFileTime(&m_fd.ftCreationTime, &tmpFileTime))
		return false;
	if(!::FileTimeToSystemTime(&tmpFileTime, &tmpSysTime))
		return false;

	TimeStamp.year = tmpSysTime.wYear;
	TimeStamp.month = tmpSysTime.wMonth;
	TimeStamp.day = tmpSysTime.wDay;
	TimeStamp.hour = tmpSysTime.wHour;
	TimeStamp.minute = tmpSysTime.wMinute;
	TimeStamp.second = tmpSysTime.wSecond;
#else
	tm *tmptm = localtime(&m_fd.time_create);
	TimeStamp.year = tmptm->tm_year + 1900;
	TimeStamp.month = tmptm->tm_mon + 1;
	TimeStamp.day = tmptm->tm_mday;
	TimeStamp.hour = tmptm->tm_hour;
	TimeStamp.minute = tmptm->tm_min;
	TimeStamp.second = tmptm->tm_sec;
#endif
	
	return true;
}


bool CBase_FindFile::MatchesMask(FileAtt dwMask) const
{
#ifdef WIN32
	assert(NULL != m_hFind);
#else
	assert(-1 != m_hFind);
#endif

	if(!m_bFound)
		return false;

#ifdef WIN32
	DWORD dwAttrib = 0;
	if(dwMask & Arch)    dwAttrib |= FILE_ATTRIBUTE_ARCHIVE;
	if(dwMask & Hidden)  dwAttrib |= FILE_ATTRIBUTE_HIDDEN;
	if(dwMask & Normal)  dwAttrib |= FILE_ATTRIBUTE_NORMAL;
	if(dwMask & ReadOnly)dwAttrib |= FILE_ATTRIBUTE_READONLY;
	if(dwMask & SubDir)  dwAttrib |= FILE_ATTRIBUTE_DIRECTORY;
	if(dwMask & System)  dwAttrib |= FILE_ATTRIBUTE_SYSTEM;

	return ((m_fd.dwFileAttributes & dwAttrib) != 0);
#else
	unsigned uAttrib = 0;
	if(dwMask & Arch)    uAttrib |= _A_ARCH;
	if(dwMask & Hidden)  uAttrib |= _A_HIDDEN;
	if(dwMask & Normal)  uAttrib |= _A_NORMAL;
	if(dwMask & ReadOnly)uAttrib |= _A_RDONLY;
	if(dwMask & SubDir)  uAttrib |= _A_SUBDIR;
	if(dwMask & System)  uAttrib |= _A_SYSTEM;
	
	return ((m_fd.attrib & uAttrib) != 0);
#endif
}

bool CBase_FindFile::IsDots() const
{	
#ifdef WIN32
	assert(NULL != m_hFind);
#else
	assert(-1 != m_hFind);
#endif

	if (m_bFound && IsDirectory())
	{
#ifdef WIN32
		if(m_fd.cFileName[0] == _T('.') && (m_fd.cFileName[1] == _T('\0') || (m_fd.cFileName[1] == _T('.') && m_fd.cFileName[2] == _T('\0'))))
			return true;
#else
	if(m_fd.name[0] == _T('.') && (m_fd.name[1] == _T('\0') || (m_fd.name[1] == _T('.') && m_fd.name[2] == _T('\0'))))
		return true;
#endif
	}

	return false;
}

bool CBase_FindFile::IsReadOnly() const
{
	return MatchesMask(ReadOnly);
}

bool CBase_FindFile::IsDirectory() const
{
	return MatchesMask(SubDir);
}

bool CBase_FindFile::IsSystem() const
{
	return MatchesMask(System);
}

bool CBase_FindFile::IsHidden() const
{
	return MatchesMask(Hidden);
}

bool CBase_FindFile::IsNormal() const
{
	return MatchesMask(Normal);
}

bool CBase_FindFile::IsArchived() const
{
	return MatchesMask(Arch);
}

bool CBase_FindFile::FindFile( const TCHAR* pstrName /*= NULL*/, const bool bFloder /*= false */)
{
	Close();
	if(NULL == pstrName)
		pstrName = _T("*.*");

#ifdef WIN32
	_tcscpy(m_fd.cFileName, pstrName);
	if( !bFloder )
		m_hFind = ::FindFirstFile(pstrName, &m_fd);
	else
		m_hFind = ::FindFirstFileEx( pstrName,FindExInfoStandard, &m_fd, FindExSearchLimitToDirectories, NULL, 0 );


	if(INVALID_HANDLE_VALUE == m_hFind)
		return false;
#else
	_tcscpy(m_fd.name, pstrName);
	m_hFind = _tfindfirst(pstrName, &m_fd);

	if(-1 == m_hFind)
		return false;
#endif
	const TCHAR* pstr = _tfullpath(m_lpszRoot, pstrName, CUSTOM_PATH);

	if(NULL == pstr)
	{
		Close();
		return false;
	}
	else
	{
		TCHAR* pstr1  = _tcsrchr(m_lpszRoot, _T('\\'));
		TCHAR* pstr2 = _tcsrchr(m_lpszRoot, _T('/'));
										
		if(NULL != pstr1 || NULL != pstr2)
		{
			if(NULL == pstr1)
				pstr1 = m_lpszRoot;
			if(NULL == pstr2)
				pstr2 = m_lpszRoot;

			if(pstr1 >= pstr2)
				*pstr1 = _T('\0');
			else
				*pstr2 = _T('\0');
		}
	}

	m_bFound = true;
	return true;

}

bool CBase_FindFile::FindNextFile()
{
	if(NULL == m_hFind || !m_bFound)
		return false;

#ifdef WIN32
	 if(::FindNextFile(m_hFind, &m_fd))
		 m_bFound = true;
	 else
		 m_bFound = false;
#else
	int res = _tfindnext(m_hFind, &m_fd);
	if(0 == res)
		m_bFound = true;
	else
		m_bFound = false;
#endif

	return m_bFound;
}

void CBase_FindFile::Close()
{
	m_bFound = false;
#ifdef WIN32
	if(NULL != m_hFind && INVALID_HANDLE_VALUE != m_hFind)
	{
		::FindClose(m_hFind);
		m_hFind = NULL;
	}
#else
	if(-1 != m_hFind)
	{
		_findclose(m_hFind);
		m_hFind = -1;
	}
#endif
}
