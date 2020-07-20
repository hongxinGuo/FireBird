#include "stdafx.h"

#include"globedef.h"
#include"ChinaStockHistoryData.h"

CChinaStockHistoryData::CChinaStockHistoryData() : CObject() {
  Reset();
}

bool CChinaStockHistoryData::LoadEntendData(CSetWeekLineExtendInfo& setWeekLineExtendInfo) {
  ASSERT(setWeekLineExtendInfo.IsOpen());
  m_lTransactionNumber = atol(setWeekLineExtendInfo.m_TransactionNumber);
  m_lTransactionNumberBelow5000 = atol(setWeekLineExtendInfo.m_TransactionNumberBelow5000);
  m_lTransactionNumberBelow50000 = atol(setWeekLineExtendInfo.m_TransactionNumberBelow50000);
  m_lTransactionNumberBelow200000 = atol(setWeekLineExtendInfo.m_TransactionNumberBelow200000);
  m_lTransactionNumberAbove200000 = atol(setWeekLineExtendInfo.m_TransactionNumberAbove200000);
  m_lAttackBuyVolume = atol(setWeekLineExtendInfo.m_AttackBuyVolume);
  m_lStrongBuyVolume = atol(setWeekLineExtendInfo.m_StrongBuyVolume);
  m_lAttackSellVolume = atol(setWeekLineExtendInfo.m_AttackSellVolume);
  m_lStrongSellVolume = atol(setWeekLineExtendInfo.m_StrongSellVolume);
  m_lUnknownVolume = atol(setWeekLineExtendInfo.m_UnknownVolume);
  m_lCanceledBuyVolume = atol(setWeekLineExtendInfo.m_CanceledBuyVolume);
  m_lCanceledSellVolume = atol(setWeekLineExtendInfo.m_CanceledSellVolume);
  m_lOrdinaryBuyVolume = atol(setWeekLineExtendInfo.m_OrdinaryBuyVolume);
  m_lAttackBuyBelow50000 = atol(setWeekLineExtendInfo.m_AttackBuyBelow50000);
  m_lAttackBuyBelow200000 = atol(setWeekLineExtendInfo.m_AttackBuyBelow200000);
  m_lAttackBuyAbove200000 = atol(setWeekLineExtendInfo.m_AttackBuyAbove200000);
  m_lOrdinarySellVolume = atol(setWeekLineExtendInfo.m_OrdinarySellVolume);
  m_lAttackSellBelow50000 = atol(setWeekLineExtendInfo.m_AttackSellBelow50000);
  m_lAttackSellBelow200000 = atol(setWeekLineExtendInfo.m_AttackSellBelow200000);
  m_lAttackSellAbove200000 = atol(setWeekLineExtendInfo.m_AttackSellAbove200000);

  m_lOrdinaryBuyVolumeBelow5000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeBelow5000);
  m_lOrdinaryBuyVolumeBelow10000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeBelow10000);
  m_lOrdinaryBuyVolumeBelow20000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeBelow20000);
  m_lOrdinaryBuyVolumeBelow50000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeBelow50000);
  m_lOrdinaryBuyVolumeBelow100000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeBelow100000);
  m_lOrdinaryBuyVolumeBelow200000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeBelow200000);
  m_lOrdinaryBuyVolumeAbove200000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeAbove200000);
  m_lOrdinarySellVolumeBelow5000 = atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeBelow5000);
  m_lOrdinarySellVolumeBelow10000 = atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeBelow10000);
  m_lOrdinarySellVolumeBelow20000 = atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeBelow20000);
  m_lOrdinarySellVolumeBelow50000 = atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeBelow50000);
  m_lOrdinarySellVolumeBelow100000 = atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeBelow100000);
  m_lOrdinarySellVolumeBelow200000 = atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeBelow200000);
  m_lOrdinarySellVolumeAbove200000 = atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeAbove200000);

  m_lOrdinaryBuyNumberBelow5000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberBelow5000);
  m_lOrdinaryBuyNumberBelow10000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberBelow10000);
  m_lOrdinaryBuyNumberBelow20000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberBelow20000);
  m_lOrdinaryBuyNumberBelow50000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberBelow50000);
  m_lOrdinaryBuyNumberBelow100000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberBelow100000);
  m_lOrdinaryBuyNumberBelow200000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberBelow200000);
  m_lOrdinaryBuyNumberAbove200000 = atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberAbove200000);
  m_lOrdinarySellNumberBelow5000 = atoll(setWeekLineExtendInfo.m_OrdinarySellNumberBelow5000);
  m_lOrdinarySellNumberBelow10000 = atoll(setWeekLineExtendInfo.m_OrdinarySellNumberBelow10000);
  m_lOrdinarySellNumberBelow20000 = atoll(setWeekLineExtendInfo.m_OrdinarySellNumberBelow20000);
  m_lOrdinarySellNumberBelow50000 = atoll(setWeekLineExtendInfo.m_OrdinarySellNumberBelow50000);
  m_lOrdinarySellNumberBelow100000 = atoll(setWeekLineExtendInfo.m_OrdinarySellNumberBelow100000);
  m_lOrdinarySellNumberBelow200000 = atoll(setWeekLineExtendInfo.m_OrdinarySellNumberBelow200000);
  m_lOrdinarySellNumberAbove200000 = atoll(setWeekLineExtendInfo.m_OrdinarySellNumberAbove200000);

  m_lCanceledBuyVolumeBelow5000 = atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeBelow5000);
  m_lCanceledBuyVolumeBelow10000 = atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeBelow10000);
  m_lCanceledBuyVolumeBelow20000 = atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeBelow20000);
  m_lCanceledBuyVolumeBelow50000 = atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeBelow50000);
  m_lCanceledBuyVolumeBelow100000 = atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeBelow100000);
  m_lCanceledBuyVolumeBelow200000 = atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeBelow200000);
  m_lCanceledBuyVolumeAbove200000 = atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeAbove200000);
  m_lCanceledSellVolumeBelow5000 = atoll(setWeekLineExtendInfo.m_CanceledSellVolumeBelow5000);
  m_lCanceledSellVolumeBelow10000 = atoll(setWeekLineExtendInfo.m_CanceledSellVolumeBelow10000);
  m_lCanceledSellVolumeBelow20000 = atoll(setWeekLineExtendInfo.m_CanceledSellVolumeBelow20000);
  m_lCanceledSellVolumeBelow50000 = atoll(setWeekLineExtendInfo.m_CanceledSellVolumeBelow50000);
  m_lCanceledSellVolumeBelow100000 = atoll(setWeekLineExtendInfo.m_CanceledSellVolumeBelow100000);
  m_lCanceledSellVolumeBelow200000 = atoll(setWeekLineExtendInfo.m_CanceledSellVolumeBelow200000);
  m_lCanceledSellVolumeAbove200000 = atoll(setWeekLineExtendInfo.m_CanceledSellVolumeAbove200000);

  return true;
}

void CChinaStockHistoryData::CalculateRSLogarithm(double dRS) {
  double dLog50 = log10(50.0);
  double dLog100 = log10(100.0);
  double dLog = dLog100 - dLog50;
  if ((dRS - 50) > 0) {
    m_dRSLogarithm = 50 + (log10(dRS) - dLog50) * 50 / dLog;
  }
  else if ((dRS - 50) < 0) {
    m_dRSLogarithm = 50 - (log10(100 - dRS) - dLog50) * 50 / dLog;
  }
  else m_dRSLogarithm = 50;
}

void CChinaStockHistoryData::Reset(void) {
  m_lDay = 0;		// 类型
  m_time = 0;
  m_wMarket = 0;
  m_strStockCode = _T("");
  m_strStockName = _T("");
  m_lLastClose = m_lOpen = m_lHigh = m_lLow = m_lClose = 0;							// 收盘价
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
  m_dRelativeStrong = m_dRSLogarithm = m_dRelativeStrongIndex = m_dRelativeStrongBackup = 0.0;
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

bool CChinaStockHistoryData::SaveData(CSetWeekLineBasicInfo& setWeekLineBasicInfo) {
  ASSERT(setWeekLineBasicInfo.IsOpen());

  setWeekLineBasicInfo.m_Day = GetFormatedMarketDay();
  setWeekLineBasicInfo.m_Market = GetMarket();
  setWeekLineBasicInfo.m_StockCode = GetStockCode();
  setWeekLineBasicInfo.m_StockName = GetStockName();
  setWeekLineBasicInfo.m_LastClose = ConvertValueToString(GetLastClose(), 1000);
  setWeekLineBasicInfo.m_High = ConvertValueToString(GetHigh(), 1000);
  setWeekLineBasicInfo.m_Low = ConvertValueToString(GetLow(), 1000);
  setWeekLineBasicInfo.m_Open = ConvertValueToString(GetOpen(), 1000);
  setWeekLineBasicInfo.m_Close = ConvertValueToString(GetClose(), 1000);
  setWeekLineBasicInfo.m_Volume = ConvertValueToString(GetVolume());
  setWeekLineBasicInfo.m_Amount = ConvertValueToString(GetAmount());
  setWeekLineBasicInfo.m_UpAndDown = ConvertValueToString(GetUpDown());
  setWeekLineBasicInfo.m_UpDownRate = ConvertValueToString(GetUpDownRate());
  setWeekLineBasicInfo.m_ChangeHandRate = ConvertValueToString(GetChangeHandRate());
  setWeekLineBasicInfo.m_TotalValue = ConvertValueToString(GetTotalValue());
  setWeekLineBasicInfo.m_CurrentValue = ConvertValueToString(GetCurrentValue());
  setWeekLineBasicInfo.m_RelativeStrong = ConvertValueToString(GetRelativeStrong());
  setWeekLineBasicInfo.m_RelativeStrongIndex = ConvertValueToString(GetRelativeStrongIndex());
  setWeekLineBasicInfo.m_RelativeStrongBackup = ConvertValueToString(GetRelativeStrongBackup());

  return true;
}

bool CChinaStockHistoryData::AppendData(CSetWeekLineBasicInfo& setWeekLineBasicInfo) {
  ASSERT(setWeekLineBasicInfo.IsOpen());
  setWeekLineBasicInfo.AddNew();
  SaveData(setWeekLineBasicInfo);
  setWeekLineBasicInfo.Update();

  return true;
}

bool CChinaStockHistoryData::LoadBasicData(CSetWeekLineBasicInfo& setWeekLineBasicInfo) {
  ASSERT(setWeekLineBasicInfo.IsOpen());
  m_lDay = setWeekLineBasicInfo.m_Day;
  m_wMarket = setWeekLineBasicInfo.m_Market;
  m_strStockCode = setWeekLineBasicInfo.m_StockCode;
  m_lLastClose = atof(setWeekLineBasicInfo.m_LastClose) * 1000;
  m_lOpen = atof(setWeekLineBasicInfo.m_Open) * 1000;
  m_lHigh = atof(setWeekLineBasicInfo.m_High) * 1000;
  m_lLow = atof(setWeekLineBasicInfo.m_Low) * 1000;
  m_lClose = atof(setWeekLineBasicInfo.m_Close) * 1000;
  m_dUpDown = atof(setWeekLineBasicInfo.m_UpAndDown);
  m_llVolume = atoll(setWeekLineBasicInfo.m_Volume);
  m_llAmount = atoll(setWeekLineBasicInfo.m_Amount);
  m_dUpDownRate = atof(setWeekLineBasicInfo.m_UpDownRate);
  m_dChangeHandRate = atof(setWeekLineBasicInfo.m_ChangeHandRate);
  m_llTotalValue = atoll(setWeekLineBasicInfo.m_TotalValue);
  m_llCurrentValue = atoll(setWeekLineBasicInfo.m_CurrentValue);
  m_dRelativeStrong = atof(setWeekLineBasicInfo.m_RelativeStrong);
  m_dRelativeStrongIndex = atof(setWeekLineBasicInfo.m_RelativeStrongIndex);
  m_dRelativeStrongBackup = atof(setWeekLineBasicInfo.m_RelativeStrongBackup);
  CalculateRSLogarithm(m_dRelativeStrong);
  return true;
}