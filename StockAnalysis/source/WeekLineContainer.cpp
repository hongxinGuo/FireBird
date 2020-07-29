#include"globedef.h"

#include "WeekLineContainer.h"
#include"WeekLine.h"

#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"

bool CWeekLineContainer::LoadData() {
  CSetWeekLineBasicInfo setWeekLineBasicInfo;
  CSetWeekLineExtendInfo setWeekLineExtendInfo;

  // 装入WeekLine数据
  setWeekLineBasicInfo.m_strFilter = _T("[StockCode] = '");
  setWeekLineBasicInfo.m_strFilter += m_vHistoryData.at(0)->GetStockCode();
  setWeekLineBasicInfo.m_strFilter += _T("'");
  setWeekLineBasicInfo.m_strSort = _T("[Day]");
  setWeekLineBasicInfo.Open();
  LoadWeekLineBasicInfo(&setWeekLineBasicInfo);
  setWeekLineBasicInfo.Close();

  // 装入WeekLineInfo数据
  setWeekLineExtendInfo.m_strFilter = _T("[StockCode] = '");
  setWeekLineExtendInfo.m_strFilter += m_vHistoryData.at(0)->GetStockCode();
  setWeekLineExtendInfo.m_strFilter += _T("'");
  setWeekLineExtendInfo.m_strSort = _T("[Day]");
  setWeekLineExtendInfo.Open();
  LoadWeekLineExtendInfo(&setWeekLineExtendInfo);
  setWeekLineExtendInfo.Close();

  m_fDataLoaded = true;
  return true;
}

bool CWeekLineContainer::LoadWeekLineBasicInfo(CSetWeekLineBasicInfo* psetWeekLineBasicInfo) {
  CWeekLinePtr pWeekLine;

  if (gl_fNormalMode) ASSERT(!m_fLoadDataFirst);
  // 装入WeekLine数据
  m_vHistoryData.clear();
  while (!psetWeekLineBasicInfo->IsEOF()) {
    pWeekLine = make_shared<CWeekLine>();
    pWeekLine->LoadBasicData(psetWeekLineBasicInfo);
    //m_vHistoryData.push_back(pWeekLine);
    psetWeekLineBasicInfo->MoveNext();
  }
  m_fLoadDataFirst = true;
  return true;
}

bool CWeekLineContainer::LoadWeekLineExtendInfo(CSetWeekLineExtendInfo* psetWeekLineExtendInfo) {
  CWeekLinePtr pWeekLine;
  int iPosition = 0;

  if (gl_fNormalMode) ASSERT(m_fLoadDataFirst);
  /*
  while (!psetWeekLineExtendInfo->IsEOF()) {
    pWeekLine = m_vHistoryData.at(iPosition);
    while ((pWeekLine->GetFormatedMarketDay() < psetWeekLineExtendInfo->m_Day)
           && (m_vHistoryData.size() > (iPosition + 1))) {
      iPosition++;
      pWeekLine = m_vHistoryData.at(iPosition);
    }
    if (pWeekLine->GetFormatedMarketDay() == psetWeekLineExtendInfo->m_Day) {
      pWeekLine->LoadExtendData(psetWeekLineExtendInfo);
    }
    if (m_vHistoryData.size() <= (iPosition + 1)) break;
    psetWeekLineExtendInfo->MoveNext();
  }
  */
  m_fLoadDataFirst = false;
  return true;
}