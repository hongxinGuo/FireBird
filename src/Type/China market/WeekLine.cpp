#include"pch.h"

#include"TimeConvert.h"
#include "WeekLine.h"

namespace {
	CChinaStock s_stock;
}

CWeekLine::CWeekLine() {
	m_ratio = s_stock.GetRatio();
}

bool CWeekLine::UpdateWeekLine(const CVirtualHistoryCandle* pDayLine) {
	if (GetMarketDate().time_since_epoch().count() == 0) {
		SetDate(GetCurrentMonday(pDayLine->GetDate()));
	}
	if (m_tpDate.time_since_epoch().count() == 0) m_tpDate = pDayLine->GetMarketDate();
	if (m_strStockSymbol.empty()) m_strStockSymbol = pDayLine->GetStockSymbol();

	if (m_lOpen == 0) m_lOpen = pDayLine->GetOpen();
	if (m_lLastClose == 0) m_lLastClose = pDayLine->GetLastClose();
	m_lClose = pDayLine->GetClose();
	if (m_lHigh == 0) m_lHigh = pDayLine->GetHigh();
	else m_lHigh = m_lHigh > pDayLine->GetHigh() ? m_lHigh : pDayLine->GetHigh();
	if (m_lLow == 0) m_lLow = pDayLine->GetLow();
	else m_lLow = m_lLow < pDayLine->GetLow() ? m_lLow : pDayLine->GetLow();
	m_dUpDown = static_cast<double>(m_lClose - m_lLastClose) / GetRatio();
	if (m_lLastClose > 0) m_dUpDownRate = m_dUpDown * 100000 / m_lLastClose;
	else m_dUpDownRate = 0;

	m_llVolume += pDayLine->GetVolume();
	m_llAmount += pDayLine->GetAmount();
	m_llTotalValue = pDayLine->GetTotalValue();
	m_llCurrentValue = pDayLine->GetCurrentValue();
	m_dChangeHandRate += pDayLine->GetChangeHandRate();

	return false;
}
