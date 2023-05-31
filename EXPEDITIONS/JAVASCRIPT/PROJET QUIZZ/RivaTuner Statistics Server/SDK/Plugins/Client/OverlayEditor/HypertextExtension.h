// HypertextExtension.h: interface for the CHypertextExtension class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
#include "HypertextParser.h"
//////////////////////////////////////////////////////////////////////
#define DEFAULT_GRAPH_WIDTH										-16
#define DEFAULT_GRAPH_HEIGHT									-2
#define DEFAULT_GRAPH_MARGIN									1
#define DEFAULT_GRAPH_MIN										0
#define DEFAULT_GRAPH_MAX										100
#define DEFAULT_GRAPH_FLAGS										0
//////////////////////////////////////////////////////////////////////
#define DEFAULT_ANIMATED_IMAGE_WIDTH							-2
#define DEFAULT_ANIMATED_IMAGE_HEIGHT							-2
#define DEFAULT_ANIMATED_IMAGE_MIN								0
#define DEFAULT_ANIMATED_IMAGE_MAX								100
#define DEFAULT_ANIMATED_IMAGE_SPRITE_X							0
#define DEFAULT_ANIMATED_IMAGE_SPRITE_Y							0
#define DEFAULT_ANIMATED_IMAGE_SPRITE_WIDTH						128
#define DEFAULT_ANIMATED_IMAGE_SPRITE_HEIGHT					128
#define DEFAULT_ANIMATED_IMAGE_SPRITES_NUM						1
#define DEFAULT_ANIMATED_IMAGE_SPRITES_PER_LINE					0
#define DEFAULT_ANIMATED_IMAGE_BIAS								0
/////////////////////////////////////////////////////////////////////////////
#define OBJ_UNKNOWN												-1
#define OBJ_GRAPH												0
#define OBJ_IMAGE												1
#define OBJ_ANIMATED_IMAGE										2
#define OBJ_MACRO												3
#define OBJ_TEXT_TABLE											4
/////////////////////////////////////////////////////////////////////////////
#define	GRAPH_TAG												"<G"
#define GRAPH_TAG_LEN											2
#define	IMAGE_TAG												"<I"
#define IMAGE_TAG_LEN											2
#define	ANIMATED_IMAGE_TAG										"<AI"
#define ANIMATED_IMAGE_TAG_LEN									3
#define FILE_TAG												"<F"
#define FILE_TAG_LEN											2
#define	TEXT_TABLE_TAG											"<TT"
#define TEXT_TABLE_TAG_LEN										3
/////////////////////////////////////////////////////////////////////////////
class COverlay;
class CHypertextExtension : public CHypertextParser
{
public:
	CHypertextExtension();
	virtual ~CHypertextExtension();

	CString	TranslateExtensionString(COverlay* lpOverlay, LPCSTR lpString, BOOL bOptimize, BOOL bEmbed, LPBYTE lpEmbeddedObjectBuffer, DWORD dwEmbeddedObjectBufferSize, DWORD& dwEmbeddedObjectOffset, DWORD dwColor, int nSize, BOOL bContextHighlighting = TRUE);
		// This function perfoms the following things:

		//- translates hypertext extension string and replaces all extension tags with native RTSS hypertext tags
		//- optionally embeds translated objects into the buffer
		//- recognizes embedded object tag in cursor position and saves object type and text boundaries for context highlighting

	LPCSTR	ScanGraphTag(LPCSTR lpsz, CString& strSource, DWORD& dwWidth, DWORD& dwHeight, DWORD& dwMargin, float& fltMin, float& fltMax, DWORD& dwFlags, int& nParams);
		//scans graph hypertext extension tag, returns pointer to the end of tag or NULL if tag is not recogninzed
	LPCSTR	ScanImageTag(LPCSTR lpsz, DWORD& dwWidth, DWORD& dwHeight, DWORD& dwSpriteX, DWORD& dwSpriteY, DWORD& dwSpriteWidth, DWORD& dwSpriteHeight);
		//scans image hypertext extension tag, returns pointer to the end of tag or NULL if tag is not recogninzed
	LPCSTR	ScanAnimatedImageTag(LPCSTR lpsz, CString& strSource, DWORD& dwWidth, DWORD& dwHeight, float& fltMin, float& fltMax, DWORD& dwSpriteX, DWORD& dwSpriteY, DWORD& dwSpriteWidth, DWORD& dwSpriteHeight, DWORD& dwSpritesNum, DWORD& dwSpritesPerLine, float& fltBias, int& nParams);
		//scans animated image hypertext extension tag, returns pointer to the end of tag or NULL if tag is not recogninzed
	LPCSTR	ScanFileTag(LPCSTR lpsz, CString& strFile);
		//scans file hypertext extension tag, returns pointer to the end of tag or NULL if tag is not recogninzed
	LPCSTR	ScanTextTableTag(LPCSTR lpsz, CString& strTable);
		//scans text table hypertext extension tag, returns pointer to the end of tag or NULL if tag is not recogninzed

	virtual CString GetTextFromFile(LPCSTR lpFile);
		//returns the text from external file for file hypertext extension tag

	CString FormatGraphTag(BOOL bCustomTemplate, LPCSTR lpSource, DWORD dwWidth, DWORD dwHeight, DWORD dwMargin, float fltMin, float fltMax, DWORD dwFlags);
		//formats graph hypertext extension tag
	CString	FormatAnimatedImageTag(BOOL bCustomTemplate, LPCSTR lpSource, DWORD dwWidth, DWORD dwHeight, float fltMin, float fltMax, DWORD dwSpriteX, DWORD dwSpriteY, DWORD dwSpriteWidth, DWORD dwSpriteHeight, DWORD dwSpritesNum, DWORD dwSpritesPerLine, float fltBias);
		//formats animated image hypertext extension tag
	CString	FormatImageTag(DWORD dwWidth, DWORD dwHeight, DWORD dwSpriteX, DWORD dwSpriteY, DWORD dwSpriteWidth, DWORD dwSpriteHeight);
		//formats image hypertext extension tag

	void	SetPos(int nPos);
	int		GetPos();	
		//sets/returns cursor position
		
		//NOTE: cursor position is used inside TranslateExtensionString for context highlighting 

	int		GetObj();
	int		GetObjFrom();
	int		GetObjTo();
	int		GetObjParams();
		//returns the last recognized embedded object type and its text boundaries

protected:
	int		m_nPos;

	int		m_nObj;
	int		m_nObjFrom;
	int		m_nObjTo;
	int		m_nObjParams;

	DWORD	m_dwColor;
	int		m_nSize;
};
//////////////////////////////////////////////////////////////////////
