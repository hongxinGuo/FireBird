﻿// StockAnalysis.h: StockAnalysis 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“pch.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

// CStockAnalysisApp:
// 有关此类的实现，请参阅 StockAnalysis.cpp
//

class CStockAnalysisApp : public CWinAppEx
{
public:
	CStockAnalysisApp();

	// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// 实现
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CStockAnalysisApp theApp;
