#pragma once
class CHighPerformanceCounter {
public:
	CHighPerformanceCounter();
	~CHighPerformanceCounter() {}
	void Start() { QueryPerformanceCounter(&m_start); }
	void Stop() { QueryPerformanceCounter(&m_end); }

	void SetFrequency(long long freq) { s_frequency = freq; }

	long long GetElapseTick(void) { return (long long)(m_end.QuadPart - m_start.QuadPart); }
	long long GetElapsedSecond(void) { return (long long)((m_end.QuadPart - m_start.QuadPart) / s_frequency); }
	long long GetElapsedMilliSecond(void) { return (long long)((m_end.QuadPart - m_start.QuadPart) * 1000 / s_frequency); }
	long long GetElapsedMicroSecond(void) { return (long long)((m_end.QuadPart - m_start.QuadPart) * 1000000 / s_frequency); }
	long long GetElapsedNanoSecond(void) { return (long long)((m_end.QuadPart - m_start.QuadPart) * 1000000000 / s_frequency); }
protected:
	LARGE_INTEGER m_start;
	LARGE_INTEGER m_end;
	static long long  s_frequency;
};

extern CHighPerformanceCounter g_highPerformanceCounter;
