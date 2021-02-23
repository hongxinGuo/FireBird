#include"globedef.h"
#include"accessory.h"
#include "VirtualStock.h"

CVirtualStock::CVirtualStock() : CObject() {
  Reset();
}

CVirtualStock::~CVirtualStock() {
}

void CVirtualStock::Reset(void) {
  m_fTodayNewStock = false;
  m_fUpdateStockProfileDB = false;
  m_fActive = false;
  m_lDayLineStartDate = 29900101;
  m_lDayLineEndDate = 19000101;
  m_lIPOStatus = __STAKE_NOT_CHECKED__;   // 默认状态为无效股票代码。

  m_fDayLineNeedUpdate = true; // 默认状态下日线需要更新
  m_fDayLineNeedProcess = false; // 初始状态为尚未读取日线历史数据，无需处理
  m_fDayLineNeedSaving = false;

}
