// OverlayEditorDlg.cpp : implementation file
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HotkeyHandler.h"
#include "OverlayEditorDlg.h"
#include "OverlayModifierDlg.h"
//////////////////////////////////////////////////////////////////////
// COverlayEditorDlg dialog
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(COverlayEditorDlg, CDialog)
//////////////////////////////////////////////////////////////////////
COverlayEditorDlg::COverlayEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COverlayEditorDlg::IDD, pParent)
{
	m_strOVM1Desc		= "";
	m_strOVM1Message	= "";
	m_strOVM1Layer		= "";
	m_strOVM1Params		= "";

	m_strOVM2Desc		= "";
	m_strOVM2Message	= "";
	m_strOVM2Layer		= "";
	m_strOVM2Params		= "";

	m_strOVM3Desc		= "";
	m_strOVM3Message	= "";
	m_strOVM3Layer		= "";
	m_strOVM3Params		= "";

	m_strOVM4Desc		= "";
	m_strOVM4Message	= "";
	m_strOVM4Layer		= "";
	m_strOVM4Params		= "";
}
//////////////////////////////////////////////////////////////////////
COverlayEditorDlg::~COverlayEditorDlg()
{
}
//////////////////////////////////////////////////////////////////////
void COverlayEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OVM1_HOTKEY, m_ctrlOVM1Hotkey);
	DDX_Control(pDX, IDC_OVM2_HOTKEY, m_ctrlOVM2Hotkey);
	DDX_Control(pDX, IDC_OVM3_HOTKEY, m_ctrlOVM3Hotkey);
	DDX_Control(pDX, IDC_OVM4_HOTKEY, m_ctrlOVM4Hotkey);
}
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(COverlayEditorDlg, CDialog)
	ON_BN_CLICKED(IDC_OVM1_SETTINGS_BUTTON, &COverlayEditorDlg::OnBnClickedOvm1SettingsButton)
	ON_BN_CLICKED(IDC_OVM2_SETTINGS_BUTTON, &COverlayEditorDlg::OnBnClickedOvm2SettingsButton)
	ON_BN_CLICKED(IDC_OVM3_SETTINGS_BUTTON, &COverlayEditorDlg::OnBnClickedOvm3SettingsButton)
	ON_BN_CLICKED(IDC_OVM4_SETTINGS_BUTTON, &COverlayEditorDlg::OnBnClickedOvm4SettingsButton)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////
// COverlayEditorDlg message handlers
//////////////////////////////////////////////////////////////////////
BOOL COverlayEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LocalizeWnd(m_hWnd);
	AdjustWindowPos(this, GetParent());

	SetHotkeyCtrl(&m_ctrlOVM1Hotkey			, HOTKEY_OVERLAY_MESSAGE1	);
	SetHotkeyCtrl(&m_ctrlOVM2Hotkey			, HOTKEY_OVERLAY_MESSAGE2	);
	SetHotkeyCtrl(&m_ctrlOVM3Hotkey			, HOTKEY_OVERLAY_MESSAGE3	);
	SetHotkeyCtrl(&m_ctrlOVM4Hotkey			, HOTKEY_OVERLAY_MESSAGE4	);

	UpdateControls();

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
void COverlayEditorDlg::SetHotkeyCtrl(CHotKeyCtrl* pCtrl, DWORD dwHotkeyID)
{
	DWORD dwHotkey = m_hotkeyManager.GetHotkey(dwHotkeyID);

	pCtrl->SetHotKey((WORD)(dwHotkey & 0xffff), (WORD)(dwHotkey>>16));
}
//////////////////////////////////////////////////////////////////////
void COverlayEditorDlg::GetHotkeyCtrl(CHotKeyCtrl* pCtrl, DWORD dwHotkeyID)
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
LRESULT COverlayEditorDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ((message == WM_COMMAND) && ((wParam>>16) == EN_CHANGE))
	{
		switch (wParam & 0xFFFF)
		{
		case IDC_OVM1_HOTKEY:
			GetHotkeyCtrl(&m_ctrlOVM1Hotkey		, HOTKEY_OVERLAY_MESSAGE1);
			break;
		case IDC_OVM2_HOTKEY:
			GetHotkeyCtrl(&m_ctrlOVM2Hotkey		, HOTKEY_OVERLAY_MESSAGE2);
			break;
		case IDC_OVM3_HOTKEY:
			GetHotkeyCtrl(&m_ctrlOVM3Hotkey		, HOTKEY_OVERLAY_MESSAGE3);
			break;
		case IDC_OVM4_HOTKEY:
			GetHotkeyCtrl(&m_ctrlOVM4Hotkey		, HOTKEY_OVERLAY_MESSAGE4);
			break;
		}
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////
void COverlayEditorDlg::OnBnClickedOvm1SettingsButton()
{
	COverlayModifierDlg dlg(this);

	dlg.m_strDesc		= m_strOVM1Desc;
	dlg.m_strMessage	= m_strOVM1Message;
	dlg.m_strLayer		= m_strOVM1Layer;
	dlg.m_strParams		= m_strOVM1Params;

	if (dlg.DoModal() == IDOK)
	{
		m_strOVM1Desc		= dlg.m_strDesc;
		m_strOVM1Message	= dlg.m_strMessage;
		m_strOVM1Layer		= dlg.m_strLayer;
		m_strOVM1Params		= dlg.m_strParams;
	}

	UpdateControls();
}
//////////////////////////////////////////////////////////////////////
void COverlayEditorDlg::OnBnClickedOvm2SettingsButton()
{
	COverlayModifierDlg dlg(this);

	dlg.m_strDesc		= m_strOVM2Desc;
	dlg.m_strMessage	= m_strOVM2Message;
	dlg.m_strLayer		= m_strOVM2Layer;
	dlg.m_strParams		= m_strOVM2Params;

	if (dlg.DoModal() == IDOK)
	{
		m_strOVM2Desc		= dlg.m_strDesc;
		m_strOVM2Message	= dlg.m_strMessage;
		m_strOVM2Layer		= dlg.m_strLayer;
		m_strOVM2Params		= dlg.m_strParams;
	}

	UpdateControls();
}
//////////////////////////////////////////////////////////////////////
void COverlayEditorDlg::OnBnClickedOvm3SettingsButton()
{
	COverlayModifierDlg dlg(this);

	dlg.m_strDesc		= m_strOVM3Desc;
	dlg.m_strMessage	= m_strOVM3Message;
	dlg.m_strLayer		= m_strOVM3Layer;
	dlg.m_strParams		= m_strOVM3Params;

	if (dlg.DoModal() == IDOK)
	{
		m_strOVM3Desc		= dlg.m_strDesc;
		m_strOVM3Message	= dlg.m_strMessage;
		m_strOVM3Layer		= dlg.m_strLayer;
		m_strOVM3Params		= dlg.m_strParams;
	}

	UpdateControls();

}
//////////////////////////////////////////////////////////////////////
void COverlayEditorDlg::OnBnClickedOvm4SettingsButton()
{
	COverlayModifierDlg dlg(this);

	dlg.m_strDesc		= m_strOVM4Desc;
	dlg.m_strMessage	= m_strOVM4Message;
	dlg.m_strLayer		= m_strOVM4Layer;
	dlg.m_strParams		= m_strOVM4Params;

	if (dlg.DoModal() == IDOK)
	{
		m_strOVM4Desc		= dlg.m_strDesc;
		m_strOVM4Message	= dlg.m_strMessage;
		m_strOVM4Layer		= dlg.m_strLayer;
		m_strOVM4Params		= dlg.m_strParams;
	}

	UpdateControls();

}
//////////////////////////////////////////////////////////////////////
void COverlayEditorDlg::UpdateControls()
{
	if (strlen(m_strOVM1Desc))
		GetDlgItem(IDC_OVM1_DESC_CAPTION)->SetWindowText(LocalizeStr(m_strOVM1Desc));
	else
		GetDlgItem(IDC_OVM1_DESC_CAPTION)->SetWindowText(LocalizeStr("Programmable overlay modifier 1"));

	if (strlen(m_strOVM2Desc))
		GetDlgItem(IDC_OVM2_DESC_CAPTION)->SetWindowText(LocalizeStr(m_strOVM2Desc));
	else
		GetDlgItem(IDC_OVM2_DESC_CAPTION)->SetWindowText(LocalizeStr("Programmable overlay modifier 2"));

	if (strlen(m_strOVM3Desc))
		GetDlgItem(IDC_OVM3_DESC_CAPTION)->SetWindowText(LocalizeStr(m_strOVM3Desc));
	else
		GetDlgItem(IDC_OVM3_DESC_CAPTION)->SetWindowText(LocalizeStr("Programmable overlay modifier 3"));

	if (strlen(m_strOVM4Desc))
		GetDlgItem(IDC_OVM4_DESC_CAPTION)->SetWindowText(LocalizeStr(m_strOVM4Desc));
	else
		GetDlgItem(IDC_OVM4_DESC_CAPTION)->SetWindowText(LocalizeStr("Programmable overlay modifier 4"));
}
//////////////////////////////////////////////////////////////////////
