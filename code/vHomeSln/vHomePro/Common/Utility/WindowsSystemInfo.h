#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <psapi.h>

struct _disk
{
	char letter[4];//盘符
	__int64 totalSize;//总大小
	__int64 freeSize;//剩余大小
};


class CWindowsSystemInfo
{
public:
	CWindowsSystemInfo(void);
	~CWindowsSystemInfo(void);

	long GetCPUFreq();//获取CPU频率,单位: MHZ
	std::string GetManID();//获取制造商信息
	std::string GetCPUType();//获取CPU型号
	int GetCPUUseRate();
	bool GetMemoryInfo(__int64 &totalSize,int &useRate);
	void GetDiskInfo( std::vector<_disk>&vec,bool bExcludeSystemDisk /*是否排除系统盘*/ );//返回所有磁盘信息，按剩余空间从大到小排序
	bool GetCurrentProcessMemoryInfo( PROCESS_MEMORY_COUNTERS &pmc );

private:
	void ExeCPUID(DWORD veax);//初始化CPU

};

 
class CCPUUseRate
{
public:
	BOOL Initialize() 
	{
		FILETIME ftIdle, ftKernel, ftUser;
		BOOL flag = FALSE;
		if (flag = GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
		{
			m_fOldCPUIdleTime = FileTimeToDouble(ftIdle);
			m_fOldCPUKernelTime = FileTimeToDouble(ftKernel);
			m_fOldCPUUserTime = FileTimeToDouble(ftUser);

		}
		return flag;
	}
	//调用Initialize后要等待1秒再调用此函数
	int GetCPUUseRate()
	{
		int nCPUUseRate = -1;
		FILETIME ftIdle, ftKernel, ftUser;
		if (GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
		{
			double fCPUIdleTime = FileTimeToDouble(ftIdle);
			double fCPUKernelTime = FileTimeToDouble(ftKernel);
			double fCPUUserTime = FileTimeToDouble(ftUser);
			nCPUUseRate= (int)(100.0 - (fCPUIdleTime - m_fOldCPUIdleTime) 
				/ (fCPUKernelTime - m_fOldCPUKernelTime + fCPUUserTime - m_fOldCPUUserTime) 
				*100.0);
			m_fOldCPUIdleTime = fCPUIdleTime;
			m_fOldCPUKernelTime = fCPUKernelTime;
			m_fOldCPUUserTime = fCPUUserTime;
		}
		return nCPUUseRate;
	}
private:
	double FileTimeToDouble(FILETIME &filetime)
	{
		return (double)(filetime.dwHighDateTime * 4.294967296E9) + (double)filetime.dwLowDateTime;
	}
private:
	double m_fOldCPUIdleTime;
	double m_fOldCPUKernelTime;
	double m_fOldCPUUserTime;
};