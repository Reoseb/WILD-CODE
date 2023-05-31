#ifdef HOTKEYHANDLER_EXPORTS
#define HOTKEYHANDLER_API extern "C" __declspec(dllexport)
#else
#define HOTKEYHANDLER_API extern "C" __declspec(dllimport)
#endif
//////////////////////////////////////////////////////////////////////
#include "RTSSExports.h"
#include "HotkeyHandlerWnd.h"
//////////////////////////////////////////////////////////////////////
extern HINSTANCE					g_hModule;

//host API functions

extern GET_HOST_APP_PROPERTY_PROC	g_pGetHostAppProperty;
extern LOCALIZEWND_PROC				g_pLocalizeWnd;
extern LOCALIZESTR_PROC				g_pLocalizeStr;

//host skin color scheme

extern DWORD						g_dwHeaderBgndColor;
extern DWORD						g_dwHeaderTextColor;

//main client window

extern CHotkeyHandlerWnd			g_clientWnd;
//////////////////////////////////////////////////////////////////////
int		CreateDirectory(LPCSTR lpDirectory);
	//helper for screen/video capture folder creation
LPCSTR	LocalizeStr(LPCSTR lpStr);
void	LocalizeWnd(HWND hWnd);
	//wrapppers for host localization API
void	AdjustWindowPos(CWnd* pWnd, CWnd* pParent);
	//helper for child window position adjustment
//////////////////////////////////////////////////////////////////////

