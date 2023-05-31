// DesktopOverlayHostWnd.h: interface for the CDesktopOverlayHostWnd class.
//
// created by Unwinder
/////////////////////////////////////////////////////////////////////////////
#pragma once
/////////////////////////////////////////////////////////////////////////////
#include "MMTimer.h"
#include "DpiAwareHelper.h"
#include "TaskSchedulerInterface.h"

#include <d3d9.h>
/////////////////////////////////////////////////////////////////////////////
// CDesktopOverlayHostWnd
/////////////////////////////////////////////////////////////////////////////
#define CHAR_BUF_SIZE 4096
/////////////////////////////////////////////////////////////////////////////
const UINT WM_TRAYNOTIFICATION	= ::RegisterWindowMessage("WM_TRAYNOTIFICATION");
const UINT WM_TASKBARCREATED	= ::RegisterWindowMessage("TaskbarCreated");
/////////////////////////////////////////////////////////////////////////////
typedef struct NOTIFYICONDATA_V1
 {
    DWORD cbSize;
    HWND hWnd;
    UINT uID;
    UINT uFlags;
    UINT uCallbackMessage;
    HICON hIcon;
    CHAR   szTip[128];
    DWORD dwState;
    DWORD dwStateMask;
    CHAR   szInfo[256];
    union {
        UINT  uTimeout;
        UINT  uVersion;
    } DUMMYUNIONNAME;
    CHAR   szInfoTitle[64];
    DWORD dwInfoFlags;
} NOTIFYICONDATA_V1,*LPNOTIFYICONDATA_V1;
/////////////////////////////////////////////////////////////////////////////
typedef struct NID_DESC
{
	char szName[256];
	NOTIFYICONDATA_V1	nid;
	DWORD				flags;
} NID_DESC, *LPNID_DESC;
/////////////////////////////////////////////////////////////////////////////
class CDesktopOverlayHostWnd : public CWnd
{
	DECLARE_DYNAMIC(CDesktopOverlayHostWnd)

public:
	CDesktopOverlayHostWnd();
	virtual ~CDesktopOverlayHostWnd();

	BOOL	InitRenderer();	
	void	ResetRenderer(int nWidth, int nHeight);
	void	UninitRenderer();
	void	Render();
	void	InitTimer();

	void	UpdateAttributes();

	CString GetCfgPath();

	CString GetConfigStr(LPCSTR lpSection, LPCSTR lpName, LPCTSTR lpDefault);
	void	SetConfigStr(LPCSTR lpSection, LPCSTR lpName, LPCSTR lpValue);
	int		GetConfigInt(LPCSTR lpSection, LPCSTR lpName, int nDefault);
	void	SetConfigInt(LPCSTR lpSection, LPCSTR lpName, int nValue);
	DWORD	GetConfigHex(LPCSTR lpSection, LPCSTR lpName, DWORD dwDefault);
	void	SetConfigHex(LPCSTR lpSection, LPCSTR lpName, DWORD dwValue);

	CRect	LoadWndPos();
	void	SaveWndPos();

	void	LoadSettings();
	void	SaveSettings();

	void	AddTrayIcon();
	void	DeleteTrayIcon();
	void	RestoreTrayIcon();

	BOOL	MyShell_NotifyIcon(DWORD dwMessage, PNOTIFYICONDATA lpdata);

	void	OnSetup();
	void	OnStartWithWindows();

	DWORD	SwapRGB(DWORD dwColor);

protected:
	DECLARE_MESSAGE_MAP()

	LPDIRECT3D9				m_lpd3d; 
	LPDIRECT3DDEVICE9		m_lpd3dDevice; 
	D3DPRESENT_PARAMETERS	m_d3dpp;

	DWORD					m_dwTimerPeriod;

	BOOL					m_bTransparent;
	BOOL					m_bColorKey;
	DWORD					m_dwBgndColor;
	DWORD					m_dwAlpha;

	BOOL					m_bCaptured;
	int						m_nCapturedX;
	int						m_nCapturedY;

	CMMTimer				m_mmTimer;

	HICON					m_hIcon;
	HICON					m_hIconTray;
	NID_DESC				m_nidDesc;

	ÑDpiAwareHelper			m_dpi;
	CTaskSchedulerInterface	m_tsi;

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnExitSizeMove();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
/////////////////////////////////////////////////////////////////////////////
