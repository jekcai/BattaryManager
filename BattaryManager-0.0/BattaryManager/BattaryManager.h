
// BattaryManager.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBattaryManagerApp: 
// �йش����ʵ�֣������ BattaryManager.cpp
//

class CBattaryManagerApp : public CWinApp
{
public:
	CBattaryManagerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBattaryManagerApp theApp;