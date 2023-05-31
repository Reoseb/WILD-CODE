// DesktopOverlayHostWnd.cpp : implementation file
//
// created by Unwinder
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DesktopOverlayHostWnd.h"
#include "DesktopOverlayHostDlg.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDesktopOverlayHostWnd
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CDesktopOverlayHostWnd, CWnd)
/////////////////////////////////////////////////////////////////////////////
#define SAFE_RELEASE(ptr) if (ptr) { ptr->Release(); ptr = NULL; } 
#define SIZING_BORDER_SIZE			8
#define HOST_TIMER_ID				'HTID'
/////////////////////////////////////////////////////////////////////////////
struct SOLIDVERTEX
{
    float			x, y, z, h;
    D3DCOLOR		color;
};
/////////////////////////////////////////////////////////////////////////////
#define D3DFVF_SOLIDVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
/////////////////////////////////////////////////////////////////////////////
CDesktopOverlayHostWnd::CDesktopOverlayHostWnd()
{
	m_lpd3d				= NULL; 
	m_lpd3dDevice		= NULL; 

	m_dwTimerPeriod		= 0;

	m_bTransparent		= FALSE;
	m_bColorKey			= FALSE;
	m_dwBgndColor		= 0;
	m_dwAlpha			= 0;

	m_bCaptured			= FALSE;
	m_nCapturedX		= 0;
	m_nCapturedY		= 0;
}
/////////////////////////////////////////////////////////////////////////////
CDesktopOverlayHostWnd::~CDesktopOverlayHostWnd()
{
}
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDesktopOverlayHostWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
	ON_WM_EXITSIZEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CDesktopOverlayHostWnd message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL CDesktopOverlayHostWnd::InitRenderer()
{
	UninitRenderer();

	m_lpd3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_lpd3d)
		return FALSE;

	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.Windowed				= TRUE;
	m_d3dpp.SwapEffect				= D3DSWAPEFFECT_COPY;
	m_d3dpp.BackBufferFormat		= D3DFMT_UNKNOWN;
	m_d3dpp.EnableAutoDepthStencil	= TRUE;
	m_d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;

	if (FAILED(m_lpd3d->CreateDevice(0, D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3dpp, &m_lpd3dDevice)))
	{
		SAFE_RELEASE(m_lpd3d);

		return FALSE;
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::Render()
{
	HRESULT err;

	if (m_lpd3dDevice)
	{
		m_lpd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, SwapRGB(m_dwBgndColor), 1.0f, 0);

		if (SUCCEEDED(m_lpd3dDevice->BeginScene()))
		{
			m_lpd3dDevice->SetFVF(D3DFVF_SOLIDVERTEX);

			SOLIDVERTEX vertex;
			vertex.x		= 0;
			vertex.y		= 0;
			vertex.z		= 0;
			vertex.h		= 1;
			vertex.color	= SwapRGB(m_dwBgndColor);

			m_lpd3dDevice->DrawPrimitiveUP(D3DPT_POINTLIST, 1, &vertex, sizeof(SOLIDVERTEX));
				//note: 3D application must render at least 1 primitive to get OSD displayed, so we render a point

			m_lpd3dDevice->EndScene();
		}

		err = m_lpd3dDevice->Present(NULL, NULL, NULL, NULL);

		if ((err == D3DERR_DEVICELOST) || 
			(err = D3DERR_INVALIDCALL))
		{
			if (m_lpd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			{
				CRect cr;
				GetClientRect(&cr);

				ResetRenderer(cr.Width(), cr.Height());
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::UninitRenderer()
{
	SAFE_RELEASE(m_lpd3dDevice);
	SAFE_RELEASE(m_lpd3d);
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::OnPaint()
{
	CPaintDC dc(this); 

	Render();
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::ResetRenderer(int nWidth, int nHeight)
{
	if (m_lpd3dDevice)
	{
		m_d3dpp.BackBufferWidth		= nWidth;
		m_d3dpp.BackBufferHeight	= nHeight;

		m_lpd3dDevice->Reset(&m_d3dpp);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	ResetRenderer(cx, cy);
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bCaptured)
	{	
		SetCapture();

		m_bCaptured			= TRUE;
		m_nCapturedX		= point.x;
		m_nCapturedY		= point.y;

		Render();
	}

	CWnd::OnLButtonDown(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bCaptured)
	{
		ReleaseCapture();

		m_bCaptured = FALSE;

		Render();
	}

	CWnd::OnLButtonUp(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bCaptured)
	{
		ClientToScreen(&point);

		SetWindowPos(NULL, point.x - m_nCapturedX, point.y - m_nCapturedY, 0, 0, SWP_NOSIZE);

		SaveWndPos();
	}

	CWnd::OnMouseMove(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::InitTimer()
{
	if (m_dwTimerPeriod)
		m_mmTimer.Create(m_dwTimerPeriod, 15, 0, m_hWnd, WM_TIMER, HOST_TIMER_ID, 0);
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::OnDestroy()
{
	m_mmTimer.Kill();
	UninitRenderer();
	DeleteTrayIcon();

	CWnd::OnDestroy();
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == HOST_TIMER_ID)
	{
		Render();

		m_mmTimer.EatMessages();
	}

	CWnd::OnTimer(nIDEvent);
}
/////////////////////////////////////////////////////////////////////////////
LRESULT CDesktopOverlayHostWnd::OnNcHitTest(CPoint point)
{
	LRESULT result = CWnd::OnNcHitTest(point);

	if (result == HTCLIENT)
	{
		CRect wr;
		GetWindowRect(wr);

		if (point.y - wr.top < SIZING_BORDER_SIZE)
		{
			if (point.x - wr.left < SIZING_BORDER_SIZE)
				return HTTOPLEFT;
			else
			if (wr.right - point.x < SIZING_BORDER_SIZE)
				return HTTOPRIGHT;

			return HTTOP;
		}
		else
		if (wr.bottom - point.y < SIZING_BORDER_SIZE)
		{
			if (point.x - wr.left < SIZING_BORDER_SIZE)
				return HTBOTTOMLEFT;
			else
			if (wr.right - point.x < SIZING_BORDER_SIZE)
				return HTBOTTOMRIGHT;

			return HTBOTTOM;
		}
		else
		if  (point.x - wr.left < SIZING_BORDER_SIZE)
			return HTLEFT;
		else
		if (wr.right - point.x < SIZING_BORDER_SIZE)
			return HTRIGHT;

		return HTCLIENT;
	}

	return result;
}
/////////////////////////////////////////////////////////////////////////////
CString CDesktopOverlayHostWnd::GetCfgPath()
{
	char szCfgPath[MAX_PATH];
	GetModuleFileName(NULL, szCfgPath, MAX_PATH);
	PathRenameExtension(szCfgPath, ".cfg");

	return szCfgPath;
}
/////////////////////////////////////////////////////////////////////////////
CString CDesktopOverlayHostWnd::GetConfigStr(LPCSTR lpSection, LPCSTR lpName, LPCTSTR lpDefault)
{
	char szBuf[CHAR_BUF_SIZE];
	GetPrivateProfileString(lpSection, lpName, lpDefault, szBuf, CHAR_BUF_SIZE, GetCfgPath());

	return szBuf;
}
//////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::SetConfigStr(LPCSTR lpSection, LPCSTR lpName, LPCSTR lpValue)
{
	WritePrivateProfileString(lpSection, lpName, lpValue, GetCfgPath());
}
//////////////////////////////////////////////////////////////////////
int	CDesktopOverlayHostWnd::GetConfigInt(LPCSTR lpSection, LPCSTR lpName, int nDefault)
{
	return GetPrivateProfileInt(lpSection, lpName, nDefault, GetCfgPath());
}
//////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::SetConfigInt(LPCSTR lpSection, LPCSTR lpName, int nValue)
{
	char szValue[MAX_PATH];
	sprintf_s(szValue, sizeof(szValue), "%d", nValue);

	WritePrivateProfileString(lpSection, lpName, szValue, GetCfgPath());
}
//////////////////////////////////////////////////////////////////////
DWORD CDesktopOverlayHostWnd::GetConfigHex(LPCSTR lpSection, LPCSTR lpName, DWORD dwDefault)
{
	char szValue[MAX_PATH];
	GetPrivateProfileString(lpSection, lpName, "", szValue, MAX_PATH, GetCfgPath());

	DWORD dwResult = dwDefault; 
	sscanf_s(szValue, "%08X", &dwResult);

	return dwResult;
}
//////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::SetConfigHex(LPCSTR lpSection, LPCSTR lpName, DWORD dwValue)
{
	char szValue[MAX_PATH];
	sprintf_s(szValue, sizeof(szValue), "%08X", dwValue);

	WritePrivateProfileString(lpSection, lpName, szValue, GetCfgPath());
}
//////////////////////////////////////////////////////////////////////
CRect CDesktopOverlayHostWnd::LoadWndPos()
{
	int x = GetConfigInt("Settings", "WindowX", 0);
	int y = GetConfigInt("Settings", "WindowY", 0);
	int w = GetConfigInt("Settings", "WindowW", 0);
	int h = GetConfigInt("Settings", "WindowH", 0);

	if (!w || !h)
	{
		w = 1024;
		h = 768;
	}

	return CRect(x, y, x + w, y + h);
}
//////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::SaveWndPos()
{
	CRect wr;

	GetWindowRect(&wr);
	
	SetConfigInt("Settings", "WindowX", wr.left);
	SetConfigInt("Settings", "WindowY", wr.top);
	SetConfigInt("Settings", "WindowW", wr.Width());
	SetConfigInt("Settings", "WindowH", wr.Height());
}
//////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::OnExitSizeMove()
{
	SaveWndPos();

	CWnd::OnExitSizeMove();
}
//////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint cp;
	GetCursorPos(&cp);

	CMenu menu;
	menu.LoadMenu(IDR_CONTEXT_MENU);
	menu.GetSubMenu(0)->CheckMenuItem(ID_START_WITH_WINDOWS, MF_BYCOMMAND | ((m_tsi.IsTaskExist("DesktopOverlayHost") == TSI_OK) ? MF_CHECKED : MF_UNCHECKED));
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_NONOTIFY, cp.x, cp.y, this);

	CWnd::OnRButtonDown(nFlags, point);
}
//////////////////////////////////////////////////////////////////////
LRESULT CDesktopOverlayHostWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_TASKBARCREATED)
		RestoreTrayIcon();

	if (message == WM_COMMAND)
	{
		switch (wParam & 0xFFFF)
		{
		case ID_SETUP:
			OnSetup();
			break;
		case ID_START_WITH_WINDOWS:
			OnStartWithWindows();
			break;
		case ID_CLOSE:
			PostMessage(WM_QUIT);
			break;
		}
	}

	if (message == WM_TRAYNOTIFICATION)
	{
		switch (lParam) 
		{
		case WM_LBUTTONUP:
			if (GetAsyncKeyState(VK_CONTROL) < 0)
				SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE);
			else
				OnSetup();
			break;
		case WM_RBUTTONUP:
			CPoint cp;
			GetCursorPos(&cp);

			SetForegroundWindow();

			CMenu menu;
			menu.LoadMenu(IDR_CONTEXT_MENU);
			menu.GetSubMenu(0)->TrackPopupMenu(TPM_NONOTIFY, cp.x, cp.y, this);
			break;
		}
	}



	return CWnd::DefWindowProc(message, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////
int CDesktopOverlayHostWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hIcon					= (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME),IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
	m_hIconTray				= (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);

	SetIcon(m_hIcon		, TRUE);
	SetIcon(m_hIconTray	, FALSE);

	ZeroMemory(&m_nidDesc, sizeof(m_nidDesc));

	m_nidDesc.nid.cbSize			= sizeof(m_nidDesc.nid); 
	m_nidDesc.nid.hWnd				= m_hWnd;
	m_nidDesc.nid.uID				= (UINT)m_hWnd;
	m_nidDesc.nid.uFlags			= NIF_ICON | NIF_TIP | NIF_MESSAGE;
	m_nidDesc.nid.uCallbackMessage	= WM_TRAYNOTIFICATION;
	m_nidDesc.nid.hIcon				= m_hIconTray;

	strcpy_s(m_nidDesc.nid.szTip, sizeof(m_nidDesc.nid.szTip), "Desktop Overlay Host v1.0.1");

	LoadSettings();

	UpdateAttributes();

	AddTrayIcon();

	m_dpi.SetDpiAware(DPI_AWARE_PER_MONITOR_V2);
	m_tsi.Init();

	return 0;
}
//////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::AddTrayIcon()
{
	if (!m_nidDesc.flags)
	{
		MyShell_NotifyIcon(NIM_ADD,(NOTIFYICONDATA*)&m_nidDesc.nid);

		m_nidDesc.flags = 1;
	}
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::DeleteTrayIcon()
{
	if (m_nidDesc.flags)
	{
		Shell_NotifyIcon(NIM_DELETE,(NOTIFYICONDATA*)&m_nidDesc.nid);

		m_nidDesc.flags = 0;
	}
}
/////////////////////////////////////////////////////////////////////////////
BOOL CDesktopOverlayHostWnd::MyShell_NotifyIcon(DWORD dwMessage, PNOTIFYICONDATA lpdata)
{
	for (DWORD dwRetry=0; dwRetry<5; dwRetry++)
	{
		if (Shell_NotifyIcon(dwMessage, lpdata))
			return TRUE;

		Sleep(1000);
	}

	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::RestoreTrayIcon()
{
	if (m_nidDesc.flags)
		MyShell_NotifyIcon(NIM_ADD,(NOTIFYICONDATA*)&m_nidDesc.nid);
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::LoadSettings()
{
	m_dwTimerPeriod	= GetConfigInt("Settings", "TimerPeriod", 33);

	m_bTransparent	= GetConfigInt("Settings", "Transparent", 0);
	m_bColorKey		= GetConfigInt("Settings", "ColorKey"	, 0);
	m_dwBgndColor	= GetConfigHex("Settings", "BgndColor"	, 0x800080);
	m_dwAlpha		= GetConfigHex("Settings", "Alpha"		, 0xFF);
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::SaveSettings()
{
	SetConfigInt("Settings", "Transparent"	, m_bTransparent	? 1 : 0);
	SetConfigInt("Settings", "ColorKey"		, m_bColorKey		? 1 : 0);
	SetConfigHex("Settings", "BgndColor"	, m_dwBgndColor);
	SetConfigHex("Settings", "Alpha"		, m_dwAlpha);
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::OnSetup()
{
	CDesktopOverlayHostDlg dlg;

	dlg.m_bTransparent	= m_bTransparent;
	dlg.m_bColorKey		= m_bColorKey;
	dlg.m_dwBgndColor	= m_dwBgndColor;
	dlg.m_dwAlpha		= m_dwAlpha;

	if (IDOK == dlg.DoModal())
	{
		m_bTransparent		= dlg.m_bTransparent;
		m_bColorKey			= dlg.m_bColorKey;
		m_dwBgndColor		= dlg.m_dwBgndColor;
		m_dwAlpha			= dlg.m_dwAlpha;

		UpdateAttributes();

		SaveSettings();
	}
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::OnStartWithWindows()
{
	if (m_tsi.IsTaskExist("DesktopOverlayHost") == TSI_OK)
	{
		int err = m_tsi.DeleteTask("DesktopOverlayHost");

		if (err)
			MessageBox("Failed to delete startup task!", "DesktopOverlayHost", MB_ICONWARNING|MB_OK);
	}
	else
	{
		char szPath[MAX_PATH];
		GetModuleFileName(NULL, szPath, MAX_PATH);

		int err = m_tsi.AddTask("DesktopOverlayHost", "", szPath, "");

		if (err)
			MessageBox("Failed to add startup task!", "DesktopOverlayHost", MB_ICONWARNING|MB_OK);
	}
}
/////////////////////////////////////////////////////////////////////////////
DWORD CDesktopOverlayHostWnd::SwapRGB(DWORD dwColor)
{
	return (0xFF & (dwColor>>16)) | (dwColor & 0xFF00) | ((dwColor & 0xFF)<<16);
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::UpdateAttributes()
{
	SetLayeredWindowAttributes(m_dwBgndColor, (BYTE)m_dwAlpha, LWA_ALPHA | (m_bColorKey ? LWA_COLORKEY : 0)); 

	if (m_bTransparent) 
		ModifyStyleEx(0, WS_EX_TRANSPARENT);
	else
		ModifyStyleEx(WS_EX_TRANSPARENT, 0);
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnSetup();

	CWnd::OnLButtonDblClk(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
