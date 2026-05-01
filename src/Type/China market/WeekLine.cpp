#include"pch.h"

#include"TimeConvert.h"
#include "WeekLine.h"

bool CWeekLine::UpdateWeekLine(const CVirtualHistoryCandlePtr& pHistoryCandleExtend) {
	if (m_lDate == 0) m_lDate = GetCurrentMonday(pHistoryCandleExtend->GetDate());
	if (GetMarketTime() == 0) m_tpTime = pHistoryCandleExtend->GetMarketTimePoint();
	if (m_strStockSymbol.empty()) m_strStockSymbol = pHistoryCandleExtend->GetStockSymbol();

	if (m_lOpen == 0) m_lOpen = pHistoryCandleExtend->GetOpen();
	if (m_lLastClose == 0) m_lLastClose = pHistoryCandleExtend->GetLastClose();
	m_lClose = pHistoryCandleExtend->GetClose();
	if (m_lHigh == 0) m_lHigh = pHistoryCandleExtend->GetHigh();
	else m_lHigh = m_lHigh > pHistoryCandleExtend->GetHigh() ? m_lHigh : pHistoryCandleExtend->GetHigh();
	if (m_lLow == 0) m_lLow = pHistoryCandleExtend->GetLow();
	else m_lLow = m_lLow < pHistoryCandleExtend->GetLow() ? m_lLow : pHistoryCandleExtend->GetLow();
	m_dUpDown = static_cast<double>(m_lClose - m_lLastClose) / GetRatio();
	if (m_lLastClose > 0) m_dUpDownRate = m_dUpDown * 100000 / m_lLastClose;
	else m_dUpDownRate = 0;

	m_llVolume += pHistoryCandleExtend->GetVolume();
	m_llAmount += pHistoryCandleExtend->GetAmount();
	m_llTotalValue = pHistoryCandleExtend->GetTotalValue();
	m_llCurrentValue = pHistoryCandleExtend->GetCurrentValue();
	m_dChangeHandRate += pHistoryCandleExtend->GetChangeHandRate();

	return false;
}
