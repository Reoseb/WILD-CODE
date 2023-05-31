// HotkeyListenerDirectInputThread.h: interface for the CHotkeyListenerDirectInputThread class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#ifndef _HOTKEYLISTENERDIRECTINPUTTHREAD_H_
#define _HOTKEYLISTENERDIRECTINPUTTHREAD_H_
/////////////////////////////////////////////////////////////////////////////
#include "HotkeyListener.h"
/////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif
/////////////////////////////////////////////////////////////////////////////
#define DIRECTINPUT_VERSION 0x800

#include <dinput.h>
/////////////////////////////////////////////////////////////////////////////
// CHotkeyListenerDirectInputThread thread
/////////////////////////////////////////////////////////////////////////////
class CHotkeyHandlerWnd;
class CHotkeyListenerDirectInputThread : public CWinThread
{
	DECLARE_DYNCREATE(CHotkeyListenerDirectInputThread)
protected:
	CHotkeyListenerDirectInputThread();           // protected constructor used by dynamic creation

// Attributes
public:
	CHotkeyListenerDirectInputThread(CHotkeyHandlerWnd* pWnd);           // protected constructor used by dynamic creation

// Operations
public:
	BOOL Init();
	void Uninit();
	void Destroy();
	void Kill();

	DWORD DIKToVK(DWORD dwCode);
	void ResetState();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHotkeyListenerDirectInputThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HANDLE m_hEventKill;
	HANDLE m_hEventDead;
	HANDLE m_hEventNotification;

	CHotkeyHandlerWnd* m_pWnd;

	LPDIRECTINPUT8			m_lpDirectInput8;
	LPDIRECTINPUTDEVICE8	m_lpDirectInputDevice8;

	DWORD m_dwHotkey;

	virtual ~CHotkeyListenerDirectInputThread();

	// Generated message map functions
	//{{AFX_MSG(CHotkeyListenerDirectInputThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////
