
// ls_config_set.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cls_config_setApp:
// �йش����ʵ�֣������ ls_config_set.cpp
//

class Cls_config_setApp : public CWinApp
{
public:
	Cls_config_setApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cls_config_setApp theApp;