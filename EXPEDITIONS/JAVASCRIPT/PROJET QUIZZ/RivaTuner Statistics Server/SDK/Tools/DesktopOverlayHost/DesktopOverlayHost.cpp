// DesktopOverlayHost.cpp : Defines the class behaviors for the application.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DesktopOverlayHost.h"
#include "DesktopOverlayHostDlg.h"
#include "DesktopOverlayHostWnd.h"
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// CDesktopOverlayHostApp
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDesktopOverlayHostApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////
// CDesktopOverlayHostApp construction
//////////////////////////////////////////////////////////////////////
CDesktopOverlayHostApp::CDesktopOverlayHostApp()
{
}
//////////////////////////////////////////////////////////////////////
// The one and only CDesktopOverlayHostApp object
//////////////////////////////////////////////////////////////////////
CDesktopOverlayHostApp	theApp;
CDesktopOverlayHostWnd	theWnd;
//////////////////////////////////////////////////////////////////////
// CDesktopOverlayHostApp initialization
//////////////////////////////////////////////////////////////////////
BOOL CDesktopOverlayHostApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	CRect rect = theWnd.LoadWndPos(); 

	theWnd.CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED, AfxRegisterWndClass(CS_OWNDC | CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)), "DesktopOverlayHost", WS_POPUP | WS_VISIBLE, rect, NULL, 0);

	if (!theWnd.InitRenderer())
	{
		MessageBox(NULL, "Failed to initialize Direct3D!", "DesktopOverlayHost", MB_ICONWARNING|MB_OK);

		return FALSE;
	}

	theWnd.InitTimer();

	m_pMainWnd = &theWnd;

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
int CDesktopOverlayHostApp::ExitInstance()
{
	theWnd.DestroyWindow();

	return CWinAppEx::ExitInstance();
}
//////////////////////////////////////////////////////////////////////
