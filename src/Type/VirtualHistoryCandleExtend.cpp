#include"pch.h"

#include"ConvertToString.h"
#include"VirtualHistoryCandleExtend.h"

CVirtualHistoryCandleExtend::CVirtualHistoryCandleExtend() {
}

bool CVirtualHistoryCandleExtend::SaveExtendData(CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend) const {
	ASSERT(pVirtualSetHistoryCandleExtend->IsOpen());
	pVirtualSetHistoryCandleExtend->m_Date = m_lDate;
	pVirtualSetHistoryCandleExtend->m_Symbol = m_strStockSymbol.c_str();
	pVirtualSetHistoryCandleExtend->m_TransactionNumber = ConvertValueToCString(m_lTransactionNumber);
	pVirtualSetHistoryCandleExtend->m_TransactionNumberBelow5000 = ConvertValueToCString(m_lTransactionNumberBelow5000);
	pVirtualSetHistoryCandleExtend->m_TransactionNumberBelow50000 = ConvertValueToCString(m_lTransactionNumberBelow50000);
	pVirtualSetHistoryCandleExtend->m_TransactionNumberBelow200000 = ConvertValueToCString(m_lTransactionNumberBelow200000);
	pVirtualSetHistoryCandleExtend->m_TransactionNumberAbove200000 = ConvertValueToCString(m_lTransactionNumberAbove200000);

	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolume = ConvertValueToCString(m_lCanceledBuyVolume);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolume = ConvertValueToCString(m_lCanceledSellVolume);
	pVirtualSetHistoryCandleExtend->m_AttackBuyVolume = ConvertValueToCString(m_lAttackBuyVolume);
	pVirtualSetHistoryCandleExtend->m_AttackSellVolume = ConvertValueToCString(m_lAttackSellVolume);
	pVirtualSetHistoryCandleExtend->m_StrongBuyVolume = ConvertValueToCString(m_lStrongBuyVolume);
	pVirtualSetHistoryCandleExtend->m_StrongSellVolume = ConvertValueToCString(m_lStrongSellVolume);
	pVirtualSetHistoryCandleExtend->m_UnknownVolume = ConvertValueToCString(m_lUnknownVolume);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolume = ConvertValueToCString(m_lOrdinaryBuyVolume);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolume = ConvertValueToCString(m_lOrdinarySellVolume);
	pVirtualSetHistoryCandleExtend->m_AttackBuyBelow50000 = ConvertValueToCString(m_lAttackBuyBelow50000);
	pVirtualSetHistoryCandleExtend->m_AttackBuyBelow200000 = ConvertValueToCString(m_lAttackBuyBelow200000);
	pVirtualSetHistoryCandleExtend->m_AttackBuyAbove200000 = ConvertValueToCString(m_lAttackBuyAbove200000);
	pVirtualSetHistoryCandleExtend->m_AttackSellBelow50000 = ConvertValueToCString(m_lAttackSellBelow50000);
	pVirtualSetHistoryCandleExtend->m_AttackSellBelow200000 = ConvertValueToCString(m_lAttackSellBelow200000);
	pVirtualSetHistoryCandleExtend->m_AttackSellAbove200000 = ConvertValueToCString(m_lAttackSellAbove200000);

	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow5000 = ConvertValueToCString(m_lOrdinaryBuyVolumeBelow5000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow10000 = ConvertValueToCString(m_lOrdinaryBuyVolumeBelow10000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow20000 = ConvertValueToCString(m_lOrdinaryBuyVolumeBelow20000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow50000 = ConvertValueToCString(m_lOrdinaryBuyVolumeBelow50000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow100000 = ConvertValueToCString(m_lOrdinaryBuyVolumeBelow100000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeBelow200000 = ConvertValueToCString(m_lOrdinaryBuyVolumeBelow200000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyVolumeAbove200000 = ConvertValueToCString(m_lOrdinaryBuyVolumeAbove200000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow5000 = ConvertValueToCString(m_lOrdinarySellVolumeBelow5000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow10000 = ConvertValueToCString(m_lOrdinarySellVolumeBelow10000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow20000 = ConvertValueToCString(m_lOrdinarySellVolumeBelow20000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow50000 = ConvertValueToCString(m_lOrdinarySellVolumeBelow50000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow100000 = ConvertValueToCString(m_lOrdinarySellVolumeBelow100000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeBelow200000 = ConvertValueToCString(m_lOrdinarySellVolumeBelow200000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellVolumeAbove200000 = ConvertValueToCString(m_lOrdinarySellVolumeAbove200000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow5000 = ConvertValueToCString(m_lOrdinaryBuyNumberBelow5000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow10000 = ConvertValueToCString(m_lOrdinaryBuyNumberBelow10000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow20000 = ConvertValueToCString(m_lOrdinaryBuyNumberBelow20000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow50000 = ConvertValueToCString(m_lOrdinaryBuyNumberBelow50000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow100000 = ConvertValueToCString(m_lOrdinaryBuyNumberBelow100000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberBelow200000 = ConvertValueToCString(m_lOrdinaryBuyNumberBelow200000);
	pVirtualSetHistoryCandleExtend->m_OrdinaryBuyNumberAbove200000 = ConvertValueToCString(m_lOrdinaryBuyNumberAbove200000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow5000 = ConvertValueToCString(m_lOrdinarySellNumberBelow5000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow10000 = ConvertValueToCString(m_lOrdinarySellNumberBelow10000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow20000 = ConvertValueToCString(m_lOrdinarySellNumberBelow20000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow50000 = ConvertValueToCString(m_lOrdinarySellNumberBelow50000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow100000 = ConvertValueToCString(m_lOrdinarySellNumberBelow100000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberBelow200000 = ConvertValueToCString(m_lOrdinarySellNumberBelow200000);
	pVirtualSetHistoryCandleExtend->m_OrdinarySellNumberAbove200000 = ConvertValueToCString(m_lOrdinarySellNumberAbove200000);

	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow5000 = ConvertValueToCString(m_lCanceledBuyVolumeBelow5000);
	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow10000 = ConvertValueToCString(m_lCanceledBuyVolumeBelow10000);
	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow20000 = ConvertValueToCString(m_lCanceledBuyVolumeBelow20000);
	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow50000 = ConvertValueToCString(m_lCanceledBuyVolumeBelow50000);
	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow100000 = ConvertValueToCString(m_lCanceledBuyVolumeBelow100000);
	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeBelow200000 = ConvertValueToCString(m_lCanceledBuyVolumeBelow200000);
	pVirtualSetHistoryCandleExtend->m_CanceledBuyVolumeAbove200000 = ConvertValueToCString(m_lCanceledBuyVolumeAbove200000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow5000 = ConvertValueToCString(m_lCanceledSellVolumeBelow5000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow10000 = ConvertValueToCString(m_lCanceledSellVolumeBelow10000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow20000 = ConvertValueToCString(m_lCanceledSellVolumeBelow20000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow50000 = ConvertValueToCString(m_lCanceledSellVolumeBelow50000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow100000 = ConvertValueToCString(m_lCanceledSellVolumeBelow100000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeBelow200000 = ConvertValueToCString(m_lCanceledSellVolumeBelow200000);
	pVirtualSetHistoryCandleExtend->m_CanceledSellVolumeAbove200000 = ConvertValueToCString(m_lCanceledSellVolumeAbove200000);

	return true;
}

bool CVirtualHistoryCandleExtend::AppendExtendData(CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend) const {
	ASSERT(pVirtualSetHistoryCandleExtend->IsOpen());
	pVirtualSetHistoryCandleExtend->AddNew();
	SaveExtendData(pVirtualSetHistoryCandleExtend);
	pVirtualSetHistoryCandleExtend->Update();
	return true;
}

bool CVirtualHistoryCandleExtend::LoadExtendData(const CVirtualSetHistoryCandleExtend* pVirtualSetHistoryCandleExtend) {
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

bool CVirtualHistoryCandleExtend::Save(CVirtualSetHistoryCandle* pSet) const {
	ASSERT(pSet->IsOpen());
	pSet->m_Date = GetMarketDate();
	pSet->m_Exchange = GetExchange().c_str();
	pSet->m_Symbol = GetStockSymbol().c_str();
	pSet->m_DisplaySymbol = GetDisplaySymbol().c_str();
	pSet->m_LastClose = ConvertValueToCString(GetLastClose(), GetRatio());
	pSet->m_High = ConvertValueToCString(GetHigh(), GetRatio());
	pSet->m_Low = ConvertValueToCString(GetLow(), GetRatio());
	pSet->m_Open = ConvertValueToCString(GetOpen(), GetRatio());
	pSet->m_Close = ConvertValueToCString(GetClose(), GetRatio());
	pSet->m_Volume = ConvertValueToCString(GetVolume());
	pSet->m_Amount = ConvertValueToCString(GetAmount());
	pSet->m_UpAndDown = ConvertValueToCString(GetUpDown());
	pSet->m_UpDownRate = ConvertValueToCString(GetUpDownRate());
	pSet->m_ChangeHandRate = ConvertValueToCString(GetChangeHandRate());
	pSet->m_TotalValue = ConvertValueToCString(GetTotalValue());
	pSet->m_CurrentValue = ConvertValueToCString(GetCurrentValue());
	pSet->m_RS = ConvertValueToCString(GetRS());
	pSet->m_RSIndex = ConvertValueToCString(GetRSIndex());
	pSet->m_RSBackup = ConvertValueToCString(GetRSBackup());

	pSet->m_TransactionNumber = ConvertValueToCString(m_lTransactionNumber);
	pSet->m_TransactionNumberBelow5000 = ConvertValueToCString(m_lTransactionNumberBelow5000);
	pSet->m_TransactionNumberBelow50000 = ConvertValueToCString(m_lTransactionNumberBelow50000);
	pSet->m_TransactionNumberBelow200000 = ConvertValueToCString(m_lTransactionNumberBelow200000);
	pSet->m_TransactionNumberAbove200000 = ConvertValueToCString(m_lTransactionNumberAbove200000);

	pSet->m_CanceledBuyVolume = ConvertValueToCString(m_lCanceledBuyVolume);
	pSet->m_CanceledSellVolume = ConvertValueToCString(m_lCanceledSellVolume);
	pSet->m_AttackBuyVolume = ConvertValueToCString(m_lAttackBuyVolume);
	pSet->m_AttackSellVolume = ConvertValueToCString(m_lAttackSellVolume);
	pSet->m_StrongBuyVolume = ConvertValueToCString(m_lStrongBuyVolume);
	pSet->m_StrongSellVolume = ConvertValueToCString(m_lStrongSellVolume);
	pSet->m_UnknownVolume = ConvertValueToCString(m_lUnknownVolume);
	pSet->m_OrdinaryBuyVolume = ConvertValueToCString(m_lOrdinaryBuyVolume);
	pSet->m_OrdinarySellVolume = ConvertValueToCString(m_lOrdinarySellVolume);
	pSet->m_AttackBuyBelow50000 = ConvertValueToCString(m_lAttackBuyBelow50000);
	pSet->m_AttackBuyBelow200000 = ConvertValueToCString(m_lAttackBuyBelow200000);
	pSet->m_AttackBuyAbove200000 = ConvertValueToCString(m_lAttackBuyAbove200000);
	pSet->m_AttackSellBelow50000 = ConvertValueToCString(m_lAttackSellBelow50000);
	pSet->m_AttackSellBelow200000 = ConvertValueToCString(m_lAttackSellBelow200000);
	pSet->m_AttackSellAbove200000 = ConvertValueToCString(m_lAttackSellAbove200000);

	pSet->m_OrdinaryBuyVolumeBelow5000 = ConvertValueToCString(m_lOrdinaryBuyVolumeBelow5000);
	pSet->m_OrdinaryBuyVolumeBelow10000 = ConvertValueToCString(m_lOrdinaryBuyVolumeBelow10000);
	pSet->m_OrdinaryBuyVolumeBelow20000 = ConvertValueToCString(m_lOrdinaryBuyVolumeBelow20000);
	pSet->m_OrdinaryBuyVolumeBelow50000 = ConvertValueToCString(m_lOrdinaryBuyVolumeBelow50000);
	pSet->m_OrdinaryBuyVolumeBelow100000 = ConvertValueToCString(m_lOrdinaryBuyVolumeBelow100000);
	pSet->m_OrdinaryBuyVolumeBelow200000 = ConvertValueToCString(m_lOrdinaryBuyVolumeBelow200000);
	pSet->m_OrdinaryBuyVolumeAbove200000 = ConvertValueToCString(m_lOrdinaryBuyVolumeAbove200000);
	pSet->m_OrdinarySellVolumeBelow5000 = ConvertValueToCString(m_lOrdinarySellVolumeBelow5000);
	pSet->m_OrdinarySellVolumeBelow10000 = ConvertValueToCString(m_lOrdinarySellVolumeBelow10000);
	pSet->m_OrdinarySellVolumeBelow20000 = ConvertValueToCString(m_lOrdinarySellVolumeBelow20000);
	pSet->m_OrdinarySellVolumeBelow50000 = ConvertValueToCString(m_lOrdinarySellVolumeBelow50000);
	pSet->m_OrdinarySellVolumeBelow100000 = ConvertValueToCString(m_lOrdinarySellVolumeBelow100000);
	pSet->m_OrdinarySellVolumeBelow200000 = ConvertValueToCString(m_lOrdinarySellVolumeBelow200000);
	pSet->m_OrdinarySellVolumeAbove200000 = ConvertValueToCString(m_lOrdinarySellVolumeAbove200000);
	pSet->m_OrdinaryBuyNumberBelow5000 = ConvertValueToCString(m_lOrdinaryBuyNumberBelow5000);
	pSet->m_OrdinaryBuyNumberBelow10000 = ConvertValueToCString(m_lOrdinaryBuyNumberBelow10000);
	pSet->m_OrdinaryBuyNumberBelow20000 = ConvertValueToCString(m_lOrdinaryBuyNumberBelow20000);
	pSet->m_OrdinaryBuyNumberBelow50000 = ConvertValueToCString(m_lOrdinaryBuyNumberBelow50000);
	pSet->m_OrdinaryBuyNumberBelow100000 = ConvertValueToCString(m_lOrdinaryBuyNumberBelow100000);
	pSet->m_OrdinaryBuyNumberBelow200000 = ConvertValueToCString(m_lOrdinaryBuyNumberBelow200000);
	pSet->m_OrdinaryBuyNumberAbove200000 = ConvertValueToCString(m_lOrdinaryBuyNumberAbove200000);
	pSet->m_OrdinarySellNumberBelow5000 = ConvertValueToCString(m_lOrdinarySellNumberBelow5000);
	pSet->m_OrdinarySellNumberBelow10000 = ConvertValueToCString(m_lOrdinarySellNumberBelow10000);
	pSet->m_OrdinarySellNumberBelow20000 = ConvertValueToCString(m_lOrdinarySellNumberBelow20000);
	pSet->m_OrdinarySellNumberBelow50000 = ConvertValueToCString(m_lOrdinarySellNumberBelow50000);
	pSet->m_OrdinarySellNumberBelow100000 = ConvertValueToCString(m_lOrdinarySellNumberBelow100000);
	pSet->m_OrdinarySellNumberBelow200000 = ConvertValueToCString(m_lOrdinarySellNumberBelow200000);
	pSet->m_OrdinarySellNumberAbove200000 = ConvertValueToCString(m_lOrdinarySellNumberAbove200000);

	pSet->m_CanceledBuyVolumeBelow5000 = ConvertValueToCString(m_lCanceledBuyVolumeBelow5000);
	pSet->m_CanceledBuyVolumeBelow10000 = ConvertValueToCString(m_lCanceledBuyVolumeBelow10000);
	pSet->m_CanceledBuyVolumeBelow20000 = ConvertValueToCString(m_lCanceledBuyVolumeBelow20000);
	pSet->m_CanceledBuyVolumeBelow50000 = ConvertValueToCString(m_lCanceledBuyVolumeBelow50000);
	pSet->m_CanceledBuyVolumeBelow100000 = ConvertValueToCString(m_lCanceledBuyVolumeBelow100000);
	pSet->m_CanceledBuyVolumeBelow200000 = ConvertValueToCString(m_lCanceledBuyVolumeBelow200000);
	pSet->m_CanceledBuyVolumeAbove200000 = ConvertValueToCString(m_lCanceledBuyVolumeAbove200000);
	pSet->m_CanceledSellVolumeBelow5000 = ConvertValueToCString(m_lCanceledSellVolumeBelow5000);
	pSet->m_CanceledSellVolumeBelow10000 = ConvertValueToCString(m_lCanceledSellVolumeBelow10000);
	pSet->m_CanceledSellVolumeBelow20000 = ConvertValueToCString(m_lCanceledSellVolumeBelow20000);
	pSet->m_CanceledSellVolumeBelow50000 = ConvertValueToCString(m_lCanceledSellVolumeBelow50000);
	pSet->m_CanceledSellVolumeBelow100000 = ConvertValueToCString(m_lCanceledSellVolumeBelow100000);
	pSet->m_CanceledSellVolumeBelow200000 = ConvertValueToCString(m_lCanceledSellVolumeBelow200000);
	pSet->m_CanceledSellVolumeAbove200000 = ConvertValueToCString(m_lCanceledSellVolumeAbove200000);

	return true;
}

bool CVirtualHistoryCandleExtend::Append(CVirtualSetHistoryCandle* pSet) const {
	ASSERT(pSet->IsOpen());
	pSet->AddNew();
	Save(pSet);
	pSet->Update();
	return true;
}

bool CVirtualHistoryCandleExtend::Load(const CVirtualSetHistoryCandle* pSet) {
	ASSERT(pSet->IsOpen());
	m_lDate = pSet->m_Date;
	m_strExchange = pSet->m_Exchange;
	m_strStockSymbol = pSet->m_Symbol;
	CString str;
	str = pSet->m_DisplaySymbol;
	m_strDisplaySymbol = str.GetString();
	m_lLastClose = atof(pSet->m_LastClose) * GetRatio();
	m_lOpen = atof(pSet->m_Open) * GetRatio();
	m_lHigh = atof(pSet->m_High) * GetRatio();
	m_lLow = atof(pSet->m_Low) * GetRatio();
	m_lClose = atof(pSet->m_Close) * GetRatio();
	m_dUpDown = atof(pSet->m_UpAndDown);
	m_llVolume = atoll(pSet->m_Volume);
	m_llAmount = atoll(pSet->m_Amount);
	m_dUpDownRate = atof(pSet->m_UpDownRate);
	m_dChangeHandRate = atof(pSet->m_ChangeHandRate);
	m_llTotalValue = atoll(pSet->m_TotalValue);
	m_llCurrentValue = atoll(pSet->m_CurrentValue);
	m_dRS = atof(pSet->m_RS);
	m_dRSIndex = atof(pSet->m_RSIndex);
	m_dRSBackup = atof(pSet->m_RSBackup);

	m_lTransactionNumber = atol(pSet->m_TransactionNumber);
	m_lTransactionNumberBelow5000 = atol(pSet->m_TransactionNumberBelow5000);
	m_lTransactionNumberBelow50000 = atol(pSet->m_TransactionNumberBelow50000);
	m_lTransactionNumberBelow200000 = atol(pSet->m_TransactionNumberBelow200000);
	m_lTransactionNumberAbove200000 = atol(pSet->m_TransactionNumberAbove200000);
	m_lAttackBuyVolume = atol(pSet->m_AttackBuyVolume);
	m_lStrongBuyVolume = atol(pSet->m_StrongBuyVolume);
	m_lAttackSellVolume = atol(pSet->m_AttackSellVolume);
	m_lStrongSellVolume = atol(pSet->m_StrongSellVolume);
	m_lUnknownVolume = atol(pSet->m_UnknownVolume);
	m_lCanceledBuyVolume = atol(pSet->m_CanceledBuyVolume);
	m_lCanceledSellVolume = atol(pSet->m_CanceledSellVolume);
	m_lOrdinaryBuyVolume = atol(pSet->m_OrdinaryBuyVolume);
	m_lAttackBuyBelow50000 = atol(pSet->m_AttackBuyBelow50000);
	m_lAttackBuyBelow200000 = atol(pSet->m_AttackBuyBelow200000);
	m_lAttackBuyAbove200000 = atol(pSet->m_AttackBuyAbove200000);
	m_lOrdinarySellVolume = atol(pSet->m_OrdinarySellVolume);
	m_lAttackSellBelow50000 = atol(pSet->m_AttackSellBelow50000);
	m_lAttackSellBelow200000 = atol(pSet->m_AttackSellBelow200000);
	m_lAttackSellAbove200000 = atol(pSet->m_AttackSellAbove200000);

	m_lOrdinaryBuyVolumeBelow5000 = atoll(pSet->m_OrdinaryBuyVolumeBelow5000);
	m_lOrdinaryBuyVolumeBelow10000 = atoll(pSet->m_OrdinaryBuyVolumeBelow10000);
	m_lOrdinaryBuyVolumeBelow20000 = atoll(pSet->m_OrdinaryBuyVolumeBelow20000);
	m_lOrdinaryBuyVolumeBelow50000 = atoll(pSet->m_OrdinaryBuyVolumeBelow50000);
	m_lOrdinaryBuyVolumeBelow100000 = atoll(pSet->m_OrdinaryBuyVolumeBelow100000);
	m_lOrdinaryBuyVolumeBelow200000 = atoll(pSet->m_OrdinaryBuyVolumeBelow200000);
	m_lOrdinaryBuyVolumeAbove200000 = atoll(pSet->m_OrdinaryBuyVolumeAbove200000);
	m_lOrdinarySellVolumeBelow5000 = atoll(pSet->m_OrdinarySellVolumeBelow5000);
	m_lOrdinarySellVolumeBelow10000 = atoll(pSet->m_OrdinarySellVolumeBelow10000);
	m_lOrdinarySellVolumeBelow20000 = atoll(pSet->m_OrdinarySellVolumeBelow20000);
	m_lOrdinarySellVolumeBelow50000 = atoll(pSet->m_OrdinarySellVolumeBelow50000);
	m_lOrdinarySellVolumeBelow100000 = atoll(pSet->m_OrdinarySellVolumeBelow100000);
	m_lOrdinarySellVolumeBelow200000 = atoll(pSet->m_OrdinarySellVolumeBelow200000);
	m_lOrdinarySellVolumeAbove200000 = atoll(pSet->m_OrdinarySellVolumeAbove200000);

	m_lOrdinaryBuyNumberBelow5000 = atoll(pSet->m_OrdinaryBuyNumberBelow5000);
	m_lOrdinaryBuyNumberBelow10000 = atoll(pSet->m_OrdinaryBuyNumberBelow10000);
	m_lOrdinaryBuyNumberBelow20000 = atoll(pSet->m_OrdinaryBuyNumberBelow20000);
	m_lOrdinaryBuyNumberBelow50000 = atoll(pSet->m_OrdinaryBuyNumberBelow50000);
	m_lOrdinaryBuyNumberBelow100000 = atoll(pSet->m_OrdinaryBuyNumberBelow100000);
	m_lOrdinaryBuyNumberBelow200000 = atoll(pSet->m_OrdinaryBuyNumberBelow200000);
	m_lOrdinaryBuyNumberAbove200000 = atoll(pSet->m_OrdinaryBuyNumberAbove200000);
	m_lOrdinarySellNumberBelow5000 = atoll(pSet->m_OrdinarySellNumberBelow5000);
	m_lOrdinarySellNumberBelow10000 = atoll(pSet->m_OrdinarySellNumberBelow10000);
	m_lOrdinarySellNumberBelow20000 = atoll(pSet->m_OrdinarySellNumberBelow20000);
	m_lOrdinarySellNumberBelow50000 = atoll(pSet->m_OrdinarySellNumberBelow50000);
	m_lOrdinarySellNumberBelow100000 = atoll(pSet->m_OrdinarySellNumberBelow100000);
	m_lOrdinarySellNumberBelow200000 = atoll(pSet->m_OrdinarySellNumberBelow200000);
	m_lOrdinarySellNumberAbove200000 = atoll(pSet->m_OrdinarySellNumberAbove200000);

	m_lCanceledBuyVolumeBelow5000 = atoll(pSet->m_CanceledBuyVolumeBelow5000);
	m_lCanceledBuyVolumeBelow10000 = atoll(pSet->m_CanceledBuyVolumeBelow10000);
	m_lCanceledBuyVolumeBelow20000 = atoll(pSet->m_CanceledBuyVolumeBelow20000);
	m_lCanceledBuyVolumeBelow50000 = atoll(pSet->m_CanceledBuyVolumeBelow50000);
	m_lCanceledBuyVolumeBelow100000 = atoll(pSet->m_CanceledBuyVolumeBelow100000);
	m_lCanceledBuyVolumeBelow200000 = atoll(pSet->m_CanceledBuyVolumeBelow200000);
	m_lCanceledBuyVolumeAbove200000 = atoll(pSet->m_CanceledBuyVolumeAbove200000);
	m_lCanceledSellVolumeBelow5000 = atoll(pSet->m_CanceledSellVolumeBelow5000);
	m_lCanceledSellVolumeBelow10000 = atoll(pSet->m_CanceledSellVolumeBelow10000);
	m_lCanceledSellVolumeBelow20000 = atoll(pSet->m_CanceledSellVolumeBelow20000);
	m_lCanceledSellVolumeBelow50000 = atoll(pSet->m_CanceledSellVolumeBelow50000);
	m_lCanceledSellVolumeBelow100000 = atoll(pSet->m_CanceledSellVolumeBelow100000);
	m_lCanceledSellVolumeBelow200000 = atoll(pSet->m_CanceledSellVolumeBelow200000);
	m_lCanceledSellVolumeAbove200000 = atoll(pSet->m_CanceledSellVolumeAbove200000);

	CalculateRSLogarithm1(m_dRS);

	return true;
}
