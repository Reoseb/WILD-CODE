// HotkeyHandlerConfig.h: interface for the CHotkeyHandlerConfig class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
#include "HotkeyManager.h"
//////////////////////////////////////////////////////////////////////
#define CHAR_BUF_SIZE											4096
//////////////////////////////////////////////////////////////////////
class CHotkeyHandlerConfig
{
public:
	CHotkeyHandlerConfig();
	virtual ~CHotkeyHandlerConfig();

	void	Load();
	void	Save();
	void	Copy(CHotkeyHandlerConfig* pSrc);

	CString	GetCfgPath();

	CString	GetConfigStr(LPCSTR lpSection, LPCSTR lpName, LPCTSTR lpDefault);
	void	SetConfigStr(LPCSTR lpSection, LPCSTR lpName, LPCSTR lpValue);

	int		GetConfigInt(LPCSTR lpSection, LPCSTR lpName, int nDefault);
	void	SetConfigInt(LPCSTR lpSection, LPCSTR lpName, int nValue);
		
	DWORD	GetConfigHex(LPCSTR lpSection, LPCSTR lpName, DWORD dwDefault);
	void	SetConfigHex(LPCSTR lpSection, LPCSTR lpName, DWORD dwValue);
	
	CString	GetDefaultScreenCaptureFolder();
	CString	GetDefaultVideoCaptureFolder();
	CString	GetDefaultBenchmarkPath();

	DWORD	GetVideoCaptureFormatFOURCC();

	//screen capture config

	CString	m_strScreenCaptureFormat;
	DWORD	m_dwScreenCaptureQuality;
	CString	m_strScreenCaptureFolder;
	BOOL	m_bCaptureOSD;	//shared between screen and video capture configs
	
	//video capture config

	CString	m_strVideoCaptureContainer;
	CString	m_strVideoCaptureFormat;
	DWORD	m_dwVideoCaptureQuality;
	CString	m_strVideoCaptureFolder;
	DWORD	m_dwVideoCaptureFramesize;
	DWORD	m_dwVideoCaptureFramerate;
	DWORD	m_dwAudioCaptureFlags;
	DWORD	m_dwAudioCaptureFlags2;
	DWORD	m_dwVideoCaptureFlagsEx;
	DWORD	m_dwPrerecordSizeLimit;
	DWORD	m_dwPrerecordTimeLimit;
	BOOL	m_bAutoPrerecord;

	//benchmark config

	CString	m_strBenchmarkPath;
	BOOL	m_bAppendBenchmark;

	//programmable profile modifier 1 config

	CString m_strPPM1Desc;
	CString m_strPPM1Profile;
	CString m_strPPM1Property;
	DWORD	m_dwPPM1Type;
	DWORD	m_dwPPM1Value;

	//programmable profile modifier 2 config

	CString m_strPPM2Desc;
	CString m_strPPM2Profile;
	CString m_strPPM2Property;
	DWORD	m_dwPPM2Type;
	DWORD	m_dwPPM2Value;

	//programmable profile modifier 3 config

	CString m_strPPM3Desc;
	CString m_strPPM3Profile;
	CString m_strPPM3Property;
	DWORD	m_dwPPM3Type;
	DWORD	m_dwPPM3Value;

	//programmable profile modifier 4 config

	CString m_strPPM4Desc;
	CString m_strPPM4Profile;
	CString m_strPPM4Property;
	DWORD	m_dwPPM4Type;
	DWORD	m_dwPPM4Value;

	//overlay message 1 config

	CString m_strOVM1Desc;
	CString m_strOVM1Message;
	CString m_strOVM1Layer;
	CString m_strOVM1Params;

	//overlay message 2 config

	CString m_strOVM2Desc;
	CString m_strOVM2Message;
	CString m_strOVM2Layer;
	CString m_strOVM2Params;

	//overlay message 3 config

	CString m_strOVM3Desc;
	CString m_strOVM3Message;
	CString m_strOVM3Layer;
	CString m_strOVM3Params;

	//overlay message 4 config

	CString m_strOVM4Desc;
	CString m_strOVM4Message;
	CString m_strOVM4Layer;
	CString m_strOVM4Params;

	//hotkey manager, stores all hotkey mappings

	CHotkeyManager m_hotkeyManager;

protected:
	CString	m_strCfgPath;
};
//////////////////////////////////////////////////////////////////////
