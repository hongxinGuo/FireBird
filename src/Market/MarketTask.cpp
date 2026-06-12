#include "pch.h"

#include "MarketTask.h"

map<long, string> gl_mapMarketMapIndex;

CMarketTask::CMarketTask() {
	m_lType = 0;
	m_tpTime = chrono::local_seconds{};
}

CMarketTask::CMarketTask(long lType, chrono::local_seconds lTime) {
	m_lType = lType;
	m_tpTime = lTime;
}
