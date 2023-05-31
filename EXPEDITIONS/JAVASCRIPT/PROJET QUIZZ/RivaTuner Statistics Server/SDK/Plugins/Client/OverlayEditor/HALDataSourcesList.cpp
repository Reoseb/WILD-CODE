// HALDataSourcesList.cpp: implementation file
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "HAL.h"
#include "HALDataSourcesList.h"
//////////////////////////////////////////////////////////////////////
CHALDataSourcesList::CHALDataSourcesList()
{
	m_lpHAL = NULL;
}
//////////////////////////////////////////////////////////////////////
CHALDataSourcesList::~CHALDataSourcesList()
{
	Destroy();
}
//////////////////////////////////////////////////////////////////////
void CHALDataSourcesList::Init(CHAL* lpHAL)
{
	Destroy();

	m_lpHAL = lpHAL;

	//CPU

	DWORD dwCpuCount = lpHAL->GetCpuCount();

	for (DWORD dwCpu=0; dwCpu<dwCpuCount; dwCpu++)
	{
		CString strSource;
		strSource.Format("CPU%d usage", dwCpu + 1);

		AddSource(strSource, "%", "CPU", HALDATASOURCE_ID_CPU_USAGE, dwCpu);
	}

	AddSource("CPU usage"					, "%", "CPU", HALDATASOURCE_ID_CPU_USAGE_TOTAL				);
	AddSource("CPU usage max"				, "%", "CPU", HALDATASOURCE_ID_CPU_USAGE_MAX				);

	//RAM

	AddSource("RAM usage"					, "MB"	, "RAM", HALDATASOURCE_ID_RAM_USAGE					);
	AddSource("RAM usage percent"			, "%"	, "RAM", HALDATASOURCE_ID_RAM_USAGE_PERCENT			);
	AddSource("Commit charge"				, "MB"	, "RAM", HALDATASOURCE_ID_COMMIT_CHARGE				);
	AddSource("Total RAM"					, "MB"	, "RAM", HALDATASOURCE_ID_RAM_TOTAL					);
	AddSource("RAM usage \\ process"		, "MB"	, "RAM", HALDATASOURCE_ID_RAM_USAGE_PROCESS			);
	AddSource("RAM usage percent \\ process", "%"	, "RAM", HALDATASOURCE_ID_RAM_USAGE_PERCENT_PROCESS );

	//NET

	AddSource("Ping"				, "ms"	, "NET", HALDATASOURCE_ID_PING				);

	//SYS

	AddSource("Timer"				, "ms"	, "SYS", HALDATASOURCE_ID_TIMER				);

	//GPU

	DWORD dwGpuCount = lpHAL->GetGpuCount();

	for (DWORD dwGpu=0; dwGpu<dwGpuCount; dwGpu++)
	{
		CHALImplementation* lpGpu = lpHAL->GetGpu(dwGpu);

		if (lpGpu->IsCoreClockReportingSupported())
		{
			CString strSource;
			strSource.Format("GPU%d clock", dwGpu + 1);

			AddSource(strSource, "MHz", "GPU", HALDATASOURCE_ID_GPU_CLOCK_CORE, dwGpu);
		}

		if (lpGpu->IsMemoryClockReportingSupported())
		{
			CString strSource;
			strSource.Format("GPU%d memory clock", dwGpu + 1);

			AddSource(strSource, "MHz", "GPU", HALDATASOURCE_ID_GPU_CLOCK_MEMORY, dwGpu);
		}

		if (lpGpu->IsCoreVoltageReportingSupported())
		{
			CString strSource;
			strSource.Format("GPU%d voltage", dwGpu + 1);

			AddSource(strSource, "mV", "GPU", HALDATASOURCE_ID_GPU_VOLTAGE_CORE, dwGpu);
		}

		if (lpGpu->IsMemoryVoltageReportingSupported())
		{
			CString strSource;
			strSource.Format("GPU%d memory voltage", dwGpu + 1);

			AddSource(strSource, "mV", "GPU", HALDATASOURCE_ID_GPU_VOLTAGE_MEMORY, dwGpu);
		}

		DWORD dwSensorCount = lpGpu->GetCoreTemperatureSensorCount();

		for (DWORD dwSensor=0; dwSensor<dwSensorCount; dwSensor++)
		{
			CString strSource;

			if (dwSensor)
				strSource.Format("GPU%d temperature %d", dwGpu + 1, dwSensor + 1);
			else
				strSource.Format("GPU%d temperature", dwGpu + 1);

			AddSource(strSource, "�C", "GPU", HALDATASOURCE_ID_GPU_TEMPERATURE_CORE, dwGpu, dwSensor);
		}

		dwSensorCount = lpGpu->GetMemoryTemperatureSensorCount();

		for (DWORD dwSensor=0; dwSensor<dwSensorCount; dwSensor++)
		{
			CString strSource;

			if (dwSensor)
				strSource.Format("GPU%d memory temperature %d", dwGpu + 1, dwSensor + 1);
			else
				strSource.Format("GPU%d memory temperature", dwGpu + 1);

			AddSource(strSource, "�C", "GPU", HALDATASOURCE_ID_GPU_TEMPERATURE_MEMORY, dwGpu, dwSensor);
		}

		dwSensorCount = lpGpu->GetVRMTemperatureSensorCount();

		for (DWORD dwSensor=0; dwSensor<dwSensorCount; dwSensor++)
		{
			CString strSource;

			if (dwSensor)
				strSource.Format("GPU%d VRM temperature %d", dwGpu + 1, dwSensor + 1);
			else
				strSource.Format("GPU%d VRM temperature", dwGpu + 1);

			AddSource(strSource, "�C", "GPU", HALDATASOURCE_ID_GPU_TEMPERATURE_VRM, dwGpu, dwSensor);
		}

		DWORD dwFanCount = lpGpu->GetFanCount();

		for (DWORD dwFan=0; dwFan<dwFanCount; dwFan++)
		{
			if (lpGpu->IsFanSpeedReportingSupported(dwFan))
			{
				CString strSource;

				if (dwFan)
					strSource.Format("GPU%d fan speed %d", dwGpu + 1, dwFan + 1);
				else
					strSource.Format("GPU%d fan speed", dwGpu + 1);

				AddSource(strSource, "%", "GPU", HALDATASOURCE_ID_GPU_FAN_SPEED, dwGpu, dwFan);
			}

			if (lpGpu->IsFanSpeedRPMReportingSupported(dwFan))
			{
				CString strSource;

				if (dwFan)
					strSource.Format("GPU%d fan tachometer %d", dwGpu + 1, dwFan + 1);
				else
					strSource.Format("GPU%d fan tachometer", dwGpu + 1);

				AddSource(strSource, "RPM", "GPU", HALDATASOURCE_ID_GPU_FAN_SPEED_RPM, dwGpu, dwFan);
			}
		}

		if (lpGpu->IsGraphicsEngineUsageReportingSupported())
		{
			CString strSource;
			strSource.Format("GPU%d usage", dwGpu + 1);

			AddSource(strSource, "%", "GPU", HALDATASOURCE_ID_GPU_GRAPHICS_ENGINE_USAGE, dwGpu);
		}

		if (lpGpu->IsMemoryControllerUsageReportingSupported())
		{
			CString strSource;
			strSource.Format("GPU%d memory controller usage", dwGpu + 1);

			AddSource(strSource, "%", "GPU", HALDATASOURCE_ID_GPU_MEMORY_CONTROLLER_USAGE, dwGpu);
		}

		if (lpGpu->IsVideoEngineUsageReportingSupported())
		{
			CString strSource;
			strSource.Format("GPU%d video engine usage", dwGpu + 1);

			AddSource(strSource, "%", "GPU", HALDATASOURCE_ID_GPU_VIDEO_ENGINE_USAGE, dwGpu);
		}

		if (lpGpu->IsBusUsageReportingSupported())
		{
			CString strSource;
			strSource.Format("GPU%d bus usage", dwGpu + 1);

			AddSource(strSource, "%", "GPU", HALDATASOURCE_ID_GPU_BUS_USAGE, dwGpu);
		}

		if (lpGpu->IsAbsPowerReportingSupported())
		{
			CString strSource;
			strSource.Format("GPU%d power", dwGpu + 1);

			AddSource(strSource, "W", "GPU", HALDATASOURCE_ID_GPU_ABS_POWER, dwGpu);
		}

		if (lpGpu->IsRelPowerReportingSupported())
		{
			CString strSource;
			strSource.Format("GPU%d power percent", dwGpu + 1);

			AddSource(strSource, "%", "GPU", HALDATASOURCE_ID_GPU_REL_POWER, dwGpu);
		}

		if (lpGpu->IsVideomemoryUsageReportingSupported())
		{
			CString strSource;
			strSource.Format("GPU%d memory usage", dwGpu + 1);

			AddSource(strSource, "MB", "GPU", HALDATASOURCE_ID_GPU_VIDEOMEMORY_USAGE, dwGpu);

			strSource.Format("GPU%d memory usage percent", dwGpu + 1);

			AddSource(strSource, "%", "GPU", HALDATASOURCE_ID_GPU_VIDEOMEMORY_USAGE_PERCENT, dwGpu);

			strSource.Format("GPU%d total memory", dwGpu + 1);

			AddSource(strSource, "MB", "GPU", HALDATASOURCE_ID_GPU_VIDEOMEMORY_TOTAL, dwGpu);

			strSource.Format("GPU%d memory usage \\ process", dwGpu + 1);

			AddSource(strSource, "MB", "GPU", HALDATASOURCE_ID_GPU_VIDEOMEMORY_USAGE_PROCESS, dwGpu);

			strSource.Format("GPU%d memory usage percent \\ process", dwGpu + 1);

			AddSource(strSource, "%", "GPU", HALDATASOURCE_ID_GPU_VIDEOMEMORY_USAGE_PERCENT_PROCESS, dwGpu);
		}
	}
}
//////////////////////////////////////////////////////////////////////
void CHALDataSourcesList::Destroy()
{
	POSITION pos = GetHeadPosition();

	while (pos)
		delete GetNext(pos);

	RemoveAll();
}
//////////////////////////////////////////////////////////////////////
void CHALDataSourcesList::AddSource(LPCSTR lpName, LPCSTR lpUnits, LPCSTR lpGroup, DWORD dwID, DWORD dwParam0, DWORD dwParam1)
{
	CHALDataSource* lpSource = new CHALDataSource(lpName, lpUnits, lpGroup, dwID, dwParam0, dwParam1);

	lpSource->SetHAL(m_lpHAL);

	AddTail(lpSource);
}
//////////////////////////////////////////////////////////////////////
CHALDataSource* CHALDataSourcesList::FindSource(LPCSTR lpName)
{
	POSITION pos = GetHeadPosition();

	while (pos)
	{
		CHALDataSource* lpSource = GetNext(pos);

		if (!_stricmp(lpSource->GetName(), lpName))
			return lpSource;
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////
