#include "StockID.h"

CStockID::CStockID() {
  m_wMarket = 0;
  m_strStockCode = "";
  m_nIndex = -1;
  m_lDayLineStartDay = 19900101; // 
  m_lDayLineEndDay = 19900101; // 
  m_lNewestDayLineDay = 19900101; // ���µ����������ա����������ʵʱ���ݣ��������Ҫ����m_lDayLineEndDay
  m_lIPOed = __STOCK_NOT_CHECKED__;   // Ĭ��״̬Ϊ��Ч��Ʊ���롣
  m_fActive = false;
  m_fDayLineNeedUpdated = true;
  m_fInquiriingOnce = false;
}

CStockID::~CStockID() {

}
