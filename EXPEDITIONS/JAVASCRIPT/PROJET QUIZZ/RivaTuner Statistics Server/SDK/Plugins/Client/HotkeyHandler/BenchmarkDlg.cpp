// BenchmarkDlg.cpp: implementation of the CBenchmarkDlg class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HotkeyHandler.h"
#include "BenchmarkDlg.h"
//////////////////////////////////////////////////////////////////////
// CBenchmarkDlg dialog
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CBenchmarkDlg, CDialog)
//////////////////////////////////////////////////////////////////////
CBenchmarkDlg::CBenchmarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBenchmarkDlg::IDD, pParent)
{
	m_strBenchmarkPath	= "";
	m_bRecreateResults	= TRUE;
}
//////////////////////////////////////////////////////////////////////
CBenchmarkDlg::~CBenchmarkDlg()
{
}
//////////////////////////////////////////////////////////////////////
void CBenchmarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BENCHMARK_PATH_EDIT, m_strBenchmarkPath);
	DDX_Check(pDX, IDC_RECREATE_BENCHMARK_CHECK, m_bRecreateResults);
}
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CBenchmarkDlg, CDialog)
	ON_BN_CLICKED(IDC_BROWSE_BUTTON, &CBenchmarkDlg::OnBnClickedBrowseButton)
	ON_BN_CLICKED(IDC_VIEW_BUTTON, &CBenchmarkDlg::OnBnClickedViewButton)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////
// CBenchmarkDlg message handlers
//////////////////////////////////////////////////////////////////////
void CBenchmarkDlg::OnBnClickedBrowseButton()
{
	CFileDialog dlg(TRUE, "txt", NULL, OFN_HIDEREADONLY, LocalizeStr("Text files (*.txt)|*.txt|All files (*.*)|*.*||"), this);

	if (dlg.DoModal() == IDOK)
	{
		m_strBenchmarkPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}
//////////////////////////////////////////////////////////////////////
void CBenchmarkDlg::OnBnClickedViewButton()
{
	ShellExecute(GetSafeHwnd(), "open", m_strBenchmarkPath, NULL, NULL, SW_SHOWNORMAL );
}
//////////////////////////////////////////////////////////////////////
BOOL CBenchmarkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LocalizeWnd(m_hWnd);
	AdjustWindowPos(this, GetParent());

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
