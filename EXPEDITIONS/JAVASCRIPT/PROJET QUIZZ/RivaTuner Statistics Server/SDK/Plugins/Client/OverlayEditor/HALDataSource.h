// HALDataSource.h: interface for the CHALDataSource class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
#define HALDATASOURCE_ID_UNKNOWN							0xFFFFFFFF

#define HALDATASOURCE_ID_CPU_USAGE							0x00000000
#define HALDATASOURCE_ID_CPU_USAGE_TOTAL					0x00000001
#define HALDATASOURCE_ID_CPU_USAGE_MAX						0x00000002

#define HALDATASOURCE_ID_RAM_USAGE							0x00000100
#define HALDATASOURCE_ID_RAM_USAGE_PERCENT					0x00000101
#define HALDATASOURCE_ID_COMMIT_CHARGE						0x00000102		
#define HALDATASOURCE_ID_RAM_TOTAL							0x00000103		
#define HALDATASOURCE_ID_RAM_USAGE_PROCESS					0x00000104
#define HALDATASOURCE_ID_RAM_USAGE_PERCENT_PROCESS			0x00000105

#define HALDATASOURCE_ID_GPU_CLOCK_CORE						0x00000200
#define HALDATASOURCE_ID_GPU_CLOCK_MEMORY					0x00000201

#define HALDATASOURCE_ID_GPU_VOLTAGE_CORE					0x00000210
#define HALDATASOURCE_ID_GPU_VOLTAGE_MEMORY					0x00000211

#define HALDATASOURCE_ID_GPU_TEMPERATURE_CORE				0x00000220
#define HALDATASOURCE_ID_GPU_TEMPERATURE_MEMORY				0x00000221
#define HALDATASOURCE_ID_GPU_TEMPERATURE_VRM				0x00000222

#define HALDATASOURCE_ID_GPU_FAN_SPEED						0x00000230
#define HALDATASOURCE_ID_GPU_FAN_SPEED_RPM					0x00000231

#define HALDATASOURCE_ID_GPU_GRAPHICS_ENGINE_USAGE			0x00000240
#define HALDATASOURCE_ID_GPU_MEMORY_CONTROLLER_USAGE		0x00000241
#define HALDATASOURCE_ID_GPU_VIDEO_ENGINE_USAGE				0x00000242
#define HALDATASOURCE_ID_GPU_BUS_USAGE						0x00000243

#define HALDATASOURCE_ID_GPU_REL_POWER						0x00000250
#define HALDATASOURCE_ID_GPU_ABS_POWER						0x00000251

#define HALDATASOURCE_ID_GPU_VIDEOMEMORY_USAGE				0x00000260
#define HALDATASOURCE_ID_GPU_VIDEOMEMORY_USAGE_PERCENT		0x00000261
#define HALDATASOURCE_ID_GPU_VIDEOMEMORY_TOTAL				0x00000262
#define HALDATASOURCE_ID_GPU_VIDEOMEMORY_USAGE_PROCESS		0x00000263
#define HALDATASOURCE_ID_GPU_VIDEOMEMORY_USAGE_PERCENT_PROCESS 0x00000264

#define HALDATASOURCE_ID_PING								0x00000300

#define HALDATASOURCE_ID_TIMER								0x00000400
//////////////////////////////////////////////////////////////////////
class CHAL;
class CHALDataSource
{
public:
	CHALDataSource(LPCSTR lpName, LPCSTR lpUnits, LPCSTR lpGroup, DWORD dwID, DWORD dwParam0, DWORD dwParam1);
	virtual ~CHALDataSource();

	void	SetHAL(CHAL* lpHAL);

	void	Poll();
	BOOL	IsDirty();
	void	SetDirty(BOOL bDirty);

	float	GetData();
	float	GetPollingTime();

	DWORD	GetID();
	CString	GetName();
	CString GetUnits();
	CString GetGroup();

protected: 
	CHAL*	m_lpHAL;

	BOOL	m_bDirty;

	float	m_fltData;
	float	m_fltPollingTime;	

	CString m_strName;
	CString m_strUnits;
	CString m_strGroup;

	DWORD	m_dwID;
	DWORD	m_dwParam0;
	DWORD	m_dwParam1;
};
//////////////////////////////////////////////////////////////////////
