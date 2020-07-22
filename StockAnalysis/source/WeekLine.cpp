#include "stdafx.h"

#include"globedef.h"
#include "WeekLine.h"

CWeekLine::CWeekLine() : CChinaStockHistoryData() {
  Reset();
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

void CWeekLine::Reset(void) {
  CChinaStockHistoryData::Reset();

  m_psetWeekLineBasicInfo = nullptr;
  m_psetWeekLineExtendInfo = nullptr;
}

bool CWeekLine::SaveData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo) {
  ASSERT(psetWeekLineBasicInfo->IsOpen());

  psetWeekLineBasicInfo->m_Day = GetFormatedMarketDay();
  psetWeekLineBasicInfo->m_Market = GetMarket();
  psetWeekLineBasicInfo->m_StockCode = GetStockCode();
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
  psetWeekLineBasicInfo->m_RelativeStrong = ConvertValueToString(GetRelativeStrong());
  psetWeekLineBasicInfo->m_RelativeStrongIndex = ConvertValueToString(GetRelativeStrongIndex());
  psetWeekLineBasicInfo->m_RelativeStrongBackup = ConvertValueToString(GetRelativeStrongBackup());

  return true;
}

bool CWeekLine::AppendData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo) {
  ASSERT(psetWeekLineBasicInfo->IsOpen());
  psetWeekLineBasicInfo->AddNew();
  SaveData(psetWeekLineBasicInfo);
  psetWeekLineBasicInfo->Update();

  return true;
}

bool CWeekLine::LoadBasicData(CSetWeekLineBasicInfo* psetWeekLineBasicInfo) {
  ASSERT(psetWeekLineBasicInfo->IsOpen());
  m_lDay = psetWeekLineBasicInfo->m_Day;
  m_wMarket = psetWeekLineBasicInfo->m_Market;
  m_strStockCode = psetWeekLineBasicInfo->m_StockCode;
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
  m_dRelativeStrong = atof(psetWeekLineBasicInfo->m_RelativeStrong);
  m_dRelativeStrongIndex = atof(psetWeekLineBasicInfo->m_RelativeStrongIndex);
  m_dRelativeStrongBackup = atof(psetWeekLineBasicInfo->m_RelativeStrongBackup);
  CalculateRSLogarithm(m_dRelativeStrong);
  return true;
}