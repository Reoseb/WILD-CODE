// BenchmarkDlg.h: interface for the CBenchmarkDlg class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
#include "resource.h"
//////////////////////////////////////////////////////////////////////
// CBenchmarkDlg dialog
//////////////////////////////////////////////////////////////////////
class CBenchmarkDlg : public CDialog
{
	DECLARE_DYNAMIC(CBenchmarkDlg)

public:
	CBenchmarkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBenchmarkDlg();

	CString m_strBenchmarkPath;
	BOOL	m_bRecreateResults;

// Dialog Data
	enum { IDD = IDD_BENCHMARK_SETTINGS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBrowseButton();
	afx_msg void OnBnClickedViewButton();
};
//////////////////////////////////////////////////////////////////////
