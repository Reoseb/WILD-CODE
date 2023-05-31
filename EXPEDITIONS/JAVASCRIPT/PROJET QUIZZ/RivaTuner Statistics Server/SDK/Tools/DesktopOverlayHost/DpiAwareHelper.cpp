// DpiAwareHelper.cpp: implementation of the ÑDpiAwareHelper class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DpiAwareHelper.h"
//////////////////////////////////////////////////////////////////////
ÑDpiAwareHelper::ÑDpiAwareHelper()
{
	m_dwDpiAware = 0;
}
//////////////////////////////////////////////////////////////////////
ÑDpiAwareHelper::~ÑDpiAwareHelper()
{
}
//////////////////////////////////////////////////////////////////////
#ifndef DPI_ENUMS_DECLARED
/////////////////////////////////////////////////////////////////////////////
typedef enum PROCESS_DPI_AWARENESS
{
    PROCESS_DPI_UNAWARE				= 0,
    PROCESS_SYSTEM_DPI_AWARE		= 1,
    PROCESS_PER_MONITOR_DPI_AWARE	= 2
} PROCESS_DPI_AWARENESS;
/////////////////////////////////////////////////////////////////////////////
DECLARE_HANDLE(DPI_AWARENESS_CONTEXT);
/////////////////////////////////////////////////////////////////////////////
#define DPI_AWARENESS_CONTEXT_UNAWARE              ((DPI_AWARENESS_CONTEXT)-1)
#define DPI_AWARENESS_CONTEXT_SYSTEM_AWARE         ((DPI_AWARENESS_CONTEXT)-2)
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE    ((DPI_AWARENESS_CONTEXT)-3)
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
#define DPI_AWARENESS_CONTEXT_UNAWARE_GDISCALED    ((DPI_AWARENESS_CONTEXT)-5)
/////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////
typedef BOOL	(WINAPI * SETPROCESSDPIAWARE	)();
typedef HRESULT (WINAPI * SETPROCESSDPIAWARENESS)(PROCESS_DPI_AWARENESS);
typedef BOOL	(WINAPI * SETPROCESSDPIAWARENESSCONTEXT)(DPI_AWARENESS_CONTEXT);
/////////////////////////////////////////////////////////////////////////////
DWORD ÑDpiAwareHelper::SetDpiAware(DWORD dwDpiAware) 
{
    HMODULE hShcoreDLL = LoadLibraryA("Shcore.dll");

    SETPROCESSDPIAWARENESS pSetProcessDpiAwareness = NULL;
    if (hShcoreDLL)
        pSetProcessDpiAwareness = (SETPROCESSDPIAWARENESS)GetProcAddress(hShcoreDLL, "SetProcessDpiAwareness");

    HMODULE hUser32DLL = LoadLibraryA("User32.dll");

    SETPROCESSDPIAWARE pSetProcessDPIAware = NULL;
    if (hUser32DLL)
		pSetProcessDPIAware = (SETPROCESSDPIAWARE)GetProcAddress(hUser32DLL, "SetProcessDPIAware");

	SETPROCESSDPIAWARENESSCONTEXT pSetProcessDpiAwarenessContext = NULL;
	if (hUser32DLL)
		pSetProcessDpiAwarenessContext = (SETPROCESSDPIAWARENESSCONTEXT)GetProcAddress(hUser32DLL, "SetProcessDpiAwarenessContext");

	if (pSetProcessDpiAwarenessContext)
	{	
		switch (dwDpiAware)
		{
		case DPI_AWARE_PER_MONITOR_V2:
			if (pSetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2))
				m_dwDpiAware = DPI_AWARE_PER_MONITOR_V2;
			break;
		default:
			if (pSetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE))
				m_dwDpiAware = DPI_AWARE_PER_MONITOR;
			break;
		}
	}
	else
	if (pSetProcessDpiAwareness) 
	{
		if (pSetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE) != E_INVALIDARG)
			m_dwDpiAware = DPI_AWARE_PER_MONITOR;
	}
	else 
	if (pSetProcessDPIAware)
	{
		if (pSetProcessDPIAware())
			m_dwDpiAware = DPI_AWARE_PER_MONITOR;
	}

    if (hUser32DLL)
        FreeLibrary(hUser32DLL);

	if (hShcoreDLL)
		FreeLibrary(hShcoreDLL);

	return m_dwDpiAware;
}
/////////////////////////////////////////////////////////////////////////////
DWORD ÑDpiAwareHelper::GetDpiAware()
{
	return m_dwDpiAware;
}
/////////////////////////////////////////////////////////////////////////////
UINT ÑDpiAwareHelper::GetDpi(HWND hWnd)
{
	switch (m_dwDpiAware)
	{
	case DPI_AWARE_PER_MONITOR:
		return GetDpiForMonitor(NULL);
	case DPI_AWARE_PER_MONITOR_V2:
		return GetDpiForWindow(hWnd);
	}

	return 96;
}
/////////////////////////////////////////////////////////////////////////////
typedef enum MONITOR_DPI_TYPE {
  MDT_EFFECTIVE_DPI,
  MDT_ANGULAR_DPI,
  MDT_RAW_DPI,
  MDT_DEFAULT
} ;
/////////////////////////////////////////////////////////////////////////////
typedef HRESULT (WINAPI * GETDPIFORMONITOR)(HMONITOR, MONITOR_DPI_TYPE, UINT*, UINT*);
/////////////////////////////////////////////////////////////////////////////
UINT ÑDpiAwareHelper::GetDpiForMonitor(HMONITOR hMonitor)
{
	UINT result = 96;

    HMODULE hShcoreDLL = LoadLibraryA("Shcore.dll");

    GETDPIFORMONITOR pGetDpiForMonitor = NULL;

    if (hShcoreDLL)
		pGetDpiForMonitor = (GETDPIFORMONITOR)GetProcAddress(hShcoreDLL, "GetDpiForMonitor");

    if (pGetDpiForMonitor) 
	{	
		UINT dpiX, dpiY;
			
		if (!hMonitor)
		{
			POINT pt;

			pt.x = 0;
			pt.y = 0;

			hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
		}

        if (SUCCEEDED(pGetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY)))
			result = dpiX;
	}
	else
	{
		HDC hDC	= GetDC(NULL);
	
		result = GetDeviceCaps(hDC, LOGPIXELSX);

		ReleaseDC(NULL, hDC);
	}

    if (hShcoreDLL)
        FreeLibrary(hShcoreDLL);

	return result;
}
/////////////////////////////////////////////////////////////////////////////
typedef UINT (WINAPI * GETDPIFORWINDOW)(HWND);
/////////////////////////////////////////////////////////////////////////////
UINT ÑDpiAwareHelper::GetDpiForWindow(HWND hWnd)
{
	UINT result = 96;

    HMODULE hUser32DLL = LoadLibraryA("User32.dll");

    GETDPIFORWINDOW pGetDpiForWindow = NULL;

    if (hUser32DLL)
		pGetDpiForWindow = (GETDPIFORWINDOW)GetProcAddress(hUser32DLL, "GetDpiForWindow");

    if (pGetDpiForWindow) 
        result = pGetDpiForWindow(hWnd);
	else
	{
		HDC hDC	= GetDC(hWnd);
	
		result = GetDeviceCaps(hDC, LOGPIXELSX);

		ReleaseDC(hWnd, hDC);
	}

    if (hUser32DLL)
        FreeLibrary(hUser32DLL);

	return result;
}
/////////////////////////////////////////////////////////////////////////////
typedef int (WINAPI * GETSYSTEMMETRICSFORDPI)(int, UINT);
/////////////////////////////////////////////////////////////////////////////
int ÑDpiAwareHelper::GetSystemMetricsForDpi(int nIndex, UINT dpi)
{
	int result = GetSystemMetrics(nIndex);

    HMODULE hUser32DLL = LoadLibraryA("User32.dll");

    GETSYSTEMMETRICSFORDPI pGetSystemMetricsForDpi = NULL;

    if (hUser32DLL)
		pGetSystemMetricsForDpi = (GETSYSTEMMETRICSFORDPI)GetProcAddress(hUser32DLL, "GetSystemMetricsForDpi");

    if (pGetSystemMetricsForDpi) 
        result = pGetSystemMetricsForDpi(nIndex, dpi);

    if (hUser32DLL)
        FreeLibrary(hUser32DLL);

	return result;
}
/////////////////////////////////////////////////////////////////////////////
