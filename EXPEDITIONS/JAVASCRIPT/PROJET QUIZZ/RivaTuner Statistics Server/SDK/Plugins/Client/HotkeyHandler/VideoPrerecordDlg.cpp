// VideoPrerecordDlg.h: interface for the CVideoPrerecordDlg class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HotkeyHandler.h"
#include "VideoPrerecordDlg.h"
#include "SaveMediaTypes.h"
//////////////////////////////////////////////////////////////////////
// CVideoPrerecordDlg dialog
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CVideoPrerecordDlg, CDialog)
//////////////////////////////////////////////////////////////////////
CVideoPrerecordDlg::CVideoPrerecordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoPrerecordDlg::IDD, pParent)
{
	m_nPrerecordLocation	= 0;
	m_dwVideoCaptureFlagsEx = 0;
	m_dwPrerecordTimeLimit	= 0;
	m_dwPrerecordSizeLimit	= 0;
	m_nPrerecordLocation	= 0;
	m_bAutoPrerecord		= FALSE;
}
//////////////////////////////////////////////////////////////////////
CVideoPrerecordDlg::~CVideoPrerecordDlg()
{
}
//////////////////////////////////////////////////////////////////////
void CVideoPrerecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PRERECORD_TIME_LIMIT_EDIT, m_dwPrerecordTimeLimit);
	DDX_Text(pDX, IDC_PRERECORD_SIZE_LIMIT_EDIT, m_dwPrerecordSizeLimit);
	DDX_CBIndex(pDX, IDC_PRERECORD_LOCATION_COMBO, m_nPrerecordLocation);
	DDX_Check(pDX, IDC_AUTO_PRERECORD_CHECK, m_bAutoPrerecord);
}
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CVideoPrerecordDlg, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PRERECORD_TIME_LIMIT_SPIN, &CVideoPrerecordDlg::OnDeltaposPrerecordTimeLimitSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PRERECORD_SIZE_LIMIT_SPIN, &CVideoPrerecordDlg::OnDeltaposPrerecordSizeLimitSpin)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////
// CVideoPrerecordDlg message handlers
//////////////////////////////////////////////////////////////////////
BOOL CVideoPrerecordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LocalizeWnd(m_hWnd);
	AdjustWindowPos(this, GetParent());

	if (m_dwVideoCaptureFlagsEx & SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_PRERECORD_TO_RAM)
		m_nPrerecordLocation = 1;
	else
		m_nPrerecordLocation = 0;

	return TRUE; 
}
//////////////////////////////////////////////////////////////////////
void CVideoPrerecordDlg::OnOK()
{
	UpdateData(TRUE);

	if (m_nPrerecordLocation)
		m_dwVideoCaptureFlagsEx |= SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_PRERECORD_TO_RAM;
	else
		m_dwVideoCaptureFlagsEx &= ~SAVEMEDIAPARAM_VIDEO_CAPTURE_FLAGEX_PRERECORD_TO_RAM;

	CDialog::OnOK();
}
//////////////////////////////////////////////////////////////////////
void CVideoPrerecordDlg::OnDeltaposPrerecordTimeLimitSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	UpdateData(TRUE);

	int nPrerecordTimeLimit = m_dwPrerecordTimeLimit - pNMUpDown->iDelta;

	if (nPrerecordTimeLimit < 0)
		nPrerecordTimeLimit = 0;

	if (nPrerecordTimeLimit > 3600)
		nPrerecordTimeLimit = 3600;

	m_dwPrerecordTimeLimit	= nPrerecordTimeLimit;

	UpdateData(FALSE);

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////
void CVideoPrerecordDlg::OnDeltaposPrerecordSizeLimitSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	UpdateData(TRUE);

	int nPrerecordSizeLimit = m_dwPrerecordSizeLimit - pNMUpDown->iDelta;

	if (nPrerecordSizeLimit < 0)
		nPrerecordSizeLimit = 0;

	if (nPrerecordSizeLimit > 8192)
		nPrerecordSizeLimit = 8192;

	m_dwPrerecordSizeLimit	= nPrerecordSizeLimit;

	UpdateData(FALSE);

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////
