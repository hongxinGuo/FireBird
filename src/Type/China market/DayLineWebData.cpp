#include"pch.h"

#include"DayLineWebData.h"
#include"WebData.h"

CDayLineWebData::CDayLineWebData() {
	Reset();
}

void CDayLineWebData::Reset() {
	m_strStockCode = "";
	m_sDataBuffer.resize(0);
	m_vTempDayLine.resize(0);
	m_lCurrentPos = 0;
}

bool CDayLineWebData::TransferWebDataToBuffer(const CWebDataPtr& pWebData) {
	// 将读取的日线数据放入相关股票的日线数据缓冲区中，并设置相关标识。
	//m_sDataBuffer = std::move(pWebData->m_sDataBuffer);
	m_sDataBuffer = pWebData->MoveDataBuffer();
	m_strStockCode = pWebData->GetStockCode();
	m_lCurrentPos = 0;

	return true;
}
