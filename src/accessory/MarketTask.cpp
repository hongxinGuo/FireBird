#include "pch.h"

#include "MarketTask.h"

map<long, CString> gl_mapMarketMapIndex;
map<CString, long> gl_mapMarketMapString;

CMarketTask::CMarketTask() {
	m_lTime = 0;
	m_lType = 0;
}

CMarketTask::CMarketTask(long lTime, long lType) {
	m_lTime = lTime;
	m_lType = lType;
}