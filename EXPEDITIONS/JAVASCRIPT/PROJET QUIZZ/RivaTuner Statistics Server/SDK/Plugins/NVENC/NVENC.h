#ifdef NVENC_EXPORTS
#define NVENC_API extern "C" __declspec(dllexport)
#else
#define NVENC_API extern "C" __declspec(dllimport)
#endif

#include "RTSSExports.h"

extern LOCALIZEWND_PROC	g_pLocalizeWnd;
extern LOCALIZESTR_PROC	g_pLocalizeStr;

int GetConfigInt(LPCSTR lpSection, LPCSTR lpName, int nDefault);
void SetConfigInt(LPCSTR lpSection, LPCSTR lpName, int nValue);

LPCSTR LocalizeStr(LPCSTR lpStr);
void LocalizeWnd(HWND hWnd);
