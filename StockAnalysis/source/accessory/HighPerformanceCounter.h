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

	static void SetPerformanceFrequency(long long const freq) { s_performanceFrequency = freq; }

	static bool IsInitialized(void) noexcept { return s_isInitialized; }

	long long GetElapseTick(void) const { ASSERT(s_isInitialized); return (long long)(m_end.QuadPart - m_start.QuadPart); }
	long long GetElapsedSecond(void) const { ASSERT(s_isInitialized); return (long long)((m_end.QuadPart - m_start.QuadPart)) / s_performanceFrequency; }
	long long GetElapsedMilliSecond(void) const { ASSERT(s_isInitialized); return (long long)((m_end.QuadPart - m_start.QuadPart)) * 1000 / s_performanceFrequency; }
	long long GetElapsedMicroSecond(void)const { ASSERT(s_isInitialized); return (long long)((m_end.QuadPart - m_start.QuadPart)) * 1000000 / s_performanceFrequency; }
	long long GetElapsedNanoSecond(void) const { ASSERT(s_isInitialized); return (long long)((m_end.QuadPart - m_start.QuadPart)) * 1000000000 / s_performanceFrequency; }
protected:
	static long long  s_performanceFrequency;
	static bool s_isInitialized;

	LARGE_INTEGER m_start{ 0, 0 };
	LARGE_INTEGER m_end{ 0, 0 };
};
