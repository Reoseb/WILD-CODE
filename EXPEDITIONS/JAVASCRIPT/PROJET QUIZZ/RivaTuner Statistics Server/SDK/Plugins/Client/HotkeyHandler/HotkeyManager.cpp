// HotkeyManager.cpp: implementation of the CHotkeyManager class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HotkeyManager.h"
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHotkeyManager::CHotkeyManager()
{
}
//////////////////////////////////////////////////////////////////////
CHotkeyManager::~CHotkeyManager()
{
	Destroy();
}
//////////////////////////////////////////////////////////////////////
void CHotkeyManager::SetHotkey(DWORD dwHotkeyID, DWORD dwHotkey)
{
	LPHOTKEY_DESC pDesc = FindByID(dwHotkeyID);

	if (pDesc)
	{
		pDesc->dwHotkey		= dwHotkey;
		pDesc->dwHotkey2	= GetAlternateHotkey(dwHotkey);
	}
	else
	{
		pDesc				= new HOTKEY_DESC;

		pDesc->dwHotkeyID	= dwHotkeyID;
		pDesc->dwHotkey		= dwHotkey;
		pDesc->dwHotkey2	= GetAlternateHotkey(dwHotkey);

		AddTail(pDesc);
	}
}
//////////////////////////////////////////////////////////////////////
LPHOTKEY_DESC CHotkeyManager::FindByID(DWORD dwHotkeyID)
{
	POSITION pos = GetHeadPosition();

	while (pos)
	{
		LPHOTKEY_DESC pDesc = GetNext(pos);

		if (pDesc->dwHotkeyID == dwHotkeyID)
			return pDesc;
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////
DWORD CHotkeyManager::GetAlternateHotkey(DWORD dwHotkey)
{
	DWORD dwAlternateHotkey = 0;

	switch (dwHotkey & 0x8ffff)
	{
		//numpad OFF to numpad ON 

	case VK_END:
		dwAlternateHotkey = VK_NUMPAD1;
		break;
	case VK_DOWN:
		dwAlternateHotkey = VK_NUMPAD2;
		break;
	case VK_NEXT:
		dwAlternateHotkey = VK_NUMPAD3;
		break;
	case VK_LEFT:
		dwAlternateHotkey = VK_NUMPAD4;
		break;
	case VK_CLEAR:
		dwAlternateHotkey = VK_NUMPAD5;
		break;
	case VK_RIGHT:
		dwAlternateHotkey = VK_NUMPAD6;
		break;
	case VK_HOME:
		dwAlternateHotkey = VK_NUMPAD7;
		break;
	case VK_UP:
		dwAlternateHotkey = VK_NUMPAD8;
		break;
	case VK_PRIOR:
		dwAlternateHotkey = VK_NUMPAD9;
		break;
	case VK_INSERT:
		dwAlternateHotkey = VK_NUMPAD0;
		break;
	case VK_DELETE:
		dwAlternateHotkey = VK_DECIMAL;
		break;

	//numpad ON to numpad OFF 

	case VK_NUMPAD1:
		dwAlternateHotkey = VK_END;
		break;
	case VK_NUMPAD2:
		dwAlternateHotkey = VK_DOWN;
		break;
	case VK_NUMPAD3:
		dwAlternateHotkey = VK_NEXT;
		break;
	case VK_NUMPAD4:
		dwAlternateHotkey = VK_LEFT;
		break;
	case VK_NUMPAD5:
		dwAlternateHotkey = VK_CLEAR;
		break;
	case VK_NUMPAD6:
		dwAlternateHotkey = VK_RIGHT;
		break;
	case VK_NUMPAD7:
		dwAlternateHotkey = VK_HOME;
		break;
	case VK_NUMPAD8:
		dwAlternateHotkey = VK_UP;
		break;
	case VK_NUMPAD9:
		dwAlternateHotkey = VK_PRIOR;
		break;
	case VK_NUMPAD0:
		dwAlternateHotkey = VK_INSERT;
		break;
	case VK_DECIMAL:
		dwAlternateHotkey = VK_DELETE;
		break;
	}

	if (dwAlternateHotkey)
		return (dwHotkey & 0xfff70000) | dwAlternateHotkey;

	return dwHotkey;
}
//////////////////////////////////////////////////////////////////////
LPHOTKEY_DESC CHotkeyManager::FindByHotkey(DWORD dwHotkey)
{
	POSITION pos = GetHeadPosition();

	while (pos)
	{
		LPHOTKEY_DESC pDesc = GetNext(pos);

		if ((pDesc->dwHotkey == dwHotkey) || (pDesc->dwHotkey2 == dwHotkey))
			return pDesc;
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////
void CHotkeyManager::Destroy()
{
	POSITION pos = GetHeadPosition();

	while (pos)
	{
		LPHOTKEY_DESC pDesc = GetNext(pos);

		delete pDesc;
	}

	RemoveAll();
}
//////////////////////////////////////////////////////////////////////
DWORD CHotkeyManager::GetHotkey(DWORD dwHotkeyID)
{
	LPHOTKEY_DESC pDesc = FindByID(dwHotkeyID);

	if (pDesc)
		return pDesc->dwHotkey;

	return 0;
}
//////////////////////////////////////////////////////////////////////
CString CHotkeyManager::GetHotkeyStr(DWORD dwHotkey)
{
	CString strHotkey;

	WORD vk		= dwHotkey & 0xFFFF;
	WORD mod	= (dwHotkey>>16) & 0xFFFF;
	WORD scan	= (WORD)MapVirtualKey(vk, 0);

	char szBuf[MAX_PATH];

	if (mod & HOTKEYF_CONTROL)
	{
		scan = (WORD)MapVirtualKey(VK_CONTROL, 0);
		GetKeyNameText((scan<<16), szBuf, MAX_PATH);
		strHotkey += szBuf;
		strHotkey += " + ";
	}

	if (mod & HOTKEYF_SHIFT)
	{
		scan = (WORD)MapVirtualKey(VK_SHIFT, 0);
		GetKeyNameText((scan<<16), szBuf, MAX_PATH);
		strHotkey += szBuf;
		strHotkey += " + ";
	}

	if (mod & HOTKEYF_ALT)
	{
		scan = (WORD)MapVirtualKey(VK_MENU, 0);
		GetKeyNameText((scan<<16), szBuf, MAX_PATH);
		strHotkey += szBuf;
		strHotkey += " + ";
	}

	scan = (WORD)MapVirtualKey(vk, 0);
	GetKeyNameText((scan<<16) | (mod & HOTKEYF_EXT ? 1<<24 : 0), szBuf, MAX_PATH);
	strHotkey += szBuf;

	return strHotkey;
}
//////////////////////////////////////////////////////////////////////
void CHotkeyManager::Copy(CHotkeyManager* pSrc)
{
	Destroy();

	POSITION pos = pSrc->GetHeadPosition();

	while (pos)
	{
		LPHOTKEY_DESC pDesc = pSrc->GetNext(pos);

		SetHotkey(pDesc->dwHotkeyID, pDesc->dwHotkey);
	}
}
//////////////////////////////////////////////////////////////////////
BOOL CHotkeyManager::ValidateHotkey(DWORD dwHotkeyID, DWORD dwHotkey)
{
	switch (dwHotkey)
		//add restricted combinations here
	{
	case 0x00020050:	//Ctrl+P is reserved to open "Plugins" tab in RTSS properties
		return FALSE;
	}

	LPHOTKEY_DESC lpDesc = FindByHotkey(dwHotkey);

	if (lpDesc && (lpDesc->dwHotkeyID != dwHotkeyID))	
		//this combination is already reserved by different hotkey
		return FALSE;

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
