#include"pch.h"
#include"globedef.h"

#include "FinnhubForexSymbol.h"

CFinnhubForexSymbol::CFinnhubForexSymbol() : CVirtualStock() {
  Reset();
}

void CFinnhubForexSymbol::Reset(void) {
  CVirtualStock::Reset();
  m_strDescription = _T(" ");
  m_strDisplaySymbol = _T(" ");
  m_strExchange = _T(" ");
  m_strSymbol = _T(" ");
  m_lDayLineStartDate = 29900101;
  m_lDayLineEndDate = 19800101;
}

void CFinnhubForexSymbol::Load(CSetFinnhubForexSymbol& setForexSymbol) {
  m_strDescription = setForexSymbol.m_Description;
  m_strDisplaySymbol = setForexSymbol.m_DisplaySymbol;
  m_strExchange = setForexSymbol.m_Exchange;
  m_strSymbol = setForexSymbol.m_Symbol;
  m_lDayLineStartDate = setForexSymbol.m_DayLineStartDate;
  m_lDayLineEndDate = setForexSymbol.m_DayLineEndDate;
  m_lIPOStatus = setForexSymbol.m_IPOStatus;
}

void CFinnhubForexSymbol::Append(CSetFinnhubForexSymbol& setForexSymbol) {
  setForexSymbol.AddNew();
  Save(setForexSymbol);
  setForexSymbol.Update();
}

void CFinnhubForexSymbol::Update(CSetFinnhubForexSymbol& setForexSymbol) {
  setForexSymbol.Edit();
  Save(setForexSymbol);
  setForexSymbol.Update();
}

void CFinnhubForexSymbol::Save(CSetFinnhubForexSymbol& setForexSymbol) {
  setForexSymbol.m_Description = m_strDescription;
  setForexSymbol.m_DisplaySymbol = m_strDisplaySymbol;
  setForexSymbol.m_Exchange = m_strExchange;
  setForexSymbol.m_Symbol = m_strSymbol;
  setForexSymbol.m_DayLineStartDate = m_lDayLineStartDate;
  setForexSymbol.m_DayLineEndDate = m_lDayLineEndDate;
  setForexSymbol.m_IPOStatus = m_lIPOStatus;
}

bool CFinnhubForexSymbol::SaveDayLine() {
  CSetForexDayLine setForexDayLine;
  size_t lSize = 0;
  vector<CDayLinePtr> vDayLine;
  CDayLinePtr pDayLine = nullptr;
  long lCurrentPos = 0, lSizeOfOldDayLine = 0;
  bool fNeedUpdate = false;

  ASSERT(m_vDayLine.size() > 0);

  lSize = m_vDayLine.size();
  setForexDayLine.m_strFilter = _T("[Symbol] = '");
  setForexDayLine.m_strFilter += m_strSymbol + _T("'");
  setForexDayLine.m_strSort = _T("[Date]");

  setForexDayLine.Open();
  while (!setForexDayLine.IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->LoadForexDayLine(&setForexDayLine);
    vDayLine.push_back(pDayLine);
    lSizeOfOldDayLine++;
    setForexDayLine.MoveNext();
  }
  setForexDayLine.Close();
  if (lSizeOfOldDayLine > 0) {
    if (vDayLine.at(0)->GetFormatedMarketDate() < m_lDayLineStartDate) {
      m_lDayLineStartDate = vDayLine.at(0)->GetFormatedMarketDate();
    }
  }

  lCurrentPos = 0;
  setForexDayLine.m_strFilter = _T("[ID] = 1");
  setForexDayLine.Open();
  setForexDayLine.m_pDatabase->BeginTrans();
  for (int i = 0; i < lSize; i++) { // ����������洢�ģ���Ҫ��ͷ����ʼ�洢
    pDayLine = m_vDayLine.at(i);
    while ((lCurrentPos < lSizeOfOldDayLine) && (vDayLine.at(lCurrentPos)->GetFormatedMarketDate() < pDayLine->GetFormatedMarketDate())) lCurrentPos++;
    if (lCurrentPos < lSizeOfOldDayLine) {
      if (vDayLine.at(lCurrentPos)->GetFormatedMarketDate() > pDayLine->GetFormatedMarketDate()) {
        pDayLine->AppendForexDayLine(&setForexDayLine);
        fNeedUpdate = true;
      }
    }
    else {
      pDayLine->AppendForexDayLine(&setForexDayLine);
      fNeedUpdate = true;
    }
  }
  setForexDayLine.m_pDatabase->CommitTrans();
  setForexDayLine.Close();

  return fNeedUpdate;
}

void CFinnhubForexSymbol::SetCheckingDayLineStatus(void) {
  ASSERT(IsDayLineNeedUpdate()); // Ĭ��״̬Ϊ����������Ҫ����
  if (m_lIPOStatus == __STAKE_NULL__) {
    SetDayLineNeedUpdate(false);
  }
  // ���ٸ����������ݱ��ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
  else if (gl_pWorldMarket->GetLastTradeDate() <= GetDayLineEndDate()) { // ������������Ϊ���ջ�����һ�������յ����ݡ�
    SetDayLineNeedUpdate(false); // �������ݲ���Ҫ����
  }
}

CString CFinnhubForexSymbol::GetFinnhubDayLineInquiryString(time_t tCurrentTime) {
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  char buffer[50];
  time_t tStartTime = 0;

  strMiddle += m_strSymbol;
  strMiddle += _T("&resolution=D");
  strMiddle += _T("&from=");
  tStartTime = FormatToTTime(m_lDayLineEndDate);
  if (tStartTime < (tCurrentTime - (time_t)(365) * 24 * 3600)) {// ����˻�ֻ�ܶ�ȡһ�����ڵ��������ݡ�
    tStartTime = (tCurrentTime - (time_t)(365) * 24 * 3600);
  }
  sprintf_s(buffer, _T("%I64i"), (INT64)tStartTime);
  strTemp = buffer;
  strMiddle += strTemp;
  strMiddle += _T("&to=");
  sprintf_s(buffer, _T("%I64i"), tCurrentTime);
  strTemp = buffer;
  strMiddle += strTemp;

  return strMiddle;
}

void CFinnhubForexSymbol::UpdateDayLine(vector<CDayLinePtr>& vDayLine) {
  m_vDayLine.resize(0);
  for (auto& pDayLine : vDayLine) {
    m_vDayLine.push_back(pDayLine);
  }
}

void CFinnhubForexSymbol::UpdateDayLineStartEndDate(void) {
  if (m_vDayLine.size() == 0) {
    SetDayLineStartDate(29900101);
    SetDayLineEndDate(19800101);
  }
  else {
    if (m_vDayLine.at(0)->GetFormatedMarketDate() < GetDayLineStartDate()) {
      SetDayLineStartDate(m_vDayLine.at(0)->GetFormatedMarketDate());
      m_fUpdateStockProfileDB = true;
    }
    if (m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDate() > m_lDayLineEndDate) {
      SetDayLineEndDate(m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDate());
      m_fUpdateStockProfileDB = true;
    }
  }
}

bool CFinnhubForexSymbol::HaveNewDayLineData(void) {
  if (m_vDayLine.size() <= 0) return false;
  if (m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDate() > m_lDayLineEndDate) return true;
  else return false;
}