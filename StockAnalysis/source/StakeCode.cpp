#include "StakeCode.h"

CStakeCode::CStakeCode() : CObject() {
  m_wMarket = 0;
  m_strStakeCode = _T("");
  m_strStakeName = _T("");
  m_lDayLineEndDate = 19900101;
  m_lDayLineStartDate = 19900101;
  m_lIPOStatus = __STAKE_NOT_CHECKED__;
  m_lOffsetInContainer = 0;
  m_nHand = 100;
}

CStakeCode::~CStakeCode(void) {
}