#pragma once

#ifndef __AFXWIN_H__
#error"在包含此文件之前必须先包含“stdafx.h”文件以生成 PCH 文件"
#endif

#if _MSVC_LANG < 202002L
#error "本系统使用C++20及以上标准"
#endif

#include"SystemConstant.h"
#include"SystemConfiguration.h"
#include"SystemData.h"
#include"SystemMessage.h"

#include"ClassDeclaration.h"

using std::binary_semaphore;

extern counting_semaphore<8> gl_BackgroundWorkingThread; // 最多后台工作线程允许数量

extern binary_semaphore gl_UpdateWorldMarketDB; // 此信号量用于更新WorldMarket数据库
extern binary_semaphore gl_ProcessChinaMarketRTData; // 处理中国市场的实时数据时，不允许同时存储之。
extern binary_semaphore gl_UpdateChinaMarketDB; // 用于更新ChinaMarket数据库

extern time_t gl_tUTCTime; // 所有的市场使用同一个协调世界时（Coordinated Universal Time）
inline time_t GetUTCTime() noexcept { return gl_tUTCTime; }
inline void TestSetUTCTime(time_t time) noexcept { gl_tUTCTime = time; }
