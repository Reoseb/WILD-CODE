// CPUInfo.cpp : implementation file
//
// created by Unwinder
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "CPUInfo.h"

#include <intrin.h>
/////////////////////////////////////////////////////////////////////////////
CCPUInfo::CCPUInfo()
{
	m_dwCpuStepping					= 0;
	m_dwCpuModel					= 0;
	m_dwCpuModelExt					= 0;
	m_dwCpuFamily					= 0;
	m_dwCpuFamilyExt				= 0;
	ZeroMemory(m_szCpuString		, sizeof(m_szCpuString		));
	ZeroMemory(m_szCpuBrandString	, sizeof(m_szCpuBrandString	));
}
/////////////////////////////////////////////////////////////////////////////
CCPUInfo::~CCPUInfo()
{
}
/////////////////////////////////////////////////////////////////////////////
void CCPUInfo::Init()
{
	int cpuInfo[4] = { 0 };

	__cpuid(cpuInfo, 0);

    *((int*)(m_szCpuString		))	= cpuInfo[1];
	*((int*)(m_szCpuString + 4	))	= cpuInfo[3];
	*((int*)(m_szCpuString + 8	))	= cpuInfo[2];

	__cpuid(cpuInfo, 1);

	m_dwCpuStepping					= (cpuInfo[0]	 ) & 0x0F;
	m_dwCpuModel					= (cpuInfo[0]>>4 ) & 0x0F;
	m_dwCpuFamily					= (cpuInfo[0]>>8 ) & 0x0F;

	m_dwCpuModelExt					= (cpuInfo[0]>>16) & 0x0F;
	m_dwCpuFamilyExt				= (cpuInfo[0]>>20) & 0xFF;

	__cpuid(cpuInfo, 0x80000000);

	char szCpuBrandString[0x40] = { 0 };

	int nMaxEx = cpuInfo[0];

	if  (nMaxEx >= 0x80000002)
	{
		__cpuid(cpuInfo, 0x80000002);
		CopyMemory(m_szCpuBrandString, cpuInfo, sizeof(cpuInfo));

	}

	if  (nMaxEx >= 0x80000003)
	{
		__cpuid(cpuInfo, 0x80000003);
		CopyMemory(m_szCpuBrandString + 16, cpuInfo, sizeof(cpuInfo));
	}

	if  (nMaxEx >= 0x80000004)
	{
		__cpuid(cpuInfo, 0x80000004);
		CopyMemory(m_szCpuBrandString + 32, cpuInfo, sizeof(cpuInfo));
	}
}
/////////////////////////////////////////////////////////////////////////////
BOOL CCPUInfo::IsIntelCpu()
{
	return !_stricmp(m_szCpuString, "GenuineIntel");
}
/////////////////////////////////////////////////////////////////////////////
BOOL CCPUInfo::IsAMDCpu()
{
	return !_stricmp(m_szCpuString, "AuthenticAMD");
}
/////////////////////////////////////////////////////////////////////////////
LPCSTR CCPUInfo::GetCpuString()
{
	return m_szCpuString;
}
/////////////////////////////////////////////////////////////////////////////
LPCSTR CCPUInfo::GetCpuBrandString()
{
	return m_szCpuBrandString;
}
/////////////////////////////////////////////////////////////////////////////
DWORD CCPUInfo::GetCpuFamily()
{
	if (IsIntelCpu())
	//	http://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-2a-manual.pdf
	//
	// The Extended Family ID needs to be examined only when the Family ID is 0FH. Integrate the fields into a display
	// using the following rule:
	// IF Family_ID != 0FH
	// THEN DisplayFamily = Family_ID;
	// ELSE DisplayFamily = Extended_Family_ID + Family_ID;
	// (* Right justify and zero-extend 4-bit field. *)
	// FI;
	// (* Show DisplayFamily as HEX field. *)
	{
		if (m_dwCpuFamily != 0x0F)
			return m_dwCpuFamily;

		return m_dwCpuFamily + m_dwCpuFamilyExt;
	}

	if (IsAMDCpu())
	// http://support.amd.com/TechDocs/25481.pdf
	//
	// The Family is an 8-bit value and is defined as: Family[7:0] = ({0000b,BaseFamily[3:0]} + ExtendedFamily[7:0]).
	// For example, if BaseFamily[3:0] = Fh and ExtendedFamily[7:0] = 01h, then Family[7:0] = 10h. If
	// BaseFamily[3:0] is less than Fh then ExtendedFamily[7:0] is reserved and Family is equal to BaseFamily[3:0].
	{
		if (m_dwCpuFamily < 0x0F)
			return m_dwCpuFamily;

		return m_dwCpuFamily + m_dwCpuFamilyExt;
	}

	return m_dwCpuFamily;
}
/////////////////////////////////////////////////////////////////////////////
DWORD CCPUInfo::GetCpuModel()
{
	if (IsIntelCpu())
	//	http://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-2a-manual.pdf
	//
	// The Extended Model ID needs to be examined only when the Family ID is 06H or 0FH. Integrate the field into a
	// display using the following rule:
	// IF (Family_ID = 06H or Family_ID = 0FH)
	// THEN DisplayModel = (Extended_Model_ID < 4) + Model_ID;
	// (* Right justify and zero-extend 4-bit field; display Model_ID as HEX field.*)
	// ELSE DisplayModel = Model_ID;
	// FI;
	// (* Show DisplayModel as HEX field. *)
	{
		if ((m_dwCpuFamily == 0x06) ||
			(m_dwCpuFamily == 0x0F))
			return m_dwCpuModel + (m_dwCpuModelExt<<4);

		return m_dwCpuModel;
	}

	if (IsAMDCpu())
	// http://support.amd.com/TechDocs/25481.pdf
	//
	//Model is an 8-bit value and is defined as: Model[7:0] = {ExtendedModel[3:0],BaseModel[3:0]}. For example,
	//if ExtendedModel[3:0] = Eh and BaseModel[3:0] = 8h, then Model[7:0] = E8h. If BaseFamily[3:0] is less than
	//0Fh then ExtendedModel[3:0] is reserved and Model is equal to BaseModel[3:0].
	{
		if (m_dwCpuFamily < 0x0F)
			return m_dwCpuModel;

		return m_dwCpuModel + (m_dwCpuModelExt<<4);
	}

	return m_dwCpuModel;
}
/////////////////////////////////////////////////////////////////////////////
DWORD CCPUInfo::GetCpuStepping()
{
	return m_dwCpuStepping;
}
/////////////////////////////////////////////////////////////////////////////

