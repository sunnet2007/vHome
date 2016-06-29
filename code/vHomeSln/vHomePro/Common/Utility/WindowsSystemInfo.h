#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <psapi.h>

struct _disk
{
	char letter[4];//�̷�
	__int64 totalSize;//�ܴ�С
	__int64 freeSize;//ʣ���С
};


class CWindowsSystemInfo
{
public:
	CWindowsSystemInfo(void);
	~CWindowsSystemInfo(void);

	long GetCPUFreq();//��ȡCPUƵ��,��λ: MHZ
	std::string GetManID();//��ȡ��������Ϣ
	std::string GetCPUType();//��ȡCPU�ͺ�
	int GetCPUUseRate();
	bool GetMemoryInfo(__int64 &totalSize,int &useRate);
	void GetDiskInfo( std::vector<_disk>&vec,bool bExcludeSystemDisk /*�Ƿ��ų�ϵͳ��*/ );//�������д�����Ϣ����ʣ��ռ�Ӵ�С����
	bool GetCurrentProcessMemoryInfo( PROCESS_MEMORY_COUNTERS &pmc );

private:
	void ExeCPUID(DWORD veax);//��ʼ��CPU

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
	//����Initialize��Ҫ�ȴ�1���ٵ��ô˺���
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