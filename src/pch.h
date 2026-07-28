// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H_INCLUDE_
#define PCH_H_INCLUDE_

#if _MSVC_LANG <= 202002L
#error "本系统使用C++23及以上标准"
#endif

#define NOMINMAX // 禁止windows.h中定义min和max宏，避免与std::min和std::max冲突

#include"stdafx.h"

#include"nlohmann/json.hpp"

#include"StockMarketSQLTable.h"

using std::shared_ptr;
using std::weak_ptr;
using std::chrono::time_point_cast;
using std::chrono::seconds;
using std::chrono::system_clock;
using std::chrono::sys_seconds;

#define UNICODE_LITERALS_FOR_TEST_NAMES

extern std::chrono::sys_seconds gl_tpNow; // 所有的市场使用同一个协调世界时（Coordinated Universal Time）
extern const std::chrono::time_zone* gl_pTimeZoneLocal; // 软件运行所在的当地时区
inline time_t GetUTCTime() noexcept { return gl_tpNow.time_since_epoch().count(); }
inline std::chrono::sys_seconds toSysTime(time_t tt) { return time_point_cast<seconds>(system_clock::from_time_t(tt)); }
inline void TestSetUTCTime(time_t time) noexcept { gl_tpNow = time_point_cast<seconds>(system_clock::from_time_t(time)); }
inline void TestSetUTCTime(sys_seconds st) noexcept { gl_tpNow = st; }

#endif
