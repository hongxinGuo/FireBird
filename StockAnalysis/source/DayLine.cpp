#include "stdafx.h"

#include "DayLine.h"

CDayLine::CDayLine() : CObject() {
  Reset();
}

CDayLine::CDayLine(CDayLine& oneDl) {
  m_time = oneDl.m_time;
  m_lDay = oneDl.m_lDay;
  m_wMarket = oneDl.m_wMarket;
  m_lLastClose = oneDl.m_lLastClose;
  m_lOpen = oneDl.m_lOpen;
  m_lHigh = oneDl.m_lHigh;
  m_lLow = oneDl.m_lLow;
  m_lClose = oneDl.m_lClose;
  m_llVolume = oneDl.m_llVolume;
  m_llAmount = oneDl.m_llAmount;
  m_dUpDown = oneDl.m_dUpDown;
  m_dUpDownRate = oneDl.m_dUpDownRate;
  m_dChangeHandRate = oneDl.m_dChangeHandRate;
  m_llTotalValue = oneDl.m_llTotalValue;
  m_llCurrentValue = oneDl.m_llCurrentValue;
  m_lTransactionNumber = oneDl.m_lTransactionNumber;
  m_lTransactionNumberBelow5000 = oneDl.m_lTransactionNumberBelow5000;
  m_lTransactionNumberBelow50000 = oneDl.m_lTransactionNumberBelow50000;
  m_lTransactionNumberBelow200000 = oneDl.m_lTransactionNumberBelow200000;
  m_lTransactionNumberAbove200000 = oneDl.m_lTransactionNumberAbove200000;
  m_lAttackBuyVolume = oneDl.m_lAttackBuyVolume;
  m_lStrongBuyVolume = oneDl.m_lStrongBuyVolume;
  m_lAttackSellVolume = oneDl.m_lAttackSellVolume;
  m_lStrongSellVolume = oneDl.m_lStrongSellVolume;
  m_lUnknownVolume = oneDl.m_lUnknownVolume;
  m_lCancelBuyVolume = oneDl.m_lCancelBuyVolume;
  m_lCancelSellVolume = oneDl.m_lCancelSellVolume;
  m_dRelativeStrong = oneDl.m_dRelativeStrong;
  m_lOrdinaryBuyVolume = oneDl.m_lOrdinaryBuyVolume;
  m_lAttackBuyBelow50000 = oneDl.m_lAttackBuyBelow50000;
  m_lAttackBuyBelow200000 = oneDl.m_lAttackBuyBelow200000;
  m_lAttackBuyAbove200000 = oneDl.m_lAttackBuyAbove200000;
  m_lOrdinarySellVolume = oneDl.m_lOrdinarySellVolume;
  m_lAttackSellBelow50000 = oneDl.m_lAttackSellBelow50000;
  m_lAttackSellBelow200000 = oneDl.m_lAttackSellBelow200000;
  m_lAttackSellAbove200000 = oneDl.m_lAttackSellAbove200000;
  m_d3DayRS = oneDl.m_d3DayRS;
  m_d5DayRS = oneDl.m_d5DayRS;
  m_d10DayRS = oneDl.m_d10DayRS;
  m_d30DayRS = oneDl.m_d30DayRS;
  m_d60DayRS = oneDl.m_d60DayRS;
  m_d120DayRS = oneDl.m_d120DayRS;
}

void CDayLine::operator =(CDayLine& oneDl) {
  m_lDay = oneDl.m_lDay;
  m_lLastClose = oneDl.m_lLastClose;
  m_lOpen = oneDl.m_lOpen;
  m_lHigh = oneDl.m_lHigh;
  m_lLow = oneDl.m_lLow;
  m_lClose = oneDl.m_lClose;
  m_llVolume = oneDl.m_llVolume;
  m_llAmount = oneDl.m_llAmount;
  m_dUpDown = oneDl.m_dUpDown;
  m_dUpDownRate = oneDl.m_dUpDownRate;
  m_dChangeHandRate = oneDl.m_dChangeHandRate;
  m_llTotalValue = oneDl.m_llTotalValue;
  m_llCurrentValue = oneDl.m_llCurrentValue;
  m_lTransactionNumber = oneDl.m_lTransactionNumber;
  m_lTransactionNumberBelow5000 = oneDl.m_lTransactionNumberBelow5000;
  m_lTransactionNumberBelow50000 = oneDl.m_lTransactionNumberBelow50000;
  m_lTransactionNumberBelow200000 = oneDl.m_lTransactionNumberBelow200000;
  m_lTransactionNumberAbove200000 = oneDl.m_lTransactionNumberAbove200000;
  m_lAttackBuyVolume = oneDl.m_lAttackBuyVolume;
  m_lStrongBuyVolume = oneDl.m_lStrongBuyVolume;
  m_lAttackSellVolume = oneDl.m_lAttackSellVolume;
  m_lStrongSellVolume = oneDl.m_lStrongSellVolume;
  m_lUnknownVolume = oneDl.m_lUnknownVolume;
  m_lCancelBuyVolume = oneDl.m_lCancelBuyVolume;
  m_lCancelSellVolume = oneDl.m_lCancelSellVolume;
  m_dRelativeStrong = oneDl.m_dRelativeStrong;
  m_lOrdinaryBuyVolume = oneDl.m_lOrdinaryBuyVolume;
  m_lAttackBuyBelow50000 = oneDl.m_lAttackBuyBelow50000;
  m_lAttackBuyBelow200000 = oneDl.m_lAttackBuyBelow200000;
  m_lAttackBuyAbove200000 = oneDl.m_lAttackBuyAbove200000;
  m_lOrdinarySellVolume = oneDl.m_lOrdinarySellVolume;
  m_lAttackSellBelow50000 = oneDl.m_lAttackSellBelow50000;
  m_lAttackSellBelow200000 = oneDl.m_lAttackSellBelow200000;
  m_lAttackSellAbove200000 = oneDl.m_lAttackSellAbove200000;
  m_d3DayRS = oneDl.m_d3DayRS;
  m_d5DayRS = oneDl.m_d5DayRS;
  m_d10DayRS = oneDl.m_d10DayRS;
  m_d30DayRS = oneDl.m_d30DayRS;
  m_d60DayRS = oneDl.m_d60DayRS;
  m_d120DayRS = oneDl.m_d120DayRS;
}

void CDayLine::SetData(CSetDayLine& setDayLine) {
  m_lDay = setDayLine.m_Day;
  m_wMarket = setDayLine.m_Market;
  m_lLastClose = atof(setDayLine.m_LastClose) * 1000;
  m_lOpen = atof(setDayLine.m_Open) * 1000;
  m_lHigh = atof(setDayLine.m_High) * 1000;
  m_lLow = atof(setDayLine.m_Low) * 1000;
  m_lClose = atof(setDayLine.m_Close) * 1000;
  m_llVolume = atoll(setDayLine.m_Volume);
  m_llAmount = atoll(setDayLine.m_Amount);
  m_dUpDown = atof(setDayLine.m_UpAndDown);
  m_dUpDownRate = atof(setDayLine.m_UpDownRate);
  m_dChangeHandRate = atof(setDayLine.m_ChangeHandRate);
  m_llTotalValue = atoll(setDayLine.m_TotalValue);
  m_llCurrentValue = atoll(setDayLine.m_CurrentValue);
  m_dRelativeStrong = atof(setDayLine.m_RelativeStrong);
}

void CDayLine::SetData(CSetDayLineInfo& setDayLineInfo) {
  m_lTransactionNumber = atol(setDayLineInfo.m_TransactionNumber);
  m_lTransactionNumberBelow5000 = atol(setDayLineInfo.m_TransactionNumberBelow5000);
  m_lTransactionNumberBelow50000 = atol(setDayLineInfo.m_TransactionNumberBelow50000);
  m_lTransactionNumberBelow200000 = atol(setDayLineInfo.m_TransactionNumberBelow200000);
  m_lTransactionNumberAbove200000 = atol(setDayLineInfo.m_TransactionNumberAbove200000);
  m_lAttackBuyVolume = atol(setDayLineInfo.m_AttackBuyVolume);
  m_lStrongBuyVolume = atol(setDayLineInfo.m_StrongBuyVolume);
  m_lAttackSellVolume = atol(setDayLineInfo.m_AttackSellVolume);
  m_lStrongSellVolume = atol(setDayLineInfo.m_StrongSellVolume);
  m_lUnknownVolume = atol(setDayLineInfo.m_UnknownVolume);
  m_lCancelBuyVolume = atol(setDayLineInfo.m_CancelBuyVolume);
  m_lCancelSellVolume = atol(setDayLineInfo.m_CancelSellVolume);
  m_lOrdinaryBuyVolume = atol(setDayLineInfo.m_OrdinaryBuyVolume);
  m_lAttackBuyBelow50000 = atol(setDayLineInfo.m_AttackBuyBelow50000);
  m_lAttackBuyBelow200000 = atol(setDayLineInfo.m_AttackBuyBelow200000);
  m_lAttackBuyAbove200000 = atol(setDayLineInfo.m_AttackBuyAbove200000);
  m_lOrdinarySellVolume = atol(setDayLineInfo.m_OrdinarySellVolume);
  m_lAttackSellBelow50000 = atol(setDayLineInfo.m_AttackSellBelow50000);
  m_lAttackSellBelow200000 = atol(setDayLineInfo.m_AttackSellBelow200000);
  m_lAttackSellAbove200000 = atol(setDayLineInfo.m_AttackSellAbove200000);
}

void CDayLine::Reset(void) {
  m_lDay = 0;		// 类型
  m_time = 0;
  m_wMarket = 0;
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
  m_lUnknownVolume = m_lCancelBuyVolume = m_lCancelSellVolume = m_dRelativeStrong = 0;
  m_lOrdinaryBuyVolume = m_lAttackBuyBelow50000 = m_lAttackBuyBelow200000 = m_lAttackBuyAbove200000 = 0;
  m_lOrdinarySellVolume = m_lAttackSellBelow50000 = m_lAttackSellBelow200000 = m_lAttackSellAbove200000 = 0;
  m_d3DayRS = m_d5DayRS = m_d10DayRS = m_d30DayRS = m_d60DayRS = m_d120DayRS = 0;
}

bool CDayLine::SaveData(CSetDayLine& setDayLine) {
  ASSERT(setDayLine.IsOpen());
  setDayLine.AddNew();
  setDayLine.m_Day = GetDay();
  setDayLine.m_Market = GetMarket();
  setDayLine.m_StockCode = GetStockCode();
  setDayLine.m_StockName = GetStockName();
  setDayLine.m_LastClose = ConvertValueToString(GetLastClose(), 1000);
  setDayLine.m_High = ConvertValueToString(GetHigh(), 1000);
  setDayLine.m_Low = ConvertValueToString(GetLow(), 1000);
  setDayLine.m_Open = ConvertValueToString(GetOpen(), 1000);
  setDayLine.m_Close = ConvertValueToString(GetClose(), 1000);
  setDayLine.m_Volume = ConvertValueToString(GetVolume());
  setDayLine.m_Amount = ConvertValueToString(GetAmount());
  setDayLine.m_UpAndDown = ConvertValueToString(GetUpDown());
  setDayLine.m_UpDownRate = ConvertValueToString(GetUpDownRate());
  setDayLine.m_ChangeHandRate = ConvertValueToString(GetChangeHandRate());
  setDayLine.m_TotalValue = ConvertValueToString(GetTotalValue());
  setDayLine.m_CurrentValue = ConvertValueToString(GetCurrentValue());
  setDayLine.m_RelativeStrong = ConvertValueToString(GetRelativeStrong());
  setDayLine.Update();

  return true;
}

bool CDayLine::LoadData(CSetDayLine& setDayLine) {
  m_lDay = setDayLine.m_Day;
  m_wMarket = setDayLine.m_Market;
  m_strStockCode = setDayLine.m_StockCode;
  m_lLastClose = atof(setDayLine.m_LastClose) * 1000;
  m_lOpen = atof(setDayLine.m_Open) * 1000;
  m_lHigh = atof(setDayLine.m_High) * 1000;
  m_lLow = atof(setDayLine.m_Low) * 1000;
  m_lClose = atof(setDayLine.m_Close) * 1000;
  m_dUpDown = atof(setDayLine.m_UpAndDown);
  m_llVolume = atoll(setDayLine.m_Volume);
  m_llAmount = atoll(setDayLine.m_Amount);
  m_dUpDownRate = atof(setDayLine.m_UpDownRate);
  m_dChangeHandRate = atof(setDayLine.m_ChangeHandRate);
  m_llTotalValue = atoll(setDayLine.m_TotalValue);
  m_llCurrentValue = atoll(setDayLine.m_CurrentValue);
  m_dRelativeStrong = atof(setDayLine.m_RelativeStrong);
  return false;
}