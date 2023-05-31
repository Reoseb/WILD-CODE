// CPUUsage.cpp : implementation file
//
// created by Unwinder
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "CPUUsage.h"

#include <float.h>
/////////////////////////////////////////////////////////////////////////////
CCPUUsage::CCPUUsage()
{
	m_dwNumberOfProcessors			= 0;

	m_lpNtQuerySystemInformation	= NULL;

	for (DWORD dwCpu=0; dwCpu<MAX_CPU; dwCpu++)
	{
		m_dwTickCount[dwCpu]		= 0;
		m_idleTime[dwCpu].QuadPart	= 0;
		m_fltCpuUsage[dwCpu]		= FLT_MAX;
	}
}
/////////////////////////////////////////////////////////////////////////////
CCPUUsage::~CCPUUsage()
{
}
/////////////////////////////////////////////////////////////////////////////
void CCPUUsage::Init()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info); 

	m_dwNumberOfProcessors			= info.dwNumberOfProcessors;
	m_lpNtQuerySystemInformation	= (NTQUERYSYSTEMINFORMATION)GetProcAddress(GetModuleHandle("NTDLL"), "NtQuerySystemInformation");
}
/////////////////////////////////////////////////////////////////////////////
DWORD CCPUUsage::GetCpuCount()
{
	return m_dwNumberOfProcessors;
}
/////////////////////////////////////////////////////////////////////////////
float CCPUUsage::GetCpuUsage(DWORD dwCpu)
{
	//validate NtQuerySystemInformation pointer and return FLT_MAX on error

	if (!m_lpNtQuerySystemInformation)
		return FLT_MAX;

	//validate specified CPU index and return FLT_MAX on error

	if (dwCpu >=  m_dwNumberOfProcessors)
		return FLT_MAX;

	DWORD dwTickCount = GetTickCount();
		//get standard timer tick count

	if (dwTickCount - m_dwTickCount[dwCpu] >= 100)
		//update usage once per 100 ms
	{
		SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION info[MAX_CPU];

		if (SUCCEEDED(m_lpNtQuerySystemInformation(SystemProcessorPerformanceInformation, &info, sizeof(info), NULL)))
				//query CPU usage
		{
			if (m_idleTime[dwCpu].QuadPart)
				//ensure that this function was already called at least once
				//and we have the previous idle time value
			{
				m_fltCpuUsage[dwCpu] = 100.0f - 0.01f * (info[dwCpu].IdleTime.QuadPart - m_idleTime[dwCpu].QuadPart) / (dwTickCount - m_dwTickCount[dwCpu]);
					//calculate new CPU usage value by estimating amount of time
					//CPU was in idle during the last second
					
				//clip calculated CPU usage to [0-100] range to filter calculation non-ideality

				if (m_fltCpuUsage[dwCpu] < 0.0f)
					m_fltCpuUsage[dwCpu] = 0.0f;

				if (m_fltCpuUsage[dwCpu] > 100.0f)
					m_fltCpuUsage[dwCpu] = 100.0f;
			}

			m_idleTime[dwCpu]		= info[dwCpu].IdleTime;
				//save new idle time for specified CPU
			m_dwTickCount[dwCpu]	= dwTickCount;
				//save new tick count for specified CPU
		}
	}

	return m_fltCpuUsage[dwCpu];
}
/////////////////////////////////////////////////////////////////////////////
float CCPUUsage::GetTotalCpuUsage()
{
	float result = FLT_MAX;

	DWORD dwCpuCount = GetCpuCount();

	for (DWORD dwCpu=0; dwCpu<dwCpuCount; dwCpu++)
	{
		float usage = GetCpuUsage(dwCpu);

		if (usage != FLT_MAX)
		{
			if (result != FLT_MAX)
				result = result + usage;
			else
				result = usage;
		}
	}

	if (result != FLT_MAX)
	{
		if (dwCpuCount)
			result = result / dwCpuCount;
	}

	return result;
}
//////////////////////////////////////////////////////////////////////
float CCPUUsage::GetMaxCpuUsage()
{
	float result = 0.0f;

	DWORD dwCpuCount = GetCpuCount();

	for (DWORD dwCpu=0; dwCpu<dwCpuCount; dwCpu++)
	{
		float usage = GetCpuUsage(dwCpu);

		if (usage != FLT_MAX)
		{
			if (result < usage)
				result = usage;
		}
	}

	return result;
}
//////////////////////////////////////////////////////////////////////
