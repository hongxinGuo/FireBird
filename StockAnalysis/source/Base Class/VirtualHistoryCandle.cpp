#include"pch.h"

#include"VirtualHistoryCandle.h"

CVirtualHistoryCandle::CVirtualHistoryCandle() {
	Reset();
}

CVirtualHistoryCandle::~CVirtualHistoryCandle() {
}

void CVirtualHistoryCandle::Reset() {
	m_lDate = 0;		// 类型
	m_time = 0;
	m_strStockSymbol = _T("");
	m_strDisplaySymbol = _T("");
	m_lLastClose = m_lOpen = m_lHigh = m_lLow = m_lClose = 0; // 收盘价
	m_llVolume = 0;
	m_llAmount = 0;
	m_dUpDown = 0;
	m_dUpDownRate = 0;
	m_llCurrentValue = m_llTotalValue = 0;
	m_dChangeHandRate = 0;
	m_dRS = m_dRSLogarithm = m_dRSIndex = m_dRSBackup = 0.0;
	m_d3RS = m_d5RS = m_d10RS = m_d30RS = m_d60RS = m_d120RS = 0.0;
}

void CVirtualHistoryCandle::CalculateRSLogarithm1(double dRS) {
	const double dLog50 = log10(50.0);
	const double dLog100 = log10(100.0);
	const double dLog = dLog100 - dLog50;
	if ((dRS - 50) > 0) {
		m_dRSLogarithm = 50 + (log10(dRS) - dLog50) * 50 / dLog;
	}
	else if ((dRS - 50) < 0) {
		m_dRSLogarithm = 50 - (log10(100 - dRS) - dLog50) * 50 / dLog;
	}
	else m_dRSLogarithm = 50;
}

bool CVirtualHistoryCandle::SaveHistoryCandle(not_null<CVirtualSetHistoryCandleBasic*> pSetHistoryCandle) {
	ASSERT(pSetHistoryCandle->IsOpen());

	pSetHistoryCandle->m_Date = GetFormatedMarketDate();
	pSetHistoryCandle->m_Exchange = GetExchange();
	pSetHistoryCandle->m_Symbol = GetStockSymbol();
	pSetHistoryCandle->m_DisplaySymbol = GetDisplaySymbol();
	pSetHistoryCandle->m_LastClose = ConvertValueToString(GetLastClose(), GetRatio());
	pSetHistoryCandle->m_High = ConvertValueToString(GetHigh(), GetRatio());
	pSetHistoryCandle->m_Low = ConvertValueToString(GetLow(), GetRatio());
	pSetHistoryCandle->m_Open = ConvertValueToString(GetOpen(), GetRatio());
	pSetHistoryCandle->m_Close = ConvertValueToString(GetClose(), GetRatio());
	pSetHistoryCandle->m_Volume = ConvertValueToString(GetVolume());
	pSetHistoryCandle->m_Amount = ConvertValueToString(GetAmount());
	pSetHistoryCandle->m_UpAndDown = ConvertValueToString(GetUpDown());
	pSetHistoryCandle->m_UpDownRate = ConvertValueToString(GetUpDownRate());
	pSetHistoryCandle->m_ChangeHandRate = ConvertValueToString(GetChangeHandRate());
	pSetHistoryCandle->m_TotalValue = ConvertValueToString(GetTotalValue());
	pSetHistoryCandle->m_CurrentValue = ConvertValueToString(GetCurrentValue());
	pSetHistoryCandle->m_RS = ConvertValueToString(GetRS());
	pSetHistoryCandle->m_RSIndex = ConvertValueToString(GetRSIndex());
	pSetHistoryCandle->m_RSBackup = ConvertValueToString(GetRSBackup());

	return true;
}

bool CVirtualHistoryCandle::AppendHistoryCandle(not_null<CVirtualSetHistoryCandleBasic*> pSetHistoryCandle) {
	ASSERT(pSetHistoryCandle->IsOpen());
	pSetHistoryCandle->AddNew();
	SaveHistoryCandle(pSetHistoryCandle);
	pSetHistoryCandle->Update();

	return true;
}

bool CVirtualHistoryCandle::LoadHistoryCandle(not_null<CVirtualSetHistoryCandleBasic*> pSetHistoryCandle) {
	ASSERT(pSetHistoryCandle->IsOpen());
	m_lDate = pSetHistoryCandle->m_Date;
	m_strExchange = pSetHistoryCandle->m_Exchange;
	m_strStockSymbol = pSetHistoryCandle->m_Symbol;
	m_strDisplaySymbol = pSetHistoryCandle->m_DisplaySymbol;
	m_lLastClose = atof(pSetHistoryCandle->m_LastClose) * GetRatio();
	m_lOpen = atof(pSetHistoryCandle->m_Open) * GetRatio();
	m_lHigh = atof(pSetHistoryCandle->m_High) * GetRatio();
	m_lLow = atof(pSetHistoryCandle->m_Low) * GetRatio();
	m_lClose = atof(pSetHistoryCandle->m_Close) * GetRatio();
	m_dUpDown = atof(pSetHistoryCandle->m_UpAndDown);
	m_llVolume = atoll(pSetHistoryCandle->m_Volume);
	m_llAmount = atoll(pSetHistoryCandle->m_Amount);
	m_dUpDownRate = atof(pSetHistoryCandle->m_UpDownRate);
	m_dChangeHandRate = atof(pSetHistoryCandle->m_ChangeHandRate);
	m_llTotalValue = atoll(pSetHistoryCandle->m_TotalValue);
	m_llCurrentValue = atoll(pSetHistoryCandle->m_CurrentValue);
	m_dRS = atof(pSetHistoryCandle->m_RS);
	m_dRSIndex = atof(pSetHistoryCandle->m_RSIndex);
	m_dRSBackup = atof(pSetHistoryCandle->m_RSBackup);

	CalculateRSLogarithm1(m_dRS);
	return true;
}