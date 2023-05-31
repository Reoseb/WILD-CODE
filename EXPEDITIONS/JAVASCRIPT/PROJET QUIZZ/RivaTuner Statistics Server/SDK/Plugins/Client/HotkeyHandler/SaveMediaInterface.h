// SaveMediaInterface.h: interface for the CSaveMediaInterface class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
// VFW configuration descriptor
//////////////////////////////////////////////////////////////////////
typedef struct CONFIGURE_VIDC_DESC
{
	DWORD	dwFourCC;
	DWORD	dwFlags;
	DWORD	dwQuality;
	char	szName[MAX_PATH];
	char	szDesc[MAX_PATH];
	char	szDriver[MAX_PATH];
} CONFIGURE_VIDC_DESC, *LPCONFIGURE_VIDC_DESC;
//////////////////////////////////////////////////////////////////////
// external encoder plugin configuration descriptor
//////////////////////////////////////////////////////////////////////
typedef struct CONFIGURE_EXT_DESC
{
	DWORD	dwFourCC;
	DWORD	dwFlags;
	char	szLibrary[MAX_PATH];
	DWORD	dwEncoder;
	char	szDesc[MAX_PATH];
} CONFIGURE_EXT_DESC, *LPCONFIGURE_EXT_DESC;
//////////////////////////////////////////////////////////////////////
#define CONFIGURE_FLAGS_TYPE_MASK							0x000000FF
#define CONFIGURE_FLAGS_TYPE_VIDC							0
#define CONFIGURE_FLAGS_TYPE_EXT							1
#define CONFIGURE_FLAGS_CONFIGURE							0x00000100
//////////////////////////////////////////////////////////////////////
typedef DWORD (*CONFIGURE)(DWORD, DWORD, LPVOID, HWND);
typedef DWORD (*ENUMAUDIOSOURCES)(DWORD, DWORD, LPVOID);
//////////////////////////////////////////////////////////////////////
class CSaveMediaInterface
{
public:
	DWORD Configure(DWORD dwFlags, DWORD dwSize, LPVOID lpData, HWND hWnd);
		//configure or read configuration of video encoder's VFW or extended encoder plugin 
	DWORD EnumAudioSources(DWORD dwType, DWORD dwSize, LPVOID lpData);
		//enumerate available audio sources and return the result as REG_MULTI_SZ in user specified buffer

	CSaveMediaInterface();
	virtual ~CSaveMediaInterface();
};
//////////////////////////////////////////////////////////////////////
