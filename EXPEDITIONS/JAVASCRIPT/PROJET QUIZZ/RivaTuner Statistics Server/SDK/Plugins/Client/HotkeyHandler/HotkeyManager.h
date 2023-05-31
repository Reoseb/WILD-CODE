// HotkeyManager.h: interface for the CHotkeyManager class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#ifndef _HOTKEYMANAGER_H_INCLUDED_
#define _HOTKEYMANAGER_H_INCLUDED_
//////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////////////////////////////////////////////
#include <afxtempl.h>
//////////////////////////////////////////////////////////////////////
#define HOTKEY_OSD_ON										0x00000000
#define HOTKEY_OSD_OFF										0x00000001
#define HOTKEY_OSD_TOGGLE									0x00000002

#define HOTKEY_LIMITER_ON									0x00000010
#define HOTKEY_LIMITER_OFF									0x00000011
#define HOTKEY_LIMITER_TOGGLE								0x00000012

#define HOTKEY_SCREEN_CAPTURE								0x00000020

#define HOTKEY_VIDEO_CAPTURE								0x00000030
#define HOTKEY_VIDEO_CAPTURE_PTT							0x00000031
#define HOTKEY_VIDEO_CAPTURE_PTT2							0x00000032
#define HOTKEY_VIDEO_PRERECORD								0x00000033

#define HOTKEY_BENCHMARK_BEGIN								0x00000040
#define HOTKEY_BENCHMARK_END								0x00000041

#define HOTKEY_PROFILE_MODIFIER1							0x00000050
#define HOTKEY_PROFILE_MODIFIER2							0x00000051
#define HOTKEY_PROFILE_MODIFIER3							0x00000052
#define HOTKEY_PROFILE_MODIFIER4							0x00000053

#define HOTKEY_OVERLAY_MESSAGE1								0x00000060
#define HOTKEY_OVERLAY_MESSAGE2								0x00000061
#define HOTKEY_OVERLAY_MESSAGE3								0x00000062
#define HOTKEY_OVERLAY_MESSAGE4								0x00000063

//////////////////////////////////////////////////////////////////////
typedef struct HOTKEY_DESC
{
	DWORD dwHotkeyID;
	DWORD dwHotkey;
	DWORD dwHotkey2;
		//the same physical hotkeys may have alternate encodings, e.g. numpad keys are encoded differently in numlock ON and OFF states
		//so we store and handle both encodings
} HOTKEY_DESC, *LPHOTKEY_DESC;
//////////////////////////////////////////////////////////////////////
class CHotkeyManager : public CList<LPHOTKEY_DESC, LPHOTKEY_DESC>
{
public:
	CHotkeyManager();
	virtual ~CHotkeyManager();

	void	Copy(CHotkeyManager* pSrc);
	void	Destroy();

	void	SetHotkey(DWORD dwHotkeyID, DWORD dwHotkey);
	DWORD	GetHotkey(DWORD dwHotkeyID);
	DWORD	GetAlternateHotkey(DWORD dwHotkeyID);
	CString GetHotkeyStr(DWORD dwHotkey);

	BOOL ValidateHotkey(DWORD dwHotkeyID, DWORD dwHotkey);

	LPHOTKEY_DESC FindByID(DWORD dwHotkeyID);
	LPHOTKEY_DESC FindByHotkey(DWORD dwHotkey);
};
//////////////////////////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////////////////////////
