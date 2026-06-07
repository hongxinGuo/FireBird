#pragma once

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
		} catch (...) {
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

// Macros: use like SPDLOG_ASSERT(x) or SPDLOG_ASSERT_MSG(x, "detail")
#define SPDLOG_ASSERT(expr) \
  ((expr) ? static_cast<void>(0) : SpdlogAssertFail(#expr, __FILE__, __LINE__, __func__, nullptr))

#define SPDLOG_ASSERT_MSG(expr, message) \
  ((expr) ? static_cast<void>(0) : SpdlogAssertFail(#expr, __FILE__, __LINE__, __func__, (message)))
