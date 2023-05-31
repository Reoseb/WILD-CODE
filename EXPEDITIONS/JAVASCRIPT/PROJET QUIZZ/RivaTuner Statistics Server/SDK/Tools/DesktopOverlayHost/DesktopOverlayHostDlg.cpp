// DesktopOverlayHostDlg.cpp : implementation file
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DesktopOverlayHost.h"
#include "DesktopOverlayHostDlg.h"
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}
//////////////////////////////////////////////////////////////////////
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////
// CDesktopOverlayHostDlg dialog
//////////////////////////////////////////////////////////////////////
CDesktopOverlayHostDlg::CDesktopOverlayHostDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDesktopOverlayHostDlg::IDD, pParent)
{
	m_bTransparent				= FALSE;
	m_bColorKey					= FALSE;

	m_dwBgndColor				= 0;
	m_dwAlpha					= 0;

	m_hBrushBgndColorPreview	= NULL;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_TRANSPARENT_CHECK, m_bTransparent);
	DDX_Check(pDX, IDC_USE_COLOR_KEY_CHECK, m_bColorKey);
	DDX_Control(pDX, IDC_ALPHA_SLIDER, m_ctrlAlphaSlider);
}
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDesktopOverlayHostDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_BGND_COLOR_PREVIEW, &CDesktopOverlayHostDlg::OnStnClickedBgndColorPreview)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////
// CDesktopOverlayHostDlg message handlers
//////////////////////////////////////////////////////////////////////
BOOL CDesktopOverlayHostDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_ctrlAlphaSlider.SetRange(0, 255);
	m_ctrlAlphaSlider.SetPos(m_dwAlpha);

	UpdateColorControls();

	return TRUE;  // return TRUE  unless you set the focus to a control
}
//////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
//////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
//////////////////////////////////////////////////////////////////////
HCURSOR CDesktopOverlayHostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_ctrlAlphaSlider)
		m_dwAlpha = m_ctrlAlphaSlider.GetPos();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
//////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostDlg::UpdateColorControls()
{
	if (m_hBrushBgndColorPreview)
		DeleteObject(m_hBrushBgndColorPreview);

	m_hBrushBgndColorPreview = CreateSolidBrush(m_dwBgndColor);

	GetDlgItem(IDC_BGND_COLOR_PREVIEW)->RedrawWindow();
}
//////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostDlg::OnDestroy()
{
	if (m_hBrushBgndColorPreview)
		DeleteObject(m_hBrushBgndColorPreview);

	CDialog::OnDestroy();
}
//////////////////////////////////////////////////////////////////////
HBRUSH CDesktopOverlayHostDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT nID = pWnd->GetDlgCtrlID();

	if (nID == IDC_BGND_COLOR_PREVIEW)
	{
		if (GetDlgItem(IDC_BGND_COLOR_PREVIEW)->IsWindowEnabled())
			return m_hBrushBgndColorPreview;
	}

	return hbr;
}
/////////////////////////////////////////////////////////////////////////////
void CDesktopOverlayHostDlg::OnStnClickedBgndColorPreview()
{
	CColorDialog dlg(m_dwBgndColor, CC_FULLOPEN);

	if (IDOK == dlg.DoModal())
	{
		m_dwBgndColor = dlg.m_cc.rgbResult;

		UpdateColorControls();
	}
}
/////////////////////////////////////////////////////////////////////////////
