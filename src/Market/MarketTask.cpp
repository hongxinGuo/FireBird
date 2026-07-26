#include "pch.h"

#include "MarketTask.h"
#include"TimeConvert.h"

map<long, string> gl_mapMarketMapIndex;

CMarketTask::CMarketTask() {
	m_lType = 0;
	m_time = local_seconds{};
}

CMarketTask::CMarketTask(long lType, local_seconds time) {
	m_lType = lType;
	m_time = time;
}

void CMarketTask::SetTime(const hh_mm_ss<seconds>& hhmmss) {
	m_time = toLocalTime(hhmmss);
}

void CMarketTask::SetTime(long lTime) {
	m_time = toLocalTime(lTime);
}
