#include"pch.h"

#include"DayLineWebData.h"

CDayLineWebData::CDayLineWebData() {
	Reset();
}

void CDayLineWebData::Reset() {
	m_strStockCode = "";
	m_sDataBuffer.resize(0);
	m_vTempDayLine.resize(0);
	m_lCurrentPos = 0;
}
