module;

#include "spdlog/spdlog.h"

export module FireBirdLib.Log;

using std::shared_ptr;

export {
	inline shared_ptr<spdlog::logger> gl_dailyLogger{nullptr};
	inline shared_ptr<spdlog::logger> gl_traceLogger{nullptr}; // 每日跟踪日志，用于系统调试
	inline shared_ptr<spdlog::logger> gl_errorLogger{nullptr}; // 错误跟踪日志，用于系统调试，默认日志。
	inline shared_ptr<spdlog::logger> gl_dailyWebSocketLogger{nullptr};
	inline shared_ptr<spdlog::logger> gl_SoftwareDevelopingLogger{nullptr};
}
