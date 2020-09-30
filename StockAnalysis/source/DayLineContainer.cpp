#include"globedef.h"

#include "DayLineContainer.h"

CDLContainer::CDLContainer() {
}

CDLContainer::~CDLContainer() {
}

bool CDLContainer::SaveData(CString strStockCode) {
  SaveDLBasicInfo(strStockCode);

  return true;
}

bool CDLContainer::LoadData(CString strStockCode) {
  LoadDL(strStockCode);

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线历史数据存入数据库．默认数据库为空。
//  此函数被工作线程调用，需要注意数据同步问题。
// 当存在旧日线历史数据时，本函数只是更新。
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CDLContainer::SaveDLBasicInfo(CString strStockCode) {
  CSetDLBasicInfo setDLBasicInfo;
  size_t lSize = 0;
  vector<CDLPtr> vDL;
  CDLPtr pDL = nullptr;
  long lCurrentPos = 0, lSizeOfOldDL = 0;
  bool fNeedUpdate = false;

  ASSERT(GetDataSize() > 0);

  lSize = GetDataSize();
  setDLBasicInfo.m_strFilter = _T("[StockCode] = '");
  setDLBasicInfo.m_strFilter += strStockCode + _T("'");
  setDLBasicInfo.m_strSort = _T("[Date]");

  setDLBasicInfo.Open();
  while (!setDLBasicInfo.IsEOF()) {
    pDL = make_shared<CDL>();
    pDL->LoadBasicData(&setDLBasicInfo);
    vDL.push_back(pDL);
    lCurrentPos++;
    setDLBasicInfo.MoveNext();
  }
  setDLBasicInfo.Close();

  lSizeOfOldDL = lCurrentPos;
  lCurrentPos = 0;
  setDLBasicInfo.m_strFilter = _T("[ID] = 1");
  setDLBasicInfo.Open();
  setDLBasicInfo.m_pDatabase->BeginTrans();
  for (int i = 0; i < lSize; i++) { // 数据是正序存储的，需要从头部开始存储
    pDL = GetData(i);
    while ((lCurrentPos < lSizeOfOldDL) && (vDL.at(lCurrentPos)->GetFormatedMarketDate() < pDL->GetFormatedMarketDate())) lCurrentPos++;
    if (lCurrentPos < lSizeOfOldDL) {
      if (vDL.at(lCurrentPos)->GetFormatedMarketDate() > pDL->GetFormatedMarketDate()) {
        pDL->AppendData(&setDLBasicInfo);
        fNeedUpdate = true;
      }
    }
    else {
      pDL->AppendData(&setDLBasicInfo);
      fNeedUpdate = true;
    }
  }
  setDLBasicInfo.m_pDatabase->CommitTrans();
  setDLBasicInfo.Close();

  return fNeedUpdate;
}

bool CDLContainer::LoadDL(CString strStockCode) {
  CSetDLBasicInfo setDLBasicInfo;
  CSetDLExtendInfo setDLExtendInfo;

  ASSERT(!m_fLoadDataFirst);

  // 装入DL数据
  setDLBasicInfo.m_strFilter = _T("[StockCode] = '");
  setDLBasicInfo.m_strFilter += strStockCode;
  setDLBasicInfo.m_strFilter += _T("'");
  setDLBasicInfo.m_strSort = _T("[Date]");
  setDLBasicInfo.Open();
  LoadDLBasicInfo(&setDLBasicInfo);
  setDLBasicInfo.Close();

  // 装入DLInfo数据
  setDLExtendInfo.m_strFilter = _T("[StockCode] = '");
  setDLExtendInfo.m_strFilter += strStockCode;
  setDLExtendInfo.m_strFilter += _T("'");
  setDLExtendInfo.m_strSort = _T("[Date]");
  setDLExtendInfo.Open();
  LoadDLExtendInfo(&setDLExtendInfo);
  setDLExtendInfo.Close();

  m_fDataLoaded = true;
  ASSERT(!m_fLoadDataFirst);
  return true;
}

bool CDLContainer::LoadDLBasicInfo(CSetDLBasicInfo* psetDLBasicInfo) {
  CDLPtr pDL;

  if (gl_fNormalMode) ASSERT(!m_fLoadDataFirst);
  // 装入DL数据
  Unload();
  while (!psetDLBasicInfo->IsEOF()) {
    pDL = make_shared<CDL>();
    pDL->LoadBasicData(psetDLBasicInfo);
    StoreData(pDL);
    psetDLBasicInfo->MoveNext();
  }
  m_fLoadDataFirst = true;
  return true;
}

/////////////////////////////////////////////////////////////////////////////
//
// 装载DLInfo表必须在装载DL表之后。
//
//
////////////////////////////////////////////////////////////////////////////
bool CDLContainer::LoadDLExtendInfo(CSetDLExtendInfo* psetDLExtendInfo) {
  CDLPtr pDL;
  int iPosition = 0;

  if (gl_fNormalMode) ASSERT(m_fLoadDataFirst);

  while (!psetDLExtendInfo->IsEOF()) {
    pDL = GetData(iPosition);
    while ((pDL->GetFormatedMarketDate() < psetDLExtendInfo->m_Date)
           && (GetDataSize() > (iPosition + 1))) {
      iPosition++;
      pDL = GetData(iPosition);
    }
    if (pDL->GetFormatedMarketDate() == psetDLExtendInfo->m_Date) {
      pDL->LoadExtendData(psetDLExtendInfo);
    }
    if (GetDataSize() <= (iPosition + 1)) break;
    psetDLExtendInfo->MoveNext();
  }
  m_fLoadDataFirst = false;
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CDLContainer::UpdateData(vector<CDLPtr>& vTempDL) {
  CDLPtr pDL = nullptr;
  Unload(); // 清除已载入的日线数据（如果有的话）
  // 将日线数据以时间为正序存入
  for (int i = 0; i < vTempDL.size(); i++) {
    pDL = vTempDL.at(i);
    if (pDL->IsActive()) {
      // 清除掉不再交易（停牌或退市后出现的）的股票日线
      StoreData(pDL);
    }
  }
  SetDataLoaded(true);
}

bool CDLContainer::BuildWeekLine(vector<CWeekLinePtr>& vWeekLine) {
  ASSERT(IsDataLoaded());
  ASSERT(GetDataSize() > 0);
  long lCurrentDLPos = 0;
  CWeekLinePtr pWeekLine = nullptr;

  vWeekLine.clear();
  do {
    pWeekLine = CreateNewWeekLine(lCurrentDLPos);
    vWeekLine.push_back(pWeekLine);
  } while (lCurrentDLPos < GetDataSize());

  return true;
}

CWeekLinePtr CDLContainer::CreateNewWeekLine(long& lCurrentDLPos) {
  ASSERT(GetDataSize() > 0);
  ASSERT(lCurrentDLPos < GetDataSize());

  long lNextMonday = GetNextMonday(GetData(lCurrentDLPos)->GetFormatedMarketDate());
  long lNewestDay = GetData(GetDataSize() - 1)->GetFormatedMarketDate();
  CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
  if (lNextMonday < lNewestDay) { // 中间数据
    while (GetData(lCurrentDLPos)->GetFormatedMarketDate() < lNextMonday) {
      pWeekLine->UpdateWeekLine(GetData(lCurrentDLPos++));
    }
  }
  else { // 最后一组数据
    while (lCurrentDLPos <= (GetDataSize() - 1)) {
      pWeekLine->UpdateWeekLine(GetData(lCurrentDLPos++));
    }
  }

  if (pWeekLine->GetLastClose() > 0) {
    pWeekLine->SetUpDownRate(pWeekLine->GetUpDown() * 100 * 1000 / pWeekLine->GetLastClose());
  }
  else {
    pWeekLine->SetUpDownRate(pWeekLine->GetUpDown() * 100 * 1000 / pWeekLine->GetOpen());
  }

  return pWeekLine;
}