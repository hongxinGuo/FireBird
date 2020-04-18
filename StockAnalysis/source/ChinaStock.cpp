#include "stdafx.h"

#include "globedef.h"
//#include "timer.h"

#include"Accessory.h"

#include "ChinaStock.h"
#include"ChinaMarket.h"

using namespace std;
#include"string"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChinaStock::CChinaStock() : CObject() {
  Reset();
}

CChinaStock::~CChinaStock(void) {
  if (m_pDayLineBuffer != nullptr) {
    delete m_pDayLineBuffer;
    m_pDayLineBuffer = nullptr;
  }
}

void CChinaStock::Reset(void) {
  m_wMarket = 0;
  m_strStockCode = _T("");
  m_strStockName = _T("");
  m_lOffsetInContainer = -1;
  m_lDayLineStartDay = __CHINA_MARKET_BEGIN_DAY__; //
  m_lDayLineEndDay = __CHINA_MARKET_BEGIN_DAY__; //
  m_lIPOed = __STOCK_NOT_CHECKED__;   // Ĭ��״̬Ϊ��Ч��Ʊ���롣
  m_nHand = 100;

  m_TransactionTime = 0;
  m_lLastClose = m_lOpen = 0;
  m_lHigh = m_lLow = m_lNew = 0;
  m_lHighLimit = m_lLowLimit = 0;
  m_llVolume = 0;
  m_llAmount = 0;
  m_lUpDown = 0;
  m_dUpDownRate = 0;
  m_dChangeHandRate = 0;
  m_llTotalValue = m_llCurrentValue = 0;
  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = m_lPSell.at(i) = 0;
    m_lVBuy.at(i) = m_lVSell.at(i) = 0;
  }
  m_dRelativeStrong = 0;

  m_lAttackBuyAmount = 0;
  m_lAttackSellAmount = 0;
  m_lAttackBuyVolume = 0;
  m_lCurrentAttackBuy = 0;
  m_lAttackSellVolume = 0;
  m_lCurrentAttackSell = 0;
  m_lStrongBuyVolume = 0;
  m_lCurrentStrongBuy = 0;
  m_lStrongSellVolume = 0;
  m_lCurrentStrongSell = 0;
  m_lUnknownVolume = 0;
  m_lCurrentUnknown = 0;
  m_lCancelBuyVolume = 0;
  m_lCancelSellVolume = 0;
  m_lTransactionNumber = 0;
  m_lTransactionNumberBelow5000 = 0;
  m_lTransactionNumberBelow50000 = 0;
  m_lTransactionNumberBelow200000 = 0;
  m_lTransactionNumberAbove200000 = 0;

  m_lOrdinaryBuyNumberBelow5000 = 0;
  m_lOrdinaryBuyNumberBelow10000 = 0;
  m_lOrdinaryBuyNumberBelow20000 = 0;
  m_lOrdinaryBuyNumberBelow50000 = 0;
  m_lOrdinaryBuyNumberBelow100000 = 0;
  m_lOrdinaryBuyNumberBelow200000 = 0;
  m_lOrdinaryBuyNumberAbove200000 = 0;
  m_lOrdinarySellNumberBelow5000 = 0;
  m_lOrdinarySellNumberBelow10000 = 0;
  m_lOrdinarySellNumberBelow20000 = 0;
  m_lOrdinarySellNumberBelow50000 = 0;
  m_lOrdinarySellNumberBelow100000 = 0;
  m_lOrdinarySellNumberBelow200000 = 0;
  m_lOrdinarySellNumberAbove200000 = 0;
  m_lOrdinaryBuyVolumeBelow5000 = 0;
  m_lOrdinaryBuyVolumeBelow10000 = 0;
  m_lOrdinaryBuyVolumeBelow20000 = 0;
  m_lOrdinaryBuyVolumeBelow50000 = 0;
  m_lOrdinaryBuyVolumeBelow100000 = 0;
  m_lOrdinaryBuyVolumeBelow200000 = 0;
  m_lOrdinaryBuyVolumeAbove200000 = 0;
  m_lOrdinarySellVolumeBelow5000 = 0;
  m_lOrdinarySellVolumeBelow10000 = 0;
  m_lOrdinarySellVolumeBelow20000 = 0;
  m_lOrdinarySellVolumeBelow50000 = 0;
  m_lOrdinarySellVolumeBelow100000 = 0;
  m_lOrdinarySellVolumeBelow200000 = 0;
  m_lOrdinarySellVolumeAbove200000 = 0;

  m_lCancelBuyVolumeBelow5000 = 0;
  m_lCancelBuyVolumeBelow10000 = 0;
  m_lCancelBuyVolumeBelow20000 = 0;
  m_lCancelBuyVolumeBelow50000 = 0;
  m_lCancelBuyVolumeBelow100000 = 0;
  m_lCancelBuyVolumeBelow200000 = 0;
  m_lCancelBuyVolumeAbove200000 = 0;
  m_lCancelSellVolumeBelow5000 = 0;
  m_lCancelSellVolumeBelow10000 = 0;
  m_lCancelSellVolumeBelow20000 = 0;
  m_lCancelSellVolumeBelow50000 = 0;
  m_lCancelSellVolumeBelow100000 = 0;
  m_lCancelSellVolumeBelow200000 = 0;
  m_lCancelSellVolumeAbove200000 = 0;

  m_lOrdinaryBuyVolume = m_lAttackBuyBelow50000 = m_lAttackBuyBelow200000 = m_lAttackBuyAbove200000 = 0;
  m_lOrdinarySellVolume = m_lAttackSellBelow50000 = m_lAttackSellBelow200000 = m_lAttackSellAbove200000 = 0;

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
  m_fSaveToChoicedStockDB = false;
  m_fMinLineUpdated = false;
  m_fDayLineUpdated = false;

  m_fDayLineDBUpdated = false;

  m_fHaveFirstRTData = false;  // ʵʱ���ݿ�ʼ�����ʶ����һ��ʵʱ����ֻ��������ʼ��ϵͳ���������ڼ��㡣�ӵڶ������ݿ�ʼ�������Ч��
  m_fNeedProcessRTData = true;
  m_fRTDataCalculated = false;
  m_fRecordRTData = false;
  m_pLastRTData = nullptr;

  ClearRTDataDeque();

  m_fLoadDayLineFirst = false;
}

void CChinaStock::ClearRTDataDeque(void) {
  long lTotalNumber = GetRTDataQueueSize();
  for (int i = 0; i < lTotalNumber; i++) {
    CRTDataPtr pRTData = PopRTData();
  }
}

bool CChinaStock::HaveNewDayLineData(void) {
  if (m_vDayLine.size() <= 0) return false;
  if (m_vDayLine.at(m_vDayLine.size() - 1)->GetDay() > GetDayLineEndDay()) return true;
  else return false;
}

void CChinaStock::SetDayLineNeedSaving(bool fFlag) {
  if (fFlag) {
    ASSERT(!m_fDayLineNeedSaving);
    m_fDayLineNeedSaving = true;
    gl_pChinaStockMarket->IncreaseNeteaseDayLineNeedSaveNumber();
  }
  else {
    ASSERT(m_fDayLineNeedSaving);
    m_fDayLineNeedSaving = false;
    gl_pChinaStockMarket->DecreaseNeteaseDayLineNeedSaveNumber();
  }
}

bool CChinaStock::IsDayLineNeedSavingAndClearFlag(void) {
  bool fNeedSaveing = m_fDayLineNeedSaving.exchange(false);
  if (fNeedSaveing) gl_pChinaStockMarket->DecreaseNeteaseDayLineNeedSaveNumber();
  return fNeedSaveing;
}

bool CChinaStock::TransferNeteaseDayLineWebDataToBuffer(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData) {
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
bool CChinaStock::ProcessNeteaseDayLineData(void) {
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
    if (GetDayLineEndDay() == __CHINA_MARKET_BEGIN_DAY__) { // �����ʼ���߽������ڴ���û�б��������˹�Ʊ������δ��ʹ�ù�
      SetIPOStatus(__STOCK_NULL__);   // �˹�Ʊ������δʹ�á�
      //TRACE("��Ч��Ʊ����:%s\n", GetStockCode().GetBuffer());
    }
    else { // �Ѿ����еĹ�Ʊ
      if (gl_pChinaStockMarket->IsEarlyThen(GetDayLineEndDay(), gl_pChinaStockMarket->GetDay(), 30)) {
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
  if (gl_pChinaStockMarket->IsEarlyThen(vTempDayLine.at(0)->GetDay(), gl_pChinaStockMarket->GetDay(), 30)) { // ��ȡ���Ĺ�Ʊ���������������������ϸ��µ���������գ������˻������������һ���µ�ʱ��۲죩��
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

bool CChinaStock::SkipNeteaseDayLineInformationHeader() {
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

void CChinaStock::SetTodayActive(WORD wMarket, CString strStockCode, CString strStockName) {
  SetActive(true);
  SetDayLineLoaded(false);
  SetMarket(wMarket);
  SetStockCode(strStockCode); // ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
  SetStockName(strStockName);// ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
  gl_pChinaStockMarket->SetTotalActiveStock(gl_pChinaStockMarket->GetTotalActiveStock() + 1);
}

void CChinaStock::StoreDayLine(vector<CDayLinePtr>& vTempDayLine) {
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

void CChinaStock::ReportDayLineDownLoaded(void) {
  CString strTemp = GetStockCode();
  strTemp += _T("�����������.");
  gl_systemMessage.PushDayLineInfoMessage(strTemp);
}

void CChinaStock::SaveBasicInfo(CSetDayLine& setDayLine) {
  ASSERT(setDayLine.IsOpen());
  setDayLine.m_Day = FormatToDay(m_TransactionTime);
  setDayLine.m_Market = m_wMarket;
  setDayLine.m_StockCode = m_strStockCode;
  setDayLine.m_StockName = m_strStockName;
  setDayLine.m_LastClose = ConvertValueToString(m_lLastClose, 1000);
  setDayLine.m_Open = ConvertValueToString(m_lOpen, 1000);
  setDayLine.m_High = ConvertValueToString(m_lHigh, 1000);
  setDayLine.m_Low = ConvertValueToString(m_lLow, 1000);
  setDayLine.m_Close = ConvertValueToString(m_lNew, 1000);
  setDayLine.m_Volume = ConvertValueToString(m_llVolume);
  setDayLine.m_Amount = ConvertValueToString(m_llAmount);
  setDayLine.m_UpAndDown = ConvertValueToString(m_lUpDown, 1000);
  setDayLine.m_UpDownRate = ConvertValueToString(m_dUpDownRate);
  if (m_llTotalValue != 0) setDayLine.m_ChangeHandRate = ConvertValueToString((double)100 * m_llAmount / m_llTotalValue);
  else setDayLine.m_ChangeHandRate = ConvertValueToString(0);
  setDayLine.m_TotalValue = ConvertValueToString(m_llTotalValue);
  setDayLine.m_CurrentValue = ConvertValueToString(m_llCurrentValue);
}

void CChinaStock::SaveTempInfo(CSetDayLineToday& setDayLineToday) {
  ASSERT(setDayLineToday.IsOpen());
  setDayLineToday.m_Day = FormatToDay(m_TransactionTime);
  setDayLineToday.m_Market = m_wMarket;
  setDayLineToday.m_StockCode = m_strStockCode;
  setDayLineToday.m_StockName = m_strStockName;
  setDayLineToday.m_LastClose = ConvertValueToString(m_lLastClose, 1000);
  setDayLineToday.m_Open = ConvertValueToString(m_lOpen, 1000);
  setDayLineToday.m_High = ConvertValueToString(m_lHigh, 1000);
  setDayLineToday.m_Low = ConvertValueToString(m_lLow, 1000);
  setDayLineToday.m_Close = ConvertValueToString(m_lNew, 1000);
  setDayLineToday.m_Volume = ConvertValueToString(m_llVolume);
  setDayLineToday.m_Amount = ConvertValueToString(m_llAmount);
  setDayLineToday.m_UpAndDown = ConvertValueToString(m_lUpDown, 1000);
  setDayLineToday.m_UpDownRate = ConvertValueToString(m_dUpDownRate);
  setDayLineToday.m_TotalValue = ConvertValueToString(m_llTotalValue);
  setDayLineToday.m_CurrentValue = ConvertValueToString(m_llCurrentValue);
  setDayLineToday.m_TransactionNumber = ConvertValueToString(m_lTransactionNumber);
  setDayLineToday.m_TransactionNumberBelow5000 = ConvertValueToString(m_lTransactionNumberBelow5000);
  setDayLineToday.m_TransactionNumberBelow50000 = ConvertValueToString(m_lTransactionNumberBelow50000);
  setDayLineToday.m_TransactionNumberBelow200000 = ConvertValueToString(m_lTransactionNumberBelow200000);
  setDayLineToday.m_TransactionNumberAbove200000 = ConvertValueToString(m_lTransactionNumberAbove200000);
  setDayLineToday.m_CancelBuyVolume = ConvertValueToString(m_lCancelBuyVolume);
  setDayLineToday.m_CancelSellVolume = ConvertValueToString(m_lCancelSellVolume);
  setDayLineToday.m_AttackBuyVolume = ConvertValueToString(m_lAttackBuyVolume);
  setDayLineToday.m_AttackSellVolume = ConvertValueToString(m_lAttackSellVolume);
  setDayLineToday.m_StrongBuyVolume = ConvertValueToString(m_lStrongBuyVolume);
  setDayLineToday.m_StrongSellVolume = ConvertValueToString(m_lStrongSellVolume);
  setDayLineToday.m_UnknownVolume = ConvertValueToString(m_lUnknownVolume);
  setDayLineToday.m_OrdinaryBuyVolume = ConvertValueToString(m_lOrdinaryBuyVolume);
  setDayLineToday.m_OrdinarySellVolume = ConvertValueToString(m_lOrdinarySellVolume);
  setDayLineToday.m_AttackBuyBelow50000 = ConvertValueToString(m_lAttackBuyBelow50000);
  setDayLineToday.m_AttackBuyBelow200000 = ConvertValueToString(m_lAttackBuyBelow200000);
  setDayLineToday.m_AttackBuyAbove200000 = ConvertValueToString(m_lAttackBuyAbove200000);
  setDayLineToday.m_AttackSellBelow50000 = ConvertValueToString(m_lAttackSellBelow50000);
  setDayLineToday.m_AttackSellBelow200000 = ConvertValueToString(m_lAttackSellBelow200000);
  setDayLineToday.m_AttackSellAbove200000 = ConvertValueToString(m_lAttackSellAbove200000);
}

void CChinaStock::UpdateStatus(CRTDataPtr pRTData) {
  SetTransactionTime(pRTData->GetTransactionTime());
  SetLastClose(pRTData->GetLastClose());
  SetNew(pRTData->GetNew());
  SetHigh(pRTData->GetHigh());
  SetLow(pRTData->GetLow());
  SetOpen(pRTData->GetOpen());
  SetVolume(pRTData->GetVolume());
  SetAmount(pRTData->GetAmount());
  SetUpDown(m_lNew - m_lLastClose);
  if (m_lLastClose != 0) SetUpDownRate((double)m_lUpDown * 100 / m_lLastClose);
  else SetUpDownRate(0);
  // ����ʵʱ������û������ֵ����ͨ��ֵ������ģ��ʶ���Ҫ�ж�һ���Ƿ�Ϊ��
  if (pRTData->GetTotalValue() > 0) SetTotalValue(pRTData->GetTotalValue());
  if (pRTData->GetCurrentValue() > 0) SetCurrentValue(pRTData->GetCurrentValue());
  if (m_llTotalValue != 0) m_dChangeHandRate = (double)m_llAmount * 100 / m_llTotalValue;
  else m_dChangeHandRate = 0;
  for (int i = 0; i < 5; i++) {
    SetPBuy(i, pRTData->GetPBuy(i));
    SetVBuy(i, pRTData->GetVBuy(i));
    SetPSell(i, pRTData->GetPSell(i));
    SetVSell(i, pRTData->GetVSell(i));
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��������ʷ���ݴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//  �˺����������̵߳��ã���Ҫע������ͬ�����⡣
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::SaveDayLine(void) {
  CSetDayLine setDayLine;
  long lSize = 0;
  vector<CDayLinePtr> vDayLine;
  CDayLinePtr pDayLine = nullptr;
  long lCurrentPos = 0, lSizeOfOldDayLine = 0;
  bool fNeedUpdate = false;

  ASSERT(m_vDayLine.size() > 0);
  ASSERT(m_vDayLine.at(m_vDayLine.size() - 1)->GetDay() > m_lDayLineEndDay);

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
    SetDayLineStartDay(gl_pChinaStockMarket->GetDay());
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

void CChinaStock::UpdateDayLineStartEndDay(void) {
  if (m_vDayLine.at(0)->GetDay() < GetDayLineStartDay()) {
    SetDayLineStartDay(m_vDayLine.at(0)->GetDay());
    SetDayLineDBUpdated(true);
  }
  if (m_vDayLine.at(m_vDayLine.size() - 1)->GetDay() > GetDayLineEndDay()) {
    SetDayLineEndDay(m_vDayLine.at(m_vDayLine.size() - 1)->GetDay());
    SetDayLineDBUpdated(true);
  }
}

void CChinaStock::SaveCalculatedInfo(CSetDayLineInfo& setDayLineInfo) {
  ASSERT(setDayLineInfo.IsOpen());
  setDayLineInfo.m_Day = FormatToDay(m_TransactionTime);
  setDayLineInfo.m_Market = m_wMarket;
  setDayLineInfo.m_StockCode = m_strStockCode;
  setDayLineInfo.m_TransactionNumber = ConvertValueToString(m_lTransactionNumber);
  setDayLineInfo.m_TransactionNumberBelow5000 = ConvertValueToString(m_lTransactionNumberBelow5000);
  setDayLineInfo.m_TransactionNumberBelow50000 = ConvertValueToString(m_lTransactionNumberBelow50000);
  setDayLineInfo.m_TransactionNumberBelow200000 = ConvertValueToString(m_lTransactionNumberBelow200000);
  setDayLineInfo.m_TransactionNumberAbove200000 = ConvertValueToString(m_lTransactionNumberAbove200000);

  setDayLineInfo.m_CancelBuyVolume = ConvertValueToString(m_lCancelBuyVolume);
  setDayLineInfo.m_CancelSellVolume = ConvertValueToString(m_lCancelSellVolume);
  setDayLineInfo.m_AttackBuyVolume = ConvertValueToString(m_lAttackBuyVolume);
  setDayLineInfo.m_AttackSellVolume = ConvertValueToString(m_lAttackSellVolume);
  setDayLineInfo.m_StrongBuyVolume = ConvertValueToString(m_lStrongBuyVolume);
  setDayLineInfo.m_StrongSellVolume = ConvertValueToString(m_lStrongSellVolume);
  setDayLineInfo.m_UnknownVolume = ConvertValueToString(m_lUnknownVolume);
  setDayLineInfo.m_OrdinaryBuyVolume = ConvertValueToString(m_lOrdinaryBuyVolume);
  setDayLineInfo.m_OrdinarySellVolume = ConvertValueToString(m_lOrdinarySellVolume);
  setDayLineInfo.m_AttackBuyBelow50000 = ConvertValueToString(m_lAttackBuyBelow50000);
  setDayLineInfo.m_AttackBuyBelow200000 = ConvertValueToString(m_lAttackBuyBelow200000);
  setDayLineInfo.m_AttackBuyAbove200000 = ConvertValueToString(m_lAttackBuyAbove200000);
  setDayLineInfo.m_AttackSellBelow50000 = ConvertValueToString(m_lAttackSellBelow50000);
  setDayLineInfo.m_AttackSellBelow200000 = ConvertValueToString(m_lAttackSellBelow200000);
  setDayLineInfo.m_AttackSellAbove200000 = ConvertValueToString(m_lAttackSellAbove200000);
}

////////////////////////////////////////////////////////////////////////////
//
// ֻ��һ�����ñ�������������ϵͳ��ʼ��ʱװ��֮ǰ���������Ϣ����������˵Ļ���.
// ��Ҫͬʱ�����ܳɽ����������ݴ�˹��������ڼ���δ������ɽ����� �ܳɽ��������µ�ʵʱ��������ʱ��ͬ�����£��ʶ��޷����ڼ��㣩
//
////////////////////////////////////////////////////////////////////////////
void CChinaStock::LoadTempInfo(CSetDayLineToday& setDayLineToday) {
  m_lUnknownVolume = atoll(setDayLineToday.m_UnknownVolume);

  m_lTransactionNumber = atol(setDayLineToday.m_TransactionNumber);
  m_lTransactionNumberBelow5000 = atol(setDayLineToday.m_TransactionNumberBelow5000);
  m_lTransactionNumberBelow50000 = atol(setDayLineToday.m_TransactionNumberBelow50000);
  m_lTransactionNumberBelow200000 = atol(setDayLineToday.m_TransactionNumberBelow200000);
  m_lTransactionNumberAbove200000 = atol(setDayLineToday.m_TransactionNumberAbove200000);
  m_lCancelBuyVolume = atoll(setDayLineToday.m_CancelBuyVolume);
  m_lCancelSellVolume = atoll(setDayLineToday.m_CancelSellVolume);
  m_lAttackBuyVolume = atoll(setDayLineToday.m_AttackBuyVolume);
  m_lAttackSellVolume = atoll(setDayLineToday.m_AttackSellVolume);
  m_lStrongBuyVolume = atoll(setDayLineToday.m_StrongBuyVolume);
  m_lStrongSellVolume = atoll(setDayLineToday.m_StrongSellVolume);
  m_lOrdinaryBuyVolume = atoll(setDayLineToday.m_OrdinaryBuyVolume);
  m_lOrdinarySellVolume = atoll(setDayLineToday.m_OrdinarySellVolume);
  m_lAttackBuyBelow50000 = atoll(setDayLineToday.m_AttackBuyBelow50000);
  m_lAttackBuyBelow200000 = atoll(setDayLineToday.m_AttackBuyBelow200000);
  m_lAttackBuyAbove200000 = atoll(setDayLineToday.m_AttackBuyAbove200000);
  m_lAttackSellBelow50000 = atoll(setDayLineToday.m_AttackSellBelow50000);
  m_lAttackSellBelow200000 = atoll(setDayLineToday.m_AttackSellBelow200000);
  m_lAttackSellAbove200000 = atoll(setDayLineToday.m_AttackSellAbove200000);
  m_llVolume = atoll(setDayLineToday.m_Volume);
  m_llLastSavedVolume = atoll(setDayLineToday.m_Volume);
}

bool CChinaStock::LoadDayLineAndDayLineInfo(void) {
  CSetDayLine setDayLine;
  CSetDayLineInfo setDayLineInfo;

  // װ��DayLine����
  setDayLine.m_strFilter = _T("[StockCode] = '");
  setDayLine.m_strFilter += GetStockCode();
  setDayLine.m_strFilter += _T("'");
  setDayLine.m_strSort = _T("[Day]");
  setDayLine.Open();
  LoadDayLine(setDayLine);
  setDayLine.Close();

  // װ��DayLineInfo����
  setDayLineInfo.m_strFilter = _T("[StockCode] = '");
  setDayLineInfo.m_strFilter += GetStockCode();
  setDayLineInfo.m_strFilter += _T("'");
  setDayLineInfo.m_strSort = _T("[Day]");
  setDayLineInfo.Open();
  LoadDayLineInfo(setDayLineInfo);
  setDayLineInfo.Close();

  return true;
}

bool CChinaStock::LoadDayLine(CSetDayLine& setDayLine) {
  CDayLinePtr pDayLine;

  if (gl_fNormalMode) ASSERT(!m_fLoadDayLineFirst);
  // װ��DayLine����
  m_vDayLine.clear();
  while (!setDayLine.IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->LoadData(setDayLine);
    m_vDayLine.push_back(pDayLine);
    setDayLine.MoveNext();
  }
  m_fLoadDayLineFirst = true;
  return true;
}

/////////////////////////////////////////////////////////////////////////////
//
// װ��DayLineInfo�������װ��DayLine��֮��
//
//
////////////////////////////////////////////////////////////////////////////
bool CChinaStock::LoadDayLineInfo(CSetDayLineInfo& setDayLineInfo) {
  CDayLinePtr pDayLine;
  int iPosition = 0;

  if (gl_fNormalMode) ASSERT(m_fLoadDayLineFirst);

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
  m_fLoadDayLineFirst = false;
  return true;
}

bool CChinaStock::CalculateDayLineRelativeStrong(void) {
  CalculateDayLineRS(3);
  CalculateDayLineRS(5);
  CalculateDayLineRS(10);
  CalculateDayLineRS(30);
  CalculateDayLineRS(60);
  CalculateDayLineRS(120);
  return true;
}

bool CChinaStock::CalculateDayLineRelativeStrongLogarithm(void) {
  CalculateDayLineRSLogarithm(3);
  CalculateDayLineRSLogarithm(5);
  CalculateDayLineRSLogarithm(10);
  CalculateDayLineRSLogarithm(30);
  CalculateDayLineRSLogarithm(60);
  CalculateDayLineRSLogarithm(120);
  return true;
}

bool CChinaStock::CalculateDayLineRelativeStrongIndex(void) {
  CalculateDayLineRSIndex(3);
  CalculateDayLineRSIndex(5);
  CalculateDayLineRSIndex(10);
  CalculateDayLineRSIndex(30);
  CalculateDayLineRSIndex(60);
  CalculateDayLineRSIndex(120);
  return true;
}

bool CChinaStock::CalculateDayLineRSLogarithm(INT64 lNumber) {
  double dTempRS = 0;
  const INT64 lTotalNumber = m_vDayLine.size();
  for (INT64 i = lNumber; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (INT64 j = i - lNumber; j < i; j++) {
      dTempRS += m_vDayLine.at(j)->GetRelativeStrongLogarithm();
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
  return true;
}

bool CChinaStock::CalculateDayLineRS(INT64 lNumber) {
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
  return true;
}

bool CChinaStock::CalculateDayLineRSIndex(INT64 lNumber) {
  double dTempRS = 0;
  const INT64 lTotalNumber = m_vDayLine.size();
  for (INT64 i = lNumber; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (INT64 j = i - lNumber; j < i; j++) {
      dTempRS += m_vDayLine.at(j)->GetRelativeStrongIndex();
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
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// �˺����������߳�ThreadCalculatingRTDataProc���ã��������ٶ�ȫ�ֱ����Ĳ�����
//
// ��������͹ҵ�����ƺ��ֿ�����ȽϺã�˼·����������Ŀǰ�������������й�����Ҫ���Ǿɻ������ٶ��ˡ�
//
//
////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::ProcessRTData(void) {
  CRTDataPtr pRTData;

  INT64 lTotalNumber = GetRTDataQueueSize(); //  ������еĳ��ȡ�����ͬ�����ƻ�ȡ����ֵ.
  // ����Ϊ����ҵ��仯����Ʊ��Ծ�ȡ����������
  for (INT64 i = 0; i < lTotalNumber; i++) {
    pRTData = PopRTData(); // ����ͬ�����ƻ�ȡ����
    if (pRTData->IsActive()) { // ������Ч
      UpdateStatus(pRTData);   // ���¹�Ʊ��ʱ״̬��
      if (gl_pChinaStockMarket->IsMarketOpened() && IsNeedProcessRTData()) {// ����ʱ���ڼ�����������ָ�����Ʊ������㽻������͹ҵ��仯
        ProcessOneRTData(pRTData);
        CheckCurrentRTData();
        m_fRTDataCalculated = true;
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
bool CChinaStock::ProcessOneRTData(CRTDataPtr pRTData) {
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
void CChinaStock::InitializeCalculatingRTDataEnvionment(CRTDataPtr pRTData) {
  SetLastRTData(pRTData);
  SetHavingFirstRTData(true);
  // ��һ�ιҵ����޷��ж�����״̬���ʶ�������Ϊ�޷��жϡ����֮ǰ�Ѿ����й�ϵͳ���˴��ǿ�����;��¼�ģ���ϵͳ�洢����ʱ���������ݿ��У�
  // ��ϵͳ����ʱ�Ѿ�������ʱ״̬�����ˣ��ʶ�m_lUnknownVolume��Ϊ�㣬�ʶ���Ҫ���¼���m_lUnknownVolume.
  // m_lUnknownVolume = ��ǰ�ĳɽ��� - ֮ǰ��ʱ�洢�ĳɽ��� + ֮ǰ�洢��m_lUnkonwnVolume.
  SetVolume(pRTData->GetVolume());
  SetUnknownVolume(GetUnknownVolume() + m_pLastRTData->GetVolume() - m_llLastSavedVolume);

  // ���õ�һ�εĹҵ�ӳ�䡣
  for (int j = 0; j < 5; j++) {
    SetGuadan(pRTData->GetPBuy(j), pRTData->GetVBuy(j));
    SetGuadan(pRTData->GetPSell(j), pRTData->GetVSell(j));
  }
}

void CChinaStock::CalculateOneRTData(CRTDataPtr pRTData) {
  long lCurrentGuadanTransactionPrice = 0;

  ResetCalculatingData();
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
  SetLastRTData(pRTData);
}

void CChinaStock::CalculateOneDeal(CRTDataPtr pRTData, INT64 lCurrentGuadanTransactionPrice) {
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
  ASSERT(GetTransactionTime() >= pRTData->GetTransactionTime());
  const INT64 I = pRTData->GetVolume();
  const INT64 j = GetOrdinaryBuyVolume() + GetOrdinarySellVolume()
    + GetAttackBuyVolume() + GetAttackSellVolume()
    + GetStrongBuyVolume() + GetStrongSellVolume() + GetUnknownVolume();
  ASSERT(pRTData->GetVolume() == GetOrdinaryBuyVolume() + GetOrdinarySellVolume()
         + GetAttackBuyVolume() + GetAttackSellVolume()
         + GetStrongBuyVolume() + GetStrongSellVolume() + GetUnknownVolume());
}

void CChinaStock::IncreaseTransactionNumber(void) {
  m_lTransactionNumber++; // �ɽ�����һ��
  if (m_lCurrentGuadanTransactionVolume < 5000) {
    m_lTransactionNumberBelow5000++;
  }
  else if (m_lCurrentGuadanTransactionVolume < 50000) {
    m_lTransactionNumberBelow50000++;
  }
  else if (m_lCurrentGuadanTransactionVolume < 200000) {
    m_lTransactionNumberBelow200000++;
  }
  else {
    m_lTransactionNumberAbove200000++;
  }
  ASSERT(GetTransactionNumber() == (GetTransactionNumberAbove200000()
                                    + GetTransactionNumberBelow200000()
                                    + GetTransactionNumberBelow50000() + GetTransactionNumberBelow5000()));
}

void CChinaStock::CalculateOrdinaryBuySell(INT64 lCurrentGuadanTransactionPrice) {
  if ((m_pLastRTData->GetPSell(0) - lCurrentGuadanTransactionPrice) <= 2) { //һ��������
    m_lOrdinaryBuyVolume += m_lCurrentGuadanTransactionVolume;
    m_nCurrentTransactionType = __ORDINARY_BUY__;
    CalculateOrdinaryBuyVolume();
  }
  else if ((lCurrentGuadanTransactionPrice - m_pLastRTData->GetPBuy(0)) <= 2) { // һ��������
    m_nCurrentTransactionType = __ORDINARY_SELL__;
    m_lOrdinarySellVolume += m_lCurrentGuadanTransactionVolume;
    CalculateOrdinarySellVolume();
  }
  else { // �������ӣ���������
    m_nCurrentTransactionType = __UNKNOWN_BUYSELL__;
    m_lUnknownVolume += m_lCurrentGuadanTransactionVolume;
  }
}

void CChinaStock::CalculateOrdinaryBuyVolume(void) {
  if (m_lCurrentGuadanTransactionVolume < 5000) {
    m_lOrdinaryBuyNumberBelow5000++;
    m_lOrdinaryBuyVolumeBelow5000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 10000) {
    m_lOrdinaryBuyNumberBelow10000++;
    m_lOrdinaryBuyVolumeBelow10000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 20000) {
    m_lOrdinaryBuyNumberBelow20000++;
    m_lOrdinaryBuyVolumeBelow20000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 50000) {
    m_lOrdinaryBuyNumberBelow50000++;
    m_lOrdinaryBuyVolumeBelow50000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 100000) {
    m_lOrdinaryBuyNumberBelow100000++;
    m_lOrdinaryBuyVolumeBelow100000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 200000) {
    m_lOrdinaryBuyNumberBelow200000++;
    m_lOrdinaryBuyVolumeBelow200000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume >= 200000) {
    m_lOrdinaryBuyNumberAbove200000++;
    m_lOrdinaryBuyVolumeAbove200000 += m_lCurrentGuadanTransactionVolume;
  }
}

void CChinaStock::CalculateOrdinarySellVolume(void) {
  if (m_lCurrentGuadanTransactionVolume < 5000) {
    m_lOrdinarySellNumberBelow5000++;
    m_lOrdinarySellVolumeBelow5000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 10000) {
    m_lOrdinarySellNumberBelow10000++;
    m_lOrdinarySellVolumeBelow10000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 20000) {
    m_lOrdinarySellNumberBelow20000++;
    m_lOrdinarySellVolumeBelow20000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 50000) {
    m_lOrdinarySellNumberBelow50000++;
    m_lOrdinarySellVolumeBelow50000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 100000) {
    m_lOrdinarySellNumberBelow100000++;
    m_lOrdinarySellVolumeBelow100000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 200000) {
    m_lOrdinarySellNumberBelow200000++;
    m_lOrdinarySellVolumeBelow200000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume >= 200000) {
    m_lOrdinarySellNumberAbove200000++;
    m_lOrdinarySellVolumeAbove200000 += m_lCurrentGuadanTransactionVolume;
  }
}

void CChinaStock::CalculateAttackBuy(void) {
  m_nCurrentTransactionType = __ATTACK_BUY__;
  m_lAttackBuyVolume += m_lCurrentGuadanTransactionVolume;
  CalculateAttackBuyVolume();
}

void CChinaStock::CalculateStrongBuy(void) {
  m_nCurrentTransactionType = __STRONG_BUY__;
  m_lStrongBuyVolume += m_lCurrentGuadanTransactionVolume;
  CalculateAttackBuyVolume();
}

void CChinaStock::CalculateAttackBuyVolume(void) {
  if (m_lCurrentGuadanTransactionVolume < 50000) {
    m_lAttackBuyBelow50000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 200000) {
    m_lAttackBuyBelow200000 += m_lCurrentGuadanTransactionVolume;
  }
  else {
    m_lAttackBuyAbove200000 += m_lCurrentGuadanTransactionVolume;
  }
}

void CChinaStock::CalculateAttackSell(void) {
  m_nCurrentTransactionType = __ATTACK_SELL__;
  m_lAttackSellVolume += m_lCurrentGuadanTransactionVolume;
  CalculateAttackSellVolume();
}

void CChinaStock::CalculateStrongSell(void) {
  m_nCurrentTransactionType = __STRONG_SELL__;
  m_lStrongSellVolume += m_lCurrentGuadanTransactionVolume;
  CalculateAttackSellVolume();
}

void CChinaStock::CalculateAttackSellVolume(void) {
  if (m_lCurrentGuadanTransactionVolume < 50000) {
    m_lAttackSellBelow50000 += m_lCurrentGuadanTransactionVolume;
  }
  else if (m_lCurrentGuadanTransactionVolume < 200000) {
    m_lAttackSellBelow200000 += m_lCurrentGuadanTransactionVolume;
  }
  else {
    m_lAttackSellAbove200000 += m_lCurrentGuadanTransactionVolume;
  }
}

void CChinaStock::ResetCalculatingData(void) {
  m_lCurrentCanselBuyVolume = 0;
  m_lCurrentCanselSellVolume = 0;
  m_lCurrentGuadanTransactionVolume = 0;
  m_lCurrentStrongBuy = 0;
  m_lCurrentStrongSell = 0;
  m_lCurrentUnknown = 0;
  m_lCurrentAttackBuy = 0;
  m_lCurrentAttackSell = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ҵ����ı仯���ɹ����̵߳���
//
// ����map�ṹ�洢����������������⣬Ψһ�������Ѱַ����ʱ�䳤����Ҫ���Ժ󿴿��ٶ����(���Ժ��ٶ�û�κ����⣩��
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::AnalysisGuadan(CRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice) {
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

void CChinaStock::SelectGuadanThatNeedToCalculate(CRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice, array<bool, 10>& fNeedCheck) {
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

void CChinaStock::SetCurrentGuadan(CRTDataPtr pCurrentRTData) {
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

void CChinaStock::CheckGuadan(CRTDataPtr pCurrentRTData, array<bool, 10>& fNeedCheck) {
  for (int i = 0; i < 5; i++) {
    CheckSellGuadan(fNeedCheck, i);
    CheckBuyGuadan(fNeedCheck, i);
  }
}

void CChinaStock::CheckSellGuadan(array<bool, 10>& fNeedCheck, int i) {
  ASSERT((i < 5) && (i >= 0));
  if (fNeedCheck.at(4 - i)) {
    if (GetGuadan(m_pLastRTData->GetPSell(i)) < m_pLastRTData->GetVSell(i)) { // �����˵Ļ�
      m_lCurrentCanselSellVolume += m_pLastRTData->GetVSell(i) - GetGuadan(m_pLastRTData->GetPSell(i));
      CalculateCancelSellVolume(m_lCurrentCanselSellVolume);
      m_lCancelSellVolume += m_pLastRTData->GetVSell(i) - GetGuadan(m_pLastRTData->GetPSell(i));
    }
  }
}

void CChinaStock::CalculateCancelSellVolume(INT64 lCurrentCancelSellVolume) {
  if (lCurrentCancelSellVolume < 5000) {
    m_lCancelSellVolumeBelow5000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 10000) {
    m_lCancelSellVolumeBelow10000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 20000) {
    m_lCancelSellVolumeBelow20000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 50000) {
    m_lCancelSellVolumeBelow50000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 100000) {
    m_lCancelSellVolumeBelow100000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 200000) {
    m_lCancelSellVolumeBelow200000 += lCurrentCancelSellVolume;
  }
  else {
    m_lCancelSellVolumeAbove200000 += lCurrentCancelSellVolume;
  }
}

void CChinaStock::CheckBuyGuadan(array<bool, 10>& fNeedCheck, int i) {
  ASSERT((i < 5) && (i >= 0));
  if (fNeedCheck.at(5 + i)) {
    if (GetGuadan(m_pLastRTData->GetPBuy(i)) < m_pLastRTData->GetVBuy(i)) { // �����˵Ļ�
      m_lCurrentCanselBuyVolume += m_pLastRTData->GetVBuy(i) - GetGuadan(m_pLastRTData->GetPBuy(i));
      CalculateCancelBuyVolume(m_lCurrentCanselBuyVolume);
      m_lCancelBuyVolume += m_pLastRTData->GetVBuy(i) - GetGuadan(m_pLastRTData->GetPBuy(i));
    }
  }
}

void CChinaStock::CalculateCancelBuyVolume(INT64 lCurrentCancelBuyVolume) {
  if (lCurrentCancelBuyVolume < 5000) {
    m_lCancelBuyVolumeBelow5000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 10000) {
    m_lCancelBuyVolumeBelow10000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 20000) {
    m_lCancelBuyVolumeBelow20000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 50000) {
    m_lCancelBuyVolumeBelow50000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 100000) {
    m_lCancelBuyVolumeBelow100000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 200000) {
    m_lCancelBuyVolumeBelow200000 += lCurrentCancelBuyVolume;
  }
  else {
    m_lCancelBuyVolumeAbove200000 += lCurrentCancelBuyVolume;
  }
}

bool CChinaStock::HaveGuadan(INT64 lPrice) {
  if (m_mapGuadan.find(lPrice) == m_mapGuadan.end()) return false;
  else if (m_mapGuadan.at(lPrice) == 0) return false;
  return true;
}

bool CChinaStock::CheckCurrentRTData() {
  if ((GetOrdinaryBuyVolume() < 0) || (GetOrdinarySellVolume() < 0)
      || (GetAttackBuyVolume() < 0) || (GetAttackSellVolume() < 0)
      || (GetStrongBuyVolume() < 0) || (GetStrongSellVolume() < 0)) {
    int j = 0;
    if (GetOrdinaryBuyVolume() < 0) j = 1;
    if (GetOrdinarySellVolume() < 0) j += 2;
    if (GetAttackBuyVolume() < 0) j += 4;
    if (GetAttackSellVolume() < 0) j += 8;
    if (GetStrongBuyVolume() < 0) j += 16;
    if (GetStrongSellVolume() < 0) j += 32;
    TRACE(_T("%06d %s Error in volume. Error  code = %d\n"), gl_pChinaStockMarket->GetTime(), GetStockCode().GetBuffer(), j);
    return false;
  }
  return true;
}

void CChinaStock::ShowCurrentTransaction() {
  // ��ʾ��ǰ�������

  if (GetCurrentTransationVolume() > 0) {
    ReportGuadanTransaction();
  }
}

void CChinaStock::ShowCurrentInformationofCancelingGuadan(void) {
  // ��ʾ��ǰȡ���ҵ������
  ReportGuadan();
}

void CChinaStock::ReportGuadanTransaction(void) {
  char buffer[100];
  CString str, str2, strTime;
  const CTime ctime(m_pLastRTData->GetTransactionTime());
  sprintf_s(buffer, "%02d:%02d:%02d", ctime.GetHour(), ctime.GetMinute(), ctime.GetSecond());
  strTime = buffer;
  sprintf_s(buffer, _T(" %s %I64d�ɳɽ���%10.3f    "), GetStockCode().GetBuffer(),
            m_lCurrentGuadanTransactionVolume, m_dCurrentGuadanTransactionPrice);
  str = strTime;
  str += buffer;
  CString str1;
  switch (m_nCurrentTransactionType) {
  case __STRONG_BUY__:
  str1 = _T(" STRONG BUY");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetStrongBuyVolume());
  break;
  case __STRONG_SELL__:
  str1 = _T(" STRONG SELL");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetStrongSellVolume());
  break;
  case __ATTACK_BUY__:
  str1 = _T(" ATTACK BUY");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetAttackBuyVolume());
  break;
  case __ATTACK_SELL__:
  str1 = _T(" ATTACK SELL");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetAttackSellVolume());
  break;
  case __ORDINARY_BUY__:
  str1 = _T(" ORDINARY BUY");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetOrdinaryBuyVolume());
  break;
  case __ORDINARY_SELL__:
  str1 = _T(" ORDINARY SELL");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetOrdinarySellVolume());
  break;
  case __UNKNOWN_BUYSELL__:
  str1 = _T(" UNKNOWN BUYSELL");
  sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetUnknownVolume());
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

void CChinaStock::ReportGuadan(void) {
  CString str1;
  char buffer[30];
  if (m_lCurrentCanselSellVolume > 0) {
    sprintf_s(buffer, _T("��ǰȡ����������%I64d"), m_lCurrentCanselSellVolume);
    str1 = buffer;
    sprintf_s(buffer, _T("  ��ȡ����������%I64d"), GetCancelSellVolume());
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1);   // ����ͬ�����ƴ�����Ϣ
  }
  if (m_lCurrentCanselBuyVolume > 0) {
    sprintf_s(buffer, _T("��ǰȡ��������%I64d"), m_lCurrentCanselBuyVolume);
    str1 = buffer;
    sprintf_s(buffer, _T("  ��ȡ��������%I64d"), GetCancelBuyVolume());
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1); // ����ͬ�����ƴ�����Ϣ
  }
}

void CChinaStock::SaveStockCodeDB(CSetStockCode& setStockCode) {
  CString str;
  setStockCode.m_Counter = GetOffset();
  setStockCode.m_StockType = GetMarket();
  setStockCode.m_StockCode = GetStockCode();
  if (GetStockName() != _T("")) {   // ����˹�ƱID�����µ����֣�
    setStockCode.m_StockName = GetStockName(); // ��洢�µ�����
  }
  if (GetIPOStatus() == __STOCK_IPOED__) { // ����˹�Ʊ�ǻ�Ծ��Ʊ
    if (gl_pChinaStockMarket->IsEarlyThen(GetDayLineEndDay(), gl_pChinaStockMarket->GetDay(), 30)) { // ����˹�Ʊ��������ʷ�����Ѿ�����һ�����ˣ������ô˹�Ʊ״̬Ϊ������
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

void CChinaStock::AppendStockCodeDB(CSetStockCode& setStockCode) {
  setStockCode.AddNew();
  SaveStockCodeDB(setStockCode);
  setStockCode.Update();
}

bool CChinaStock::LoadStockCodeDB(CSetStockCode& setStockCode) {
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

void CChinaStock::SetCheckingDayLineStatus(void) {
  ASSERT(IsDayLineNeedUpdate());
  // ���ٸ����������ݱ��ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
  if (gl_pChinaStockMarket->GetLastTradeDay() <= GetDayLineEndDay()) { // ������������Ϊ���ջ�����һ�������յ����ݡ�
    SetDayLineNeedUpdate(false); // �������ݲ���Ҫ����
  }
  else if (GetIPOStatus() == __STOCK_NULL__) { // ��Ч���벻�������������
    SetDayLineNeedUpdate(false);
  }
  else if (GetIPOStatus() == __STOCK_DELISTED__) { // ���й�Ʊ��������ع��������ݣ���ÿ����һ������������
    if ((gl_pChinaStockMarket->GetDayOfWeek() != 1) && (GetDayLineEndDay() != __CHINA_MARKET_BEGIN_DAY__)) {
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
void CChinaStock::PushRTData(CRTDataPtr pData) {
  m_qRTData.push(pData);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ��������ȡʵʱ���ݡ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
CRTDataPtr CChinaStock::PopRTData(void) {
  CRTDataPtr pData = m_qRTData.front();
  m_qRTData.pop();
  return pData;
}

CRTDataPtr CChinaStock::GetRTDataAtHead(void) {
  return m_qRTData.front();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ��������ȡջ�ڵ�ʵʱ����������
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
INT64 CChinaStock::GetRTDataQueueSize(void) {
  return m_qRTData.size();
}

bool CChinaStock::IsSameStock(CChinaStockPtr pStock) {
  ASSERT(pStock != nullptr);
  if (m_lOffsetInContainer == pStock->GetOffset()) return true;
  else return false;
}

////////////////////////////////////////////////////////////////////////////////
//
// �жϹ�Ʊ�����Ƿ���Ч��������߼ۡ���ͼۡ��ɽ����ͳɽ������жϣ������Ϊ�㣬����Ϊ�˹ɽ���û����Ч����
// �����ÿ��̼ۣ����̼��п��ܲ�Ϊ��ʱ���������ݽ�Ϊ�㣨ͣ��ʱ����״̬����
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::IsTodayDataActive(void) {
  if (!m_fActive) return false;
  else {
    return IsTodayDataChanged();
  }
}

bool CChinaStock::IsTodayDataChanged(void) {
  if ((GetHigh() != 0) || (GetLow() != 0) || (GetAmount() != 0) || (GetVolume() != 0)) {
    return true;
  }
  else {
    return false;
  }
}

void CChinaStock::SetDayLineNeedUpdate(bool fFlag) {
  if (fFlag) {
    ASSERT(!m_fDayLineNeedUpdate);
    m_fDayLineNeedUpdate = true;
    gl_pChinaStockMarket->IncreaseNeteaseDayLineNeedUpdateNumber();
  }
  else {
    ASSERT(m_fDayLineNeedUpdate);
    m_fDayLineNeedUpdate = false;
    gl_pChinaStockMarket->DecreaseNeteaseDayLineNeedUpdateNumber();
  }
}

void CChinaStock::SetDayLineNeedProcess(bool fFlag) {
  if (fFlag) {
    ASSERT(!m_fDayLineNeedProcess);
    m_fDayLineNeedProcess = true;
    gl_pChinaStockMarket->IncreaseNeteaseDayLineNeedProcessNumber();
  }
  else {
    ASSERT(m_fDayLineNeedProcess);
    m_fDayLineNeedProcess = false;
    gl_pChinaStockMarket->DecreaseNeteaseDayLineNeedProcessNumber();
  }
}

void CChinaStock::ShowDayLine(CDC* pDC, CRect rectClient) {
  const COLORREF crBlue(RGB(0, 0, 255)), crGreen(RGB(0, 255, 0));
  CPen penGreen1(PS_SOLID, 1, crGreen);
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
  pDC->SelectObject(&penGreen1);
  for (; it != m_vDayLine.begin(); it--) {
    x = rectClient.right - 2 - i * 3;
    y = (0.5 - (double)((*it)->GetHigh() - lLow) / (2 * (lHigh - lLow))) * rectClient.Height();
    pDC->MoveTo(x, y);
    if ((*it)->GetHigh() == (*it)->GetLow()) {
      y = y - 1;
    }
    else {
      y = (0.5 - (double)((*it)->GetLow() - lLow) / (2 * (lHigh - lLow))) * rectClient.Height();
    }
    pDC->LineTo(x, y);
    lDay = (*it)->GetDay();
    i++;
    if (3 * i > m_vDayLine.size()) break;
    if (rectClient.right <= 3 * i) break; // ����������߿�Ϊֹ
  }
}

void CChinaStock::GetRS1Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->GetRelativeStrongIndex();
  }
}

void CChinaStock::GetRSIndex1Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->GetRelativeStrongIndex();
  }
}

void CChinaStock::GetRSLogarithm1Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->GetRelativeStrongLogarithm();
  }
}

void CChinaStock::GetRS3Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->Get3DayRS();
  }
}

void CChinaStock::GetRS5Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->Get5DayRS();
  }
}

void CChinaStock::GetRS10Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->Get10DayRS();
  }
}

void CChinaStock::GetRS30Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->Get30DayRS();
  }
}

void CChinaStock::GetRS60Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->Get60DayRS();
  }
}

void CChinaStock::GetRS120Day(vector<double>& vRS) {
  for (int i = 0; i < m_vDayLine.size(); i++) {
    vRS[i] = m_vDayLine.at(i)->Get120DayRS();
  }
}

void CChinaStock::__TestSetDayLineBuffer(INT64 lBufferLength, char* pDayLineBuffer) {
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

bool CChinaStock::IsVolumeConsistence(void) noexcept {
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
void CChinaStock::AssertValid() const {
  CObject::AssertValid();
}

void CChinaStock::Dump(CDumpContext& dc) const {
  CObject::Dump(dc);
}

#endif //_DEBUG