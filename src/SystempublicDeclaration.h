////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 公共系统声明区
// 各软件共同使用的变量和值
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include<string>

inline std::wstring gl_wsFireBirdRunning{ L"FireBirdStockAnalysisRunningMutex" };
inline std::wstring gl_wsFireBirdSchedulingExit{ L"FireBirdStockAnalysisSchedulingExitMutex" };
inline std::wstring gl_wsFireBirdCheckRunningExit{ L"FireBirdStockAnalysisCheckRunningExitMutex" };
inline std::wstring gl_wsFireBirdExit{ L"FireBirdStockAnalysisExitMutex" };

inline std::string sFireBirdApp{ "FireBirdStockAnalysis" };
inline std::string sWatchDogApp{ "WatchdogQT" };
