#include "StdAfx.h"
#include "MyTime.h"
#include <time.h>


CMyTime::CMyTime(void)
{
	m_time = 0;
}
CMyTime::~CMyTime(void)
{
}
CMyTime::CMyTime(__int64 t)
{
	m_time = t;
}
CMyTime::CMyTime( int nYear,int nMonth,int nDay,int nHour,int nMin,int nSec )
{
	tm t = {nSec,nMin,nHour,nDay,nMonth-1,nYear-1900};
	m_time = mktime( &t );
}

CMyTime::CMyTime( string str )
{
	m_time = 0;

	tm t;

	int iStart=0;
	int iEnd  =0;

	if ( str.empty() )
	{
		return;
	}

	do 
	{
		//年
		iEnd = str.find('-',iStart);
		if ( iEnd > iStart )
		{
			string tmp = str.substr(iStart,iEnd-iStart);
			t.tm_year = atoi( tmp.c_str() ) - 1900;
			iStart = iEnd +1;
		}
		else
		{
			break;
		}

		//月
		iEnd = str.find('-',iStart );
		if ( iEnd > iStart )
		{
			string tmp = str.substr(iStart,iEnd-iStart);
			t.tm_mon = atoi( tmp.c_str() ) -1;
			iStart = iEnd +1;
		}
		else
		{
			break;
		}

		//日
		iEnd = str.find(' ',iStart );
		if ( iEnd > iStart )
		{
			string tmp = str.substr(iStart,iEnd-iStart);
			t.tm_mday = atoi( tmp.c_str() ) ;
			iStart = iEnd +1;
		}
		else
		{
			break;
		}

		//时
		iEnd = str.find(':',iStart );
		if ( iEnd > iStart )
		{
			string tmp = str.substr(iStart,iEnd-iStart);
			t.tm_hour = atoi( tmp.c_str() ) ;
			iStart = iEnd +1;
		}
		else
		{
			break;
		}

		//分
		iEnd = str.find(':',iStart);
		if ( iEnd > iStart )
		{
			string tmp = str.substr(iStart,iEnd-iStart);
			t.tm_min = atoi( tmp.c_str() ) ;
			iStart = iEnd +1;
		}
		else
		{
			break;
		}

		//秒
		string tmp = str.substr(iStart);
		t.tm_sec = atoi( tmp.c_str() ) ;


		m_time = mktime( &t );

	} while (false);


	return ;
}

CMyTime CMyTime::GetCurrentTime()
{
	__int64 t = time(NULL);
	CMyTime mt( t );
	return mt;
}

string CMyTime::toString( bool bOnlyDay )
{
	tm *ptm = localtime( &m_time );
	char tmp[256]={0};
	if ( bOnlyDay )
	{
		sprintf( tmp,"%d-%d-%d",ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday);
	}
	else
	{
		sprintf( tmp,"%d-%d-%d %d:%d:%d",ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec );
	}
	
	string str = tmp;
	return str;
}

__int64 CMyTime::getTime()
{
	return m_time;
}
__int64 CMyTime::UpdateTime( int nYear,int nMonth,int nDay,int nHour,int nMin,int nSec )
{
	tm *ptm = localtime( &m_time );
	ptm->tm_year += nYear;
	ptm->tm_mon  += nMonth;
	ptm->tm_mday += nDay;
	ptm->tm_hour += nHour;
	ptm->tm_min  += nMin;
	ptm->tm_sec  += nSec;
	m_time = mktime( ptm );
	return m_time;
}

int CMyTime::GetYear()
{
	tm *ptm = localtime( &m_time );
	return ptm->tm_year+1900;
}
int CMyTime::GetMonth()
{
	tm *ptm = localtime( &m_time );
	return ptm->tm_mon+1;
}
int CMyTime::GetDay()
{
	tm *ptm = localtime( &m_time );
	return ptm->tm_mday;
}
int CMyTime::GetHour()
{
	tm *ptm = localtime( &m_time );
	return ptm->tm_hour;
}
int CMyTime::GetMinute()
{
	tm *ptm = localtime( &m_time );
	return ptm->tm_min;
}
int CMyTime::GetSecond()
{
	tm *ptm = localtime( &m_time );
	return ptm->tm_sec;
}
int CMyTime::GetDayOfWeek()
{
	tm *ptm = localtime( &m_time );
	return ptm->tm_wday;
}
