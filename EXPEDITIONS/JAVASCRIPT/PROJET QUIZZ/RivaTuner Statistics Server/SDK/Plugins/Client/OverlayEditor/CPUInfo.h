// CPUInfo.h: interface for the CCPUInfo class.
//
// created by Unwinder
/////////////////////////////////////////////////////////////////////////////
#pragma once
/////////////////////////////////////////////////////////////////////////////
class CCPUInfo
{
public:
	CCPUInfo();
	virtual ~CCPUInfo();

	void	Init();

	BOOL	IsIntelCpu();
	BOOL	IsAMDCpu();

	LPCSTR	GetCpuString();
	LPCSTR	GetCpuBrandString();
	DWORD	GetCpuFamily();
	DWORD	GetCpuModel();
	DWORD	GetCpuStepping();

protected:
	DWORD	m_dwCpuStepping;
	DWORD	m_dwCpuModel;
	DWORD	m_dwCpuModelExt;
	DWORD	m_dwCpuFamily;
	DWORD	m_dwCpuFamilyExt;
	char	m_szCpuString[0x20];
	char	m_szCpuBrandString[0x40];
};
/////////////////////////////////////////////////////////////////////////////
