#pragma once
#include <chrono>
#include <string>
#include <vector>

#ifndef __AFXWIN_H__
#error"在包含此文件之前必须先包含“stdafx.h”文件以生成 PCH 文件"
#endif

#if _MSVC_LANG <= 202002L
#error "本系统使用C++23及以上标准"
#endif

extern std::vector<std::string> gl_vNasdaq100Stocks;

#include <semaphore>
//using std::binary_semaphore;

#include"SystemData.h"

#define UNICODE_LITERALS_FOR_TEST_NAMES

#define MAX_BACKGROUND_WORKING_THREAD_ 32

constexpr double EPSILON = 1e-8;

extern std::counting_semaphore<MAX_BACKGROUND_WORKING_THREAD_> gl_BackgroundWorkingThread; // 最多后台工作线程允许数量

extern std::chrono::sys_seconds gl_tpNow; // 所有的市场使用同一个协调世界时（Coordinated Universal Time）
extern const std::chrono::time_zone* gl_pTimeZoneLocal; // 软件运行所在的当地时区
inline time_t GetUTCTime() noexcept { return gl_tpNow.time_since_epoch().count(); }
inline std::chrono::sys_seconds toSysTime(time_t tt) { return std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(tt)); }
inline void TestSetUTCTime(time_t time) noexcept { gl_tpNow = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(time)); }
inline void TestSetUTCTime(std::chrono::sys_seconds st) noexcept { gl_tpNow = st; }

extern int64_t gl_TiingoTotalData;
extern int64_t gl_FinnhubTotalData;
extern int64_t gl_ChinaMarketTotalData;

#define TRACE_SCHEDULE_TASK_ // 跟踪任务调度
