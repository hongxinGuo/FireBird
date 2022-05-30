#pragma once
class CHighPerformanceCounter {
public:
	CHighPerformanceCounter();
	~CHighPerformanceCounter() {}
	void Start() { QueryPerformanceCounter(&m_start); }
	void Stop() { QueryPerformanceCounter(&m_end); }
	long long GetElapsedTime() { return m_end.QuadPart - m_start.QuadPart; }
protected:
	LARGE_INTEGER m_start;
	LARGE_INTEGER m_end;
	LARGE_INTEGER m_frequency;
};
