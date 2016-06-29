//////////////////////////////////////////////////////////////////////////
//  <tchar.h>�ǿ��Ե����ó���ʹ�õģ�������ƽ̨�����ƣ����Ծ�ֱ�Ӱ����ˡ�
//  CUSTOM_TIME �� CUSTOM_PATH��Ϊ��ʹc���win api����һͬʹ�á�
//  FindFile�� FindNextFile ���÷����Բο�msdn��win api���÷���
//   ʹ�����ӿɲο�mfc��cfilefind��һ������

#pragma once
#include <Windows.h>
#include <tchar.h>

const int CUSTOM_PATH = 260;
const int CUSTOM_NAME = 128;


struct  CUSTOM_TIME
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};

class  CBase_FindFile
{
public:
	enum FileAtt
	{
		Arch      = 0x00000001,
		Hidden    = 0x00000002,
		Normal    =	0x00000004,
		ReadOnly  = 0x00000010,
		SubDir    = 0x00000020,
		System    = 0x00000040
	};

public:
	CBase_FindFile();
	virtual ~CBase_FindFile();
	
	unsigned __int64 GetFileSize() const;
	bool GetFileName(TCHAR* lpstrFileName, unsigned int cchLength) const;
	bool GetFilePath(TCHAR* lpstrFilePath, unsigned int cchLength) const;
	bool GetFileTitle(TCHAR* lpstrFileTitle, unsigned int cchLength) const;
	bool GetFileURL(TCHAR* lpstrFileURL, unsigned int cchLength) const;
	bool GetRoot(TCHAR* lpstrRoot, unsigned int cchLength) const;

	bool GetLastWriteTime(CUSTOM_TIME& TimeStamp) const;
	bool GetLastAccessTime(CUSTOM_TIME& TimeStamp) const;
	bool GetCreationTime(CUSTOM_TIME& TimeStamp) const;

	bool MatchesMask(FileAtt dwMask) const;
	bool IsDots() const;
	bool IsReadOnly() const;
	bool IsDirectory() const;
	bool IsSystem() const;
	bool IsHidden() const;
	bool IsNormal() const;
	bool IsArchived() const;

	bool FindFile( const TCHAR* pstrName = NULL, const bool bFloder = false );
	bool FindNextFile();
	void Close();

private:
#ifdef WIN32
	WIN32_FIND_DATA m_fd;
	HANDLE m_hFind;
#else
	_tfinddata_t m_fd;
	/*intptr_t*/  int m_hFind;
#endif
	TCHAR m_lpszRoot[CUSTOM_PATH];
	TCHAR m_chDirSeparator;
	bool m_bFound;
};

