#include"pch.h"

#include"VirtualHistoryCandle.h"

void CVirtualHistoryCandle::Reset() {
	m_tpDate = chrono::local_days{};
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
