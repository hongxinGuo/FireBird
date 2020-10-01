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
  if (m_pDLBuffer != nullptr) {
    delete m_pDLBuffer;
    m_pDLBuffer = nullptr;
  }
}

void CChinaStock::Reset(void) {
  m_wMarket = 0;
  m_strStockCode = _T("");
  m_strStockName = _T("");
  m_lOffsetInContainer = -1;
  m_lDLStartDate = __CHINA_MARKET_BEGIN_DATE__; //
  m_lDLEndDate = __CHINA_MARKET_BEGIN_DATE__; //
  m_lIPOStatus = __STOCK_NOT_CHECKED__;   // Ĭ��״̬Ϊ��Ч��Ʊ���롣
  m_nHand = 100;

  m_TransactionTime = 0;
  m_lLastClose = m_lOpen = 0;
  m_lHigh = m_lLow = m_lNew = 0;
  m_lHighLimit = m_lLowLimit = 0;
  m_lHighLimit2 = m_lLowLimit2 = 0;
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
  m_dRealtimeRS = 0;
  m_dRealtimeRSIndex = 0;

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
  m_lCanceledBuyVolume = 0;
  m_lCanceledSellVolume = 0;
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

  m_lCanceledBuyVolumeBelow5000 = 0;
  m_lCanceledBuyVolumeBelow10000 = 0;
  m_lCanceledBuyVolumeBelow20000 = 0;
  m_lCanceledBuyVolumeBelow50000 = 0;
  m_lCanceledBuyVolumeBelow100000 = 0;
  m_lCanceledBuyVolumeBelow200000 = 0;
  m_lCanceledBuyVolumeAbove200000 = 0;
  m_lCanceledSellVolumeBelow5000 = 0;
  m_lCanceledSellVolumeBelow10000 = 0;
  m_lCanceledSellVolumeBelow20000 = 0;
  m_lCanceledSellVolumeBelow50000 = 0;
  m_lCanceledSellVolumeBelow100000 = 0;
  m_lCanceledSellVolumeBelow200000 = 0;
  m_lCanceledSellVolumeAbove200000 = 0;

  m_lOrdinaryBuyVolume = m_lAttackBuyBelow50000 = m_lAttackBuyBelow200000 = m_lAttackBuyAbove200000 = 0;
  m_lOrdinarySellVolume = m_lAttackSellBelow50000 = m_lAttackSellBelow200000 = m_lAttackSellAbove200000 = 0;

  m_llLastSavedVolume = 0;

  m_lCurrentCanceledSellVolume = m_lCurrentCanceledBuyVolume = m_lCurrentGuadanTransactionVolume = 0;

  m_dCurrentGuadanTransactionPrice = 0;
  m_nCurrentTransactionType = 0;

  m_fActive = false;

  m_fDLNeedUpdate = true;
  m_fDLNeedProcess = false; // �������϶�ȡ��������ʷ����
  m_fDLNeedSaving = false;
  m_lDLBufferLength = 0;
  m_pDLBuffer = nullptr;

  m_fChoiced = false;
  m_fSaveToChoicedStockDB = false;

  m_fDLDBUpdated = false;

  m_fHaveFirstRTData = false;  // ʵʱ���ݿ�ʼ�����ʶ����һ��ʵʱ����ֻ��������ʼ��ϵͳ���������ڼ��㡣�ӵڶ������ݿ�ʼ�������Ч��
  m_fNeedProcessRTData = true;
  m_fRTDataCalculated = false;
  m_fRecordRTData = false;
  m_pLastRTData = nullptr;

  ClearRTDataDeque();
}

void CChinaStock::ClearRTDataDeque(void) {
  long lTotalNumber = GetRTDataQueueSize();
  for (int i = 0; i < lTotalNumber; i++) {
    CWebRTDataPtr pRTData = PopRTData();
  }
}

bool CChinaStock::HaveNewDLData(void) {
  if (m_DL.GetDataSize() <= 0) return false;
  if (m_DL.GetData(m_DL.GetDataSize() - 1)->GetFormatedMarketDate() > GetDLEndDate()) return true;
  else return false;
}

void CChinaStock::SetDLNeedSaving(bool fFlag) {
  if (fFlag) {
    ASSERT(!m_fDLNeedSaving);
    m_fDLNeedSaving = true;
    gl_pChinaStockMarket->IncreaseNeteaseDLNeedSaveNumber();
  }
  else {
    ASSERT(m_fDLNeedSaving);
    m_fDLNeedSaving = false;
    gl_pChinaStockMarket->DecreaseNeteaseDLNeedSaveNumber();
  }
}

bool CChinaStock::IsDLNeedSavingAndClearFlag(void) {
  bool fNeedSaveing = m_fDLNeedSaving.exchange(false);
  if (fNeedSaveing) gl_pChinaStockMarket->DecreaseNeteaseDLNeedSaveNumber();
  return fNeedSaveing;
}

bool CChinaStock::TransferNeteaseDLWebDataToBuffer(CNeteaseDLWebInquiry* pNeteaseWebDLData) {
  // ����ȡ���������ݷ�����ع�Ʊ���������ݻ������У���������ر�ʶ��
  char* p = pNeteaseWebDLData->GetBufferAddr();
  if (m_pDLBuffer != nullptr) delete m_pDLBuffer;
  m_pDLBuffer = new char[pNeteaseWebDLData->GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
  char* pbuffer = m_pDLBuffer;
  for (int i = 0; i < pNeteaseWebDLData->GetByteReaded() + 1; i++) {
    *pbuffer++ = *p++;
  }
  m_lDLBufferLength = pNeteaseWebDLData->GetByteReaded();
  SetDLNeedProcess(true);

  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// ������������߷������϶�ȡ�Ĺ�Ʊ�������ݡ�
// ������ʽΪ�� ����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::ProcessNeteaseDLData(void) {
  char* pTestPos = m_pDLBuffer;
  vector<CDLPtr> vTempDL;
  shared_ptr<CDL> pDL;

  ASSERT(m_fDLNeedProcess);
  ASSERT(m_fDLNeedSaving == false);
  SetDLNeedProcess(false); // �����Ƿ���ȷ����������ʹ��
  if (m_lDLBufferLength == 0) { // û�����ݶ��룿����״̬�ǲ�ѯ�Ĺ�ƱΪ��Ч�������ڣ�����
    return false;
  }

  ASSERT(m_pDLBuffer[m_lDLBufferLength] == 0x000); // ����ַ�Ϊ���ӵ�0x000.
  ResetCurrentPos();
  if (!SkipNeteaseDLInformationHeader()) return false;

  pTestPos = m_pDLBuffer + m_llCurrentPos;
  ASSERT(*pTestPos == *m_pCurrentPos);
  if (m_llCurrentPos == m_lDLBufferLength) {// ��Ч��Ʊ���룬����ֻ��ǰ׺˵����û��ʵ����Ϣ�����������ˣ������Ѿ������ˣ��������¹����еĵ�һ��
    if (GetDLEndDate() == __CHINA_MARKET_BEGIN_DATE__) { // �����ʼ���߽������ڴ���û�б��������˹�Ʊ������δ��ʹ�ù�
      SetIPOStatus(__STOCK_NULL__);   // �˹�Ʊ������δʹ�á�
      //TRACE("��Ч��Ʊ����:%s\n", GetStockCode().GetBuffer());
    }
    else { // �Ѿ����еĹ�Ʊ
      if (gl_pChinaStockMarket->IsEarlyThen(GetDLEndDate(), gl_pChinaStockMarket->GetFormatedMarketDate(), 30)) {
        SetIPOStatus(__STOCK_DELISTED__);   // �˹�Ʊ�����Ѿ����С�
      }
      //TRACE("%Sû�пɸ��µ���������\n", GetStockCode().GetBuffer());
    }
    return false;
  }

  CString strTemp;
  pTestPos = m_pDLBuffer + m_llCurrentPos;
  ASSERT(*pTestPos == *m_pCurrentPos);
  while (m_llCurrentPos < m_lDLBufferLength) {
    pDL = make_shared<CDL>();
    if (!pDL->ProcessNeteaseData(GetStockCode(), m_pCurrentPos, m_llCurrentPos)) { // ����һ����������
      TRACE(_T("%s�������ݳ���\n"), GetStockCode().GetBuffer());
      // ������ݴ����������
      vTempDL.clear();
      return false; // ���ݳ�����������
    }
    pTestPos = m_pDLBuffer + m_llCurrentPos;
    ASSERT(*pTestPos == *m_pCurrentPos);
    if (!IsActive()) { // �µĹ�Ʊ���룿
      // �����¹�Ʊ
      SetTodayActive(pDL->GetMarket(), pDL->GetStockCode(), pDL->GetStockName());
      TRACE("�������ߺ��������µĻ�Ծ��Ʊ%s\n", GetStockCode().GetBuffer());
    }
    vTempDL.push_back(pDL); // �ݴ�����ʱvector�У���Ϊ�����������ݵ�ʱ��˳���ǵߵ��ģ����µ�����ǰ��
  }
  ReportDLDownLoaded();
  if (gl_pChinaStockMarket->IsEarlyThen(vTempDL.at(0)->GetFormatedMarketDate(), gl_pChinaStockMarket->GetFormatedMarketDate(), 30)) { // ��ȡ���Ĺ�Ʊ���������������������ϸ��µ���������գ������˻������������һ���µ�ʱ��۲죩��
    SetIPOStatus(__STOCK_DELISTED__); // �����л���ͣ���ס�
  }
  else {
    SetIPOStatus(__STOCK_IPOED__); // �������׹�Ʊ
  }

  m_DL.Unload(); // �����������������ݣ�����еĻ���
  // ������������ʱ��Ϊ�������
  for (int i = vTempDL.size() - 1; i >= 0; i--) {
    pDL = vTempDL.at(i);
    if (pDL->IsActive()) {
      // ��������ٽ��ף�ͣ�ƻ����У��Ĺ�Ʊ����
      m_DL.StoreData(pDL);
    }
  }
  vTempDL.clear();
  if (m_pDLBuffer != nullptr) {
    delete m_pDLBuffer;
    m_pDLBuffer = nullptr;
  }
  SetDLLoaded(true);
  SetDLNeedSaving(true); // ���ô洢���߱�ʶ

  return true;
}

bool CChinaStock::SkipNeteaseDLInformationHeader() {
  ASSERT(m_pCurrentPos == m_pDLBuffer);
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
  SetDLLoaded(false);
  SetMarket(wMarket);
  SetStockCode(strStockCode); // ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
  if (strStockName != _T("")) SetStockName(strStockName);// ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
  gl_pChinaStockMarket->SetTotalActiveStock(gl_pChinaStockMarket->GetTotalActiveStock() + 1);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// ��������������
//
/////////////////////////////////////////////////////////////////////////////////////
void CChinaStock::UpdateDL(vector<CDLPtr>& vTempDL) {
  m_DL.UpdateData(vTempDL);
}

void CChinaStock::ReportDLDownLoaded(void) {
  CString strTemp = GetStockCode();
  strTemp += _T("�����������.");
  gl_systemMessage.PushDLInfoMessage(strTemp);
}

void CChinaStock::SaveTodayBasicInfo(CSetDLBasicInfo* psetDLBasicInfo) {
  ASSERT(psetDLBasicInfo->IsOpen());
  psetDLBasicInfo->m_Date = FormatToDate(m_TransactionTime);
  psetDLBasicInfo->m_Market = m_wMarket;
  psetDLBasicInfo->m_StockCode = m_strStockCode;
  psetDLBasicInfo->m_StockName = m_strStockName;
  psetDLBasicInfo->m_LastClose = ConvertValueToString(m_lLastClose, 1000);
  psetDLBasicInfo->m_Open = ConvertValueToString(m_lOpen, 1000);
  psetDLBasicInfo->m_High = ConvertValueToString(m_lHigh, 1000);
  psetDLBasicInfo->m_Low = ConvertValueToString(m_lLow, 1000);
  psetDLBasicInfo->m_Close = ConvertValueToString(m_lNew, 1000);
  psetDLBasicInfo->m_Volume = ConvertValueToString(m_llVolume);
  psetDLBasicInfo->m_Amount = ConvertValueToString(m_llAmount);
  psetDLBasicInfo->m_UpAndDown = ConvertValueToString(m_lUpDown, 1000);
  psetDLBasicInfo->m_UpDownRate = ConvertValueToString(m_dUpDownRate);
  if (m_llTotalValue != 0) psetDLBasicInfo->m_ChangeHandRate = ConvertValueToString((double)100 * m_llAmount / m_llTotalValue);
  else psetDLBasicInfo->m_ChangeHandRate = ConvertValueToString(0);
  psetDLBasicInfo->m_TotalValue = ConvertValueToString(m_llTotalValue);
  psetDLBasicInfo->m_CurrentValue = ConvertValueToString(m_llCurrentValue);
}

void CChinaStock::SaveTempInfo(CSetDLToday& setDLToday) {
  ASSERT(setDLToday.IsOpen());
  setDLToday.m_Date = FormatToDate(m_TransactionTime);
  setDLToday.m_Market = m_wMarket;
  setDLToday.m_StockCode = m_strStockCode;
  setDLToday.m_StockName = m_strStockName;
  setDLToday.m_LastClose = ConvertValueToString(m_lLastClose, 1000);
  setDLToday.m_Open = ConvertValueToString(m_lOpen, 1000);
  setDLToday.m_High = ConvertValueToString(m_lHigh, 1000);
  setDLToday.m_Low = ConvertValueToString(m_lLow, 1000);
  setDLToday.m_Close = ConvertValueToString(m_lNew, 1000);
  setDLToday.m_Volume = ConvertValueToString(m_llVolume);
  setDLToday.m_Amount = ConvertValueToString(m_llAmount);
  setDLToday.m_UpAndDown = ConvertValueToString(m_lUpDown, 1000);
  setDLToday.m_UpDownRate = ConvertValueToString(m_dUpDownRate);
  setDLToday.m_TotalValue = ConvertValueToString(m_llTotalValue);
  setDLToday.m_CurrentValue = ConvertValueToString(m_llCurrentValue);
  setDLToday.m_TransactionNumber = ConvertValueToString(m_lTransactionNumber);
  setDLToday.m_TransactionNumberBelow5000 = ConvertValueToString(m_lTransactionNumberBelow5000);
  setDLToday.m_TransactionNumberBelow50000 = ConvertValueToString(m_lTransactionNumberBelow50000);
  setDLToday.m_TransactionNumberBelow200000 = ConvertValueToString(m_lTransactionNumberBelow200000);
  setDLToday.m_TransactionNumberAbove200000 = ConvertValueToString(m_lTransactionNumberAbove200000);
  setDLToday.m_CanceledBuyVolume = ConvertValueToString(m_lCanceledBuyVolume);
  setDLToday.m_CanceledSellVolume = ConvertValueToString(m_lCanceledSellVolume);
  setDLToday.m_AttackBuyVolume = ConvertValueToString(m_lAttackBuyVolume);
  setDLToday.m_AttackSellVolume = ConvertValueToString(m_lAttackSellVolume);
  setDLToday.m_StrongBuyVolume = ConvertValueToString(m_lStrongBuyVolume);
  setDLToday.m_StrongSellVolume = ConvertValueToString(m_lStrongSellVolume);
  setDLToday.m_UnknownVolume = ConvertValueToString(m_lUnknownVolume);
  setDLToday.m_OrdinaryBuyVolume = ConvertValueToString(m_lOrdinaryBuyVolume);
  setDLToday.m_OrdinarySellVolume = ConvertValueToString(m_lOrdinarySellVolume);
  setDLToday.m_AttackBuyBelow50000 = ConvertValueToString(m_lAttackBuyBelow50000);
  setDLToday.m_AttackBuyBelow200000 = ConvertValueToString(m_lAttackBuyBelow200000);
  setDLToday.m_AttackBuyAbove200000 = ConvertValueToString(m_lAttackBuyAbove200000);
  setDLToday.m_AttackSellBelow50000 = ConvertValueToString(m_lAttackSellBelow50000);
  setDLToday.m_AttackSellBelow200000 = ConvertValueToString(m_lAttackSellBelow200000);
  setDLToday.m_AttackSellAbove200000 = ConvertValueToString(m_lAttackSellAbove200000);
  setDLToday.m_OrdinaryBuyVolumeBelow5000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow5000);
  setDLToday.m_OrdinaryBuyVolumeBelow10000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow10000);
  setDLToday.m_OrdinaryBuyVolumeBelow20000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow20000);
  setDLToday.m_OrdinaryBuyVolumeBelow50000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow50000);
  setDLToday.m_OrdinaryBuyVolumeBelow100000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow100000);
  setDLToday.m_OrdinaryBuyVolumeBelow200000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow200000);
  setDLToday.m_OrdinaryBuyVolumeAbove200000 = ConvertValueToString(m_lOrdinaryBuyVolumeAbove200000);
  setDLToday.m_OrdinarySellVolumeBelow5000 = ConvertValueToString(m_lOrdinarySellVolumeBelow5000);
  setDLToday.m_OrdinarySellVolumeBelow10000 = ConvertValueToString(m_lOrdinarySellVolumeBelow10000);
  setDLToday.m_OrdinarySellVolumeBelow20000 = ConvertValueToString(m_lOrdinarySellVolumeBelow20000);
  setDLToday.m_OrdinarySellVolumeBelow50000 = ConvertValueToString(m_lOrdinarySellVolumeBelow50000);
  setDLToday.m_OrdinarySellVolumeBelow100000 = ConvertValueToString(m_lOrdinarySellVolumeBelow100000);
  setDLToday.m_OrdinarySellVolumeBelow200000 = ConvertValueToString(m_lOrdinarySellVolumeBelow200000);
  setDLToday.m_OrdinarySellVolumeAbove200000 = ConvertValueToString(m_lOrdinarySellVolumeAbove200000);
  setDLToday.m_OrdinaryBuyNumberBelow5000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow5000);
  setDLToday.m_OrdinaryBuyNumberBelow10000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow10000);
  setDLToday.m_OrdinaryBuyNumberBelow20000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow20000);
  setDLToday.m_OrdinaryBuyNumberBelow50000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow50000);
  setDLToday.m_OrdinaryBuyNumberBelow100000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow100000);
  setDLToday.m_OrdinaryBuyNumberBelow200000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow200000);
  setDLToday.m_OrdinaryBuyNumberAbove200000 = ConvertValueToString(m_lOrdinaryBuyNumberAbove200000);
  setDLToday.m_OrdinarySellNumberBelow5000 = ConvertValueToString(m_lOrdinarySellNumberBelow5000);
  setDLToday.m_OrdinarySellNumberBelow10000 = ConvertValueToString(m_lOrdinarySellNumberBelow10000);
  setDLToday.m_OrdinarySellNumberBelow20000 = ConvertValueToString(m_lOrdinarySellNumberBelow20000);
  setDLToday.m_OrdinarySellNumberBelow50000 = ConvertValueToString(m_lOrdinarySellNumberBelow50000);
  setDLToday.m_OrdinarySellNumberBelow100000 = ConvertValueToString(m_lOrdinarySellNumberBelow100000);
  setDLToday.m_OrdinarySellNumberBelow200000 = ConvertValueToString(m_lOrdinarySellNumberBelow200000);
  setDLToday.m_OrdinarySellNumberAbove200000 = ConvertValueToString(m_lOrdinarySellNumberAbove200000);

  setDLToday.m_CanceledBuyVolumeBelow5000 = ConvertValueToString(m_lCanceledBuyVolumeBelow5000);
  setDLToday.m_CanceledBuyVolumeBelow10000 = ConvertValueToString(m_lCanceledBuyVolumeBelow10000);
  setDLToday.m_CanceledBuyVolumeBelow20000 = ConvertValueToString(m_lCanceledBuyVolumeBelow20000);
  setDLToday.m_CanceledBuyVolumeBelow50000 = ConvertValueToString(m_lCanceledBuyVolumeBelow50000);
  setDLToday.m_CanceledBuyVolumeBelow100000 = ConvertValueToString(m_lCanceledBuyVolumeBelow100000);
  setDLToday.m_CanceledBuyVolumeBelow200000 = ConvertValueToString(m_lCanceledBuyVolumeBelow200000);
  setDLToday.m_CanceledBuyVolumeAbove200000 = ConvertValueToString(m_lCanceledBuyVolumeAbove200000);
  setDLToday.m_CanceledSellVolumeBelow5000 = ConvertValueToString(m_lCanceledSellVolumeBelow5000);
  setDLToday.m_CanceledSellVolumeBelow10000 = ConvertValueToString(m_lCanceledSellVolumeBelow10000);
  setDLToday.m_CanceledSellVolumeBelow20000 = ConvertValueToString(m_lCanceledSellVolumeBelow20000);
  setDLToday.m_CanceledSellVolumeBelow50000 = ConvertValueToString(m_lCanceledSellVolumeBelow50000);
  setDLToday.m_CanceledSellVolumeBelow100000 = ConvertValueToString(m_lCanceledSellVolumeBelow100000);
  setDLToday.m_CanceledSellVolumeBelow200000 = ConvertValueToString(m_lCanceledSellVolumeBelow200000);
  setDLToday.m_CanceledSellVolumeAbove200000 = ConvertValueToString(m_lCanceledSellVolumeAbove200000);
}

void CChinaStock::UpdateStatus(CWebRTDataPtr pRTData) {
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
// �����ھ�������ʷ����ʱ��������ֻ�Ǹ��¡�
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::SaveDLBasicInfo(void) {
  return m_DL.SaveDLBasicInfo(GetStockCode());
}

void CChinaStock::UpdateDLStartEndDate(void) {
  if (m_DL.GetDataSize() == 0) {
    SetDLStartDate(__CHINA_MARKET_BEGIN_DATE__);
    SetDLEndDate(__CHINA_MARKET_BEGIN_DATE__);
  }
  else {
    if (m_DL.GetData(0)->GetFormatedMarketDate() < GetDLStartDate()) {
      SetDLStartDate(m_DL.GetData(0)->GetFormatedMarketDate());
      SetDLDBUpdated(true);
    }
    if (m_DL.GetData(m_DL.GetDataSize() - 1)->GetFormatedMarketDate() > GetDLEndDate()) {
      SetDLEndDate(m_DL.GetData(m_DL.GetDataSize() - 1)->GetFormatedMarketDate());
      SetDLDBUpdated(true);
    }
  }
}

void CChinaStock::SaveTodayExtendInfo(CSetDLExtendInfo* psetDLExtendInfo) {
  ASSERT(psetDLExtendInfo->IsOpen());
  psetDLExtendInfo->m_Date = FormatToDate(m_TransactionTime);
  psetDLExtendInfo->m_Market = m_wMarket;
  psetDLExtendInfo->m_StockCode = m_strStockCode;
  psetDLExtendInfo->m_TransactionNumber = ConvertValueToString(m_lTransactionNumber);
  psetDLExtendInfo->m_TransactionNumberBelow5000 = ConvertValueToString(m_lTransactionNumberBelow5000);
  psetDLExtendInfo->m_TransactionNumberBelow50000 = ConvertValueToString(m_lTransactionNumberBelow50000);
  psetDLExtendInfo->m_TransactionNumberBelow200000 = ConvertValueToString(m_lTransactionNumberBelow200000);
  psetDLExtendInfo->m_TransactionNumberAbove200000 = ConvertValueToString(m_lTransactionNumberAbove200000);

  psetDLExtendInfo->m_CanceledBuyVolume = ConvertValueToString(m_lCanceledBuyVolume);
  psetDLExtendInfo->m_CanceledSellVolume = ConvertValueToString(m_lCanceledSellVolume);
  psetDLExtendInfo->m_AttackBuyVolume = ConvertValueToString(m_lAttackBuyVolume);
  psetDLExtendInfo->m_AttackSellVolume = ConvertValueToString(m_lAttackSellVolume);
  psetDLExtendInfo->m_StrongBuyVolume = ConvertValueToString(m_lStrongBuyVolume);
  psetDLExtendInfo->m_StrongSellVolume = ConvertValueToString(m_lStrongSellVolume);
  psetDLExtendInfo->m_UnknownVolume = ConvertValueToString(m_lUnknownVolume);
  psetDLExtendInfo->m_OrdinaryBuyVolume = ConvertValueToString(m_lOrdinaryBuyVolume);
  psetDLExtendInfo->m_OrdinarySellVolume = ConvertValueToString(m_lOrdinarySellVolume);
  psetDLExtendInfo->m_AttackBuyBelow50000 = ConvertValueToString(m_lAttackBuyBelow50000);
  psetDLExtendInfo->m_AttackBuyBelow200000 = ConvertValueToString(m_lAttackBuyBelow200000);
  psetDLExtendInfo->m_AttackBuyAbove200000 = ConvertValueToString(m_lAttackBuyAbove200000);
  psetDLExtendInfo->m_AttackSellBelow50000 = ConvertValueToString(m_lAttackSellBelow50000);
  psetDLExtendInfo->m_AttackSellBelow200000 = ConvertValueToString(m_lAttackSellBelow200000);
  psetDLExtendInfo->m_AttackSellAbove200000 = ConvertValueToString(m_lAttackSellAbove200000);

  psetDLExtendInfo->m_OrdinaryBuyVolumeBelow5000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow5000);
  psetDLExtendInfo->m_OrdinaryBuyVolumeBelow10000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow10000);
  psetDLExtendInfo->m_OrdinaryBuyVolumeBelow20000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow20000);
  psetDLExtendInfo->m_OrdinaryBuyVolumeBelow50000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow50000);
  psetDLExtendInfo->m_OrdinaryBuyVolumeBelow100000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow100000);
  psetDLExtendInfo->m_OrdinaryBuyVolumeBelow200000 = ConvertValueToString(m_lOrdinaryBuyVolumeBelow200000);
  psetDLExtendInfo->m_OrdinaryBuyVolumeAbove200000 = ConvertValueToString(m_lOrdinaryBuyVolumeAbove200000);
  psetDLExtendInfo->m_OrdinarySellVolumeBelow5000 = ConvertValueToString(m_lOrdinarySellVolumeBelow5000);
  psetDLExtendInfo->m_OrdinarySellVolumeBelow10000 = ConvertValueToString(m_lOrdinarySellVolumeBelow10000);
  psetDLExtendInfo->m_OrdinarySellVolumeBelow20000 = ConvertValueToString(m_lOrdinarySellVolumeBelow20000);
  psetDLExtendInfo->m_OrdinarySellVolumeBelow50000 = ConvertValueToString(m_lOrdinarySellVolumeBelow50000);
  psetDLExtendInfo->m_OrdinarySellVolumeBelow100000 = ConvertValueToString(m_lOrdinarySellVolumeBelow100000);
  psetDLExtendInfo->m_OrdinarySellVolumeBelow200000 = ConvertValueToString(m_lOrdinarySellVolumeBelow200000);
  psetDLExtendInfo->m_OrdinarySellVolumeAbove200000 = ConvertValueToString(m_lOrdinarySellVolumeAbove200000);
  psetDLExtendInfo->m_OrdinaryBuyNumberBelow5000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow5000);
  psetDLExtendInfo->m_OrdinaryBuyNumberBelow10000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow10000);
  psetDLExtendInfo->m_OrdinaryBuyNumberBelow20000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow20000);
  psetDLExtendInfo->m_OrdinaryBuyNumberBelow50000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow50000);
  psetDLExtendInfo->m_OrdinaryBuyNumberBelow100000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow100000);
  psetDLExtendInfo->m_OrdinaryBuyNumberBelow200000 = ConvertValueToString(m_lOrdinaryBuyNumberBelow200000);
  psetDLExtendInfo->m_OrdinaryBuyNumberAbove200000 = ConvertValueToString(m_lOrdinaryBuyNumberAbove200000);
  psetDLExtendInfo->m_OrdinarySellNumberBelow5000 = ConvertValueToString(m_lOrdinarySellNumberBelow5000);
  psetDLExtendInfo->m_OrdinarySellNumberBelow10000 = ConvertValueToString(m_lOrdinarySellNumberBelow10000);
  psetDLExtendInfo->m_OrdinarySellNumberBelow20000 = ConvertValueToString(m_lOrdinarySellNumberBelow20000);
  psetDLExtendInfo->m_OrdinarySellNumberBelow50000 = ConvertValueToString(m_lOrdinarySellNumberBelow50000);
  psetDLExtendInfo->m_OrdinarySellNumberBelow100000 = ConvertValueToString(m_lOrdinarySellNumberBelow100000);
  psetDLExtendInfo->m_OrdinarySellNumberBelow200000 = ConvertValueToString(m_lOrdinarySellNumberBelow200000);
  psetDLExtendInfo->m_OrdinarySellNumberAbove200000 = ConvertValueToString(m_lOrdinarySellNumberAbove200000);

  psetDLExtendInfo->m_CanceledBuyVolumeBelow5000 = ConvertValueToString(m_lCanceledBuyVolumeBelow5000);
  psetDLExtendInfo->m_CanceledBuyVolumeBelow10000 = ConvertValueToString(m_lCanceledBuyVolumeBelow10000);
  psetDLExtendInfo->m_CanceledBuyVolumeBelow20000 = ConvertValueToString(m_lCanceledBuyVolumeBelow20000);
  psetDLExtendInfo->m_CanceledBuyVolumeBelow50000 = ConvertValueToString(m_lCanceledBuyVolumeBelow50000);
  psetDLExtendInfo->m_CanceledBuyVolumeBelow100000 = ConvertValueToString(m_lCanceledBuyVolumeBelow100000);
  psetDLExtendInfo->m_CanceledBuyVolumeBelow200000 = ConvertValueToString(m_lCanceledBuyVolumeBelow200000);
  psetDLExtendInfo->m_CanceledBuyVolumeAbove200000 = ConvertValueToString(m_lCanceledBuyVolumeAbove200000);
  psetDLExtendInfo->m_CanceledSellVolumeBelow5000 = ConvertValueToString(m_lCanceledSellVolumeBelow5000);
  psetDLExtendInfo->m_CanceledSellVolumeBelow10000 = ConvertValueToString(m_lCanceledSellVolumeBelow10000);
  psetDLExtendInfo->m_CanceledSellVolumeBelow20000 = ConvertValueToString(m_lCanceledSellVolumeBelow20000);
  psetDLExtendInfo->m_CanceledSellVolumeBelow50000 = ConvertValueToString(m_lCanceledSellVolumeBelow50000);
  psetDLExtendInfo->m_CanceledSellVolumeBelow100000 = ConvertValueToString(m_lCanceledSellVolumeBelow100000);
  psetDLExtendInfo->m_CanceledSellVolumeBelow200000 = ConvertValueToString(m_lCanceledSellVolumeBelow200000);
  psetDLExtendInfo->m_CanceledSellVolumeAbove200000 = ConvertValueToString(m_lCanceledSellVolumeAbove200000);
}

////////////////////////////////////////////////////////////////////////////
//
// ֻ��һ�����ñ�������������ϵͳ��ʼ��ʱװ��֮ǰ���������Ϣ����������˵Ļ���.
// ��Ҫͬʱ�����ܳɽ����������ݴ�˹��������ڼ���δ������ɽ����� �ܳɽ��������µ�ʵʱ��������ʱ��ͬ�����£��ʶ��޷����ڼ��㣩
//
////////////////////////////////////////////////////////////////////////////
void CChinaStock::LoadTempInfo(CSetDLToday& setDLToday) {
  m_lUnknownVolume = atoll(setDLToday.m_UnknownVolume);

  m_lTransactionNumber = atol(setDLToday.m_TransactionNumber);
  m_lTransactionNumberBelow5000 = atol(setDLToday.m_TransactionNumberBelow5000);
  m_lTransactionNumberBelow50000 = atol(setDLToday.m_TransactionNumberBelow50000);
  m_lTransactionNumberBelow200000 = atol(setDLToday.m_TransactionNumberBelow200000);
  m_lTransactionNumberAbove200000 = atol(setDLToday.m_TransactionNumberAbove200000);
  m_lCanceledBuyVolume = atoll(setDLToday.m_CanceledBuyVolume);
  m_lCanceledSellVolume = atoll(setDLToday.m_CanceledSellVolume);
  m_lAttackBuyVolume = atoll(setDLToday.m_AttackBuyVolume);
  m_lAttackSellVolume = atoll(setDLToday.m_AttackSellVolume);
  m_lStrongBuyVolume = atoll(setDLToday.m_StrongBuyVolume);
  m_lStrongSellVolume = atoll(setDLToday.m_StrongSellVolume);
  m_lOrdinaryBuyVolume = atoll(setDLToday.m_OrdinaryBuyVolume);
  m_lOrdinarySellVolume = atoll(setDLToday.m_OrdinarySellVolume);
  m_lAttackBuyBelow50000 = atoll(setDLToday.m_AttackBuyBelow50000);
  m_lAttackBuyBelow200000 = atoll(setDLToday.m_AttackBuyBelow200000);
  m_lAttackBuyAbove200000 = atoll(setDLToday.m_AttackBuyAbove200000);
  m_lAttackSellBelow50000 = atoll(setDLToday.m_AttackSellBelow50000);
  m_lAttackSellBelow200000 = atoll(setDLToday.m_AttackSellBelow200000);
  m_lAttackSellAbove200000 = atoll(setDLToday.m_AttackSellAbove200000);
  m_llVolume = atoll(setDLToday.m_Volume);
  m_llLastSavedVolume = atoll(setDLToday.m_Volume);

  m_lOrdinaryBuyVolumeBelow5000 = atoll(setDLToday.m_OrdinaryBuyVolumeBelow5000);
  m_lOrdinaryBuyVolumeBelow10000 = atoll(setDLToday.m_OrdinaryBuyVolumeBelow10000);
  m_lOrdinaryBuyVolumeBelow20000 = atoll(setDLToday.m_OrdinaryBuyVolumeBelow20000);
  m_lOrdinaryBuyVolumeBelow50000 = atoll(setDLToday.m_OrdinaryBuyVolumeBelow50000);
  m_lOrdinaryBuyVolumeBelow100000 = atoll(setDLToday.m_OrdinaryBuyVolumeBelow100000);
  m_lOrdinaryBuyVolumeBelow200000 = atoll(setDLToday.m_OrdinaryBuyVolumeBelow200000);
  m_lOrdinaryBuyVolumeAbove200000 = atoll(setDLToday.m_OrdinaryBuyVolumeAbove200000);
  m_lOrdinarySellVolumeBelow5000 = atoll(setDLToday.m_OrdinarySellVolumeBelow5000);
  m_lOrdinarySellVolumeBelow10000 = atoll(setDLToday.m_OrdinarySellVolumeBelow10000);
  m_lOrdinarySellVolumeBelow20000 = atoll(setDLToday.m_OrdinarySellVolumeBelow20000);
  m_lOrdinarySellVolumeBelow50000 = atoll(setDLToday.m_OrdinarySellVolumeBelow50000);
  m_lOrdinarySellVolumeBelow100000 = atoll(setDLToday.m_OrdinarySellVolumeBelow100000);
  m_lOrdinarySellVolumeBelow200000 = atoll(setDLToday.m_OrdinarySellVolumeBelow200000);
  m_lOrdinarySellVolumeAbove200000 = atoll(setDLToday.m_OrdinarySellVolumeAbove200000);
  m_lOrdinaryBuyNumberBelow5000 = atoll(setDLToday.m_OrdinaryBuyNumberBelow5000);
  m_lOrdinaryBuyNumberBelow10000 = atoll(setDLToday.m_OrdinaryBuyNumberBelow10000);
  m_lOrdinaryBuyNumberBelow20000 = atoll(setDLToday.m_OrdinaryBuyNumberBelow20000);
  m_lOrdinaryBuyNumberBelow50000 = atoll(setDLToday.m_OrdinaryBuyNumberBelow50000);
  m_lOrdinaryBuyNumberBelow100000 = atoll(setDLToday.m_OrdinaryBuyNumberBelow100000);
  m_lOrdinaryBuyNumberBelow200000 = atoll(setDLToday.m_OrdinaryBuyNumberBelow200000);
  m_lOrdinaryBuyNumberAbove200000 = atoll(setDLToday.m_OrdinaryBuyNumberAbove200000);
  m_lOrdinarySellNumberBelow5000 = atoll(setDLToday.m_OrdinarySellNumberBelow5000);
  m_lOrdinarySellNumberBelow10000 = atoll(setDLToday.m_OrdinarySellNumberBelow10000);
  m_lOrdinarySellNumberBelow20000 = atoll(setDLToday.m_OrdinarySellNumberBelow20000);
  m_lOrdinarySellNumberBelow50000 = atoll(setDLToday.m_OrdinarySellNumberBelow50000);
  m_lOrdinarySellNumberBelow100000 = atoll(setDLToday.m_OrdinarySellNumberBelow100000);
  m_lOrdinarySellNumberBelow200000 = atoll(setDLToday.m_OrdinarySellNumberBelow200000);
  m_lOrdinarySellNumberAbove200000 = atoll(setDLToday.m_OrdinarySellNumberAbove200000);

  m_lCanceledBuyVolumeBelow5000 = atoll(setDLToday.m_CanceledBuyVolumeBelow5000);
  m_lCanceledBuyVolumeBelow10000 = atoll(setDLToday.m_CanceledBuyVolumeBelow10000);
  m_lCanceledBuyVolumeBelow20000 = atoll(setDLToday.m_CanceledBuyVolumeBelow20000);
  m_lCanceledBuyVolumeBelow50000 = atoll(setDLToday.m_CanceledBuyVolumeBelow50000);
  m_lCanceledBuyVolumeBelow100000 = atoll(setDLToday.m_CanceledBuyVolumeBelow100000);
  m_lCanceledBuyVolumeBelow200000 = atoll(setDLToday.m_CanceledBuyVolumeBelow200000);
  m_lCanceledBuyVolumeAbove200000 = atoll(setDLToday.m_CanceledBuyVolumeAbove200000);
  m_lCanceledSellVolumeBelow5000 = atoll(setDLToday.m_CanceledSellVolumeBelow5000);
  m_lCanceledSellVolumeBelow10000 = atoll(setDLToday.m_CanceledSellVolumeBelow10000);
  m_lCanceledSellVolumeBelow20000 = atoll(setDLToday.m_CanceledSellVolumeBelow20000);
  m_lCanceledSellVolumeBelow50000 = atoll(setDLToday.m_CanceledSellVolumeBelow50000);
  m_lCanceledSellVolumeBelow100000 = atoll(setDLToday.m_CanceledSellVolumeBelow100000);
  m_lCanceledSellVolumeBelow200000 = atoll(setDLToday.m_CanceledSellVolumeBelow200000);
  m_lCanceledSellVolumeAbove200000 = atoll(setDLToday.m_CanceledSellVolumeAbove200000);
}

bool CChinaStock::LoadDL(CString strStockCode) {
  return m_DL.LoadData(strStockCode);
}

bool CChinaStock::LoadDLBasicInfo(CSetDLBasicInfo* psetDLBasicInfo) {
  return m_DL.LoadDLBasicInfo(psetDLBasicInfo);
}

/////////////////////////////////////////////////////////////////////////////
//
// װ��DLInfo�������װ��DL��֮��
//
//
////////////////////////////////////////////////////////////////////////////
bool CChinaStock::LoadDLExtendInfo(CSetDLExtendInfo* psetDLExtendInfo) {
  return m_DL.LoadDLExtendInfo(psetDLExtendInfo);
}

bool CChinaStock::CalculateDLRS(void) {
  return m_DL.CalculateRS0();
}

bool CChinaStock::CalculateDLRSLogarithm(void) {
  return m_DL.CalculateRSLogarithm0();
}

bool CChinaStock::CalculateDLRSIndex(void) {
  return m_DL.CalculateRSIndex0();
}

bool CChinaStock::CalculateWeekLineRS(void) {
  return m_WeekLine.CalculateRS0();
}

bool CChinaStock::CalculateWeekLineRSLogarithm(void) {
  return m_WeekLine.CalculateRSLogarithm0();
}

bool CChinaStock::CalculateWeekLineRSIndex(void) {
  return m_WeekLine.CalculateRSIndex0();
}

bool CChinaStock::Calculate10RSStrong2StockSet(void) {
  CSetDLBasicInfo setDLBasicInfo;
  vector<double> m_vRS10Day;
  int iCountFirst = 0, iCountSecond = 0;

  ASSERT(m_DL.IsDataLoaded());
  size_t iDLSize = GetDLSize();
  if (iDLSize > 155) {
    m_vRS10Day.resize(iDLSize);
    CalculateDLRSIndex();
    GetRS10Day(m_vRS10Day);
    int i = 0;
    for (i = iDLSize - 1; i > iDLSize - 5; i--) {
      if (m_vRS10Day.at(i) > 55) iCountFirst++;
      if (iCountFirst >= 3) break;
    }
    if (iCountFirst >= 3) {
      while (m_vRS10Day.at(i) > 53) i--;
      for (int j = i; j > i - 50; j--) {
        if (m_vRS10Day.at(j) > 55) iCountSecond++;
      }
      if (iCountSecond >= 3) {
        return true;
      }
    }
  }

  return false;
}

bool CChinaStock::Calculate10RSStrong1StockSet(void) {
  CSetDLBasicInfo setDLBasicInfo;
  vector<double> m_vRS10Day;
  int iCountFirst = 0, iCountSecond = 0, iCountThird = 0;

  ASSERT(m_DL.IsDataLoaded());
  size_t iDLSize = GetDLSize();

  if (iDLSize < 350) return false;
  m_vRS10Day.resize(iDLSize);
  CalculateDLRSIndex();
  GetRS10Day(m_vRS10Day);
  int i = 0, j = 0;

  for (i = iDLSize - 1; i > iDLSize - 40; i--) {
    if (m_vRS10Day.at(i) > 55) iCountFirst++;
    if (iCountFirst >= 3) break;
  }
  if (iCountFirst < 3) return false;
  while (m_vRS10Day.at(i) > 53) i--;
  for (j = i; j > i - 80; j--) {
    if (m_vRS10Day.at(j) < 55) iCountSecond++;
    if (iCountSecond >= 78) break;
  }
  if (iCountSecond < 78) return false;
  while (m_vRS10Day.at(j) > 53) j--;
  for (int k = j; k > j - 130; k--) {
    if (m_vRS10Day.at(k) > 47) iCountThird++;
    if (iCountThird > 128) break;
  }
  if (iCountThird < 128) return false;

  return true;
}

bool CChinaStock::Calculate10RSStrongStockSet(CRSReference* pRef) {
  CSetDLBasicInfo setDLBasicInfo;
  vector<double> m_vRS10Day;
  int iCountFirst = 0, iCountSecond = 0, iCountThird = 0, iCountFourth = 0;
  bool fFind1 = false, fFind2 = false, fFind3 = false, fFind4 = false;
  double dStrong1, dStrong2, dStrong3, dStrong4;
  bool fFindHigh1, fFindHigh2, fFindHigh3, fFindHigh4;

  if (pRef->m_dRSStrong[0] < 0) {
    dStrong1 = -pRef->m_dRSStrong[0];
    fFindHigh1 = false;
  }
  else {
    dStrong1 = pRef->m_dRSStrong[0];
    fFindHigh1 = true;
  }
  if (pRef->m_dRSStrong[1] < 0) {
    dStrong2 = -pRef->m_dRSStrong[1];
    fFindHigh2 = false;
  }
  else {
    dStrong2 = pRef->m_dRSStrong[1];
    fFindHigh2 = true;
  }
  if (pRef->m_dRSStrong[2] < 0) {
    dStrong3 = -pRef->m_dRSStrong[2];
    fFindHigh3 = false;
  }
  else {
    dStrong3 = pRef->m_dRSStrong[2];
    fFindHigh3 = true;
  }
  if (pRef->m_dRSStrong[3] < 0) {
    dStrong4 = -pRef->m_dRSStrong[3];
    fFindHigh4 = false;
  }
  else {
    dStrong4 = pRef->m_dRSStrong[3];
    fFindHigh4 = true;
  }

  ASSERT(m_DL.IsDataLoaded());
  size_t iDLSize = GetDLSize();
  if ((iDLSize < (pRef->m_lDayLength[0] + pRef->m_lDayLength[1] + pRef->m_lDayLength[2] + 10))
      || (iDLSize < pRef->m_lDayLength[3] + 10)) return false;

  m_vRS10Day.resize(iDLSize);
  CalculateDLRSIndex();
  GetRS10Day(m_vRS10Day);
  int i = 0, j = 0;

  if (pRef->m_lDayLength[0] == 0) fFind1 = true;
  else if (fFindHigh1) {
    for (i = iDLSize - 1; i > (iDLSize - pRef->m_lDayLength[0]); i--) {
      if (m_vRS10Day.at(i) > dStrong1) iCountFirst++;
      if (iCountFirst >= pRef->m_lStrongDayLength[0]) {
        fFind1 = true;
        break;
      }
    }
  }
  else {
    for (i = iDLSize - 1; i > (iDLSize - pRef->m_lDayLength[0]); i--) {
      if (m_vRS10Day.at(i) < dStrong1) iCountFirst++;
      if (iCountFirst >= pRef->m_lStrongDayLength[0]) {
        fFind1 = true;
        break;
      }
    }
  }
  if (!fFind1) return false;
  while (m_vRS10Day.at(i) > (dStrong1 - 2)) i--;

  if (pRef->m_lDayLength[1] == 0) fFind2 = true;
  else if (fFindHigh2) {
    for (j = i; j > (i - pRef->m_lDayLength[1]); j--) {
      if (m_vRS10Day.at(j) > dStrong2) iCountSecond++;
      if (iCountSecond >= pRef->m_lStrongDayLength[1]) {
        fFind2 = true;
        break;
      }
    }
  }
  else {
    for (j = i; j > (i - pRef->m_lDayLength[1]); j--) {
      if (m_vRS10Day.at(j) < dStrong2) iCountSecond++;
      if (iCountSecond >= pRef->m_lStrongDayLength[1]) {
        fFind2 = true;
        break;
      }
    }
  }
  if (!fFind2) return false;
  while (m_vRS10Day.at(j) > (dStrong2 - 2)) j--;

  if (pRef->m_lDayLength[2] == 0) fFind3 = true;
  else if (fFindHigh3) {
    for (int k = j; k > (j - pRef->m_lDayLength[2]); k--) {
      if (m_vRS10Day.at(k) > dStrong3) iCountThird++;
      if (iCountThird >= pRef->m_lStrongDayLength[2]) {
        fFind3 = true;
        break;
      }
    }
  }
  else {
    for (int k = j; k > (j - pRef->m_lDayLength[2]); k--) {
      if (m_vRS10Day.at(k) < dStrong3) iCountThird++;
      if (iCountThird >= pRef->m_lStrongDayLength[2]) {
        fFind3 = true;
        break;
      }
    }
  }
  if (!fFind3) return false;

  if (pRef->m_lDayLength[3] == 0) return true; // DayLength4Ϊ��Ļ�������ͨ��ѡ��
  else if (fFindHigh4) {
    for (i = iDLSize - 1; i > (iDLSize - pRef->m_lDayLength[3]); i--) {
      if (m_vRS10Day.at(i) > dStrong4) iCountFourth++;
      if (iCountFourth >= pRef->m_lStrongDayLength[3]) {
        fFind4 = true;
        break;
      }
    }
  }
  else {
    for (i = iDLSize - 1; i > (iDLSize - pRef->m_lDayLength[3]); i--) {
      if (m_vRS10Day.at(i) < dStrong4) iCountFourth++;
      if (iCountFourth >= pRef->m_lStrongDayLength[3]) {
        fFind4 = true;
        break;
      }
    }
  }
  if (!fFind4) return false;

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
  CWebRTDataPtr pRTData;

  INT64 lTotalNumber = GetRTDataQueueSize(); //  ������еĳ��ȡ�����ͬ�����ƻ�ȡ����ֵ.
  if (lTotalNumber == 0) return false;
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
bool CChinaStock::ProcessOneRTData(CWebRTDataPtr pRTData) {
  CalculateHighLowLimit(pRTData);
  if (HaveFirstRTData()) { // �����ʼ���㣨�ڶ����յ�ʵʱ���ݼ��Ժ�
    CalculateOneRTData(pRTData);
  }
  else { // ��һ���յ�ʵʱ���ݣ���ֻ��ʼ��ϵͳ��������
    InitializeCalculatingRTDataEnvionment(pRTData);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// �й����е��ǵ�ͣ��۸���㷽��Ŀǰ��֪��ֻ�ܿ�������

void CChinaStock::CalculateHighLowLimit(CWebRTDataPtr pRTData) {
  double d1, d2;
  int iAdjust = 0;
  int iCompare = 10;
  int i2 = 0;

  if (pRTData->GetPSell(0) == 0) { // ��һ�۸�Ϊ��
    if (pRTData->GetPBuy(0) > 0) {
      m_lHighLimit2 = pRTData->GetPBuy(0);
      if (pRTData->GetLastClose() < 1200) { // �ͼ۹ɣ�
        m_lLowLimit2 = pRTData->GetLastClose() - (m_lHighLimit2 - pRTData->GetLastClose());
      }
      else {
        i2 = pRTData->GetPBuy(0) - pRTData->GetLastClose();
        iCompare = ((double)i2 * 100 + pRTData->GetLastClose() * 0.65) / pRTData->GetLastClose(); // ϵ��0.70��ʵ������ģ�Ŀǰ��ͨ�á�
        if (iCompare <= 21) {
          if ((iCompare % 5) != 0) { // ȷ���ǵ���Ϊ5%�ı���
            TRACE("%s iCompare = %i, ����5�ı���\n", m_strStockCode.GetBuffer(), iCompare);
          }
          d1 = (double)i2 * 100 / pRTData->GetLastClose();
          if (d1 > iCompare) {
            d2 = (double)(i2 - 10) * 100 / pRTData->GetLastClose();
            if ((iCompare - d2) <= (d1 - iCompare)) { // �������ͣ��ʱ���������һ��ʱ��9.7777��10.22222�������ý�С�ģ�9.7777����
              iAdjust = 10;
            }
          }
          m_lLowLimit2 = pRTData->GetLastClose() - i2 + iAdjust;
        }
        else {
          m_lLowLimit2 = m_lLowLimit;
        }
      }
      if (m_lHighLimit == 0) m_lHighLimit = m_lHighLimit2;
      if (m_lLowLimit == 0) m_lLowLimit = m_lLowLimit2;
    }
    else { // ��һ��һͬʱΪ��
      m_lHighLimit2 = m_lHighLimit;
      m_lLowLimit2 = m_lLowLimit;
    }
  }
  else if (pRTData->GetPBuy(0) == 0) { // ��һ�۸�Ϊ��
    if (pRTData->GetPSell(0) > 0) {
      m_lLowLimit2 = pRTData->GetPSell(0);
      if (pRTData->GetLastClose() < 1200) { // �ͼ۹ɣ�
        m_lHighLimit2 = pRTData->GetLastClose() + (pRTData->GetLastClose() - m_lLowLimit2);
      }
      else {
        i2 = pRTData->GetLastClose() - pRTData->GetPSell(0);
        iCompare = ((double)i2 * 100 + pRTData->GetLastClose() * 0.65) / pRTData->GetLastClose(); // ϵ��0.70��ʵ������ģ�Ŀǰ��ͨ�á�
        if (iCompare <= 21) {
          if ((iCompare % 5) != 0) { // ȷ���ǵ���Ϊ5%�ı���
            TRACE("%s iCompare = %i, ����5�ı���\n", m_strStockCode.GetBuffer(), iCompare);
          }
          d1 = (double)i2 * 100 / pRTData->GetLastClose();
          if (d1 < iCompare) {
            d2 = (double)(i2 + 10) * 100 / pRTData->GetLastClose();
            if ((d2 - iCompare) <= (iCompare - d1)) { // ��������ͣ��ʱ���������һ��ʱ��9.7777��10.22222�������ýϴ�ģ�10.2222����
              iAdjust = 10;
            }
          }
          m_lHighLimit2 = pRTData->GetLastClose() + i2 + iAdjust;
        }
        else {
          m_lHighLimit2 = m_lHighLimit;
        }
      }
      if (m_lHighLimit == 0) m_lHighLimit = m_lHighLimit2;
      if (m_lLowLimit == 0) m_lLowLimit = m_lLowLimit2;
    }
    else { // ��һ��һͬʱΪ��
      m_lHighLimit2 = m_lHighLimit;
      m_lLowLimit2 = m_lLowLimit;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////
//
// ��һ���յ�ʵʱ����ʱ��ֻ��ʼ��ϵͳ�������㣨��Ϊû�г�ʼ���ݣ�
//
////////////////////////////////////////////////////////////////////////////////////////
void CChinaStock::InitializeCalculatingRTDataEnvionment(CWebRTDataPtr pRTData) {
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

void CChinaStock::CalculateOneRTData(CWebRTDataPtr pRTData) {
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
    if ((lCurrentGuadanTransactionPrice == m_lHighLimit) || (lCurrentGuadanTransactionPrice == m_lLowLimit)) { // �ǵ�ͣ��ʱ���ɽ�������δ֪�ɽ�����
      IncreaseTransactionNumber();
      m_nCurrentTransactionType = __UNKNOWN_BUYSELL__;
      m_lUnknownVolume += m_lCurrentGuadanTransactionVolume;
    }
    else {
      CalculateOneDeal(pRTData, lCurrentGuadanTransactionPrice);
    }
  }

  // ���濪ʼ�����ҵ����
  AnalysisGuadan(pRTData, lCurrentGuadanTransactionPrice);

  // ����ǰ����״̬
  SetLastRTData(pRTData);
}

void CChinaStock::CalculateOneDeal(CWebRTDataPtr pRTData, INT64 lCurrentGuadanTransactionPrice) {
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
  m_lCurrentCanceledBuyVolume = 0;
  m_lCurrentCanceledSellVolume = 0;
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
bool CChinaStock::AnalysisGuadan(CWebRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice) {
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

void CChinaStock::SelectGuadanThatNeedToCalculate(CWebRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice, array<bool, 10>& fNeedCheck) {
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

void CChinaStock::SetCurrentGuadan(CWebRTDataPtr pCurrentRTData) {
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

void CChinaStock::CheckGuadan(CWebRTDataPtr pCurrentRTData, array<bool, 10>& fNeedCheck) {
  for (int i = 0; i < 5; i++) {
    CheckSellGuadan(fNeedCheck, i);
    CheckBuyGuadan(fNeedCheck, i);
  }
}

void CChinaStock::CheckSellGuadan(array<bool, 10>& fNeedCheck, int i) {
  ASSERT((i < 5) && (i >= 0));
  INT64 iTemp = 0;
  if (fNeedCheck.at(4 - i)) {
    if (GetGuadan(m_pLastRTData->GetPSell(i)) < m_pLastRTData->GetVSell(i)) { // �����˵Ļ�
      iTemp = m_pLastRTData->GetVSell(i) - GetGuadan(m_pLastRTData->GetPSell(i));
      m_lCurrentCanceledSellVolume += iTemp;
      CalculateCanceledSellVolume(iTemp);
      m_lCanceledSellVolume += iTemp;
    }
  }
  ASSERT(m_lCanceledSellVolume == (m_lCanceledSellVolumeAbove200000 + m_lCanceledSellVolumeBelow10000 + m_lCanceledSellVolumeBelow100000
                                   + m_lCanceledSellVolumeBelow20000 + m_lCanceledSellVolumeBelow200000 + m_lCanceledSellVolumeBelow5000
                                   + m_lCanceledSellVolumeBelow50000));
}

void CChinaStock::CalculateCanceledSellVolume(INT64 lCurrentCancelSellVolume) {
  if (lCurrentCancelSellVolume < 5000) {
    m_lCanceledSellVolumeBelow5000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 10000) {
    m_lCanceledSellVolumeBelow10000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 20000) {
    m_lCanceledSellVolumeBelow20000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 50000) {
    m_lCanceledSellVolumeBelow50000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 100000) {
    m_lCanceledSellVolumeBelow100000 += lCurrentCancelSellVolume;
  }
  else if (lCurrentCancelSellVolume < 200000) {
    m_lCanceledSellVolumeBelow200000 += lCurrentCancelSellVolume;
  }
  else {
    m_lCanceledSellVolumeAbove200000 += lCurrentCancelSellVolume;
  }
}

void CChinaStock::CheckBuyGuadan(array<bool, 10>& fNeedCheck, int i) {
  ASSERT((i < 5) && (i >= 0));
  INT64 iTemp = 0;
  if (fNeedCheck.at(5 + i)) {
    if (GetGuadan(m_pLastRTData->GetPBuy(i)) < m_pLastRTData->GetVBuy(i)) { // �����˵Ļ�
      iTemp = m_pLastRTData->GetVBuy(i) - GetGuadan(m_pLastRTData->GetPBuy(i));
      m_lCurrentCanceledBuyVolume += iTemp;
      CalculateCanceledBuyVolume(iTemp);
      m_lCanceledBuyVolume += iTemp;
    }
  }
  ASSERT(m_lCanceledBuyVolume == (m_lCanceledBuyVolumeAbove200000 + m_lCanceledBuyVolumeBelow10000 + m_lCanceledBuyVolumeBelow100000
                                  + m_lCanceledBuyVolumeBelow20000 + m_lCanceledBuyVolumeBelow200000 + m_lCanceledBuyVolumeBelow5000
                                  + m_lCanceledBuyVolumeBelow50000));
}

void CChinaStock::CalculateCanceledBuyVolume(INT64 lCurrentCancelBuyVolume) {
  if (lCurrentCancelBuyVolume < 5000) {
    m_lCanceledBuyVolumeBelow5000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 10000) {
    m_lCanceledBuyVolumeBelow10000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 20000) {
    m_lCanceledBuyVolumeBelow20000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 50000) {
    m_lCanceledBuyVolumeBelow50000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 100000) {
    m_lCanceledBuyVolumeBelow100000 += lCurrentCancelBuyVolume;
  }
  else if (lCurrentCancelBuyVolume < 200000) {
    m_lCanceledBuyVolumeBelow200000 += lCurrentCancelBuyVolume;
  }
  else {
    m_lCanceledBuyVolumeAbove200000 += lCurrentCancelBuyVolume;
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
    TRACE(_T("%06d %s Error in volume. Error  code = %d\n"), gl_pChinaStockMarket->GetFormatedMarketTime(), GetStockCode().GetBuffer(), j);
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

void CChinaStock::ShowCurrentInformationOfCancelingGuadan(void) {
  // ��ʾ��ǰȡ���ҵ������
  ReportGuadan();
}

void CChinaStock::ReportGuadanTransaction(void) {
  char buffer[100];
  CString str, str2, strTime;
  const CTime ctime(m_pLastRTData->GetTransactionTime());
  sprintf_s(buffer, _T("%02d:%02d:%02d"), ctime.GetHour(), ctime.GetMinute(), ctime.GetSecond());
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
  if (m_lCurrentCanceledSellVolume > 0) {
    sprintf_s(buffer, _T("��ǰȡ����������%I64d"), m_lCurrentCanceledSellVolume);
    str1 = buffer;
    sprintf_s(buffer, _T("  ��ȡ����������%I64d"), GetCanceledSellVolume());
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1);   // ����ͬ�����ƴ�����Ϣ
  }
  if (m_lCurrentCanceledBuyVolume > 0) {
    sprintf_s(buffer, _T("��ǰȡ��������%I64d"), m_lCurrentCanceledBuyVolume);
    str1 = buffer;
    sprintf_s(buffer, _T("  ��ȡ��������%I64d"), GetCanceledBuyVolume());
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
  if (IsIPOed()) { // ����˹�Ʊ�ǻ�Ծ��Ʊ
    if (gl_pChinaStockMarket->IsEarlyThen(GetDLEndDate(), gl_pChinaStockMarket->GetFormatedMarketDate(), 30)) { // ����˹�Ʊ��������ʷ�����Ѿ�����һ�����ˣ������ô˹�Ʊ״̬Ϊ������
      setStockCode.m_IPOStatus = __STOCK_DELISTED__;
    }
    else {
      setStockCode.m_IPOStatus = GetIPOStatus();
    }
  }
  else {
    setStockCode.m_IPOStatus = GetIPOStatus();
  }
  setStockCode.m_DLStartDate = GetDLStartDate();
  setStockCode.m_DLEndDate = GetDLEndDate();
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
  SetIPOStatus(setStockCode.m_IPOStatus);
  SetDLStartDate(setStockCode.m_DLStartDate);
  if (GetDLEndDate() < setStockCode.m_DLEndDate) { // ��ʱһ����Ʊ���ж����¼������������Ϊ׼��
    SetDLEndDate(setStockCode.m_DLEndDate);
  }
  SetCheckingDLStatus();
  return true;
}

void CChinaStock::SetCheckingDLStatus(void) {
  ASSERT(IsDLNeedUpdate());
  // ���ٸ����������ݱ��ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
  if (gl_pChinaStockMarket->GetLastTradeDate() <= GetDLEndDate()) { // ������������Ϊ���ջ�����һ�������յ����ݡ�
    SetDLNeedUpdate(false); // �������ݲ���Ҫ����
  }
  else if (IsNullStock()) { // ��Ч���벻�������������
    SetDLNeedUpdate(false);
  }
  else if (IsDelisted()) { // ���й�Ʊ��������ع��������ݣ���ÿ����һ������������
    if ((gl_pChinaStockMarket->GetDayOfWeek() != 1) && (GetDLEndDate() != __CHINA_MARKET_BEGIN_DATE__)) {
      SetDLNeedUpdate(false);
    }
  }
}

bool CChinaStock::BuildWeekLine(long lStartDate) {
  if (IsNullStock()) return true;
  if (!IsDLLoaded()) {
    LoadDL(GetStockCode());
  }
  if (GetDLSize() <= 0) return true;

  if (CalculatingWeekLine(lStartDate)) {
    SaveWeekLine();
  }

  if (gl_pChinaStockMarket->GetCurrentStock() != nullptr) {
    if (gl_pChinaStockMarket->GetCurrentStock()->GetOffset() != m_lOffsetInContainer) {
      UnloadDL();
      UnloadWeekLine();
    }
  }
  else {
    UnloadDL();
    UnloadWeekLine();
  }
  return true;
}

bool CChinaStock::SaveWeekLine() {
  return m_WeekLine.SaveData(GetStockCode());
}

bool CChinaStock::SaveWeekLineBasicInfo() {
  m_WeekLine.SaveBasicInfo();

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ֻ�洢�н��׼�¼����չ���ݡ�����û����Ϣ��ֱ�ӿ����
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::SaveWeekLineExtendInfo() {
  m_WeekLine.SaveExtendInfo();

  return true;
}

bool CChinaStock::LoadWeekLine() {
  return m_WeekLine.LoadData(GetStockCode());
}

bool CChinaStock::LoadWeekLineBasicInfo(CSetWeekLineBasicInfo* psetWeekLineBasicInfo) {
  return m_WeekLine.LoadBasicInfo(psetWeekLineBasicInfo);
}

bool CChinaStock::LoadWeekLineExtendInfo(CSetWeekLineExtendInfo* psetWeekLineExtendInfo) {
  return m_WeekLine.LoadExtendInfo(psetWeekLineExtendInfo);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ�����ƴ洢ʵʱ���ݡ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CChinaStock::PushRTData(CWebRTDataPtr pData) {
  m_qRTData.PushData(pData);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ��������ȡʵʱ���ݡ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
CWebRTDataPtr CChinaStock::PopRTData(void) {
  return m_qRTData.PopData();
}

CWebRTDataPtr CChinaStock::GetRTDataAtHead(void) {
  return m_qRTData.GetHead();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ��������ȡջ�ڵ�ʵʱ����������
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
INT64 CChinaStock::GetRTDataQueueSize(void) {
  return m_qRTData.GetDataSize();
}

bool CChinaStock::IsSameStock(CChinaStockPtr pStock) {
  if (pStock == nullptr) return false;
  else if (m_lOffsetInContainer == pStock->GetOffset()) return true;
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

////////////////////////////////////////////////////////////////////////////////
//
// �жϹ�Ʊ�����Ƿ���Ч��������߼ۡ���ͼۡ��ɽ����ͳɽ������жϣ������Ϊ�㣬����Ϊ�˹ɽ���û����Ч����
// �����ÿ��̼ۣ����̼��п��ܲ�Ϊ��ʱ���������ݽ�Ϊ�㣨ͣ��ʱ����״̬����
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::IsTodayDataChanged(void) {
  if ((GetHigh() != 0) || (GetLow() != 0) || (GetAmount() != 0) || (GetVolume() != 0)) {
    return true;
  }
  else {
    return false;
  }
}

void CChinaStock::SetDLNeedUpdate(bool fFlag) {
  if (fFlag) {
    ASSERT(!m_fDLNeedUpdate);
    m_fDLNeedUpdate = true;
    gl_pChinaStockMarket->IncreaseNeteaseDLNeedUpdateNumber();
  }
  else {
    ASSERT(m_fDLNeedUpdate);
    m_fDLNeedUpdate = false;
    gl_pChinaStockMarket->DecreaseNeteaseDLNeedUpdateNumber();
  }
}

void CChinaStock::SetDLNeedProcess(bool fFlag) {
  if (fFlag) {
    ASSERT(!m_fDLNeedProcess);
    m_fDLNeedProcess = true;
    gl_pChinaStockMarket->IncreaseNeteaseDLNeedProcessNumber();
  }
  else {
    ASSERT(m_fDLNeedProcess);
    m_fDLNeedProcess = false;
    gl_pChinaStockMarket->DecreaseNeteaseDLNeedProcessNumber();
  }
}

void CChinaStock::ShowDL(CDC* pDC, CRect rectClient) {
  m_DL.ShowData(pDC, rectClient);
}

void CChinaStock::ShowWeekLine(CDC* pDC, CRect rectClient) {
  m_WeekLine.ShowData(pDC, rectClient);
}

void CChinaStock::GetRS1Day(vector<double>& vRS) {
  m_DL.GetRS1(vRS);
}

void CChinaStock::GetRSIndex1Day(vector<double>& vRS) {
  m_DL.GetRSIndex1(vRS);
}

void CChinaStock::GetRSLogarithm1Day(vector<double>& vRS) {
  m_DL.GetRSLogarithm1(vRS);
}

void CChinaStock::GetRS3Day(vector<double>& vRS) {
  m_DL.GetRS3(vRS);
}

void CChinaStock::GetRS5Day(vector<double>& vRS) {
  m_DL.GetRS5(vRS);
}

void CChinaStock::GetRS10Day(vector<double>& vRS) {
  m_DL.GetRS10(vRS);
}

void CChinaStock::GetRS30Day(vector<double>& vRS) {
  m_DL.GetRS30(vRS);
}

void CChinaStock::GetRS60Day(vector<double>& vRS) {
  m_DL.GetRS60(vRS);
}

void CChinaStock::GetRS120Day(vector<double>& vRS) {
  m_DL.GetRS120(vRS);
}

void CChinaStock::__TestSetDLBuffer(INT64 lBufferLength, char* pDLBuffer) {
  char* p;
  if (m_pDLBuffer != nullptr) delete m_pDLBuffer;
  m_pDLBuffer = new char[lBufferLength + 1];
  p = m_pDLBuffer;
  for (int i = 0; i < lBufferLength; i++) {
    *p++ = pDLBuffer[i];
  }
  m_pDLBuffer[lBufferLength] = 0x000;
  m_lDLBufferLength = lBufferLength;
}

bool CChinaStock::IsVolumeConsistence(void) noexcept {
  if ((m_lHighLimit2 > 0) && (m_lLowLimit2 > 0)) {
    if ((m_lHighLimit != m_lHighLimit2) || (m_lLowLimit != m_lLowLimit2)) {
      TRACE(_T("%s�ǵ�ͣ��۸񲻷���%d %d    %d  %d\n"), GetStockCode().GetBuffer(), m_lHighLimit, m_lHighLimit2, m_lLowLimit, m_lLowLimit2);
    }
    if ((m_lPBuy[0] > 0) && (m_lPSell[0] > 0)) { // ���ǵ�ͣ���ʱ
      m_lHighLimit2 = m_lLowLimit2 = 0; // ���ô�������
    }
  }
  if (GetVolume() != GetOrdinaryBuyVolume() + GetOrdinarySellVolume() + GetAttackBuyVolume()
      + GetAttackSellVolume() + GetStrongBuyVolume() + GetStrongSellVolume() + GetUnknownVolume()) {
    TRACE(_T("%14Id %s����%d\n"), FormatToDateTime(m_TransactionTime), GetStockCode().GetBuffer(), GetVolume());
    TRACE(_T("%d %d %d %d %d %d %d\n"), GetOrdinaryBuyVolume(), GetOrdinarySellVolume(), GetAttackBuyVolume(),
          GetAttackSellVolume(), GetStrongBuyVolume(), GetStrongSellVolume(), GetUnknownVolume());
    return false;
  }
  else return true;
}

bool CChinaStock::CalculatingWeekLine(long lStartDate) {
  ASSERT(IsDLLoaded());
  ASSERT(m_DL.GetDataSize() > 0);
  long i = 0;
  CWeekLinePtr pWeekLine = nullptr;

  m_WeekLine.Unload();
  while ((i < m_DL.GetDataSize()) && (m_DL.GetData(i)->GetFormatedMarketDate() < lStartDate)) {
    i++;
  }
  if (i < m_DL.GetDataSize()) {
    do {
      pWeekLine = m_DL.CreateNewWeekLine(i);
      m_WeekLine.StoreData(pWeekLine);
    } while (i < m_DL.GetDataSize());
    m_WeekLine.SetDataLoaded(true);
    return true;
  }
  else return false;
}

#ifdef _DEBUG
void CChinaStock::AssertValid() const {
  CObject::AssertValid();
}

void CChinaStock::Dump(CDumpContext& dc) const {
  CObject::Dump(dc);
}

#endif //_DEBUG