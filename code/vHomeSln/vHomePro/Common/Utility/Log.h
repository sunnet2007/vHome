#ifndef _LOG_FILE_H
#define _LOG_FILE_H

#if (defined(_WIN32) || defined(_WIN64))

#include <windows.h>

#define snprintf   _snprintf
#define vsnprintf  _vsnprintf
#define strcasecmp stricmp
#define strcasestr stristr

#pragma warning(disable : 4786)

#else

#define int64 long long

#endif

#include <iostream>
#include <string>
using namespace std;

#include "Lock.h"
#include <time.h>
#include "thread.h"

class CLog :public Runnable
{
public:
	CLog(const char * filename);
	CLog();
	~CLog();

	enum
	{
		LOG_ALL = 0,
		LOG_DEBUG,
		LOG_WARN,
		LOG_ERROR
	};

	//文件路径不带后缀
	int Open(const char * filename,int maxFileSize = 16777216/*16M*/,int maxFileIndex =10);
	void Close();
	int SetLogLevel( int level);
	int Log(const char * format, ...);
	int LogWarning(const char * format, ...);
	int LogError(const char * format, ...);
	int LogDebug(const char * format, ...);

	void run(CThread *thread, void *arg);
private:
	string GetDateTime();
	void _open(bool reset=false);
	void _close();

	CLock m_lock;
	string m_filePath;//不带后缀
	string m_logbuffer;
	CThread	writeThread;//写线程
	bool m_bStop;
	FILE * m_fp;
	int	 m_level;
	int	 m_lastWriteTime;
	int  m_fileindex;
	int  m_maxFileIndex;
	int	 m_maxFileSize;
	int  m_writenBytes;
};
extern CLog LOG;
#endif

