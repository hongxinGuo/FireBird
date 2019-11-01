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
  m_ID.Reset();

  m_lLastClose = m_lOpen = 0;
  m_lHigh = m_lLow = m_lNew = 0;
  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = m_lPSell.at(i) = 0;
    m_lVBuy.at(i) = m_lVSell.at(i) = 0;
  }
  m_llVolume = 0;
  m_llAmount = 0;
  m_dRelativeStrong = 0;
  m_nHand = 100;

  m_lHigh = m_lNew = m_lLow = 0;
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