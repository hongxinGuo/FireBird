// FireBird.h: FireBird 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“pch.h”以生成 PCH 文件"
#endif

#if _MSVC_LANG <= 201703
#error "本系统使用C++20及以上标准"
#endif

#include "resource.h"       // 主符号

// CFireBirdApp:
// 有关此类的实现，请参阅 FireBird.cpp
//

class CFireBirdApp final : public CWinAppEx {
public:
	CFireBirdApp();

	// 重写
public:
	BOOL InitInstance() override;
	int ExitInstance() override;

	// 实现
	BOOL m_bHiColorIcons;

	void PreLoadState() override;
	void LoadCustomState() override;
	void SaveCustomState() override;

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFireBirdApp theApp;
