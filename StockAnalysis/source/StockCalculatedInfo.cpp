#include"StockCalculatedInfo.h"

#include"Accessory.h"

CStockCalculatedInfo::CStockCalculatedInfo() {
  Reset();
}

CStockCalculatedInfo::~CStockCalculatedInfo() {
}

void CStockCalculatedInfo::Reset() {
  m_lAttackBuyAmount = 0;
  m_lAttackSellAmount = 0;
  m_lCurrentVolume = 0;
  m_lAttackBuyVolume = 0;
  m_lCurrentAttackBuy = 0;
  m_lAttackSellVolume = 0;
  m_lCurrentAttackSell = 0;
  m_lStrongBuyVolume = 0;
  m_lCurrentStrongBuy = 0;
  m_lStrongSellVolume = 0;
  m_lCurrentStrongSell = 0;
  m_lUnknownVolume = 0;
  m_lCurrentUnknown = 0;
  m_lCancelBuyVolume = 0;
  m_lCancelSellVolume = 0;
  m_lTransactionNumber = 0;
  m_lTransactionNumberBelow5000 = 0;
  m_lTransactionNumberBelow50000 = 0;
  m_lTransactionNumberBelow200000 = 0;
  m_lTransactionNumberAbove200000 = 0;

  m_lOrdinaryBuyVolume = m_lAttackBuyBelow50000 = m_lAttackBuyBelow200000 = m_lAttackBuyAbove200000 = 0;
  m_lOrdinarySellVolume = m_lAttackSellBelow50000 = m_lAttackSellBelow200000 = m_lAttackSellAbove200000 = 0;
}

void CStockCalculatedInfo::StoreTodayInfo(CSetDayLineInfo& setDayLineInfo) {
  ASSERT(setDayLineInfo.IsOpen());
  setDayLineInfo.m_TransactionNumber = ConvertValueToString(m_lTransactionNumber);
  setDayLineInfo.m_TransactionNumberBelow5000 = ConvertValueToString(m_lTransactionNumberBelow5000);
  setDayLineInfo.m_TransactionNumberBelow50000 = ConvertValueToString(m_lTransactionNumberBelow50000);
  setDayLineInfo.m_TransactionNumberBelow200000 = ConvertValueToString(m_lTransactionNumberBelow200000);
  setDayLineInfo.m_TransactionNumberAbove200000 = ConvertValueToString(m_lTransactionNumberAbove200000);

  setDayLineInfo.m_CancelBuyVolume = ConvertValueToString(m_lCancelBuyVolume);
  setDayLineInfo.m_CancelSellVolume = ConvertValueToString(m_lCancelSellVolume);
  setDayLineInfo.m_AttackBuyVolume = ConvertValueToString(m_lAttackBuyVolume);
  setDayLineInfo.m_AttackSellVolume = ConvertValueToString(m_lAttackSellVolume);
  setDayLineInfo.m_StrongBuyVolume = ConvertValueToString(m_lStrongBuyVolume);
  setDayLineInfo.m_StrongSellVolume = ConvertValueToString(m_lStrongSellVolume);
  setDayLineInfo.m_UnknownVolume = ConvertValueToString(m_lUnknownVolume);
  setDayLineInfo.m_OrdinaryBuyVolume = ConvertValueToString(m_lOrdinaryBuyVolume);
  setDayLineInfo.m_OrdinarySellVolume = ConvertValueToString(m_lOrdinarySellVolume);
  setDayLineInfo.m_AttackBuyBelow50000 = ConvertValueToString(m_lAttackBuyBelow50000);
  setDayLineInfo.m_AttackBuyBelow200000 = ConvertValueToString(m_lAttackBuyBelow200000);
  setDayLineInfo.m_AttackBuyAbove200000 = ConvertValueToString(m_lAttackBuyAbove200000);
  setDayLineInfo.m_AttackSellBelow50000 = ConvertValueToString(m_lAttackSellBelow50000);
  setDayLineInfo.m_AttackSellBelow200000 = ConvertValueToString(m_lAttackSellBelow200000);
  setDayLineInfo.m_AttackSellAbove200000 = ConvertValueToString(m_lAttackSellAbove200000);
}

void CStockCalculatedInfo::StoreTempInfo(CSetDayLineToday& setDayLineToday) {
  ASSERT(setDayLineToday.IsOpen());
  setDayLineToday.m_TransactionNumber = ConvertValueToString(m_lTransactionNumber);
  setDayLineToday.m_TransactionNumberBelow5000 = ConvertValueToString(m_lTransactionNumberBelow5000);
  setDayLineToday.m_TransactionNumberBelow50000 = ConvertValueToString(m_lTransactionNumberBelow50000);
  setDayLineToday.m_TransactionNumberBelow200000 = ConvertValueToString(m_lTransactionNumberBelow200000);
  setDayLineToday.m_TransactionNumberAbove200000 = ConvertValueToString(m_lTransactionNumberAbove200000);
  setDayLineToday.m_CancelBuyVolume = ConvertValueToString(m_lCancelBuyVolume);
  setDayLineToday.m_CancelSellVolume = ConvertValueToString(m_lCancelSellVolume);
  setDayLineToday.m_AttackBuyVolume = ConvertValueToString(m_lAttackBuyVolume);
  setDayLineToday.m_AttackSellVolume = ConvertValueToString(m_lAttackSellVolume);
  setDayLineToday.m_StrongBuyVolume = ConvertValueToString(m_lStrongBuyVolume);
  setDayLineToday.m_StrongSellVolume = ConvertValueToString(m_lStrongSellVolume);
  setDayLineToday.m_UnknownVolume = ConvertValueToString(m_lUnknownVolume);
  setDayLineToday.m_OrdinaryBuyVolume = ConvertValueToString(m_lOrdinaryBuyVolume);
  setDayLineToday.m_OrdinarySellVolume = ConvertValueToString(m_lOrdinarySellVolume);
  setDayLineToday.m_AttackBuyBelow50000 = ConvertValueToString(m_lAttackBuyBelow50000);
  setDayLineToday.m_AttackBuyBelow200000 = ConvertValueToString(m_lAttackBuyBelow200000);
  setDayLineToday.m_AttackBuyAbove200000 = ConvertValueToString(m_lAttackBuyAbove200000);
  setDayLineToday.m_AttackSellBelow50000 = ConvertValueToString(m_lAttackSellBelow50000);
  setDayLineToday.m_AttackSellBelow200000 = ConvertValueToString(m_lAttackSellBelow200000);
  setDayLineToday.m_AttackSellAbove200000 = ConvertValueToString(m_lAttackSellAbove200000);
}

////////////////////////////////////////////////////////////////////////////
//
// 只有一处调用本函数，就是在系统初始化时装入之前计算出的信息（如果开市了的话），故而未知成交数量这项需要特殊处理
//
////////////////////////////////////////////////////////////////////////////
void CStockCalculatedInfo::LoadAndCalculateTempInfo(CSetDayLineToday& setDayLineToday) {
  // 要设置m_lUnknownVolume为记录集中的m_UnknownVolume - m_Volume，这是因为第一次计算时只是初始化系统。
  // 需要设置m_lUnknownVolume = pRTData->m_lVolume - setDayLineToday.m_Volume + setDayLineToday.m_UnknownVolume
  // 而第一次执行计算实时数据时，只是初始化系统环境，其中设置m_lUnknownVolume += pRTData->GetVolume
  // 故而此处这样计算。
  m_lUnknownVolume = atoll(setDayLineToday.m_UnknownVolume) - atoll(setDayLineToday.m_Volume);  // 需要如此设置m_lUnknownVolume

  m_lTransactionNumber = atol(setDayLineToday.m_TransactionNumber);
  m_lTransactionNumberBelow5000 = atol(setDayLineToday.m_TransactionNumberBelow5000);
  m_lTransactionNumberBelow50000 = atol(setDayLineToday.m_TransactionNumberBelow50000);
  m_lTransactionNumberBelow200000 = atol(setDayLineToday.m_TransactionNumberBelow200000);
  m_lTransactionNumberAbove200000 = atol(setDayLineToday.m_TransactionNumberAbove200000);
  m_lCancelBuyVolume = atoll(setDayLineToday.m_CancelBuyVolume);
  m_lCancelSellVolume = atoll(setDayLineToday.m_CancelSellVolume);
  m_lAttackBuyVolume = atoll(setDayLineToday.m_AttackBuyVolume);
  m_lAttackSellVolume = atoll(setDayLineToday.m_AttackSellVolume);
  m_lStrongBuyVolume = atoll(setDayLineToday.m_StrongBuyVolume);
  m_lStrongSellVolume = atoll(setDayLineToday.m_StrongSellVolume);
  m_lOrdinaryBuyVolume = atoll(setDayLineToday.m_OrdinaryBuyVolume);
  m_lOrdinarySellVolume = atoll(setDayLineToday.m_OrdinarySellVolume);
  m_lAttackBuyBelow50000 = atoll(setDayLineToday.m_AttackBuyBelow50000);
  m_lAttackBuyBelow200000 = atoll(setDayLineToday.m_AttackBuyBelow200000);
  m_lAttackBuyAbove200000 = atoll(setDayLineToday.m_AttackBuyAbove200000);
  m_lAttackSellBelow50000 = atoll(setDayLineToday.m_AttackSellBelow50000);
  m_lAttackSellBelow200000 = atoll(setDayLineToday.m_AttackSellBelow200000);
  m_lAttackSellAbove200000 = atoll(setDayLineToday.m_AttackSellAbove200000);
}