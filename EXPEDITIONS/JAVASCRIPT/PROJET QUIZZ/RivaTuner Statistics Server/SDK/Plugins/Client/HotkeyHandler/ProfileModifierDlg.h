// ProfileModifierDlg.h: interface for the CProfileModifierDlg class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
#include "resource.h"
#include "afxwin.h"
//////////////////////////////////////////////////////////////////////
// CProfileModifierDlg dialog
//////////////////////////////////////////////////////////////////////
class CProfileModifierDlg : public CDialog
{
	DECLARE_DYNAMIC(CProfileModifierDlg)

public:
	CProfileModifierDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProfileModifierDlg();

	CString m_strDesc;
	CString m_strPreset;
	CString m_strProfile;
	CString m_strProperty;
	int		m_nType;
	int		m_nValue;

// Dialog Data
	enum { IDD = IDD_PROFILE_MODIFIER_SETTINGS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();

public:
	afx_msg void OnCbnSelchangePresetCombo();
	CComboBox m_ctrlPresetCombo;
	virtual BOOL OnInitDialog();
};
//////////////////////////////////////////////////////////////////////
