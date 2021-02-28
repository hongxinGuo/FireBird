#include"globedef.h"

#include "WeekLineContainer.h"
#include"WeekLine.h"

#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"

bool CWeekLineContainer::LoadData(CString strStockCode) {
  LoadWeekLine(strStockCode);

  return true;
}

bool CWeekLineContainer::SaveData(CString strStockCode) {
  SaveWeekLine();

  return true;
}

bool CWeekLineContainer::SaveWeekLine(void) {
  SaveBasicInfo();
  SaveExtendInfo();

  return true;
}

bool CWeekLineContainer::SaveCurrentWeekLine(void) {
  CSetWeekLineInfo setWeekLineInfo;
  CWeekLinePtr pWeekLine = nullptr;

  ASSERT(m_vHistoryData.size() > 0);

  setWeekLineInfo.m_strFilter = _T("[ID] = 1");
  setWeekLineInfo.Open();
  setWeekLineInfo.m_pDatabase->BeginTrans();
  for (auto pData : m_vHistoryData) {
    (dynamic_pointer_cast<CWeekLine>(pData))->AppendData(&setWeekLineInfo);
  }
  setWeekLineInfo.m_pDatabase->CommitTrans();
  setWeekLineInfo.Close();
  TRACE("存储了%d个当前周周线数据\n", m_vHistoryData.size());

  return true;
}

bool CWeekLineContainer::SaveBasicInfo(void) {
  CSetWeekLineBasicInfo setWeekLineBasicInfo;
  CWeekLinePtr pWeekLine = nullptr;

  ASSERT(m_vHistoryData.size() > 0);

  setWeekLineBasicInfo.m_strFilter = _T("[ID] = 1");
  setWeekLineBasicInfo.Open();
  setWeekLineBasicInfo.m_pDatabase->BeginTrans();
  for (auto pData : m_vHistoryData) {
    (dynamic_pointer_cast<CWeekLine>(pData))->AppendData(&setWeekLineBasicInfo);
  }
  setWeekLineBasicInfo.m_pDatabase->CommitTrans();
  setWeekLineBasicInfo.Close();
  TRACE("存储了%d个周线基本数据\n", m_vHistoryData.size());

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 只存储有交易记录的扩展数据。对于没有信息的直接跨过。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CWeekLineContainer::SaveExtendInfo(void) {
  CSetWeekLineExtendInfo setWeekLineExtendInfo;
  CWeekLinePtr pWeekLine = nullptr;

  ASSERT(m_vHistoryData.size() > 0);

  setWeekLineExtendInfo.m_strFilter = _T("[ID] = 1");
  setWeekLineExtendInfo.Open();
  setWeekLineExtendInfo.m_pDatabase->BeginTrans();
  for (auto pData : m_vHistoryData) {
    (dynamic_pointer_cast<CWeekLine>(pData))->AppendData(&setWeekLineExtendInfo);
  }
  setWeekLineExtendInfo.m_pDatabase->CommitTrans();
  setWeekLineExtendInfo.Close();
  TRACE("存储了%d个周线扩展数据\n", m_vHistoryData.size());

  return true;
}

bool CWeekLineContainer::LoadWeekLine(CString strStockCode) {
  CSetWeekLineBasicInfo setWeekLineBasicInfo;
  CSetWeekLineExtendInfo setWeekLineExtendInfo;

  ASSERT(!m_fLoadDataFirst);

  // 装入WeekLine数据
  setWeekLineBasicInfo.m_strFilter = _T("[StockCode] = '");
  setWeekLineBasicInfo.m_strFilter += strStockCode;
  setWeekLineBasicInfo.m_strFilter += _T("'");
  setWeekLineBasicInfo.m_strSort = _T("[Date]");
  setWeekLineBasicInfo.Open();
  LoadBasicInfo(&setWeekLineBasicInfo);
  setWeekLineBasicInfo.Close();

  // 装入WeekLineInfo数据
  setWeekLineExtendInfo.m_strFilter = _T("[StockCode] = '");
  setWeekLineExtendInfo.m_strFilter += strStockCode;
  setWeekLineExtendInfo.m_strFilter += _T("'");
  setWeekLineExtendInfo.m_strSort = _T("[Date]");
  setWeekLineExtendInfo.Open();
  LoadExtendInfo(&setWeekLineExtendInfo);
  setWeekLineExtendInfo.Close();

  m_fDataLoaded = true;

  ASSERT(!m_fLoadDataFirst);

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
    while ((pWeekLine->GetFormatedMarketDate() < psetWeekLineExtendInfo->m_Date)
           && (m_vHistoryData.size() > (iPosition + 1))) {
      iPosition++;
      pWeekLine = GetData(iPosition);
    }
    if (pWeekLine->GetFormatedMarketDate() == psetWeekLineExtendInfo->m_Date) {
      pWeekLine->LoadExtendData(psetWeekLineExtendInfo);
    }
    if (m_vHistoryData.size() <= (iPosition + 1)) break;
    psetWeekLineExtendInfo->MoveNext();
  }

  m_fLoadDataFirst = false;
  return true;
}

bool CWeekLineContainer::StoreData(vector<CWeekLinePtr>& vWeekLine) {
  for (auto pWeekLine : vWeekLine) {
    StoreData(pWeekLine);
  }
  SetDataLoaded(true);

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CWeekLineContainer::UpdateData(vector<CWeekLinePtr>& vTempWeekLine) {
  Unload(); // 清除已载入的周线数据（如果有的话）
  // 将日线数据以时间为正序存入
  for (auto pWeekLine : vTempWeekLine) {
    StoreData(pWeekLine);
  }
  SetDataLoaded(true);
}

bool CWeekLineContainer::UpdateData(CDayLinePtr pDayLine) {
  for (auto pData : m_vHistoryData) {
    if (strcmp(pData->GetSymbol(), pDayLine->GetSymbol()) == 0) { //
      dynamic_pointer_cast<CWeekLine>(pData)->UpdateWeekLine(pDayLine);
      break;
    }
  }
  return true;
}