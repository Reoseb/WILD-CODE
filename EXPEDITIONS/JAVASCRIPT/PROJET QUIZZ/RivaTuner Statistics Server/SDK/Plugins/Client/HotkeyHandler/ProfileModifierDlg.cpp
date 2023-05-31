// ProfileModifierDlg.h: interface for the CProfileModifierDlg class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HotkeyHandler.h"
#include "ProfileModifierDlg.h"
#include "RTSSHooksInterface.h"
//////////////////////////////////////////////////////////////////////
// CProfileModifierDlg dialog
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CProfileModifierDlg, CDialog)
//////////////////////////////////////////////////////////////////////
#define PRESET_CUSTOM												0
#define PRESET_SET_FRAMERATE_LIMIT									1
#define PRESET_INCREASE_FRAMERATE_LIMIT								2
#define PRESET_DECREASE_FRAMERATE_LIMIT								3
#define PRESET_INCREASE_SCANLINE_SYNC_INDEX							4
#define PRESET_DECREASE_SCANLINE_SYNC_INDEX							5
//////////////////////////////////////////////////////////////////////
#define FRAMERATE_LIMIT_PROPERTY				"FramerateLimit"
#define SCANLINE_SYNC_PROPERTY					"ScanlineSync"
//////////////////////////////////////////////////////////////////////
CProfileModifierDlg::CProfileModifierDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProfileModifierDlg::IDD, pParent)
{
	m_strDesc		= "";
	m_strProfile	= "";
	m_strProperty	= "";
	m_nType			= 0;
	m_nValue		= 0;

	m_strPreset		= "";
}
//////////////////////////////////////////////////////////////////////
CProfileModifierDlg::~CProfileModifierDlg()
{
}
//////////////////////////////////////////////////////////////////////
void CProfileModifierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DESC_EDIT, m_strDesc);
	DDX_Text(pDX, IDC_PROFILE_EDIT, m_strProfile);
	DDX_Text(pDX, IDC_PROPERTY_EDIT, m_strProperty);
	DDX_CBIndex(pDX, IDC_TYPE_COMBO, m_nType);
	DDX_Text(pDX, IDC_VALUE_EDIT, m_nValue);
	DDX_CBString(pDX, IDC_PRESET_COMBO, m_strPreset);
	DDX_Control(pDX, IDC_PRESET_COMBO, m_ctrlPresetCombo);
}
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CProfileModifierDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_PRESET_COMBO, &CProfileModifierDlg::OnCbnSelchangePresetCombo)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////
// CProfileModifierDlg message handlers
//////////////////////////////////////////////////////////////////////
void CProfileModifierDlg::OnCbnSelchangePresetCombo()
{
	UpdateData(TRUE);

	switch (m_ctrlPresetCombo.GetCurSel())
	{
	case PRESET_SET_FRAMERATE_LIMIT:
		m_strDesc		= m_strPreset;
		m_strProperty	= FRAMERATE_LIMIT_PROPERTY;
		m_nType			= 0;
		m_nValue		= 60;
		break;
	case PRESET_INCREASE_FRAMERATE_LIMIT:
		m_strDesc		= m_strPreset;
		m_strProperty	= FRAMERATE_LIMIT_PROPERTY;
		m_nType			= 1;
		m_nValue		= 1;
		break;
	case PRESET_DECREASE_FRAMERATE_LIMIT:
		m_strDesc		= m_strPreset;
		m_strProperty	= FRAMERATE_LIMIT_PROPERTY;
		m_nType			= 1;
		m_nValue		= -1;
		break;
	case PRESET_INCREASE_SCANLINE_SYNC_INDEX:
		m_strDesc		= m_strPreset;
		m_strProperty	= SCANLINE_SYNC_PROPERTY;
		m_nType			= 1;
		m_nValue		= 1;
		break;
	case PRESET_DECREASE_SCANLINE_SYNC_INDEX:
		m_strDesc		= m_strPreset;
		m_strProperty	= SCANLINE_SYNC_PROPERTY;
		m_nType			= 1;
		m_nValue		= -1;
		break;
	}

	UpdateData(FALSE);
}
//////////////////////////////////////////////////////////////////////
BOOL CProfileModifierDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LocalizeWnd(m_hWnd);
	AdjustWindowPos(this, GetParent());

	m_ctrlPresetCombo.SetCurSel(0);

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
void CProfileModifierDlg::OnOK()
{
	UpdateData(TRUE);

	if (strlen(m_strProperty))
		//validate property name via RTSSHooks interface
	{
		CRTSSHooksInterface rtssInterface;

		DWORD dwProperty;

		if (!rtssInterface.GetProfileProperty(m_strProperty, (LPBYTE)&dwProperty, sizeof(dwProperty)))
			//we don't even need to load profile, just try to get uninitialized property value to see if the name exists
		{
			MessageBox(LocalizeStr("You've specified invalid profile property!"), LocalizeStr("Warning"), MB_OK | MB_ICONWARNING);

			return;
		}
	}

	CDialog::OnOK();
}
//////////////////////////////////////////////////////////////////////
