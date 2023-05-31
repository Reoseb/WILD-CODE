// HotkeyHandlerConfig.cpp: implementation of the CHotkeyHandlerConfig class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "HotkeyHandlerConfig.h"
#include "HotkeyHandler.h"

#include <shlwapi.h>
#include <direct.h>
//////////////////////////////////////////////////////////////////////
CHotkeyHandlerConfig::CHotkeyHandlerConfig()
{
	m_strCfgPath				= "";

	m_strScreenCaptureFormat	= "";
	m_dwScreenCaptureQuality	= 100;
	m_strScreenCaptureFolder	= "";
	m_bCaptureOSD				= TRUE;
	
	m_strVideoCaptureContainer	= "mkv";
	m_strVideoCaptureFormat		= "MJPG";
	m_dwVideoCaptureQuality		= 75;
	m_strVideoCaptureFolder		= "";
	m_dwVideoCaptureFramesize	= 0;
	m_dwVideoCaptureFramerate	= 60;
	m_dwAudioCaptureFlags		= 0x00000003;
	m_dwAudioCaptureFlags2		= 0x00000002;
	m_dwVideoCaptureFlagsEx		= 0x00000000;
	m_dwPrerecordTimeLimit		= 0;
	m_dwPrerecordSizeLimit		= 0;
	m_bAutoPrerecord			= FALSE;

	m_strBenchmarkPath			= "";
	m_bAppendBenchmark			= TRUE;
	
	m_strPPM1Desc				= "";
	m_strPPM1Profile			= "";
	m_strPPM1Property			= "";
	m_dwPPM1Type				= 0;
	m_dwPPM1Value				= 0;

	m_strPPM2Desc				= "";
	m_strPPM2Profile			= "";
	m_strPPM2Property			= "";
	m_dwPPM2Type				= 0;
	m_dwPPM2Value				= 0;

	m_strPPM3Desc				= "";
	m_strPPM3Profile			= "";
	m_strPPM3Property			= "";
	m_dwPPM3Type				= 0;
	m_dwPPM3Value				= 0;

	m_strPPM4Desc				= "";
	m_strPPM4Profile			= "";
	m_strPPM4Property			= "";
	m_dwPPM4Type				= 0;
	m_dwPPM4Value				= 0;

	m_strOVM1Desc				= "";
	m_strOVM1Message			= "";
	m_strOVM1Layer				= "";
	m_strOVM1Params				= "";

	m_strOVM2Desc				= "";
	m_strOVM2Message			= "";
	m_strOVM2Layer				= "";
	m_strOVM2Params				= "";

	m_strOVM3Desc				= "";
	m_strOVM3Message			= "";
	m_strOVM3Layer				= "";
	m_strOVM3Params				= "";

	m_strOVM4Desc				= "";
	m_strOVM4Message			= "";
	m_strOVM4Layer				= "";
	m_strOVM4Params				= "";
}
//////////////////////////////////////////////////////////////////////
CHotkeyHandlerConfig::~CHotkeyHandlerConfig()
{
}
//////////////////////////////////////////////////////////////////////
CString CHotkeyHandlerConfig::GetCfgPath()
{
	if (!strlen(m_strCfgPath))
	{
		char szCfgPath[MAX_PATH];
		GetModuleFileName(g_hModule, szCfgPath, MAX_PATH);
		PathRenameExtension(szCfgPath, ".cfg");

		m_strCfgPath = szCfgPath;
	}

	return m_strCfgPath;
}
//////////////////////////////////////////////////////////////////////
CString CHotkeyHandlerConfig::GetConfigStr(LPCSTR lpSection, LPCSTR lpName, LPCTSTR lpDefault)
{
	char szBuf[CHAR_BUF_SIZE];
	GetPrivateProfileString(lpSection, lpName, lpDefault, szBuf, CHAR_BUF_SIZE, GetCfgPath());

	return szBuf;
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerConfig::SetConfigStr(LPCSTR lpSection, LPCSTR lpName, LPCSTR lpValue)
{
	WritePrivateProfileString(lpSection, lpName, lpValue, GetCfgPath());
}
//////////////////////////////////////////////////////////////////////
int	CHotkeyHandlerConfig::GetConfigInt(LPCSTR lpSection, LPCSTR lpName, int nDefault)
{
	return GetPrivateProfileInt(lpSection, lpName, nDefault, GetCfgPath());
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerConfig::SetConfigInt(LPCSTR lpSection, LPCSTR lpName, int nValue)
{
	char szValue[MAX_PATH];
	sprintf_s(szValue, sizeof(szValue), "%d", nValue);

	WritePrivateProfileString(lpSection, lpName, szValue, GetCfgPath());
}
//////////////////////////////////////////////////////////////////////
DWORD CHotkeyHandlerConfig::GetConfigHex(LPCSTR lpSection, LPCSTR lpName, DWORD dwDefault)
{
	char szValue[MAX_PATH];
	GetPrivateProfileString(lpSection, lpName, "", szValue, MAX_PATH, GetCfgPath());

	DWORD dwResult = dwDefault; 
	sscanf_s(szValue, "%08X", &dwResult);

	return dwResult;
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerConfig::SetConfigHex(LPCSTR lpSection, LPCSTR lpName, DWORD dwValue)
{
	char szValue[MAX_PATH];
	sprintf_s(szValue, sizeof(szValue), "%08X", dwValue);

	WritePrivateProfileString(lpSection, lpName, szValue, GetCfgPath());
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerConfig::Load()
{
	m_hotkeyManager.SetHotkey(HOTKEY_OSD_ON					, GetConfigHex("Settings", "OSDOnHotkey"			, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_OSD_OFF				, GetConfigHex("Settings", "OSDOffHotkey"			, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_OSD_TOGGLE				, GetConfigHex("Settings", "OSDToggleHotkey"		, 0));

	m_hotkeyManager.SetHotkey(HOTKEY_LIMITER_ON				, GetConfigHex("Settings", "LimiterOnHotkey"		, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_LIMITER_OFF			, GetConfigHex("Settings", "LimiterOffHotkey"		, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_LIMITER_TOGGLE			, GetConfigHex("Settings", "LimiterToggleHotkey"	, 0));

	m_hotkeyManager.SetHotkey(HOTKEY_SCREEN_CAPTURE			, GetConfigHex("Settings", "ScreenCaptureHotkey"	, 0));

	m_hotkeyManager.SetHotkey(HOTKEY_VIDEO_CAPTURE			, GetConfigHex("Settings", "VideoCaptureHotkey"		, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_VIDEO_CAPTURE_PTT		, GetConfigHex("Settings", "PTTHotkey"				, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_VIDEO_CAPTURE_PTT2		, GetConfigHex("Settings", "PTT2Hotkey"				, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_VIDEO_PRERECORD		, GetConfigHex("Settings", "VideoPrerecordHotkey"	, 0));

	m_hotkeyManager.SetHotkey(HOTKEY_BENCHMARK_BEGIN		, GetConfigHex("Settings", "BenchmarkBeginHotkey"	, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_BENCHMARK_END			, GetConfigHex("Settings", "BenchmarkEndHotkey"		, 0));

	m_hotkeyManager.SetHotkey(HOTKEY_PROFILE_MODIFIER1		, GetConfigHex("Settings", "PPM1Hotkey"				, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_PROFILE_MODIFIER2		, GetConfigHex("Settings", "PPM2Hotkey"				, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_PROFILE_MODIFIER3		, GetConfigHex("Settings", "PPM3Hotkey"				, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_PROFILE_MODIFIER4		, GetConfigHex("Settings", "PPM4Hotkey"				, 0));

	m_hotkeyManager.SetHotkey(HOTKEY_OVERLAY_MESSAGE1		, GetConfigHex("Settings", "OVM1Hotkey"				, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_OVERLAY_MESSAGE2		, GetConfigHex("Settings", "OVM2Hotkey"				, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_OVERLAY_MESSAGE3		, GetConfigHex("Settings", "OVM3Hotkey"				, 0));
	m_hotkeyManager.SetHotkey(HOTKEY_OVERLAY_MESSAGE4		, GetConfigHex("Settings", "OVM4Hotkey"				, 0));

	m_strScreenCaptureFormat	= GetConfigStr("Settings", "ScreenCaptureFormat"		, "png"								);
	m_dwScreenCaptureQuality	= GetConfigInt("Settings", "ScreenCaptureQuality"		, 100								);
	m_strScreenCaptureFolder	= GetConfigStr("Settings", "ScreenCaptureFolder"		, GetDefaultScreenCaptureFolder()	);
	m_bCaptureOSD				= GetConfigInt("Settings", "CaptureOSD"					, 1									) != 0;

	m_strVideoCaptureContainer	= GetConfigStr("Settings", "VideoCaptureContainer"		, "mkv"								);
	m_strVideoCaptureFormat		= GetConfigStr("Settings", "VideoCaptureFormat"			, "MJPG"							);
	m_dwVideoCaptureQuality		= GetConfigInt("Settings", "VideoCaptureQuality"		, 75								);
	m_strVideoCaptureFolder		= GetConfigStr("Settings", "VideoCaptureFolder"			, GetDefaultVideoCaptureFolder()	);
	m_dwVideoCaptureFramesize	= GetConfigHex("Settings", "VideoCaptureFramesize"		, 0									);
	m_dwVideoCaptureFramerate	= GetConfigInt("Settings", "VideoCaptureFramerate"		, 60								);
	m_dwAudioCaptureFlags		= GetConfigHex("Settings", "AudioCaptureFlags"			, 0x00000003						);
	m_dwAudioCaptureFlags2		= GetConfigHex("Settings", "AudioCaptureFlags2"			, 0x00000002						);
	m_dwVideoCaptureFlagsEx		= GetConfigHex("Settings", "VideoCaptureFlagsEx"		, 0x00000000						);
	m_dwPrerecordSizeLimit		= GetConfigInt("Settings", "PrerecordSizeLimit"			, 256								);
	m_dwPrerecordTimeLimit		= GetConfigInt("Settings", "PrerecordTimeLimit"			, 600								);
	m_bAutoPrerecord			= GetConfigInt("Settings", "AutoPrerecord"				, 0									) != 0;

	m_strBenchmarkPath			= GetConfigStr("Settings", "BenchmarkPath"				, GetDefaultBenchmarkPath()			);
	m_bAppendBenchmark			= GetConfigInt("Settings", "AppendBenchmark"			, 0									) != 0;

	m_strPPM1Desc				= GetConfigStr("Settings", "PPM1Desc"					, ""								);
	m_strPPM1Profile			= GetConfigStr("Settings", "PPM1Profile"				, ""								);
	m_strPPM1Property			= GetConfigStr("Settings", "PPM1Property"				, ""								);
	m_dwPPM1Type				= GetConfigInt("Settings", "PPM1Type"					, 0									);
	m_dwPPM1Value				= GetConfigInt("Settings", "PPM1Value"					, 0									);

	m_strPPM2Desc				= GetConfigStr("Settings", "PPM2Desc"					, ""								);
	m_strPPM2Profile			= GetConfigStr("Settings", "PPM2Profile"				, ""								);
	m_strPPM2Property			= GetConfigStr("Settings", "PPM2Property"				, ""								);
	m_dwPPM2Type				= GetConfigInt("Settings", "PPM2Type"					, 0									);
	m_dwPPM2Value				= GetConfigInt("Settings", "PPM2Value"					, 0									);

	m_strPPM3Desc				= GetConfigStr("Settings", "PPM3Desc"					, ""								);
	m_strPPM3Profile			= GetConfigStr("Settings", "PPM3Profile"				, ""								);
	m_strPPM3Property			= GetConfigStr("Settings", "PPM3Property"				, ""								);
	m_dwPPM3Type				= GetConfigInt("Settings", "PPM3Type"					, 0									);
	m_dwPPM3Value				= GetConfigInt("Settings", "PPM3Value"					, 0									);

	m_strPPM4Desc				= GetConfigStr("Settings", "PPM4Desc"					, ""								);
	m_strPPM4Profile			= GetConfigStr("Settings", "PPM4Profile"				, ""								);
	m_strPPM4Property			= GetConfigStr("Settings", "PPM4Property"				, ""								);
	m_dwPPM4Type				= GetConfigInt("Settings", "PPM4Type"					, 0									);
	m_dwPPM4Value				= GetConfigInt("Settings", "PPM4Value"					, 0									);

	m_strOVM1Desc				= GetConfigStr("Settings", "OVM1Desc"					, ""								);
	m_strOVM1Message			= GetConfigStr("Settings", "OVM1Message"				, ""								);
	m_strOVM1Layer				= GetConfigStr("Settings", "OVM1Layer"					, ""								);
	m_strOVM1Params				= GetConfigStr("Settings", "OVM1Params"					, ""								);

	m_strOVM2Desc				= GetConfigStr("Settings", "OVM2Desc"					, ""								);
	m_strOVM2Message			= GetConfigStr("Settings", "OVM2Message"				, ""								);
	m_strOVM2Layer				= GetConfigStr("Settings", "OVM2Layer"					, ""								);
	m_strOVM2Params				= GetConfigStr("Settings", "OVM2Params"					, ""								);


	m_strOVM3Desc				= GetConfigStr("Settings", "OVM3Desc"					, ""								);
	m_strOVM3Message			= GetConfigStr("Settings", "OVM3Message"				, ""								);
	m_strOVM3Layer				= GetConfigStr("Settings", "OVM3Layer"					, ""								);
	m_strOVM3Params				= GetConfigStr("Settings", "OVM3Params"					, ""								);

	m_strOVM4Desc				= GetConfigStr("Settings", "OVM4Desc"					, ""								);
	m_strOVM4Message			= GetConfigStr("Settings", "OVM4Message"				, ""								);
	m_strOVM4Layer				= GetConfigStr("Settings", "OVM4Layer"					, ""								);
	m_strOVM4Params				= GetConfigStr("Settings", "OVM4Params"					, ""								);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerConfig::Save()
{
	SetConfigHex("Settings", "OSDOnHotkey"				, m_hotkeyManager.GetHotkey(HOTKEY_OSD_ON				));
	SetConfigHex("Settings", "OSDOffHotkey"				, m_hotkeyManager.GetHotkey(HOTKEY_OSD_OFF				));
	SetConfigHex("Settings", "OSDToggleHotkey"			, m_hotkeyManager.GetHotkey(HOTKEY_OSD_TOGGLE			));

	SetConfigHex("Settings", "LimiterOnHotkey"			, m_hotkeyManager.GetHotkey(HOTKEY_LIMITER_ON			));
	SetConfigHex("Settings", "LimiterOffHotkey"			, m_hotkeyManager.GetHotkey(HOTKEY_LIMITER_OFF			));
	SetConfigHex("Settings", "LimiterToggleHotkey"		, m_hotkeyManager.GetHotkey(HOTKEY_LIMITER_TOGGLE		));

	SetConfigHex("Settings", "ScreenCaptureHotkey"		, m_hotkeyManager.GetHotkey(HOTKEY_SCREEN_CAPTURE		));

	SetConfigHex("Settings", "VideoCaptureHotkey"		, m_hotkeyManager.GetHotkey(HOTKEY_VIDEO_CAPTURE		));
	SetConfigHex("Settings", "PTTHotkey"				, m_hotkeyManager.GetHotkey(HOTKEY_VIDEO_CAPTURE_PTT	));
	SetConfigHex("Settings", "PTT2Hotkey"				, m_hotkeyManager.GetHotkey(HOTKEY_VIDEO_CAPTURE_PTT2	));
	SetConfigHex("Settings", "VideoPrerecordHotkey"		, m_hotkeyManager.GetHotkey(HOTKEY_VIDEO_PRERECORD		));

	SetConfigHex("Settings", "BenchmarkBeginHotkey"		, m_hotkeyManager.GetHotkey(HOTKEY_BENCHMARK_BEGIN		));
	SetConfigHex("Settings", "BenchmarkEndHotkey"		, m_hotkeyManager.GetHotkey(HOTKEY_BENCHMARK_END		));

	SetConfigHex("Settings", "PPM1Hotkey"				, m_hotkeyManager.GetHotkey(HOTKEY_PROFILE_MODIFIER1	));
	SetConfigHex("Settings", "PPM2Hotkey"				, m_hotkeyManager.GetHotkey(HOTKEY_PROFILE_MODIFIER2	));
	SetConfigHex("Settings", "PPM3Hotkey"				, m_hotkeyManager.GetHotkey(HOTKEY_PROFILE_MODIFIER3	));
	SetConfigHex("Settings", "PPM4Hotkey"				, m_hotkeyManager.GetHotkey(HOTKEY_PROFILE_MODIFIER4	));

	SetConfigHex("Settings", "OVM1Hotkey"				, m_hotkeyManager.GetHotkey(HOTKEY_OVERLAY_MESSAGE1		));
	SetConfigHex("Settings", "OVM2Hotkey"				, m_hotkeyManager.GetHotkey(HOTKEY_OVERLAY_MESSAGE2		));
	SetConfigHex("Settings", "OVM3Hotkey"				, m_hotkeyManager.GetHotkey(HOTKEY_OVERLAY_MESSAGE3		));
	SetConfigHex("Settings", "OVM4Hotkey"				, m_hotkeyManager.GetHotkey(HOTKEY_OVERLAY_MESSAGE4		));

	SetConfigStr("Settings", "ScreenCaptureFormat"		, m_strScreenCaptureFormat);
	SetConfigInt("Settings", "ScreenCaptureQuality"		, m_dwScreenCaptureQuality);
	SetConfigStr("Settings", "ScreenCaptureFolder"		, m_strScreenCaptureFolder);
	SetConfigInt("Settings", "CaptureOSD"				, m_bCaptureOSD ? 1 : 0);

	SetConfigStr("Settings", "VideoCaptureContainer"	, m_strVideoCaptureContainer);
	SetConfigStr("Settings", "VideoCaptureFormat"		, m_strVideoCaptureFormat);
	SetConfigInt("Settings", "VideoCaptureQuality"		, m_dwVideoCaptureQuality);
	SetConfigStr("Settings", "VideoCaptureFolder"		, m_strVideoCaptureFolder);
	SetConfigHex("Settings", "VideoCaptureFramesize"	, m_dwVideoCaptureFramesize);
	SetConfigInt("Settings", "VideoCaptureFramerate"	, m_dwVideoCaptureFramerate);
	SetConfigHex("Settings", "AudioCaptureFlags"		, m_dwAudioCaptureFlags);
	SetConfigHex("Settings", "AudioCaptureFlags2"		, m_dwAudioCaptureFlags2);
	SetConfigHex("Settings", "VideoCaptureFlagsEx"		, m_dwVideoCaptureFlagsEx);
	SetConfigInt("Settings", "PrerecordSizeLimit"		, m_dwPrerecordSizeLimit);
	SetConfigInt("Settings", "PrerecordTimeLimit"		, m_dwPrerecordTimeLimit);
	SetConfigInt("Settings", "AutoPrerecord"			, m_bAutoPrerecord ? 1 : 0);

	SetConfigStr("Settings", "BenchmarkPath"			, m_strBenchmarkPath);
	SetConfigInt("Settings", "AppendBenchmark"			, m_bAppendBenchmark ? 1 : 0);

	SetConfigStr("Settings", "PPM1Desc"					, m_strPPM1Desc);
	SetConfigStr("Settings", "PPM1Profile"				, m_strPPM1Profile);
	SetConfigStr("Settings", "PPM1Property"				, m_strPPM1Property);
	SetConfigInt("Settings", "PPM1Type"					, m_dwPPM1Type);
	SetConfigInt("Settings", "PPM1Value"				, m_dwPPM1Value);

	SetConfigStr("Settings", "PPM2Desc"					, m_strPPM2Desc);
	SetConfigStr("Settings", "PPM2Profile"				, m_strPPM2Profile);
	SetConfigStr("Settings", "PPM2Property"				, m_strPPM2Property);
	SetConfigInt("Settings", "PPM2Type"					, m_dwPPM2Type);
	SetConfigInt("Settings", "PPM2Value"				, m_dwPPM2Value);

	SetConfigStr("Settings", "PPM3Desc"					, m_strPPM3Desc);
	SetConfigStr("Settings", "PPM3Profile"				, m_strPPM3Profile);
	SetConfigStr("Settings", "PPM3Property"				, m_strPPM3Property);
	SetConfigInt("Settings", "PPM3Type"					, m_dwPPM3Type);
	SetConfigInt("Settings", "PPM3Value"				, m_dwPPM3Value);

	SetConfigStr("Settings", "PPM4Desc"					, m_strPPM4Desc);
	SetConfigStr("Settings", "PPM4Profile"				, m_strPPM4Profile);
	SetConfigStr("Settings", "PPM4Property"				, m_strPPM4Property);
	SetConfigInt("Settings", "PPM4Type"					, m_dwPPM4Type);
	SetConfigInt("Settings", "PPM4Value"				, m_dwPPM4Value);

	SetConfigStr("Settings", "OVM1Desc"					, m_strOVM1Desc);
	SetConfigStr("Settings", "OVM1Message"				, m_strOVM1Message);
	SetConfigStr("Settings", "OVM1Layer"				, m_strOVM1Layer);
	SetConfigStr("Settings", "OVM1Params"				, m_strOVM1Params);

	SetConfigStr("Settings", "OVM2Desc"					, m_strOVM2Desc);
	SetConfigStr("Settings", "OVM2Message"				, m_strOVM2Message);
	SetConfigStr("Settings", "OVM2Layer"				, m_strOVM2Layer);
	SetConfigStr("Settings", "OVM2Params"				, m_strOVM2Params);

	SetConfigStr("Settings", "OVM3Desc"					, m_strOVM3Desc);
	SetConfigStr("Settings", "OVM3Message"				, m_strOVM3Message);
	SetConfigStr("Settings", "OVM3Layer"				, m_strOVM3Layer);
	SetConfigStr("Settings", "OVM3Params"				, m_strOVM3Params);

	SetConfigStr("Settings", "OVM4Desc"					, m_strOVM4Desc);
	SetConfigStr("Settings", "OVM4Message"				, m_strOVM4Message);
	SetConfigStr("Settings", "OVM4Layer"				, m_strOVM4Layer);
	SetConfigStr("Settings", "OVM4Params"				, m_strOVM4Params);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerConfig::Copy(CHotkeyHandlerConfig* pSrc)
{
	m_hotkeyManager.Copy(&pSrc->m_hotkeyManager);

	m_strScreenCaptureFormat	= pSrc->m_strScreenCaptureFormat;
	m_dwScreenCaptureQuality	= pSrc->m_dwScreenCaptureQuality;
	m_strScreenCaptureFolder	= pSrc->m_strScreenCaptureFolder;
	m_bCaptureOSD				= pSrc->m_bCaptureOSD;

	m_strVideoCaptureContainer	= pSrc->m_strVideoCaptureContainer;
	m_strVideoCaptureFormat		= pSrc->m_strVideoCaptureFormat;
	m_dwVideoCaptureQuality		= pSrc->m_dwVideoCaptureQuality;
	m_strVideoCaptureFolder		= pSrc->m_strVideoCaptureFolder;
	m_dwVideoCaptureFramesize	= pSrc->m_dwVideoCaptureFramesize;
	m_dwVideoCaptureFramerate	= pSrc->m_dwVideoCaptureFramerate;
	m_dwAudioCaptureFlags		= pSrc->m_dwAudioCaptureFlags;
	m_dwAudioCaptureFlags2		= pSrc->m_dwAudioCaptureFlags2;
	m_dwVideoCaptureFlagsEx		= pSrc->m_dwVideoCaptureFlagsEx;
	m_dwPrerecordSizeLimit		= pSrc->m_dwPrerecordSizeLimit;
	m_dwPrerecordTimeLimit		= pSrc->m_dwPrerecordTimeLimit;
	m_bAutoPrerecord			= pSrc->m_bAutoPrerecord;

	m_strBenchmarkPath			= pSrc->m_strBenchmarkPath;
	m_bAppendBenchmark			= pSrc->m_bAppendBenchmark;

	m_strPPM1Desc				= pSrc->m_strPPM1Desc;
	m_strPPM1Profile			= pSrc->m_strPPM1Profile;
	m_strPPM1Property			= pSrc->m_strPPM1Property;
	m_dwPPM1Type				= pSrc->m_dwPPM1Type;
	m_dwPPM1Value				= pSrc->m_dwPPM1Value;

	m_strPPM2Desc				= pSrc->m_strPPM2Desc;
	m_strPPM2Profile			= pSrc->m_strPPM2Profile;
	m_strPPM2Property			= pSrc->m_strPPM2Property;
	m_dwPPM2Type				= pSrc->m_dwPPM2Type;
	m_dwPPM2Value				= pSrc->m_dwPPM2Value;

	m_strPPM3Desc				= pSrc->m_strPPM3Desc;
	m_strPPM3Profile			= pSrc->m_strPPM3Profile;
	m_strPPM3Property			= pSrc->m_strPPM3Property;
	m_dwPPM3Type				= pSrc->m_dwPPM3Type;
	m_dwPPM3Value				= pSrc->m_dwPPM3Value;

	m_strPPM4Desc				= pSrc->m_strPPM4Desc;
	m_strPPM4Profile			= pSrc->m_strPPM4Profile;
	m_strPPM4Property			= pSrc->m_strPPM4Property;
	m_dwPPM4Type				= pSrc->m_dwPPM4Type;
	m_dwPPM4Value				= pSrc->m_dwPPM4Value;

	m_strOVM1Desc				= pSrc->m_strOVM1Desc;
	m_strOVM1Message			= pSrc->m_strOVM1Message;
	m_strOVM1Layer				= pSrc->m_strOVM1Layer;
	m_strOVM1Params				= pSrc->m_strOVM1Params;

	m_strOVM2Desc				= pSrc->m_strOVM2Desc;
	m_strOVM2Message			= pSrc->m_strOVM2Message;
	m_strOVM2Layer				= pSrc->m_strOVM2Layer;
	m_strOVM2Params				= pSrc->m_strOVM2Params;

	m_strOVM3Desc				= pSrc->m_strOVM3Desc;
	m_strOVM3Message			= pSrc->m_strOVM3Message;
	m_strOVM3Layer				= pSrc->m_strOVM3Layer;
	m_strOVM3Params				= pSrc->m_strOVM3Params;

	m_strOVM4Desc				= pSrc->m_strOVM4Desc;
	m_strOVM4Message			= pSrc->m_strOVM4Message;
	m_strOVM4Layer				= pSrc->m_strOVM4Layer;
	m_strOVM4Params				= pSrc->m_strOVM4Params;
}
//////////////////////////////////////////////////////////////////////
CString CHotkeyHandlerConfig::GetDefaultScreenCaptureFolder()
{
	char szFolder[MAX_PATH];
	SHGetSpecialFolderPath(NULL, szFolder, 0x36 /*CSIDL_COMMON_PICTURES*/, TRUE);	

	return szFolder + CString("\\Screenshots");
}
//////////////////////////////////////////////////////////////////////
CString CHotkeyHandlerConfig::GetDefaultVideoCaptureFolder()
{
	char szFolder[MAX_PATH];
	SHGetSpecialFolderPath(NULL, szFolder, 0x37 /*CSIDL_COMMON_VIDEO*/, TRUE);	

	return szFolder + CString("\\Captured");
}
//////////////////////////////////////////////////////////////////////
CString CHotkeyHandlerConfig::GetDefaultBenchmarkPath()
{
	char szPath[MAX_PATH];
	GetModuleFileName(g_hModule, szPath, MAX_PATH);

	PathRemoveFileSpec(szPath);
	strcat_s(szPath, "\\Benchmark.txt");

	return szPath;
}
//////////////////////////////////////////////////////////////////////
DWORD CHotkeyHandlerConfig::GetVideoCaptureFormatFOURCC()
{
	CString strFormat = m_strVideoCaptureFormat;
	while (strlen(strFormat) < 4)
		strFormat += " ";

	return *(LPDWORD)(LPCSTR)strFormat;
}
//////////////////////////////////////////////////////////////////////

