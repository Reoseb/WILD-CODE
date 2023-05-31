// HAL.cpp: implementation file
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "HAL.h"

#include "HALImplementationNVAPI.h"
#include "HALImplementationATIADL.h"
#include "HALImplementationUnified.h"
#include "RTSSsharedMemoryInterface.h"
#include "Log.h"

#include <intrin.h>
#include <float.h>
//////////////////////////////////////////////////////////////////////
CHAL::CHAL()
{
	m_bInitialized					= FALSE;

	m_qwTotalRAM					= 0;

	m_lpPingThread					= NULL;
	m_strPingAddr					= "";

	m_lpPollingThread				= NULL;
	m_fltPollingTime				= 0;

	m_dwCreationTimestamp			= GetTickCount();
}
//////////////////////////////////////////////////////////////////////
CHAL::~CHAL()
{
	Uninit();
}
//////////////////////////////////////////////////////////////////////
void CHAL::Uninit()
{
	POSITION pos = GetHeadPosition();

	while (pos)
	{
		CHALImplementation* lpImplementation = GetNext(pos);

		delete lpImplementation;
	}

	RemoveAll();
}
//////////////////////////////////////////////////////////////////////
void CHAL::Init()
{
	if (!m_bInitialized)
	{
		//init CPU monitoring/identification related objects

		m_cpuInfo.Init();
		m_cpuUsage.Init();
		m_cpuTopology.Init();

		DWORD dwCpuCount = GetCpuCount();

		APPEND_LOG("Initializing CPU info");
		APPEND_LOG1("%s", Trim(GetCpuBrandString()));
		APPEND_LOG4("%s family %X model %X stepping %X", GetCpuString(), GetCpuFamily(), GetCpuModel(), GetCpuStepping());
		APPEND_LOG1("%d logical cores", dwCpuCount);

		if (dwCpuCount > 1)
		{
			for (DWORD dwCpu=0; dwCpu<dwCpuCount; dwCpu++)
			{
				APPEND_LOG3("CPU%d on package %d, core %d", dwCpu, m_cpuTopology.GetPckg(dwCpu), m_cpuTopology.GetCore(dwCpu));
			}
		}

		//init total RAM

		m_qwTotalRAM	= GetTotalRAM();

		//init GPU API wrappers

		m_d3dkmtWrapper.Init();
		m_nvapiWrapper.Init();
		m_atiadlWrapper.Init();

		//populate GPU implementations list

		APPEND_LOG("Populating HAL GPU implementations");

		for (DWORD dwGpu=0; dwGpu<m_nvapiWrapper.GetGpuCount(); dwGpu++)
			//popluate NVAPI implementations list
		{
			CHALImplementationNVAPI* lpImplementation = new CHALImplementationNVAPI;

			lpImplementation->SetD3DKMTWrapper(&m_d3dkmtWrapper);
			lpImplementation->SetNVAPIWrapper(&m_nvapiWrapper);
			lpImplementation->SetGpu(dwGpu);
			lpImplementation->BindD3DKMTDeviceDesc();

			DWORD dwBus;
			DWORD dwDev;
			DWORD dwFn;

			lpImplementation->GetLocation(&dwBus, &dwDev, &dwFn);

			APPEND_LOG4("GPU%d : NVAPI implementation on bus %d, dev %d, fn %d", GetCount(), dwBus, dwDev, dwFn);

			AddTail(lpImplementation);
		}

		for (DWORD dwGpu=0; dwGpu<m_atiadlWrapper.GetGpuCount(); dwGpu++)
			//popluate ATIADL implementations list
		{
			CHALImplementationATIADL* lpImplementation = new CHALImplementationATIADL;

			lpImplementation->SetD3DKMTWrapper(&m_d3dkmtWrapper);
			lpImplementation->SetATIADLWrapper(&m_atiadlWrapper);
			lpImplementation->SetGpu(dwGpu);
			lpImplementation->BindD3DKMTDeviceDesc();

			DWORD dwBus;
			DWORD dwDev;
			DWORD dwFn;

			lpImplementation->GetLocation(&dwBus, &dwDev, &dwFn);

			APPEND_LOG4("GPU%d : ATIADL implementation on bus %d, dev %d, fn %d", GetCount(), dwBus, dwDev, dwFn);

			AddTail(lpImplementation);
		}

		POSITION pos = m_d3dkmtWrapper.GetDeviceList()->GetHeadPosition();

		while (pos)
			//populate unified implementations list for D3DKMT devices unsupported by NVAPI/ATIADL (e.g. Intel iGPUs)
		{
			LPD3DKMTDEVICE_DESC lpDesc = m_d3dkmtWrapper.GetDeviceList()->GetNext(pos);

			if (lpDesc->Bus || lpDesc->Dev)
				//ensure that we're processing actual hardware - skip basic render, which has no PCI location info
			{
				if (!FindGpu(lpDesc->Bus, lpDesc->Dev, lpDesc->Fn))
					//check if current D3DKMT device is already mapped to existing NVAPI/ATIADL implementations
				{
					CHALImplementationUnified* lpImplementation = new CHALImplementationUnified;

					lpImplementation->SetD3DKMTWrapper(&m_d3dkmtWrapper);
					lpImplementation->SetD3DKMTDeviceDesc(lpDesc);

					APPEND_LOG4("GPU%d : unified implementation on bus %d, dev %d, fn %d", GetCount(), lpDesc->Bus, lpDesc->Dev, lpDesc->Fn);

					AddTail(lpImplementation);
				}
			}
		}

		//init data sources list

		m_dataSourcesList.Init(this);

		//init HAL macro list with system specs

		CString strCPUFull	= Trim(GetCpuBrandString());
		CString strCPU		= PackCPUString(strCPUFull, FALSE);
		CString	strCPUShort	= PackCPUString(strCPUFull, TRUE);

		m_strMacroList.AddTail("%CPU%");
		m_strMacroList.AddTail(strCPU);
		m_strMacroList.AddTail("%CPUFull%");
		m_strMacroList.AddTail(strCPUFull);
		m_strMacroList.AddTail("%CPUShort%");
		m_strMacroList.AddTail(strCPUShort);

		CString strRAM;
		ULONGLONG qwTotalRAM	= GetTotalRAM();
		DWORD dwGB				= (DWORD)(qwTotalRAM / 1048576);
		DWORD dwGBChunk			= (DWORD)(qwTotalRAM % 1048576) * 100 / 1048576;
		if (dwGBChunk)
			strRAM.Format("%d.%02dGB", dwGB, dwGBChunk);
		else
			strRAM.Format("%dGB", dwGB);

		m_strMacroList.AddTail("%RAM%");
		m_strMacroList.AddTail(strRAM);

		if (GetGpuCount())
		{
			CHALImplementation* lpImplementation = GetHead();

			CString strGPUFull	= lpImplementation->GetName();
			CString strGPU		= PackGPUString(strGPUFull);

			m_strMacroList.AddTail("%GPU%");
			m_strMacroList.AddTail(strGPU);
			m_strMacroList.AddTail("%GPUFull%");
			m_strMacroList.AddTail(strGPUFull);

			CString strVRAM;
			DWORD dwTotalVRAM		= lpImplementation->GetTotalVideomemory();
			DWORD dwGB				= (dwTotalVRAM / 1048576);
			DWORD dwGBChunk			= (dwTotalVRAM % 1048576) * 100 / 1048576;

			if (dwGBChunk)
				strRAM.Format("%d.%02dGB", dwGB, dwGBChunk);
			else
				strRAM.Format("%dGB", dwGB);

			m_strMacroList.AddTail("%VRAM%");
			m_strMacroList.AddTail(strRAM);

			CString strDriver	= lpImplementation->GetDriverStr();

			m_strMacroList.AddTail("%Driver%");
			m_strMacroList.AddTail(strDriver);
		}

		m_bInitialized	= TRUE;
	}
}
//////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI* PGETPHYSICALLYINSTALLEDSYSTEMMEMORY)(PULONGLONG TotalMemoryInKilobytes);
//////////////////////////////////////////////////////////////////////
ULONGLONG CHAL::GetTotalRAM()
{
	if (m_qwTotalRAM)
		return m_qwTotalRAM;

	ULONGLONG qwTotalRAM;

	HINSTANCE hKernel = GetModuleHandle("KERNEL32.DLL");

	if (hKernel)
	{
		PGETPHYSICALLYINSTALLEDSYSTEMMEMORY pGetPhysicallyInstalledSystemMemory = (PGETPHYSICALLYINSTALLEDSYSTEMMEMORY)GetProcAddress(hKernel, "GetPhysicallyInstalledSystemMemory");

		if (pGetPhysicallyInstalledSystemMemory)
		{
			if (pGetPhysicallyInstalledSystemMemory(&qwTotalRAM))
				return qwTotalRAM;
		}
	}

	MEMORYSTATUSEX status; 
	status.dwLength = sizeof(MEMORYSTATUSEX);

	GlobalMemoryStatusEx(&status);

	return status.ullTotalPhys / 1024;
}
//////////////////////////////////////////////////////////////////////
CString CHAL::Trim(LPCSTR lpString)
{
	CString strResult = lpString;

	while (strResult.Replace("  ", " "))
	{
	}

	strResult.Trim();

	return strResult;
}
/////////////////////////////////////////////////////////////////////////////
CString CHAL::PackCPUString(LPCSTR lpString, BOOL bStripClockFrequency)
{
	CString strResult = lpString;

	strResult.Replace("AMD"				, "");
	strResult.Replace("Quad-Core"		, "");
	strResult.Replace("Six-Core"		, "");
	strResult.Replace("Eight-Core"		, "");
	strResult.Replace("8-Core"			, "");
	strResult.Replace("12-Core"			, "");
	strResult.Replace("16-Core"			, "");
	strResult.Replace("24-Core"			, "");
	strResult.Replace("32-Core"			, "");
	strResult.Replace("Processor"		, "");
	strResult.Replace("Intel(R)"		, "");
	strResult.Replace("Core(TM)2 Duo"	, "");
	strResult.Replace("Core(TM)2 Quad"	, "");
	strResult.Replace("Core(TM)"		, "");
	strResult.Replace("CPU"				, "");

	if (bStripClockFrequency)
	{
		int nPos = strResult.Find("@");

		if (nPos != -1)
			strResult = strResult.Left(nPos);
	}


	return Trim(strResult);
}
//////////////////////////////////////////////////////////////////////
CString CHAL::PackGPUString(LPCSTR lpString)
{
	CString strResult = lpString;

	strResult.Replace("Intel(R)"	, "");
	strResult.Replace("Graphics"	, "");
	strResult.Replace("NVIDIA"		, "");
	strResult.Replace("GeForce"		, "");
	strResult.Replace("ATI"			, "");
	strResult.Replace("AMD"			, "");
	strResult.Replace("Radeon"		, "");
	strResult.Replace("Series"		, "");
	strResult.Replace("(TM)"		, "");

	return Trim(strResult);
}
//////////////////////////////////////////////////////////////////////
DWORD CHAL::GetCpuCount()
{
	return m_cpuUsage.GetCpuCount();
}
//////////////////////////////////////////////////////////////////////
float CHAL::GetCpuUsage(DWORD dwCpu)
{
	return m_cpuUsage.GetCpuUsage(dwCpu);
}
//////////////////////////////////////////////////////////////////////
float CHAL::GetTotalCpuUsage()
{
	return m_cpuUsage.GetTotalCpuUsage();
}
//////////////////////////////////////////////////////////////////////
float CHAL::GetMaxCpuUsage()
{
	return m_cpuUsage.GetMaxCpuUsage();
}
//////////////////////////////////////////////////////////////////////
ULONGLONG CHAL::GetPhysicalMemoryUsage()
{
	MEMORYSTATUSEX status; 
	status.dwLength = sizeof(MEMORYSTATUSEX);

	GlobalMemoryStatusEx(&status);

	return (status.ullTotalPhys - status.ullAvailPhys) / 1048576;
}
/////////////////////////////////////////////////////////////////////////////
ULONGLONG CHAL::GetCommitCharge()
{
	MEMORYSTATUSEX status; 
	status.dwLength = sizeof(MEMORYSTATUSEX);

	GlobalMemoryStatusEx(&status);

	return (status.ullTotalPageFile - status.ullAvailPageFile) / 1048576;
}
/////////////////////////////////////////////////////////////////////////////
float CHAL::GetPhysicalMemoryUsagePercent()
{
	MEMORYSTATUSEX status; 
	status.dwLength = sizeof(MEMORYSTATUSEX);

	GlobalMemoryStatusEx(&status);

	return 100.0f * (status.ullTotalPhys - status.ullAvailPhys) / status.ullTotalPhys;
}
/////////////////////////////////////////////////////////////////////////////
void CHAL::InitPingThread(LPCSTR lpAddr)
{
	m_strPingAddr = lpAddr;

	if (strlen(lpAddr))
	{
		if (m_lpPingThread)
			m_lpPingThread->SetAddr(lpAddr);
		else
		{
			m_lpPingThread = new CPingThread(lpAddr);

			m_lpPingThread->CreateThread(CREATE_SUSPENDED);
			m_lpPingThread->SetThreadPriority(THREAD_PRIORITY_NORMAL);
			m_lpPingThread->ResumeThread();
		}
	}
	else
		UninitPingThread();
}
/////////////////////////////////////////////////////////////////////////////
CString	CHAL::GetPingAddr()
{
	return m_strPingAddr;
}
/////////////////////////////////////////////////////////////////////////////
float CHAL::GetPing()
{
	if (m_lpPingThread)
	{
		m_lpPingThread->UpdatePingAsync();

		return m_lpPingThread->GetPing();
	}

	return FLT_MAX;
}
/////////////////////////////////////////////////////////////////////////////
void CHAL::UninitPingThread()
{
	if (m_lpPingThread)
		m_lpPingThread->Kill();
	m_lpPingThread = NULL;

	m_strPingAddr = "";
}
/////////////////////////////////////////////////////////////////////////////
CHALImplementation* CHAL::FindGpu(DWORD dwBus, DWORD dwDev, DWORD dwFn)
{
	POSITION pos = GetHeadPosition();

	while (pos)
	{
		CHALImplementation* lpImplementation = GetNext(pos);

		DWORD dwImplementationBus;
		DWORD dwImplementationDev;
		DWORD dwImplementationFn;

		if (lpImplementation->GetLocation(&dwImplementationBus, &dwImplementationDev, &dwImplementationFn))
		{
			if ((dwBus == dwImplementationBus) &&
				(dwDev == dwImplementationDev) &&
				(dwFn  == dwImplementationFn ))
				return lpImplementation;
		}
	}

	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
DWORD CHAL::GetGpuCount()
{
	return GetCount();
}
/////////////////////////////////////////////////////////////////////////////
CHALImplementation* CHAL::GetGpu(DWORD dwGpu)
{
	POSITION pos = FindIndex(dwGpu);

	if (pos)
		return GetAt(pos);

	return NULL;
}
/////////////////////////////////////////////////////////////////////////////
CHALDataSourcesList* CHAL::GetDataSourcesList()
{
	return &m_dataSourcesList;
}
/////////////////////////////////////////////////////////////////////////////
CHALDataSource* CHAL::FindSource(LPCSTR lpSource)
{
	return m_dataSourcesList.FindSource(lpSource);
}
/////////////////////////////////////////////////////////////////////////////
void CHAL::SetTimestamp(DWORD dwTimestamp)
{
	POSITION pos = GetHeadPosition();

	while (pos)
	{
		CHALImplementation* lpImplementation = GetNext(pos);

		lpImplementation->SetTimestamp(dwTimestamp);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CHAL::UpdateSources()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, "Global\\Access_hw_poll");

	if (hMutex)
		WaitForSingleObject(hMutex, INFINITE);

	TIMING_INIT
	TIMING_BEGIN		

	POSITION pos = m_dataSourcesList.GetHeadPosition();

	while (pos)
	{
		CHALDataSource* lpSource = m_dataSourcesList.GetNext(pos);

		if (lpSource->IsDirty())
		{
			lpSource->Poll();

			lpSource->SetDirty(FALSE);
		}
	}

	TIMING_END

	m_fltPollingTime = timing;

	if (hMutex)
	{
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CHAL::UpdateSourcesAsync()
{
	InitPollingThread();

	if (m_lpPollingThread)
		m_lpPollingThread->AsyncPoll();
	else
		UpdateSources();
}
/////////////////////////////////////////////////////////////////////////////
void CHAL::InitPollingThread()
{
	if (!m_lpPollingThread)
	{
		m_lpPollingThread = new CHALPollingThread(this);

		m_lpPollingThread->CreateThread(CREATE_SUSPENDED);
		m_lpPollingThread->SetThreadPriority(THREAD_PRIORITY_NORMAL);
		m_lpPollingThread->ResumeThread();
	}
}
/////////////////////////////////////////////////////////////////////////////
void CHAL::UninitPollingThread()
{
	if (m_lpPollingThread)
		m_lpPollingThread->Kill();
	m_lpPollingThread = NULL;
}
/////////////////////////////////////////////////////////////////////////////
float CHAL::GetPollingTime()
{
	return m_fltPollingTime;
}
/////////////////////////////////////////////////////////////////////////////
BOOL CHAL::IsIntelCpu()
{
	return m_cpuInfo.IsIntelCpu();
}
//////////////////////////////////////////////////////////////////////
BOOL CHAL::IsAMDCpu()
{
	return m_cpuInfo.IsAMDCpu();
}
//////////////////////////////////////////////////////////////////////
LPCSTR CHAL::GetCpuString()
{
	return m_cpuInfo.GetCpuString();
}
/////////////////////////////////////////////////////////////////////////////
LPCSTR CHAL::GetCpuBrandString()
{
	return m_cpuInfo.GetCpuBrandString();
}
/////////////////////////////////////////////////////////////////////////////
DWORD CHAL::GetCpuFamily()
{
	return m_cpuInfo.GetCpuFamily();
}
/////////////////////////////////////////////////////////////////////////////
DWORD CHAL::GetCpuModel()
{
	return m_cpuInfo.GetCpuModel();
}
/////////////////////////////////////////////////////////////////////////////
DWORD CHAL::GetCpuStepping()
{
	return m_cpuInfo.GetCpuStepping();
}
/////////////////////////////////////////////////////////////////////////////
CStringList* CHAL::GetMacroList()
{
	return &m_strMacroList;
}
/////////////////////////////////////////////////////////////////////////////
DWORD CHAL::GetLifetime()
{
	return GetTickCount() - m_dwCreationTimestamp;
}
/////////////////////////////////////////////////////////////////////////////
DWORD CHAL::GetProcessMemoryUsage()
{
	DWORD dwUsage = 0;

/*
	//We could use GetProcessMemoryInfo to read process memory usage directly, but it won't work for the applications protected with
	//BattlEye/EAC due to restricted OpenProcess, so commented part is provided as an example of alternate inplace implementation 
	//for third party developers
	//We'll use RTSS process specific performance counters access interface instead, it will do the same in context of protected process

	HANDLE hProcess = OpenForegroundProcess();

	if (hProcess)
	{
		PROCESS_MEMORY_COUNTERS pmc;

		if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(PROCESS_MEMORY_COUNTERS)))
			dwUsage = pmc.WorkingSetSize>>10L;

		CloseHandle(hProcess);
	}
*/

	CRTSSSharedMemoryInterface sharedMemoryInterface;
	sharedMemoryInterface.GetProcessPerfCounter(0, 1000, PROCESS_PERF_COUNTER_ID_RAM_USAGE, 0, &dwUsage);

	return dwUsage;
}
/////////////////////////////////////////////////////////////////////////////
float CHAL::GetProcessMemoryUsagePercent()
{
	DWORD dwUsage = (DWORD)GetProcessMemoryUsage();
	DWORD dwTotal = (DWORD)GetTotalRAM();

	if (dwTotal)
		return 100.0f * dwUsage / dwTotal;

	return 0;
}
/////////////////////////////////////////////////////////////////////////////
