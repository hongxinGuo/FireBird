////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 公共系统声明区
// 各软件共同使用的变量和值
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#define WM_FIREBIRD_RUNNING_            WM_APP + 1
#define WM_FIREBIRD_SCHEDULING_EXIT_    WM_APP + 2
#define WM_FIREBIRD_EXIT_               WM_APP + 10
#define WM_FIREBIRD_NOTHING_            WM_APP + 4

#include<string>

inline std::string sFireBirdApp{ "FireBirdStockAnalysis" };
inline std::string sWatchDogApp{ "WatchdogQT" };
