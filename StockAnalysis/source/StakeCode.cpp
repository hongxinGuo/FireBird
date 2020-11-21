#include "StakeCode.h"

void CStakeCode::LoadStakeCodeDB(CSetStakeCode& setStakeCode) {
  m_strStakeCode = setStakeCode.m_StakeCode;
  CString str = setStakeCode.m_StakeName; // 用str中间过渡一下，就可以读取UniCode制式的m_StockName了。
  m_strStakeName = str;
  m_lIPOStatus = setStakeCode.m_IPOStatus;
  m_lDayLineStartDate = setStakeCode.m_DayLineStartDate;
  if (m_lDayLineEndDate < setStakeCode.m_DayLineEndDate) { // 有时一个股票会有多个记录，以最后的日期为准。
    m_lDayLineEndDate = setStakeCode.m_DayLineEndDate;
  }
  m_wMarket = setStakeCode.m_StakeType;
}

void CStakeCode::SaveToStakeCodeDB(CSetStakeCode& setStakeCode) {
  ASSERT(setStakeCode.IsOpen());
  setStakeCode.AddNew();
  setStakeCode.m_StakeCode = m_strStakeCode;
  setStakeCode.m_StakeName = m_strStakeName;
  setStakeCode.m_StakeType = m_wMarket;
  setStakeCode.Update();
}