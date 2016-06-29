#include "StdAfx.h"
#include "WindowsSystemInfo.h"
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"psapi.lib")
using namespace std;


static __int64 CompareFileTime ( FILETIME time1, FILETIME time2 )
{
	__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime ;
	__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime ;

	return (b - a);
}

DWORD deax;
DWORD debx;
DWORD decx;
DWORD dedx;


CWindowsSystemInfo::CWindowsSystemInfo(void)
{
}


CWindowsSystemInfo::~CWindowsSystemInfo(void)
{
}

long CWindowsSystemInfo::GetCPUFreq()
{
	int start1,start2;
	_asm rdtsc
	_asm mov start1,eax
	Sleep(50);
	_asm rdtsc
	_asm mov start2,eax
	return ((start2-start1)/50)/(1024);
}

string CWindowsSystemInfo::GetManID()
{
	char ID[25];
	memset(ID,0,sizeof(ID));
	ExeCPUID(0);//初始化
	memcpy(ID+0,&debx,4);//制造商信息前四个字符复制到数组
	memcpy(ID+4,&dedx,4);//中间四个
	memcpy(ID+8,&decx,4);//最后四个

	string str;
	str.assign( ID,16);
	 
	return str;
}

string CWindowsSystemInfo::GetCPUType()
{
	const DWORD id = 0x80000002; //从0x80000002开始,到0x80000004结束
	char CPUType[49];
	memset(CPUType,0,sizeof(CPUType));

	for(DWORD t = 0 ; t < 3 ; t++ )
	{
		ExeCPUID(id+t);

		memcpy(CPUType+16*t+ 0,&deax,4);
		memcpy(CPUType+16*t+ 4,&debx,4);
		memcpy(CPUType+16*t+ 8,&decx,4);
		memcpy(CPUType+16*t+12,&dedx,4);
	}

	return CPUType;
}

int CWindowsSystemInfo::GetCPUUseRate()
{
	HANDLE hEvent;
	BOOL res ;

	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;

	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;

	res = GetSystemTimes( &idleTime, &kernelTime, &userTime );

	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime ;

	hEvent = CreateEvent (NULL,FALSE,FALSE,NULL); // 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled

	WaitForSingleObject( hEvent,1000 ); //等待500毫秒
	res = GetSystemTimes( &idleTime, &kernelTime, &userTime );

	int idle = CompareFileTime( preidleTime,idleTime);
	int kernel = CompareFileTime( prekernelTime, kernelTime);
	int user = CompareFileTime(preuserTime, userTime);

	int cpu = (kernel +user - idle) *100/(kernel+user);
	int cpuidle = ( idle) *100/(kernel+user);

	return cpu;
}


typedef BOOL (__stdcall*g_pGMemoryStatus)(LPMEMORYSTATUSEX); 
bool CWindowsSystemInfo::GetMemoryInfo( __int64 &totalSize,int &useRate )
{
	HINSTANCE hIL = LoadLibrary( _T("kernel32.dll") ); 
	g_pGMemoryStatus pGMemoryStatus = (BOOL(__stdcall*)(LPMEMORYSTATUSEX))GetProcAddress(hIL,"GlobalMemoryStatusEx" ); 
	if ( pGMemoryStatus )
	{
		MEMORYSTATUSEX m;
		memset(&m,sizeof(m),0);
		m.dwLength = sizeof(m); 
		pGMemoryStatus( &m );
		totalSize = m.ullTotalPhys;
		useRate = m.dwMemoryLoad;
		return true;
	}
	return false;
}


void CWindowsSystemInfo::GetDiskInfo( std::vector<_disk>&vec,bool bExcludeSystemDisk )
{
	int len = GetLogicalDriveStringsA(0,NULL);
	char* pStr = new char[len];
	GetLogicalDriveStringsA(len,pStr);


	int i = 0;
	if ( bExcludeSystemDisk )
	{
		i += 4;
	}
	for(;i<len;i+=4)
	{
		_disk disk = {0};
		
		strncpy( disk.letter,pStr + i,3 );
		if ( GetDriveTypeA(pStr+i) != DRIVE_FIXED )
		{
			continue;
		}

		unsigned __int64 i64FreeBytesToCaller;
		BOOL fResult = GetDiskFreeSpaceExA (
			disk.letter,
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&disk.totalSize,
			(PULARGE_INTEGER)&disk.freeSize);

		if(fResult)//判断驱动器是否在工作状态
		{
			BOOL bFind = false;
			vector<_disk>::iterator it = vec.begin();
			for ( ;it != vec.end();it++ )
			{
				if ( disk.freeSize >= it->freeSize )
				{
					vec.insert( it,disk);
					bFind = TRUE;
					break;
				}
			}

			if ( !bFind )
			{
				vec.push_back( disk );
			}
		}
		else
		{

		}
	}
	delete pStr;
}

bool CWindowsSystemInfo::GetCurrentProcessMemoryInfo( PROCESS_MEMORY_COUNTERS &pmc)
{
	return GetProcessMemoryInfo( GetCurrentProcess(),&pmc,sizeof(pmc));
	//EmptyWorkingSet(GetCurrentProcess());
}
//////////////////////////////////////////////////////////////////////////
void CWindowsSystemInfo::ExeCPUID(DWORD veax)
{
	__asm
	{
		mov eax,veax
			cpuid
			mov deax,eax
			mov debx,ebx
			mov decx,ecx
			mov dedx,edx
	}
}