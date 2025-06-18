#pragma once

#ifndef __AFXWIN_H__
#error"在包含此文件之前必须先包含“stdafx.h”文件以生成 PCH 文件"
#endif

#if _MSVC_LANG < 202002L
#error "本系统使用C++20及以上标准"
#endif

#include <semaphore>
using std::binary_semaphore;

#include"SystemConstantChinaMarket.h"
#include"SystemConfiguration.h"
#include"SystemData.h"
#include"SystemMessage.h"

#include"ClassDeclaration.h"

#include "spdlog/sinks/daily_file_sink.h"
#define __MAX_BACKGROUND_WORKING_THREAD__ 32

extern HANDLE gl_hFireBirdMutex;

extern std::counting_semaphore<__MAX_BACKGROUND_WORKING_THREAD__> gl_BackgroundWorkingThread; // 最多后台工作线程允许数量

extern binary_semaphore gl_UpdateWorldMarketDB; // 此信号量用于更新WorldMarket数据库
extern binary_semaphore gl_ProcessChinaMarketRTData; // 处理中国市场的实时数据时，不允许同时存储之。
extern binary_semaphore gl_UpdateChinaMarketDB; // 用于更新ChinaMarket数据库

extern map<int, string> gl_stringList;

extern shared_ptr<spdlog::logger> gl_dailyLogger;
extern shared_ptr<spdlog::logger> gl_traceLogger; // 每日跟踪日志，用于系统调试
extern shared_ptr<spdlog::logger> gl_warnLogger; // 每日警告日志，用于系统调试
extern shared_ptr<spdlog::logger> gl_dailyWebSocketLogger;
extern shared_ptr<spdlog::logger> gl_SoftwareDevelopingLogger;

extern std::chrono::sys_seconds gl_tpNow; // 所有的市场使用同一个协调世界时（Coordinated Universal Time）
inline time_t GetUTCTime() noexcept { return gl_tpNow.time_since_epoch().count(); }
inline void TestSetUTCTime(time_t time) noexcept { gl_tpNow = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(time)); }

extern int64_t gl_TiingoTotalData;
extern int64_t gl_FinnhubTotalData;
extern int64_t gl_ChinaMarketTotalData;

#define _TRACE_SCHEDULE_TASK___ // 跟踪任务调度
