
// Plugin.h : Plugin ���ε{�����D���Y��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"       // �D�n�Ÿ�


// CPluginApp:
// �аѾ\��@�����O�� Plugin.cpp
//

class CPluginApp : public CWinApp
{
public:
	CPluginApp();


// �мg
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �{���X��@
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPluginApp theApp;
