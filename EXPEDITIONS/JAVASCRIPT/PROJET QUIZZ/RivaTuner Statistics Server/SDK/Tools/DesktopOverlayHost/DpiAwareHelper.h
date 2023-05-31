// DpiAwareHelper.h: interface for the �DpiAwareHelper class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
#define DPI_AWARE_PER_MONITOR		1
#define DPI_AWARE_PER_MONITOR_V2	2
/////////////////////////////////////////////////////////////////////////////
class �DpiAwareHelper
{
public:
	�DpiAwareHelper();
	virtual ~�DpiAwareHelper();

	DWORD	SetDpiAware(DWORD dwDpiAware);
	DWORD	GetDpiAware();
	UINT	GetDpi(HWND hWnd);

	UINT	GetDpiForMonitor(HMONITOR hMonitor);
	UINT	GetDpiForWindow(HWND hWnd);
	int		GetSystemMetricsForDpi(int nIndex, UINT dpi);

protected:
	DWORD m_dwDpiAware;
};
//////////////////////////////////////////////////////////////////////
