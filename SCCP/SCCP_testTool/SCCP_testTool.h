
// SCCP_testTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSCCP_testToolApp: 
// �йش����ʵ�֣������ SCCP_testTool.cpp
//

class CSCCP_testToolApp : public CWinApp
{
public:
	CSCCP_testToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSCCP_testToolApp theApp;