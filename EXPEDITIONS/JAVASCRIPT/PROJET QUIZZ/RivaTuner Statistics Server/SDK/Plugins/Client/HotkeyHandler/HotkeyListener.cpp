// HotkeyListener.cpp: implementation of the CHotkeyListener class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HotkeyListener.h"
#include "HotkeyListenerDirectInputThread.h"
#include "HotkeyListenerRawInput.h"
#include "HotkeyHandlerWnd.h"
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHotkeyListener::CHotkeyListener()
{
	m_pDirectInputThread	= NULL;
	m_pRawInput				= NULL;

	m_dwMode				= HOTKEYLISTENERMODE_DIRECTINPUT;
}
//////////////////////////////////////////////////////////////////////
CHotkeyListener::~CHotkeyListener()
{
	Uninit();
}
//////////////////////////////////////////////////////////////////////
void CHotkeyListener::Init(CHotkeyHandlerWnd* pWnd)
{
	Uninit();

	switch (m_dwMode)
	{
	case HOTKEYLISTENERMODE_DIRECTINPUT:
		m_pDirectInputThread = new CHotkeyListenerDirectInputThread(pWnd);

		m_pDirectInputThread->CreateThread(CREATE_SUSPENDED);
		m_pDirectInputThread->SetThreadPriority(THREAD_PRIORITY_NORMAL);
		m_pDirectInputThread->ResumeThread();
		break;
	case HOTKEYLISTENERMODE_RAWINPUT:
		m_pRawInput = new CHotkeyListenerRawInput;

		m_pRawInput->Init(pWnd);
		break;
	}
}
//////////////////////////////////////////////////////////////////////
void CHotkeyListener::Uninit()
{
	if (m_pDirectInputThread)
		m_pDirectInputThread->Kill();

	m_pDirectInputThread = NULL;

	if (m_pRawInput)
		delete m_pRawInput;

	m_pRawInput = NULL;
}
//////////////////////////////////////////////////////////////////////
void CHotkeyListener::ResetState()
{
	if (m_pDirectInputThread)
		m_pDirectInputThread->ResetState();

	if (m_pRawInput)
		m_pRawInput->ResetState();
}
//////////////////////////////////////////////////////////////////////
void CHotkeyListener::SetMode(DWORD dwMode)
{
	m_dwMode = dwMode;
}
//////////////////////////////////////////////////////////////////////
DWORD CHotkeyListener::GetMode()
{
	return m_dwMode;
}
//////////////////////////////////////////////////////////////////////
void CHotkeyListener::OnRawInput(HRAWINPUT hRawInput)
{
	if (m_pRawInput)
		m_pRawInput->OnRawInput(hRawInput);
}
//////////////////////////////////////////////////////////////////////
