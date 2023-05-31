#pragma once
//////////////////////////////////////////////////////////////////////
#include "resource.h"
#include "afxwin.h"
//////////////////////////////////////////////////////////////////////
// COverlayModifierDlg dialog
//////////////////////////////////////////////////////////////////////
class COverlayModifierDlg : public CDialog
{
	DECLARE_DYNAMIC(COverlayModifierDlg)

public:
	COverlayModifierDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COverlayModifierDlg();

	CComboBox m_ctrlPresetCombo;

	CString m_strDesc;
	CString m_strPreset;
	CString m_strMessage;
	CString m_strLayer;
	CString m_strParams;

// Dialog Data
	enum { IDD = IDD_OVERLAY_MODIFIER_SETTINGS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangePresetCombo();
};
//////////////////////////////////////////////////////////////////////
