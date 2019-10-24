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
  m_lVolume = oneDl.m_lVolume;
  m_lAmount = oneDl.m_lAmount;
  m_fUpDown = oneDl.m_fUpDown;
  m_dUpDownRate = oneDl.m_dUpDownRate;
  m_fChangeHandRate = oneDl.m_fChangeHandRate;
  m_lTotalValue = oneDl.m_lTotalValue;
  m_lCurrentValue = oneDl.m_lCurrentValue;
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
  m_lVolume = oneDl.m_lVolume;
  m_lAmount = oneDl.m_lAmount;
  m_dUpDownRate = oneDl.m_dUpDownRate;
  m_fChangeHandRate = oneDl.m_fChangeHandRate;
  m_lTotalValue = oneDl.m_lTotalValue;
  m_lCurrentValue = oneDl.m_lCurrentValue;
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

void CDayLine::SetData(CSetDayLine* pDayLine)
{
  m_lDay = pDayLine->m_Time;
  m_wMarket = pDayLine->m_Market;
  m_lLastClose = pDayLine->m_LastClose * 1000;
  m_lOpen = pDayLine->m_Open * 1000;
  m_lHigh = pDayLine->m_High * 1000;
  m_lLow = pDayLine->m_Low * 1000;
  m_lClose = pDayLine->m_Close * 1000;
  m_lVolume = pDayLine->m_Volume;
  m_lAmount = pDayLine->m_Amount;
  m_fUpDown = pDayLine->m_UpAndDown;
  m_dUpDownRate = pDayLine->m_UpDownRate;
  m_fChangeHandRate = pDayLine->m_ChangeHandRate;
  m_lTotalValue = pDayLine->m_TotalValue;
  m_lCurrentValue = pDayLine->m_CurrentValue;
  m_dRelativeStrong = pDayLine->m_RelativeStrong;
  /*
  m_lTransactionNumber = pDayLine->m_TransactionNumber;
  m_lTransactionNumberBelow5000 = pDayLine->m_TransactionNumberBelow5000;
  m_lTransactionNumberBelow50000 = pDayLine->m_TransactionNumberBelow50000;
  m_lTransactionNumberBelow200000 = pDayLine->m_TransactionNumberBelow200000;
  m_lTransactionNumberAbove200000 = pDayLine->m_TransactionNumberAbove200000;
  m_lAttackBuyVolume = pDayLine->m_AttackBuyVolume;
  m_lStrongBuyVolume = pDayLine->m_StrongBuyVolume;
  m_lAttackSellVolume = pDayLine->m_AttackSellVolume;
  m_lStrongSellVolume = pDayLine->m_StrongSellVolume;
  m_lUnknownVolume = pDayLine->m_UnknownVolume;
  m_lCancelBuyVolume = pDayLine->m_CancelBuyVolume;
  m_lCancelSellVolume = pDayLine->m_CancelSellVolume;
  m_lOrdinaryBuyVolume = pDayLine->m_OrdinaryBuyVolume;
  m_lAttackBuyBelow50000 = pDayLine->m_AttackBuyBelow50000;
  m_lAttackBuyBelow200000 = pDayLine->m_AttackBuyBelow200000;
  m_lAttackBuyAbove200000 = pDayLine->m_AttackBuyAbove200000;
  m_lOrdinarySellVolume = pDayLine->m_OrdinarySellVolume;
  m_lAttackSellBelow50000 = pDayLine->m_AttackSellBelow50000;
  m_lAttackSellBelow200000 = pDayLine->m_AttackSellBelow200000;
  m_lAttackSellAbove200000 = pDayLine->m_AttackSellAbove200000;
  */
}

void CDayLine::SetData(CSetDayLineInfo* pDayLineInfo)
{
  m_lTransactionNumber = pDayLineInfo->m_TransactionNumber;
  m_lTransactionNumberBelow5000 = pDayLineInfo->m_TransactionNumberBelow5000;
  m_lTransactionNumberBelow50000 = pDayLineInfo->m_TransactionNumberBelow50000;
  m_lTransactionNumberBelow200000 = pDayLineInfo->m_TransactionNumberBelow200000;
  m_lTransactionNumberAbove200000 = pDayLineInfo->m_TransactionNumberAbove200000;
  m_lAttackBuyVolume = pDayLineInfo->m_AttackBuyVolume;
  m_lStrongBuyVolume = pDayLineInfo->m_StrongBuyVolume;
  m_lAttackSellVolume = pDayLineInfo->m_AttackSellVolume;
  m_lStrongSellVolume = pDayLineInfo->m_StrongSellVolume;
  m_lUnknownVolume = pDayLineInfo->m_UnknownVolume;
  m_lCancelBuyVolume = pDayLineInfo->m_CancelBuyVolume;
  m_lCancelSellVolume = pDayLineInfo->m_CancelSellVolume;
  m_lOrdinaryBuyVolume = pDayLineInfo->m_OrdinaryBuyVolume;
  m_lAttackBuyBelow50000 = pDayLineInfo->m_AttackBuyBelow50000;
  m_lAttackBuyBelow200000 = pDayLineInfo->m_AttackBuyBelow200000;
  m_lAttackBuyAbove200000 = pDayLineInfo->m_AttackBuyAbove200000;
  m_lOrdinarySellVolume = pDayLineInfo->m_OrdinarySellVolume;
  m_lAttackSellBelow50000 = pDayLineInfo->m_AttackSellBelow50000;
  m_lAttackSellBelow200000 = pDayLineInfo->m_AttackSellBelow200000;
  m_lAttackSellAbove200000 = pDayLineInfo->m_AttackSellAbove200000;
}

void CDayLine::Reset(void) {
  m_lDay = 0;		// 类型
  m_time = 0;
  m_wMarket = 0;
  m_lLastClose = m_lOpen = m_lHigh = m_lLow = m_lClose = 0;							// 收盘价
  m_lVolume = 0;
  m_lAmount = 0;
  m_fUpDown = 0;
  m_dUpDownRate = 0;
  m_lCurrentValue = m_lTotalValue = 0;
  m_fChangeHandRate = 0;

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