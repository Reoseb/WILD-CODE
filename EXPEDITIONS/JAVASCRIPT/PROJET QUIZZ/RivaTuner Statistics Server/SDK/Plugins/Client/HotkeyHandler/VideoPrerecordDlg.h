// VideoCaptureDlg.h: interface for the CVideoCaptureDlg class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
#include "resource.h"
//////////////////////////////////////////////////////////////////////
// CVideoPrerecordDlg dialog
//////////////////////////////////////////////////////////////////////
class CVideoPrerecordDlg : public CDialog
{
	DECLARE_DYNAMIC(CVideoPrerecordDlg)

public:
	CVideoPrerecordDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVideoPrerecordDlg();

	DWORD	m_dwVideoCaptureFlagsEx;
	DWORD	m_dwPrerecordTimeLimit;
	DWORD	m_dwPrerecordSizeLimit;

	int		m_nPrerecordLocation;
	BOOL	m_bAutoPrerecord;

// Dialog Data
	enum { IDD = IDD_VIDEO_PRERECORD_SETTINGS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposPrerecordTimeLimitSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposPrerecordSizeLimitSpin(NMHDR *pNMHDR, LRESULT *pResult);
};
//////////////////////////////////////////////////////////////////////
