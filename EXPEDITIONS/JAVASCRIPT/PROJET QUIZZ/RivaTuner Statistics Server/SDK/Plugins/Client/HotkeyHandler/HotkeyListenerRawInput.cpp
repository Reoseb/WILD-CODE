// HotkeyListenerRawInput.cpp : implementation file
//
// created by Unwinder
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HotkeyListenerRawInput.h"
/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CHotkeyListenerRawInput
/////////////////////////////////////////////////////////////////////////////
#include "HotkeyHandlerWnd.h"
/////////////////////////////////////////////////////////////////////////////
CHotkeyListenerRawInput::CHotkeyListenerRawInput()
{
	m_pWnd		= NULL;
	m_dwHotkey	= 0;
}  
/////////////////////////////////////////////////////////////////////////////
CHotkeyListenerRawInput::~CHotkeyListenerRawInput()
{
}
/////////////////////////////////////////////////////////////////////////////
BOOL CHotkeyListenerRawInput::Init(CHotkeyHandlerWnd* pWnd)
{
	m_pWnd = pWnd;

	RAWINPUTDEVICE rid;
    rid.usUsagePage		= 1;				// HID_USAGE_PAGE_GENERIC
	rid.usUsage			= 6;				// HID_USAGE_GENERIC_KEYBOARD
	rid.dwFlags			= RIDEV_DEVNOTIFY | RIDEV_INPUTSINK;   
	rid.hwndTarget		= pWnd->m_hWnd;

	return RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));
}
/////////////////////////////////////////////////////////////////////////////
void CHotkeyListenerRawInput::OnRawInput(HRAWINPUT hRawInput)
{
	UINT dwSize = 0;
	
	GetRawInputData(hRawInput, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

	if (dwSize)
	{
		LPBYTE lpBuf = new BYTE[dwSize];
		
		if (GetRawInputData(hRawInput, RID_INPUT, lpBuf, &dwSize, sizeof(RAWINPUTHEADER)) == dwSize)
		{
			RAWINPUT* lpRawInput = (RAWINPUT*)lpBuf;

			if (lpRawInput->header.dwType == RIM_TYPEKEYBOARD)
			{
				DWORD dwVkCode			= lpRawInput->data.keyboard.VKey;	
				DWORD dwFlags			= lpRawInput->data.keyboard.Flags;
				DWORD dwHotkey			= m_dwHotkey;
				DWORD dwHotkeyReleased	= 0;

				if (dwVkCode == VK_CONTROL)
				{
					if (dwFlags & RI_KEY_BREAK)
					{
						dwHotkey &= ~(HOTKEYF_CONTROL<<16);
						dwHotkeyReleased |= (HOTKEYF_CONTROL<<16);
					}
					else
						dwHotkey |= (HOTKEYF_CONTROL<<16);
				}
				else
					if (dwVkCode == VK_SHIFT)
					{
						if (dwFlags & RI_KEY_BREAK)
						{
							dwHotkey &= ~(HOTKEYF_SHIFT<<16);
							dwHotkeyReleased |= (HOTKEYF_SHIFT<<16);
						}
						else
							dwHotkey |= (HOTKEYF_SHIFT<<16);
					}
					else
						if (dwVkCode == VK_MENU)
						{
							if (dwFlags & RI_KEY_BREAK)
							{
								dwHotkey &= ~(HOTKEYF_ALT<<16);
								dwHotkeyReleased |= (HOTKEYF_ALT<<16);
							}
							else
								dwHotkey |= (HOTKEYF_ALT<<16);
						}
						else
						{
							if (dwFlags & RI_KEY_E0)
								dwVkCode|= 0x80000;

							if (dwFlags & RI_KEY_BREAK)
							{
								dwHotkey &= 0xfff70000;
								dwHotkeyReleased &= 0xfff70000;
								dwHotkeyReleased |= dwVkCode;
							}
							else
							{
								dwHotkey &= 0xfff70000;
								dwHotkey |= dwVkCode;
							}
						}

				if (dwHotkeyReleased)
					m_pWnd->OnHotkey(dwHotkeyReleased | 0x80000000);

				if ((dwHotkey & 0x8ffff) && ((dwHotkey & 0x8ffff) != (m_dwHotkey & 0x8ffff)))
					m_pWnd->OnHotkey(dwHotkey);

				m_dwHotkey = dwHotkey;
			}
		}

		delete [] lpBuf;
	}
}
/////////////////////////////////////////////////////////////////////////////
void CHotkeyListenerRawInput::ResetState()
{
	m_dwHotkey = 0;
}
/////////////////////////////////////////////////////////////////////////////
