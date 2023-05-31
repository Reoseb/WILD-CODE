// HotkeyHandlerWnd.cpp: implementation of the CHotkeyHandlerWnd class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HotkeyHandler.h"
#include "HotkeyHandlerWnd.h"
#include "SaveMediaTypes.h"
#include "RTSSSharedMemory.h"
//////////////////////////////////////////////////////////////////////
// CHotkeyHandlerWnd
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CHotkeyHandlerWnd, CWnd)
//////////////////////////////////////////////////////////////////////
CHotkeyHandlerWnd::CHotkeyHandlerWnd()
{
	m_bProcessHotkeys			= TRUE;

	m_hWinEventHook				= NULL;
}
//////////////////////////////////////////////////////////////////////
CHotkeyHandlerWnd::~CHotkeyHandlerWnd()
{
}
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CHotkeyHandlerWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////
// CHotkeyHandlerWnd message handlers
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnHotkey(DWORD dwHotkey)
{
	if (m_bProcessHotkeys)
	{
		if (dwHotkey & 0x80000000)
			//handle hotkey release events
		{
			//handle user defined hotkeys

			LPHOTKEY_DESC pDesc = m_config.m_hotkeyManager.FindByHotkey(dwHotkey & 0x7fffffff);

			if (pDesc)
			{
				switch (pDesc->dwHotkeyID)
				{
				case HOTKEY_VIDEO_CAPTURE_PTT:
					OnPTTHotkey(FALSE, 0);
					break;
				case HOTKEY_VIDEO_CAPTURE_PTT2:
					OnPTTHotkey(FALSE, 1);
					break;
				}
			}
		}
		else
			//handle hotkey press events
		{
			//handle reserved hotkeys

			switch (dwHotkey)
			{
			case 0x00020050:	//Ctrl+P is reserved to open "Plugins" tab in RTSS properties
				OnOpenPluginsTab();
				break;
			}

			//handle user defined hotkeys
		
			LPHOTKEY_DESC pDesc = m_config.m_hotkeyManager.FindByHotkey(dwHotkey);

			if (pDesc)
			{
				switch (pDesc->dwHotkeyID)
				{
				case HOTKEY_OSD_ON:
					OnOSDOn();
					break;
				case HOTKEY_OSD_OFF:
					OnOSDOff();
					break;
				case HOTKEY_OSD_TOGGLE:
					OnOSDToggle();
					break;

				case HOTKEY_LIMITER_ON:
					OnLimiterOn();
					break;
				case HOTKEY_LIMITER_OFF:
					OnLimiterOff();
					break;
				case HOTKEY_LIMITER_TOGGLE:
					OnLimiterToggle();
					break;

				case HOTKEY_SCREEN_CAPTURE:
					OnScreenCapture();
					break;

				case HOTKEY_VIDEO_CAPTURE:
					OnVideoCapture(0);
					break;
				case HOTKEY_VIDEO_CAPTURE_PTT:
					OnPTTHotkey(TRUE, 0);
					break;
				case HOTKEY_VIDEO_CAPTURE_PTT2:
					OnPTTHotkey(TRUE, 1);
					break;
				case HOTKEY_VIDEO_PRERECORD:
					OnVideoCapture(SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_PRERECORD);
					break;

				case HOTKEY_BENCHMARK_BEGIN:
					OnBenchmarkBegin();
					break;
				case HOTKEY_BENCHMARK_END:
					OnBenchmarkEnd();
					break;

				case HOTKEY_PROFILE_MODIFIER1:
					OnProfileModifier(m_config.m_strPPM1Profile, m_config.m_strPPM1Property, m_config.m_dwPPM1Type, m_config.m_dwPPM1Value);
					break;
				case HOTKEY_PROFILE_MODIFIER2:
					OnProfileModifier(m_config.m_strPPM2Profile, m_config.m_strPPM2Property, m_config.m_dwPPM2Type, m_config.m_dwPPM2Value);
					break;
				case HOTKEY_PROFILE_MODIFIER3:
					OnProfileModifier(m_config.m_strPPM3Profile, m_config.m_strPPM3Property, m_config.m_dwPPM3Type, m_config.m_dwPPM3Value);
					break;
				case HOTKEY_PROFILE_MODIFIER4:
					OnProfileModifier(m_config.m_strPPM4Profile, m_config.m_strPPM4Property, m_config.m_dwPPM4Type, m_config.m_dwPPM4Value);
					break;

				case HOTKEY_OVERLAY_MESSAGE1:
					OnOverlayMessage(m_config.m_strOVM1Message, m_config.m_strOVM1Layer, m_config.m_strOVM1Params);
					break;
				case HOTKEY_OVERLAY_MESSAGE2:
					OnOverlayMessage(m_config.m_strOVM2Message, m_config.m_strOVM2Layer, m_config.m_strOVM2Params);
					break;
				case HOTKEY_OVERLAY_MESSAGE3:
					OnOverlayMessage(m_config.m_strOVM3Message, m_config.m_strOVM3Layer, m_config.m_strOVM3Params);
					break;
				case HOTKEY_OVERLAY_MESSAGE4:
					OnOverlayMessage(m_config.m_strOVM4Message, m_config.m_strOVM4Layer, m_config.m_strOVM4Params);
					break;
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////
void CALLBACK WinEventCallback(HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	if (dwEvent == EVENT_SYSTEM_DESKTOPSWITCH)
		g_clientWnd.ResetHotkeyListenerState();
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::ResetHotkeyListenerState()
{
	m_hotkeyListener.ResetState();
}
//////////////////////////////////////////////////////////////////////
int CHotkeyHandlerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_config.Load();
	
	m_hWinEventHook = SetWinEventHook(EVENT_SYSTEM_DESKTOPSWITCH, EVENT_SYSTEM_DESKTOPSWITCH, NULL, WinEventCallback, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNTHREAD);

	m_hotkeyListener.SetMode(HOTKEYLISTENERMODE_DIRECTINPUT);
	m_hotkeyListener.Init(this);

	AutoPrerecordProc(FALSE);

	return 0;
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnDestroy()
{
	m_hotkeyListener.Uninit();

	if (m_hWinEventHook)
	{
		UnhookWinEvent(m_hWinEventHook);

		m_hWinEventHook = NULL;
	}

	CWnd::OnDestroy();
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnOpenPluginsTab()
{
	m_rtssInterface.PostMessage(WM_RTSS_SHOW_PROPERTIES, 1 /*zero based tab index*/, 1 /*require foreground*/);
		//display the second tab of RTSS properties if RTSS window is in foreground
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnOSDOn()
{
	m_rtssInterface.SetFlags(~RTSSHOOKSFLAG_OSD_VISIBLE, RTSSHOOKSFLAG_OSD_VISIBLE);
}
/////////////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnOSDOff()
{
	m_rtssInterface.SetFlags(~RTSSHOOKSFLAG_OSD_VISIBLE, 0);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnOSDToggle()
{
	m_rtssInterface.SetFlags(0xFFFFFFFF, RTSSHOOKSFLAG_OSD_VISIBLE);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnLimiterOn()
{
	m_rtssInterface.SetFlags(~RTSSHOOKSFLAG_LIMITER_DISABLED, 0);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnLimiterOff()
{
	m_rtssInterface.SetFlags(~RTSSHOOKSFLAG_LIMITER_DISABLED, RTSSHOOKSFLAG_LIMITER_DISABLED);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnLimiterToggle()
{
	m_rtssInterface.SetFlags(0xFFFFFFFF, RTSSHOOKSFLAG_LIMITER_DISABLED);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnProfileModifier(LPCSTR lpProfile, LPCSTR lpProperty, DWORD dwType, DWORD dwValue)
{
	m_rtssInterface.LoadProfile(lpProfile);

	switch (dwType)
	{
	case 0:		//set type
		if (m_rtssInterface.SetProfileProperty(lpProperty, (LPBYTE)&dwValue, sizeof(dwValue)))
		{
			m_rtssInterface.SaveProfile(lpProfile);
			m_rtssInterface.UpdateProfiles();
		}
		else
			MessageBeep(MB_ICONERROR);
				//indicate unsupported property name by error beep
		break;
	case 1:		//add type
		{
			DWORD dwTemp;

			if (m_rtssInterface.GetProfileProperty(lpProperty, (LPBYTE)&dwTemp, sizeof(dwTemp)))
			{
				dwTemp += dwValue;

				if (m_rtssInterface.SetProfileProperty(lpProperty, (LPBYTE)&dwTemp, sizeof(dwTemp)))
				{
					m_rtssInterface.SaveProfile(lpProfile);
					m_rtssInterface.UpdateProfiles();
				}
			}
			else
				MessageBeep(MB_ICONERROR);
					//indicate unsupported property name by error beep
		}
		break;
	}
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnOverlayMessage(LPCSTR lpMessage, LPCSTR lpLayer, LPCSTR lpParams)
{
	m_overlayEditorInterface.PostOverlayMessage(lpMessage, lpLayer, lpParams);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnScreenCapture()
{
	m_rtssInterface.ScreenCapture(m_config.m_strScreenCaptureFolder + "\\%App%_%Time%." + m_config.m_strScreenCaptureFormat, m_config.m_dwScreenCaptureQuality, 0xFFFFFFFF, m_config.m_bCaptureOSD);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnVideoCapture(DWORD dwAdditionalVideoCaptureFlagsEx)
{
	VIDEO_CAPTURE_PARAM param;
	ZeroMemory(&param, sizeof(param));

	param.dwVersion				= 0x00010000;

	strcpy_s(param.szFilename, sizeof(param.szFilename), m_config.m_strVideoCaptureFolder + "\\%App%_%Time%." + m_config.m_strVideoCaptureContainer);

	param.dwFramerate			= m_config.m_dwVideoCaptureFramerate;
	param.dwFramesize			= m_config.m_dwVideoCaptureFramesize;
	param.dwFormat				= m_config.GetVideoCaptureFormatFOURCC();
	param.dwQuality				= m_config.m_dwVideoCaptureQuality;
	param.dwThreads				= 0xFFFFFFFF;
	param.bCaptureOSD			= m_config.m_bCaptureOSD;

	param.dwAudioCaptureFlags	= m_config.m_dwAudioCaptureFlags;
	param.dwAudioCaptureFlags2	= m_config.m_dwAudioCaptureFlags2;
	param.dwVideoCaptureFlagsEx = m_config.m_dwVideoCaptureFlagsEx | dwAdditionalVideoCaptureFlagsEx;
	param.dwPrerecordSizeLimit	= m_config.m_dwPrerecordSizeLimit;
	param.dwPrerecordTimeLimit	= m_config.m_dwPrerecordTimeLimit;

	m_rtssInterface.VideoCaptureEx(&param);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::StartAutoPrerecord()
{
	OnVideoCapture(SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_PRERECORD | SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_AUTO_START);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::StopAutoPrerecord()
{
	OnVideoCapture(SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_PRERECORD | SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_AUTO_STOP);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnPTTHotkey(BOOL bPush, DWORD dwTrack)
{
	m_rtssInterface.PTTEvent(bPush, dwTrack);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnBenchmarkBegin()
{
	m_rtssInterface.BeingRecord();
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::OnBenchmarkEnd()
{
	m_rtssInterface.EndRecord(m_config.m_strBenchmarkPath, m_config.m_bAppendBenchmark);
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::ProcessHotkeys(BOOL bProcessHotkeys)
{
	m_bProcessHotkeys = bProcessHotkeys;
}
//////////////////////////////////////////////////////////////////////
BOOL CHotkeyHandlerWnd::GetAutoPrerecordState()
{
	BOOL bEnabled	= FALSE;

	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "RTSSSharedMemoryV2");

	if (hMapFile)
	{
		LPVOID pMapAddr = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

		LPRTSS_SHARED_MEMORY pMem = (LPRTSS_SHARED_MEMORY)pMapAddr;

		if (pMem)
		{
			if ((pMem->dwSignature == 'RTSS') && (pMem->dwVersion >= 0x0002000d))	
				//WARNING! autoVideoCaptureParam was introduced in v2.9 shared memory, however it has different offset
				//in v2.12 and newer due to adding embedded objects and 1%/0.1% lows suppport, so we check for v2.13 
				//instead of v2.9
			{
				if (pMem->autoVideoCaptureParam.dwVersion)
					bEnabled = TRUE;
			}

			UnmapViewOfFile(pMapAddr);
		}

		CloseHandle(hMapFile);
	}

	return bEnabled;
}
//////////////////////////////////////////////////////////////////////
void CHotkeyHandlerWnd::AutoPrerecordProc(BOOL bForceStart)
{	
	BOOL bAutoPrerecordEnabled = GetAutoPrerecordState();

	if ((m_config.m_bAutoPrerecord != bAutoPrerecordEnabled) || (m_config.m_bAutoPrerecord && bForceStart))
	{
		if (m_config.m_bAutoPrerecord)
			StartAutoPrerecord();
		else
			StopAutoPrerecord();
	}
}
//////////////////////////////////////////////////////////////////////
LRESULT CHotkeyHandlerWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_INPUT)
		m_hotkeyListener.OnRawInput((HRAWINPUT)lParam);
			//pass raw input messages to hotkey listener

	return CWnd::DefWindowProc(message, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////
