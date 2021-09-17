#include"pch.h"

#include"globedef.h"
#include"ChinaStockHistoryCandle.h"

CChinaStockHistoryCandle::CChinaStockHistoryCandle() : CVirtualHistoryCandle() {
	Reset();
}

void CChinaStockHistoryCandle::Reset(void) {
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
	m_lOrdinaryBuyVolume = m_lAttackBuyBelow50000 = m_lAttackBuyBelow200000 = m_lAttackBuyAbove200000 = 0;
	m_lOrdinarySellVolume = m_lAttackSellBelow50000 = m_lAttackSellBelow200000 = m_lAttackSellAbove200000 = 0;

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

bool CChinaStockHistoryCandle::SaveExtendData(CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend) {
	ASSERT(pVirtualSetHistoryCandleExtend->IsOpen());
	pVirtualSetHistoryCandleExtend->m_Date = m_lDate;
	pVirtualSetHistoryCandleExtend->m_Symbol = m_strStockSymbol;
	pVirtualSetHistoryCandleExtend->m_TransactionNumber = ConvertValueToString(m_lTransactionNumber);
	pVirtualSetHistoryCandleExtend->m_TransactionNumberBelow5000 = ConvertValueToString(m_lTransactionNumberBelow5000);
	pVirtualSetHistoryCandleExtend->m_TransactionNumberBelow50000 = ConvertValueToString(m_lTransactionNumberBelow50000);
	pVirtualSetHistoryCandleExtend->m_TransactionNumberBelow200000 = ConvertValueToString(m_lTransactionNumberBelow200000);
	pVirtualSetHistoryCandleExtend->m_TransactionNumberAbove200000 = ConvertValueToString(m_lTransactionNumberAbove200000);

	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolume = ConvertValueToString(m_lCanceledBuyVolume);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolume = ConvertValueToString(m_lCanceledSellVolume);
	pVirtualSetHistoryCandleExtend->m_AttackBuyVolume = ConvertValueToString(m_lAttackBuyVolume);
	pVirtualSetHistoryCandleExtend->m_AttackSellVolume = ConvertValueToString(m_lAttackSellVolume);
	pVirtualSetHistoryCandleExtend->m_StrongBuyVolume = ConvertValueToString(m_lStrongBuyVolume);
	pVirtualSetHistoryCandleExtend->m_StrongSellVolume = ConvertValueToString(m_lStrongSellVolume);
	pVirtualSetHistoryCandleExtend->m_UnknownVolume = ConvertValueToString(m_lUnknownVolume);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolume = ConvertValueToString(m_lOrdinaryBuyVolume);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolume = ConvertValueToString(m_lOrdinarySellVolume);
	pVirtualSetHistoryCandleExtend->m_AttackBuyBelow50000 = ConvertValueToString(m_lAttackBuyBelow50000);
	pVirtualSetHistoryCandleExtend->m_AttackBuyBelow200000 = ConvertValueToString(m_lAttackBuyBelow200000);
	pVirtualSetHistoryCandleExtend->m_AttackBuyAbove200000 = ConvertValueToString(m_lAttackBuyAbove200000);
	pVirtualSetHistoryCandleExtend->m_AttackSellBelow50000 = ConvertValueToString(m_lAttackSellBelow50000);
	pVirtualSetHistoryCandleExtend->m_AttackSellBelow200000 = ConvertValueToString(m_lAttackSellBelow200000);
	pVirtualSetHistoryCandleExtend->m_AttackSellAbove200000 = ConvertValueToString(m_lAttackSellAbove200000);

	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow5000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow5000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow10000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow10000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow20000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow20000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow50000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow50000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow100000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow100000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow200000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow200000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeAbove200000 = ConvertValueToString(m_lOrdinaryBuyVolumeAbove200000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow5000 = ConvertValueToString(m_lOrdinarySellVolumeBelow5000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow10000 = ConvertValueToString(m_lOrdinarySellVolumeBelow10000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow20000 = ConvertValueToString(m_lOrdinarySellVolumeBelow20000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow50000 = ConvertValueToString(m_lOrdinarySellVolumeBelow50000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow100000 = ConvertValueToString(m_lOrdinarySellVolumeBelow100000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow200000 = ConvertValueToString(m_lOrdinarySellVolumeBelow200000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeAbove200000 = ConvertValueToString(m_lOrdinarySellVolumeAbove200000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow5000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow5000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow10000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow10000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow20000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow20000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow50000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow50000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow100000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow100000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow200000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow200000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberAbove200000 = ConvertValueToString(m_lOrdinaryBuyNumberAbove200000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow5000 = ConvertValueToString(m_lOrdinarySellNumberBelow5000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow10000 = ConvertValueToString(m_lOrdinarySellNumberBelow10000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow20000 = ConvertValueToString(m_lOrdinarySellNumberBelow20000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow50000 = ConvertValueToString(m_lOrdinarySellNumberBelow50000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow100000 = ConvertValueToString(m_lOrdinarySellNumberBelow100000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow200000 = ConvertValueToString(m_lOrdinarySellNumberBelow200000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberAbove200000 = ConvertValueToString(m_lOrdinarySellNumberAbove200000);

	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow5000 = ConvertValueToString(m_lCanceledBuyVolumeBelow5000);
	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow10000 = ConvertValueToString(m_lCanceledBuyVolumeBelow10000);
	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow20000 = ConvertValueToString(m_lCanceledBuyVolumeBelow20000);
	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow50000 = ConvertValueToString(m_lCanceledBuyVolumeBelow50000);
	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow100000 = ConvertValueToString(m_lCanceledBuyVolumeBelow100000);
	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow200000 = ConvertValueToString(m_lCanceledBuyVolumeBelow200000);
	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeAbove200000 = ConvertValueToString(m_lCanceledBuyVolumeAbove200000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow5000 = ConvertValueToString(m_lCanceledSellVolumeBelow5000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow10000 = ConvertValueToString(m_lCanceledSellVolumeBelow10000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow20000 = ConvertValueToString(m_lCanceledSellVolumeBelow20000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow50000 = ConvertValueToString(m_lCanceledSellVolumeBelow50000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow100000 = ConvertValueToString(m_lCanceledSellVolumeBelow100000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow200000 = ConvertValueToString(m_lCanceledSellVolumeBelow200000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeAbove200000 = ConvertValueToString(m_lCanceledSellVolumeAbove200000);

	return true;
}

bool CChinaStockHistoryCandle::AppendExtendData(CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend) {
	ASSERT(pVirtualSetHistoryCandleExtend->IsOpen());
	pVirtualSetHistoryCandleExtend->AddNew();
	SaveExtendData(pVirtualSetHistoryCandleExtend);
	pVirtualSetHistoryCandleExtend->Update();
	return true;
}

bool CChinaStockHistoryCandle::LoadExtendData(CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend) {
	ASSERT(pVirtualSetHistoryCandleExtend->IsOpen());
	m_lTransactionNumber = atol(pVirtualSetHistoryCandleExtend->m_TransactionNumber);
	m_lTransactionNumberBelow5000 = atol(pVirtualSetHistoryCandleExtend->m_TransactionNumberBelow5000);
	m_lTransactionNumberBelow50000 = atol(pVirtualSetHistoryCandleExtend->m_TransactionNumberBelow50000);
	m_lTransactionNumberBelow200000 = atol(pVirtualSetHistoryCandleExtend->m_TransactionNumberBelow200000);
	m_lTransactionNumberAbove200000 = atol(pVirtualSetHistoryCandleExtend->m_TransactionNumberAbove200000);
	m_lAttackBuyVolume = atol(pVirtualSetHistoryCandleExtend->m_AttackBuyVolume);
	m_lStrongBuyVolume = atol(pVirtualSetHistoryCandleExtend->m_StrongBuyVolume);
	m_lAttackSellVolume = atol(pVirtualSetHistoryCandleExtend->m_AttackSellVolume);
	m_lStrongSellVolume = atol(pVirtualSetHistoryCandleExtend->m_StrongSellVolume);
	m_lUnknownVolume = atol(pVirtualSetHistoryCandleExtend->m_UnknownVolume);
	m_lCanceledBuyVolume = atol(pVirtualSetHistoryCandleExtend->m_CanceledBuyVolume);
	m_lCanceledSellVolume = atol(pVirtualSetHistoryCandleExtend->m_CanceledSellVolume);
	m_lOrdinaryBuyVolume = atol(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolume);
	m_lAttackBuyBelow50000 = atol(pVirtualSetHistoryCandleExtend->m_AttackBuyBelow50000);
	m_lAttackBuyBelow200000 = atol(pVirtualSetHistoryCandleExtend->m_AttackBuyBelow200000);
	m_lAttackBuyAbove200000 = atol(pVirtualSetHistoryCandleExtend->m_AttackBuyAbove200000);
	m_lOrdinarySellVolume = atol(pVirtualSetHistoryCandleExtend->m_OrdinarySellVolume);
	m_lAttackSellBelow50000 = atol(pVirtualSetHistoryCandleExtend->m_AttackSellBelow50000);
	m_lAttackSellBelow200000 = atol(pVirtualSetHistoryCandleExtend->m_AttackSellBelow200000);
	m_lAttackSellAbove200000 = atol(pVirtualSetHistoryCandleExtend->m_AttackSellAbove200000);

	m_lOrdinaryBuyVolumeBelow5000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow5000);
	m_lOrdinaryBuyVolumeBelow10000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow10000);
	m_lOrdinaryBuyVolumeBelow20000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow20000);
	m_lOrdinaryBuyVolumeBelow50000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow50000);
	m_lOrdinaryBuyVolumeBelow100000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow100000);
	m_lOrdinaryBuyVolumeBelow200000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow200000);
	m_lOrdinaryBuyVolumeAbove200000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeAbove200000);
	m_lOrdinarySellVolumeBelow5000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow5000);
	m_lOrdinarySellVolumeBelow10000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow10000);
	m_lOrdinarySellVolumeBelow20000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow20000);
	m_lOrdinarySellVolumeBelow50000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow50000);
	m_lOrdinarySellVolumeBelow100000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow100000);
	m_lOrdinarySellVolumeBelow200000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow200000);
	m_lOrdinarySellVolumeAbove200000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeAbove200000);

	m_lOrdinaryBuyNumberBelow5000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow5000);
	m_lOrdinaryBuyNumberBelow10000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow10000);
	m_lOrdinaryBuyNumberBelow20000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow20000);
	m_lOrdinaryBuyNumberBelow50000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow50000);
	m_lOrdinaryBuyNumberBelow100000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow100000);
	m_lOrdinaryBuyNumberBelow200000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow200000);
	m_lOrdinaryBuyNumberAbove200000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberAbove200000);
	m_lOrdinarySellNumberBelow5000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow5000);
	m_lOrdinarySellNumberBelow10000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow10000);
	m_lOrdinarySellNumberBelow20000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow20000);
	m_lOrdinarySellNumberBelow50000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow50000);
	m_lOrdinarySellNumberBelow100000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow100000);
	m_lOrdinarySellNumberBelow200000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow200000);
	m_lOrdinarySellNumberAbove200000 = atoll(pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberAbove200000);

	m_lCanceledBuyVolumeBelow5000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow5000);
	m_lCanceledBuyVolumeBelow10000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow10000);
	m_lCanceledBuyVolumeBelow20000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow20000);
	m_lCanceledBuyVolumeBelow50000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow50000);
	m_lCanceledBuyVolumeBelow100000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow100000);
	m_lCanceledBuyVolumeBelow200000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow200000);
	m_lCanceledBuyVolumeAbove200000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeAbove200000);
	m_lCanceledSellVolumeBelow5000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow5000);
	m_lCanceledSellVolumeBelow10000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow10000);
	m_lCanceledSellVolumeBelow20000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow20000);
	m_lCanceledSellVolumeBelow50000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow50000);
	m_lCanceledSellVolumeBelow100000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow100000);
	m_lCanceledSellVolumeBelow200000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow200000);
	m_lCanceledSellVolumeAbove200000 = atoll(pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeAbove200000);

	return true;
}