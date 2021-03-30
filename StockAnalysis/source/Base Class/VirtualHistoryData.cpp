#include"pch.h"

#include"VirtualHistoryData.h"

CVirtualHistoryData::CVirtualHistoryData() {
  Reset();
}

CVirtualHistoryData::~CVirtualHistoryData() {
}

void CVirtualHistoryData::Reset() {
  m_lDate = 0;		// 类型
  m_time = 0;
  m_strStockSymbol = _T("");
  m_strStockName = _T("");
  m_lLastClose = m_lOpen = m_lHigh = m_lLow = m_lClose = 0; // 收盘价
  m_llVolume = 0;
  m_llAmount = 0;
  m_dUpDown = 0;
  m_dUpDownRate = 0;
  m_llCurrentValue = m_llTotalValue = 0;
  m_dChangeHandRate = 0;
}