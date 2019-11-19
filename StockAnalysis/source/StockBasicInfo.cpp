#include "StockBasicInfo.h"

CStockBasicInfo::CStockBasicInfo() {
  Reset();
}

CStockBasicInfo::~CStockBasicInfo() {
}

void CStockBasicInfo::Reset(void) {
  m_wMarket = 0;
  m_strStockCode = _T("");
  m_strStockName = _T("");
  m_nOffsetInContainer = -1;
  m_lDayLineStartDay = 19900101; //
  m_lDayLineEndDay = 19900101; //
  m_lIPOed = __STOCK_NOT_CHECKED__;   // 默认状态为无效股票代码。
  m_nHand = 100;

  m_TransactionTime = 0;
  m_lLastClose = m_lOpen = 0;
  m_lHigh = m_lLow = m_lNew = 0;
  m_llVolume = 0;
  m_llAmount = 0;
  m_llTotalValue = m_llCurrentValue = 0;
  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = m_lPSell.at(i) = 0;
    m_lVBuy.at(i) = m_lVSell.at(i) = 0;
  }
  m_dRelativeStrong = 0;
}

void CStockBasicInfo::StoreTempInfo(CSetDayLine& setDayLine) {
  ASSERT(setDayLine.IsOpen());
  setDayLine.m_Market = m_wMarket;
  setDayLine.m_StockCode = m_strStockCode;
  setDayLine.m_StockName = m_strStockName;
  setDayLine.m_LastClose = ConvertValueToString(m_lLastClose, 1000);
  setDayLine.m_Open = ConvertValueToString(m_lOpen, 1000);
  setDayLine.m_High = ConvertValueToString(m_lHigh, 1000);
  setDayLine.m_Low = ConvertValueToString(m_lLow, 1000);
  setDayLine.m_Close = ConvertValueToString(m_lNew, 1000);
  setDayLine.m_Volume = ConvertValueToString(m_llVolume);
  setDayLine.m_Amount = ConvertValueToString(m_llAmount);
  setDayLine.m_TotalValue = ConvertValueToString(m_llTotalValue);
  setDayLine.m_CurrentValue = ConvertValueToString(m_llCurrentValue);
}

void CStockBasicInfo::StoreTempInfo(CSetDayLineToday& setDayLineToday) {
  ASSERT(setDayLineToday.IsOpen());
  setDayLineToday.m_Market = m_wMarket;
  setDayLineToday.m_StockCode = m_strStockCode;
  setDayLineToday.m_StockName = m_strStockName;
  setDayLineToday.m_Day = m_TransactionTime;
  setDayLineToday.m_LastClose = ConvertValueToString(m_lLastClose, 1000);
  setDayLineToday.m_Open = ConvertValueToString(m_lOpen, 1000);
  setDayLineToday.m_High = ConvertValueToString(m_lHigh, 1000);
  setDayLineToday.m_Low = ConvertValueToString(m_lLow, 1000);
  setDayLineToday.m_Close = ConvertValueToString(m_lNew, 1000);
  setDayLineToday.m_Volume = ConvertValueToString(m_llVolume);
  setDayLineToday.m_Amount = ConvertValueToString(m_llAmount);
  setDayLineToday.m_TotalValue = ConvertValueToString(m_llTotalValue);
  setDayLineToday.m_CurrentValue = ConvertValueToString(m_llCurrentValue);
}

void CStockBasicInfo::UpdateStatus(CRTDataPtr pRTData) {
  SetLastClose(pRTData->GetLastClose());
  SetNew(pRTData->GetNew());
  SetHigh(pRTData->GetHigh());
  SetLow(pRTData->GetLow());
  SetOpen(pRTData->GetOpen());
  SetVolume(pRTData->GetVolume());
  SetAmount(pRTData->GetAmount());
  for (int i = 0; i < 5; i++) {
    SetPBuy(i, pRTData->GetPBuy(i));
    SetVBuy(i, pRTData->GetVBuy(i));
    SetPSell(i, pRTData->GetPSell(i));
    SetVSell(i, pRTData->GetVSell(i));
  }
}