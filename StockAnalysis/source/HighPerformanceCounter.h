#pragma once

class CHighPerformanceCounter {
public:
	CHighPerformanceCounter();
	~CHighPerformanceCounter() {}
	void Start() { QueryPerformanceCounter(&m_start); }
	void Stop() { QueryPerformanceCounter(&m_end); }

	void SetPerformanceFrequency(long long freq) { s_performanceFrequency = freq; }

	bool IsInitialized(void) noexcept { return s_isInitialized; }

	long long GetElapseTick(void) { ASSERT(s_isInitialized); return (long long)(m_end.QuadPart - m_start.QuadPart); }
	long long GetElapsedSecond(void) { ASSERT(s_isInitialized); return (long long)((m_end.QuadPart - m_start.QuadPart)) / s_performanceFrequency; }
	long long GetElapsedMilliSecond(void) { ASSERT(s_isInitialized); return (long long)((m_end.QuadPart - m_start.QuadPart)) * 1000 / s_performanceFrequency; }
	long long GetElapsedMicroSecond(void) { ASSERT(s_isInitialized); return (long long)((m_end.QuadPart - m_start.QuadPart)) * 1000000 / s_performanceFrequency; }
	long long GetElapsedNanoSecond(void) { ASSERT(s_isInitialized); return (long long)((m_end.QuadPart - m_start.QuadPart)) * 1000000000 / s_performanceFrequency; }
protected:
	static long long  s_performanceFrequency;
	static bool s_isInitialized;

	LARGE_INTEGER m_start;
	LARGE_INTEGER m_end;
};

extern CHighPerformanceCounter g_highPerformanceCounter;
