#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前必须先包含“stdafx.h”文件以生成 PCH 文件"
#endif

// 中国股票市场起始日
constexpr auto __CHINA_MARKET_BEGIN_DATE__ = 19900101;

#include"ClassDeclaration.h"

extern atomic_bool gl_fExitingSystem; //  系统退出标识，用于终止其他线程。
extern bool gl_fExitingCalculatingRS; // 用于通知工作线程退出的信号

extern bool gl_fNormalMode; // 正常模式标识，默认为假。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
