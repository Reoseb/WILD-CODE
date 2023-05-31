// HypertextEditCtrl.h: interface for the CHypertextEditCtrl class.
//
// created by Unwinder
/////////////////////////////////////////////////////////////////////////////
#ifndef _HYPERTEXTEDITCTRL_H_INCLUDED_
#define _HYPERTEXTEDITCTRL_H_INCLUDED_
/////////////////////////////////////////////////////////////////////////////
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
/////////////////////////////////////////////////////////////////////////////
#include "HypertextExtension.h"
/////////////////////////////////////////////////////////////////////////////
// CHypertextEditCtrl window
/////////////////////////////////////////////////////////////////////////////
const UINT UM_EDIT_EMBEDDED_OBJECT	= ::RegisterWindowMessage("UM_EDIT_EMBEDDED_OBJECT");
/////////////////////////////////////////////////////////////////////////////
class CHypertextEditCtrl : public CRichEditCtrl, public CHypertextExtension
{
// Construction
public:
	CHypertextEditCtrl();
	virtual ~CHypertextEditCtrl();
	
	void SetOverlay(COverlay* lpOverlay);
		//initialization

	void SetWindowTextEx(LPCSTR lpText);
		//sets text and updates context highlighting for cursor position

	void UpdateContextHighlighting();
		//updates context highlighting for cursor position

	//graph template properties, can be accessed when cursor is on graph tag

	CString	GetGraphSource();
	DWORD	GetGraphWidth();
	DWORD	GetGraphHeight();
	DWORD	GetGraphMargin();
	float	GetGraphMin();
	float	GetGraphMax();
	DWORD	GetGraphFlags();
	int		GetGraphParams();

	//image template properties, can be accessed when cursor is on image tag

	CString	GetImageSource();
	DWORD	GetImageWidth();
	DWORD	GetImageHeight();
	float	GetImageMin();
	float	GetImageMax();
	DWORD	GetImageSpriteX();
	DWORD	GetImageSpriteY();
	DWORD	GetImageSpriteWidth();
	DWORD	GetImageSpriteHeight();
	DWORD	GetImageSpritesNum();
	DWORD	GetImageSpritesPerLine();
	float	GetImageBias();
	int		GetImageParams();

	//text table properties, can be accessed when curson is on text table tag

	CString	GetTextTable();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHypertextEditCtrl)
	//}}AFX_VIRTUAL

protected:
	COverlay*	m_lpOverlay;

	CString		m_strGraphSource;
	DWORD		m_dwGraphWidth;
	DWORD		m_dwGraphHeight;
	DWORD		m_dwGraphMargin;
	float		m_fltGraphMin;
	float		m_fltGraphMax;
	DWORD		m_dwGraphFlags;
	int			m_nGraphParams;

	CString		m_strImageSource;
	DWORD		m_dwImageWidth;
	DWORD		m_dwImageHeight;
	float		m_fltImageMin;
	float		m_fltImageMax;
	DWORD		m_dwImageSpriteX;
	DWORD		m_dwImageSpriteY;
	DWORD		m_dwImageSpriteWidth;
	DWORD		m_dwImageSpriteHeight;
	DWORD		m_dwImageSpritesNum;
	DWORD		m_dwImageSpritesPerLine;
	float		m_fltImageBias;
	int			m_nImageParams;

	CString		m_strTextTable;

	//{{AFX_MSG(CHypertextEditCtrl)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////
