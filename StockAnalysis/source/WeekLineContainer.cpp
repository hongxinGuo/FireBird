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
  SaveWeekLine(strStockCode);

  return true;
}

bool CWeekLineContainer::SaveWeekLine(CString strStockCode) {
  SaveBasicInfo(strStockCode);
  SaveExtendInfo(strStockCode);

  return true;
}

bool CWeekLineContainer::SaveBasicInfo(CString strStockCode) {
  CSetWeekLineBasicInfo setWeekLineBasicInfo;
  size_t lSize = 0;
  CWeekLinePtr pWeekLine = nullptr;

  ASSERT(m_vHistoryData.size() > 0);

  lSize = m_vHistoryData.size();
  setWeekLineBasicInfo.m_strFilter = _T("[StockCode] = '");
  setWeekLineBasicInfo.m_strFilter += strStockCode + _T("'");
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
// ֻ�洢�н��׼�¼����չ���ݡ�����û����Ϣ��ֱ�ӿ����
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CWeekLineContainer::SaveExtendInfo(CString strStockCode) {
  CSetWeekLineExtendInfo setWeekLineExtendInfo;
  size_t lSize = 0;
  CWeekLinePtr pWeekLine = nullptr;

  ASSERT(m_vHistoryData.size() > 0);

  lSize = m_vHistoryData.size();
  setWeekLineExtendInfo.m_strFilter = _T("[StockCode] = '");
  setWeekLineExtendInfo.m_strFilter += strStockCode + _T("'");
  setWeekLineExtendInfo.m_strSort = _T("[Day]");

  setWeekLineExtendInfo.Open();
  setWeekLineExtendInfo.m_pDatabase->BeginTrans();
  for (int i = 0; i < lSize; i++) {
    pWeekLine = GetData(i);
    if (pWeekLine->GetTransactionNumber() > 0) { // ֻ�洢�н��׼�¼�����ݡ�
      pWeekLine->AppendData(&setWeekLineExtendInfo);
    }
  }
  setWeekLineExtendInfo.m_pDatabase->CommitTrans();
  setWeekLineExtendInfo.Close();

  return true;
}

bool CWeekLineContainer::LoadWeekLine(CString strStockCode) {
  CSetWeekLineBasicInfo setWeekLineBasicInfo;
  CSetWeekLineExtendInfo setWeekLineExtendInfo;

  // װ��WeekLine����
  setWeekLineBasicInfo.m_strFilter = _T("[StockCode] = '");
  setWeekLineBasicInfo.m_strFilter += strStockCode;
  setWeekLineBasicInfo.m_strFilter += _T("'");
  setWeekLineBasicInfo.m_strSort = _T("[Day]");
  setWeekLineBasicInfo.Open();
  LoadBasicInfo(&setWeekLineBasicInfo);
  setWeekLineBasicInfo.Close();

  // װ��WeekLineInfo����
  setWeekLineExtendInfo.m_strFilter = _T("[StockCode] = '");
  setWeekLineExtendInfo.m_strFilter += strStockCode;
  setWeekLineExtendInfo.m_strFilter += _T("'");
  setWeekLineExtendInfo.m_strSort = _T("[Day]");
  setWeekLineExtendInfo.Open();
  LoadExtendInfo(&setWeekLineExtendInfo);
  setWeekLineExtendInfo.Close();

  m_fDataLoaded = true;
  return true;
}

bool CWeekLineContainer::LoadBasicInfo(CSetWeekLineBasicInfo* psetWeekLineBasicInfo) {
  CWeekLinePtr pWeekLine;

  if (gl_fNormalMode) ASSERT(!m_fLoadDataFirst);
  // װ��WeekLine����
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
  SetDataLoaded(true);

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// ��������������
//
/////////////////////////////////////////////////////////////////////////////////////
void CWeekLineContainer::UpdateData(vector<CWeekLinePtr>& vTempWeekLine) {
  CWeekLinePtr pWeekLine = nullptr;
  Unload(); // �����������������ݣ�����еĻ���
  // ������������ʱ��Ϊ�������
  for (int i = 0; i < vTempWeekLine.size(); i++) {
    pWeekLine = vTempWeekLine.at(i);
    StoreData(pWeekLine);
  }
  SetDataLoaded(true);
}