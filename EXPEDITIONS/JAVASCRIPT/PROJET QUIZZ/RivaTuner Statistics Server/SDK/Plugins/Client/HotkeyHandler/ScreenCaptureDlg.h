// ScreenCaptureDlg.h: interface for the CScreenCaptureDlg class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
#include "resource.h"
#include "afxcmn.h"
//////////////////////////////////////////////////////////////////////
// CScreenCaptureDlg dialog
//////////////////////////////////////////////////////////////////////
#define FORMAT_BMP													0
#define FORMAT_PNG													1
#define FORMAT_JPG													2
//////////////////////////////////////////////////////////////////////
class CScreenCaptureDlg : public CDialog
{
	DECLARE_DYNAMIC(CScreenCaptureDlg)

public:
	CScreenCaptureDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScreenCaptureDlg();

	DWORD	StrToFormat(LPCSTR lpFormat);
	CString	FormatToStr(DWORD dwFormat);

	void	UpdateScreenCaptureQualityControls();

	CSliderCtrl m_ctrlScreenCaptureQualitySlider;

	int		m_nScreenCaptureFormat;
	CString	m_strScreenCaptureFormat;
	DWORD	m_dwScreenCaptureQuality;
	CString	m_strScreenCaptureFolder;
	BOOL	m_bCaptureOSD;

// Dialog Data
	enum { IDD = IDD_SCREEN_CAPTURE_SETTINGS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBmpRadio();
	afx_msg void OnBnClickedJpgRadio();
	afx_msg void OnBnClickedPngRadio();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBrowseButton();
	afx_msg void OnBnClickedViewButton();
};
//////////////////////////////////////////////////////////////////////
