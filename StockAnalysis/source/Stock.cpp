#include "stdafx.h"

#include "globedef.h"
//#include "timer.h"

#include"Accessory.h"

#include "Stock.h"
#include"Market.h"

using namespace std;
#include"string"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStock::CStock() : CObject() {
  Reset();
}

CStock::~CStock(void) {
  if (m_pDayLineBuffer != nullptr) {
    delete m_pDayLineBuffer;
    m_pDayLineBuffer = nullptr;
  }
}

void CStock::Reset(void) {
  m_stockBasicInfo.Reset();
  m_stockCalculatedInfo.Reset();

  m_llLastSavedVolume = 0;

  m_lCurrentCanselSellVolume = m_lCurrentCanselBuyVolume = m_lCurrentGuadanTransactionVolume = 0;

  m_dCurrentGuadanTransactionPrice = 0;
  m_nCurrentTransactionType = 0;

  m_fActive = false;

  m_fDayLineLoaded = false;
  m_fDayLineNeedUpdate = true;
  m_fDayLineNeedProcess = false; // �������϶�ȡ��������ʷ����
  m_fDayLineNeedSaving = false;
  m_lDayLineBufferLength = 0;
  m_pDayLineBuffer = nullptr;

  m_fChoiced = false;
  m_fMinLineUpdated = false;
  m_fDayLineUpdated = false;

  m_fDayLineDBUpdated = false;

  m_fHaveFirstRTData = false;  // ʵʱ���ݿ�ʼ�����ʶ����һ��ʵʱ����ֻ��������ʼ��ϵͳ���������ڼ��㡣�ӵڶ������ݿ�ʼ�������Ч��
  m_fNeedProcessRTData = true;
  m_pLastRTData = nullptr;

  ClearRTDataDeque();

  m_fDebugLoadDayLineFirst = false;
}

void CStock::ClearRTDataDeque(void) {
  long lTotalNumber = GetRTDataQueueSize();
  for (int i = 0; i < lTotalNumber; i++) {
    CRTDataPtr pRTData = PopRTData();
  }
}

void CStock::SetDayLineNeedSaving(bool fFlag) {
  if (fFlag) {
    ASSERT(!m_fDayLineNeedSaving);
    m_fDayLineNeedSaving = true;
    gl_ChinaStockMarket.IncreaseNeteaseDayLineNeedSaveNumber();
  }
  else {
    ASSERT(m_fDayLineNeedSaving);
    m_fDayLineNeedSaving = false;
    gl_ChinaStockMarket.DecreaseNeteaseDayLineNeedSaveNumber();
  }
}

bool CStock::IsDayLineNeedSavingAndClearFlag(void) {
  bool fNeedSaveing = m_fDayLineNeedSaving.exchange(false);
  if (fNeedSaveing) gl_ChinaStockMarket.DecreaseNeteaseDayLineNeedSaveNumber();
  return fNeedSaveing;
}

bool CStock::TransferNeteaseDayLineWebDataToBuffer(CNeteaseDayLineWebData* pNeteaseWebDayLineData) {
  // ����ȡ���������ݷ�����ع�Ʊ���������ݻ������У���������ر�ʶ��
  char* p = pNeteaseWebDayLineData->GetBufferAddr();
  if (m_pDayLineBuffer != nullptr) delete m_pDayLineBuffer;
  m_pDayLineBuffer = new char[pNeteaseWebDayLineData->GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
  char* pbuffer = m_pDayLineBuffer;
  for (int i = 0; i < pNeteaseWebDayLineData->GetByteReaded() + 1; i++) {
    *pbuffer++ = *p++;
  }
  m_lDayLineBufferLength = pNeteaseWebDayLineData->GetByteReaded();
  SetDayLineNeedProcess(true);

  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// ������������߷������϶�ȡ�Ĺ�Ʊ�������ݡ�
// ������ʽΪ�� ����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::ProcessNeteaseDayLineData(void) {
  char* pTestPos = m_pDayLineBuffer;
  vector<CDayLinePtr> vTempDayLine;
  shared_ptr<CDayLine> pDayLine;

  ASSERT(m_fDayLineNeedProcess);
  ASSERT(m_fDayLineNeedSaving == false);
  if (m_lDayLineBufferLength == 0) { // û�����ݶ��룿����״̬�ǲ�ѯ�Ĺ�ƱΪ��Ч�������ڣ�����
    return false;
  }

  ASSERT(m_pDayLineBuffer[m_lDayLineBufferLength] == 0x000); // ����ַ�Ϊ���ӵ�0x000.
  ResetCurrentPos();
  if (!SkipNeteaseDayLineInformationHeader()) return false;

  pTestPos = m_pDayLineBuffer + m_llCurrentPos;
  ASSERT(*pTestPos == *m_pCurrentPos);
  if (m_llCurrentPos == m_lDayLineBufferLength) {// ��Ч��Ʊ���룬����ֻ��ǰ׺˵����û��ʵ����Ϣ�����������ˣ������Ѿ������ˣ��������¹����еĵ�һ��
    if (GetDayLineEndDay() == 19900101) { // �����ʼ���߽������ڴ���û�б��������˹�Ʊ������δ��ʹ�ù�
      SetIPOStatus(__STOCK_NULL__);   // �˹�Ʊ������δʹ�á�
      //TRACE("��Ч��Ʊ����:%s\n", GetStockCode().GetBuffer());
    }
    else { // �Ѿ����еĹ�Ʊ
      if (gl_systemTime.IsEarlyThen(GetDayLineEndDay(), gl_systemTime.GetDay(), 30)) {
        SetIPOStatus(__STOCK_DELISTED__);   // �˹�Ʊ�����Ѿ����С�
      }
      //TRACE("%Sû�пɸ��µ���������\n", GetStockCode().GetBuffer());
    }
    return false;
  }

  CString strTemp;
  pTestPos = m_pDayLineBuffer + m_llCurrentPos;
  ASSERT(*pTestPos == *m_pCurrentPos);
  while (m_llCurrentPos < m_lDayLineBufferLength) {
    pDayLine = make_shared<CDayLine>();
    if (!pDayLine->ProcessNeteaseData(GetStockCode(), m_pCurrentPos, m_llCurrentPos)) { // ����һ����������
      TRACE(_T("%s�������ݳ���\n"), pDayLine->GetStockCode().GetBuffer());
      // ������ݴ����������
      vTempDayLine.clear();
      return false; // ���ݳ�����������
    }
    pTestPos = m_pDayLineBuffer + m_llCurrentPos;
    ASSERT(*pTestPos == *m_pCurrentPos);
    if (!IsActive()) { // �µĹ�Ʊ���룿
      // �����¹�Ʊ
      SetTodayActive(pDayLine->GetMarket(), pDayLine->GetStockCode(), pDayLine->GetStockName());
      TRACE("�������ߺ��������µĻ�Ծ��Ʊ%s\n", GetStockCode().GetBuffer());
    }
    vTempDayLine.push_back(pDayLine); // �ݴ�����ʱvector�У���Ϊ�����������ݵ�ʱ��˳���ǵߵ��ģ����µ�����ǰ��
  }
  strTemp = pDayLine->GetStockCode();
  strTemp += _T("�����������.");
  gl_systemMessage.PushDayLineInfoMessage(strTemp);
  if (gl_systemTime.IsEarlyThen(vTempDayLine.at(0)->GetDay(), gl_systemTime.GetDay(), 30)) { // ��ȡ���Ĺ�Ʊ���������������������ϸ��µ���������գ������˻������������һ���µ�ʱ��۲죩��
    SetIPOStatus(__STOCK_DELISTED__); // �����л���ͣ���ס�
  }
  else {
    SetIPOStatus(__STOCK_IPOED__); // �������׹�Ʊ
  }

  m_vDayLine.clear(); // �����������������ݣ�����еĻ���
  // ������������ʱ��Ϊ�������
  for (int i = vTempDayLine.size() - 1; i >= 0; i--) {
    pDayLine = vTempDayLine.at(i);
    if (pDayLine->IsActive()) {
      // ��������ٽ��ף�ͣ�ƻ����У��Ĺ�Ʊ����
      m_vDayLine.push_back(pDayLine);
    }
  }
  vTempDayLine.clear();
  if (m_pDayLineBuffer != nullptr) {
    delete m_pDayLineBuffer;
    m_pDayLineBuffer = nullptr;
  }
  SetDayLineLoaded(true);
  SetDayLineNeedSaving(true); // ���ô洢���߱�ʶ

  return true;
}

bool CStock::SkipNeteaseDayLineInformationHeader() {
  ASSERT(m_pCurrentPos == m_pDayLineBuffer);
  ASSERT(m_llCurrentPos == 0);
  while (*m_pCurrentPos != 0X0d) {
    if ((*m_pCurrentPos == 0x0a) || (*m_pCurrentPos == 0x000)) {
      return false;
    }
    IncreaseCurrentPos();
  }
  IncreaseCurrentPos();
  if (*m_pCurrentPos != 0x0a) {
    return false;
  }
  IncreaseCurrentPos();
  return true;
}

void CStock::SetTodayActive(WORD wMarket, CString strStockCode, CString strStockName) {
  SetActive(true);
  SetDayLineLoaded(false);
  SetMarket(wMarket);
  SetStockCode(strStockCode); // ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
  SetStockName(strStockName);// ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
  gl_ChinaStockMarket.SetTotalActiveStock(gl_ChinaStockMarket.GetTotalActiveStock() + 1);
}

void CStock::StoreDayLine(vector<CDayLinePtr>& vTempDayLine) {
  CDayLinePtr pDayLine = nullptr;
  m_vDayLine.clear(); // �����������������ݣ�����еĻ���
  // ������������ʱ��Ϊ�������
  for (int i = vTempDayLine.size() - 1; i >= 0; i--) {
    pDayLine = vTempDayLine.at(i);
    if (pDayLine->IsActive()) {
      // ��������ٽ��ף�ͣ�ƻ����к���ֵģ��Ĺ�Ʊ����
      m_vDayLine.push_back(pDayLine);
    }
  }
  SetDayLineLoaded(true);
}

void CStock::ReportDayLineDownLoaded(void) {
  CString strTemp = GetStockCode();
  strTemp += _T("�����������.");
  gl_systemMessage.PushDayLineInfoMessage(strTemp);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	�����ߴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//  �˺����������̵߳��ã���Ҫע������ͬ�����⡣
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CStock::SaveDayLine(void) {
  CSetDayLine setDayLine;
  long lSize = 0;
  vector<CDayLinePtr> vDayLine;
  CDayLinePtr pDayLine = nullptr;
  long lCurrentPos = 0, lSizeOfOldDayLine = 0;
  bool fNeedUpdate = false;

  lSize = m_vDayLine.size();
  setDayLine.m_strFilter = _T("[StockCode] = '");
  setDayLine.m_strFilter += GetStockCode() + _T("'");
  setDayLine.m_strSort = _T("[Day]");

  setDayLine.Open();
  while (!setDayLine.IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->LoadData(setDayLine);
    vDayLine.push_back(pDayLine);
    lCurrentPos++;
    setDayLine.MoveNext();
  }
  setDayLine.Close();
  if (vDayLine.size() == 0) {
    SetDayLineStartDay(gl_systemTime.GetDay());
    SetDayLineEndDay(__CHINA_MARKET_BEGIN_DAY__);
  }
  else {
    SetDayLineStartDay(vDayLine.at(0)->GetDay());
    SetDayLineEndDay(vDayLine.at(vDayLine.size() - 1)->GetDay());
  }

  lSizeOfOldDayLine = lCurrentPos;
  lCurrentPos = 0;
  setDayLine.Open();
  setDayLine.m_pDatabase->BeginTrans();
  for (int i = 0; i < lSize; i++) { // ����������洢�ģ���Ҫ��ͷ����ʼ�洢
    pDayLine = m_vDayLine.at(i);
    while ((lCurrentPos < lSizeOfOldDayLine) && (vDayLine.at(lCurrentPos)->GetDay() < pDayLine->GetDay())) lCurrentPos++;
    if (lCurrentPos < lSizeOfOldDayLine) {
      if (vDayLine.at(lCurrentPos)->GetDay() > pDayLine->GetDay()) {
        pDayLine->AppendData(setDayLine);
        fNeedUpdate = true;
      }
    }
    else {
      pDayLine->AppendData(setDayLine);
      fNeedUpdate = true;
    }
  }
  setDayLine.m_pDatabase->CommitTrans();
  setDayLine.Close();

  // ���������������ں���ʼ��������
  if (fNeedUpdate) {
    UpdateDayLineStartEndDay();
  }

  return fNeedUpdate;
}

void CStock::UpdateDayLineStartEndDay(void) {
  if (m_vDayLine.at(0)->GetDay() < GetDayLineStartDay()) {
    SetDayLineStartDay(m_vDayLine.at(0)->GetDay());
    SetDayLineDBUpdated(true);
  }
  if (m_vDayLine.at(m_vDayLine.size() - 1)->GetDay() > GetDayLineEndDay()) {
    SetDayLineEndDay(m_vDayLine.at(m_vDayLine.size() - 1)->GetDay());
    SetDayLineDBUpdated(true);
  }
}

void CStock::SaveBasicInfo(CSetDayLine& setDayLine) {
  m_stockBasicInfo.SaveBasicInfo(setDayLine);
}

void CStock::SaveCalculatedInfo(CSetDayLineInfo& setDayLineInfo) {
  m_stockCalculatedInfo.SaveTodayInfo(setDayLineInfo);
}

void CStock::SaveTempInfo(CSetDayLineToday& setDayLineToday) {
  ASSERT(setDayLineToday.IsOpen());
  m_stockBasicInfo.SaveTempInfo(setDayLineToday);
  m_stockCalculatedInfo.SaveTempInfo(setDayLineToday);
}

////////////////////////////////////////////////////////////////////////////
//
// ֻ��һ�����ñ�������������ϵͳ��ʼ��ʱװ��֮ǰ���������Ϣ����������˵Ļ���.
// ��Ҫͬʱ�����ܳɽ����������ݴ�˹��������ڼ���δ������ɽ����� �ܳɽ��������µ�ʵʱ��������ʱ��ͬ�����£��ʶ��޷����ڼ��㣩
//
////////////////////////////////////////////////////////////////////////////
void CStock::LoadAndCalculateTempInfo(CSetDayLineToday& setDayLineToday) {
  m_stockCalculatedInfo.LoadAndCalculateTempInfo(setDayLineToday);
  SetVolume(atoll(setDayLineToday.m_Volume));
  SetLastSavedVolume(atoll(setDayLineToday.m_Volume));
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ���¹�Ʊ��ǰ��״̬��������ʾ��
//
//
///////////////////////////////////////////////////////////////////////////////////////////
void CStock::UpdateStatus(CRTDataPtr pRTData) {
  m_stockBasicInfo.UpdateStatus(pRTData);
}

bool CStock::LoadDayLineAndDayLineInfo(void) {
  CSetDayLine setDayLine;
  CSetDayLineInfo setDayLineInfo;
  CStockPtr pCurrentStock = gl_ChinaStockMarket.GetCurrentStockPtr();

  // װ��DayLine����
  setDayLine.m_strFilter = _T("[StockCode] = '");
  setDayLine.m_strFilter += pCurrentStock->GetStockCode();
  setDayLine.m_strFilter += _T("'");
  setDayLine.m_strSort = _T("[Day]");
  setDayLine.Open();
  LoadDayLine(setDayLine);
  setDayLine.Close();

  // װ��DayLineInfo����
  setDayLineInfo.m_strFilter = _T("[StockCode] = '");
  setDayLineInfo.m_strFilter += pCurrentStock->GetStockCode();
  setDayLineInfo.m_strFilter += _T("'");
  setDayLineInfo.m_strSort = _T("[Day]");
  setDayLineInfo.Open();
  LoadDayLineInfo(setDayLineInfo);
  setDayLineInfo.Close();

  return true;
}

bool CStock::LoadDayLine(CSetDayLine& setDayLine) {
  CDayLinePtr pDayLine;

  ASSERT(m_fDebugLoadDayLineFirst == false);
  // װ��DayLine����
  m_vDayLine.clear();
  while (!setDayLine.IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->LoadData(setDayLine);
    m_vDayLine.push_back(pDayLine);
    setDayLine.MoveNext();
  }
  m_fDebugLoadDayLineFirst = true;
  return true;
}

/////////////////////////////////////////////////////////////////////////////
//
// װ��DayLineInfo�������װ��DayLine��֮��
//
//
////////////////////////////////////////////////////////////////////////////
bool CStock::LoadDayLineInfo(CSetDayLineInfo& setDayLineInfo) {
  CDayLinePtr pDayLine;
  int iPosition = 0;

  ASSERT(m_fDebugLoadDayLineFirst);

  while (!setDayLineInfo.IsEOF()) {
    pDayLine = m_vDayLine.at(iPosition);
    while ((pDayLine->GetDay() < setDayLineInfo.m_Day)
           && (m_vDayLine.size() > (iPosition + 1))) {
      iPosition++;
      pDayLine = m_vDayLine.at(iPosition);
    }
    if (pDayLine->GetDay() == setDayLineInfo.m_Day) {
      pDayLine->LoadData(setDayLineInfo);
    }
    if (m_vDayLine.size() <= (iPosition + 1)) break;
    setDayLineInfo.MoveNext();
  }
  m_fDebugLoadDayLineFirst = false;
  return true;
}

bool CStock::CalculateDayLineRS(void) {
  CalculateDayLineRS(3);
  CalculateDayLineRS(5);
  CalculateDayLineRS(10);
  CalculateDayLineRS(30);
  CalculateDayLineRS(60);
  CalculateDayLineRS(120);
  return true;
}

bool CStock::CalculateDayLineRS(INT64 lNumber) {
  double dTempRS = 0;
  const INT64 lTotalNumber = m_vDayLine.size();
  for (INT64 i = lNumber; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (INT64 j = i - lNumber; j < i; j++) {
      dTempRS += m_vDayLine.at(j)->GetRelativeStrong();
    }
    switch (lNumber) {
    case 3:
    m_vDayLine.at(i)->m_d3DayRS = dTempRS / lNumber;
    break;
    case 5:
    m_vDayLine.at(i)->m_d5DayRS = dTempRS / lNumber;
    break;
    case 10:
    m_vDayLine.at(i)->m_d10DayRS = dTempRS / lNumber;
    break;
    case 30:
    m_vDayLine.at(i)->m_d30DayRS = dTempRS / lNumber;
    break;
    case 60:
    m_vDayLine.at(i)->m_d60DayRS = dTempRS / lNumber;
    break;
    case 120:
    m_vDayLine.at(i)->m_d120DayRS = dTempRS / lNumber;
    break;
    default:
    ASSERT(0);
    }
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// �˺����������߳�ThreadCalculatingRTDataProc���ã��������ٶ�ȫ�ֱ����Ĳ�����
//
// ��������͹ҵ�����ƺ��ֿ�����ȽϺã�˼·����������Ŀǰ�������������й�����Ҫ���Ǿɻ������ٶ��ˡ�
//
//
////////////////////////////////////////////////////////////////////////////////////
bool CStock::ProcessRTData(void) {
  CRTDataPtr pRTData;

  INT64 lTotalNumber = GetRTDataQueueSize(); //  ������еĳ��ȡ�����ͬ�����ƻ�ȡ����ֵ.
  // ����Ϊ����ҵ��仯����Ʊ��Ծ�ȡ����������
  for (INT64 i = 0; i < lTotalNumber; i++) {
    pRTData = PopRTData(); // ����ͬ�����ƻ�ȡ����
    if (pRTData->IsActive()) { // ������Ч
      UpdateStatus(pRTData);   // ���¹�Ʊ��ʱ״̬��
      if (IsNeedProcessRTData() && gl_ChinaStockMarket.IsMarketOpened()) {// ����ʱ���ڼ�����������ָ�����Ʊ������㽻������͹ҵ��仯
        ProcessOneRTData(pRTData);
        CheckCurrentRTData();
        ShowCurrentTransaction();
        ShowCurrentInformationofCancelingGuadan();
      }
    }
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ���ҵ���
// ����ǵ�һ�μ��㣬��ֻ���ó�ʼ״̬��
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::ProcessOneRTData(CRTDataPtr pRTData) {
  if (HaveFirstRTData()) { // �����ʼ���㣨�ڶ����յ�ʵʱ���ݼ��Ժ�
    CalculateOneRTData(pRTData);
  }
  else { // ��һ���յ�ʵʱ���ݣ���ֻ��ʼ��ϵͳ��������
    InitializeCalculatingRTDataEnvionment(pRTData);
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// ��һ���յ�ʵʱ����ʱ��ֻ��ʼ��ϵͳ�������㣨��Ϊû�г�ʼ���ݣ�
//
////////////////////////////////////////////////////////////////////////////////////////
void CStock::InitializeCalculatingRTDataEnvionment(CRTDataPtr pRTData) {
  SetLastRTDataPtr(pRTData);
  SetHavingFirstRTData(true);
  // ��һ�ιҵ����޷��ж�����״̬���ʶ�������Ϊ�޷��жϡ����֮ǰ�Ѿ����й�ϵͳ���˴��ǿ�����;��¼�ģ���ϵͳ�洢����ʱ���������ݿ��У�
  // ��ϵͳ����ʱ�Ѿ�������ʱ״̬�����ˣ��ʶ�m_lUnknownVolume��Ϊ�㣬�ʶ���Ҫ���¼���m_lUnknownVolume.
  // m_lUnknownVolume = ��ǰ�ĳɽ��� - ֮ǰ��ʱ�洢�ĳɽ��� + ֮ǰ�洢��m_lUnkonwnVolume.
  m_stockBasicInfo.SetVolume(pRTData->GetVolume());
  m_stockCalculatedInfo.SetUnknownVolume(m_stockCalculatedInfo.GetUnknownVolume() + m_pLastRTData->GetVolume() - m_llLastSavedVolume);

  // ���õ�һ�εĹҵ�ӳ�䡣
  for (int j = 0; j < 5; j++) {
    SetGuadan(pRTData->GetPBuy(j), pRTData->GetVBuy(j));
    SetGuadan(pRTData->GetPSell(j), pRTData->GetVSell(j));
  }
}

void CStock::CalculateOneRTData(CRTDataPtr pRTData) {
  long lCurrentGuadanTransactionPrice = 0;

  m_lCurrentGuadanTransactionVolume = pRTData->GetVolume() - m_pLastRTData->GetVolume();
  if (m_lCurrentGuadanTransactionVolume == 0) { // �޳ɽ���
    // ���ҵ����
    lCurrentGuadanTransactionPrice = 0;
    m_nCurrentTransactionType = __NO_TRANSACTION__;
  }
  else {
    lCurrentGuadanTransactionPrice = (pRTData->GetAmount() - m_pLastRTData->GetAmount()) * 1000 / m_lCurrentGuadanTransactionVolume; // ���ɱȽ��õļ۸񣨷Ŵ�һǧ������ó����ͣ�
    CalculateOneDeal(pRTData, lCurrentGuadanTransactionPrice);
  }

  // ���濪ʼ�����ҵ����
  AnalysisGuadan(pRTData, lCurrentGuadanTransactionPrice);

  // ����ǰ����״̬
  SetLastRTDataPtr(pRTData);
}

void CStock::CalculateOneDeal(CRTDataPtr pRTData, INT64 lCurrentGuadanTransactionPrice) {
  IncreaseTransactionNumber();
  lCurrentGuadanTransactionPrice = (pRTData->GetAmount() - m_pLastRTData->GetAmount()) * 1000 / m_lCurrentGuadanTransactionVolume; // ���ɱȽ��õļ۸񣨷Ŵ�һǧ������ó����ͣ�
  m_dCurrentGuadanTransactionPrice = static_cast<double>(lCurrentGuadanTransactionPrice) / 1000; // �任��ʵ�ʼ۸�
  if (lCurrentGuadanTransactionPrice >= m_pLastRTData->GetPBuy(0)) { // ����ǰ����1
    if (lCurrentGuadanTransactionPrice <= m_pLastRTData->GetPSell(0)) { // ����ǰ����1
      CalculateOrdinaryBuySell(lCurrentGuadanTransactionPrice);
    }
    else if (lCurrentGuadanTransactionPrice < m_pLastRTData->GetPSell(1)) { // ��������һ���������̶������������롣AttackBuy
      CalculateAttackBuy();
    }
    else { // �������̶���ǿ�����롣StrongBuy��
      CalculateStrongBuy();
    }
  } // ����ǰ��һ
  else if (lCurrentGuadanTransactionPrice > m_pLastRTData->GetPBuy(1)) { // ��������һ���������̶���������������AttackSell
    CalculateAttackSell();
  }
  else { // �������̶���ǿ��������StrongSell
    CalculateStrongSell();
  }
  ASSERT(m_stockBasicInfo.GetTransactionTime() >= pRTData->GetTransactionTime());
  const INT64 I = pRTData->GetVolume();
  const INT64 j = m_stockCalculatedInfo.GetOrdinaryBuyVolume() + m_stockCalculatedInfo.GetOrdinarySellVolume()
    + m_stockCalculatedInfo.GetAttackBuyVolume() + m_stockCalculatedInfo.GetAttackSellVolume()
    + +m_stockCalculatedInfo.GetStrongBuyVolume() + m_stockCalculatedInfo.GetStrongSellVolume() + m_stockCalculatedInfo.GetUnknownVolume();
  ASSERT(pRTData->GetVolume() == m_stockCalculatedInfo.GetOrdinaryBuyVolume() + m_stockCalculatedInfo.GetOrdinarySellVolume()
         + m_stockCalculatedInfo.GetAttackBuyVolume() + m_stockCalculatedInfo.GetAttackSellVolume()
         + m_stockCalculatedInfo.GetStrongBuyVolume() + m_stockCalculatedInfo.GetStrongSellVolume() + m_stockCalculatedInfo.GetUnknownVolume());
}

void CStock::IncreaseTransactionNumber(void) {
  m_stockCalculatedInfo.IncreaseTransactionNumber(); // �ɽ�����һ��
  if (m_lCurrentGuadanTransactionVolume < 5000) {
    m_stockCalculatedInfo.IncreaseTransactionNumberBelow5000();
  }
  else if (m_lCurrentGuadanTransactionVolume < 50000) {
    m_stockCalculatedInfo.IncreaseTransactionNumberBelow50000();
  }
  else if (m_lCurrentGuadanTransactionVolume < 200000) {
    m_stockCalculatedInfo.IncreaseTransactionNumberBelow200000();
  }
  else {
    m_stockCalculatedInfo.IncreaseTransactionNumberAbove200000();
  }
  ASSERT(m_stockCalculatedInfo.GetTransactionNumber() == (m_stockCalculatedInfo.GetTransactionNumberAbove200000()
                                                          + m_stockCalculatedInfo.GetTransactionNumberBelow200000()
                                                          + m_stockCalculatedInfo.GetTransactionNumberBelow50000() + m_stockCalculatedInfo.GetTransactionNumberBelow5000()));
}

void CStock::CalculateOrdinaryBuySell(INT64 lCurrentGuadanTransactionPrice) {
  if ((m_pLastRTData->GetPSell(0) - lCurrentGuadanTransactionPrice) <= 2) { //һ��������
    IncreaseOrdinaryBuyVolume(m_lCurrentGuadanTransactionVolume);
    m_nCurrentTransactionType = __ORDINARY_BUY__;
  }
  else if ((lCurrentGuadanTransactionPrice - m_pLastRTData->GetPBuy(0)) <= 2) { // һ��������
    m_nCurrentTransactionType = __ORDINARY_SELL__;
    IncreaseOrdinarySellVolume(m_lCurrentGuadanTransactionVolume);
  }
  else { // �������ӣ���������
    m_nCurrentTransactionType = __UNKNOWN_BUYSELL__;
    IncreaseUnknownVolume(m_lCurrentGuadanTransactionVolume);
  }
}

void CStock::CalculateAttackBuy(void) {
  m_nCurrentTransactionType = __ATTACK_BUY__;
  IncreaseAttackBuyVolume(m_lCurrentGuadanTransactionVolume);
  CalculateAttackBuyVolume();
}

void CStock::CalculateStrongBuy(void) {
  m_nCurrentTransactionType = __STRONG_BUY__;
  IncreaseStrongBuyVolume(m_lCurrentGuadanTransactionVolume);
  CalculateAttackBuyVolume();
}

void CStock::CalculateAttackBuyVolume(void) {
  if (m_lCurrentGuadanTransactionVolume < 50000) {
    IncreaseAttackBuyBelow50000(m_lCurrentGuadanTransactionVolume);
  }
  else if (m_lCurrentGuadanTransactionVolume < 200000) {
    IncreaseAttackBuyBelow200000(m_lCurrentGuadanTransactionVolume);
  }
  else {
    IncreaseAttackBuyAbove200000(m_lCurrentGuadanTransactionVolume);
  }
}

void CStock::CalculateAttackSell(void) {
  m_nCurrentTransactionType = __ATTACK_SELL__;
  IncreaseAttackSellVolume(m_lCurrentGuadanTransactionVolume);
  CalculateAttackSellVolume();
}

void CStock::CalculateStrongSell(void) {
  m_nCurrentTransactionType = __STRONG_SELL__;
  IncreaseStrongSellVolume(m_lCurrentGuadanTransactionVolume);
  CalculateAttackSellVolume();
}

void CStock::CalculateAttackSellVolume(void) {
  if (m_lCurrentGuadanTransactionVolume < 50000) {
    IncreaseAttackSellBelow50000(m_lCurrentGuadanTransactionVolume);
  }
  else if (m_lCurrentGuadanTransactionVolume < 200000) {
    IncreaseAttackSellBelow200000(m_lCurrentGuadanTransactionVolume);
  }
  else {
    IncreaseAttackSellAbove200000(m_lCurrentGuadanTransactionVolume);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ҵ����ı仯���ɹ����̵߳���
//
// ����map�ṹ�洢����������������⣬Ψһ�������Ѱַ����ʱ�䳤����Ҫ���Ժ󿴿��ٶ����(���Ժ��ٶ�û�κ����⣩��
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::AnalysisGuadan(CRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice) {
  // ��Ҫ���Ĺҵ�λ�á�˳��Ϊ������4, ����3, ... ����0, ����0, .... ��3, ��4
  // ������˭��ǰ������ν��������ʱ��Ҫ��ס��˳��
  array<bool, 10> fNeedCheck{ true,true,true,true,true,true,true,true,true,true };

  for (int i = 0; i < 10; i++) fNeedCheck.at(i) = true; // Ԥ��Ϊ��Ҫ���㡣
  SelectGuadanThatNeedToCalculate(pCurrentRTData, lCurrentTransactionPrice, fNeedCheck);
  SetCurrentGuadan(pCurrentRTData);
  // �����ʮ���ҵ���λ�Ϲ����ı仯���.��Ŀǰ�ҵ�״̬��֮ǰ��ʮ����λ��m_pLastRTData�����ģ���ȣ��鿴�仯
  CheckGuadan(pCurrentRTData, fNeedCheck);

  return(true);
}

void CStock::SelectGuadanThatNeedToCalculate(CRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice, array<bool, 10>& fNeedCheck) {
  // ȷ����Ҫ������Щ�ҵ���һ����ʮ����û���ܵ�����Ӱ��Ķ�Ҫ���㡣
  switch (m_nCurrentTransactionType) {
  case __NO_TRANSACTION__: // û�гɽ�������ٵ���������Ӧ��λ�ϵĳ�����
  ASSERT(lCurrentTransactionPrice == 0);
  break;
  case __ATTACK_BUY__: // ����һ�Ѿ���ʧ����������Ӱ�졣���������߸��ҵ���
  fNeedCheck.at(3) = false;
  fNeedCheck.at(4) = fNeedCheck.at(5) = false;
  break;
  case __STRONG_BUY__: // ����һ����������ʧ������������δ������Ҫ���㡣
  if (lCurrentTransactionPrice < m_pLastRTData->GetPSell(2)) { // ����4������5�д�
    fNeedCheck.at(2) = false;
  }
  else if (lCurrentTransactionPrice < m_pLastRTData->GetPSell(3)) { // ����5�д�
    fNeedCheck.at(1) = false;
    fNeedCheck.at(2) = false;
  }
  else { // ���е���������Ӱ��
    fNeedCheck.at(0) = false;
    fNeedCheck.at(2) = false;
    fNeedCheck.at(1) = false;
  }
  fNeedCheck.at(3) = false;
  fNeedCheck.at(4) = false;
  fNeedCheck.at(5) = false;
  break;
  case __ORDINARY_BUY__: // ����һ��Ӱ�졣���������˸��ҵ���
  case __UNKNOWN_BUYSELL__: // ����һ����һ��Ӱ�졣���������˸��ҵ���
  case __ORDINARY_SELL__: // ��һ��Ӱ�졣���������˸��ҵ���
  fNeedCheck.at(4) = fNeedCheck.at(5) = false;
  break;
  case __ATTACK_SELL__: // ��һ��ʧ���򵥶���Ӱ�졣���������߸��ҵ���
  fNeedCheck.at(4) = fNeedCheck.at(5) = fNeedCheck.at(6) = false;
  break;
  case __STRONG_SELL__: // ��һ���򵥶���ʧ����������Ҫ���㡣
  if (m_pLastRTData->GetPBuy(3) > lCurrentTransactionPrice) { // �����򵥶���Ӱ��
    fNeedCheck.at(7) = false;
    fNeedCheck.at(8) = false;
    fNeedCheck.at(9) = false;
  }
  else if (m_pLastRTData->GetPBuy(2) > lCurrentTransactionPrice) { // ��5�д�
    fNeedCheck.at(7) = false;
    fNeedCheck.at(8) = false;
  }
  else { // ��4����5û�仯
    fNeedCheck.at(7) = false;
  }
  fNeedCheck.at(6) = false;
  fNeedCheck.at(5) = false;
  fNeedCheck.at(4) = false;
  break;
  default:
  ASSERT(0); // ������ִ�е��˷�֧��
  break;
  }
}

void CStock::SetCurrentGuadan(CRTDataPtr pCurrentRTData) {
  // ��λ�������ǳɽ��ˣ�Ҳ�����ǳ����ˣ�Ŀǰ��������Щϸ�ڣ�ͳһ��Ϊ�ǳɽ��ˣ������㳷�������Ժ��ٷ���֮��
  // ����յ�ǰ�ҵ�֮��Ĺҵ�������Ȼ�����ϵ�ǰ��������пյ��Ļ�������Ȼ����ˡ�
  for (int i = pCurrentRTData->GetPBuy(4); i <= pCurrentRTData->GetPSell(4); i += 10) {
    m_mapGuadan[i] = 0; // �˴���Ҫ����ֵ���п���֮ǰΪ�գ��ʶ�����ʹ��at��������������[]�±������
  }
  for (int i = 0; i < 5; i++) { // ��Ŀǰ��ʮ���ҵ�״̬����ӳ���У��ҵ�״̬����Ϊ����̬
    SetGuadan(pCurrentRTData->GetPSell(i), pCurrentRTData->GetVSell(i));
    SetGuadan(pCurrentRTData->GetPBuy(i), pCurrentRTData->GetVBuy(i));
  }
}

void CStock::CheckGuadan(CRTDataPtr pCurrentRTData, array<bool, 10>& fNeedCheck) {
  for (int i = 0; i < 5; i++) {
    CheckSellGuadan(fNeedCheck, i);
    CheckBuyGuadan(fNeedCheck, i);
  }
}

void CStock::CheckSellGuadan(array<bool, 10>& fNeedCheck, int i) {
  ASSERT((i < 5) && (i >= 0));
  if (fNeedCheck.at(4 - i)) {
    if (GetGuadan(m_pLastRTData->GetPSell(i)) < m_pLastRTData->GetVSell(i)) { // �����˵Ļ�
      m_lCurrentCanselSellVolume += m_pLastRTData->GetVSell(i) - GetGuadan(m_pLastRTData->GetPSell(i));
      m_stockCalculatedInfo.IncreaseCancelSellVolume(m_pLastRTData->GetVSell(i) - GetGuadan(m_pLastRTData->GetPSell(i)));
    }
  }
}

void CStock::CheckBuyGuadan(array<bool, 10>& fNeedCheck, int i) {
  ASSERT((i < 5) && (i >= 0));
  if (fNeedCheck.at(5 + i)) {
    if (GetGuadan(m_pLastRTData->GetPBuy(i)) < m_pLastRTData->GetVBuy(i)) { // �����˵Ļ�
      m_lCurrentCanselBuyVolume += m_pLastRTData->GetVBuy(i) - GetGuadan(m_pLastRTData->GetPBuy(i));
      m_stockCalculatedInfo.IncreaseCancelBuyVolume(m_pLastRTData->GetVBuy(i) - GetGuadan(m_pLastRTData->GetPBuy(i)));
    }
  }
}

bool CStock::HaveGuadan(INT64 lPrice) {
  if (m_mapGuadan.find(lPrice) == m_mapGuadan.end()) return false;
  else if (m_mapGuadan.at(lPrice) == 0) return false;
  return true;
}

bool CStock::CheckCurrentRTData() {
  if ((m_stockCalculatedInfo.GetOrdinaryBuyVolume() < 0) || (m_stockCalculatedInfo.GetOrdinarySellVolume() < 0)
      || (m_stockCalculatedInfo.GetAttackBuyVolume() < 0) || (m_stockCalculatedInfo.GetAttackSellVolume() < 0)
      || (m_stockCalculatedInfo.GetStrongBuyVolume() < 0) || (m_stockCalculatedInfo.GetStrongSellVolume() < 0)) {
    int j = 0;
    if (m_stockCalculatedInfo.GetOrdinaryBuyVolume() < 0) j = 1;
    if (m_stockCalculatedInfo.GetOrdinarySellVolume() < 0) j += 2;
    if (m_stockCalculatedInfo.GetAttackBuyVolume() < 0) j += 4;
    if (m_stockCalculatedInfo.GetAttackSellVolume() < 0) j += 8;
    if (m_stockCalculatedInfo.GetStrongBuyVolume() < 0) j += 16;
    if (m_stockCalculatedInfo.GetStrongSellVolume() < 0) j += 32;
    TRACE(_T("%06d %s Error in volume. Error  code = %d\n"), gl_systemTime.GetTime(), m_stockBasicInfo.GetStockCode().GetBuffer(), j);
    return false;
  }
  return true;
}

void CStock::ShowCurrentTransaction() {
  // ��ʾ��ǰ�������
  CStockPtr pCurrentStock = gl_ChinaStockMarket.GetCurrentStockPtr();

  if (pCurrentStock != nullptr) {
    if (pCurrentStock->GetStockCode().Compare(m_stockBasicInfo.GetStockCode()) == 0) {
      if (pCurrentStock->GetCurrentTransationVolume() > 0) {
        pCurrentStock->ReportGuadanTransaction();
      }
    }
  }
}

void CStock::ShowCurrentInformationofCancelingGuadan(void) {
  // ��ʾ��ǰȡ���ҵ������
  CStockPtr pCurrentStock = gl_ChinaStockMarket.GetCurrentStockPtr();

  if (pCurrentStock != nullptr) {
    if (pCurrentStock->GetStockCode().Compare(m_stockBasicInfo.GetStockCode()) == 0) {
      pCurrentStock->ReportGuadan();
    }
  }
}

void CStock::ReportGuadanTransaction(void) {
  char buffer[100];
  CString str, str2, strTime;
  const CTime ctime(m_pLastRTData->GetTransactionTime());
  sprintf_s(buffer, "%02d:%02d:%02d", ctime.GetHour(), ctime.GetMinute(), ctime.GetSecond());
  strTime = buffer;
  sprintf_s(buffer, _T(" %s %I64d�ɳɽ���%10.3f    "), m_stockBasicInfo.GetStockCode().GetBuffer(),
            m_lCurrentGuadanTransactionVolume, m_dCurrentGuadanTransactionPrice);
  str = strTime;
  str += buffer;
  CString str1;
  switch (m_nCurrentTransactionType) {
  case __STRONG_BUY__:
  str1 = _T(" STRONG BUY");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetStrongBuyVolume());
  break;
  case __STRONG_SELL__:
  str1 = _T(" STRONG SELL");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetStrongSellVolume());
  break;
  case __ATTACK_BUY__:
  str1 = _T(" ATTACK BUY");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetAttackBuyVolume());
  break;
  case __ATTACK_SELL__:
  str1 = _T(" ATTACK SELL");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetAttackSellVolume());
  break;
  case __ORDINARY_BUY__:
  str1 = _T(" ORDINARY BUY");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetOrdinaryBuyVolume());
  break;
  case __ORDINARY_SELL__:
  str1 = _T(" ORDINARY SELL");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetOrdinarySellVolume());
  break;
  case __UNKNOWN_BUYSELL__:
  str1 = _T(" UNKNOWN BUYSELL");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetUnknownVolume());
  break;
  default:
  break;
  }
  str += str1;
  // ��ʾ��ǰ�ɽ����
  gl_systemMessage.PushTransactionMessage(str); // ����ͬ�����ƴ�����Ϣ

  str2 = strTime;
  str2 += str1;
  str2 += buffer;
  gl_systemMessage.PushTrace2Message(str2);
}

void CStock::ReportGuadan(void) {
  CString str1;
  char buffer[30];
  if (m_lCurrentCanselSellVolume > 0) {
    sprintf_s(buffer, _T("��ǰȡ����������%I64d"), m_lCurrentCanselSellVolume);
    str1 = buffer;
    sprintf_s(buffer, _T("  ��ȡ����������%I64d"), m_stockCalculatedInfo.GetCancelSellVolume());
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1);   // ����ͬ�����ƴ�����Ϣ
  }
  if (m_lCurrentCanselBuyVolume > 0) {
    sprintf_s(buffer, _T("��ǰȡ��������%I64d"), m_lCurrentCanselBuyVolume);
    str1 = buffer;
    sprintf_s(buffer, _T("  ��ȡ��������%I64d"), m_stockCalculatedInfo.GetCancelBuyVolume());
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1); // ����ͬ�����ƴ�����Ϣ
  }
}

void CStock::SaveStockCodeDB(CSetStockCode& setStockCode) {
  CString str;
  setStockCode.m_Counter = GetOffset();
  setStockCode.m_StockType = GetMarket();
  setStockCode.m_StockCode = GetStockCode();
  if (GetStockName() != _T("")) {   // ����˹�ƱID�����µ����֣�
    setStockCode.m_StockName = GetStockName(); // ��洢�µ�����
  }
  if (GetIPOStatus() == __STOCK_IPOED__) { // ����˹�Ʊ�ǻ�Ծ��Ʊ
    if (gl_systemTime.IsEarlyThen(GetDayLineEndDay(), gl_systemTime.GetDay(), 30)) { // ����˹�Ʊ��������ʷ�����Ѿ�����һ�����ˣ������ô˹�Ʊ״̬Ϊ������
      setStockCode.m_IPOed = __STOCK_DELISTED__;
    }
    else {
      setStockCode.m_IPOed = GetIPOStatus();
    }
  }
  else {
    setStockCode.m_IPOed = GetIPOStatus();
  }
  setStockCode.m_DayLineStartDay = GetDayLineStartDay();
  setStockCode.m_DayLineEndDay = GetDayLineEndDay();
}

void CStock::AppendStockCodeDB(CSetStockCode& setStockCode) {
  setStockCode.AddNew();
  SaveStockCodeDB(setStockCode);
  setStockCode.Update();
}

bool CStock::LoadStockCodeDB(CSetStockCode& setStockCode) {
  SetStockCode(setStockCode.m_StockCode);
  CString str = setStockCode.m_StockName; // ��str�м����һ�£��Ϳ��Զ�ȡUniCode��ʽ��m_StockName�ˡ�
  SetStockName(str);
  SetIPOStatus(setStockCode.m_IPOed);
  SetDayLineStartDay(setStockCode.m_DayLineStartDay);
  if (GetDayLineEndDay() < setStockCode.m_DayLineEndDay) { // ��ʱһ����Ʊ���ж����¼������������Ϊ׼��
    SetDayLineEndDay(setStockCode.m_DayLineEndDay);
  }
  SetCheckingDayLineStatus();
  return true;
}

void CStock::SetCheckingDayLineStatus(void) {
  ASSERT(IsDayLineNeedUpdate());
  // ���ٸ����������ݱ��ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
  if (gl_systemTime.GetLastTradeDay() <= GetDayLineEndDay()) { // ������������Ϊ���ջ�����һ�������յ����ݡ�
    SetDayLineNeedUpdate(false); // �������ݲ���Ҫ����
  }
  else if (GetIPOStatus() == __STOCK_NULL__) { // ��Ч���벻�������������
    SetDayLineNeedUpdate(false);
  }
  else if (GetIPOStatus() == __STOCK_DELISTED__) { // ���й�Ʊ��������ع��������ݣ���ÿ����һ������������
    if ((gl_systemTime.GetDayOfWeek() != 1) && (GetDayLineEndDay() != __CHINA_MARKET_BEGIN_DAY__)) {
      SetDayLineNeedUpdate(false);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ�����ƴ洢ʵʱ���ݡ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CStock::PushRTData(CRTDataPtr pData) {
  m_queueRTData.push(pData);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ��������ȡʵʱ���ݡ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
CRTDataPtr CStock::PopRTData(void) {
  CRTDataPtr pData = m_queueRTData.front();
  m_queueRTData.pop();
  return pData;
}

CRTDataPtr CStock::GetRTDataAtHead(void) {
  return m_queueRTData.front();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ��������ȡջ�ڵ�ʵʱ����������
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
INT64 CStock::GetRTDataQueueSize(void) {
  return m_queueRTData.size();
}

////////////////////////////////////////////////////////////////////////////////
//
// �жϹ�Ʊ�����Ƿ���Ч��������߼ۡ���ͼۡ��ɽ����ͳɽ������жϣ������Ϊ�㣬����Ϊ�˹ɽ���û����Ч����
// �����ÿ��̼ۣ����̼��п��ܲ�Ϊ��ʱ���������ݽ�Ϊ�㣨ͣ��ʱ����״̬����
//
////////////////////////////////////////////////////////////////////////////////
bool CStock::IsTodayDataActive(void) {
  if (!m_fActive) return false;
  else {
    return IsTodayDataChanged();
  }
}

bool CStock::IsTodayDataChanged(void) {
  if ((GetHigh() != 0) || (GetLow() != 0) || (GetAmount() != 0) || (GetVolume() != 0)) {
    return true;
  }
  else {
    return false;
  }
}

void CStock::SetDayLineNeedUpdate(bool fFlag) {
  if (fFlag) {
    ASSERT(!m_fDayLineNeedUpdate);
    m_fDayLineNeedUpdate = true;
    gl_ChinaStockMarket.IncreaseNeteaseDayLineNeedUpdateNumber();
  }
  else {
    ASSERT(m_fDayLineNeedUpdate);
    m_fDayLineNeedUpdate = false;
    gl_ChinaStockMarket.DecreaseNeteaseDayLineNeedUpdateNumber();
  }
}

void CStock::SetDayLineNeedProcess(bool fFlag) {
  if (fFlag) {
    ASSERT(!m_fDayLineNeedProcess);
    m_fDayLineNeedProcess = true;
    gl_ChinaStockMarket.IncreaseNeteaseDayLineNeedProcessNumber();
  }
  else {
    ASSERT(m_fDayLineNeedProcess);
    m_fDayLineNeedProcess = false;
    gl_ChinaStockMarket.DecreaseNeteaseDayLineNeedProcessNumber();
  }
}

void CStock::ShowDayLine(CDC* pDC, CRect rectClient) {
  const COLORREF crBlue(RGB(0, 0, 255)), crWhite(RGB(255, 255, 255));
  CPen penWhite1(PS_SOLID, 1, crWhite);
  long lHigh = 0;
  long lDay;
  vector<CDayLinePtr>::iterator it = m_vDayLine.end();
  it--;
  int i = 0, y = 0;
  long lLow = (*it)->GetLow();
  for (; it != m_vDayLine.begin(); it--) {
    if (lHigh < (*it)->GetHigh()) lHigh = (*it)->GetHigh();
    if ((*it)->GetLow() > 0) {
      if (lLow > (*it)->GetLow()) lLow = (*it)->GetLow();
    }
    if (3 * i > m_vDayLine.size()) break;
    if (rectClient.right <= 3 * i) break; // ����
    else i++;
  }

  it = m_vDayLine.end();
  it--;
  i = 0;
  long x = 0;
  pDC->SelectObject(&penWhite1);
  for (; it != m_vDayLine.begin(); it--) {
    x = rectClient.right - 2 - i * 3;
    y = (0.5 - (double)((*it)->GetHigh() - lLow) / (2 * (lHigh - lLow))) * rectClient.Height();
    pDC->MoveTo(x, y);
    y = (0.5 - (double)((*it)->GetLow() - lLow) / (2 * (lHigh - lLow))) * rectClient.Height();
    pDC->LineTo(x, y);
    lDay = (*it)->GetDay();
    i++;
    if (3 * i > m_vDayLine.size()) break;
    if (rectClient.right <= 3 * i) break; // ����������߿�Ϊֹ
  }
}

bool CStock::RSLineTo(CDC* pDC, CRect rectClient, int i, double dValue) {
  int y = rectClient.bottom - dValue * rectClient.bottom / 200;
  pDC->LineTo(rectClient.right - 1 - 3 * i, y);
  if (3 * i > m_vDayLine.size()) return false;
  if (rectClient.right <= 3 * i) return false; // ����������߿�Ϊֹ
  return true;
}

void CStock::ShowDayLineRS(CDC* pDC, CRect rectClient) {
  vector<CDayLinePtr>::iterator it = m_vDayLine.end();
  int i = 1;
  it--;
  int y = rectClient.bottom - (*it--)->GetRelativeStrong() * rectClient.bottom / 200;
  pDC->MoveTo(rectClient.right - 1, y);
  for (; it != m_vDayLine.begin(); it--, i++) {
    if (!RSLineTo(pDC, rectClient, i, (*it)->GetRelativeStrong())) break;
  }
}

void CStock::ShowDayLine3RS(CDC* pDC, CRect rectClient) {
  vector<CDayLinePtr>::iterator it = m_vDayLine.end();
  int i = 1;
  it--;
  int y = rectClient.bottom - (*it--)->Get3DayRS() * rectClient.bottom / 200;
  pDC->MoveTo(rectClient.right - 1, y);
  for (; it != m_vDayLine.begin(); it--, i++) {
    if (!RSLineTo(pDC, rectClient, i, (*it)->Get3DayRS())) break;
  }
}

void CStock::ShowDayLine5RS(CDC* pDC, CRect rectClient) {
  vector<CDayLinePtr>::iterator it = m_vDayLine.end();
  int i = 1;
  it--;
  int y = rectClient.bottom - (*it--)->Get5DayRS() * rectClient.bottom / 200;
  pDC->MoveTo(rectClient.right - 1, y);
  for (; it != m_vDayLine.begin(); it--, i++) {
    if (!RSLineTo(pDC, rectClient, i, (*it)->Get5DayRS())) break;
  }
}

void CStock::ShowDayLine10RS(CDC* pDC, CRect rectClient) {
  vector<CDayLinePtr>::iterator it = m_vDayLine.end();
  int i = 1;
  it--;
  int y = rectClient.bottom - (*it--)->Get10DayRS() * rectClient.bottom / 200;
  pDC->MoveTo(rectClient.right - 1, y);
  for (; it != m_vDayLine.begin(); it--, i++) {
    if (!RSLineTo(pDC, rectClient, i, (*it)->Get10DayRS())) break;
  }
}

void CStock::ShowDayLine30RS(CDC* pDC, CRect rectClient) {
  vector<CDayLinePtr>::iterator it = m_vDayLine.end();
  int i = 1;
  it--;
  int y = rectClient.bottom - (*it--)->Get30DayRS() * rectClient.bottom / 200;
  pDC->MoveTo(rectClient.right - 1, y);
  for (; it != m_vDayLine.begin(); it--, i++) {
    if (!RSLineTo(pDC, rectClient, i, (*it)->Get30DayRS())) break;
  }
}

void CStock::ShowDayLine60RS(CDC* pDC, CRect rectClient) {
  vector<CDayLinePtr>::iterator it = m_vDayLine.end();
  int i = 1;
  it--;
  int y = rectClient.bottom - (*it--)->Get60DayRS() * rectClient.bottom / 200;
  pDC->MoveTo(rectClient.right - 1, y);
  for (; it != m_vDayLine.begin(); it--, i++) {
    if (!RSLineTo(pDC, rectClient, i, (*it)->Get60DayRS())) break;
  }
}

void CStock::ShowDayLine120RS(CDC* pDC, CRect rectClient) {
  vector<CDayLinePtr>::iterator it = m_vDayLine.end();
  int i = 1;
  it--;
  int y = rectClient.bottom - (*it--)->Get120DayRS() * rectClient.bottom / 200;
  pDC->MoveTo(rectClient.right - 1, y);
  for (; it != m_vDayLine.begin(); it--, i++) {
    if (!RSLineTo(pDC, rectClient, i, (*it)->Get120DayRS())) break;
  }
}

void CStock::__TestSetDayLineBuffer(INT64 lBufferLength, char* pDayLineBuffer) {
  char* p;
  if (m_pDayLineBuffer != nullptr) delete m_pDayLineBuffer;
  m_pDayLineBuffer = new char[lBufferLength + 1];
  p = m_pDayLineBuffer;
  for (int i = 0; i < lBufferLength; i++) {
    *p++ = pDayLineBuffer[i];
  }
  m_pDayLineBuffer[lBufferLength] = 0x000;
  m_lDayLineBufferLength = lBufferLength;
}

bool CStock::IsVolumeConsistence(void) noexcept {
  if (GetVolume() != GetOrdinaryBuyVolume() + GetOrdinarySellVolume() + GetAttackBuyVolume()
      + GetAttackSellVolume() + GetStrongBuyVolume() + GetStrongSellVolume() + GetUnknownVolume()) {
    TRACE(_T("%s����%d\n"), GetStockCode().GetBuffer(), GetVolume());
    TRACE(_T("%d %d %d %d %d %d %d\n"), GetOrdinaryBuyVolume(), GetOrdinarySellVolume(), GetAttackBuyVolume(),
          GetAttackSellVolume(), GetStrongBuyVolume(), GetStrongSellVolume(), GetUnknownVolume());
    return false;
  }
  else return true;
}

#ifdef _DEBUG
void CStock::AssertValid() const {
  CObject::AssertValid();
}

void CStock::Dump(CDumpContext& dc) const {
  CObject::Dump(dc);
}

#endif //_DEBUG