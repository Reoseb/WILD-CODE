// NVAPIInterface.cpp: implementation of the CNVAPIInterface class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "NVAPIInterface.h"
//////////////////////////////////////////////////////////////////////
CNVAPIInterface::CNVAPIInterface()
{
}
//////////////////////////////////////////////////////////////////////
CNVAPIInterface::~CNVAPIInterface()
{
}
//////////////////////////////////////////////////////////////////////
LPVOID CNVAPIInterface::QueryInterface(DWORD dwIID)
{
	HINSTANCE hNVAPI = GetModuleHandle("nvapi.dll");

	if (hNVAPI)
	{
		LPNVAPI_QUERYINTERFACE lpQueryInterface = (LPNVAPI_QUERYINTERFACE)GetProcAddress(hNVAPI, "nvapi_QueryInterface");

		if (lpQueryInterface)
			return lpQueryInterface(dwIID);
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////
NvAPI_Status CNVAPIInterface::NvAPI_GPU_PowerMonitorGetInfo(NvPhysicalGpuHandle hGpu, NV_GPU_POWER_MONITOR_GET_INFO* lpInfo)
{
	LPNVAPI_GPU_POWERMONITORGETINFO lpNvAPI_GPU_PowerMonitorGetInfo = (LPNVAPI_GPU_POWERMONITORGETINFO)QueryInterface(NVAPIIID_GPU_PowerMonitorGetInfo);

	if (lpNvAPI_GPU_PowerMonitorGetInfo)
		return lpNvAPI_GPU_PowerMonitorGetInfo(hGpu, lpInfo);

	return NVAPI_NO_IMPLEMENTATION;
}
//////////////////////////////////////////////////////////////////////
NvAPI_Status CNVAPIInterface::NvAPI_GPU_PowerMonitorGetStatus(NvPhysicalGpuHandle hGpu, NV_GPU_POWER_MONITOR_GET_STATUS* lpStatus)
{
	LPNVAPI_GPU_POWERMONITORGETSTATUS lpNvAPI_GPU_PowerMonitorGetStatus = (LPNVAPI_GPU_POWERMONITORGETSTATUS)QueryInterface(NVAPIIID_GPU_PowerMonitorGetStatus);

	if (lpNvAPI_GPU_PowerMonitorGetStatus)
		return lpNvAPI_GPU_PowerMonitorGetStatus(hGpu, lpStatus);

	return NVAPI_NO_IMPLEMENTATION;
}
//////////////////////////////////////////////////////////////////////