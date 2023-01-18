#include"pch.h"

#include"TimeConvert.h"
#include "WeekLine.h"

CWeekLine::CWeekLine() : CVirtualHistoryCandleExtend() {
	CWeekLine::Reset();
}

void CWeekLine::Reset(void) {
	CVirtualHistoryCandleExtend::Reset();
}

bool CWeekLine::UpdateWeekLine(CVirtualHistoryCandleExtendPtr pHistoryCandleExtend) {
	if (m_lDate == 0) m_lDate = GetCurrentMonday(pHistoryCandleExtend->GetMarketDate());
	if (m_time == 0) m_time = pHistoryCandleExtend->GetMarketTime();
	if (m_strStockSymbol == _T("")) m_strStockSymbol = pHistoryCandleExtend->GetStockSymbol();
	if (m_strDisplaySymbol == _T("")) m_strDisplaySymbol = pHistoryCandleExtend->GetDisplaySymbol();

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

	m_lOrdinaryBuyVolume += pHistoryCandleExtend->GetOrdinaryBuyVolume();
	m_lAttackBuyVolume += pHistoryCandleExtend->GetAttackBuyVolume();
	m_lStrongBuyVolume += pHistoryCandleExtend->GetStrongBuyVolume();
	m_lOrdinarySellVolume += pHistoryCandleExtend->GetOrdinarySellVolume();
	m_lAttackSellVolume += pHistoryCandleExtend->GetAttackSellVolume();
	m_lStrongSellVolume += pHistoryCandleExtend->GetStrongSellVolume();
	m_lUnknownVolume += pHistoryCandleExtend->GetUnknownVolume();
	m_lCanceledBuyVolume += pHistoryCandleExtend->GetCanceledBuyVolume();
	m_lCanceledSellVolume += pHistoryCandleExtend->GetCanceledSellVolume();

	m_lTransactionNumber += pHistoryCandleExtend->GetTransactionNumber();
	m_lTransactionNumberBelow5000 += pHistoryCandleExtend->GetTransactionNumberBelow5000();
	m_lTransactionNumberBelow50000 += pHistoryCandleExtend->GetTransactionNumberBelow50000();
	m_lTransactionNumberBelow200000 += pHistoryCandleExtend->GetTransactionNumberBelow200000();
	m_lTransactionNumberAbove200000 += pHistoryCandleExtend->GetTransactionNumberAbove200000();

	m_lAttackBuyBelow50000 += pHistoryCandleExtend->GetAttackBuyBelow50000();
	m_lAttackBuyBelow200000 += pHistoryCandleExtend->GetAttackBuyBelow200000();
	m_lAttackBuyAbove200000 += pHistoryCandleExtend->GetAttackBuyAbove200000();
	m_lAttackSellBelow50000 += pHistoryCandleExtend->GetAttackSellBelow50000();
	m_lAttackSellBelow200000 += pHistoryCandleExtend->GetAttackSellBelow200000();
	m_lAttackSellAbove200000 += pHistoryCandleExtend->GetAttackSellAbove200000();

	m_lOrdinaryBuyNumberBelow5000 += pHistoryCandleExtend->GetOrdinaryBuyNumberBelow5000();
	m_lOrdinaryBuyNumberBelow10000 += pHistoryCandleExtend->GetOrdinaryBuyNumberBelow10000();
	m_lOrdinaryBuyNumberBelow20000 += pHistoryCandleExtend->GetOrdinaryBuyNumberBelow20000();
	m_lOrdinaryBuyNumberBelow50000 += pHistoryCandleExtend->GetOrdinaryBuyNumberBelow50000();
	m_lOrdinaryBuyNumberBelow100000 += pHistoryCandleExtend->GetOrdinaryBuyNumberBelow100000();
	m_lOrdinaryBuyNumberBelow200000 += pHistoryCandleExtend->GetOrdinaryBuyNumberBelow200000();
	m_lOrdinaryBuyNumberAbove200000 += pHistoryCandleExtend->GetOrdinaryBuyNumberAbove200000();
	m_lOrdinarySellNumberBelow5000 += pHistoryCandleExtend->GetOrdinarySellNumberBelow5000();
	m_lOrdinarySellNumberBelow10000 += pHistoryCandleExtend->GetOrdinarySellNumberBelow10000();
	m_lOrdinarySellNumberBelow20000 += pHistoryCandleExtend->GetOrdinarySellNumberBelow20000();
	m_lOrdinarySellNumberBelow50000 += pHistoryCandleExtend->GetOrdinarySellNumberBelow50000();
	m_lOrdinarySellNumberBelow100000 += pHistoryCandleExtend->GetOrdinarySellNumberBelow100000();
	m_lOrdinarySellNumberBelow200000 += pHistoryCandleExtend->GetOrdinarySellNumberBelow200000();
	m_lOrdinarySellNumberAbove200000 += pHistoryCandleExtend->GetOrdinarySellNumberAbove200000();

	m_lOrdinaryBuyVolumeBelow5000 += pHistoryCandleExtend->GetOrdinaryBuyVolumeBelow5000();
	m_lOrdinaryBuyVolumeBelow10000 += pHistoryCandleExtend->GetOrdinaryBuyVolumeBelow10000();
	m_lOrdinaryBuyVolumeBelow20000 += pHistoryCandleExtend->GetOrdinaryBuyVolumeBelow20000();
	m_lOrdinaryBuyVolumeBelow50000 += pHistoryCandleExtend->GetOrdinaryBuyVolumeBelow50000();
	m_lOrdinaryBuyVolumeBelow100000 += pHistoryCandleExtend->GetOrdinaryBuyVolumeBelow100000();
	m_lOrdinaryBuyVolumeBelow200000 += pHistoryCandleExtend->GetOrdinaryBuyVolumeBelow200000();
	m_lOrdinaryBuyVolumeAbove200000 += pHistoryCandleExtend->GetOrdinaryBuyVolumeAbove200000();
	m_lOrdinarySellVolumeBelow5000 += pHistoryCandleExtend->GetOrdinarySellVolumeBelow5000();
	m_lOrdinarySellVolumeBelow10000 += pHistoryCandleExtend->GetOrdinarySellVolumeBelow10000();
	m_lOrdinarySellVolumeBelow20000 += pHistoryCandleExtend->GetOrdinarySellVolumeBelow20000();
	m_lOrdinarySellVolumeBelow50000 += pHistoryCandleExtend->GetOrdinarySellVolumeBelow50000();
	m_lOrdinarySellVolumeBelow100000 += pHistoryCandleExtend->GetOrdinarySellVolumeBelow100000();
	m_lOrdinarySellVolumeBelow200000 += pHistoryCandleExtend->GetOrdinarySellVolumeBelow200000();
	m_lOrdinarySellVolumeAbove200000 += pHistoryCandleExtend->GetOrdinarySellVolumeAbove200000();

	m_lCanceledBuyVolumeBelow5000 += pHistoryCandleExtend->GetCanceledBuyVolumeBelow5000();
	m_lCanceledBuyVolumeBelow10000 += pHistoryCandleExtend->GetCanceledBuyVolumeBelow10000();
	m_lCanceledBuyVolumeBelow20000 += pHistoryCandleExtend->GetCanceledBuyVolumeBelow20000();
	m_lCanceledBuyVolumeBelow50000 += pHistoryCandleExtend->GetCanceledBuyVolumeBelow50000();
	m_lCanceledBuyVolumeBelow100000 += pHistoryCandleExtend->GetCanceledBuyVolumeBelow100000();
	m_lCanceledBuyVolumeBelow200000 += pHistoryCandleExtend->GetCanceledBuyVolumeBelow200000();
	m_lCanceledBuyVolumeAbove200000 += pHistoryCandleExtend->GetCanceledBuyVolumeAbove200000();
	m_lCanceledSellVolumeBelow5000 += pHistoryCandleExtend->GetCanceledSellVolumeBelow5000();
	m_lCanceledSellVolumeBelow10000 += pHistoryCandleExtend->GetCanceledSellVolumeBelow10000();
	m_lCanceledSellVolumeBelow20000 += pHistoryCandleExtend->GetCanceledSellVolumeBelow20000();
	m_lCanceledSellVolumeBelow50000 += pHistoryCandleExtend->GetCanceledSellVolumeBelow50000();
	m_lCanceledSellVolumeBelow100000 += pHistoryCandleExtend->GetCanceledSellVolumeBelow100000();
	m_lCanceledSellVolumeBelow200000 += pHistoryCandleExtend->GetCanceledSellVolumeBelow200000();
	m_lCanceledSellVolumeAbove200000 += pHistoryCandleExtend->GetCanceledSellVolumeAbove200000();

	return false;
}
