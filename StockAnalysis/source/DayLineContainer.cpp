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
//	��������ʷ���ݴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//  �˺����������̵߳��ã���Ҫע������ͬ�����⡣
// �����ھ�������ʷ����ʱ��������ֻ�Ǹ��¡�
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
  for (int i = 0; i < lSize; i++) { // ����������洢�ģ���Ҫ��ͷ����ʼ�洢
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

  // װ��DL����
  setDLBasicInfo.m_strFilter = _T("[StockCode] = '");
  setDLBasicInfo.m_strFilter += strStockCode;
  setDLBasicInfo.m_strFilter += _T("'");
  setDLBasicInfo.m_strSort = _T("[Date]");
  setDLBasicInfo.Open();
  LoadDLBasicInfo(&setDLBasicInfo);
  setDLBasicInfo.Close();

  // װ��DLInfo����
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
  // װ��DL����
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
// װ��DLInfo�������װ��DL��֮��
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
// ��������������
//
/////////////////////////////////////////////////////////////////////////////////////
void CDLContainer::UpdateData(vector<CDLPtr>& vTempDL) {
  CDLPtr pDL = nullptr;
  Unload(); // �����������������ݣ�����еĻ���
  // ������������ʱ��Ϊ�������
  for (int i = 0; i < vTempDL.size(); i++) {
    pDL = vTempDL.at(i);
    if (pDL->IsActive()) {
      // ��������ٽ��ף�ͣ�ƻ����к���ֵģ��Ĺ�Ʊ����
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
  if (lNextMonday < lNewestDay) { // �м�����
    while (GetData(lCurrentDLPos)->GetFormatedMarketDate() < lNextMonday) {
      pWeekLine->UpdateWeekLine(GetData(lCurrentDLPos++));
    }
  }
  else { // ���һ������
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