
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#if (defined(_WIN32) || defined(_WIN64))
#include <io.h>
#include <windows.h>
#define W_OK 02
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#endif

#include "Log.h"

CLog LOG;

CLog::CLog(const char * filename)
{
    m_fileindex = 0;
	m_level = LOG_DEBUG;
	m_lastWriteTime = 0;
	m_bStop = false;
    Open(filename);
}

CLog::CLog()
{
	m_fp = stdout;
    m_fileindex = 0;
	m_level = LOG_DEBUG;
	m_lastWriteTime = 0;
	m_bStop = false;
}

CLog::~CLog()
{
	Close();
}
int CLog::Open(const char * filename,int maxFileSize,int maxFileIndex) 
{
	m_maxFileSize = maxFileSize;
	m_maxFileIndex= maxFileIndex;
	if(filename == NULL)
	{
		m_fp = stdout;
	}
	else
	{
		m_filePath = filename;
		m_fp = NULL;
		_open();
	}

	writeThread.start( this,NULL);
	return 0;
}
void CLog::Close()
{
	m_bStop = true;
	writeThread.wait_exit();
	_close();
}
int CLog::SetLogLevel( int level)
{
	m_level = level;
	return 0;
}
int CLog::Log(const char * format,...) 
{
	if(format[0] == '\0')
		return 0;

	CAutoLock lock( m_lock );
	if ( m_logbuffer.size() >= 16777216)
	{
		return -1;
	}

	char tmp[10241] = { 0 };
	sprintf(tmp,"[%s] ",GetDateTime().c_str() );
	int  len = strlen( tmp );

	va_list args;
	va_start(args, format);
	vsnprintf(tmp+len, sizeof(tmp)-1-len, format, args);
	len = strlen( tmp );
	if(tmp[len-1] != '\n')
		tmp[len] = '\n';
	va_end(args);


	m_logbuffer += tmp;

	return 0;
}

int CLog::LogWarning(const char * format,...) 
{
	if(format[0] == '\0' || m_level > LOG_WARN )
		return 0;

	CAutoLock lock( m_lock );
	if ( m_logbuffer.size() >= 16777216)
	{
		return -1;
	}

	char tmp[10241] = { 0 };
	sprintf(tmp,"[%s] Warn:",GetDateTime().c_str() );
	int  len = strlen( tmp );

	va_list args;
	va_start(args, format);
	vsnprintf(tmp+len, sizeof(tmp)-1-len, format, args);
	len = strlen( tmp );
	if(tmp[len-1] != '\n')
		tmp[len] = '\n';
	va_end(args);


	m_logbuffer += tmp;

	return 0;
}

int CLog::LogError(const char * format,...) 
{
	if(format[0] == '\0' || m_level > LOG_ERROR )
		return 0;

	CAutoLock lock( m_lock );
	if ( m_logbuffer.size() >= 16777216)
	{
		return -1;
	}

	char tmp[10241] = { 0 };
	sprintf(tmp,"[%s] Error:",GetDateTime().c_str() );
	int  len = strlen( tmp );

	va_list args;
	va_start(args, format);
	vsnprintf(tmp+len, sizeof(tmp)-1-len, format, args);
	len = strlen( tmp );
	if(tmp[len-1] != '\n')
		tmp[len] = '\n';
	va_end(args);


	m_logbuffer += tmp;

	return 0;
}

int CLog::LogDebug(const char * format,...)
{
	if(format[0] == '\0' || m_level > LOG_DEBUG )
		return 0;

	CAutoLock lock( m_lock );
	if ( m_logbuffer.size() >= 16777216)
	{
		return -1;
	}

	char tmp[10241] = { 0 };
	sprintf(tmp,"[%s] Debug:",GetDateTime().c_str() );
	int  len = strlen( tmp );

	va_list args;
	va_start(args, format);
	vsnprintf(tmp+len, sizeof(tmp)-1-len, format, args);
	len = strlen( tmp );
	if(tmp[len-1] != '\n')
		tmp[len] = '\n';
	va_end(args);


	m_logbuffer += tmp;


	return 0;
}

string CLog::GetDateTime() 
{
	char date_time[100] = { 0 };

#if (defined(_WIN32) || defined(_WIN64))
	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	sprintf(date_time, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
		int(CurTime.wYear), int(CurTime.wMonth), int(CurTime.wDay), int(CurTime.wHour), int(CurTime.wMinute), int(CurTime.wSecond), int(CurTime.wMilliseconds));
#else
	struct tm ptm;
    struct timeval tmstart;
	//time_t second = time(NULL);
	//ptm = localtime(((const time_t*)&second));
	//localtime_r(((const time_t*)&second), &ptm);
    gettimeofday(&tmstart, NULL);
    localtime_r(&(tmstart.tv_sec), &ptm);

	sprintf(date_time, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
		int(ptm.tm_year+1900), int(ptm.tm_mon+1), int(ptm.tm_mday), int(ptm.tm_hour), int(ptm.tm_min), int(ptm.tm_sec), int(tmstart.tv_usec));
#endif
	return date_time;
}

void CLog::run(CThread *thread, void *arg)
{
	bool reset = false;
	while(1)
	{
		if ( m_bStop )
		{
			break;
		}

		bool b = false;
		string tmp;

		m_lock.lock();
		if ( m_logbuffer.size() >= 1024 || ( time(NULL) - m_lastWriteTime) > 2 )
		{
			//大于1k或超过2秒
			b = true;
			tmp = m_logbuffer;
			m_logbuffer.clear();
		}
		m_lock.unlock();

		if ( b )
		{
			if(m_fp == stdout)
			{
				printf( "%s",tmp.c_str() );
			}
			else
			{
				if ( m_writenBytes >= m_maxFileSize )
				{
					m_fileindex++;
					if ( m_fileindex >= m_maxFileIndex )
					{
						reset = true;
						m_fileindex = 0;
					}
					m_writenBytes = 0;
					_open( reset );//重新打开一个文件
				}

				fprintf(m_fp,"%s",tmp.c_str());
				fflush(m_fp);

				m_writenBytes += tmp.size();

			}
			m_lastWriteTime = time( NULL );
		}
		else
		{
			Sleep(100);
		}

	}
}

void CLog::_open( bool reset ) 
{
	_close();

	char path[256] = {0};
	sprintf(path,"%s_%d.log",m_filePath.c_str(),m_fileindex );

	if ( reset )
		m_fp = fopen(path, "w");
	else
		m_fp = fopen(path, "a");

	if(m_fp == NULL) 
	{
		fprintf(stderr, "Open LOG %s failed.", path);
		m_fp = stdout;
	}
	fseek(m_fp, 0, SEEK_END);
	m_writenBytes = ftell(m_fp);
	return ;
}

void CLog::_close() 
{
	if(m_fp != NULL && m_fp != stdout)
	{
		fclose(m_fp);
	}
	m_fp = stdout;

	return ;
}