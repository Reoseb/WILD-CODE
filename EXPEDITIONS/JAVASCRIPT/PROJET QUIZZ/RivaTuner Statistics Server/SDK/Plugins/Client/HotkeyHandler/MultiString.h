#pragma once
//////////////////////////////////////////////////////////////////////
class CMultiString
{
public:
	CMultiString(LPCSTR lpmsz);
	CMultiString(LPCSTR lpmsz, DWORD size);
	~CMultiString();

	void Init(LPCSTR lpmsz);
	void Init(LPCSTR lpmsz, DWORD size);

	LPCSTR GetNext();

private:
	LPCSTR m_lpmsz;
};
//////////////////////////////////////////////////////////////////////
