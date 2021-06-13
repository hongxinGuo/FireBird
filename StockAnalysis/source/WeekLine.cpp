#include"pch.h"

#include"globedef.h"
#include "WeekLine.h"

CWeekLine::CWeekLine() : CChinaStockHistoryData() {
  Reset();
}

void CWeekLine::Reset(void) {
  CChinaStockHistoryData::Reset();
}

bool CWeekLine::AppendCurrentWeekData(CCurrentWeekLineInfo* psetCurrentWeekLineInfo) {
  ASSERT(psetCurrentWeekLineInfo->IsOpen());
  psetCurrentWeekLineInfo->AddNew();
  psetCurrentWeekLineInfo->m_Date = GetFormatedMarketDate();
  psetCurrentWeekLineInfo->m_Symbol = GetStockSymbol();
  psetCurrentWeekLineInfo->m_StockName = GetStockName();
  psetCurrentWeekLineInfo->m_LastClose = ConvertValueToString(GetLastClose(), 1000);
  psetCurrentWeekLineInfo->m_High = ConvertValueToString(GetHigh(), 1000);
  psetCurrentWeekLineInfo->m_Low = ConvertValueToString(GetLow(), 1000);
  psetCurrentWeekLineInfo->m_Open = ConvertValueToString(GetOpen(), 1000);
  psetCurrentWeekLineInfo->m_Close = ConvertValueToString(GetClose(), 1000);
  psetCurrentWeekLineInfo->m_Volume = ConvertValueToString(GetVolume());
  psetCurrentWeekLineInfo->m_Amount = ConvertValueToString(GetAmount());
  psetCurrentWeekLineInfo->m_UpAndDown = ConvertValueToString(GetUpDown());
  psetCurrentWeekLineInfo->m_UpDownRate = ConvertValueToString(GetUpDownRate());
  psetCurrentWeekLineInfo->m_ChangeHandRate = ConvertValueToString(GetChangeHandRate());
  psetCurrentWeekLineInfo->m_TotalValue = ConvertValueToString(GetTotalValue());
  psetCurrentWeekLineInfo->m_CurrentValue = ConvertValueToString(GetCurrentValue());
  psetCurrentWeekLineInfo->m_RS = ConvertValueToString(GetRS());
  psetCurrentWeekLineInfo->m_RSIndex = ConvertValueToString(GetRSIndex());
  psetCurrentWeekLineInfo->m_RSBackup = ConvertValueToString(GetRSBackup());

  psetCurrentWeekLineInfo->m_TransactionNumber = ConvertValueToString(m_lTransactionNumber);
  psetCurrentWeekLineInfo->m_TransactionNumberBelow5000 = ConvertValueToString(m_lTransactionNumberBelow5000);
  psetCurrentWeekLineInfo->m_TransactionNumberBelow50000 = ConvertValueToString(m_lTransactionNumberBelow50000);
  psetCurrentWeekLineInfo->m_TransactionNumberBelow200000 = ConvertValueToString(m_lTransactionNumberBelow200000);
  psetCurrentWeekLineInfo->m_TransactionNumberAbove200000 = ConvertValueToString(m_lTransactionNumberAbove200000);

  psetCurrentWeekLineInfo->m_CanceledBuyVolume = ConvertValueToString(m_lCanceledBuyVolume);
  psetCurrentWeekLineInfo->m_CanceledSellVolume = ConvertValueToString(m_lCanceledSellVolume);
  psetCurrentWeekLineInfo->m_AttackBuyVolume = ConvertValueToString(m_lAttackBuyVolume);
  psetCurrentWeekLineInfo->m_AttackSellVolume = ConvertValueToString(m_lAttackSellVolume);
  psetCurrentWeekLineInfo->m_StrongBuyVolume = ConvertValueToString(m_lStrongBuyVolume);
  psetCurrentWeekLineInfo->m_StrongSellVolume = ConvertValueToString(m_lStrongSellVolume);
  psetCurrentWeekLineInfo->m_UnknownVolume = ConvertValueToString(m_lUnknownVolume);
  psetCurrentWeekLineInfo->m_OrdinaryBuyVolume = ConvertValueToString(m_lOrdinaryBuyVolume);
  psetCurrentWeekLineInfo->m_OrdinarySellVolume = ConvertValueToString(m_lOrdinarySellVolume);
  psetCurrentWeekLineInfo->m_AttackBuyBelow50000 = ConvertValueToString(m_lAttackBuyBelow50000);
  psetCurrentWeekLineInfo->m_AttackBuyBelow200000 = ConvertValueToString(m_lAttackBuyBelow200000);
  psetCurrentWeekLineInfo->m_AttackBuyAbove200000 = ConvertValueToString(m_lAttackBuyAbove200000);
  psetCurrentWeekLineInfo->m_AttackSellBelow50000 = ConvertValueToString(m_lAttackSellBelow50000);
  psetCurrentWeekLineInfo->m_AttackSellBelow200000 = ConvertValueToString(m_lAttackSellBelow200000);
  psetCurrentWeekLineInfo->m_AttackSellAbove200000 = ConvertValueToString(m_lAttackSellAbove200000);

  psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeBelow5000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow5000);
  psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeBelow10000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow10000);
  psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeBelow20000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow20000);
  psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeBelow50000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow50000);
  psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeBelow100000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow100000);
  psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeBelow200000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow200000);
  psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeAbove200000 = ConvertValueToString(m_lOrdinaryBuyVolumeAbove200000);
  psetCurrentWeekLineInfo->m_OrdinarySellVolumeBelow5000 = ConvertValueToString(m_lOrdinarySellVolumeBelow5000);
  psetCurrentWeekLineInfo->m_OrdinarySellVolumeBelow10000 = ConvertValueToString(m_lOrdinarySellVolumeBelow10000);
  psetCurrentWeekLineInfo->m_OrdinarySellVolumeBelow20000 = ConvertValueToString(m_lOrdinarySellVolumeBelow20000);
  psetCurrentWeekLineInfo->m_OrdinarySellVolumeBelow50000 = ConvertValueToString(m_lOrdinarySellVolumeBelow50000);
  psetCurrentWeekLineInfo->m_OrdinarySellVolumeBelow100000 = ConvertValueToString(m_lOrdinarySellVolumeBelow100000);
  psetCurrentWeekLineInfo->m_OrdinarySellVolumeBelow200000 = ConvertValueToString(m_lOrdinarySellVolumeBelow200000);
  psetCurrentWeekLineInfo->m_OrdinarySellVolumeAbove200000 = ConvertValueToString(m_lOrdinarySellVolumeAbove200000);
  psetCurrentWeekLineInfo->m_OrdinaryBuyNumberBelow5000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow5000);
  psetCurrentWeekLineInfo->m_OrdinaryBuyNumberBelow10000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow10000);
  psetCurrentWeekLineInfo->m_OrdinaryBuyNumberBelow20000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow20000);
  psetCurrentWeekLineInfo->m_OrdinaryBuyNumberBelow50000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow50000);
  psetCurrentWeekLineInfo->m_OrdinaryBuyNumberBelow100000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow100000);
  psetCurrentWeekLineInfo->m_OrdinaryBuyNumberBelow200000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow200000);
  psetCurrentWeekLineInfo->m_OrdinaryBuyNumberAbove200000 = ConvertValueToString(m_lOrdinaryBuyNumberAbove200000);
  psetCurrentWeekLineInfo->m_OrdinarySellNumberBelow5000 = ConvertValueToString(m_lOrdinarySellNumberBelow5000);
  psetCurrentWeekLineInfo->m_OrdinarySellNumberBelow10000 = ConvertValueToString(m_lOrdinarySellNumberBelow10000);
  psetCurrentWeekLineInfo->m_OrdinarySellNumberBelow20000 = ConvertValueToString(m_lOrdinarySellNumberBelow20000);
  psetCurrentWeekLineInfo->m_OrdinarySellNumberBelow50000 = ConvertValueToString(m_lOrdinarySellNumberBelow50000);
  psetCurrentWeekLineInfo->m_OrdinarySellNumberBelow100000 = ConvertValueToString(m_lOrdinarySellNumberBelow100000);
  psetCurrentWeekLineInfo->m_OrdinarySellNumberBelow200000 = ConvertValueToString(m_lOrdinarySellNumberBelow200000);
  psetCurrentWeekLineInfo->m_OrdinarySellNumberAbove200000 = ConvertValueToString(m_lOrdinarySellNumberAbove200000);

  psetCurrentWeekLineInfo->m_CanceledBuyVolumeBelow5000 = ConvertValueToString(m_lCanceledBuyVolumeBelow5000);
  psetCurrentWeekLineInfo->m_CanceledBuyVolumeBelow10000 = ConvertValueToString(m_lCanceledBuyVolumeBelow10000);
  psetCurrentWeekLineInfo->m_CanceledBuyVolumeBelow20000 = ConvertValueToString(m_lCanceledBuyVolumeBelow20000);
  psetCurrentWeekLineInfo->m_CanceledBuyVolumeBelow50000 = ConvertValueToString(m_lCanceledBuyVolumeBelow50000);
  psetCurrentWeekLineInfo->m_CanceledBuyVolumeBelow100000 = ConvertValueToString(m_lCanceledBuyVolumeBelow100000);
  psetCurrentWeekLineInfo->m_CanceledBuyVolumeBelow200000 = ConvertValueToString(m_lCanceledBuyVolumeBelow200000);
  psetCurrentWeekLineInfo->m_CanceledBuyVolumeAbove200000 = ConvertValueToString(m_lCanceledBuyVolumeAbove200000);
  psetCurrentWeekLineInfo->m_CanceledSellVolumeBelow5000 = ConvertValueToString(m_lCanceledSellVolumeBelow5000);
  psetCurrentWeekLineInfo->m_CanceledSellVolumeBelow10000 = ConvertValueToString(m_lCanceledSellVolumeBelow10000);
  psetCurrentWeekLineInfo->m_CanceledSellVolumeBelow20000 = ConvertValueToString(m_lCanceledSellVolumeBelow20000);
  psetCurrentWeekLineInfo->m_CanceledSellVolumeBelow50000 = ConvertValueToString(m_lCanceledSellVolumeBelow50000);
  psetCurrentWeekLineInfo->m_CanceledSellVolumeBelow100000 = ConvertValueToString(m_lCanceledSellVolumeBelow100000);
  psetCurrentWeekLineInfo->m_CanceledSellVolumeBelow200000 = ConvertValueToString(m_lCanceledSellVolumeBelow200000);
  psetCurrentWeekLineInfo->m_CanceledSellVolumeAbove200000 = ConvertValueToString(m_lCanceledSellVolumeAbove200000);
  psetCurrentWeekLineInfo->Update();

  return true;
}

bool CWeekLine::SaveBasicData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo) {
  ASSERT(psetWeekLineBasicInfo->IsOpen());

  psetWeekLineBasicInfo->m_Date = GetFormatedMarketDate();
  psetWeekLineBasicInfo->m_Symbol = GetStockSymbol();
  psetWeekLineBasicInfo->m_StockName = GetStockName();
  psetWeekLineBasicInfo->m_LastClose = ConvertValueToString(GetLastClose(), 1000);
  psetWeekLineBasicInfo->m_High = ConvertValueToString(GetHigh(), 1000);
  psetWeekLineBasicInfo->m_Low = ConvertValueToString(GetLow(), 1000);
  psetWeekLineBasicInfo->m_Open = ConvertValueToString(GetOpen(), 1000);
  psetWeekLineBasicInfo->m_Close = ConvertValueToString(GetClose(), 1000);
  psetWeekLineBasicInfo->m_Volume = ConvertValueToString(GetVolume());
  psetWeekLineBasicInfo->m_Amount = ConvertValueToString(GetAmount());
  psetWeekLineBasicInfo->m_UpAndDown = ConvertValueToString(GetUpDown());
  psetWeekLineBasicInfo->m_UpDownRate = ConvertValueToString(GetUpDownRate());
  psetWeekLineBasicInfo->m_ChangeHandRate = ConvertValueToString(GetChangeHandRate());
  psetWeekLineBasicInfo->m_TotalValue = ConvertValueToString(GetTotalValue());
  psetWeekLineBasicInfo->m_CurrentValue = ConvertValueToString(GetCurrentValue());
  psetWeekLineBasicInfo->m_RS = ConvertValueToString(GetRS());
  psetWeekLineBasicInfo->m_RSIndex = ConvertValueToString(GetRSIndex());
  psetWeekLineBasicInfo->m_RSBackup = ConvertValueToString(GetRSBackup());

  return true;
}

bool CWeekLine::AppendBasicData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo) {
  ASSERT(psetWeekLineBasicInfo->IsOpen());
  psetWeekLineBasicInfo->AddNew();
  SaveBasicData(psetWeekLineBasicInfo);
  psetWeekLineBasicInfo->Update();

  return true;
}

bool CWeekLine::SaveExtendData(CSetWeekLineExtendInfo* psetWeekLineExtendInfo) {
  ASSERT(psetWeekLineExtendInfo->IsOpen());
  psetWeekLineExtendInfo->m_Date = m_lDate;
  psetWeekLineExtendInfo->m_Symbol = m_strStockSymbol;
  psetWeekLineExtendInfo->m_TransactionNumber = ConvertValueToString(m_lTransactionNumber);
  psetWeekLineExtendInfo->m_TransactionNumberBelow5000 = ConvertValueToString(m_lTransactionNumberBelow5000);
  psetWeekLineExtendInfo->m_TransactionNumberBelow50000 = ConvertValueToString(m_lTransactionNumberBelow50000);
  psetWeekLineExtendInfo->m_TransactionNumberBelow200000 = ConvertValueToString(m_lTransactionNumberBelow200000);
  psetWeekLineExtendInfo->m_TransactionNumberAbove200000 = ConvertValueToString(m_lTransactionNumberAbove200000);

  psetWeekLineExtendInfo->m_CanceledBuyVolume = ConvertValueToString(m_lCanceledBuyVolume);
  psetWeekLineExtendInfo->m_CanceledSellVolume = ConvertValueToString(m_lCanceledSellVolume);
  psetWeekLineExtendInfo->m_AttackBuyVolume = ConvertValueToString(m_lAttackBuyVolume);
  psetWeekLineExtendInfo->m_AttackSellVolume = ConvertValueToString(m_lAttackSellVolume);
  psetWeekLineExtendInfo->m_StrongBuyVolume = ConvertValueToString(m_lStrongBuyVolume);
  psetWeekLineExtendInfo->m_StrongSellVolume = ConvertValueToString(m_lStrongSellVolume);
  psetWeekLineExtendInfo->m_UnknownVolume = ConvertValueToString(m_lUnknownVolume);
  psetWeekLineExtendInfo->m_OrdinaryBuyVolume = ConvertValueToString(m_lOrdinaryBuyVolume);
  psetWeekLineExtendInfo->m_OrdinarySellVolume = ConvertValueToString(m_lOrdinarySellVolume);
  psetWeekLineExtendInfo->m_AttackBuyBelow50000 = ConvertValueToString(m_lAttackBuyBelow50000);
  psetWeekLineExtendInfo->m_AttackBuyBelow200000 = ConvertValueToString(m_lAttackBuyBelow200000);
  psetWeekLineExtendInfo->m_AttackBuyAbove200000 = ConvertValueToString(m_lAttackBuyAbove200000);
  psetWeekLineExtendInfo->m_AttackSellBelow50000 = ConvertValueToString(m_lAttackSellBelow50000);
  psetWeekLineExtendInfo->m_AttackSellBelow200000 = ConvertValueToString(m_lAttackSellBelow200000);
  psetWeekLineExtendInfo->m_AttackSellAbove200000 = ConvertValueToString(m_lAttackSellAbove200000);

  psetWeekLineExtendInfo->m_OrdinaryBuyVolumeBelow5000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow5000);
  psetWeekLineExtendInfo->m_OrdinaryBuyVolumeBelow10000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow10000);
  psetWeekLineExtendInfo->m_OrdinaryBuyVolumeBelow20000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow20000);
  psetWeekLineExtendInfo->m_OrdinaryBuyVolumeBelow50000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow50000);
  psetWeekLineExtendInfo->m_OrdinaryBuyVolumeBelow100000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow100000);
  psetWeekLineExtendInfo->m_OrdinaryBuyVolumeBelow200000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow200000);
  psetWeekLineExtendInfo->m_OrdinaryBuyVolumeAbove200000 = ConvertValueToString(m_lOrdinaryBuyVolumeAbove200000);
  psetWeekLineExtendInfo->m_OrdinarySellVolumeBelow5000 = ConvertValueToString(m_lOrdinarySellVolumeBelow5000);
  psetWeekLineExtendInfo->m_OrdinarySellVolumeBelow10000 = ConvertValueToString(m_lOrdinarySellVolumeBelow10000);
  psetWeekLineExtendInfo->m_OrdinarySellVolumeBelow20000 = ConvertValueToString(m_lOrdinarySellVolumeBelow20000);
  psetWeekLineExtendInfo->m_OrdinarySellVolumeBelow50000 = ConvertValueToString(m_lOrdinarySellVolumeBelow50000);
  psetWeekLineExtendInfo->m_OrdinarySellVolumeBelow100000 = ConvertValueToString(m_lOrdinarySellVolumeBelow100000);
  psetWeekLineExtendInfo->m_OrdinarySellVolumeBelow200000 = ConvertValueToString(m_lOrdinarySellVolumeBelow200000);
  psetWeekLineExtendInfo->m_OrdinarySellVolumeAbove200000 = ConvertValueToString(m_lOrdinarySellVolumeAbove200000);
  psetWeekLineExtendInfo->m_OrdinaryBuyNumberBelow5000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow5000);
  psetWeekLineExtendInfo->m_OrdinaryBuyNumberBelow10000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow10000);
  psetWeekLineExtendInfo->m_OrdinaryBuyNumberBelow20000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow20000);
  psetWeekLineExtendInfo->m_OrdinaryBuyNumberBelow50000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow50000);
  psetWeekLineExtendInfo->m_OrdinaryBuyNumberBelow100000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow100000);
  psetWeekLineExtendInfo->m_OrdinaryBuyNumberBelow200000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow200000);
  psetWeekLineExtendInfo->m_OrdinaryBuyNumberAbove200000 = ConvertValueToString(m_lOrdinaryBuyNumberAbove200000);
  psetWeekLineExtendInfo->m_OrdinarySellNumberBelow5000 = ConvertValueToString(m_lOrdinarySellNumberBelow5000);
  psetWeekLineExtendInfo->m_OrdinarySellNumberBelow10000 = ConvertValueToString(m_lOrdinarySellNumberBelow10000);
  psetWeekLineExtendInfo->m_OrdinarySellNumberBelow20000 = ConvertValueToString(m_lOrdinarySellNumberBelow20000);
  psetWeekLineExtendInfo->m_OrdinarySellNumberBelow50000 = ConvertValueToString(m_lOrdinarySellNumberBelow50000);
  psetWeekLineExtendInfo->m_OrdinarySellNumberBelow100000 = ConvertValueToString(m_lOrdinarySellNumberBelow100000);
  psetWeekLineExtendInfo->m_OrdinarySellNumberBelow200000 = ConvertValueToString(m_lOrdinarySellNumberBelow200000);
  psetWeekLineExtendInfo->m_OrdinarySellNumberAbove200000 = ConvertValueToString(m_lOrdinarySellNumberAbove200000);

  psetWeekLineExtendInfo->m_CanceledBuyVolumeBelow5000 = ConvertValueToString(m_lCanceledBuyVolumeBelow5000);
  psetWeekLineExtendInfo->m_CanceledBuyVolumeBelow10000 = ConvertValueToString(m_lCanceledBuyVolumeBelow10000);
  psetWeekLineExtendInfo->m_CanceledBuyVolumeBelow20000 = ConvertValueToString(m_lCanceledBuyVolumeBelow20000);
  psetWeekLineExtendInfo->m_CanceledBuyVolumeBelow50000 = ConvertValueToString(m_lCanceledBuyVolumeBelow50000);
  psetWeekLineExtendInfo->m_CanceledBuyVolumeBelow100000 = ConvertValueToString(m_lCanceledBuyVolumeBelow100000);
  psetWeekLineExtendInfo->m_CanceledBuyVolumeBelow200000 = ConvertValueToString(m_lCanceledBuyVolumeBelow200000);
  psetWeekLineExtendInfo->m_CanceledBuyVolumeAbove200000 = ConvertValueToString(m_lCanceledBuyVolumeAbove200000);
  psetWeekLineExtendInfo->m_CanceledSellVolumeBelow5000 = ConvertValueToString(m_lCanceledSellVolumeBelow5000);
  psetWeekLineExtendInfo->m_CanceledSellVolumeBelow10000 = ConvertValueToString(m_lCanceledSellVolumeBelow10000);
  psetWeekLineExtendInfo->m_CanceledSellVolumeBelow20000 = ConvertValueToString(m_lCanceledSellVolumeBelow20000);
  psetWeekLineExtendInfo->m_CanceledSellVolumeBelow50000 = ConvertValueToString(m_lCanceledSellVolumeBelow50000);
  psetWeekLineExtendInfo->m_CanceledSellVolumeBelow100000 = ConvertValueToString(m_lCanceledSellVolumeBelow100000);
  psetWeekLineExtendInfo->m_CanceledSellVolumeBelow200000 = ConvertValueToString(m_lCanceledSellVolumeBelow200000);
  psetWeekLineExtendInfo->m_CanceledSellVolumeAbove200000 = ConvertValueToString(m_lCanceledSellVolumeAbove200000);

  return true;
}

bool CWeekLine::AppendExtendData(CSetWeekLineExtendInfo* psetWeekLineExtendInfo) {
  ASSERT(psetWeekLineExtendInfo->IsOpen());
  psetWeekLineExtendInfo->AddNew();
  SaveExtendData(psetWeekLineExtendInfo);
  psetWeekLineExtendInfo->Update();
  return true;
}

bool CWeekLine::LoadCurrentWeekData(CCurrentWeekLineInfo* psetCurrentWeekLineInfo) {
  ASSERT(psetCurrentWeekLineInfo->IsOpen());
  m_lDate = psetCurrentWeekLineInfo->m_Date;
  m_strStockSymbol = psetCurrentWeekLineInfo->m_Symbol;
  m_strStockName = psetCurrentWeekLineInfo->m_StockName;
  m_lLastClose = atof(psetCurrentWeekLineInfo->m_LastClose) * 1000;
  m_lOpen = atof(psetCurrentWeekLineInfo->m_Open) * 1000;
  m_lHigh = atof(psetCurrentWeekLineInfo->m_High) * 1000;
  m_lLow = atof(psetCurrentWeekLineInfo->m_Low) * 1000;
  m_lClose = atof(psetCurrentWeekLineInfo->m_Close) * 1000;
  m_dUpDown = atof(psetCurrentWeekLineInfo->m_UpAndDown);
  m_llVolume = atoll(psetCurrentWeekLineInfo->m_Volume);
  m_llAmount = atoll(psetCurrentWeekLineInfo->m_Amount);
  m_dUpDownRate = atof(psetCurrentWeekLineInfo->m_UpDownRate);
  m_dChangeHandRate = atof(psetCurrentWeekLineInfo->m_ChangeHandRate);
  m_llTotalValue = atoll(psetCurrentWeekLineInfo->m_TotalValue);
  m_llCurrentValue = atoll(psetCurrentWeekLineInfo->m_CurrentValue);
  m_dRS = atof(psetCurrentWeekLineInfo->m_RS);
  m_dRSIndex = atof(psetCurrentWeekLineInfo->m_RSIndex);
  m_dRSBackup = atof(psetCurrentWeekLineInfo->m_RSBackup);

  m_lTransactionNumber = atol(psetCurrentWeekLineInfo->m_TransactionNumber);
  m_lTransactionNumberBelow5000 = atol(psetCurrentWeekLineInfo->m_TransactionNumberBelow5000);
  m_lTransactionNumberBelow50000 = atol(psetCurrentWeekLineInfo->m_TransactionNumberBelow50000);
  m_lTransactionNumberBelow200000 = atol(psetCurrentWeekLineInfo->m_TransactionNumberBelow200000);
  m_lTransactionNumberAbove200000 = atol(psetCurrentWeekLineInfo->m_TransactionNumberAbove200000);
  m_lAttackBuyVolume = atol(psetCurrentWeekLineInfo->m_AttackBuyVolume);
  m_lStrongBuyVolume = atol(psetCurrentWeekLineInfo->m_StrongBuyVolume);
  m_lAttackSellVolume = atol(psetCurrentWeekLineInfo->m_AttackSellVolume);
  m_lStrongSellVolume = atol(psetCurrentWeekLineInfo->m_StrongSellVolume);
  m_lUnknownVolume = atol(psetCurrentWeekLineInfo->m_UnknownVolume);
  m_lCanceledBuyVolume = atol(psetCurrentWeekLineInfo->m_CanceledBuyVolume);
  m_lCanceledSellVolume = atol(psetCurrentWeekLineInfo->m_CanceledSellVolume);
  m_lOrdinaryBuyVolume = atol(psetCurrentWeekLineInfo->m_OrdinaryBuyVolume);
  m_lAttackBuyBelow50000 = atol(psetCurrentWeekLineInfo->m_AttackBuyBelow50000);
  m_lAttackBuyBelow200000 = atol(psetCurrentWeekLineInfo->m_AttackBuyBelow200000);
  m_lAttackBuyAbove200000 = atol(psetCurrentWeekLineInfo->m_AttackBuyAbove200000);
  m_lOrdinarySellVolume = atol(psetCurrentWeekLineInfo->m_OrdinarySellVolume);
  m_lAttackSellBelow50000 = atol(psetCurrentWeekLineInfo->m_AttackSellBelow50000);
  m_lAttackSellBelow200000 = atol(psetCurrentWeekLineInfo->m_AttackSellBelow200000);
  m_lAttackSellAbove200000 = atol(psetCurrentWeekLineInfo->m_AttackSellAbove200000);

  m_lOrdinaryBuyVolumeBelow5000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeBelow5000);
  m_lOrdinaryBuyVolumeBelow10000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeBelow10000);
  m_lOrdinaryBuyVolumeBelow20000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeBelow20000);
  m_lOrdinaryBuyVolumeBelow50000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeBelow50000);
  m_lOrdinaryBuyVolumeBelow100000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeBelow100000);
  m_lOrdinaryBuyVolumeBelow200000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeBelow200000);
  m_lOrdinaryBuyVolumeAbove200000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyVolumeAbove200000);
  m_lOrdinarySellVolumeBelow5000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellVolumeBelow5000);
  m_lOrdinarySellVolumeBelow10000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellVolumeBelow10000);
  m_lOrdinarySellVolumeBelow20000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellVolumeBelow20000);
  m_lOrdinarySellVolumeBelow50000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellVolumeBelow50000);
  m_lOrdinarySellVolumeBelow100000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellVolumeBelow100000);
  m_lOrdinarySellVolumeBelow200000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellVolumeBelow200000);
  m_lOrdinarySellVolumeAbove200000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellVolumeAbove200000);

  m_lOrdinaryBuyNumberBelow5000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyNumberBelow5000);
  m_lOrdinaryBuyNumberBelow10000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyNumberBelow10000);
  m_lOrdinaryBuyNumberBelow20000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyNumberBelow20000);
  m_lOrdinaryBuyNumberBelow50000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyNumberBelow50000);
  m_lOrdinaryBuyNumberBelow100000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyNumberBelow100000);
  m_lOrdinaryBuyNumberBelow200000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyNumberBelow200000);
  m_lOrdinaryBuyNumberAbove200000 = atoll(psetCurrentWeekLineInfo->m_OrdinaryBuyNumberAbove200000);
  m_lOrdinarySellNumberBelow5000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellNumberBelow5000);
  m_lOrdinarySellNumberBelow10000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellNumberBelow10000);
  m_lOrdinarySellNumberBelow20000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellNumberBelow20000);
  m_lOrdinarySellNumberBelow50000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellNumberBelow50000);
  m_lOrdinarySellNumberBelow100000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellNumberBelow100000);
  m_lOrdinarySellNumberBelow200000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellNumberBelow200000);
  m_lOrdinarySellNumberAbove200000 = atoll(psetCurrentWeekLineInfo->m_OrdinarySellNumberAbove200000);

  m_lCanceledBuyVolumeBelow5000 = atoll(psetCurrentWeekLineInfo->m_CanceledBuyVolumeBelow5000);
  m_lCanceledBuyVolumeBelow10000 = atoll(psetCurrentWeekLineInfo->m_CanceledBuyVolumeBelow10000);
  m_lCanceledBuyVolumeBelow20000 = atoll(psetCurrentWeekLineInfo->m_CanceledBuyVolumeBelow20000);
  m_lCanceledBuyVolumeBelow50000 = atoll(psetCurrentWeekLineInfo->m_CanceledBuyVolumeBelow50000);
  m_lCanceledBuyVolumeBelow100000 = atoll(psetCurrentWeekLineInfo->m_CanceledBuyVolumeBelow100000);
  m_lCanceledBuyVolumeBelow200000 = atoll(psetCurrentWeekLineInfo->m_CanceledBuyVolumeBelow200000);
  m_lCanceledBuyVolumeAbove200000 = atoll(psetCurrentWeekLineInfo->m_CanceledBuyVolumeAbove200000);
  m_lCanceledSellVolumeBelow5000 = atoll(psetCurrentWeekLineInfo->m_CanceledSellVolumeBelow5000);
  m_lCanceledSellVolumeBelow10000 = atoll(psetCurrentWeekLineInfo->m_CanceledSellVolumeBelow10000);
  m_lCanceledSellVolumeBelow20000 = atoll(psetCurrentWeekLineInfo->m_CanceledSellVolumeBelow20000);
  m_lCanceledSellVolumeBelow50000 = atoll(psetCurrentWeekLineInfo->m_CanceledSellVolumeBelow50000);
  m_lCanceledSellVolumeBelow100000 = atoll(psetCurrentWeekLineInfo->m_CanceledSellVolumeBelow100000);
  m_lCanceledSellVolumeBelow200000 = atoll(psetCurrentWeekLineInfo->m_CanceledSellVolumeBelow200000);
  m_lCanceledSellVolumeAbove200000 = atoll(psetCurrentWeekLineInfo->m_CanceledSellVolumeAbove200000);

  CalculateRSLogarithm1(m_dRS);
  return true;
}

bool CWeekLine::LoadBasicData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo) {
  ASSERT(psetWeekLineBasicInfo->IsOpen());
  m_lDate = psetWeekLineBasicInfo->m_Date;
  m_strStockSymbol = psetWeekLineBasicInfo->m_Symbol;
  m_strStockName = psetWeekLineBasicInfo->m_StockName;
  m_lLastClose = atof(psetWeekLineBasicInfo->m_LastClose) * 1000;
  m_lOpen = atof(psetWeekLineBasicInfo->m_Open) * 1000;
  m_lHigh = atof(psetWeekLineBasicInfo->m_High) * 1000;
  m_lLow = atof(psetWeekLineBasicInfo->m_Low) * 1000;
  m_lClose = atof(psetWeekLineBasicInfo->m_Close) * 1000;
  m_dUpDown = atof(psetWeekLineBasicInfo->m_UpAndDown);
  m_llVolume = atoll(psetWeekLineBasicInfo->m_Volume);
  m_llAmount = atoll(psetWeekLineBasicInfo->m_Amount);
  m_dUpDownRate = atof(psetWeekLineBasicInfo->m_UpDownRate);
  m_dChangeHandRate = atof(psetWeekLineBasicInfo->m_ChangeHandRate);
  m_llTotalValue = atoll(psetWeekLineBasicInfo->m_TotalValue);
  m_llCurrentValue = atoll(psetWeekLineBasicInfo->m_CurrentValue);
  m_dRS = atof(psetWeekLineBasicInfo->m_RS);
  m_dRSIndex = atof(psetWeekLineBasicInfo->m_RSIndex);
  m_dRSBackup = atof(psetWeekLineBasicInfo->m_RSBackup);
  CalculateRSLogarithm1(m_dRS);
  return true;
}

bool CWeekLine::LoadExtendData(CSetWeekLineExtendInfo* psetWeekLineExtendInfo) {
  ASSERT(psetWeekLineExtendInfo->IsOpen());
  m_lTransactionNumber = atol(psetWeekLineExtendInfo->m_TransactionNumber);
  m_lTransactionNumberBelow5000 = atol(psetWeekLineExtendInfo->m_TransactionNumberBelow5000);
  m_lTransactionNumberBelow50000 = atol(psetWeekLineExtendInfo->m_TransactionNumberBelow50000);
  m_lTransactionNumberBelow200000 = atol(psetWeekLineExtendInfo->m_TransactionNumberBelow200000);
  m_lTransactionNumberAbove200000 = atol(psetWeekLineExtendInfo->m_TransactionNumberAbove200000);
  m_lAttackBuyVolume = atol(psetWeekLineExtendInfo->m_AttackBuyVolume);
  m_lStrongBuyVolume = atol(psetWeekLineExtendInfo->m_StrongBuyVolume);
  m_lAttackSellVolume = atol(psetWeekLineExtendInfo->m_AttackSellVolume);
  m_lStrongSellVolume = atol(psetWeekLineExtendInfo->m_StrongSellVolume);
  m_lUnknownVolume = atol(psetWeekLineExtendInfo->m_UnknownVolume);
  m_lCanceledBuyVolume = atol(psetWeekLineExtendInfo->m_CanceledBuyVolume);
  m_lCanceledSellVolume = atol(psetWeekLineExtendInfo->m_CanceledSellVolume);
  m_lOrdinaryBuyVolume = atol(psetWeekLineExtendInfo->m_OrdinaryBuyVolume);
  m_lAttackBuyBelow50000 = atol(psetWeekLineExtendInfo->m_AttackBuyBelow50000);
  m_lAttackBuyBelow200000 = atol(psetWeekLineExtendInfo->m_AttackBuyBelow200000);
  m_lAttackBuyAbove200000 = atol(psetWeekLineExtendInfo->m_AttackBuyAbove200000);
  m_lOrdinarySellVolume = atol(psetWeekLineExtendInfo->m_OrdinarySellVolume);
  m_lAttackSellBelow50000 = atol(psetWeekLineExtendInfo->m_AttackSellBelow50000);
  m_lAttackSellBelow200000 = atol(psetWeekLineExtendInfo->m_AttackSellBelow200000);
  m_lAttackSellAbove200000 = atol(psetWeekLineExtendInfo->m_AttackSellAbove200000);

  m_lOrdinaryBuyVolumeBelow5000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyVolumeBelow5000);
  m_lOrdinaryBuyVolumeBelow10000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyVolumeBelow10000);
  m_lOrdinaryBuyVolumeBelow20000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyVolumeBelow20000);
  m_lOrdinaryBuyVolumeBelow50000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyVolumeBelow50000);
  m_lOrdinaryBuyVolumeBelow100000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyVolumeBelow100000);
  m_lOrdinaryBuyVolumeBelow200000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyVolumeBelow200000);
  m_lOrdinaryBuyVolumeAbove200000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyVolumeAbove200000);
  m_lOrdinarySellVolumeBelow5000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellVolumeBelow5000);
  m_lOrdinarySellVolumeBelow10000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellVolumeBelow10000);
  m_lOrdinarySellVolumeBelow20000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellVolumeBelow20000);
  m_lOrdinarySellVolumeBelow50000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellVolumeBelow50000);
  m_lOrdinarySellVolumeBelow100000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellVolumeBelow100000);
  m_lOrdinarySellVolumeBelow200000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellVolumeBelow200000);
  m_lOrdinarySellVolumeAbove200000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellVolumeAbove200000);

  m_lOrdinaryBuyNumberBelow5000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyNumberBelow5000);
  m_lOrdinaryBuyNumberBelow10000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyNumberBelow10000);
  m_lOrdinaryBuyNumberBelow20000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyNumberBelow20000);
  m_lOrdinaryBuyNumberBelow50000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyNumberBelow50000);
  m_lOrdinaryBuyNumberBelow100000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyNumberBelow100000);
  m_lOrdinaryBuyNumberBelow200000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyNumberBelow200000);
  m_lOrdinaryBuyNumberAbove200000 = atoll(psetWeekLineExtendInfo->m_OrdinaryBuyNumberAbove200000);
  m_lOrdinarySellNumberBelow5000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellNumberBelow5000);
  m_lOrdinarySellNumberBelow10000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellNumberBelow10000);
  m_lOrdinarySellNumberBelow20000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellNumberBelow20000);
  m_lOrdinarySellNumberBelow50000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellNumberBelow50000);
  m_lOrdinarySellNumberBelow100000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellNumberBelow100000);
  m_lOrdinarySellNumberBelow200000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellNumberBelow200000);
  m_lOrdinarySellNumberAbove200000 = atoll(psetWeekLineExtendInfo->m_OrdinarySellNumberAbove200000);

  m_lCanceledBuyVolumeBelow5000 = atoll(psetWeekLineExtendInfo->m_CanceledBuyVolumeBelow5000);
  m_lCanceledBuyVolumeBelow10000 = atoll(psetWeekLineExtendInfo->m_CanceledBuyVolumeBelow10000);
  m_lCanceledBuyVolumeBelow20000 = atoll(psetWeekLineExtendInfo->m_CanceledBuyVolumeBelow20000);
  m_lCanceledBuyVolumeBelow50000 = atoll(psetWeekLineExtendInfo->m_CanceledBuyVolumeBelow50000);
  m_lCanceledBuyVolumeBelow100000 = atoll(psetWeekLineExtendInfo->m_CanceledBuyVolumeBelow100000);
  m_lCanceledBuyVolumeBelow200000 = atoll(psetWeekLineExtendInfo->m_CanceledBuyVolumeBelow200000);
  m_lCanceledBuyVolumeAbove200000 = atoll(psetWeekLineExtendInfo->m_CanceledBuyVolumeAbove200000);
  m_lCanceledSellVolumeBelow5000 = atoll(psetWeekLineExtendInfo->m_CanceledSellVolumeBelow5000);
  m_lCanceledSellVolumeBelow10000 = atoll(psetWeekLineExtendInfo->m_CanceledSellVolumeBelow10000);
  m_lCanceledSellVolumeBelow20000 = atoll(psetWeekLineExtendInfo->m_CanceledSellVolumeBelow20000);
  m_lCanceledSellVolumeBelow50000 = atoll(psetWeekLineExtendInfo->m_CanceledSellVolumeBelow50000);
  m_lCanceledSellVolumeBelow100000 = atoll(psetWeekLineExtendInfo->m_CanceledSellVolumeBelow100000);
  m_lCanceledSellVolumeBelow200000 = atoll(psetWeekLineExtendInfo->m_CanceledSellVolumeBelow200000);
  m_lCanceledSellVolumeAbove200000 = atoll(psetWeekLineExtendInfo->m_CanceledSellVolumeAbove200000);

  return true;
}

bool CWeekLine::UpdateWeekLine(CDayLinePtr pDayLine) {
  if (m_lDate == 0) m_lDate = GetCurrentMonday(pDayLine->GetFormatedMarketDate());;
  if (m_time == 0) m_time = pDayLine->GetFormatedMarketTime();
  if (m_strStockSymbol == _T("")) m_strStockSymbol = pDayLine->GetStockSymbol();
  if (m_strStockName == _T("")) m_strStockName = pDayLine->GetStockName();

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