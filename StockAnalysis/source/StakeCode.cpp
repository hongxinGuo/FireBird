#include "StakeCode.h"

CStakeCode::CStakeCode() : CObject() {
  m_wMarket = 0;
  m_strStakeCode = _T("");
  m_strStakeName = _T("");
  m_lDayLineEndDate = 19900101;
  m_lDayLineStartDate = 29900101;
  m_lIPOStatus = __STAKE_NOT_CHECKED__;
  m_lOffsetInContainer = 0;
  m_nHand = 100;
}

CStakeCode::~CStakeCode(void) {
}

void CStakeCode::LoadStakeCodeDB(CSetStakeCode& setStakeCode) {
  m_wMarket = setStakeCode.m_StakeType;
  m_strStakeCode = setStakeCode.m_StakeCode;
  CString str = setStakeCode.m_StakeName; // ��str�м����һ�£��Ϳ��Զ�ȡUniCode��ʽ��m_StockName�ˡ�
  m_strStakeName = str;
  m_lIPOStatus = setStakeCode.m_IPOStatus;
  m_lDayLineStartDate = setStakeCode.m_DayLineStartDate;
  if (m_lDayLineEndDate < setStakeCode.m_DayLineEndDate) { // ��ʱһ����Ʊ���ж����¼������������Ϊ׼��
    m_lDayLineEndDate = setStakeCode.m_DayLineEndDate;
  }
  m_lIPOStatus = setStakeCode.m_IPOStatus;
}

void CStakeCode::SaveToStakeCodeDB(CSetStakeCode& setStakeCode) {
  ASSERT(setStakeCode.IsOpen());
  setStakeCode.AddNew();
  setStakeCode.m_StakeType = m_wMarket;
  setStakeCode.m_StakeCode = m_strStakeCode;
  setStakeCode.m_StakeName = m_strStakeName;
  setStakeCode.m_DayLineStartDate = m_lDayLineStartDate;
  setStakeCode.m_DayLineEndDate = m_lDayLineEndDate;
  setStakeCode.m_IPOStatus = m_lIPOStatus;
  setStakeCode.Update();
}