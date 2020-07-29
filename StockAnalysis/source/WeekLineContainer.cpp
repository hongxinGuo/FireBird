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
  LoadBasicInfo(&setWeekLineBasicInfo);
  setWeekLineBasicInfo.Close();

  // 装入WeekLineInfo数据
  setWeekLineExtendInfo.m_strFilter = _T("[StockCode] = '");
  setWeekLineExtendInfo.m_strFilter += m_vHistoryData.at(0)->GetStockCode();
  setWeekLineExtendInfo.m_strFilter += _T("'");
  setWeekLineExtendInfo.m_strSort = _T("[Day]");
  setWeekLineExtendInfo.Open();
  LoadExtendInfo(&setWeekLineExtendInfo);
  setWeekLineExtendInfo.Close();

  m_fDataLoaded = true;
  return true;
}

bool CWeekLineContainer::SaveData(void) {
  SaveBasicInfo();
  SaveExtendInfo();

  return true;
}

bool CWeekLineContainer::SaveBasicInfo() {
  CSetWeekLineBasicInfo setWeekLineBasicInfo;
  size_t lSize = 0;
  CWeekLinePtr pWeekLine = nullptr;

  ASSERT(m_vHistoryData.size() > 0);

  lSize = m_vHistoryData.size();
  setWeekLineBasicInfo.m_strFilter = _T("[StockCode] = '");
  setWeekLineBasicInfo.m_strFilter += m_vHistoryData.at(0)->GetStockCode() + _T("'");
  setWeekLineBasicInfo.m_strSort = _T("[Day]");

  setWeekLineBasicInfo.Open();
  setWeekLineBasicInfo.m_pDatabase->BeginTrans();
  for (int i = 0; i < lSize; i++) {
    pWeekLine = GetData(i);
    pWeekLine->AppendData(&setWeekLineBasicInfo);
  }
  setWeekLineBasicInfo.m_pDatabase->CommitTrans();
  setWeekLineBasicInfo.Close();

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 只存储有交易记录的扩展数据。对于没有信息的直接跨过。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CWeekLineContainer::SaveExtendInfo() {
  CSetWeekLineExtendInfo setWeekLineExtendInfo;
  size_t lSize = 0;
  CWeekLinePtr pWeekLine = nullptr;

  ASSERT(m_vHistoryData.size() > 0);

  lSize = m_vHistoryData.size();
  setWeekLineExtendInfo.m_strFilter = _T("[StockCode] = '");
  setWeekLineExtendInfo.m_strFilter += m_vHistoryData.at(0)->GetStockCode() + _T("'");
  setWeekLineExtendInfo.m_strSort = _T("[Day]");

  setWeekLineExtendInfo.Open();
  setWeekLineExtendInfo.m_pDatabase->BeginTrans();
  for (int i = 0; i < lSize; i++) {
    pWeekLine = GetData(i);
    if (pWeekLine->GetTransactionNumber() > 0) { // 只存储有交易记录的数据。
      pWeekLine->AppendData(&setWeekLineExtendInfo);
    }
  }
  setWeekLineExtendInfo.m_pDatabase->CommitTrans();
  setWeekLineExtendInfo.Close();

  return true;
}

bool CWeekLineContainer::LoadBasicInfo(CSetWeekLineBasicInfo* psetWeekLineBasicInfo) {
  CWeekLinePtr pWeekLine;

  if (gl_fNormalMode) ASSERT(!m_fLoadDataFirst);
  // 装入WeekLine数据
  m_vHistoryData.clear();
  while (!psetWeekLineBasicInfo->IsEOF()) {
    pWeekLine = make_shared<CWeekLine>();
    pWeekLine->LoadBasicData(psetWeekLineBasicInfo);
    StoreData(pWeekLine);
    psetWeekLineBasicInfo->MoveNext();
  }
  m_fLoadDataFirst = true;
  return true;
}

bool CWeekLineContainer::LoadExtendInfo(CSetWeekLineExtendInfo* psetWeekLineExtendInfo) {
  CWeekLinePtr pWeekLine;
  int iPosition = 0;

  if (gl_fNormalMode) ASSERT(m_fLoadDataFirst);

  while (!psetWeekLineExtendInfo->IsEOF()) {
    pWeekLine = GetData(iPosition);
    while ((pWeekLine->GetFormatedMarketDay() < psetWeekLineExtendInfo->m_Day)
           && (m_vHistoryData.size() > (iPosition + 1))) {
      iPosition++;
      pWeekLine = GetData(iPosition);
    }
    if (pWeekLine->GetFormatedMarketDay() == psetWeekLineExtendInfo->m_Day) {
      pWeekLine->LoadExtendData(psetWeekLineExtendInfo);
    }
    if (m_vHistoryData.size() <= (iPosition + 1)) break;
    psetWeekLineExtendInfo->MoveNext();
  }

  m_fLoadDataFirst = false;
  return true;
}

bool CWeekLineContainer::StoreData(vector<CWeekLinePtr>& vWeekLine) {
  CWeekLinePtr pWeekLine = nullptr;
  for (int i = 0; i < vWeekLine.size(); i++) {
    pWeekLine = vWeekLine.at(i);
    StoreData(pWeekLine);
  }

  return true;
}