#include "StockID.h"

CStockID::CStockID() {
  Reset();
}

CStockID::~CStockID() {

}

void CStockID::Reset(void) {
  m_wMarket = 0;
  m_strStockCode = "";
  m_strStockName = "";
  m_nIndex = -1;
  m_lDayLineEndDay = 19900101; // 
  m_lIPOed = __STOCK_NOT_CHECKED__;   // Ĭ��״̬Ϊ��Ч��Ʊ���롣
  m_fActive = false;
  m_fDayLineNeedUpdate = true;
  m_fInquiringOnce = false;
  m_fHaveReadInName = false;
}
