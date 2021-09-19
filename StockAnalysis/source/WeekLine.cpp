#include"pch.h"

#include"globedef.h"
#include "WeekLine.h"

CWeekLine::CWeekLine() : CVirtualHistoryCandleExtend() {
	Reset();
}

void CWeekLine::Reset(void) {
	CVirtualHistoryCandleExtend::Reset();
}

bool CWeekLine::UpdateWeekLine(CDayLinePtr pDayLine) {
	if (m_lDate == 0) m_lDate = GetCurrentMonday(pDayLine->GetFormatedMarketDate());;
	if (m_time == 0) m_time = pDayLine->GetFormatedMarketTime();
	if (m_strStockSymbol == _T("")) m_strStockSymbol = pDayLine->GetStockSymbol();
	if (m_strDisplaySymbol == _T("")) m_strDisplaySymbol = pDayLine->GetDisplaySymbol();

	if (m_lOpen == 0) m_lOpen = pDayLine->GetOpen();
	if (m_lLastClose == 0) m_lLastClose = pDayLine->GetLastClose();
	m_lClose = pDayLine->GetClose();
	if (m_lHigh == 0) m_lHigh = pDayLine->GetHigh();
	else m_lHigh = m_lHigh > pDayLine->GetHigh() ? m_lHigh : pDayLine->GetHigh();
	if (m_lLow == 0) m_lLow = pDayLine->GetLow();
	else m_lLow = m_lLow < pDayLine->GetLow() ? m_lLow : pDayLine->GetLow();
	m_dUpDown = (double)(m_lClose - m_lLastClose) / GetRatio();
	m_dUpDownRate = m_dUpDown * 100000 / m_lLastClose;

	m_llVolume += pDayLine->GetVolume();
	m_llAmount += pDayLine->GetAmount();
	m_llTotalValue = pDayLine->GetTotalValue();
	m_llCurrentValue = pDayLine->GetCurrentValue();
	m_dChangeHandRate += pDayLine->GetChangeHandRate();

	m_lOrdinaryBuyVolume += pDayLine->GetOrdinaryBuyVolume();
	m_lAttackBuyVolume += pDayLine->GetAttackBuyVolume();
	m_lStrongBuyVolume += pDayLine->GetStrongBuyVolume();
	m_lOrdinarySellVolume += pDayLine->GetOrdinarySellVolume();
	m_lAttackSellVolume += pDayLine->GetAttackSellVolume();
	m_lStrongSellVolume += pDayLine->GetStrongSellVolume();
	m_lUnknownVolume += pDayLine->GetUnknownVolume();
	m_lCanceledBuyVolume += pDayLine->GetCanceledBuyVolume();
	m_lCanceledSellVolume += pDayLine->GetCanceledSellVolume();

	m_lTransactionNumber += pDayLine->GetTransactionNumber();
	m_lTransactionNumberBelow5000 += pDayLine->GetTransactionNumberBelow5000();
	m_lTransactionNumberBelow50000 += pDayLine->GetTransactionNumberBelow50000();
	m_lTransactionNumberBelow200000 += pDayLine->GetTransactionNumberBelow200000();
	m_lTransactionNumberAbove200000 += pDayLine->GetTransactionNumberAbove200000();

	m_lAttackBuyBelow50000 += pDayLine->GetAttackBuyBelow50000();
	m_lAttackBuyBelow200000 += pDayLine->GetAttackBuyBelow200000();
	m_lAttackBuyAbove200000 += pDayLine->GetAttackBuyAbove200000();
	m_lAttackSellBelow50000 += pDayLine->GetAttackSellBelow50000();
	m_lAttackSellBelow200000 += pDayLine->GetAttackSellBelow200000();
	m_lAttackSellAbove200000 += pDayLine->GetAttackSellAbove200000();

	m_lOrdinaryBuyNumberBelow5000 += pDayLine->GetOrdinaryBuyNumberBelow5000();
	m_lOrdinaryBuyNumberBelow10000 += pDayLine->GetOrdinaryBuyNumberBelow10000();
	m_lOrdinaryBuyNumberBelow20000 += pDayLine->GetOrdinaryBuyNumberBelow20000();
	m_lOrdinaryBuyNumberBelow50000 += pDayLine->GetOrdinaryBuyNumberBelow50000();
	m_lOrdinaryBuyNumberBelow100000 += pDayLine->GetOrdinaryBuyNumberBelow100000();
	m_lOrdinaryBuyNumberBelow200000 += pDayLine->GetOrdinaryBuyNumberBelow200000();
	m_lOrdinaryBuyNumberAbove200000 += pDayLine->GetOrdinaryBuyNumberAbove200000();
	m_lOrdinarySellNumberBelow5000 += pDayLine->GetOrdinarySellNumberBelow5000();
	m_lOrdinarySellNumberBelow10000 += pDayLine->GetOrdinarySellNumberBelow10000();
	m_lOrdinarySellNumberBelow20000 += pDayLine->GetOrdinarySellNumberBelow20000();
	m_lOrdinarySellNumberBelow50000 += pDayLine->GetOrdinarySellNumberBelow50000();
	m_lOrdinarySellNumberBelow100000 += pDayLine->GetOrdinarySellNumberBelow100000();
	m_lOrdinarySellNumberBelow200000 += pDayLine->GetOrdinarySellNumberBelow200000();
	m_lOrdinarySellNumberAbove200000 += pDayLine->GetOrdinarySellNumberAbove200000();

	m_lOrdinaryBuyVolumeBelow5000 += pDayLine->GetOrdinaryBuyVolumeBelow5000();
	m_lOrdinaryBuyVolumeBelow10000 += pDayLine->GetOrdinaryBuyVolumeBelow10000();
	m_lOrdinaryBuyVolumeBelow20000 += pDayLine->GetOrdinaryBuyVolumeBelow20000();
	m_lOrdinaryBuyVolumeBelow50000 += pDayLine->GetOrdinaryBuyVolumeBelow50000();
	m_lOrdinaryBuyVolumeBelow100000 += pDayLine->GetOrdinaryBuyVolumeBelow100000();
	m_lOrdinaryBuyVolumeBelow200000 += pDayLine->GetOrdinaryBuyVolumeBelow200000();
	m_lOrdinaryBuyVolumeAbove200000 += pDayLine->GetOrdinaryBuyVolumeAbove200000();
	m_lOrdinarySellVolumeBelow5000 += pDayLine->GetOrdinarySellVolumeBelow5000();
	m_lOrdinarySellVolumeBelow10000 += pDayLine->GetOrdinarySellVolumeBelow10000();
	m_lOrdinarySellVolumeBelow20000 += pDayLine->GetOrdinarySellVolumeBelow20000();
	m_lOrdinarySellVolumeBelow50000 += pDayLine->GetOrdinarySellVolumeBelow50000();
	m_lOrdinarySellVolumeBelow100000 += pDayLine->GetOrdinarySellVolumeBelow100000();
	m_lOrdinarySellVolumeBelow200000 += pDayLine->GetOrdinarySellVolumeBelow200000();
	m_lOrdinarySellVolumeAbove200000 += pDayLine->GetOrdinarySellVolumeAbove200000();

	m_lCanceledBuyVolumeBelow5000 += pDayLine->GetCanceledBuyVolumeBelow5000();
	m_lCanceledBuyVolumeBelow10000 += pDayLine->GetCanceledBuyVolumeBelow10000();
	m_lCanceledBuyVolumeBelow20000 += pDayLine->GetCanceledBuyVolumeBelow20000();
	m_lCanceledBuyVolumeBelow50000 += pDayLine->GetCanceledBuyVolumeBelow50000();
	m_lCanceledBuyVolumeBelow100000 += pDayLine->GetCanceledBuyVolumeBelow100000();
	m_lCanceledBuyVolumeBelow200000 += pDayLine->GetCanceledBuyVolumeBelow200000();
	m_lCanceledBuyVolumeAbove200000 += pDayLine->GetCanceledBuyVolumeAbove200000();
	m_lCanceledSellVolumeBelow5000 += pDayLine->GetCanceledSellVolumeBelow5000();
	m_lCanceledSellVolumeBelow10000 += pDayLine->GetCanceledSellVolumeBelow10000();
	m_lCanceledSellVolumeBelow20000 += pDayLine->GetCanceledSellVolumeBelow20000();
	m_lCanceledSellVolumeBelow50000 += pDayLine->GetCanceledSellVolumeBelow50000();
	m_lCanceledSellVolumeBelow100000 += pDayLine->GetCanceledSellVolumeBelow100000();
	m_lCanceledSellVolumeBelow200000 += pDayLine->GetCanceledSellVolumeBelow200000();
	m_lCanceledSellVolumeAbove200000 += pDayLine->GetCanceledSellVolumeAbove200000();

	return false;
}