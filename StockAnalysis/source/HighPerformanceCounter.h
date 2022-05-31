#pragma once
class CHighPerformanceCounter {
public:
	CHighPerformanceCounter();
	~CHighPerformanceCounter() {}
	void Start() { QueryPerformanceCounter(&m_start); }
	void Stop() { QueryPerformanceCounter(&m_end); }

	void SetFrequency(long long freq) { s_frequency = freq; s_isInitialized = true; }

	bool IsInitialized(void) noexcept { return s_isInitialized; }

	long long GetElapseTick(void) { ASSERT(s_isInitialized); return (long long)(m_end.QuadPart - m_start.QuadPart); }
	long long GetElapsedSecond(void) { ASSERT(s_isInitialized); return (long long)((m_end.QuadPart - m_start.QuadPart) / s_frequency); }
	long long GetElapsedMilliSecond(void) { ASSERT(s_isInitialized); return (long long)((m_end.QuadPart - m_start.QuadPart) * 1000 / s_frequency); }
	long long GetElapsedMicroSecond(void) { ASSERT(s_isInitialized); return (long long)((m_end.QuadPart - m_start.QuadPart) * 1000000 / s_frequency); }
	long long GetElapsedNanoSecond(void) { ASSERT(s_isInitialized); return (long long)((m_end.QuadPart - m_start.QuadPart) * 1000000000 / s_frequency); }
protected:
	LARGE_INTEGER m_start;
	LARGE_INTEGER m_end;
	static long long  s_frequency;
	static bool s_isInitialized;
};

extern CHighPerformanceCounter g_highPerformanceCounter;
