// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H_INCLUDE_
#define PCH_H_INCLUDE_

#define NOMINMAX // 禁止windows.h中定义min和max宏，避免与std::min和std::max冲突

#include"stdafx.h"
#include <absl/log/absl_check.h> // ABSL_DCHECK(condition) << "message";
#include"absl/log/absl_log.h" // ABSL_DLOG(INFO) << "message"; 
#include"nlohmann/json.hpp"

#include"StockMarketSQLTable.h"

#include"globedef.h"

#endif
