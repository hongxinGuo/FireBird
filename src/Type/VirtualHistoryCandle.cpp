#include"pch.h"

#include"ConvertToString.h"
#include"VirtualHistoryCandle.h"

#include "CharSetTransfer.h"

void CVirtualHistoryCandle::Reset() {
	m_lDate = 0;
	m_strExchange.clear();
	m_strStockSymbol.clear();
	m_lLastClose = 0;
	m_lOpen = 0;
	m_lHigh = 0;
	m_lLow = 0;
	m_lClose = 0;
	m_dSplitFactor = 1.0;
	m_dDividend = 0.0;
	m_dUpDown = 0.0;
	m_llVolume = 0;
	m_llAmount = 0;
	m_dUpDownRate = 0.0;
	m_dChangeHandRate = 0.0;
	m_llTotalValue = 0;
	m_llCurrentValue = 0;
}

void CVirtualHistoryCandle::AdjustByFactor(double dFactor) {
	m_lLastClose /= dFactor;
	m_lOpen /= dFactor;
	m_lHigh /= dFactor;
	m_lLow /= dFactor;
	m_lClose /= dFactor;
}

bool CVirtualHistoryCandle::IsActive() const {
	if (GetClose() != 0) {
		return true;
	}
	return false;
}

void CVirtualHistoryCandle::SetAverage(size_t length, long lValue) noexcept {
	switch (length) {
	case 5:
		m_l5Average = lValue;
		break;
	case 10:
		m_l10Average = lValue;
		break;
	case 20:
		m_l20Average = lValue;
		break;
	case 30:
		m_l30Average = lValue;
		break;
	case 60:
		m_l60Average = lValue;
		break;
	case 120:
		m_l120Average = lValue;
		break;
	case 200:
		m_l200Average = lValue;
		break;
	case 240:
		m_l240Average = lValue;
		break;
	default: // 例外状态时存至MA处
		m_lAverage = lValue;
		break;
	}
}

long CVirtualHistoryCandle::GetAverage(int length) const noexcept {
	switch (length) {
	case 5:
		return m_l5Average;
	case 10:
		return m_l10Average;
	case 20:
		return m_l20Average;
	case 30:
		return m_l30Average;
	case 60:
		return m_l60Average;
	case 120:
		return m_l120Average;
	case 200:
		return m_l200Average;
	case 240:
		return m_l240Average;
	default: // 例外状态
		return m_lAverage;
	}
}

void CVirtualHistoryCandle::SaveBasicData(CVirtualSetHistoryCandle* pVirtualSetHistoryCandle) const {
	ASSERT(pVirtualSetHistoryCandle->IsOpen());
	auto ratio = GetRatio();

	pVirtualSetHistoryCandle->m_Date = GetDate();
	pVirtualSetHistoryCandle->m_Exchange = GetExchange().c_str();
	pVirtualSetHistoryCandle->m_Symbol = GetStockSymbol().c_str();
	pVirtualSetHistoryCandle->m_LastClose = ConvertValueToCString(GetLastClose(), ratio);
	pVirtualSetHistoryCandle->m_High = ConvertValueToCString(GetHigh(), ratio);
	pVirtualSetHistoryCandle->m_Low = ConvertValueToCString(GetLow(), ratio);
	pVirtualSetHistoryCandle->m_Open = ConvertValueToCString(GetOpen(), ratio);
	pVirtualSetHistoryCandle->m_Close = ConvertValueToCString(GetClose(), ratio);
	pVirtualSetHistoryCandle->m_dividend = ConvertValueToCString(GetDividend());
	pVirtualSetHistoryCandle->m_splitFactor = ConvertValueToCString(GetSplitFactor());
	pVirtualSetHistoryCandle->m_Volume = ConvertValueToCString(GetVolume());
	pVirtualSetHistoryCandle->m_Amount = ConvertValueToCString(GetAmount());
	pVirtualSetHistoryCandle->m_UpAndDown = ConvertValueToCString(GetUpDown());
	pVirtualSetHistoryCandle->m_UpDownRate = ConvertValueToCString(GetUpDownRate());
	pVirtualSetHistoryCandle->m_ChangeHandRate = ConvertValueToCString(GetChangeHandRate());
	pVirtualSetHistoryCandle->m_TotalValue = ConvertValueToCString(GetTotalValue());
	pVirtualSetHistoryCandle->m_CurrentValue = ConvertValueToCString(GetCurrentValue());
}

void CVirtualHistoryCandle::AppendBasicData(CVirtualSetHistoryCandle* pVirtualSetHistoryCandle) const {
	ASSERT(pVirtualSetHistoryCandle->IsOpen());
	pVirtualSetHistoryCandle->AddNew();
	SaveBasicData(pVirtualSetHistoryCandle);
	pVirtualSetHistoryCandle->Update();
}

void CVirtualHistoryCandle::LoadBasicData(const CVirtualSetHistoryCandle* pVirtualSetHistoryCandle) {
	ASSERT(pVirtualSetHistoryCandle->IsOpen());
	auto ratio = GetRatio();

	m_lDate = pVirtualSetHistoryCandle->m_Date;
	m_strExchange = T2Utf8(pVirtualSetHistoryCandle->m_Exchange);
	m_strStockSymbol = T2Utf8(pVirtualSetHistoryCandle->m_Symbol);
	m_lLastClose = _tstof(pVirtualSetHistoryCandle->m_LastClose) * ratio;
	m_lOpen = _tstof(pVirtualSetHistoryCandle->m_Open) * ratio;
	m_lHigh = _tstof(pVirtualSetHistoryCandle->m_High) * ratio;
	m_lLow = _tstof(pVirtualSetHistoryCandle->m_Low) * ratio;
	m_lClose = _tstof(pVirtualSetHistoryCandle->m_Close) * ratio;
	m_dSplitFactor = _tstof(pVirtualSetHistoryCandle->m_splitFactor);
	m_dDividend = _tstof(pVirtualSetHistoryCandle->m_dividend);
	m_dUpDown = _tstof(pVirtualSetHistoryCandle->m_UpAndDown);
	m_llVolume = _tstoll(pVirtualSetHistoryCandle->m_Volume);
	m_llAmount = _tstoll(pVirtualSetHistoryCandle->m_Amount);
	m_dUpDownRate = _tstof(pVirtualSetHistoryCandle->m_UpDownRate);
	m_dChangeHandRate = _tstof(pVirtualSetHistoryCandle->m_ChangeHandRate);
	m_llTotalValue = _tstoll(pVirtualSetHistoryCandle->m_TotalValue);
	m_llCurrentValue = _tstoll(pVirtualSetHistoryCandle->m_CurrentValue);
}
