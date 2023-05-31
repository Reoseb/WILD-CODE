// CPUUsage.h: interface for the CCPUUsage class.
//
// created by Unwinder
/////////////////////////////////////////////////////////////////////////////
#pragma once
/////////////////////////////////////////////////////////////////////////////
#define SystemProcessorPerformanceInformation 8
/////////////////////////////////////////////////////////////////////////////
typedef HRESULT (WINAPI *NTQUERYSYSTEMINFORMATION)(UINT, PVOID, ULONG, PULONG);
/////////////////////////////////////////////////////////////////////////////
typedef struct SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION 
{
    LARGE_INTEGER	IdleTime;
    LARGE_INTEGER	KernelTime;
    LARGE_INTEGER	UserTime;
    LARGE_INTEGER	Reserved1[2];
    ULONG			Reserved2;
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;
/////////////////////////////////////////////////////////////////////////////
#define MAX_CPU 256
//////////////////////////////////////////////////////////////////////
class CCPUUsage
{
public:
	CCPUUsage();
	virtual ~CCPUUsage();

	void	Init();

	DWORD	GetCpuCount();
	float	GetCpuUsage(DWORD dwCpu);
	float	GetTotalCpuUsage();
	float	GetMaxCpuUsage();

protected:
	DWORD	m_dwNumberOfProcessors;

	NTQUERYSYSTEMINFORMATION m_lpNtQuerySystemInformation;

	DWORD	m_dwTickCount[MAX_CPU];
	LARGE_INTEGER m_idleTime[MAX_CPU];
	float	m_fltCpuUsage[MAX_CPU];
};
/////////////////////////////////////////////////////////////////////////////
