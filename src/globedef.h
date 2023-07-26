#pragma once

#ifndef __AFXWIN_H__
#error"在包含此文件之前必须先包含“stdafx.h”文件以生成 PCH 文件"
#endif

#if _MSVC_LANG <= 201703
#error "本系统使用C++20及以上标准"
#endif

#include"SystemConstant.h"
#include"SystemConfiguration.h"
#include"SystemStatus.h"
#include"SystemData.h"
#include"SystemMessage.h"

#include"ClassDeclaration.h"

#include"HighPerformanceCounter.h"
extern CHighPerformanceCounter gl_counter;
extern bool gl_bGlobeVariableInitialized;
extern time_t gl_tUTC; // 所有的市场使用同一个协调世界时（Coordinated Universal Time）
