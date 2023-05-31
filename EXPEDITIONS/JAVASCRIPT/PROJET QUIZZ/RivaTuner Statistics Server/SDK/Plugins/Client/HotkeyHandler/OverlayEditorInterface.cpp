// OverlayEditorInterface.cpp: implementation of the COverlayEditorInterface class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "OverlayEditorInterface.h"
//////////////////////////////////////////////////////////////////////
COverlayEditorInterface::COverlayEditorInterface()
{
}
//////////////////////////////////////////////////////////////////////
COverlayEditorInterface::~COverlayEditorInterface()
{
}
//////////////////////////////////////////////////////////////////////
void COverlayEditorInterface::PostOverlayMessage(LPCSTR lpMessage, LPCSTR lpLayer, LPCSTR lpParams)
{
	HMODULE hOverlayEditor	= GetModuleHandle("OverlayEditor.dll");

	if (hOverlayEditor)
	{
		POSTOVERLAYMESSAGE pPostOverlayMessage = (POSTOVERLAYMESSAGE)GetProcAddress(hOverlayEditor, "PostOverlayMessage");

		if (pPostOverlayMessage)
			pPostOverlayMessage(lpMessage, lpLayer, lpParams);
	}
}
//////////////////////////////////////////////////////////////////////
