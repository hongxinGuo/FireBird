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
}

void CStock::Reset(void) {
  m_stockBasicInfo.Reset();
  m_stockCalculatedInfo.Reset();

  m_lCurrentCanselSellVolume = m_lCurrentCanselBuyVolume = m_lCurrentGuadanTransactionVolume = 0;

  m_dCurrentGuaDanTransactionPrice = 0;
  m_nCurrentTransactionType = 0;

  m_fDayLineLoaded = false;
  m_fActive = false;
  m_fDayLineNeedUpdate = true;
  m_fInquiringOnce = false;

  m_fChoiced = false;
  m_fMinLineUpdated = false;
  m_fDayLineUpdated = false;

  m_fHaveFirstRTData = false;  // ʵʱ���ݿ�ʼ�����ʶ����һ��ʵʱ����ֻ��������ʼ��ϵͳ���������ڼ��㡣�ӵڶ������ݿ�ʼ�������Ч��
  m_pLastRTData = nullptr;

  ClearRTDataDeque();

  m_fDebugLoadDayLineFirst = false;
}

void CStock::ClearRTDataDeque(void) {
  long lTotalNumber = GetRTDataDequeSize();
  for (int i = 0; i < lTotalNumber; i++) {
    CRTDataPtr pRTData = PopRTData();
  }
}

void CStock::StoreCalculatedInfo(CSetDayLineInfo& setDayLineInfo) {
  m_stockCalculatedInfo.StoreCalculatedInfo(setDayLineInfo);
}

void CStock::StoreTempInfo(CSetDayLineToday& setDayLineToday) {
  ASSERT(setDayLineToday.IsOpen());
  m_stockBasicInfo.StoreTempInfo(setDayLineToday);
  m_stockCalculatedInfo.StoreTempInfo(setDayLineToday);
}

////////////////////////////////////////////////////////////////////////////
//
// ֻ��һ�����ñ�������������ϵͳ��ʼ��ʱװ��֮ǰ���������Ϣ����������˵Ļ������ʶ�δ֪�ɽ�����������Ҫ���⴦��
//
////////////////////////////////////////////////////////////////////////////
void CStock::LoadAndCalculateTempInfo(CSetDayLineToday& setDayLineToday) {
  m_stockCalculatedInfo.LoadAndCalculateTempInfo(setDayLineToday);
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

  // װ��DayLine����
  setDayLine.m_strFilter = _T("[StockCode] = '");
  setDayLine.m_strFilter += gl_ChinaStockMarket.m_pCurrentStock->GetStockCode();
  setDayLine.m_strFilter += _T("'");
  setDayLine.m_strSort = _T("[Day]");
  setDayLine.Open();
  LoadDayLine(&setDayLine);
  setDayLine.Close();

  // װ��DayLineInfo����
  setDayLineInfo.m_strFilter = _T("[StockCode] = '");
  setDayLineInfo.m_strFilter += gl_ChinaStockMarket.m_pCurrentStock->GetStockCode();
  setDayLineInfo.m_strFilter += _T("'");
  setDayLineInfo.m_strSort = _T("[Day]");
  setDayLineInfo.Open();
  LoadDayLineInfo(&setDayLineInfo);
  setDayLineInfo.Close();

  return true;
}

bool CStock::LoadDayLine(CSetDayLine* psetDayLine) {
  CDayLinePtr pDayLine;

  ASSERT(m_fDebugLoadDayLineFirst == false);
  // װ��DayLine����
  m_vDayLine.clear();
  while (!psetDayLine->IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetData(psetDayLine);
    m_vDayLine.push_back(pDayLine);
    psetDayLine->MoveNext();
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
bool CStock::LoadDayLineInfo(CSetDayLineInfo* psetDayLineInfo) {
  CDayLinePtr pDayLine;
  int iPosition = 0;

  ASSERT(m_fDebugLoadDayLineFirst);

  while (!psetDayLineInfo->IsEOF()) {
    pDayLine = m_vDayLine[iPosition];
    while ((pDayLine->GetDay() < psetDayLineInfo->m_Day)
           && (m_vDayLine.size() > (iPosition + 1))) {
      iPosition++;
      pDayLine = m_vDayLine[iPosition];
    }
    if (pDayLine->GetDay() == psetDayLineInfo->m_Day) {
      pDayLine->SetData(psetDayLineInfo);
    }
    if (m_vDayLine.size() <= (iPosition + 1)) break;
    psetDayLineInfo->MoveNext();
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

bool CStock::CalculateDayLineRS(long lNumber) {
  double dTempRS = 0;
  const long lTotalNumber = m_vDayLine.size();
  for (int i = lNumber; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (int j = i - lNumber; j < i; j++) {
      dTempRS += m_vDayLine.at(j)->GetRelativeStrong();
    }
    m_vDayLine.at(i)->m_d120DayRS = dTempRS / lNumber;
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

  const long lTotalNumber = GetRTDataDequeSize(); //  ������еĳ��ȡ�����ͬ�����ƻ�ȡ����ֵ.
  if (lTotalNumber == 0) return false;
  // ����Ϊ����ҵ��仯����Ʊ��Ծ�ȡ����������
  for (long i = 0; i < lTotalNumber; i++) {
    pRTData = PopRTData(); // ����ͬ�����ƻ�ȡ����
    if (pRTData->IsActive()) { // ������Ч
      UpdateStatus(pRTData);   // ���¹�Ʊ��ʱ״̬��
      ProcessOneRTData(pRTData);
      CheckCurrentRTData();
      ShowCurrentTransaction();
      ShowCurrentInformationofCancelingGuaDan();
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
  // ��ϵͳ����ʱ�Ѿ�������ʱ״̬�����ˣ��ʶ�m_lUnknownVolume��Ϊ�㣬����Ϊ�˼��㷽������Ϊ��ʱ���ݵ�m_lUnknownVolume-m_lVolume��
  // ��������m_pLastRTData->GetVolume()�����õ���ǰ��m_lUnknownVolume.
  // ��Ϊ��m_lUnknownVolume = ��ǰ�ĳɽ��� - ֮ǰ�ĳɽ��� + ֮ǰ���޷��жϳɽ���
  m_stockCalculatedInfo.SetUnknownVolume(m_stockCalculatedInfo.GetUnknownVolume() + m_pLastRTData->GetVolume());
  // ���õ�һ�εĹҵ�ӳ�䡣
  for (int j = 0; j < 5; j++) {
    SetGuaDan(pRTData->GetPBuy(j), pRTData->GetVBuy(j));
    SetGuaDan(pRTData->GetPSell(j), pRTData->GetVSell(j));
  }
}

void CStock::CalculateOneRTData(CRTDataPtr pRTData) {
  long lCurrentGuaDanTransactionPrice = 0;

  m_lCurrentGuadanTransactionVolume = pRTData->GetVolume() - m_pLastRTData->GetVolume();
  if (m_lCurrentGuadanTransactionVolume == 0) { // �޳ɽ���
    // ���ҵ����
    m_nCurrentTransactionType = __NO_TRANSACTION__;
  }
  else { // �гɽ���
    IncreaseTransactionNumber();
    lCurrentGuaDanTransactionPrice = (pRTData->GetAmount() - m_pLastRTData->GetAmount()) * 1000 / m_lCurrentGuadanTransactionVolume; // ���ɱȽ��õļ۸񣨷Ŵ�һǧ������ó����ͣ�
    m_dCurrentGuaDanTransactionPrice = static_cast<double>(lCurrentGuaDanTransactionPrice) / 1000; // �任��ʵ�ʼ۸�
    if (lCurrentGuaDanTransactionPrice >= m_pLastRTData->GetPBuy(0)) { // ����ǰ����1
      if (lCurrentGuaDanTransactionPrice <= m_pLastRTData->GetPSell(0)) { // ����ǰ����1
        CalculateOrdinaryBuySell(lCurrentGuaDanTransactionPrice);
      }
      else if (lCurrentGuaDanTransactionPrice < m_pLastRTData->GetPSell(1)) { // ��������һ���������̶������������롣AttackBuy
        CalculateAttackBuy();
      }
      else { // �������̶���ǿ�����롣StrongBuy��
        CalculateStrongBuy();
      }
    } // ����ǰ��һ
    else if (lCurrentGuaDanTransactionPrice > m_pLastRTData->GetPBuy(1)) { // ��������һ���������̶���������������AttackSell
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

  // ���濪ʼ�����ҵ����
  AnalysisGuaDan(pRTData, lCurrentGuaDanTransactionPrice);

  // ����ǰ����״̬
  SetLastRTDataPtr(pRTData);
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

void CStock::CalculateOrdinaryBuySell(long lCurrentGuaDanTransactionPrice) {
  if ((m_pLastRTData->GetPSell(0) - lCurrentGuaDanTransactionPrice) <= 2) { //һ��������
    IncreaseOrdinaryBuyVolume(m_lCurrentGuadanTransactionVolume);
    m_nCurrentTransactionType = __ORDINARY_BUY__;
  }
  else if ((lCurrentGuaDanTransactionPrice - m_pLastRTData->GetPBuy(0)) <= 2) { // һ��������
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
// ����map�ṹ�洢����������������⣬Ψһ�������Ѱַ����ʱ�䳤����Ҫ���Ժ󿴿��ٶ���Ρ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::AnalysisGuaDan(CRTDataPtr pCurrentRTData, long lCurrentTransactionPrice) {
  array<bool, 10> fNeedCheck{ true,true,true,true,true,true,true,true,true,true }; // ��Ҫ���Ĺҵ�λ�á�˳��Ϊ��Sell4, Sell3, ... Sell0, Buy0, .... Buy3, Buy4
  m_lCurrentCanselSellVolume = 0;
  m_lCurrentCanselBuyVolume = 0;

  for (int i = 0; i < 10; i++) fNeedCheck.at(i) = true; // Ԥ��Ϊ��Ҫ���㡣

  // ȷ����Ҫ������Щ�ҵ���һ����ʮ����û���յ�����Ӱ��Ķ�Ҫ���㡣
  switch (m_nCurrentTransactionType) {
  case __NO_TRANSACTION__: // û�гɽ�������ٵ���������Ӧ��λ�ϵĳ�����
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
  fNeedCheck.at(4) = fNeedCheck.at(5) = false;
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
  fNeedCheck.at(4) = fNeedCheck.at(5) = false;
  break;
  default:
  ASSERT(0); // ������ִ�е��˷�֧��
  break;
  }

  if ((pCurrentRTData->GetPSell(4) - pCurrentRTData->GetPBuy(4)) > 90) { // ʮ����λ֮���п�λ
    // ��λ�������ǳɽ��ˣ�Ҳ�����ǳ����ˣ�Ŀǰ��������Щϸ�ڣ�ͳһ��Ϊ�ǳɽ��ˣ������㳷�������Ժ��ٷ���֮��
    // ����յ�ǰ�ҵ�֮��Ĺҵ�������Ȼ�����ϵ�ǰ��������пյ��Ļ�������Ȼ����ˡ�
    for (int i2 = pCurrentRTData->GetPBuy(4); i2 <= pCurrentRTData->GetPSell(4); i2 += 10) {
      m_mapGuaDan[i2] = 0; // �˴���Ҫ����ֵ���п���֮ǰΪ�գ��ʶ�����ʹ��at��������������[]�±������
    }
  }
  for (int i = 0; i < 5; i++) { // ��Ŀǰ��ʮ���ҵ�״̬����ӳ���У��ҵ�״̬����Ϊ����̬
    SetGuaDan(pCurrentRTData->GetPSell(i), pCurrentRTData->GetVSell(i));
    SetGuaDan(pCurrentRTData->GetPBuy(i), pCurrentRTData->GetVBuy(i));
  }

  // �����ʮ���ҵ���λ�Ϲ����ı仯���.��Ŀǰ�ҵ�״̬��֮ǰ��ʮ����λ��m_pLastRTData�����ģ���ȣ��鿴�仯
  for (int i = 0; i < 10; i++) {
    switch (i) {
    case 0: // ������
    if (fNeedCheck.at(0)) {
      if (GetGuaDan(m_pLastRTData->GetPSell(4)) < m_pLastRTData->GetVSell(4)) { // �����˵Ļ�
        m_lCurrentCanselSellVolume += m_pLastRTData->GetVSell(4) - GetGuaDan(m_pLastRTData->GetPSell(4));
        m_stockCalculatedInfo.IncreaseCancelSellVolume(m_pLastRTData->GetVSell(4) - GetGuaDan(m_pLastRTData->GetPSell(4)));
      }
    }
    break;
    case 1: // ������
    if (fNeedCheck.at(1)) {
      if (GetGuaDan(m_pLastRTData->GetPSell(3)) < m_pLastRTData->GetVSell(3)) { // �����˵Ļ�
        m_lCurrentCanselSellVolume += m_pLastRTData->GetVSell(3) - GetGuaDan(m_pLastRTData->GetPSell(3));
        m_stockCalculatedInfo.IncreaseCancelSellVolume(m_pLastRTData->GetVSell(3) - GetGuaDan(m_pLastRTData->GetPSell(3)));
      }
    }
    break;
    case 2: // ������
    if (fNeedCheck.at(2)) {
      if (GetGuaDan(m_pLastRTData->GetPSell(2)) < m_pLastRTData->GetVSell(2)) { // �����˵Ļ�
        m_lCurrentCanselSellVolume += m_pLastRTData->GetVSell(2) - GetGuaDan(m_pLastRTData->GetPSell(2));
        m_stockCalculatedInfo.IncreaseCancelSellVolume(m_pLastRTData->GetVSell(2) - GetGuaDan(m_pLastRTData->GetPSell(2)));
      }
    }
    break;
    case 3: // ������
    if (fNeedCheck.at(3)) {
      if (GetGuaDan(m_pLastRTData->GetPSell(1)) < m_pLastRTData->GetVSell(1)) { // �����˵Ļ�
        m_lCurrentCanselSellVolume += m_pLastRTData->GetVSell(1) - GetGuaDan(m_pLastRTData->GetPSell(1));
        m_stockCalculatedInfo.IncreaseCancelSellVolume(m_pLastRTData->GetVSell(1) - GetGuaDan(m_pLastRTData->GetPSell(1)));
      }
    }
    break;
    case 4: // ����һ
    if (fNeedCheck.at(4)) {
      if (GetGuaDan(m_pLastRTData->GetPSell(0)) < m_pLastRTData->GetVSell(0)) { // �����˵Ļ�
        m_lCurrentCanselSellVolume += m_pLastRTData->GetVSell(0) - GetGuaDan(m_pLastRTData->GetPSell(0));
        m_stockCalculatedInfo.IncreaseCancelSellVolume(m_pLastRTData->GetVSell(0) - GetGuaDan(m_pLastRTData->GetPSell(0)));
      }
    }
    break;
    case 5:   // ����Ϊ�򵥱仯�������һ
    if (fNeedCheck.at(5)) {
      if (GetGuaDan(m_pLastRTData->GetPBuy(0)) < m_pLastRTData->GetVBuy(0)) { // �����˵Ļ�
        m_lCurrentCanselBuyVolume += m_pLastRTData->GetVBuy(0) - GetGuaDan(m_pLastRTData->GetPBuy(0));
        m_stockCalculatedInfo.IncreaseCancelBuyVolume(m_pLastRTData->GetVBuy(0) - GetGuaDan(m_pLastRTData->GetPBuy(0)));
      }
    }
    break;
    case 6: // �򵥶�
    if (fNeedCheck.at(6)) {
      if (GetGuaDan(m_pLastRTData->GetPBuy(1)) < m_pLastRTData->GetVBuy(1)) { // �����˵Ļ�
        m_lCurrentCanselBuyVolume += m_pLastRTData->GetVBuy(1) - GetGuaDan(m_pLastRTData->GetPBuy(1));
        m_stockCalculatedInfo.IncreaseCancelBuyVolume(m_pLastRTData->GetVBuy(1) - GetGuaDan(m_pLastRTData->GetPBuy(1)));
      }
    }
    break;
    case 7: // ����
    if (fNeedCheck.at(7)) {
      if (GetGuaDan(m_pLastRTData->GetPBuy(2)) < m_pLastRTData->GetVBuy(2)) { // �����˵Ļ�
        m_lCurrentCanselBuyVolume += m_pLastRTData->GetVBuy(2) - GetGuaDan(m_pLastRTData->GetPBuy(2));
        m_stockCalculatedInfo.IncreaseCancelBuyVolume(m_pLastRTData->GetVBuy(2) - GetGuaDan(m_pLastRTData->GetPBuy(2)));
      }
    }
    break;
    case 8: // ����
    if (fNeedCheck.at(8)) {
      if (GetGuaDan(m_pLastRTData->GetPBuy(3)) < m_pLastRTData->GetVBuy(3)) { // �����˵Ļ�
        m_lCurrentCanselBuyVolume += m_pLastRTData->GetVBuy(3) - GetGuaDan(m_pLastRTData->GetPBuy(3));
        m_stockCalculatedInfo.IncreaseCancelBuyVolume(m_pLastRTData->GetVBuy(3) - GetGuaDan(m_pLastRTData->GetPBuy(3)));
      }
    }
    break;
    case 9: // ����
    if (fNeedCheck.at(9)) {
      if (GetGuaDan(m_pLastRTData->GetPBuy(4)) < m_pLastRTData->GetVBuy(4)) { // �����˵Ļ�
        m_lCurrentCanselBuyVolume += m_pLastRTData->GetVBuy(4) - GetGuaDan(m_pLastRTData->GetPBuy(4));
        m_stockCalculatedInfo.IncreaseCancelBuyVolume(m_pLastRTData->GetVBuy(4) - GetGuaDan(m_pLastRTData->GetPBuy(4)));
      }
    }
    break;
    default: // û�����ѡ��
    ASSERT(0);
    }
  }

  return(true);
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
    TRACE(_T("%06d %s Error in volume. Error  code = %d\n"), gl_systemTime.GetTime(), m_stockBasicInfo.GetStockCode(), j);
    return false;
  }
  return true;
}

void CStock::ShowCurrentTransaction() {
  // ��ʾ��ǰ�������
  if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
    if (gl_ChinaStockMarket.m_pCurrentStock->GetStockCode().Compare(m_stockBasicInfo.GetStockCode()) == 0) {
      if (gl_ChinaStockMarket.m_pCurrentStock->GetCurrentTransationVolume() > 0) {
        gl_ChinaStockMarket.m_pCurrentStock->ReportGuaDanTransaction();
      }
    }
  }
}

void CStock::ShowCurrentInformationofCancelingGuaDan(void) {
  // ��ʾ��ǰȡ���ҵ������
  if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
    if (gl_ChinaStockMarket.m_pCurrentStock->GetStockCode().Compare(m_stockBasicInfo.GetStockCode()) == 0) {
      gl_ChinaStockMarket.m_pCurrentStock->ReportGuaDan();
    }
  }
}

void CStock::ReportGuaDanTransaction(void) {
  char buffer[100];
  CString str, str2, strTime;
  const CTime ctime(m_pLastRTData->GetTransactionTime());
  sprintf_s(buffer, "%02d:%02d:%02d", ctime.GetHour(), ctime.GetMinute(), ctime.GetSecond());
  strTime = buffer;
  sprintf_s(buffer, " %s %I64d�ɳɽ���%10.3f    ", m_stockBasicInfo.GetStockCode().GetBuffer(),
            m_lCurrentGuadanTransactionVolume, m_dCurrentGuaDanTransactionPrice);
  str = strTime;
  str += buffer;
  CString str1;
  switch (m_nCurrentTransactionType) {
  case __STRONG_BUY__:
  str1 = _T(" STRONG BUY");
  sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetStrongBuyVolume());
  break;
  case __STRONG_SELL__:
  str1 = _T(" STRONG SELL");
  sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetStrongSellVolume());
  break;
  case __ATTACK_BUY__:
  str1 = _T(" ATTACK BUY");
  sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetAttackBuyVolume());
  break;
  case __ATTACK_SELL__:
  str1 = _T(" ATTACK SELL");
  sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetAttackSellVolume());
  break;
  case __ORDINARY_BUY__:
  str1 = _T(" ORDINARY BUY");
  sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetOrdinaryBuyVolume());
  break;
  case __ORDINARY_SELL__:
  str1 = _T(" ORDINARY SELL");
  sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetOrdinarySellVolume());
  break;
  case __UNKNOWN_BUYSELL__:
  str1 = _T(" UNKNOWN BUYSELL");
  sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_stockCalculatedInfo.GetUnknownVolume());
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

void CStock::ReportGuaDan(void) {
  CString str1;
  char buffer[30];
  if (m_lCurrentCanselSellVolume > 0) {
    sprintf_s(buffer, "��ǰȡ����������%I64d", m_lCurrentCanselSellVolume);
    str1 = buffer;
    sprintf_s(buffer, "  ��ȡ����������%I64d", m_stockCalculatedInfo.GetCancelSellVolume());
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1);   // ����ͬ�����ƴ�����Ϣ
  }
  if (m_lCurrentCanselBuyVolume > 0) {
    sprintf_s(buffer, "��ǰȡ��������%I64d", m_lCurrentCanselBuyVolume);
    str1 = buffer;
    sprintf_s(buffer, "  ��ȡ��������%I64d", m_stockCalculatedInfo.GetCancelBuyVolume());
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1); // ����ͬ�����ƴ�����Ϣ
  }
}

bool CStock::SaveRealTimeData(CSetRealTimeData* psetRTData) {
  ASSERT(psetRTData != nullptr);
  ASSERT(psetRTData->IsOpen());
  for (auto pRTData : m_dequeRTData) {
    psetRTData->AddNew();
    psetRTData->m_Time = pRTData->GetTransactionTime();
    psetRTData->m_lMarket = m_stockBasicInfo.GetMarket();
    psetRTData->m_StockCode = m_stockBasicInfo.GetStockCode();
    psetRTData->m_StockName = m_stockBasicInfo.GetStockName();
    psetRTData->m_CurrentPrice = ConvertValueToString(pRTData->GetNew(), 1000);
    psetRTData->m_High = ConvertValueToString(pRTData->GetHigh(), 1000);
    psetRTData->m_Low = ConvertValueToString(pRTData->GetLow(), 1000);
    psetRTData->m_LastClose = ConvertValueToString(pRTData->GetLastClose(), 1000);
    psetRTData->m_Open = ConvertValueToString(pRTData->GetOpen(), 1000);
    psetRTData->m_Volume = ConvertValueToString(pRTData->GetVolume());
    psetRTData->m_Amount = ConvertValueToString(pRTData->GetAmount());
    psetRTData->m_Stroke = _T("0");
    psetRTData->m_PBuy1 = ConvertValueToString(pRTData->GetPBuy(0), 1000);
    psetRTData->m_VBuy1 = ConvertValueToString(pRTData->GetVBuy(0));
    psetRTData->m_PSell1 = ConvertValueToString(pRTData->GetPSell(0), 1000);
    psetRTData->m_VSell1 = ConvertValueToString(pRTData->GetVSell(0));

    psetRTData->m_PBuy2 = ConvertValueToString(pRTData->GetPBuy(1), 1000);
    psetRTData->m_VBuy2 = ConvertValueToString(pRTData->GetVBuy(1));
    psetRTData->m_PSell2 = ConvertValueToString(pRTData->GetPSell(1), 1000);
    psetRTData->m_VSell2 = ConvertValueToString(pRTData->GetVSell(1));

    psetRTData->m_PBuy3 = ConvertValueToString(pRTData->GetPBuy(2), 1000);
    psetRTData->m_VBuy3 = ConvertValueToString(pRTData->GetVBuy(2));
    psetRTData->m_PSell3 = ConvertValueToString(pRTData->GetPSell(2), 1000);
    psetRTData->m_VSell3 = ConvertValueToString(pRTData->GetVSell(2));

    psetRTData->m_PBuy4 = ConvertValueToString(pRTData->GetPBuy(3), 1000);
    psetRTData->m_VBuy4 = ConvertValueToString(pRTData->GetVBuy(3));
    psetRTData->m_PSell4 = ConvertValueToString(pRTData->GetPSell(3), 1000);
    psetRTData->m_VSell4 = ConvertValueToString(pRTData->GetVSell(3));

    psetRTData->m_PBuy5 = ConvertValueToString(pRTData->GetPBuy(4), 1000);
    psetRTData->m_VBuy5 = ConvertValueToString(pRTData->GetVBuy(4));
    psetRTData->m_PSell5 = ConvertValueToString(pRTData->GetPSell(4), 1000);
    psetRTData->m_VSell5 = ConvertValueToString(pRTData->GetVSell(4));
    psetRTData->Update();
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ�����ƴ洢ʵʱ���ݡ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CStock::PushRTData(CRTDataPtr pData) {
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeRTData.push_back(pData);
    singleLock.Unlock();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ��������ȡʵʱ���ݡ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
CRTDataPtr CStock::PopRTData(void) {
  CRTDataPtr pData;
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    pData = m_dequeRTData.front();
    m_dequeRTData.pop_front();
    singleLock.Unlock();
    return pData;
  }
  ASSERT(0);
  return nullptr;
}

CRTDataPtr CStock::GetRTDataAtHead(void) {
  CRTDataPtr pData;
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    pData = m_dequeRTData.front();
    singleLock.Unlock();
    return pData;
  }
  ASSERT(0);
  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ��������ȡջ�ڵ�ʵʱ����������
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
long CStock::GetRTDataDequeSize(void) {
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeRTData.size();
    singleLock.Unlock();
    return lCount;
  }
  ASSERT(0);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// �жϹ�Ʊ�����Ƿ���Ч��������߼ۡ���ͼۡ��ɽ����ͳɽ������жϣ������Ϊ�㣬����Ϊ�˹ɽ���û����Ч����
// �����ÿ��̼ۣ����̼��п��ܲ�Ϊ��ʱ���������ݽ�Ϊ�㣨ͣ��ʱ����״̬����
//
////////////////////////////////////////////////////////////////////////////////
bool CStock::TodayDataIsActive(void) {
  if (!m_fActive) return false;
  else {
    return TodayDataIsValid();
  }
}

bool CStock::TodayDataIsValid(void) {
  if ((GetHigh() != 0) || (GetLow() != 0) || (GetAmount() != 0) || (GetVolume() != 0)) {
    return true;
  }
  else {
    return false;
  }
}

#ifdef _DEBUG
void CStock::AssertValid() const {
  CObject::AssertValid();
}

void CStock::Dump(CDumpContext& dc) const {
  CObject::Dump(dc);
}

#endif //_DEBUG