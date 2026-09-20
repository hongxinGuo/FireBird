#pragma once

#include "spdlog/spdlog.h"

using std::shared_ptr;
using std::string;

inline shared_ptr<spdlog::logger> gl_dailyLogger{ nullptr };
inline shared_ptr<spdlog::logger> gl_traceLogger{ nullptr }; // 每日跟踪日志，用于系统调试
inline shared_ptr<spdlog::logger> gl_errorLogger{ nullptr }; // 每日错误日志，用于系统调试
inline shared_ptr<spdlog::logger> gl_dailyWebSocketLogger{ nullptr };
inline shared_ptr<spdlog::logger> gl_dailyWebLogger{ nullptr }; // 网络信息
inline shared_ptr<spdlog::logger> gl_SoftwareDevelopingLogger{ nullptr };

void logInfoDatabaseException(const string& typeName, const string& msg, const sqlpp::mysql::exception& e);
void logWarnDatabaseException(const string& typeName, const string& msg, const sqlpp::mysql::exception& e);
void logErrorDatabaseException(const string& typeName, const string& msg, const sqlpp::mysql::exception& e);
