// SaveMadiaInterface.cpp: implementation of the CSaveMediaInterface class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "SaveMediaInterface.h"
//////////////////////////////////////////////////////////////////////
CSaveMediaInterface::CSaveMediaInterface()
{
}
//////////////////////////////////////////////////////////////////////
CSaveMediaInterface::~CSaveMediaInterface()
{
}
//////////////////////////////////////////////////////////////////////
DWORD CSaveMediaInterface::Configure(DWORD dwFlags, DWORD dwSize, LPVOID lpData, HWND hWnd)
{
	DWORD dwResult = 0;

	HMODULE hSaveMedia	= LoadLibrary("SaveMedia.dll");

	if (hSaveMedia)
	{
		CONFIGURE pConfigure = (CONFIGURE)GetProcAddress(hSaveMedia, "Configure");

		if (pConfigure)
		{
			dwResult = pConfigure(dwFlags, dwSize, lpData, hWnd);
		}

		FreeLibrary(hSaveMedia);
	}

	return dwResult;
}
//////////////////////////////////////////////////////////////////////
DWORD CSaveMediaInterface::EnumAudioSources(DWORD dwType, DWORD dwSize, LPVOID lpData)
{
	DWORD dwResult = 0;

	HMODULE hSaveMedia	= LoadLibrary("SaveMedia.dll");

	if (hSaveMedia)
	{
		ENUMAUDIOSOURCES pEnumAudioSources = (ENUMAUDIOSOURCES)GetProcAddress(hSaveMedia, "EnumAudioSources");

		if (pEnumAudioSources)
			dwResult = pEnumAudioSources(dwType, dwSize, lpData);

		FreeLibrary(hSaveMedia);
	}

	return dwResult;
}
//////////////////////////////////////////////////////////////////////
