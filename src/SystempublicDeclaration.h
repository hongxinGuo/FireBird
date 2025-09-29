#pragma once

#define WM_FIREBIRD_RUNNING            WM_APP + 100
#define WM_FIREBIRD_EXIT               WM_FIREBIRD_RUNNING + 1
#define WM_FIREBIRD_SCHEDULING_EXIT    WM_FIREBIRD_EXIT + 1

#include<string>

inline std::string sFireBirdApp{ "FireBirdStockAnalysis" };
inline std::string sWatchDogApp{ "WatchdogQT" };
