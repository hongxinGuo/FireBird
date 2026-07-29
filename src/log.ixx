module;

#include "spdlog/spdlog.h"

export module Log;

using std::shared_ptr;

export {
	extern shared_ptr<spdlog::logger> gl_dailyLogger;
	extern shared_ptr<spdlog::logger> gl_traceLogger; // 每日跟踪日志，用于系统调试
	extern shared_ptr<spdlog::logger> gl_errorLogger; // 每日错误日志，用于系统调试
	extern shared_ptr<spdlog::logger> gl_dailyWebSocketLogger;
	extern shared_ptr<spdlog::logger> gl_SoftwareDevelopingLogger;
}
