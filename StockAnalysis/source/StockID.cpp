#include "StockID.h"

CStockID::CStockID() {
  m_wMarket = 0;
  m_strStockCode = "";
  m_nIndex = -1;
  m_lDayLineStartDay = 19900101; // 
  m_lDayLineEndDay = 19900101; // 
  m_lNewestDayLineDay = 19900101; // 最新的日线数据日。如果处理了实时数据，则此日期要新于m_lDayLineEndDay
  m_lIPOed = __STOCK_NOT_CHECKED__;   // 默认状态为无效股票代码。
  m_fActive = false;
  m_fDayLineNeedUpdated = true;
  m_fInquiriingOnce = false;
}

CStockID::~CStockID() {

}
