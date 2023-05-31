// OverlayModifierDlg.cpp : implementation file
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HotkeyHandler.h"
#include "OverlayModifierDlg.h"
//////////////////////////////////////////////////////////////////////
// COverlayModifierDlg dialog
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(COverlayModifierDlg, CDialog)
//////////////////////////////////////////////////////////////////////
#define PRESET_CUSTOM												0
#define PRESET_LOAD													1
#define PRESET_LOAD_TEXT											2
#define PRESET_VIEW_TEXT											3
#define PRESET_NEXT_TEXT_LINE										4
#define PRESET_PREV_TEXT_LINE										5
#define PRESET_SET_TIMER											6
#define PRESET_SET_PING_ADDR										7
//////////////////////////////////////////////////////////////////////
COverlayModifierDlg::COverlayModifierDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COverlayModifierDlg::IDD, pParent)
	, m_strPreset(_T(""))
{
	m_strDesc		= "";
	m_strMessage	= "";
	m_strLayer		= "";
	m_strParams		= "";
}
//////////////////////////////////////////////////////////////////////
COverlayModifierDlg::~COverlayModifierDlg()
{
}
//////////////////////////////////////////////////////////////////////
void COverlayModifierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DESC_EDIT, m_strDesc);
	DDX_Control(pDX, IDC_PRESET_COMBO, m_ctrlPresetCombo);
	DDX_Text(pDX, IDC_MESSAGE_EDIT, m_strMessage);
	DDX_Text(pDX, IDC_LAYER_EDIT, m_strLayer);
	DDX_Text(pDX, IDC_PARAMS_EDIT, m_strParams);
	DDX_CBString(pDX, IDC_PRESET_COMBO, m_strPreset);
}
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(COverlayModifierDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_PRESET_COMBO, &COverlayModifierDlg::OnCbnSelchangePresetCombo)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////
// COverlayModifierDlg message handlers
//////////////////////////////////////////////////////////////////////
BOOL COverlayModifierDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LocalizeWnd(m_hWnd);
	AdjustWindowPos(this, GetParent());

	m_ctrlPresetCombo.SetCurSel(0);

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
void COverlayModifierDlg::OnCbnSelchangePresetCombo()
{
	UpdateData(TRUE);

	switch (m_ctrlPresetCombo.GetCurSel())
	{
	case PRESET_LOAD:
		m_strDesc		= m_strPreset;
		m_strMessage	= "Load";
		m_strLayer		= "";
		m_strParams		= "default.ovl";
		break;
	case PRESET_LOAD_TEXT:
		m_strDesc		= m_strPreset;
		m_strMessage	= "LoadText";
		m_strLayer		= "";
		m_strParams		= "";
		break;
	case PRESET_VIEW_TEXT:
		m_strDesc		= m_strPreset;
		m_strMessage	= "ViewText";
		m_strLayer		= "";
		m_strParams		= "";
		break;
	case PRESET_NEXT_TEXT_LINE:
		m_strDesc		= m_strPreset;
		m_strMessage	= "NextTextLine";
		m_strLayer		= "";
		m_strParams		= "";
		break;
	case PRESET_PREV_TEXT_LINE:
		m_strDesc		= m_strPreset;
		m_strMessage	= "PrevTextLine";
		m_strLayer		= "";
		m_strParams		= "";
		break;
	case PRESET_SET_TIMER:
		m_strDesc		= m_strPreset;
		m_strMessage	= "SetTimer";
		m_strLayer		= "";
		m_strParams		= "-30";
		break;
	case PRESET_SET_PING_ADDR:
		m_strDesc		= m_strPreset;
		m_strMessage	= "SetPingAddr";
		m_strLayer		= "";
		m_strParams		= "www.guru3d.com";
		break;
	}

	UpdateData(FALSE);

}
//////////////////////////////////////////////////////////////////////
