// HAL.h: interface for the CHAL class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
#include <afxtempl.h>

#include "HALImplementation.h"
#include "CPUInfo.h"
#include "CPUUsage.h"
#include "CPUTopology.h"
#include "PingThread.h"
#include "D3DKMTWrapper.h"
#include "NVAPIWrapper.h"
#include "ATIADLWrapper.h"
#include "HALDataSourcesList.h"
#include "HALPollingThread.h"
//////////////////////////////////////////////////////////////////////
class CHAL : public CList<CHALImplementation*, CHALImplementation*>
{
public:
	CHAL();
	virtual ~CHAL();

	//HAL initialization/uninitialization

	void	Init();
	void	Uninit();

	void	SetTimestamp(DWORD dwTimestamp);
		//sets hardware polling timestamp, HAL implementations may use it to precache some hardware sensor data if we request it more than once
		//on the same hardware polling iteration

	//GPU monitoring

	DWORD	GetGpuCount();
		//returns number of physical GPUs
	CHALImplementation* GetGpu(DWORD dwGpu);
		//returns GPU implementation
	CHALImplementation* FindGpu(DWORD dwBus, DWORD dwDev, DWORD dwFn);
		//finds GPU implementation by location
	
	//CPU monitoring

	DWORD	GetCpuCount();
		//returns number of logical CPUs
	BOOL	IsIntelCpu();
		//returns TRUE if CPU is Intel
	BOOL	IsAMDCpu();
		//returns TRUE if CPU is AMD
	LPCSTR	GetCpuString();
		//returns CPU vendor string
	LPCSTR	GetCpuBrandString();
		//returns CPU brand string
	DWORD	GetCpuFamily();
		//returns CPU family
	DWORD	GetCpuModel();
		//returns CPU model
	DWORD	GetCpuStepping();
		//returns CPU stepping
	float	GetCpuUsage(DWORD dwCpu);
		//calculates and returns current CPU usage for specified logical CPU
	float	GetTotalCpuUsage();
		//calculates and returns total CPU usage
	float	GetMaxCpuUsage();
		//calculates and returns max CPU usage across all cores

	//RAM monitoring

	ULONGLONG GetTotalRAM();
		//returns total installed RAM in kilobytes
	ULONGLONG GetPhysicalMemoryUsage();
		//returns current physical memory usage in megabytes
	float GetPhysicalMemoryUsagePercent();
		//returns current physical memory usage in percents
	ULONGLONG GetCommitCharge();
		//returns current commit charge in megabytes
	DWORD GetProcessMemoryUsage();
		//returns foreground process memory usage in megabytes
	float GetProcessMemoryUsagePercent();
		//returns foreground process memory usage in percents

	//Ping monitoring

	void	InitPingThread(LPCSTR lpAddr);
		//initializes asynchronous ping monitoring thread
	void	UninitPingThread();
		//uninitializes asynchronous ping monitoring thread
	CString	GetPingAddr();	
		//returns ping URL or IP address
	float	GetPing();	
		//returns last polled ping in milliseconds

	//Timers

	DWORD	GetLifetime();
		//returns HAL object lifetime in milliseconds

	//HAL data sources 

	CHALDataSourcesList* GetDataSourcesList();
		//returns list of HAL data sources
	CHALDataSource* FindSource(LPCSTR lpSource);
		//finds HAL data source by name
	void UpdateSources();
		//syncronically polls and updates HAL data sources
	void UpdateSourcesAsync();
		//asynchronically polls and updates HAL data sources

	void InitPollingThread();
		//initializes asynchronous HAL data source polling thread
	void UninitPollingThread();
		//uninitializes asynchronous HAL data soruce polling thread
	float GetPollingTime();
		//returns last HAL polling time in milliseconds

	//HAL macro list

	CStringList* GetMacroList();
		//returns HAL macro list (preinitialized macro variables containing CPU, RAM and GPU specs)

protected:
	CString PackCPUString(LPCSTR lpString, BOOL bStripClockFrequency);
	CString PackGPUString(LPCSTR lpString);
	CString Trim(LPCSTR lpString);

	BOOL			m_bInitialized;

	CCPUInfo		m_cpuInfo;
	CCPUUsage		m_cpuUsage;
	CCPUTopology	m_cpuTopology;

	ULONGLONG		m_qwTotalRAM;

	CPingThread*	m_lpPingThread;
	CString			m_strPingAddr;

	CD3DKMTWrapper	m_d3dkmtWrapper;
	CNVAPIWrapper	m_nvapiWrapper;
	CATIADLWrapper	m_atiadlWrapper;

	CHALDataSourcesList m_dataSourcesList;

	CHALPollingThread* m_lpPollingThread;
	float			m_fltPollingTime;

	CStringList		m_strMacroList;

	DWORD			m_dwCreationTimestamp;
};
//////////////////////////////////////////////////////////////////////
