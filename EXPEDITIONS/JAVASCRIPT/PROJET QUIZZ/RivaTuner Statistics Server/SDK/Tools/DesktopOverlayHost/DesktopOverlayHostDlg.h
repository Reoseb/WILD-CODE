// DesktopOverlayHostDlg.h : header file
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "resource.h"
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
// CDesktopOverlayHostDlg dialog
//////////////////////////////////////////////////////////////////////
class CDesktopOverlayHostDlg : public CDialog
{
// Construction
public:
	CDesktopOverlayHostDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DESKTOPOVERLAYHOST_DIALOG };

	BOOL	m_bTransparent;
	BOOL	m_bColorKey;

	DWORD	m_dwBgndColor;
	DWORD	m_dwAlpha;

	HBRUSH	m_hBrushBgndColorPreview;

	CSliderCtrl m_ctrlAlphaSlider;

	void	UpdateColorControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedBgndColorPreview();
};
//////////////////////////////////////////////////////////////////////
