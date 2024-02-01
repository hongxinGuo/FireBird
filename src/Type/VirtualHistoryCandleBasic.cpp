#include"pch.h"

#include"ConvertToString.h"
#include"VirtualHistoryCandleBasic.h"

CVirtualHistoryCandleBasic::CVirtualHistoryCandleBasic() {
	CVirtualHistoryCandleBasic::Reset();
}

void CVirtualHistoryCandleBasic::Reset() {
	m_lDate = 0; // 类型
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

void CVirtualHistoryCandleBasic::CalculateRSLogarithm1(double dRS) {
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

bool CVirtualHistoryCandleBasic::IsActive() const {
	if (GetClose() != 0) {
		return true;
	}
	return false;
}

void CVirtualHistoryCandleBasic::SaveBasicData(not_null<CVirtualSetHistoryCandleBasic*> pVirtualSetHistoryCandleBasic) const {
	ASSERT(pVirtualSetHistoryCandleBasic->IsOpen());

	pVirtualSetHistoryCandleBasic->m_Date = GetMarketDate();
	pVirtualSetHistoryCandleBasic->m_Exchange = GetExchange();
	pVirtualSetHistoryCandleBasic->m_Symbol = GetStockSymbol();
	pVirtualSetHistoryCandleBasic->m_DisplaySymbol = GetDisplaySymbol();
	pVirtualSetHistoryCandleBasic->m_LastClose = ConvertValueToString(GetLastClose(), GetRatio());
	pVirtualSetHistoryCandleBasic->m_High = ConvertValueToString(GetHigh(), GetRatio());
	pVirtualSetHistoryCandleBasic->m_Low = ConvertValueToString(GetLow(), GetRatio());
	pVirtualSetHistoryCandleBasic->m_Open = ConvertValueToString(GetOpen(), GetRatio());
	pVirtualSetHistoryCandleBasic->m_Close = ConvertValueToString(GetClose(), GetRatio());
	pVirtualSetHistoryCandleBasic->m_Volume = ConvertValueToString(GetVolume());
	pVirtualSetHistoryCandleBasic->m_Amount = ConvertValueToString(GetAmount());
	pVirtualSetHistoryCandleBasic->m_UpAndDown = ConvertValueToString(GetUpDown());
	pVirtualSetHistoryCandleBasic->m_UpDownRate = ConvertValueToString(GetUpDownRate());
	pVirtualSetHistoryCandleBasic->m_ChangeHandRate = ConvertValueToString(GetChangeHandRate());
	pVirtualSetHistoryCandleBasic->m_TotalValue = ConvertValueToString(GetTotalValue());
	pVirtualSetHistoryCandleBasic->m_CurrentValue = ConvertValueToString(GetCurrentValue());
	pVirtualSetHistoryCandleBasic->m_RS = ConvertValueToString(GetRS());
	pVirtualSetHistoryCandleBasic->m_RSIndex = ConvertValueToString(GetRSIndex());
	pVirtualSetHistoryCandleBasic->m_RSBackup = ConvertValueToString(GetRSBackup());
}

void CVirtualHistoryCandleBasic::AppendBasicData(not_null<CVirtualSetHistoryCandleBasic*> pVirtualSetHistoryCandleBasic) const {
	ASSERT(pVirtualSetHistoryCandleBasic->IsOpen());
	pVirtualSetHistoryCandleBasic->AddNew();
	SaveBasicData(pVirtualSetHistoryCandleBasic);
	pVirtualSetHistoryCandleBasic->Update();
}

void CVirtualHistoryCandleBasic::LoadBasicData(not_null<CVirtualSetHistoryCandleBasic*> pVirtualSetHistoryCandleBasic) {
	ASSERT(pVirtualSetHistoryCandleBasic->IsOpen());
	m_lDate = pVirtualSetHistoryCandleBasic->m_Date;
	m_strExchange = pVirtualSetHistoryCandleBasic->m_Exchange;
	m_strStockSymbol = pVirtualSetHistoryCandleBasic->m_Symbol;
	m_strDisplaySymbol = pVirtualSetHistoryCandleBasic->m_DisplaySymbol;
	m_lLastClose = atof(pVirtualSetHistoryCandleBasic->m_LastClose) * GetRatio();
	m_lOpen = atof(pVirtualSetHistoryCandleBasic->m_Open) * GetRatio();
	m_lHigh = atof(pVirtualSetHistoryCandleBasic->m_High) * GetRatio();
	m_lLow = atof(pVirtualSetHistoryCandleBasic->m_Low) * GetRatio();
	m_lClose = atof(pVirtualSetHistoryCandleBasic->m_Close) * GetRatio();
	m_dUpDown = atof(pVirtualSetHistoryCandleBasic->m_UpAndDown);
	m_llVolume = atoll(pVirtualSetHistoryCandleBasic->m_Volume);
	m_llAmount = atoll(pVirtualSetHistoryCandleBasic->m_Amount);
	m_dUpDownRate = atof(pVirtualSetHistoryCandleBasic->m_UpDownRate);
	m_dChangeHandRate = atof(pVirtualSetHistoryCandleBasic->m_ChangeHandRate);
	m_llTotalValue = atoll(pVirtualSetHistoryCandleBasic->m_TotalValue);
	m_llCurrentValue = atoll(pVirtualSetHistoryCandleBasic->m_CurrentValue);
	m_dRS = atof(pVirtualSetHistoryCandleBasic->m_RS);
	m_dRSIndex = atof(pVirtualSetHistoryCandleBasic->m_RSIndex);
	m_dRSBackup = atof(pVirtualSetHistoryCandleBasic->m_RSBackup);

	CalculateRSLogarithm1(m_dRS);
}
