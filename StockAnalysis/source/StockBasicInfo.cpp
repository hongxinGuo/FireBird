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
  m_lUpDown = 0;
  m_dUpDownRate = 0;
  m_llTotalValue = m_llCurrentValue = 0;
  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = m_lPSell.at(i) = 0;
    m_lVBuy.at(i) = m_lVSell.at(i) = 0;
  }
  m_dRelativeStrong = 0;
}

void CStockBasicInfo::StoreBasicInfo(CSetDayLine* psetDayLine) {
  ASSERT(psetDayLine->IsOpen());
  tm tm_;
  localtime_s(&tm_, &m_TransactionTime);

  psetDayLine->m_Day = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
  psetDayLine->m_Market = m_wMarket;
  psetDayLine->m_StockCode = m_strStockCode;
  psetDayLine->m_StockName = m_strStockName;
  psetDayLine->m_LastClose = ConvertValueToString(m_lLastClose, 1000);
  psetDayLine->m_Open = ConvertValueToString(m_lOpen, 1000);
  psetDayLine->m_High = ConvertValueToString(m_lHigh, 1000);
  psetDayLine->m_Low = ConvertValueToString(m_lLow, 1000);
  psetDayLine->m_Close = ConvertValueToString(m_lNew, 1000);
  psetDayLine->m_Volume = ConvertValueToString(m_llVolume);
  psetDayLine->m_Amount = ConvertValueToString(m_llAmount);
  psetDayLine->m_UpAndDown = ConvertValueToString(m_lUpDown, 1000);
  psetDayLine->m_UpDownRate = ConvertValueToString(m_dUpDownRate);
  psetDayLine->m_TotalValue = ConvertValueToString(m_llTotalValue);
  psetDayLine->m_CurrentValue = ConvertValueToString(m_llCurrentValue);
}

void CStockBasicInfo::StoreTempInfo(CSetDayLineToday& setDayLineToday) {
  ASSERT(setDayLineToday.IsOpen());
  setDayLineToday.m_Day = gl_systemTime.GetDay();
  setDayLineToday.m_Market = m_wMarket;
  setDayLineToday.m_StockCode = m_strStockCode;
  setDayLineToday.m_StockName = m_strStockName;
  setDayLineToday.m_LastClose = ConvertValueToString(m_lLastClose, 1000);
  setDayLineToday.m_Open = ConvertValueToString(m_lOpen, 1000);
  setDayLineToday.m_High = ConvertValueToString(m_lHigh, 1000);
  setDayLineToday.m_Low = ConvertValueToString(m_lLow, 1000);
  setDayLineToday.m_Close = ConvertValueToString(m_lNew, 1000);
  setDayLineToday.m_Volume = ConvertValueToString(m_llVolume);
  setDayLineToday.m_Amount = ConvertValueToString(m_llAmount);
  setDayLineToday.m_UpAndDown = ConvertValueToString(m_lUpDown, 1000);
  setDayLineToday.m_UpDownRate = ConvertValueToString(m_dUpDownRate);
  setDayLineToday.m_TotalValue = ConvertValueToString(m_llTotalValue);
  setDayLineToday.m_CurrentValue = ConvertValueToString(m_llCurrentValue);
}

void CStockBasicInfo::UpdateStatus(CRTDataPtr pRTData) {
  SetTransactionTime(pRTData->GetTransactionTime());
  SetLastClose(pRTData->GetLastClose());
  SetNew(pRTData->GetNew());
  SetHigh(pRTData->GetHigh());
  SetLow(pRTData->GetLow());
  SetOpen(pRTData->GetOpen());
  SetVolume(pRTData->GetVolume());
  SetAmount(pRTData->GetAmount());
  SetUpDown(m_lNew - m_lLastClose);
  if (m_lLastClose != 0) SetUpDownRate((double)m_lUpDown * 100 / m_lLastClose);
  else SetUpDownRate(0);
  for (int i = 0; i < 5; i++) {
    SetPBuy(i, pRTData->GetPBuy(i));
    SetVBuy(i, pRTData->GetVBuy(i));
    SetPSell(i, pRTData->GetPSell(i));
    SetVSell(i, pRTData->GetVSell(i));
  }
}