// HotkeyListener.h: interface for the CHotkeyListener class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#ifndef _HOTKEYLISTENER_H_INCLUDED_
#define _HOTKEYLISTENER_H_INCLUDED_
//////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif
//////////////////////////////////////////////////////////////////////
#define DIRECTINPUT_VERSION 0x800

#include <dinput.h>
//////////////////////////////////////////////////////////////////////
#define HOTKEYLISTENERMODE_DIRECTINPUT								0
#define HOTKEYLISTENERMODE_RAWINPUT									1
//////////////////////////////////////////////////////////////////////
class CHotkeyListenerDirectInputThread;
class CHotkeyListenerRawInput;
class CHotkeyHandlerWnd;
class CHotkeyListener  
{
public:
	void	Init(CHotkeyHandlerWnd* pWnd);
	void	Uninit();

	void	SetMode(DWORD dwMode);
	DWORD	GetMode();

	void	OnRawInput(HRAWINPUT hRawInput);

	void	ResetState();

	CHotkeyListener();
	virtual ~CHotkeyListener();

protected:
	CHotkeyListenerDirectInputThread*	m_pDirectInputThread;
	CHotkeyListenerRawInput*			m_pRawInput;	

	DWORD	m_dwMode;
};
//////////////////////////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////////////////////////
