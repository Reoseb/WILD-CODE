// NVENC.cpp : Defines the initialization routines for the DLL.
//
// created by Unwinder
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <float.h>
#include <shlwapi.h>
#include <afxdllx.h>
#include <winbase.h>
#include <io.h>
 
#include "NVENC.h"
#include "NVENCEncoder.h"
#include "NVENCConfigurationDlg.h"
#include "EncoderPluginTypes.h"
/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
static AFX_EXTENSION_MODULE NVENCDLL = { NULL, NULL };
/////////////////////////////////////////////////////////////////////////////
HINSTANCE					g_hModule				= 0;
CNVENCEncoder*				g_lpEncoder				= NULL;
char						g_szCfgPath[MAX_PATH]	= { 0 };
char						g_szDllPath[MAX_PATH]	= { 0 };
int							g_lastError				= 0;

LOCALIZEWND_PROC			g_pLocalizeWnd			= NULL;
LOCALIZESTR_PROC			g_pLocalizeStr			= NULL;
/////////////////////////////////////////////////////////////////////////////
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(NVENCDLL, hInstance))
			return 0;

		new CDynLinkLibrary(NVENCDLL);

		g_hModule = hInstance;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(NVENCDLL);
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////
NVENC_API HRESULT Init(LPCSTR lpCfgPath)
{
	HMODULE hHost	= GetModuleHandle(NULL);

	g_pLocalizeWnd	= (LOCALIZEWND_PROC)GetProcAddress(hHost, "LocalizeWnd");
	g_pLocalizeStr	= (LOCALIZESTR_PROC)GetProcAddress(hHost, "LocalizeStr");

	if (lpCfgPath)
		strcpy_s(g_szCfgPath, sizeof(g_szCfgPath), lpCfgPath);
	else
	{
		GetModuleFileName(g_hModule, g_szCfgPath, sizeof(g_szCfgPath));

		PathRenameExtension(g_szCfgPath, ".cfg");
	}

	GetModuleFileName(g_hModule, g_szDllPath, sizeof(g_szDllPath));

	PathRemoveFileSpec(g_szDllPath);

	if (g_lpEncoder)
		return S_FALSE;

	g_lpEncoder = new CNVENCEncoder;

	return S_OK;
}
//////////////////////////////////////////////////////////////////////
NVENC_API HRESULT Uninit()
{
	if (g_lpEncoder)
	{
		delete g_lpEncoder;

		g_lpEncoder = NULL;

		return S_OK;
	}

	return S_FALSE;
}
//////////////////////////////////////////////////////////////////////
NVENC_API DWORD GetEncodersNum()
{
	return 1;
}
//////////////////////////////////////////////////////////////////////
NVENC_API HRESULT GetEncoderCaps(DWORD dwEncoder, LPENCODER_CAPS lpCaps)
{
	if (!lpCaps)
		return E_POINTER;

	switch (dwEncoder)
	{
	case 0:
		lpCaps->dwFlags				= ENCODER_CAPS_FLAGS_CONFIGURE_SUPPORTED | ENCODER_CAPS_FLAGS_MULTITHREADING_SAFE;
		lpCaps->dwInputFormatsNum	= 1;
		lpCaps->dwInputFormats[0]	= ENCODER_INPUT_FORMAT_NV12;
		lpCaps->dwOutputFormat		= ' CVA';
		strcpy_s(lpCaps->szDesc, sizeof(lpCaps->szDesc), "NVIDIA NVENC H.264");
		return S_OK;
	}

	return E_INVALIDARG;
}
//////////////////////////////////////////////////////////////////////
NVENC_API HRESULT GetEncoderStat(DWORD dwEncoder, LPENCODER_STAT lpStat)
{
	if (!g_lpEncoder)
		return E_UNEXPECTED;
		
	if (!lpStat || !lpStat->lpData)
		return E_POINTER;

	switch (dwEncoder)
	{
	case 0:
		switch (lpStat->dwType)
		{
		case ENCODER_STAT_TYPE_DESC:
			strcpy_s((LPSTR)lpStat->lpData, lpStat->dwSize, g_lpEncoder->GetDesc());
			return S_OK;
		case ENCODER_STAT_TYPE_LAST_ERROR:
			strcpy_s((LPSTR)lpStat->lpData, lpStat->dwSize, g_lpEncoder->GetStatusStr(g_lastError));
			return S_OK;
		}
	}

	return E_INVALIDARG;
}
//////////////////////////////////////////////////////////////////////
NVENC_API HRESULT Configure(DWORD dwEncoder, HWND hParent)
{
	switch (dwEncoder)
	{
	case 0:
		HMODULE hHost	= GetModuleHandle(NULL);

		g_pLocalizeWnd	= (LOCALIZEWND_PROC)GetProcAddress(hHost, "LocalizeWnd");
		g_pLocalizeStr	= (LOCALIZESTR_PROC)GetProcAddress(hHost, "LocalizeStr");

		CNVENCConfigurationDlg dlg;
		dlg.DoModal();

		return S_OK;
	}

	return E_INVALIDARG;
}
//////////////////////////////////////////////////////////////////////
int GetConfigInt(LPCSTR lpSection, LPCSTR lpName, int nDefault)
{
	return GetPrivateProfileInt(lpSection, lpName, nDefault, g_szCfgPath);
}
//////////////////////////////////////////////////////////////////////
void SetConfigInt(LPCSTR lpSection, LPCSTR lpName, int nValue)
{
	char szValue[MAX_PATH];
	sprintf_s(szValue, sizeof(szValue), "%d", nValue);

	WritePrivateProfileString(lpSection, lpName, szValue, g_szCfgPath);
}
//////////////////////////////////////////////////////////////////////
NVENC_API HRESULT StartEncoding(DWORD dwEncoder, LPENCODER_INPUT lpInput)
{
	if (!g_lpEncoder)
		return E_UNEXPECTED;
		
	if (!lpInput)
		return E_POINTER;

	switch (dwEncoder)
	{
	case 0:

		//get encoder config
		int device			= GetConfigInt("AVC", "Device"			, 0									);
		int preset			= GetConfigInt("AVC", "Preset"			, 1									);
		int targetBitrate	= GetConfigInt("AVC", "TargetBitrate"	, 5000								);
		int profile			= GetConfigInt("AVC", "Profile"			, 0									);
		int asyncMode		= GetConfigInt("AVC", "AsyncMode"		, 1									);
		int lowLatency		= GetConfigInt("AVC", "LowLatency"		, 1									);

		g_lastError			= g_lpEncoder->StartEncoding(lpInput->dwInputWidth, lpInput->dwInputHeight, lpInput->dwFramerate, targetBitrate, device, preset, profile, asyncMode, lowLatency);

		if (g_lastError != NVENC_ERR_NONE)
			return E_FAIL;

		return S_OK;
	}

	return E_INVALIDARG;
}
//////////////////////////////////////////////////////////////////////
NVENC_API HRESULT Encode(DWORD dwEncoder, LPENCODER_INPUT lpInput)
{
	if (!g_lpEncoder)
		return E_UNEXPECTED;
		
	if (!lpInput)
		return E_POINTER;

	switch (dwEncoder)
	{
	case 0:
		if (lpInput->dwInputFormat != ENCODER_INPUT_FORMAT_NV12)
			return E_INVALIDARG;

		g_lastError = g_lpEncoder->Encode(lpInput->dwInputWidth, lpInput->dwInputHeight, lpInput->lpInputData, lpInput->qwTimestamp);

		if (g_lastError != NVENC_ERR_NONE)
			return E_FAIL;

		return S_OK;
	}

	return E_INVALIDARG;
}
//////////////////////////////////////////////////////////////////////
NVENC_API BOOL StopEncoding(DWORD dwEncoder)
{
	if (!g_lpEncoder)
		return E_UNEXPECTED;
		
	switch (dwEncoder)
	{
	case 0:
		g_lastError = g_lpEncoder->StopEncoding();

		if (g_lastError != NVENC_ERR_NONE)
			return E_FAIL;

		return S_OK;
	}

	return E_INVALIDARG;
}
//////////////////////////////////////////////////////////////////////
NVENC_API LPBYTE GetHeader(DWORD dwEncoder)
{
	if (!g_lpEncoder)
		return NULL;
		
	switch (dwEncoder)
	{
	case 0:
		return g_lpEncoder->GetHeader();
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////
NVENC_API DWORD GetHeaderSize(DWORD dwEncoder)
{
	if (!g_lpEncoder)
		return 0;

	switch (dwEncoder)
	{
	case 0:
		return g_lpEncoder->GetHeaderSize();
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////
NVENC_API HRESULT GetFrame(DWORD dwEncoder, LPENCODER_OUTPUT lpOutput)
{
	if (!g_lpEncoder)
		return E_UNEXPECTED;

	if (!lpOutput)
		return E_POINTER;

	switch (dwEncoder)
	{
	case 0:
		LPNVENC_ENCODED_FRAME lpFrame = g_lpEncoder->GetFrame();

		if (lpFrame)
		{
			lpOutput->dwFlags		= 0;

			if (lpFrame->dwFrameType == NV_ENC_PIC_TYPE_IDR)
				lpOutput->dwFlags |= ENCODER_OUTPUT_FLAG_KEYFRAME;

			lpOutput->qwTimestamp	= lpFrame->qwTimestamp;
			lpOutput->lpOutputData	= lpFrame->lpData;
			lpOutput->dwOutputSize	= lpFrame->dwSize;
			lpOutput->lpContext		= lpFrame;

			return S_OK;
		}

		lpOutput->dwFlags		= 0;
		lpOutput->qwTimestamp	= 0;
		lpOutput->lpOutputData	= NULL;
		lpOutput->dwOutputSize	= 0;
		lpOutput->lpContext		= NULL;

		return S_FALSE;
	}

	return E_INVALIDARG;
}
//////////////////////////////////////////////////////////////////////
NVENC_API HRESULT ReleaseFrame(DWORD dwEncoder, LPENCODER_OUTPUT lpOutput)
{
	if (!g_lpEncoder)
		return E_UNEXPECTED;

	if (!lpOutput || !lpOutput->lpContext)
		return E_POINTER;

	switch (dwEncoder)
	{
	case 0:
		g_lpEncoder->DestroyFrame((LPNVENC_ENCODED_FRAME)lpOutput->lpContext);

		lpOutput->lpOutputData	= NULL;
		lpOutput->dwOutputSize	= 0;
		lpOutput->lpContext		= NULL;

		return S_OK;
	}

	return E_INVALIDARG;
}
//////////////////////////////////////////////////////////////////////
LPCSTR LocalizeStr(LPCSTR lpStr)
{
	if (g_pLocalizeStr)
		return g_pLocalizeStr(lpStr);

	return lpStr;
}
/////////////////////////////////////////////////////////////////////////////
void LocalizeWnd(HWND hWnd)
{
	if (g_pLocalizeWnd)
		g_pLocalizeWnd(hWnd);
}
/////////////////////////////////////////////////////////////////////////////


