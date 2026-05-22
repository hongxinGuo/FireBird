// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H_INCLUDE_
#define PCH_H_INCLUDE_

#include <sqlpp11/sqlpp11.h> // 包含sqlpp11的主头文件，确保所有必要的定义都被包含
#include"StockMarketSQLTable.h" // 包含数据库表定义，确保sqlpp11的相关定义被包含

#include"stdafx.h"
using namespace std;

#define UNICODE_LITERALS_FOR_TEST_NAMES
#include"gtest/gtest.h"
#include"gmock/gmock.h"
using namespace testing;

// fmt库
#define FMT_HEADER_ONLY
#define FMT_UNICODE 1

// 使用spdlog作为日志库
#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include "spdlog/sinks/daily_file_sink.h"
#include <spdlog/sinks/basic_file_sink.h>

// nlohmann json库
#include"nlohmann/json.hpp"

// simdjson库
#include"simdjson.h"
using namespace simdjson;

#include"globedef.h"

#endif
