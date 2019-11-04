#include "StockBasicInfo.h"

CStockBasicInfo::CStockBasicInfo()
{
  Reset();
}

CStockBasicInfo::~CStockBasicInfo()
{
}

void CStockBasicInfo::Reset(void)
{
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