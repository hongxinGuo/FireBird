#pragma once

class CHighPerformanceCounter {
public:
	CHighPerformanceCounter();
	CHighPerformanceCounter(const CHighPerformanceCounter&) = delete;
	CHighPerformanceCounter& operator=(const CHighPerformanceCounter&) = delete;
	CHighPerformanceCounter(const CHighPerformanceCounter&&) noexcept = delete;
	CHighPerformanceCounter& operator=(const CHighPerformanceCounter&&) noexcept = delete;
	~CHighPerformanceCounter() = default;
	void start() { QueryPerformanceCounter(&m_start); }
	void stop() { QueryPerformanceCounter(&m_end); }

	static void SetPerformanceFrequency(const long long freq) { s_performanceFrequency = freq; }

	static bool IsInitialized() noexcept { return s_isInitialized; }

	[[nodiscard]] long long GetElapseTick() const {
		ASSERT(s_isInitialized);
		return (long long)(m_end.QuadPart - m_start.QuadPart);
	}

	[[nodiscard]] long long GetElapsedSecond() const {
		ASSERT(s_isInitialized);
		return (long long)((m_end.QuadPart - m_start.QuadPart)) / s_performanceFrequency;
	}

	[[nodiscard]] long long GetElapsedMilliSecond() const {
		ASSERT(s_isInitialized);
		return (long long)((m_end.QuadPart - m_start.QuadPart)) * 1000 / s_performanceFrequency;
	}

	[[nodiscard]] long long GetElapsedMicroSecond() const {
		ASSERT(s_isInitialized);
		return (long long)((m_end.QuadPart - m_start.QuadPart)) * 1000000 / s_performanceFrequency;
	}

	[[nodiscard]] long long GetElapsedNanoSecond() const {
		ASSERT(s_isInitialized);
		return (long long)((m_end.QuadPart - m_start.QuadPart)) * 1000000000 / s_performanceFrequency;
	}

protected:
	static long long s_performanceFrequency;
	static bool s_isInitialized;

	LARGE_INTEGER m_start{0, 0};
	LARGE_INTEGER m_end{0, 0};
};
