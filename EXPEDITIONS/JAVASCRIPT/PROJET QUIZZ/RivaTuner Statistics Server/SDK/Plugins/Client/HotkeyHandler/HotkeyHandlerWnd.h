// HotkeyHandlerWnd.h: interface for the CHotkeyHandlerWnd class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
// CHotkeyHandlerWnd
//////////////////////////////////////////////////////////////////////
#include "HotkeyListener.h"
#include "HotkeyManager.h"
#include "HotkeyHandlerConfig.h"
#include "RTSSHooksInterface.h"
#include "OverlayEditorInterface.h"
//////////////////////////////////////////////////////////////////////
class CHotkeyHandlerWnd : public CWnd
{
	DECLARE_DYNAMIC(CHotkeyHandlerWnd)

public:
	CHotkeyHandlerWnd();
	virtual ~CHotkeyHandlerWnd();

	//hotkey handling

	void	ProcessHotkeys(BOOL bProcessHotkeys);
		//enables/disables hotkey events processing

		//NOTE: We'll use it to forcibly disable hotkey handler when hotkey editing GUI is open. It is is done this way to prevent
		//you from spamming unwanted screenshots when editing a hotkey associated with screen capture and so on

	void	OnHotkey(DWORD dwHotkey);
		//common hotkey handler and hotkey listener callback, called for both hotkey press and release events

	void	OnOpenPluginsTab();
		//reserved Ctrl+P hotkey handler, we use it to open "Plugins" tab in RTSS properties 

	void	OnOSDOn();
	void	OnOSDOff();
	void	OnOSDToggle();
		//OSD visibility control related hotkey handlers

	void	OnLimiterOn();
	void	OnLimiterOff();
	void	OnLimiterToggle();
		//framerate limiter control related hotkey handlers

	void	OnScreenCapture();
		//screeen capture hotkey handler

	void	OnVideoCapture(DWORD dwAdditionalVideoCaptureFlagsEx);
	void	OnPTTHotkey(BOOL bPush, DWORD dwTrack);
		//video capture related hotkey handlers

	void	OnBenchmarkBegin();
	void	OnBenchmarkEnd();
		//benchmark related hotkey handlers

	void	OnProfileModifier(LPCSTR lpProfile, LPCSTR lpProperty, DWORD dwType, DWORD dwValue);	
		//programmable profile modifier handler
	void	OnOverlayMessage(LPCSTR lpMessage, LPCSTR lpLayer, LPCSTR lpParams);	
		//overlay editor message handler

	//automatic video prerecord
	
	void	StartAutoPrerecord();
		//start automatic video prerecord
	void	StopAutoPrerecord();
		//stop automatic video prerecord
	void	AutoPrerecordProc(BOOL bForceStart);
		//main automatic video prerecord routine, (re)starts/stops prerecording depending on its current state
	BOOL	GetAutoPrerecordState();
		//get automatic prerecord state via RTSSHooks shared memory

	//misc
	
	void	ResetHotkeyListenerState();
		//Any forms of keyboard input are suppressed during lock screen, which can be tricky and may result in our keyboard listerner wrongly seeing
		//some keys as pressed if user press them before entering lock screen (e.g. Ctrl+Alt+Del) and release them while lock screen is active. 
		//In this case we never receive pressed key release events due to supressed input at that stage. To bypass it we need to reset state of our
		//listerner (which internally keeps pressed keys statå) after leaving lock screen. We intercept this by hooking desktop switch events 

	CHotkeyHandlerConfig m_config;
		//stores full configuration including hotkeys mapping and settings for each hotkey event handler

protected:
	DECLARE_MESSAGE_MAP()

	CHotkeyListener m_hotkeyListener;
		//low-latency DirectInput based hotkey listener, registers keybooard events and passses them to our OnHotkey callback
	CRTSSHooksInterface	m_rtssInterface;
		//interface for RTSSHooks.dll funtionality
	COverlayEditorInterface m_overlayEditorInterface;
		//interface for OverlayEditor.dll functionality

	BOOL	m_bProcessHotkeys;
		//enables/disables hotkey events processing

		//NOTE: We'll use it to forcibly disable hotkeys handler when hotkey editing GUI is open. It is is done this way to prevent
		//you from spamming unwanted screenshots when editing a hotkey associated with screen capture and so on

	HWINEVENTHOOK m_hWinEventHook;
		//windows event hook, we'll use it to detect desktop switch event to intercept lock scren enter/leave events for proper hotkeys handling

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
//////////////////////////////////////////////////////////////////////


