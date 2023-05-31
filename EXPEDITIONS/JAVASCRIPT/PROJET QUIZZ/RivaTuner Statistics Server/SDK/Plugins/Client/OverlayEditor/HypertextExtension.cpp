// HypertextExtension.cpp: implementation of the CHypertextExtension class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "HypertextExtension.h"
#include "RTSSSharedMemoryInterface.h"
#include "Overlay.h"
#include "OverlayDataSourceInternal.h"

#include <float.h>
//////////////////////////////////////////////////////////////////////
CHypertextExtension::CHypertextExtension()
{
	m_nPos			= -1;
	m_nObj			= OBJ_UNKNOWN;
	m_nObjFrom		= -1;
	m_nObjTo		= -1;
	m_nObjParams	= 0;
} 
//////////////////////////////////////////////////////////////////////
CHypertextExtension::~CHypertextExtension()
{
}
//////////////////////////////////////////////////////////////////////
CString	CHypertextExtension::TranslateExtensionString(COverlay* lpOverlay, LPCSTR lpString, BOOL bOptimize, BOOL bEmbed, LPBYTE lpEmbeddedObjectBuffer, DWORD dwEmbeddedObjectBufferSize, DWORD& dwEmbeddedObjectOffset, DWORD dwColor, int nSize, BOOL bContextHighlighting)
{
	CString strResult;

	DWORD	dwGraphWidth						= DEFAULT_GRAPH_WIDTH;
	DWORD	dwGraphHeight						= DEFAULT_GRAPH_HEIGHT;
	DWORD	dwGraphMargin						= DEFAULT_GRAPH_MARGIN;
	float	fltGraphMin							= DEFAULT_GRAPH_MIN;
	float	fltGraphMax							= DEFAULT_GRAPH_MAX;
	DWORD	dwGraphFlags						= DEFAULT_GRAPH_FLAGS;

	DWORD	dwAnimatedImageWidth				= DEFAULT_ANIMATED_IMAGE_WIDTH;
	DWORD	dwAnimatedImageHeight				= DEFAULT_ANIMATED_IMAGE_HEIGHT;
	float	fltAnimatedImageMin					= DEFAULT_ANIMATED_IMAGE_MIN;
	float	fltAnimatedImageMax					= DEFAULT_ANIMATED_IMAGE_MAX;
	DWORD	dwAnimatedImageSpriteX				= DEFAULT_ANIMATED_IMAGE_SPRITE_X;
	DWORD	dwAnimatedImageSpriteY				= DEFAULT_ANIMATED_IMAGE_SPRITE_Y;
	DWORD	dwAnimatedImageSpriteWidth			= DEFAULT_ANIMATED_IMAGE_SPRITE_WIDTH;
	DWORD	dwAnimatedImageSpriteHeight			= DEFAULT_ANIMATED_IMAGE_SPRITE_HEIGHT;
	DWORD	dwAnimatedImageSpritesNum			= DEFAULT_ANIMATED_IMAGE_SPRITES_NUM;
	DWORD	dwAnimatedImageSpritesPerLine		= DEFAULT_ANIMATED_IMAGE_SPRITES_PER_LINE;
	float	fltAnimatedImageBias				= DEFAULT_ANIMATED_IMAGE_BIAS;

	DWORD	dwImageWidth						= DEFAULT_ANIMATED_IMAGE_WIDTH;
	DWORD	dwImageHeight						= DEFAULT_ANIMATED_IMAGE_HEIGHT;
	DWORD	dwImageSpriteX						= DEFAULT_ANIMATED_IMAGE_SPRITE_X;
	DWORD	dwImageSpriteY						= DEFAULT_ANIMATED_IMAGE_SPRITE_Y;
	DWORD	dwImageSpriteWidth					= DEFAULT_ANIMATED_IMAGE_SPRITE_WIDTH;
	DWORD	dwImageSpriteHeight					= DEFAULT_ANIMATED_IMAGE_SPRITE_HEIGHT;

	LPCSTR	lpStart		= lpString;

	if (bContextHighlighting)
	{
		m_nObj				= OBJ_UNKNOWN;
		m_nObjFrom			= -1;
		m_nObjTo			= -1;
		m_nObjParams		= 0;
	}

	while (*lpString)
	{
		BOOL bAppendChar = FALSE;

		int nObjFrom	= -1;
		int nObjTo		= -1;
		int nObjParams	= 0;

		if (*lpString == '%')
		{
			COverlayMacroList* lpMacroList = lpOverlay->GetMacroList();

			BOOL bMacro = FALSE;

			POSITION pos = lpMacroList->GetHeadPosition();

			while (pos)
			{
				CString strSrc = lpMacroList->GetNext(pos);
				CString strDst = lpMacroList->GetNext(pos);

				int nLen = strlen(strSrc);

				if (!strncmp(lpString, strSrc, nLen))
				{
					nObjFrom	= lpString - lpStart;
					nObjTo		= nObjFrom + nLen;

					if (bContextHighlighting)
					{
						if ((m_nPos > nObjFrom	) &&
							(m_nPos < nObjTo	))
						{
							m_nObj		= OBJ_MACRO;
							m_nObjFrom	= nObjFrom;
							m_nObjTo	= nObjTo;
						}
					}

					strResult	+= strDst;
					lpString	+= nLen;

					bMacro = TRUE;

					break;
				}
			}

			if (!bMacro)
				bAppendChar = TRUE;
		}
		else
		if (*lpString == '<')
			//tags
		{
			if (!strncmp(lpString, TEXT_TABLE_TAG, TEXT_TABLE_TAG_LEN))
			{
				nObjFrom = lpString - lpStart;

				CString strTable;

				LPCSTR lpNext = ScanTextTableTag(lpString + TEXT_TABLE_TAG_LEN, strTable);

				if (lpNext && (*lpNext == '>'))
				{
					nObjTo = lpNext - lpStart + 1;

					if (bContextHighlighting)
					{
						if ((m_nPos > nObjFrom	) &&
							(m_nPos < nObjTo	))
						{
							m_nObj			= OBJ_TEXT_TABLE;
							m_nObjFrom		= nObjFrom;
							m_nObjTo		= nObjTo;
							m_nObjParams	= 1;
						}
					}

					CTextTable* lpTable = lpOverlay->FindTextTable(strTable);

					if (lpTable)
						strResult += TranslateExtensionString(lpOverlay, lpTable->GetHypertext(bOptimize, dwColor, nSize), bOptimize, bEmbed, lpEmbeddedObjectBuffer, dwEmbeddedObjectBufferSize, dwEmbeddedObjectOffset, dwColor, nSize, FALSE);

					lpString = lpNext + 1;
				}
				else
					bAppendChar = TRUE;

			}
			else
			if (!strncmp(lpString, FILE_TAG, FILE_TAG_LEN))
			{
				CString strFile;

				LPCSTR lpNext = ScanFileTag(lpString + FILE_TAG_LEN, strFile);

				if (lpNext && (*lpNext == '>'))
				{
					strResult += TranslateExtensionString(lpOverlay, GetTextFromFile(strFile), bOptimize, bEmbed, lpEmbeddedObjectBuffer, dwEmbeddedObjectBufferSize, dwEmbeddedObjectOffset, dwColor, nSize, FALSE);

					lpString = lpNext + 1;
				}
				else
					bAppendChar = TRUE;

			}
			else
			if (!strncmp(lpString, IMAGE_TAG, IMAGE_TAG_LEN))
			{
				nObjFrom = lpString - lpStart;

				CString strSource;

				LPCSTR lpNext = ScanImageTag(lpString + IMAGE_TAG_LEN, dwImageWidth, dwImageHeight, dwImageSpriteX, dwImageSpriteY, dwImageSpriteWidth, dwImageSpriteHeight);

				if (lpNext && (*lpNext == '>'))
				{
					nObjTo = lpNext - lpStart + 1;

					if (bContextHighlighting)
					{
						if ((m_nPos > nObjFrom	) &&
							(m_nPos < nObjTo	))
						{
							m_nObj			= OBJ_IMAGE;
							m_nObjFrom		= nObjFrom;
							m_nObjTo		= nObjTo;
							m_nObjParams	= nObjParams;
						}
					}
				}

				bAppendChar = TRUE;
			}
			else
			if (!strncmp(lpString, ANIMATED_IMAGE_TAG, ANIMATED_IMAGE_TAG_LEN))
			{
				nObjFrom = lpString - lpStart;

				CString strSource;

				LPCSTR lpNext = ScanAnimatedImageTag(lpString + ANIMATED_IMAGE_TAG_LEN, strSource, dwAnimatedImageWidth, dwAnimatedImageHeight, fltAnimatedImageMin, fltAnimatedImageMax, dwAnimatedImageSpriteX, dwAnimatedImageSpriteY, dwAnimatedImageSpriteWidth, dwAnimatedImageSpriteHeight, dwAnimatedImageSpritesNum, dwAnimatedImageSpritesPerLine, fltAnimatedImageBias, nObjParams);

				if (lpNext && (*lpNext == '>'))
				{
					nObjTo = lpNext - lpStart + 1;

					if (bContextHighlighting)
					{
						if ((m_nPos > nObjFrom	) &&
							(m_nPos < nObjTo	))
						{
							m_nObj			= OBJ_ANIMATED_IMAGE;
							m_nObjFrom		= nObjFrom;
							m_nObjTo		= nObjTo;
							m_nObjParams	= nObjParams;
						}
					}

					COverlayDataSource* lpSource = lpOverlay->FindDataSource(strSource);

					if (lpSource)
					{
						FLOAT fltData			= lpSource->GetData();
						DWORD dwSprite			= 0;
						DWORD dwSpriteIndexX	= 0;
						DWORD dwSpriteIndexY	= 0;

						if ((fltData != FLT_MAX) && (fltAnimatedImageMax > fltAnimatedImageMin))
						{
							if (fltData > fltAnimatedImageMax)
								fltData = fltAnimatedImageMax;

							if (fltData < fltAnimatedImageMin)
								fltData = fltAnimatedImageMin;

							dwSprite = (DWORD)(dwAnimatedImageSpritesNum * (fltData - fltAnimatedImageMin) / (fltAnimatedImageMax - fltAnimatedImageMin) + fltAnimatedImageBias);

							if ((LONG)dwSprite < 0)
								dwSprite = 0;

							if (dwSprite > dwAnimatedImageSpritesNum - 1)
								dwSprite = dwAnimatedImageSpritesNum - 1;
						}
						
						if (!dwAnimatedImageSpritesPerLine)
							dwAnimatedImageSpritesPerLine = dwAnimatedImageSpritesNum;

						if (dwAnimatedImageSpritesNum)
						{
							dwSpriteIndexX	= dwSprite % dwAnimatedImageSpritesPerLine;
							dwSpriteIndexY	= dwSprite / dwAnimatedImageSpritesPerLine;
						}

						strResult += FormatImageTag(dwAnimatedImageWidth, dwAnimatedImageHeight, dwAnimatedImageSpriteX + dwSpriteIndexX * dwAnimatedImageSpriteWidth, dwAnimatedImageSpriteY + dwSpriteIndexY * dwAnimatedImageSpriteHeight, dwAnimatedImageSpriteWidth, dwAnimatedImageSpriteHeight);
					}

					lpString = lpNext + 1;
				}
				else
					bAppendChar = TRUE;
			}
			else
			if (!strncmp(lpString, GRAPH_TAG, GRAPH_TAG_LEN))
			{
				nObjFrom = lpString - lpStart;

				CString strSource;

				LPCSTR lpNext = ScanGraphTag(lpString + GRAPH_TAG_LEN, strSource, dwGraphWidth, dwGraphHeight, dwGraphMargin, fltGraphMin, fltGraphMax, dwGraphFlags, nObjParams);

				if (lpNext && (*lpNext == '>'))
				{
					nObjTo = lpNext - lpStart + 1;

					if (bContextHighlighting)
					{
						if ((m_nPos > nObjFrom	) &&
							(m_nPos < nObjTo	))
						{
							m_nObj			= OBJ_GRAPH;
							m_nObjFrom		= nObjFrom;
							m_nObjTo		= nObjTo;
							m_nObjParams	= nObjParams;
						}
					}

					BOOL bFramerate					= !_stricmp(strSource, INTERNAL_SOURCE_FRAMERATE);
					BOOL bFrametime					= !_stricmp(strSource, INTERNAL_SOURCE_FRAMETIME);

					BOOL bFramerateMin				= !_stricmp(strSource, INTERNAL_SOURCE_FRAMERATE_MIN);
					BOOL bFramerateAvg				= !_stricmp(strSource, INTERNAL_SOURCE_FRAMERATE_AVG);
					BOOL bFramerateMax				= !_stricmp(strSource, INTERNAL_SOURCE_FRAMERATE_MAX);
					BOOL bFramerate1Dot0PercentLow	= !_stricmp(strSource, INTERNAL_SOURCE_FRAMERATE_1DOT0_PERCENT_LOW);
					BOOL bFramerate0Dot1PercentLow	= !_stricmp(strSource, INTERNAL_SOURCE_FRAMERATE_0DOT1_PERCENT_LOW);

					if (bFramerate || bFrametime || bFramerateMin || bFramerateAvg || bFramerateMax || bFramerate1Dot0PercentLow || bFramerate0Dot1PercentLow)
					{
						CString strObj;
						if (bOptimize)
							strObj.Format("<OBJ>", dwEmbeddedObjectOffset);
						else
							strObj.Format("<OBJ=%X>", dwEmbeddedObjectOffset);

						strResult += strObj;

						if (bEmbed)
						{
							CRTSSSharedMemoryInterface sharedMemoryInterface;
							if (bFramerate)
								dwEmbeddedObjectOffset += sharedMemoryInterface.EmbedGraphInObjBuffer(lpEmbeddedObjectBuffer, dwEmbeddedObjectBufferSize, dwEmbeddedObjectOffset, NULL, 0, 0, dwGraphWidth, dwGraphHeight, dwGraphMargin, fltGraphMin, fltGraphMax, dwGraphFlags | RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMERATE); 
							else
							if (bFrametime)
								dwEmbeddedObjectOffset += sharedMemoryInterface.EmbedGraphInObjBuffer(lpEmbeddedObjectBuffer, dwEmbeddedObjectBufferSize, dwEmbeddedObjectOffset, NULL, 0, 0, dwGraphWidth, dwGraphHeight, dwGraphMargin, fltGraphMin * 1000, fltGraphMax * 1000, dwGraphFlags | RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMETIME); 
							else
							if (bFramerateMin)
								dwEmbeddedObjectOffset += sharedMemoryInterface.EmbedGraphInObjBuffer(lpEmbeddedObjectBuffer, dwEmbeddedObjectBufferSize, dwEmbeddedObjectOffset, NULL, 0, 0, dwGraphWidth, dwGraphHeight, dwGraphMargin, fltGraphMin, fltGraphMax, dwGraphFlags | RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMERATE_MIN); 
							else
							if (bFramerateAvg)
								dwEmbeddedObjectOffset += sharedMemoryInterface.EmbedGraphInObjBuffer(lpEmbeddedObjectBuffer, dwEmbeddedObjectBufferSize, dwEmbeddedObjectOffset, NULL, 0, 0, dwGraphWidth, dwGraphHeight, dwGraphMargin, fltGraphMin, fltGraphMax, dwGraphFlags | RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMERATE_AVG); 
							else
							if (bFramerateMax)
								dwEmbeddedObjectOffset += sharedMemoryInterface.EmbedGraphInObjBuffer(lpEmbeddedObjectBuffer, dwEmbeddedObjectBufferSize, dwEmbeddedObjectOffset, NULL, 0, 0, dwGraphWidth, dwGraphHeight, dwGraphMargin, fltGraphMin, fltGraphMax, dwGraphFlags | RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMERATE_MAX); 
							else
							if (bFramerate1Dot0PercentLow)
								dwEmbeddedObjectOffset += sharedMemoryInterface.EmbedGraphInObjBuffer(lpEmbeddedObjectBuffer, dwEmbeddedObjectBufferSize, dwEmbeddedObjectOffset, NULL, 0, 0, dwGraphWidth, dwGraphHeight, dwGraphMargin, fltGraphMin, fltGraphMax, dwGraphFlags | RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMERATE_1DOT0_PERCENT_LOW); 
							else
							if (bFramerate0Dot1PercentLow)
								dwEmbeddedObjectOffset += sharedMemoryInterface.EmbedGraphInObjBuffer(lpEmbeddedObjectBuffer, dwEmbeddedObjectBufferSize, dwEmbeddedObjectOffset, NULL, 0, 0, dwGraphWidth, dwGraphHeight, dwGraphMargin, fltGraphMin, fltGraphMax, dwGraphFlags | RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FRAMERATE_0DOT1_PERCENT_LOW); 
						}

					}
					else
					{
						COverlayDataSource* lpSource = lpOverlay->FindDataSource(strSource);

						if (lpSource)
						{
							CString strObj;
							if (bOptimize)
								strObj.Format("<OBJ>", dwEmbeddedObjectOffset);
							else
								strObj.Format("<OBJ=%X>", dwEmbeddedObjectOffset);

							strResult += strObj;

							if (bEmbed)
							{
								CRTSSSharedMemoryInterface sharedMemoryInterface;
								dwEmbeddedObjectOffset += sharedMemoryInterface.EmbedGraphInObjBuffer(lpEmbeddedObjectBuffer, dwEmbeddedObjectBufferSize, dwEmbeddedObjectOffset, lpSource->GetBuffer(), lpSource->GetBufferPos(), RING_BUFFER_SIZE, dwGraphWidth, dwGraphHeight, dwGraphMargin, fltGraphMin, fltGraphMax, dwGraphFlags); 
							}
						}
					}

					lpString = lpNext + 1;
				}
				else
					bAppendChar = TRUE;
			}
			else
				bAppendChar = TRUE;
		}
		else
			bAppendChar = TRUE;

		if (bAppendChar)
			strResult += *lpString++;
	}

	return strResult;
}
/////////////////////////////////////////////////////////////////////////////
LPCSTR CHypertextExtension::ScanFileTag(LPCSTR lpsz, CString& strFile)
{
	if (*lpsz != '=')
		return NULL;

	return ScanStr(lpsz + 1, strFile);
}
/////////////////////////////////////////////////////////////////////////////
LPCSTR CHypertextExtension::ScanTextTableTag(LPCSTR lpsz, CString& strTable)
{
	if (*lpsz != '=')
		return NULL;

	return ScanStr(lpsz + 1, strTable);
}
/////////////////////////////////////////////////////////////////////////////
LPCSTR CHypertextExtension::ScanGraphTag(LPCSTR lpsz, CString& strSource, DWORD& dwWidth, DWORD& dwHeight, DWORD& dwMargin, float& fltMin, float& fltMax, DWORD& dwFlags, int& nParams)
{
	if (*lpsz != '=')
		return NULL;

	nParams = 0;

	lpsz = ScanStr(lpsz + 1, strSource);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 1;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwWidth);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 2;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwHeight);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 3;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwMargin);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 4;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanFlt(lpsz + 1, fltMin);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 5;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanFlt(lpsz + 1, fltMax);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 6;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	nParams = 7;

	return ScanHex(lpsz + 1, dwFlags);
}
/////////////////////////////////////////////////////////////////////////////
LPCSTR CHypertextExtension::ScanImageTag(LPCSTR lpsz, DWORD& dwWidth, DWORD& dwHeight, DWORD& dwSpriteX, DWORD& dwSpriteY, DWORD& dwSpriteWidth, DWORD& dwSpriteHeight)
{
	if (*lpsz != '=')
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwWidth);
	if (!lpsz || (*lpsz != ','))
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwHeight);
	if (!lpsz || (*lpsz != ','))
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwSpriteX);
	if (!lpsz || (*lpsz != ','))
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwSpriteY);
	if (!lpsz || (*lpsz != ','))
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwSpriteWidth);
	if (!lpsz || (*lpsz != ','))
		return NULL;

	return ScanDec(lpsz + 1, dwSpriteHeight);
}
/////////////////////////////////////////////////////////////////////////////
LPCSTR	CHypertextExtension::ScanAnimatedImageTag(LPCSTR lpsz, CString& strSource, DWORD& dwWidth, DWORD& dwHeight, float& fltMin, float& fltMax, DWORD& dwSpriteX, DWORD& dwSpriteY, DWORD& dwSpriteWidth, DWORD& dwSpriteHeight, DWORD& dwSpritesNum, DWORD& dwSpritesPerLine, float& fltBias, int& nParams)
{
	if (*lpsz != '=')
		return NULL;

	nParams = 0;

	lpsz = ScanStr(lpsz + 1, strSource);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 1;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwWidth);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 2;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwHeight);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 3;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanFlt(lpsz + 1, fltMin);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 4;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanFlt(lpsz + 1, fltMax);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 5;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwSpriteX);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 6;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwSpriteY);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 7;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwSpriteWidth);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 8;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwSpriteHeight);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 9;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwSpritesNum);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 10;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	lpsz = ScanDec(lpsz + 1, dwSpritesPerLine);
	if (!lpsz)
		return NULL;
	if (*lpsz == '>')
	{
		nParams = 11;
		return lpsz;
	}
	if (*lpsz != ',')
		return NULL;

	nParams = 12;

	return ScanFlt(lpsz + 1, fltBias);
}
/////////////////////////////////////////////////////////////////////////////
void CHypertextExtension::SetPos(int nPos)
{
	m_nPos = nPos;
}
/////////////////////////////////////////////////////////////////////////////
int CHypertextExtension::GetPos()
{
	return m_nPos;
}
/////////////////////////////////////////////////////////////////////////////
int CHypertextExtension::GetObj()
{
	return m_nObj;
}
/////////////////////////////////////////////////////////////////////////////
int CHypertextExtension::GetObjFrom()
{
	return m_nObjFrom;
}
/////////////////////////////////////////////////////////////////////////////
int CHypertextExtension::GetObjTo()
{
	return m_nObjTo;
}
/////////////////////////////////////////////////////////////////////////////
int CHypertextExtension::GetObjParams()
{
	return m_nObjParams;
}
/////////////////////////////////////////////////////////////////////////////
CString CHypertextExtension::FormatGraphTag(BOOL bCustomTemplate, LPCSTR lpSource, DWORD dwWidth, DWORD dwHeight, DWORD dwMargin, float fltMin, float fltMax, DWORD dwFlags)
{
	CString strTag;
	if (bCustomTemplate)
	{
		CString strMin = COverlayDataSource::FormatFloat(fltMin);
		CString strMax = COverlayDataSource::FormatFloat(fltMax);

		strTag.Format("<G=%s,%d,%d,%d,%s,%s,%X>", lpSource, dwWidth, dwHeight, dwMargin, strMin, strMax, dwFlags);
	}
	else
		strTag.Format("<G=%s>", lpSource);

	return strTag;
}
/////////////////////////////////////////////////////////////////////////////
CString	CHypertextExtension::FormatImageTag(DWORD dwWidth, DWORD dwHeight, DWORD dwSpriteX, DWORD dwSpriteY, DWORD dwSpriteWidth, DWORD dwSpriteHeight)
{
	CString strTag;
	strTag.Format("<I=%d,%d,%d,%d,%d,%d>", dwWidth, dwHeight, dwSpriteX, dwSpriteY, dwSpriteWidth, dwSpriteHeight);

	return strTag;
}
/////////////////////////////////////////////////////////////////////////////
CString	CHypertextExtension::FormatAnimatedImageTag(BOOL bCustomTemplate, LPCSTR lpSource, DWORD dwWidth, DWORD dwHeight, float fltMin, float fltMax, DWORD dwSpriteX, DWORD dwSpriteY, DWORD dwSpriteWidth, DWORD dwSpriteHeight, DWORD dwSpritesNum, DWORD dwSpritesPerLine, float fltBias)
{
	CString strTag;
	if (bCustomTemplate)
	{
		CString strMin	= COverlayDataSource::FormatFloat(fltMin);
		CString strMax	= COverlayDataSource::FormatFloat(fltMax);
		CString strBias	= COverlayDataSource::FormatFloat(fltBias);

		strTag.Format("<AI=%s,%d,%d,%s,%s,%d,%d,%d,%d,%d,%d,%s>", lpSource, dwWidth, dwHeight, strMin, strMax, dwSpriteX, dwSpriteY, dwSpriteWidth, dwSpriteHeight, dwSpritesNum, dwSpritesPerLine, strBias);
	}
	else
		strTag.Format("<AI=%s>", lpSource);

	return strTag;

}
/////////////////////////////////////////////////////////////////////////////
CString CHypertextExtension::GetTextFromFile(LPCSTR lpFile)
{
	return "";
}
/////////////////////////////////////////////////////////////////////////////
