#include "pch.h"

#include "MarketTask.h"

map<long, string> gl_mapMarketMapIndex;

CMarketTask::CMarketTask() {
	m_lType = 0;
	m_time = chrono::local_seconds{};
}

CMarketTask::CMarketTask(long lType, chrono::local_seconds time) {
	m_lType = lType;
	m_time = time;
}
