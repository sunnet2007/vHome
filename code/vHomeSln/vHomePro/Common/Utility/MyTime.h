#pragma once
#include <string>
using namespace std;


class  CMyTime
{
public:
	CMyTime( );
	~CMyTime(void);
	CMyTime(__int64 t);
	CMyTime(
		_In_ int nYear,
		_In_ int nMonth,
		_In_ int nDay,
		_In_ int nHour,
		_In_ int nMin,
		_In_ int nSec);
	CMyTime(string str);//"2014-12-17 14:45:36"

	static CMyTime GetCurrentTime();

	string  toString(bool bOnlyDay=false);
	__int64 getTime();
	__int64 UpdateTime(
		_In_ int nYear=0,
		_In_ int nMonth=0,
		_In_ int nDay=0,
		_In_ int nHour=0,
		_In_ int nMin=0,
		_In_ int nSec=0);

	int GetYear();
	int GetMonth();
	int GetDay();
	int GetHour();
	int GetMinute();
	int GetSecond();
	int GetDayOfWeek();/* days since Sunday - [0,6] */

private:
	__int64 m_time;
};
