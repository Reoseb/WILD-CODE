// VideoCaptureDlg.h: interface for the CVideoCaptureDlg class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
//////////////////////////////////////////////////////////////////////
// CVideoCaptureDlg dialog
//////////////////////////////////////////////////////////////////////
#define FORMAT_UNCOMPRESSED									0
#define FORMAT_NV12											1
#define FORMAT_RTV1											2
#define FORMAT_MJPG											3
#define FORMAT_VFW											4
#define FORMAT_EXT											5
//////////////////////////////////////////////////////////////////////
#define CONTAINER_AVI										0
#define CONTAINER_MKV										1
//////////////////////////////////////////////////////////////////////
class CVideoCaptureDlg : public CDialog
{
	DECLARE_DYNAMIC(CVideoCaptureDlg)

public:
	CVideoCaptureDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVideoCaptureDlg();

	DWORD	GetFormat();
	void	SetFormat(DWORD dwFormat);
	DWORD	StrToFormat(LPCSTR lpFormat);
	CString	FormatToStr(DWORD dwFormat);

	DWORD	StrToContainer(LPCSTR lpContainer);
	CString	ContainerToStr(DWORD dwContainer);

	void	SetHotkeyCtrl(CHotKeyCtrl* pCtrl, DWORD dwHotkeyID);
	void	GetHotkeyCtrl(CHotKeyCtrl* pCtrl, DWORD dwHotkeyID);

	int		InitAudioSourcesList(CComboBox* lpCtrl, int nAudioSourceType, int nAudioSource);
	void	InitAudioSourcesList();
	void	InitAudioSourcesList2();
	void	InitAudioControls();

	void	UpdateVideoCaptureQualityControls();
	void	UpdateVideoCaptureFramerateControls();
	void	UpdateVideoCaptureFormatControls(BOOL bReinit = FALSE);

	CComboBox	m_ctrlVideoCaptureFormatCombo;
	CSliderCtrl m_ctrlVideoCaptureQualitySlider;
	CComboBox	m_ctrlVideoCaptureFramesizeCombo;
	CSliderCtrl m_ctrlVideoCaptureFramerateSlider;
	CComboBox	m_ctrlAudioSourceCombo;
	CComboBox	m_ctrlAudioSourceCombo2;
	CHotKeyCtrl m_ctrlPTTHotkey;
	CHotKeyCtrl m_ctrlPTT2Hotkey;

	CString	m_strVideoCaptureContainer;
	CString	m_strVideoCaptureFormat;
	CString	m_strVideoCaptureFolder;
	DWORD	m_dwVideoCaptureQuality;
	DWORD	m_dwVideoCaptureFramesize;
	DWORD	m_dwVideoCaptureFramerate;
	DWORD	m_dwAudioCaptureFlags;
	DWORD	m_dwAudioCaptureFlags2;
	DWORD	m_dwVideoCaptureFlagsEx;
	CString	m_strCustomFramesizes;
	int		m_nCaptureMode;
	int		m_nAudioSourceType;
	int		m_nAudioSource;
	int		m_nAudioSourceType2;
	int		m_nAudioSource2;
	BOOL	m_bAudioCaptureForceStereo;
	int		m_nForceStereoMode;
	BOOL	m_bAudioCaptureMix;
	BOOL	m_bUseEncoderServer;
	BOOL	m_bCaptureOSD;

	CHotkeyManager m_hotkeyManager;

// Dialog Data
	enum { IDD = IDD_VIDEO_CAPTURE_SETTINGS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

public:
	virtual	BOOL OnInitDialog();
	afx_msg void OnBnClickedBrowseButton();
	afx_msg void OnBnClickedViewButton();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelchangeVideoFormatCombo();
	afx_msg void OnBnClickedVideoFormatSettingsButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeAudioCaptureSourceTypeCombo();
	afx_msg void OnCbnSelchangeAudioCaptureSourceCombo();
	afx_msg void OnCbnSelchangeAudioCaptureSourceType2Combo();
	afx_msg void OnCbnSelchangeAudioCapture2SourceCombo();
	afx_msg void OnBnClickedForceStereoCheck();

protected:
	HBRUSH	m_hBrush;

	BOOL	m_bWASAPIAvailable;	
		//WASAPI audio capture mode is only available under Vista and newer OS, so this flag will be set depending on OS version

	CString m_strVFWCodec;
		//Name of external VFW codec
	BOOL	m_bVFWQualitySupported;
		//this flag will inidcate if selected VFW codec is capable of quality adjustment, in this case we'll map video quality control
		//slider to it

	CString m_strEXTCodec;
		//Name of external encoder module
	DWORD	m_dwEXTCodec;
		//FOURCC of external encoder module

	//VFW and extended encoder configuration is handled by SaveMedia.dll media encoding library, the following methods use SaveMedia 
	//interface to get current VFW and excended endcoder configs or to display configuration GUI for them

	void	ConfigureVFW(DWORD dwFlags);
	void	ConfigureEXT(DWORD dwFlags);
};
//////////////////////////////////////////////////////////////////////
