#include "pch.h"
#include "HighPerformanceCounter.h"

CHighPerformanceCounter::CHighPerformanceCounter() {
	QueryPerformanceFrequency(&m_frequency);
}