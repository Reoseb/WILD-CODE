// VideoCaptureDlg.h: interface for the CVideoCaptureDlg class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HotkeyHandler.h"
#include "VideoCaptureDlg.h"
#include "SaveMediaInterface.h"
#include "SaveMediaTypes.h"
#include "TokenString.h"
#include "MultiString.h"

#include <vfw.h>
//////////////////////////////////////////////////////////////////////
// CVideoCaptureDlg dialog
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CVideoCaptureDlg, CDialog)
//////////////////////////////////////////////////////////////////////
#define CAPTURE_MODE_MIXED											0
#define CAPTURE_MODE_DESKTOP										1
#define CAPTURE_MODE_PROCESS										2
//////////////////////////////////////////////////////////////////////
#define AUDIO_SOURCE_TYPE_DISABLED									0
#define AUDIO_SOURCE_TYPE_AUTO										1
#define AUDIO_SOURCE_TYPE_DIRECTSOUND_CAPTURE						2
#define AUDIO_SOURCE_TYPE_WASAPI_RENDER								3
#define AUDIO_SOURCE_TYPE_WASAPI_CAPTURE							4
//////////////////////////////////////////////////////////////////////
CVideoCaptureDlg::CVideoCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoCaptureDlg::IDD, pParent)
{
	m_strVideoCaptureContainer			= "mkv";
	m_strVideoCaptureFormat				= "MJPG";
	m_dwVideoCaptureQuality				= 75;
	m_strVideoCaptureFolder				= "";

	m_strCustomFramesizes				= "";
	m_dwVideoCaptureFramesize			= 0;
	m_dwVideoCaptureFramerate			= 60;

	m_dwAudioCaptureFlags				= 0x00000003;
	m_dwAudioCaptureFlags2				= 0x00000002;
	m_nCaptureMode						= 0;
	m_nAudioSourceType					= 0;
	m_nAudioSource						= 0;
	m_nAudioSourceType2					= 0;
	m_nAudioSource2						= 0;

	m_bAudioCaptureForceStereo			= TRUE;
	m_nForceStereoMode					= 0;
	m_bAudioCaptureMix					= FALSE;

	m_bUseEncoderServer					= FALSE;
	m_bCaptureOSD						= TRUE;

	m_bWASAPIAvailable					= FALSE;

	m_strVFWCodec						= "";
	m_bVFWQualitySupported				= FALSE;

	m_strEXTCodec						= "";
	m_dwEXTCodec						= 0;

	m_hBrush							= NULL;
}
//////////////////////////////////////////////////////////////////////
CVideoCaptureDlg::~CVideoCaptureDlg()
{
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VIDEO_CAPTURE_FOLDER_EDIT, m_strVideoCaptureFolder);
	DDX_Check(pDX, IDC_CAPTURE_OSD_CHECK, m_bCaptureOSD);
	DDX_Control(pDX, IDC_VIDEO_CAPTURE_QUALITY_SLIDER, m_ctrlVideoCaptureQualitySlider);
	DDX_Control(pDX, IDC_VIDEO_FORMAT_COMBO, m_ctrlVideoCaptureFormatCombo);
	DDX_Control(pDX, IDC_VIDEO_CAPTURE_FRAMESIZE_COMBO, m_ctrlVideoCaptureFramesizeCombo);
	DDX_Control(pDX, IDC_VIDEO_CAPTURE_FRAMERATE_SLIDER, m_ctrlVideoCaptureFramerateSlider);
	DDX_CBIndex(pDX, IDC_AUDIO_CAPTURE_SOURCE_TYPE_COMBO, m_nAudioSourceType);
	DDX_CBIndex(pDX, IDC_AUDIO_CAPTURE_SOURCE_TYPE_2_COMBO, m_nAudioSourceType2);
	DDX_CBIndex(pDX, IDC_FORCE_STEREO_MODE_COMBO, m_nForceStereoMode);
	DDX_Check(pDX, IDC_FORCE_STEREO_CHECK, m_bAudioCaptureForceStereo);
	DDX_Check(pDX, IDC_MIX_CHECK, m_bAudioCaptureMix);
	DDX_Control(pDX, IDC_AUDIO_CAPTURE_SOURCE_COMBO, m_ctrlAudioSourceCombo);
	DDX_Control(pDX, IDC_AUDIO_CAPTURE_2_SOURCE_COMBO, m_ctrlAudioSourceCombo2);
	DDX_Control(pDX, IDC_PTT_HOTKEY, m_ctrlPTTHotkey);
	DDX_Control(pDX, IDC_PTT2_HOTKEY, m_ctrlPTT2Hotkey);
	DDX_Check(pDX, IDC_USE_ENCODER_SERVER_CHECK, m_bUseEncoderServer);
	DDX_CBIndex(pDX, IDC_VIDEO_CAPTURE_MODE_COMBO, m_nCaptureMode);
}
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CVideoCaptureDlg, CDialog)
	ON_BN_CLICKED(IDC_BROWSE_BUTTON, &CVideoCaptureDlg::OnBnClickedBrowseButton)
	ON_BN_CLICKED(IDC_VIEW_BUTTON, &CVideoCaptureDlg::OnBnClickedViewButton)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_VIDEO_FORMAT_COMBO, &CVideoCaptureDlg::OnCbnSelchangeVideoFormatCombo)
	ON_BN_CLICKED(IDC_VIDEO_FORMAT_SETTINGS_BUTTON, &CVideoCaptureDlg::OnBnClickedVideoFormatSettingsButton)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_AUDIO_CAPTURE_SOURCE_TYPE_COMBO, &CVideoCaptureDlg::OnCbnSelchangeAudioCaptureSourceTypeCombo)
	ON_CBN_SELCHANGE(IDC_AUDIO_CAPTURE_SOURCE_COMBO, &CVideoCaptureDlg::OnCbnSelchangeAudioCaptureSourceCombo)
	ON_CBN_SELCHANGE(IDC_AUDIO_CAPTURE_SOURCE_TYPE_2_COMBO, &CVideoCaptureDlg::OnCbnSelchangeAudioCaptureSourceType2Combo)
	ON_CBN_SELCHANGE(IDC_AUDIO_CAPTURE_2_SOURCE_COMBO, &CVideoCaptureDlg::OnCbnSelchangeAudioCapture2SourceCombo)
	ON_BN_CLICKED(IDC_FORCE_STEREO_CHECK, &CVideoCaptureDlg::OnBnClickedForceStereoCheck)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////
// CVideoCaptureDlg message handlers
//////////////////////////////////////////////////////////////////////
BOOL CVideoCaptureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LocalizeWnd(m_hWnd);
	AdjustWindowPos(this, GetParent());

	SetHotkeyCtrl(&m_ctrlPTTHotkey					, HOTKEY_VIDEO_CAPTURE_PTT	);
	SetHotkeyCtrl(&m_ctrlPTT2Hotkey					, HOTKEY_VIDEO_CAPTURE_PTT2	);

	m_ctrlVideoCaptureQualitySlider.SetRange(25, 100);
	m_ctrlVideoCaptureQualitySlider.SetPos(m_dwVideoCaptureQuality);

	m_ctrlVideoCaptureFramerateSlider.SetRange(1, 120);
	m_ctrlVideoCaptureFramerateSlider.SetPos(m_dwVideoCaptureFramerate );

	for (int i=0; i<m_ctrlVideoCaptureFramesizeCombo.GetCount(); i++)
		m_ctrlVideoCaptureFramesizeCombo.SetItemData(i, i + 1);

	CTokenString ts;

	LPCSTR lpToken = ts.strtok(m_strCustomFramesizes, ",");

	while (lpToken)
	{
		CString strName = lpToken;

		lpToken = ts.strtok(NULL, ",");
		if (!lpToken)
			break;

		CString strFramesize = lpToken;

		lpToken = ts.strtok(NULL, ",");
		if (!lpToken)
			break;

		DWORD dwFramesize;

		if (sscanf_s(strFramesize, "%x", &dwFramesize) == 1)
			m_ctrlVideoCaptureFramesizeCombo.SetItemData(m_ctrlVideoCaptureFramesizeCombo.AddString(strName), dwFramesize);
	}

	m_ctrlVideoCaptureFramesizeCombo.SetCurSel(0);

	for (int i=0; i<m_ctrlVideoCaptureFramesizeCombo.GetCount(); i++)
	{
		if (m_ctrlVideoCaptureFramesizeCombo.GetItemData(i) == m_dwVideoCaptureFramesize)
		{
			m_ctrlVideoCaptureFramesizeCombo.SetCurSel(i);
			break;
		}
	}

	OSVERSIONINFO osVersionInfo;
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osVersionInfo);

	if	((osVersionInfo.dwPlatformId	== VER_PLATFORM_WIN32_NT) &&
		 (osVersionInfo.dwMajorVersion	>= 6					))
		 m_bWASAPIAvailable = TRUE;
	else
		m_bWASAPIAvailable = FALSE;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_AUDIO_CAPTURE_SOURCE_TYPE_COMBO);

	if (pCombo)
	{
		if (m_bWASAPIAvailable)
		{
			pCombo->AddString(LocalizeStr("WASAPI playback device"));
			pCombo->AddString(LocalizeStr("WASAPI capture device"));
		}
	}

	pCombo = (CComboBox*)GetDlgItem(IDC_AUDIO_CAPTURE_SOURCE_TYPE_2_COMBO);

	if (pCombo)
	{
		if (m_bWASAPIAvailable)
		{
			pCombo->AddString(LocalizeStr("WASAPI playback device"));
			pCombo->AddString(LocalizeStr("WASAPI capture device"));
		}
	}

	ConfigureVFW(CONFIGURE_FLAGS_TYPE_VIDC);
	ConfigureEXT(CONFIGURE_FLAGS_TYPE_EXT);
	
	UpdateVideoCaptureFormatControls(TRUE);
	UpdateVideoCaptureQualityControls();
	UpdateVideoCaptureFramerateControls();

	BOOL bAudioCapture			= (m_dwAudioCaptureFlags  & SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_ENABLED			) != 0;
	BOOL bAudioCapture2			= (m_dwAudioCaptureFlags2 & SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_ENABLED			) != 0;

	m_bAudioCaptureForceStereo	= (m_dwAudioCaptureFlags  & SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_FORCE_STEREO		) != 0;
	m_nForceStereoMode			= ((m_dwAudioCaptureFlags & SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_FORCE_STEREO_MIX	) != 0) ? 1 : 0;
	m_bAudioCaptureMix			= (m_dwAudioCaptureFlags  & SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_MIX				) != 0;

	m_bUseEncoderServer			= (m_dwVideoCaptureFlagsEx & SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_ENCODER_SERVER	) != 0;

	if (m_dwVideoCaptureFlagsEx & SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_FORCE_DESKTOP)
		m_nCaptureMode = CAPTURE_MODE_DESKTOP;
	else
	if (m_dwVideoCaptureFlagsEx & SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_FORCE_PROCESS)
		m_nCaptureMode = CAPTURE_MODE_PROCESS;
	else
		m_nCaptureMode = CAPTURE_MODE_MIXED;

	m_nAudioSourceType			= bAudioCapture  ? ((m_dwAudioCaptureFlags  & SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_TYPE_MASK)>>SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_TYPE_SHIFT) + 1 : 0;
	m_nAudioSourceType2			= bAudioCapture2 ? ((m_dwAudioCaptureFlags2 & SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_TYPE_MASK)>>SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_TYPE_SHIFT) + 1 : 0;
	m_nAudioSource				= (m_dwAudioCaptureFlags  & SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_MASK)>>SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_SHIFT;
	m_nAudioSource2				= (m_dwAudioCaptureFlags2 & SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_MASK)>>SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_SHIFT;

	InitAudioSourcesList();
	InitAudioSourcesList2();
	InitAudioControls();

	UpdateData(FALSE);

	return TRUE; 
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::OnBnClickedBrowseButton()
{
	char szBuf[MAX_PATH];

	CString strTitle = LocalizeStr("Please select target folder for video capture"); 

	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));

	bi.hwndOwner		= m_hWnd;
	bi.pszDisplayName	= szBuf;
	bi.lpszTitle		= strTitle;

	ITEMIDLIST* piid = SHBrowseForFolder(&bi);

	if (piid && SHGetPathFromIDList(piid, szBuf))
	{
		m_strVideoCaptureFolder = szBuf;
		UpdateData(FALSE);
	}
	
	if (piid)
	{
		LPMALLOC lpMalloc;
		VERIFY(::SHGetMalloc(&lpMalloc) == NOERROR);
		lpMalloc->Free(piid);
		lpMalloc->Release();
	}
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::OnBnClickedViewButton()
{
	CreateDirectory(m_strVideoCaptureFolder);

	ShellExecute(GetSafeHwnd(), "open", m_strVideoCaptureFolder, NULL, NULL, SW_SHOWNORMAL );
}
//////////////////////////////////////////////////////////////////////
DWORD CVideoCaptureDlg::StrToFormat(LPCSTR lpFormat)
{
	if (!strcmp("DIB", lpFormat))
		return FORMAT_UNCOMPRESSED;
	if (!strcmp("NV12", lpFormat))
		return FORMAT_NV12;
	if (!strcmp("RTV1", lpFormat))
		return FORMAT_RTV1;
	if (!strcmp("MJPG", lpFormat))
		return FORMAT_MJPG;
	if (!strcmp("VFW", lpFormat))
		return FORMAT_VFW;
	if (!strcmp("EXT", lpFormat))
		return FORMAT_EXT;

	return FORMAT_MJPG;
}
//////////////////////////////////////////////////////////////////////
CString CVideoCaptureDlg::FormatToStr(DWORD dwFormat)
{
	switch (dwFormat)
	{
	case FORMAT_UNCOMPRESSED:
		return "DIB";
	case FORMAT_NV12:
		return "NV12";
	case FORMAT_RTV1:
		return "RTV1";
	case FORMAT_MJPG:
		return "MJPG";
	case FORMAT_VFW:
		return "VFW";
	case FORMAT_EXT:
		return "EXT";
	}

	return "MJPG";
}
//////////////////////////////////////////////////////////////////////
DWORD CVideoCaptureDlg::StrToContainer(LPCSTR lpContainer)
{
	if (!_stricmp("avi", lpContainer))
		return CONTAINER_AVI;
	if (!_stricmp("mkv", lpContainer))
		return CONTAINER_MKV;

	return CONTAINER_AVI;
}
//////////////////////////////////////////////////////////////////////
CString CVideoCaptureDlg::ContainerToStr(DWORD dwContainer)
{
	switch (dwContainer)
	{
	case CONTAINER_AVI:
		return "avi";
	case CONTAINER_MKV:
		return "mkv";
	}

	return "avi";
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_ctrlVideoCaptureQualitySlider)
	{
		m_dwVideoCaptureQuality = m_ctrlVideoCaptureQualitySlider.GetPos();

		UpdateVideoCaptureQualityControls();
	}

	if (pScrollBar == (CScrollBar*)&m_ctrlVideoCaptureFramerateSlider)
	{
		m_dwVideoCaptureFramerate = m_ctrlVideoCaptureFramerateSlider.GetPos();

		UpdateVideoCaptureFramerateControls();
	}


	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::UpdateVideoCaptureQualityControls()
{
	CString strCaption;
	strCaption.Format(LocalizeStr("%d%% quality"), m_dwVideoCaptureQuality);

	GetDlgItem(IDC_VIDEO_QUALITY_CAPTION)->SetWindowText(strCaption);

	UpdateData(TRUE);

	DWORD dwFormat = GetFormat();

	m_ctrlVideoCaptureQualitySlider.EnableWindow((dwFormat == FORMAT_MJPG) || (dwFormat == FORMAT_RTV1) || ((dwFormat == FORMAT_VFW) && m_bVFWQualitySupported));
}
//////////////////////////////////////////////////////////////////////
DWORD CVideoCaptureDlg::GetFormat()
{
	DWORD dwFormat = FORMAT_MJPG;

	int nSel = m_ctrlVideoCaptureFormatCombo.GetCurSel();

	if (nSel != -1)
		dwFormat = m_ctrlVideoCaptureFormatCombo.GetItemData(nSel);

	return dwFormat;
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::SetFormat(DWORD dwFormat)
{
	for (int iItem=0; iItem<m_ctrlVideoCaptureFormatCombo.GetCount(); iItem++)
	{
		if (m_ctrlVideoCaptureFormatCombo.GetItemData(iItem) == dwFormat)
		{
			m_ctrlVideoCaptureFormatCombo.SetCurSel(iItem);
			return;
		}

		if (m_ctrlVideoCaptureFormatCombo.GetItemData(iItem) == FORMAT_MJPG)
			m_ctrlVideoCaptureFormatCombo.SetCurSel(iItem);
	}
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::UpdateVideoCaptureFormatControls(BOOL bReinit)
{
	if (bReinit)
	{
		m_ctrlVideoCaptureFormatCombo.ResetContent();

		m_ctrlVideoCaptureFormatCombo.SetItemData(m_ctrlVideoCaptureFormatCombo.AddString(LocalizeStr("uncompressed"	)), FORMAT_UNCOMPRESSED);
		m_ctrlVideoCaptureFormatCombo.SetItemData(m_ctrlVideoCaptureFormatCombo.AddString(LocalizeStr("NV12 compression")), FORMAT_NV12);
		m_ctrlVideoCaptureFormatCombo.SetItemData(m_ctrlVideoCaptureFormatCombo.AddString(LocalizeStr("RTV1 compression")), FORMAT_RTV1);
		m_ctrlVideoCaptureFormatCombo.SetItemData(m_ctrlVideoCaptureFormatCombo.AddString(LocalizeStr("MJPG compression")), FORMAT_MJPG);

		CString strVFW;
		strVFW.Format(LocalizeStr("VFW compression : %s"), m_strVFWCodec.IsEmpty() ?  LocalizeStr("not configured") : m_strVFWCodec);
		m_ctrlVideoCaptureFormatCombo.SetItemData(m_ctrlVideoCaptureFormatCombo.AddString(strVFW), FORMAT_VFW);

		CString strEXT;
		strEXT.Format(LocalizeStr("external plugin : %s"), m_strEXTCodec.IsEmpty() ?  LocalizeStr("not configured") : m_strEXTCodec);
		m_ctrlVideoCaptureFormatCombo.SetItemData(m_ctrlVideoCaptureFormatCombo.AddString(strEXT), FORMAT_EXT);

		SetFormat(StrToFormat(m_strVideoCaptureFormat));
	}

	DWORD dwFormat = GetFormat();

	GetDlgItem(IDC_VIDEO_FORMAT_SETTINGS_BUTTON)->EnableWindow((dwFormat == FORMAT_VFW) || (dwFormat == FORMAT_EXT));
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::OnCbnSelchangeVideoFormatCombo()
{
	UpdateVideoCaptureQualityControls();
	UpdateVideoCaptureFormatControls();
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::OnBnClickedVideoFormatSettingsButton()
{
	DWORD dwFormat = GetFormat();

	if (dwFormat == FORMAT_VFW)
		ConfigureVFW(CONFIGURE_FLAGS_TYPE_VIDC | CONFIGURE_FLAGS_CONFIGURE);
	else
	if (GetFormat() == FORMAT_EXT)
		ConfigureEXT(CONFIGURE_FLAGS_TYPE_EXT | CONFIGURE_FLAGS_CONFIGURE);

	m_strVideoCaptureFormat = FormatToStr(dwFormat);

	UpdateVideoCaptureFormatControls(TRUE);
	UpdateVideoCaptureQualityControls();
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::ConfigureVFW(DWORD dwFlags)
{
	CONFIGURE_VIDC_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.dwQuality = m_dwVideoCaptureQuality;

	CSaveMediaInterface encoderInterface;
	
	if (encoderInterface.Configure(dwFlags, sizeof(desc), (LPVOID)&desc, m_hWnd) == sizeof(desc))
	{
		m_strVFWCodec			= desc.szDesc;
		m_bVFWQualitySupported	= (desc.dwFlags & VIDCF_QUALITY) ? TRUE : FALSE;

		if (dwFlags & CONFIGURE_FLAGS_CONFIGURE)
		{
			if (m_bVFWQualitySupported)
			{
				m_dwVideoCaptureQuality = desc.dwQuality;

				if (m_dwVideoCaptureQuality < 25)
					m_dwVideoCaptureQuality = 25;

				m_ctrlVideoCaptureQualitySlider.SetPos(m_dwVideoCaptureQuality);
			} 
		}
	}
	else
	{
		m_strVFWCodec			= "";
		m_bVFWQualitySupported	= FALSE;
	}
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::ConfigureEXT(DWORD dwFlags)
{
	CONFIGURE_EXT_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	CSaveMediaInterface encoderInterface;

	if (encoderInterface.Configure(dwFlags, sizeof(desc), (LPVOID)&desc, m_hWnd) == sizeof(desc))
	{
		m_strEXTCodec			= desc.szDesc;
		m_dwEXTCodec			= desc.dwFourCC;
	}
	else
	{
		m_strEXTCodec			= "";
		m_dwEXTCodec			= 0;
	}
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::OnOK()
{
	UpdateData(TRUE);

	m_strVideoCaptureFormat		= FormatToStr(GetFormat());

	m_dwVideoCaptureQuality		= m_ctrlVideoCaptureQualitySlider.GetPos();
	m_dwVideoCaptureFramerate	= m_ctrlVideoCaptureFramerateSlider.GetPos();
	m_dwVideoCaptureFramesize	= m_ctrlVideoCaptureFramesizeCombo.GetItemData(m_ctrlVideoCaptureFramesizeCombo.GetCurSel());

	//audio source #1

	BOOL bAudioCapture = m_nAudioSourceType > AUDIO_SOURCE_TYPE_DISABLED;

	if (bAudioCapture)
		m_dwAudioCaptureFlags |= SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_ENABLED;
	else
		m_dwAudioCaptureFlags &= ~SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_ENABLED;
	
	if (m_bAudioCaptureForceStereo)
		m_dwAudioCaptureFlags |= SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_FORCE_STEREO;
	else
		m_dwAudioCaptureFlags &= ~SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_FORCE_STEREO;

	if (m_nForceStereoMode)
		m_dwAudioCaptureFlags |= SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_FORCE_STEREO_MIX;
	else
		m_dwAudioCaptureFlags &= ~SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_FORCE_STEREO_MIX;

	if (m_bAudioCaptureMix)
		m_dwAudioCaptureFlags |= SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_MIX;
	else
		m_dwAudioCaptureFlags &= ~SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_MIX;

	if (m_hotkeyManager.GetHotkey(HOTKEY_VIDEO_CAPTURE_PTT))
		m_dwAudioCaptureFlags |= SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_PTT;
	else
		m_dwAudioCaptureFlags &= ~SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_PTT;

	m_dwAudioCaptureFlags &= ~(SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_TYPE_MASK | SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_MASK);

	if (bAudioCapture)
	{
		m_dwAudioCaptureFlags |= (m_nAudioSourceType - 1)<<SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_TYPE_SHIFT;
		m_dwAudioCaptureFlags |= m_nAudioSource<<SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_SHIFT;
	}

	//audio source #2

	BOOL bAudioCapture2 = m_nAudioSourceType2 > AUDIO_SOURCE_TYPE_DISABLED;

	if (bAudioCapture2)
		m_dwAudioCaptureFlags2 |= SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_ENABLED;
	else
		m_dwAudioCaptureFlags2 &= ~SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_ENABLED;
	
	if (m_bAudioCaptureForceStereo)
		m_dwAudioCaptureFlags2 |= SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_FORCE_STEREO;
	else
		m_dwAudioCaptureFlags2 &= ~SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_FORCE_STEREO;

	if (m_nForceStereoMode)
		m_dwAudioCaptureFlags2 |= SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_FORCE_STEREO_MIX;
	else
		m_dwAudioCaptureFlags2 &= ~SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_FORCE_STEREO_MIX;

	if (m_bAudioCaptureMix)
		m_dwAudioCaptureFlags2 |= SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_MIX;
	else
		m_dwAudioCaptureFlags2 &= ~SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_MIX;

	if (m_hotkeyManager.GetHotkey(HOTKEY_VIDEO_CAPTURE_PTT2))
		m_dwAudioCaptureFlags2 |= SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_PTT;
	else
		m_dwAudioCaptureFlags2 &= ~SAVEMEDIAPARAM_AUDIO_CAPTURE_FLAG_PTT;

	m_dwAudioCaptureFlags2 &= ~(SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_TYPE_MASK | SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_MASK);

	if (bAudioCapture2)
	{
		m_dwAudioCaptureFlags2 |= (m_nAudioSourceType2 - 1)<<SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_TYPE_SHIFT;
		m_dwAudioCaptureFlags2 |= m_nAudioSource2<<SAVEMEDIAPARAM_AUDIO_CAPTURE_SOURCE_SHIFT;
	}

	if(m_bUseEncoderServer)
		m_dwVideoCaptureFlagsEx |= SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_ENCODER_SERVER;
	else
		m_dwVideoCaptureFlagsEx &= ~SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_ENCODER_SERVER;

	if (m_nCaptureMode == CAPTURE_MODE_DESKTOP)
	{
		m_dwVideoCaptureFlagsEx &= ~(SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_FORCE_DESKTOP | SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_FORCE_PROCESS);
		m_dwVideoCaptureFlagsEx |= SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_FORCE_DESKTOP;
	}
	else
	if (m_nCaptureMode == CAPTURE_MODE_PROCESS)
	{
		m_dwVideoCaptureFlagsEx &= ~(SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_FORCE_DESKTOP | SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_FORCE_PROCESS);
		m_dwVideoCaptureFlagsEx |= SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_FORCE_PROCESS;
	}
	else
		m_dwVideoCaptureFlagsEx &= ~(SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_FORCE_DESKTOP | SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_FORCE_PROCESS);

	CDialog::OnOK();
}
//////////////////////////////////////////////////////////////////////
HBRUSH CVideoCaptureDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	COLORREF clrBk		= g_dwHeaderBgndColor;
	COLORREF clrText	= g_dwHeaderTextColor;

	UINT nID			= pWnd->GetDlgCtrlID();

	if ((nID == IDC_GENERAL_VIDEO_PROPERTIES_HEADER				) ||
		(nID == IDC_VIDEO_STREAM_PROPERTIES_HEADER				) ||
		(nID == IDC_AUDIO_STREAM_PROPERTIES_HEADER				))
	{
		if (!m_hBrush)
 			m_hBrush = CreateSolidBrush(clrBk);

		pDC->SetBkColor(clrBk);
		pDC->SetTextColor(clrText);
	}
	else 
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return m_hBrush;
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::OnDestroy()
{
	if (m_hBrush)
	{
		DeleteObject(m_hBrush);

		m_hBrush = NULL;
	}

	CDialog::OnDestroy();
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::UpdateVideoCaptureFramerateControls()
{
	CString strCaption;
	strCaption.Format(LocalizeStr("Framerate %d FPS"), m_dwVideoCaptureFramerate);

	GetDlgItem(IDC_VIDEO_FRAMERATE_CAPTION)->SetWindowText(strCaption);
}
//////////////////////////////////////////////////////////////////////
#define ENUMAUDIOSOURCES_BUF_SIZE 0x10000
//////////////////////////////////////////////////////////////////////
int CVideoCaptureDlg::InitAudioSourcesList(CComboBox* lpCtrl, int nAudioSourceType, int nAudioSource)
{
	lpCtrl->ResetContent();

	char mszBuf[ENUMAUDIOSOURCES_BUF_SIZE];

	CSaveMediaInterface encoderInterface;

	if (encoderInterface.EnumAudioSources(nAudioSourceType - 1, ENUMAUDIOSOURCES_BUF_SIZE, mszBuf))
		//enumerate audio sources by type via media encoder library interface then parse result
		//retuned in REG_MULTI_SZ format to populate the list of sources
	{
		CMultiString mstr(mszBuf);

		LPCSTR lpString = mstr.GetNext();

		while (lpString)
		{
			if (!lpCtrl->GetCount())
				lpCtrl->AddString(LocalizeStr("Auto select"));

			lpCtrl->AddString(lpString);

			lpString = mstr.GetNext();
		}
	}

	if (nAudioSource >= lpCtrl->GetCount())
		//correct audio source index if it is invalid
		nAudioSource = 0;

	if (nAudioSourceType > AUDIO_SOURCE_TYPE_AUTO)
	{
		lpCtrl->ShowWindow(SW_SHOW);

		if (!lpCtrl->GetCount())
		{
			lpCtrl->AddString(LocalizeStr("There are no devices"));
			lpCtrl->SetCurSel(0);
			lpCtrl->EnableWindow(FALSE);
		}
		else
		{
			lpCtrl->SetCurSel(nAudioSource);
			lpCtrl->EnableWindow(TRUE);
		}
	}
	else
	{
		lpCtrl->ShowWindow(SW_HIDE);
	}

	return nAudioSource;
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::InitAudioSourcesList()
{
	m_nAudioSource = InitAudioSourcesList(&m_ctrlAudioSourceCombo, m_nAudioSourceType, m_nAudioSource);
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::InitAudioSourcesList2()
{
	m_nAudioSource2 = InitAudioSourcesList(&m_ctrlAudioSourceCombo2, m_nAudioSourceType2, m_nAudioSource2);
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::InitAudioControls()
{
	if (m_bWASAPIAvailable)
	{
		if ((m_nAudioSourceType  == AUDIO_SOURCE_TYPE_AUTO			) || 
			(m_nAudioSourceType  >= AUDIO_SOURCE_TYPE_WASAPI_RENDER	) ||
			(m_nAudioSourceType2 == AUDIO_SOURCE_TYPE_AUTO			) || 
			(m_nAudioSourceType2 >= AUDIO_SOURCE_TYPE_WASAPI_RENDER))
		{
			GetDlgItem(IDC_FORCE_STEREO_CHECK		)->EnableWindow(TRUE);
			GetDlgItem(IDC_FORCE_STEREO_MODE_COMBO	)->EnableWindow(m_bAudioCaptureForceStereo);
		}
		else
		{
			GetDlgItem(IDC_FORCE_STEREO_CHECK)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORCE_STEREO_MODE_COMBO)->EnableWindow(FALSE);
		}
	}
	else
	{
		GetDlgItem(IDC_FORCE_STEREO_CHECK		)->EnableWindow(FALSE);
		GetDlgItem(IDC_FORCE_STEREO_MODE_COMBO	)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_MIX_CHECK)->EnableWindow(m_nAudioSourceType && m_nAudioSourceType2);
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::OnCbnSelchangeAudioCaptureSourceTypeCombo()
{
	UpdateData(TRUE);

	m_nAudioSource = 0;

	InitAudioSourcesList();
	InitAudioControls();
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::OnCbnSelchangeAudioCaptureSourceCombo()
{
	m_nAudioSource = m_ctrlAudioSourceCombo.GetCurSel();
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::OnCbnSelchangeAudioCaptureSourceType2Combo()
{
	UpdateData(TRUE);

	m_nAudioSource2 = 0;

	InitAudioSourcesList2();
	InitAudioControls();
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::OnCbnSelchangeAudioCapture2SourceCombo()
{
	m_nAudioSource2 = m_ctrlAudioSourceCombo2.GetCurSel();
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::OnBnClickedForceStereoCheck()
{
	UpdateData(TRUE);

	InitAudioControls();
}
//////////////////////////////////////////////////////////////////////
LRESULT CVideoCaptureDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ((message == WM_COMMAND) && ((wParam>>16) == EN_CHANGE))
	{
		switch (wParam & 0xFFFF)
		{
		case IDC_PTT_HOTKEY:
			GetHotkeyCtrl(&m_ctrlPTTHotkey, HOTKEY_VIDEO_CAPTURE_PTT);
			break;
		case IDC_PTT2_HOTKEY:
			GetHotkeyCtrl(&m_ctrlPTT2Hotkey, HOTKEY_VIDEO_CAPTURE_PTT2);
			break;
		}
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::SetHotkeyCtrl(CHotKeyCtrl* pCtrl, DWORD dwHotkeyID)
{
	DWORD dwHotkey = m_hotkeyManager.GetHotkey(dwHotkeyID);

	pCtrl->SetHotKey((WORD)(dwHotkey & 0xffff), (WORD)(dwHotkey>>16));
}
//////////////////////////////////////////////////////////////////////
void CVideoCaptureDlg::GetHotkeyCtrl(CHotKeyCtrl* pCtrl, DWORD dwHotkeyID)
{
	WORD wVirtualKeyCode, wModifiers;
	pCtrl->GetHotKey(wVirtualKeyCode, wModifiers);

	DWORD dwHotkey = wVirtualKeyCode | (wModifiers<<16);

	if (!m_hotkeyManager.ValidateHotkey(dwHotkeyID, dwHotkey))
	{
		pCtrl->SetHotKey(0, 0);

		m_hotkeyManager.SetHotkey(dwHotkeyID, 0);
	}
	else
		m_hotkeyManager.SetHotkey(dwHotkeyID, dwHotkey);
}
//////////////////////////////////////////////////////////////////////
