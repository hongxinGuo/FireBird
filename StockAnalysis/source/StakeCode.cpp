#include "StakeCode.h"

void CStakeCode::LoadStakeCodeDB(CSetStakeCode& setStakeCode) {
  m_strStakeCode = setStakeCode.m_StakeCode;
  CString str = setStakeCode.m_StakeName; // ��str�м����һ�£��Ϳ��Զ�ȡUniCode��ʽ��m_StockName�ˡ�
  m_strStakeName = str;
  m_lIPOStatus = setStakeCode.m_IPOStatus;
  m_lDayLineStartDate = setStakeCode.m_DayLineStartDate;
  if (m_lDayLineEndDate < setStakeCode.m_DayLineEndDate) { // ��ʱһ����Ʊ���ж����¼������������Ϊ׼��
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