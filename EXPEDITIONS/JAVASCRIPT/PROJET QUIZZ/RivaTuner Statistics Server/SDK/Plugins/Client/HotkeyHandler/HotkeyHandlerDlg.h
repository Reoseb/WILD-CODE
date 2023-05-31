// HotkeyHandlerDlg.h: interface for the CHotkeyHandlerDlg class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
#include "resource.h"
#include "afxcmn.h"
#include "HotkeyManager.h"
//////////////////////////////////////////////////////////////////////
// CHotkeyHandlerDlg dialog
//////////////////////////////////////////////////////////////////////
class CHotkeyHandlerDlg : public CDialog
{
	DECLARE_DYNAMIC(CHotkeyHandlerDlg)

public:
	CHotkeyHandlerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHotkeyHandlerDlg();

	void SetHotkeyCtrl(CHotKeyCtrl* pCtrl, DWORD dwHotkeyID);
	void GetHotkeyCtrl(CHotKeyCtrl* pCtrl, DWORD dwHotkeyID);

	void UpdateProfileControls();

	CHotKeyCtrl m_ctrlOSDOnHotkey;
	CHotKeyCtrl m_ctrlOSDOffHotkey;
	CHotKeyCtrl m_ctrlOSDToggleHotkey;
	CHotKeyCtrl m_ctrlLimiterOnHotkey;
	CHotKeyCtrl m_ctrlLimiterOffHotkey;
	CHotKeyCtrl m_ctrlLimiterToggleHotkey;
	CHotKeyCtrl m_ctrlScreenCaptureHotkey;
	CHotKeyCtrl m_ctrlVideoCaptureHotkey;
	CHotKeyCtrl m_ctrlVideoPrerecordHotkey;
	CHotKeyCtrl m_ctrlBenchmarkBeginHotkey;
	CHotKeyCtrl m_ctrlBenchmarkEndHotkey;
	CHotKeyCtrl m_ctrlPPM1Hotkey;
	CHotKeyCtrl m_ctrlPPM2Hotkey;
	CHotKeyCtrl m_ctrlPPM3Hotkey;
	CHotKeyCtrl m_ctrlPPM4Hotkey;

	CHotkeyHandlerConfig m_config;

// Dialog Data
	enum { IDD = IDD_HOTKEY_HANDLER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedScreenCaptureSettingsButton();
	afx_msg void OnBnClickedBenchmarkSettingsButton();
	afx_msg void OnBnClickedVideoCaptureSettingsButton();
	afx_msg void OnBnClickedPrerecordSettingsButton();
	afx_msg void OnBnClickedPpm1SettingsButton();
	afx_msg void OnBnClickedPpm2SettingsButton();
	afx_msg void OnBnClickedPpm3SettingsButton();
	afx_msg void OnBnClickedPpm4SettingsButton();

protected:
	HBRUSH	m_hBrush;
public:
	afx_msg void OnBnClickedConfigureOverlayEditorButton();
};
//////////////////////////////////////////////////////////////////////
