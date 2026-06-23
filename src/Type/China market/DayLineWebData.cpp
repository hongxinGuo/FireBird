#include"pch.h"

#include"ChinaMarket.h"
#include"DayLineWebData.h"

#include "CharSetTransfer.h"
#include"WebData.h"

#include "JsonParse.h"

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
	m_sDataBuffer = std::move(pWebData->m_sDataBuffer);
	m_strStockCode = pWebData->GetStockCode();
	m_lCurrentPos = 0;

	return true;
}

void CDayLineWebData::ReportDayLineDownLoaded() {
	//string strTemp = GetSymbol();
	//strTemp += "日线下载完成.");
	//gl_systemMessage.PushDayLineInfoMessage(strTemp);
}
