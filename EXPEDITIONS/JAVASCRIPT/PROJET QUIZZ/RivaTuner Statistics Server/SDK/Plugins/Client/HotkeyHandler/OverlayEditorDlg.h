#pragma once
//////////////////////////////////////////////////////////////////////
#include "resource.h"
#include "afxcmn.h"
//////////////////////////////////////////////////////////////////////
// COverlayEditorDlg dialog
//////////////////////////////////////////////////////////////////////
class COverlayEditorDlg : public CDialog
{
	DECLARE_DYNAMIC(COverlayEditorDlg)

public:
	COverlayEditorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COverlayEditorDlg();

	void UpdateControls();

	void SetHotkeyCtrl(CHotKeyCtrl* pCtrl, DWORD dwHotkeyID);
	void GetHotkeyCtrl(CHotKeyCtrl* pCtrl, DWORD dwHotkeyID);

	CHotKeyCtrl m_ctrlOVM1Hotkey;
	CHotKeyCtrl m_ctrlOVM2Hotkey;
	CHotKeyCtrl m_ctrlOVM3Hotkey;
	CHotKeyCtrl m_ctrlOVM4Hotkey;

	CString m_strOVM1Desc;
	CString m_strOVM1Message;
	CString m_strOVM1Layer;
	CString m_strOVM1Params;

	CString m_strOVM2Desc;
	CString m_strOVM2Message;
	CString m_strOVM2Layer;
	CString m_strOVM2Params;

	CString m_strOVM3Desc;
	CString m_strOVM3Message;
	CString m_strOVM3Layer;
	CString m_strOVM3Params;

	CString m_strOVM4Desc;
	CString m_strOVM4Message;
	CString m_strOVM4Layer;
	CString m_strOVM4Params;

	CHotkeyManager m_hotkeyManager;

// Dialog Data
	enum { IDD = IDD_OVERLAY_EDITOR_SETTINGS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOvm1SettingsButton();
	afx_msg void OnBnClickedOvm2SettingsButton();
	afx_msg void OnBnClickedOvm3SettingsButton();
	afx_msg void OnBnClickedOvm4SettingsButton();
};
//////////////////////////////////////////////////////////////////////
