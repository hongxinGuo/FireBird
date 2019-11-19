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

void CStockCalculatedInfo::StoreTodayInfo(CSetDayLineInfo* psetDayLineInfo) {
  ASSERT(psetDayLineInfo->IsOpen());
  psetDayLineInfo->m_TransactionNumber = ConvertValueToString(m_lTransactionNumber);
  psetDayLineInfo->m_TransactionNumberBelow5000 = ConvertValueToString(m_lTransactionNumberBelow5000);
  psetDayLineInfo->m_TransactionNumberBelow50000 = ConvertValueToString(m_lTransactionNumberBelow50000);
  psetDayLineInfo->m_TransactionNumberBelow200000 = ConvertValueToString(m_lTransactionNumberBelow200000);
  psetDayLineInfo->m_TransactionNumberAbove200000 = ConvertValueToString(m_lTransactionNumberAbove200000);

  psetDayLineInfo->m_CancelBuyVolume = ConvertValueToString(m_lCancelBuyVolume);
  psetDayLineInfo->m_CancelSellVolume = ConvertValueToString(m_lCancelSellVolume);
  psetDayLineInfo->m_AttackBuyVolume = ConvertValueToString(m_lAttackBuyVolume);
  psetDayLineInfo->m_AttackSellVolume = ConvertValueToString(m_lAttackSellVolume);
  psetDayLineInfo->m_StrongBuyVolume = ConvertValueToString(m_lStrongBuyVolume);
  psetDayLineInfo->m_StrongSellVolume = ConvertValueToString(m_lStrongSellVolume);
  psetDayLineInfo->m_UnknownVolume = ConvertValueToString(m_lUnknownVolume);
  psetDayLineInfo->m_OrdinaryBuyVolume = ConvertValueToString(m_lOrdinaryBuyVolume);
  psetDayLineInfo->m_OrdinarySellVolume = ConvertValueToString(m_lOrdinarySellVolume);
  psetDayLineInfo->m_AttackBuyBelow50000 = ConvertValueToString(m_lAttackBuyBelow50000);
  psetDayLineInfo->m_AttackBuyBelow200000 = ConvertValueToString(m_lAttackBuyBelow200000);
  psetDayLineInfo->m_AttackBuyAbove200000 = ConvertValueToString(m_lAttackBuyAbove200000);
  psetDayLineInfo->m_AttackSellBelow50000 = ConvertValueToString(m_lAttackSellBelow50000);
  psetDayLineInfo->m_AttackSellBelow200000 = ConvertValueToString(m_lAttackSellBelow200000);
  psetDayLineInfo->m_AttackSellAbove200000 = ConvertValueToString(m_lAttackSellAbove200000);
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
// ֻ��һ�����ñ�������������ϵͳ��ʼ��ʱװ��֮ǰ���������Ϣ����������˵Ļ������ʶ�δ֪�ɽ�����������Ҫ���⴦��
//
////////////////////////////////////////////////////////////////////////////
void CStockCalculatedInfo::LoadAndCalculateTempInfo(CSetDayLineToday& setDayLineToday) {
  // Ҫ����m_lUnknownVolumeΪ��¼���е�m_UnknownVolume - m_Volume��������Ϊ��һ�μ���ʱֻ�ǳ�ʼ��ϵͳ��
  // ��Ҫ����m_lUnknownVolume = pRTData->m_lVolume - setDayLineToday.m_Volume + setDayLineToday.m_UnknownVolume
  // ����һ��ִ�м���ʵʱ����ʱ��ֻ�ǳ�ʼ��ϵͳ��������������m_lUnknownVolume += pRTData->GetVolume
  // �ʶ��˴��������㡣
  m_lUnknownVolume = atoll(setDayLineToday.m_UnknownVolume) - atoll(setDayLineToday.m_Volume);  // ��Ҫ�������m_lUnknownVolume

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