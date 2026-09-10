#pragma once

#include "log.h"
#include "spdlog/spdlog.h"

using std::shared_ptr;


extern shared_ptr<spdlog::logger> gl_dailyLogger;
extern shared_ptr<spdlog::logger> gl_traceLogger; // 每日跟踪日志，用于系统调试
extern shared_ptr<spdlog::logger> gl_errorLogger; // 每日错误日志，用于系统调试
extern shared_ptr<spdlog::logger> gl_dailyWebSocketLogger;
extern shared_ptr<spdlog::logger> gl_dailyWebLogger; // 网络信息
extern shared_ptr<spdlog::logger> gl_SoftwareDevelopingLogger;


inline void logInfoDatabaseException(const string& typeName, const string& msg, const sqlpp::mysql::exception& e) { gl_dailyLogger->info("{}'s {} exception: {}", typeName, msg, e.what()); }
inline void logWarnDatabaseException(const string& typeName, const string& msg, const sqlpp::mysql::exception& e) { gl_dailyLogger->warn("{}'s {} exception: {}", typeName, msg, e.what()); }
inline void logErrorDatabaseException(const string& typeName, const string& msg, const sqlpp::mysql::exception& e) { gl_dailyLogger->error("{}'s {} exception: {}", typeName, msg, e.what()); }
