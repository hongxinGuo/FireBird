#include"pch.h"

#include"globedef.h"
#include"ChinaStockHistoryData.h"

int CChinaStockHistoryData::s_iRatio = 1000;

CChinaStockHistoryData::CChinaStockHistoryData() : CVirtualHistoryData() {
	Reset();
}

void CChinaStockHistoryData::CalculateRSLogarithm1(double dRS) {
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

void CChinaStockHistoryData::Reset(void) {
	m_lDate = 0;		// 类型
	m_time = 0;
	m_strExchange = _T("");
	m_strStockSymbol = _T("");
	m_strDisplaySymbol = _T("");
	m_lLastClose = m_lOpen = m_lHigh = m_lLow = m_lClose = 0; // 收盘价
	m_llVolume = 0;
	m_llAmount = 0;
	m_dUpDown = 0;
	m_dUpDownRate = 0;
	m_llCurrentValue = m_llTotalValue = 0;
	m_dChangeHandRate = 0;

	m_lTransactionNumber = 0; //
	m_lTransactionNumberBelow5000 = 0; //
	m_lTransactionNumberBelow50000 = 0; //
	m_lTransactionNumberBelow200000 = 0; //
	m_lTransactionNumberAbove200000 = 0; //

	m_lAttackBuyVolume = m_lStrongBuyVolume = m_lAttackSellVolume = m_lStrongSellVolume = 0;
	m_lUnknownVolume = m_lCanceledBuyVolume = m_lCanceledSellVolume = 0;
	m_dRS = m_dRSLogarithm = m_dRSIndex = m_dRSBackup = 0.0;
	m_lOrdinaryBuyVolume = m_lAttackBuyBelow50000 = m_lAttackBuyBelow200000 = m_lAttackBuyAbove200000 = 0;
	m_lOrdinarySellVolume = m_lAttackSellBelow50000 = m_lAttackSellBelow200000 = m_lAttackSellAbove200000 = 0;
	m_d3RS = m_d5RS = m_d10RS = m_d30RS = m_d60RS = m_d120RS = 0.0;

	m_lOrdinaryBuyVolumeBelow5000 = 0;
	m_lOrdinaryBuyVolumeBelow10000 = 0;
	m_lOrdinaryBuyVolumeBelow20000 = 0;
	m_lOrdinaryBuyVolumeBelow50000 = 0;
	m_lOrdinaryBuyVolumeBelow100000 = 0;
	m_lOrdinaryBuyVolumeBelow200000 = 0;
	m_lOrdinaryBuyVolumeAbove200000 = 0;
	m_lOrdinarySellVolumeBelow5000 = 0;
	m_lOrdinarySellVolumeBelow10000 = 0;
	m_lOrdinarySellVolumeBelow20000 = 0;
	m_lOrdinarySellVolumeBelow50000 = 0;
	m_lOrdinarySellVolumeBelow100000 = 0;
	m_lOrdinarySellVolumeBelow200000 = 0;
	m_lOrdinarySellVolumeAbove200000 = 0;
	m_lOrdinaryBuyNumberBelow5000 = 0;
	m_lOrdinaryBuyNumberBelow10000 = 0;
	m_lOrdinaryBuyNumberBelow20000 = 0;
	m_lOrdinaryBuyNumberBelow50000 = 0;
	m_lOrdinaryBuyNumberBelow100000 = 0;
	m_lOrdinaryBuyNumberBelow200000 = 0;
	m_lOrdinaryBuyNumberAbove200000 = 0;
	m_lOrdinarySellNumberBelow5000 = 0;
	m_lOrdinarySellNumberBelow10000 = 0;
	m_lOrdinarySellNumberBelow20000 = 0;
	m_lOrdinarySellNumberBelow50000 = 0;
	m_lOrdinarySellNumberBelow100000 = 0;
	m_lOrdinarySellNumberBelow200000 = 0;
	m_lOrdinarySellNumberAbove200000 = 0;

	m_lCanceledBuyVolumeBelow5000 = 0;
	m_lCanceledBuyVolumeBelow10000 = 0;
	m_lCanceledBuyVolumeBelow20000 = 0;
	m_lCanceledBuyVolumeBelow50000 = 0;
	m_lCanceledBuyVolumeBelow100000 = 0;
	m_lCanceledBuyVolumeBelow200000 = 0;
	m_lCanceledBuyVolumeAbove200000 = 0;
	m_lCanceledSellVolumeBelow5000 = 0;
	m_lCanceledSellVolumeBelow10000 = 0;
	m_lCanceledSellVolumeBelow20000 = 0;
	m_lCanceledSellVolumeBelow50000 = 0;
	m_lCanceledSellVolumeBelow100000 = 0;
	m_lCanceledSellVolumeBelow200000 = 0;
	m_lCanceledSellVolumeAbove200000 = 0;
}