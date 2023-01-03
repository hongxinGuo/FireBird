// StockAnalysis.h: StockAnalysis 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“pch.h”以生成 PCH 文件"
#endif

#if _MSVC_LANG <= 201703
#error "本系统使用C++20及以上标准"
#endif

#include "resource.h"       // 主符号

// CStockAnalysisApp:
// 有关此类的实现，请参阅 StockAnalysis.cpp
//

class CStockAnalysisApp final: public CWinAppEx
{
public:
	CStockAnalysisApp();

	// 重写
public:
	virtual BOOL InitInstance() override;
	virtual int ExitInstance() override;

	// 实现
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState() override;
	virtual void LoadCustomState() override;
	virtual void SaveCustomState() override;

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CStockAnalysisApp theApp;
