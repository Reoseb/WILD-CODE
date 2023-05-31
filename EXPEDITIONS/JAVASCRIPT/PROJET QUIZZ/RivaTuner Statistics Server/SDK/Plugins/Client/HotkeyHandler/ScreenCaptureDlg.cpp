// ScreenCaptureDlg.cpp: implementation of the CScreenCaptureDlg class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HotkeyHandler.h"
#include "ScreenCaptureDlg.h"
//////////////////////////////////////////////////////////////////////
// CScreenCaptureDlg dialog
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CScreenCaptureDlg, CDialog)
//////////////////////////////////////////////////////////////////////
CScreenCaptureDlg::CScreenCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenCaptureDlg::IDD, pParent)
{
	m_nScreenCaptureFormat		= FORMAT_PNG;
	m_strScreenCaptureFormat	= "png";
	m_dwScreenCaptureQuality	= 100;
	m_strScreenCaptureFolder	= "";
	m_bCaptureOSD				= TRUE;
}
//////////////////////////////////////////////////////////////////////
CScreenCaptureDlg::~CScreenCaptureDlg()
{
}
//////////////////////////////////////////////////////////////////////
void CScreenCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_BMP_RADIO, m_nScreenCaptureFormat);
	DDX_Control(pDX, IDC_SCREEN_CAPTURE_QUALITY_SLIDER, m_ctrlScreenCaptureQualitySlider);
	DDX_Text(pDX, IDC_SCREEN_CAPTURE_FOLDER_EDIT, m_strScreenCaptureFolder);
	DDX_Check(pDX, IDC_CAPTURE_OSD_CHECK, m_bCaptureOSD);
}
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CScreenCaptureDlg, CDialog)
	ON_BN_CLICKED(IDC_BMP_RADIO, &CScreenCaptureDlg::OnBnClickedBmpRadio)
	ON_BN_CLICKED(IDC_JPG_RADIO, &CScreenCaptureDlg::OnBnClickedJpgRadio)
	ON_BN_CLICKED(IDC_PNG_RADIO, &CScreenCaptureDlg::OnBnClickedPngRadio)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BROWSE_BUTTON, &CScreenCaptureDlg::OnBnClickedBrowseButton)
	ON_BN_CLICKED(IDC_VIEW_BUTTON, &CScreenCaptureDlg::OnBnClickedViewButton)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////
BOOL CScreenCaptureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LocalizeWnd(m_hWnd);
	AdjustWindowPos(this, GetParent());

	m_nScreenCaptureFormat = StrToFormat(m_strScreenCaptureFormat);

	UpdateData(FALSE);

	m_ctrlScreenCaptureQualitySlider.SetRange(25, 100);
	m_ctrlScreenCaptureQualitySlider.SetPos(m_dwScreenCaptureQuality);

	UpdateScreenCaptureQualityControls();

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
void CScreenCaptureDlg::UpdateScreenCaptureQualityControls()
{
	CString strCaption;
	strCaption.Format(LocalizeStr("%d%% quality"), m_dwScreenCaptureQuality);

	GetDlgItem(IDC_SCREENSHOT_QUALITY_CAPTION)->SetWindowText(strCaption);

	UpdateData(TRUE);

	m_ctrlScreenCaptureQualitySlider.EnableWindow(m_nScreenCaptureFormat == FORMAT_JPG);
}
//////////////////////////////////////////////////////////////////////
void CScreenCaptureDlg::OnBnClickedBmpRadio()
{
	UpdateData(TRUE);

	m_strScreenCaptureFormat = FormatToStr(m_nScreenCaptureFormat);
	
	UpdateScreenCaptureQualityControls();
}
//////////////////////////////////////////////////////////////////////
void CScreenCaptureDlg::OnBnClickedJpgRadio()
{
	UpdateData(TRUE);

	m_strScreenCaptureFormat = FormatToStr(m_nScreenCaptureFormat);

	UpdateScreenCaptureQualityControls();
}
//////////////////////////////////////////////////////////////////////
void CScreenCaptureDlg::OnBnClickedPngRadio()
{
	UpdateData(TRUE);

	m_strScreenCaptureFormat = FormatToStr(m_nScreenCaptureFormat);

	UpdateScreenCaptureQualityControls();
}
//////////////////////////////////////////////////////////////////////
void CScreenCaptureDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_ctrlScreenCaptureQualitySlider)
	{
		m_dwScreenCaptureQuality = m_ctrlScreenCaptureQualitySlider.GetPos();

		UpdateScreenCaptureQualityControls();
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
//////////////////////////////////////////////////////////////////////
void CScreenCaptureDlg::OnBnClickedBrowseButton()
{
	char szBuf[MAX_PATH];

	CString strTitle = LocalizeStr("Please select target folder for screen capture"); 

	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));

	bi.hwndOwner		= m_hWnd;
	bi.pszDisplayName	= szBuf;
	bi.lpszTitle		= strTitle;

	ITEMIDLIST* piid = SHBrowseForFolder(&bi);

	if (piid && SHGetPathFromIDList(piid, szBuf))
	{
		m_strScreenCaptureFolder = szBuf;
		UpdateData(FALSE);
	}
	
	if (piid)
	{
		LPMALLOC lpMalloc;
		VERIFY(::SHGetMalloc(&lpMalloc) == NOERROR);
		lpMalloc->Free(piid);
		lpMalloc->Release();
	}
}
//////////////////////////////////////////////////////////////////////
void CScreenCaptureDlg::OnBnClickedViewButton()
{
	CreateDirectory(m_strScreenCaptureFolder);

	ShellExecute(GetSafeHwnd(), "open", m_strScreenCaptureFolder, NULL, NULL, SW_SHOWNORMAL);
}
/////////////////////////////////////////////////////////////////////////////
DWORD CScreenCaptureDlg::StrToFormat(LPCSTR lpFormat)
{
	if (!_stricmp(lpFormat, "bmp"))
		return FORMAT_BMP;
	if (!_stricmp(lpFormat, "png"))
		return FORMAT_PNG;
	if (!_stricmp(lpFormat, "jpg"))
		return FORMAT_JPG;

	return FORMAT_BMP;
}
/////////////////////////////////////////////////////////////////////////////
CString CScreenCaptureDlg::FormatToStr(DWORD dwFormat)
{
	switch (dwFormat)
	{
	case FORMAT_BMP:
		return "bmp";
	case FORMAT_PNG:
		return "png";
	case FORMAT_JPG:
		return "jpg";
	}

	return "bmp";
}
/////////////////////////////////////////////////////////////////////////////
