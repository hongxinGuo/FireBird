#include "pch.h"
#include "HighPerformanceCounter.h"

long long CHighPerformanceCounter::s_frequency = 10000000;// 频率计数器。高精度计数器每秒频率. 默认为1千万（i710700KF处理器的默认值）。
bool CHighPerformanceCounter::s_isInitialized = false;

CHighPerformanceCounter::CHighPerformanceCounter() {
	m_start.QuadPart = 0;
	m_end.QuadPart = 0;
}