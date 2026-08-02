module;

#include"spdlog/spdlog.h"
#include <source_location>

export module FireBirdLib.Accessory.SpdlogAssert;

export {
#ifdef _DEBUG
	inline void SpdlogAssertFail(const char* expr,
															 const char* file,
															 int line,
															 const char* func,
															 const char* msg = nullptr) noexcept {
		static std::atomic_flag triggered = ATOMIC_FLAG_INIT;

		// Ensure only first failing thread logs and calls abort.
		if (!triggered.test_and_set()) {
			try {
				// Prefer the raw default logger for speed; fall back to spdlog API.
				if (auto l = spdlog::default_logger(); l != nullptr) {
					l->error("[ASSERT] {} at {}:{} ({}){}", expr ? expr : "<null>", file ? file : "<unknown>", line, func ? func : "<unknown>",
									 (msg && msg[0] != '\0') ? std::format(" - {}", msg) : std::string());
					l->flush();
				}
				else {
					spdlog::error("[ASSERT] {} at {}:{} ({}){}", expr ? expr : "<null>", file ? file : "<unknown>", line, func ? func : "<unknown>",
												(msg && msg[0] != '\0') ? std::format(" - {}", msg) : std::string());
					if (auto def = spdlog::default_logger()) def->flush();
				}
			}
			catch (...) {
				// Last-resort fallback to stderr (no exceptions).
				std::fprintf(stderr,
										 "ASSERTION FAILED: %s\n  at %s:%d (%s)\n",
										 expr ? expr : "<null>",
										 file ? file : "<unknown>",
										 line,
										 func ? func : "<unknown>");
				if (msg && msg[0] != '\0') std::fprintf(stderr, "  message: %s\n", msg);
				std::fflush(stderr);
			}

			// Terminate: safest action for inconsistent program state.
			std::abort();
		}

		// If another thread already triggered abort, yield briefly while process exits.
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
#else
	// In release builds, do nothing.
	inline void SpdlogAssertFail(const char* expr,
															 const char* file,
															 int line,
															 const char* func,
															 const char* msg = nullptr) noexcept {
	}
#endif

	// Module-friendly replacements for the previous macros.
	// Note: macros cannot be exported from modules. Use these inline functions instead.
	// - Use `SPDLOG_ASSERT(expr)` if you don't need the expression string.
	// - Use `SPDLOG_ASSERT_MSG(expr, "msg")` to provide a message.
	// - Use `SPDLOG_ASSERT_EX(expr, "expr_text", "msg")` to also supply the expression text.

	inline void SPDLOG_ASSERT(bool expr,
													 std::source_location loc = std::source_location::current()) noexcept {
		if (!expr) {
			SpdlogAssertFail(nullptr, loc.file_name(), static_cast<int>(loc.line()), loc.function_name(), nullptr);
		}
	}

	inline void SPDLOG_ASSERT_MSG(bool expr,
															 const char* message,
															 std::source_location loc = std::source_location::current()) noexcept {
		if (!expr) {
			SpdlogAssertFail(nullptr, loc.file_name(), static_cast<int>(loc.line()), loc.function_name(), message);
		}
	}

	inline void SPDLOG_ASSERT_EX(bool expr,
															 const char* exprStr,
															 const char* message = nullptr,
															 std::source_location loc = std::source_location::current()) noexcept {
		if (!expr) {
			SpdlogAssertFail(exprStr, loc.file_name(), static_cast<int>(loc.line()), loc.function_name(), message);
		}
	}
}