#include "DayLineContainer.h"

bool CDayLineContainer::SaveData(void) {
  return false;
}

bool CDayLineContainer::LoadData(void) {
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��������ʷ���ݴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//  �˺����������̵߳��ã���Ҫע������ͬ�����⡣
// �����ھ�������ʷ����ʱ��������ֻ�Ǹ��¡�
//
//////////////////////////////////////////////////////////////////////////////////////////

bool CDayLineContainer::SaveDayLineBasicInfo(void) {
  CSetDayLineBasicInfo setDayLineBasicInfo;
  size_t lSize = 0;
  vector<CDayLinePtr> vDayLine;
  CDayLinePtr pDayLine = nullptr;
  long lCurrentPos = 0, lSizeOfOldDayLine = 0;
  bool fNeedUpdate = false;

  ASSERT(GetDataSize() > 0);

  lSize = GetDataSize();
  setDayLineBasicInfo.m_strFilter = _T("[StockCode] = '");
  setDayLineBasicInfo.m_strFilter += GetData(0)->GetStockCode() + _T("'");
  setDayLineBasicInfo.m_strSort = _T("[Day]");

  setDayLineBasicInfo.Open();
  while (!setDayLineBasicInfo.IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->LoadBasicData(&setDayLineBasicInfo);
    vDayLine.push_back(pDayLine);
    lCurrentPos++;
    setDayLineBasicInfo.MoveNext();
  }
  setDayLineBasicInfo.Close();

  lSizeOfOldDayLine = lCurrentPos;
  lCurrentPos = 0;
  setDayLineBasicInfo.Open();
  setDayLineBasicInfo.m_pDatabase->BeginTrans();
  for (int i = 0; i < lSize; i++) { // ����������洢�ģ���Ҫ��ͷ����ʼ�洢
    pDayLine = GetData(i);
    while ((lCurrentPos < lSizeOfOldDayLine) && (vDayLine.at(lCurrentPos)->GetFormatedMarketDay() < pDayLine->GetFormatedMarketDay())) lCurrentPos++;
    if (lCurrentPos < lSizeOfOldDayLine) {
      if (vDayLine.at(lCurrentPos)->GetFormatedMarketDay() > pDayLine->GetFormatedMarketDay()) {
        pDayLine->AppendData(&setDayLineBasicInfo);
        fNeedUpdate = true;
      }
    }
    else {
      pDayLine->AppendData(&setDayLineBasicInfo);
      fNeedUpdate = true;
    }
  }
  setDayLineBasicInfo.m_pDatabase->CommitTrans();
  setDayLineBasicInfo.Close();

  return fNeedUpdate;
}

bool CDayLineContainer::BuildWeekLine(vector<CWeekLinePtr>& vWeekLine) {
  ASSERT(IsDataLoaded());
  ASSERT(GetDataSize() > 0);
  long i = 0;
  CWeekLinePtr pWeekLine = nullptr;

  vWeekLine.clear();
  do {
    pWeekLine = CreateNewWeekLine(i);
    vWeekLine.push_back(pWeekLine);
  } while (i < GetDataSize());

  return true;
}

CWeekLinePtr CDayLineContainer::CreateNewWeekLine(long& lCurrentDayLinePos) {
  ASSERT(GetDataSize() > 0);
  long lNextMonday = GetNextMonday(GetData(lCurrentDayLinePos)->GetFormatedMarketDay());
  long lNewestDay = GetData(GetDataSize() - 1)->GetFormatedMarketDay();
  CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
  if (lNextMonday < lNewestDay) { // �м�����
    while (GetData(lCurrentDayLinePos)->GetFormatedMarketDay() < lNextMonday) {
      pWeekLine->CreateWeekLine(GetData(lCurrentDayLinePos++));
    }
  }
  else { // ���һ������
    while (lCurrentDayLinePos <= (GetDataSize() - 1)) {
      pWeekLine->CreateWeekLine(GetData(lCurrentDayLinePos++));
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