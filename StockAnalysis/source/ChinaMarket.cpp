//////////////////////////////////////////////////////////////////////////////////////////////////

//#include"stdafx.h"

#include"globedef.h"
#include"Thread.h"

#include"WebInquirer.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

#include"SetDayLineExtendInfo.h"
#include"SetDayLineToday.h"
#include"SetOption.h"
#include"SetOptionChinaStockMarket.h"
#include"SetCrweberIndex.h"
#include"SetChoicedStock.h"
#include"SetRSStrong2Stock.h"
#include"SetRSStrong1Stock.h"
#include"SetRSStrongStock.h"
#include"SetRSOption.h"

using namespace std;
#include<thread>
#include<algorithm>

// �ź�����������Ϊȫ�ֱ�����Ϊ�˳�ʼ����
Semaphore gl_SaveOneStockDayLine(4);  // ���ź�����������������ʷ���ݿ�
Semaphore gl_ProcessSinaRTDataQueue(1);   // ����ʵʱ���ݴ���ͬʱֻ����һ���̴߳���
Semaphore gl_ProcessTengxunRTDataQueue(1);
Semaphore gl_ProcessNeteaseRTDataQueue(1);
Semaphore gl_SemaphoreBackGroundTaskThreads(cMaxBackGroundTaskThreads); // ��̨�����߳��������Ϊ8

CRTDataContainer gl_RTDataContainer;
CWebInquirer gl_WebInquirer;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChinaMarket::CChinaMarket(void) : CVirtualMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE(_T("ChinaMarket�г�����ֻ�������һ��ʵ��\n"));
  }
  m_strMarketId = _T("�й���Ʊ�г�");
  m_lTimeZoneOffset = -8 * 3600; // ������׼ʱ��λ�ڶ���������ǰGMT8Сʱ
  m_fSaveRTData = false; // �˴洢ʵʱ���ݱ�ʶ�����ڴ洢�����Ժ����õ�ʵʱ���ݡ�Ŀǰ�����Ѿ���ɡ�
  Reset();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ȫ�ֱ����Ľ���λ�ڳ����˳������Ҫ����CMainFrame�Ľ������ʶ����Ҫ�뽫ϵͳ�˳��Ĺ��̷��������Ҫ�о���
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CChinaMarket::~CChinaMarket() {
}

void CChinaMarket::ResetMarket(void) {
  TRACE(_T("�����й�����\n"));
  CString str = _T("�����й������ڱ�����׼ʱ�䣺");
  str += GetMarketTimeString();
  gl_systemMessage.PushInformationMessage(str);
  while (gl_ThreadStatus.IsBackGroundthreadsWorking() || gl_ThreadStatus.IsCalculatingRTData() || gl_ThreadStatus.IsSavingTempData()
         || gl_ThreadStatus.IsSavingDayLine()) {
    Sleep(1);
  }
  Reset();
  LoadStockCodeDB();
  LoadOptionDB();
  LoadOptionChinaStockMarketDB();
  LoadChoicedStockDB();
  Load10DayRSStrong1StockSet();
  Load10DayRSStrong2StockSet();
  LoadCalculatingRSOption();
  Load10DayRSStrongStockDB();
}

void CChinaMarket::Reset(void) {
  CalculateTime(); // ��ʼ���г�ʱ��
  m_lTotalActiveStock = 0; // ��ʼʱ��Ʊ���еĹ�Ʊ����Ϊ��

  m_llRTDataReceived = 0;
  m_lStockNeedUpdated = 0;

  m_fLoadedSelectedStock = false;
  m_fSystemReady = false;    // �г���ʼ״̬Ϊδ���úá�
  m_fCurrentStockChanged = false;
  m_fChoiced10RSStrong1StockSet = false;
  m_fChoiced10RSStrong2StockSet = false;
  m_fChoiced10RSStrongStockSet = false;
  m_fCurrentEditStockChanged = false;

  m_lTotalMarketBuy = m_lTotalMarketSell = 0;

  m_ttNewestTransactionTime = 0;

  if (GetFormatedMarketTime() >= 150400) { // �й���Ʊ�г��Ѿ�����
    m_fTodayStockProcessed = true; // ���к��ִ�б�ϵͳ������Ϊ�Ѿ���������չ�Ʊ�����ˡ�
  }
  else m_fTodayStockProcessed = false;

  m_lRelativeStrongEndDay = m_lRelativeStrongStartDay = m_lLastLoginDay = __CHINA_MARKET_BEGIN_DAY__;
  m_lUpdatedDayFor10DayRS2 = m_lUpdatedDayFor10DayRS1 = m_lUpdatedDayFor10DayRS = __CHINA_MARKET_BEGIN_DAY__;

  m_fSaveDayLine = false;
  m_fMarketOpened = false;
  m_fSaveTempData = true;

  m_fTodayTempDataLoaded = false;

  m_lCurrentRSStrongIndex = 0;
  m_lCurrentSelectedStockSet = -1; // ѡ��ʹ��ȫ���Ʊ����
  m_lCurrentSelectedPosition = 0;

  m_fRTDataSetCleared = false;

  m_fCheckActiveStock = true;  //��鵱�ջ�Ծ��Ʊ������Ϊ�档

  m_fGetRTData = true;
  m_iCountDownSlowReadingRTData = 3; // 400����ÿ��

  m_iRTDataServer = 0; // ʹ������ʵʱ���ݷ�����

  m_fUsingSinaRTDataReceiver = true; // ʹ������ʵʱ������ȡ��
  m_fUsingTengxunRTDataReceiver = true; // Ĭ��״̬�¶�ȡ��Ѷʵʱ����
  m_fUsingNeteaseRTDataReceiver = true; // ʹ������ʵʱ������ȡ��
  m_iCountDownTengxunNumber = 5;

  m_fUpdateStockCodeDB = false;
  m_fUpdateChoicedStockDB = false;

  m_iDayLineNeedProcess = 0;
  m_iDayLineNeedSave = 0;
  m_iDayLineNeedUpdate = 0;

  m_lSinaRTDataInquiringIndex = 0;
  m_lTengxunRTDataInquiringIndex = 0;
  m_lNeteaseRTDataInquiringIndex = 0;
  m_lNeteaseDayLineDataInquiringIndex = 0;

  m_pCurrentStock = nullptr;

  // ���ɹ�Ʊ�����
  CreateTotalStockContainer();
}

#ifdef _DEBUG
void CChinaMarket::AssertValid() const {
  CVirtualMarket::AssertValid();
}

void CChinaMarket::Dump(CDumpContext& dc) const {
  CVirtualMarket::Dump(dc);
}
#endif //_DEBUG

bool CChinaMarket::CheckMarketReady(void) noexcept {
  if (!m_fSystemReady) {
    if (m_llRTDataReceived > m_lTotalStock * 2) {
      m_fSystemReady = true;
      gl_systemMessage.PushInformationMessage(_T("�й���Ʊ�г���ʼ�����"));
    }
  }
  return m_fSystemReady;
}

bool CChinaMarket::ChangeToNextStock(void) {
  ASSERT(m_pCurrentStock != nullptr);
  long lIndex = m_pCurrentStock->GetOffset();
  CChinaStockPtr pStock = m_pCurrentStock;

  if (IsTotalStockSetSelected()) {
    bool fFound = false;
    int i = 1;
    while (!fFound) {
      if ((lIndex + i) < 12000) {
        pStock = GetStock(lIndex + i);
      }
      else {
        pStock = GetStock(lIndex + i - 12000);
      }
      if (!pStock->IsNullStock()) fFound = true;
      i++;
    }
  }
  else {
    ASSERT(m_avChoicedStock[m_lCurrentSelectedStockSet].size() > 0); //
    if (m_lCurrentSelectedPosition >= (m_avChoicedStock[m_lCurrentSelectedStockSet].size() - 1)) {
      m_lCurrentSelectedPosition = 0;
      pStock = m_avChoicedStock[m_lCurrentSelectedStockSet][m_lCurrentSelectedPosition];
    }
    else {
      m_lCurrentSelectedPosition++;
      pStock = m_avChoicedStock[m_lCurrentSelectedStockSet][m_lCurrentSelectedPosition];
    }
  }

  SetCurrentStock(pStock);
  return true;
}

bool CChinaMarket::ChangeToPrevStock(void) {
  ASSERT(m_pCurrentStock != nullptr);
  long lIndex = m_pCurrentStock->GetOffset();
  CChinaStockPtr pStock = m_pCurrentStock;

  if (IsTotalStockSetSelected()) {
    bool fFound = false;
    int i = 1;
    while (!fFound) {
      if ((lIndex - i) >= 0) {
        pStock = GetStock(lIndex - i);
      }
      else {
        pStock = GetStock(lIndex + 12000 - i);
      }
      if (!pStock->IsNullStock()) fFound = true;
      i++;
    }
  }
  else {
    ASSERT(m_avChoicedStock[m_lCurrentSelectedStockSet].size() > 0); //
    if (m_lCurrentSelectedPosition == 0) {
      m_lCurrentSelectedPosition = m_avChoicedStock[m_lCurrentSelectedStockSet].size() - 1;
      pStock = m_avChoicedStock[m_lCurrentSelectedStockSet][m_lCurrentSelectedPosition];
    }
    else {
      m_lCurrentSelectedPosition--;
      pStock = m_avChoicedStock[m_lCurrentSelectedStockSet][m_lCurrentSelectedPosition];
    }
  }
  SetCurrentStock(pStock);
  return true;
}

bool CChinaMarket::ChangeToPrevStockSet(void) {
  do {
    if (m_lCurrentSelectedStockSet > -1) m_lCurrentSelectedStockSet--;
    else {
      m_lCurrentSelectedStockSet = c_10DayRSStockSetStartPosition + 9;
    }
    ASSERT(m_lCurrentSelectedStockSet < 20);
  } while ((m_lCurrentSelectedStockSet != -1) && (m_avChoicedStock[m_lCurrentSelectedStockSet].size() == 0));

  return true;
}

bool CChinaMarket::ChangeToNextStockSet(void) {
  do {
    if (m_lCurrentSelectedStockSet == (c_10DayRSStockSetStartPosition + 9)) m_lCurrentSelectedStockSet = -1;
    else {
      m_lCurrentSelectedStockSet++;
    }
    ASSERT(m_lCurrentSelectedStockSet < 20);
  } while ((m_lCurrentSelectedStockSet != -1) && (m_avChoicedStock[m_lCurrentSelectedStockSet].size() == 0));

  return true;
}

size_t CChinaMarket::GetCurrentStockSetSize(void) {
  if (IsTotalStockSetSelected()) return m_lTotalStock;
  else return m_avChoicedStock[m_lCurrentSelectedStockSet].size();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ʼ�����п��ܵĹ�Ʊ����أ�ֻ��CChinaMarket�ĳ�ʼ��������һ�Ρ�
// ���������Ҫ����ȫ�ֱ�����ʼ����֧�֣��ʶ�gl_ChinaStockMarketʵ����Ҫ��������ȫ�ֱ��������
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::CreateTotalStockContainer(void) {
  char buffer[10]{};

  CChinaStockPtr pStock = nullptr;
  int iCount = 0;

  // ���֮ǰ�����ݣ�����еĻ�����Resetʱ�������������оʹ������ݣ���
  m_vChinaMarketStock.clear();
  m_mapChinaMarketAStock.clear();

  // �����Ϻ���Ʊ����
  for (int i = 600000; i < 602000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStock = make_shared<CChinaStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // �Ϻ�����
    pStock->SetOffset(iCount);
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
    ASSERT(pStock->IsDayLineNeedUpdate());
    m_iDayLineNeedUpdate++;
  }

  // ���������Ʊ����
  for (int i = 603000; i < 604000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStock = make_shared<CChinaStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // �Ϻ�����
    pStock->SetOffset(iCount);
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // ���ɿƴ����Ʊ����
  for (int i = 688000; i < 689000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStock = make_shared<CChinaStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // �Ϻ��ƴ���
    pStock->SetOffset(iCount);
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // ����B�ɹ�Ʊ����
  for (int i = 900000; i < 901000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStock = make_shared<CChinaStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // �Ϻ�B��
    pStock->SetOffset(iCount);
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // �����Ϻ�ָ������
  for (int i = 0; i < 1000; i++) {
    CString str = _T("sh");
    sprintf_s(buffer, 10, _T("%06d"), i);
    pStock = make_shared<CChinaStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // �Ϻ�ָ��
    pStock->SetOffset(iCount);
    pStock->SetNeedProcessRTData(false);
    m_vChinaMarketStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  ///////////////////////////////////////////////
  // �������������Ʊ����
  for (int i = 0; i < 2000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, _T("%06d"), i);
    pStock = make_shared<CChinaStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // ��������
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // ����������С���Ʊ����
  for (int i = 2000; i < 3000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, _T("%06d"), i);
    pStock = make_shared<CChinaStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // ������С��
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // ����B�ɹ�Ʊ����
  for (int i = 200000; i < 201000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, _T("%06d"), i);
    pStock = make_shared<CChinaStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // ����B��
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // ���ɴ�ҵ���Ʊ����
  for (int i = 300000; i < 301000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, _T("%06d"), i);
    pStock = make_shared<CChinaStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // ���ڴ�ҵ��
    pStock->SetNeedProcessRTData(true);
    m_vChinaMarketStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // ��������ָ��
  for (int i = 399000; i < 400000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, _T("%06d"), i);
    pStock = make_shared<CChinaStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // ����ָ��
    pStock->SetNeedProcessRTData(false);
    m_vChinaMarketStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }
  m_lTotalStock = m_vChinaMarketStock.size();
  ASSERT(m_lTotalStock == m_iDayLineNeedUpdate);
  ASSERT(m_iDayLineNeedUpdate == 12000); // Ŀǰ�ܲ�ѯ��Ʊ��Ϊ12000����
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����������߹�Ʊ������ַ��������ڲ�ѯ�˹�Ʊ�ڵ�ǰ�г��Ƿ��ڻ�Ծ״̬�������Ƿ���ڴ˹�Ʊ���룩
//
//  �˺����Ǽ��m_vChinaMarketStock��Ʊ��
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::CreateNeteaseDayLineInquiringStr(CString& strReturn) {
  bool fFound = false;
  int iCount = 0;
  CString strTemp;
  strReturn = _T("");
  while (!fFound && (iCount++ < 1000)) {
    CChinaStockPtr pStock = m_vChinaMarketStock.at(m_lNeteaseDayLineDataInquiringIndex);
    if (!pStock->IsDayLineNeedUpdate()) { // �������ݲ���Ҫ���¡���ϵͳ��ʼʱ�����ô�m_fDayLineNeedUpdate��ʶ
      // TRACE("%S ���������������\n", static_cast<LPCWSTR>(pStock->m_strStockCode));
      IncreaseStockInquiringIndex(m_lNeteaseDayLineDataInquiringIndex);
    }
    else if (pStock->IsNullStock()) {	// ��δʹ�ù��Ĺ�Ʊ���������ѯ��������
      pStock->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
      // TRACE("��Ч��Ʊ���룺%S, �����ѯ��������\n", static_cast<LPCWSTR>(pStock->m_strStockCode));
      IncreaseStockInquiringIndex(m_lNeteaseDayLineDataInquiringIndex);
    }
    else if (pStock->GetDayLineEndDay() >= GetLastTradeDay()) { //��һ�����յ����������Ѿ��洢����ʱ�Ѿ������һ�����������ˣ������ٴδ���
      pStock->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
      // TRACE("%S �������ݱ����Ѹ���\n", static_cast<LPCWSTR>(pStock->m_strStockCode));
      IncreaseStockInquiringIndex(m_lNeteaseDayLineDataInquiringIndex);
    }
    else if (pStock->IsDayLineNeedProcess()) { // �������������ص���δ����һ������������֣�
      IncreaseStockInquiringIndex(m_lNeteaseDayLineDataInquiringIndex);
    }
    else {
      fFound = true;
    }
  }

  if (iCount >= 1000) { //  û���ҵ���Ҫ�������ߵĹ�Ʊ
    TRACE("δ�ҵ������������ʷ���ݵĹ�Ʊ\n");
    return false;
  }

  // �ҵ���������������ʷ���ݵĹ�Ʊ��siCounterΪ������
  CChinaStockPtr pStock = m_vChinaMarketStock.at(m_lNeteaseDayLineDataInquiringIndex);
  ASSERT(!pStock->IsDayLineNeedSaving());
  ASSERT(!pStock->IsDayLineNeedProcess());
  ASSERT(pStock->IsDayLineNeedUpdate());
  pStock->SetDayLineNeedUpdate(false);
  switch (pStock->GetMarket()) { // ת����������������������ʽ���Ϻ�Ϊ��0��������Ϊ��1����
  case __SHANGHAI_MARKET__: // �Ϻ��г���
  case __SHANGHAI_MAIN__: // �Ϻ����壿
  case __SHANGHAI_INDEX__: // �Ϻ�ָ��
  case __SHANGHAI_3BAN__: // �Ϻ�3��
  case __SHANGHAI_KECHUANG__: // �Ϻ��ƴ���
  case __SHANGHAI_B_SHARE__: // �Ϻ�B��
  strReturn += '0'; // �Ϻ��г���ʶ
  break;
  case __SHENZHEN_MARKET__: // �����г���
  case __SHENZHEN_INDEX__: // ����ָ��
  case __SHENZHEN_3BAN__: // ������С��
  case __SHENZHEN_B_SHARE__: // ����B��
  case __SHENZHEN_CHUANGYE__: // ���ڴ�ҵ��
  case __SHENZHEN_MAIN__: // ��������
  strReturn += '1'; // �����г���ʶ
  break;
  default: // Խ��
  ASSERT(0);
  }
  strReturn += pStock->GetStockCode().Right(6); // ȡ��Ʊ������ұ���λ���֡�
  IncreaseStockInquiringIndex(m_lNeteaseDayLineDataInquiringIndex);
  return true;
}

long CChinaMarket::IncreaseStockInquiringIndex(long& lIndex) {
  if (++lIndex == m_lTotalStock) {
    lIndex = 0;
  }
  return lIndex;
}

/////////////////////////////////////////////////////////////////////////
//
//	�õ���ʱ��ƫ������09:30Ϊ0��15:00Ϊ240,����Ϊ1����
//
//
////////////////////////////////////////////////////////////////////////
long CChinaMarket::GetMinLineOffset(time_t Time) {
  ASSERT(Time >= 0);
  tm tmTemp{};
  time_t t = 0;
  long lIndex = 0;

  gmtime_s(&tmTemp, &Time);
  tmTemp.tm_hour = 9;
  tmTemp.tm_min = 30;
  tmTemp.tm_sec = 0;
  t = mktime(&tmTemp);
  lIndex = (Time - t) / 60;
  if (lIndex < 0) lIndex = 0;
  if ((lIndex >= 120) && (lIndex < 209)) lIndex = 119;
  if (lIndex >= 210) lIndex -= 90;
  if (lIndex >= 240) lIndex = 239;

  ASSERT((lIndex >= 0) && (lIndex < 240));
  return(lIndex);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		�ж�pStock�Ƿ�Ϊ����A�ɡ�
//		����A�ɴ�����6��ͷ������A�ɴ�����00��ͷ��
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::IsAStock(CChinaStockPtr pStock) {
  ASSERT(pStock != nullptr);

  return(IsAStock(pStock->GetStockCode()));
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		�ж�strStockCode�Ƿ�Ϊ����A������Ĺ�Ʊ���롣
//		����A�ɴ�����600��601��ͷ������A�ɴ�����000��001��ͷ��
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::IsAStock(CString strStockCode) {
  if (strStockCode[0] == 's') {
    if ((strStockCode[1] == 'h') && (strStockCode[2] == '6') && (strStockCode[3] == '0')) {
      if ((strStockCode[4] == '0') || (strStockCode[4] == '1')) {
        return true;
      }
    }
    else {
      if ((strStockCode[1] == 'z') && (strStockCode[2] == '0') && (strStockCode[3] == '0')) {
        if ((strStockCode[4] == '0') || (strStockCode[4] == '2')) {
          return true;
        }
      }
    }
  }
  return(false);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		�жϴ����Ƿ�Ϊ�����Ʊ��
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::IsStock(CString strStockCode) {
  CChinaStockPtr pStock = GetStock(strStockCode);
  if (pStock != nullptr) {
    return(true);
  }
  else {
    return(false);
  }
}

bool CChinaMarket::IsDayLineDBUpdated(void) {
  for (auto pStock : m_vChinaMarketStock) {
    if (pStock->IsDayLineDBUpdated()) return true;
  }
  return false;
}

void CChinaMarket::ClearDayLineDBUpdatedFlag(void) {
  for (auto pStock : m_vChinaMarketStock) {
    pStock->SetDayLineDBUpdated(false);
  }
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////
INT64 CChinaMarket::GetTotalAttackBuyAmount(void) {
  INT64 lAmount = 0;
  for (auto pStock : m_vChinaMarketStock) {
    if (pStock->IsActive() && IsAStock(pStock)) {
      lAmount += pStock->GetAttackBuyAmount();
    }
  }
  return(lAmount);
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////
INT64 CChinaMarket::GetTotalAttackSellAmount(void) {
  INT64 lAmount = 0;
  for (auto pStock : m_vChinaMarketStock) {
    if (pStock->IsActive() && IsAStock(pStock)) {
      lAmount += pStock->GetAttackSellAmount();
    }
  }
  return(lAmount);
}

bool CChinaMarket::TaskGetNeteaseDayLineFromWeb(void) {
  ASSERT(IsSystemReady());
  if (m_iDayLineNeedUpdate > 0) {
    // ץȡ��������.
    // ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
    return(gl_WebInquirer.GetNeteaseDayLineData());
  }
  else return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ���ݵȣ���SchedulingTaskPerSecond��������,ÿ����ִ��һ�Ρ�
// ��ʵʱ�����ݴ�����е����ݷֱ��ŵ����Թ�Ʊ��ʵʱ�����С�
// �ַ�����ʱ��ֻ�ַ��µģ�����ʱ������֮ǰ���ݵģ�ʵʱ���ݡ�
//
// �˺����õ�������ȫ�ֱ��������Ƿ������߳�Ϊ�á������߳�Ŀǰ����ֻ��������ɵĹҵ������
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskDistributeSinaRTDataToProperStock(void) {
  gl_ProcessSinaRTDataQueue.Wait();
  CChinaStockPtr pStock;
  const size_t lTotalNumber = gl_RTDataContainer.GetSinaRTDataSize();
  CString strVolume;

  for (int iCount = 0; iCount < lTotalNumber; iCount++) {
    CRTDataPtr pRTData = gl_RTDataContainer.PopSinaRTData();
    if (pRTData->GetDataSource() == __INVALID_RT_WEB_DATA__) {
      gl_systemMessage.PushInnerSystemInformationMessage(_T("����ʵʱ����Դ��������"));
      continue;
    }
    if (pRTData->IsActive()) { // ��ʵʱ������Ч��
      if (m_ttNewestTransactionTime < pRTData->GetTransactionTime()) {
        m_ttNewestTransactionTime = pRTData->GetTransactionTime();
      }
      pStock = GetStock(pRTData->GetStockCode());
      if (!pStock->IsActive()) {
        if (pRTData->IsValidTime()) {
          pStock->SetTodayActive(pRTData->GetMarket(), pRTData->GetStockCode(), pRTData->GetStockName());
          pStock->SetIPOStatus(__STOCK_IPOED__);
        }
      }
      if (pRTData->GetTransactionTime() > pStock->GetTransactionTime()) { // �µ����ݣ�
        pStock->PushRTData(pRTData); // �洢�µ����������ݳ�
        if (pStock->IsRecordRTData()) {
          StoreChoiceRTData(pRTData);
        }
        pStock->SetTransactionTime(pRTData->GetTransactionTime());   // �������½��ܵ�ʵʱ���ݵ�ʱ��
      }
    }
  }
  gl_ThreadStatus.SetRTDataNeedCalculate(true); // ���ý��յ�ʵʱ���ݱ�ʶ
  ASSERT(gl_RTDataContainer.GetSinaRTDataSize() == 0); // ����һ�δ���ȫ�����ݡ�
  gl_ProcessSinaRTDataQueue.Signal();

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ���ݵȣ���SchedulingTaskPerSecond��������,ÿ����ִ��һ�Ρ�
// ��ʵʱ�����ݴ�����е����ݷֱ��ŵ����Թ�Ʊ��ʵʱ�����С�
// �ַ�����ʱ��ֻ�ַ��µģ�����ʱ������֮ǰ���ݵģ�ʵʱ���ݡ�
//
// �˺����õ�������ȫ�ֱ��������Ƿ������߳�Ϊ�á������߳�Ŀǰ����ֻ��������ɵĹҵ������
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskDistributeNeteaseRTDataToProperStock(void) {
  gl_ProcessNeteaseRTDataQueue.Wait();
  CChinaStockPtr pStock;
  const size_t lTotalNumber = gl_RTDataContainer.GetNeteaseRTDataSize();
  CString strVolume;

  for (int iCount = 0; iCount < lTotalNumber; iCount++) {
    CRTDataPtr pRTData = gl_RTDataContainer.PopNeteaseRTData();
    if (pRTData->GetDataSource() == __INVALID_RT_WEB_DATA__) {
      gl_systemMessage.PushInnerSystemInformationMessage(_T("����ʵʱ����Դ��������"));
      continue;
    }
    if (pRTData->IsActive()) { // ��ʵʱ������Ч��
      if (m_ttNewestTransactionTime < pRTData->GetTransactionTime()) {
        m_ttNewestTransactionTime = pRTData->GetTransactionTime();
      }
      pStock = GetStock(pRTData->GetStockCode());
      if (!pStock->IsActive()) {
        if (pRTData->IsValidTime()) {
          pStock->SetTodayActive(pRTData->GetMarket(), pRTData->GetStockCode(), pRTData->GetStockName());
          pStock->SetIPOStatus(__STOCK_IPOED__);
        }
      }
      if (pRTData->GetTransactionTime() > pStock->GetTransactionTime()) { // �µ����ݣ�
        pStock->PushRTData(pRTData); // �洢�µ����������ݳ�
        if (pStock->IsRecordRTData()) {
          StoreChoiceRTData(pRTData);
        }
        pStock->SetTransactionTime(pRTData->GetTransactionTime());   // �������½��ܵ�ʵʱ���ݵ�ʱ��
      }
    }
  }
  gl_ThreadStatus.SetRTDataNeedCalculate(true); // ���ý��յ�ʵʱ���ݱ�ʶ
  ASSERT(gl_RTDataContainer.GetNeteaseRTDataSize() == 0); // ����һ�δ���ȫ�����ݡ�
  gl_ProcessNeteaseRTDataQueue.Signal();

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ÿ�β�ѯ����ʵʱ��Ʊ���ݵ��ַ���
//
//////////////////////////////////////////////////////////////////////////////////////////
CString CChinaMarket::GetSinaInquiringStockStr(long lTotalNumber, bool fSkipUnactiveStock) {
  return GetNextInquiringMiddleStr(m_lSinaRTDataInquiringIndex, _T(","), lTotalNumber, fSkipUnactiveStock);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ÿ�β�ѯ��Ѷʵʱ��Ʊ���ݵ��ַ���
//
//////////////////////////////////////////////////////////////////////////////////////////
CString CChinaMarket::GetTengxunInquiringStockStr(long lTotalNumber, bool fSkipUnactiveStock) {
  ASSERT(IsSystemReady());
  return GetNextInquiringMiddleStr(m_lTengxunRTDataInquiringIndex, _T(","), lTotalNumber, fSkipUnactiveStock);
}

CString CChinaMarket::GetNeteaseInquiringStockStr(long lTotalNumber, bool fSkipUnactiveStock) {
  CString strStockCode, strRight6, strLeft2, strPrefix;

  m_strNeteaseRTDataInquiringStr = _T("");
  if (fSkipUnactiveStock) StepToActiveStockIndex(m_lNeteaseRTDataInquiringIndex);
  strStockCode = m_vChinaMarketStock.at(m_lNeteaseRTDataInquiringIndex)->GetStockCode();
  IncreaseStockInquiringIndex(m_lNeteaseRTDataInquiringIndex);
  strRight6 = strStockCode.Right(6);
  strLeft2 = strStockCode.Left(2);
  if (strLeft2.Compare(_T("sh")) == 0) {
    strPrefix = _T("0");
  }
  else strPrefix = _T("1");
  m_strNeteaseRTDataInquiringStr += strPrefix + strRight6;  // �õ���һ����Ʊ����
  int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
  while ((m_lNeteaseRTDataInquiringIndex < m_lTotalStock) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
    if (fSkipUnactiveStock) StepToActiveStockIndex(m_lNeteaseRTDataInquiringIndex);
    iCount++;
    m_strNeteaseRTDataInquiringStr += _T(",");
    strStockCode = m_vChinaMarketStock.at(m_lNeteaseRTDataInquiringIndex)->GetStockCode();
    strRight6 = strStockCode.Right(6);
    strLeft2 = strStockCode.Left(2);
    if (strLeft2.Compare(_T("sh")) == 0) {
      strPrefix = _T("0");
    }
    else strPrefix = _T("1");
    m_strNeteaseRTDataInquiringStr += strPrefix + strRight6;  // �õ���һ����Ʊ����
    if (m_lNeteaseRTDataInquiringIndex == 0) break;
    IncreaseStockInquiringIndex(m_lNeteaseRTDataInquiringIndex);
  }
  if (m_lNeteaseRTDataInquiringIndex > 0) m_lNeteaseRTDataInquiringIndex--;// �˺�һ������ֹ���һ����Ʊ��ѯ������ʵ����Ҫ�ˣ�

  return m_strNeteaseRTDataInquiringStr;
}

bool CChinaMarket::CheckValidOfNeteaseDayLineInquiringStr(CString str) {
  long lLength = str.GetLength();
  char* p = str.GetBuffer();
  int i = 0;
  char buffer[8];
  CString strStockCode, strRight;

  while (i < lLength) {
    strncpy_s(buffer, p, 7);
    p += 8;
    i += 8;
    buffer[7] = 0x000;
    strRight = buffer;
    if (buffer[0] == '0') strStockCode = "sh";
    else if (buffer[0] == '1') strStockCode = "sz";
    else return false;
    strStockCode += strRight.Right(6);
    CChinaStockPtr pStock = GetStock(strStockCode);
    if (pStock == nullptr) {
      CString strReport = _T("�������߲�ѯ��Ʊ�������");
      TRACE(_T("�������߲�ѯ��Ʊ�������%s\n"), strStockCode.GetBuffer());
      strReport += strStockCode;
      gl_systemMessage.PushInnerSystemInformationMessage(strReport);
      return false;
    }
  }
  return true;
}

CString CChinaMarket::GetNextInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber, bool fSkipUnactiveStock) {
  CString str = _T("");

  if (fSkipUnactiveStock) StepToActiveStockIndex(iStockIndex);
  str += m_vChinaMarketStock.at(iStockIndex)->GetStockCode();  // �õ���һ����Ʊ����
  IncreaseStockInquiringIndex(iStockIndex);
  int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
  while ((iStockIndex < GetTotalStock()) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
    if (fSkipUnactiveStock) StepToActiveStockIndex(iStockIndex);
    iCount++;
    str += strPostfix;
    str += m_vChinaMarketStock.at(iStockIndex)->GetStockCode();
    if (iStockIndex == 0) break;
    IncreaseStockInquiringIndex(iStockIndex);
  }
  if (iStockIndex > 0) iStockIndex--; // �˺�һ������ֹ���һ����Ʊ��ѯ������ʵ����Ҫ�ˣ�

  return str;
}

bool CChinaMarket::StepToActiveStockIndex(long& iStockIndex) {
  while (!m_vChinaMarketStock.at(iStockIndex)->IsActive()) {
    IncreaseStockInquiringIndex(iStockIndex);
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// �ɹ����߳�ThreadCalculatingRTDataProc���ã�ע��ȫ�ֱ�����ʹ��
//
// Ŀǰ���õķ�ʽ��ֱ�Ӽ������߳��У������ù����߳�ģʽ���á�
//
/////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskProcessRTData(void) {
  ASSERT(gl_ThreadStatus.IsRTDataNeedCalculate());
  ASSERT(!gl_ThreadStatus.IsSavingTempData()); // �������̻߳���
  for (auto pStock : m_vChinaMarketStock) {
    if (pStock->IsActive()) {
      pStock->ProcessRTData();
    }
  }
  return true;
}

bool CChinaMarket::TaskProcessWebRTDataGetFromSinaServer(void) {
  CWebDataPtr pWebDataReceived = nullptr;
  size_t lTotalData = gl_WebInquirer.GetSinaRTDataSize();
  for (int i = 0; i < lTotalData; i++) {
    pWebDataReceived = gl_WebInquirer.PopSinaRTData();
    pWebDataReceived->SetCurrentPos(0);
    while (pWebDataReceived->GetCurrentPos() < pWebDataReceived->GetBufferLength()) {
      CRTDataPtr pRTData = make_shared<CRTData>();
      if (pRTData->ReadSinaData(pWebDataReceived)) {
        m_llRTDataReceived++;
        gl_RTDataContainer.PushSinaRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
      }
      else return false;  // ��������ݳ����⣬�׵����á�
    }
  }
  return true;
}

void CChinaMarket::StoreChoiceRTData(CRTDataPtr pRTData) {
  m_qRTData.push(pRTData);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ������ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
// ��ʼ��Ϊ��һ��{��������Ϊ�����ڶ���}��������������账���򱻴�����ַ�Ϊ','�����û�������ˣ��򱻴�����ַ�Ϊ' '��
//
// Ҫ��ȡ�������飬�������ݽӿڣ�http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// ����ʵʱ����ȱ�ٹؼ��Եĳɽ����һ��ʶ��޷���Ϊ�������ݣ�ֻ����Ϊ������.��turnover��Ϊ�ɽ�������ʹ��֮��05/12/2020��
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskProcessWebRTDataGetFromNeteaseServer(void) {
  CWebDataPtr pWebDataReceived = nullptr;
  int iCount = 0;

  size_t lTotalData = gl_WebInquirer.GetNeteaseRTDataSize();
  for (int i = 0; i < lTotalData; i++) {
    pWebDataReceived = gl_WebInquirer.PopNeteaseRTData();
    pWebDataReceived->m_pCurrentPos = pWebDataReceived->m_pDataBuffer;
    pWebDataReceived->ResetCurrentPos();
    if (!IsInvalidNeteaseRTData(pWebDataReceived)) {
      if (!IsValidNeteaseRTDataPrefix(pWebDataReceived)) return false;
      iCount = 0;
      while (!((*pWebDataReceived->m_pCurrentPos == ' ') || (pWebDataReceived->m_lCurrentPos >= (pWebDataReceived->m_lBufferLength - 4)))) {
        CRTDataPtr pRTData = make_shared<CRTData>();
        if (pRTData->ReadNeteaseData(pWebDataReceived)) {// ���һ��
          ValidateNeteaseRTData(pRTData);
          iCount++;
          m_llRTDataReceived++;
          gl_RTDataContainer.PushNeteaseRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
        }
        else return false;  // ��������ݳ����⣬�׵����á�
      }
      //TRACE(_T("ReadNetease��������,��������%d������\n"), iCount);
    }
  }

  return true;
}

bool CChinaMarket::IsInvalidNeteaseRTData(CWebDataPtr pWebDataReceived) {
  char buffer[50];
  CString strInvalidStock = _T("_ntes_quote_callback({ });"); // ��Ϊ��Ч��Ʊ��ѯ�������ݸ�ʽ����26���ַ�
  strncpy_s(buffer, pWebDataReceived->m_pCurrentPos, 26);
  buffer[26] = 0x000;
  CString str1 = buffer;

  if (str1.Compare(strInvalidStock) == 0) {
    ASSERT(pWebDataReceived->GetBufferLength() == 26);
    return true;
  }
  else return false;
}

bool CChinaMarket::IsValidNeteaseRTDataPrefix(CWebDataPtr pWebDataReceived) {
  char buffer[50];
  CString strInvalidStock = _T("_ntes_quote_callback("); // ��Ϊ��Ч��Ʊ��ѯ�������ݸ�ʽ����22���ַ�

  strncpy_s(buffer, pWebDataReceived->m_pCurrentPos, 21); // ����"_ntes_quote_callback("
  buffer[21] = 0x000;
  CString str1;
  str1 = buffer;
  if (strInvalidStock.Compare(str1) != 0) { // ���ݸ�ʽ����
    return false;
  }
  pWebDataReceived->IncreaseCurrentPos(21);
  return true;
}

bool CChinaMarket::ValidateNeteaseRTData(CRTDataPtr pRTData) {
  // ���һ��
  CString str;
  ASSERT(pRTData->GetDataSource() == __NETEASE_RT_WEB_DATA__);
  if (pRTData->IsActive()) {
    CChinaStockPtr pStock = nullptr;
    if ((pStock = GetStock(pRTData->GetStockCode())) != nullptr) {
      if (!pStock->IsActive()) {
        str = pStock->GetStockCode();
        str += _T(" ����ʵʱ��⵽�����ڻ�Ծ״̬");
        //gl_systemMessage.PushInnerSystemInformationMessage(str);
      }
    }
    else {
      str = pRTData->GetStockCode();
      str += _T(" ��Ч��Ʊ���루����ʵʱ���ݣ�");
      TRACE("\n��Ч��Ʊ����%s\n", pRTData->GetStockCode().GetBuffer());
      TRACE("����Ĺ�Ʊ��Ϊ�� %s\n\n", m_strNeteaseRTDataInquiringStr.GetBuffer());
      gl_systemMessage.PushInnerSystemInformationMessage(str);
      return false;
    }
    return true;
  }
  else return false;
}

bool CChinaMarket::TaskDiscardNeteaseRTData(void) {
  CRTDataPtr pRTData = nullptr;
  size_t lTotalData = gl_RTDataContainer.GetNeteaseRTDataSize();

  for (size_t i = 0; i < lTotalData; i++) {
    // Ŀǰ��ʹ������ʵʱ���ݣ�����ֻ�Ǽ򵥵�ȡ�����ӵ���
    pRTData = gl_RTDataContainer.PopNeteaseRTData();
    pRTData = nullptr;
  }

  return true;
}

bool CChinaMarket::TaskDiscardSinaRTData(void) {
  CRTDataPtr pRTData = nullptr;
  size_t lTotalData = gl_RTDataContainer.GetSinaRTDataSize();

  for (size_t i = 0; i < lTotalData; i++) {
    // Ŀǰ��ʹ������ʵʱ���ݣ�����ֻ�Ǽ򵥵�ȡ�����ӵ���
    pRTData = gl_RTDataContainer.PopSinaRTData();
    pRTData = nullptr;
  }

  return true;
}

bool CChinaMarket::TaskDiscardTengxunRTData(void) {
  CRTDataPtr pRTData = nullptr;
  size_t lTotalData = gl_RTDataContainer.GetTengxunRTDataSize();

  for (size_t i = 0; i < lTotalData; i++) {
    // Ŀǰ��ʹ������ʵʱ���ݣ�����ֻ�Ǽ򵥵�ȡ�����ӵ���
    pRTData = gl_RTDataContainer.PopTengxunRTData();
    pRTData = nullptr;
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ��Ѷʵʱ���ݣ������������ѯ��Ʊ����Ϊ������ʱ��ֻ�Ǽ��Թ������������ݡ��ʶ���ѯ900����Ʊ�����ص�������ҪС��900.
// ֻ�е����еĲ�ѯ��Ʊ��Ϊ������ʱ���ŷ���һ��21���ַ�����v_pv_none_match=\"1\";\n
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskProcessWebRTDataGetFromTengxunServer(void) {
  CWebDataPtr pWebDataReceived = nullptr;
  int j = 0;

  size_t lTotalData = gl_WebInquirer.GetTengxunRTDataSize();
  for (size_t i = 0; i < lTotalData; i++) {
    pWebDataReceived = gl_WebInquirer.PopTengxunRTData();
    pWebDataReceived->ResetCurrentPos();
    if (!IsInvalidTengxunRTData(pWebDataReceived)) { // ������21���ַ����ĺ������Է������Ҳ���Է�����ǰ�档
      j = 0;
      while (pWebDataReceived->GetCurrentPos() < pWebDataReceived->GetBufferLength()) {
        CRTDataPtr pRTData = make_shared<CRTData>();
        if (pRTData->ReadTengxunData(pWebDataReceived)) {
          CheckTengxunRTData(pRTData); // ���һ��
          j++;
          gl_RTDataContainer.PushTengxunRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
        }
        else return false;  // ��������ݳ����⣬�׵����á�
      }
      //TRACE(_T("���յ�%d����Ѷʵʱ����\n"), j);
    }
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����б���ѯ�Ĺ�Ʊ��Ϊ�����й�Ʊʱ����Ѷʵʱ��Ʊ�������᷵��һ��21���ַ������ַ�����v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::IsInvalidTengxunRTData(CWebDataPtr pWebDataReceived) {
  char buffer[50];
  CString strInvalidStock = _T("v_pv_none_match=\"1\";\n"); // ��Ϊ��Ч��Ʊ��ѯ�������ݸ�ʽ����21���ַ�

  strncpy_s(buffer, pWebDataReceived->m_pCurrentPos, 21);
  buffer[21] = 0x000;
  CString str1 = buffer;

  if (str1.Compare(strInvalidStock) == 0) {
    ASSERT(pWebDataReceived->GetBufferLength() == 21);
    return true;
  }
  else return false;
}

void CChinaMarket::CheckTengxunRTData(CRTDataPtr pRTData) {
  CString str;
  ASSERT(pRTData->GetDataSource() == __TENGXUN_RT_WEB_DATA__);
  if (pRTData->IsActive()) {
    CChinaStockPtr pStock = nullptr;
    if ((pStock = GetStock(pRTData->GetStockCode())) != nullptr) {
      if (!pStock->IsActive()) {
        str = pStock->GetStockCode();
        str += _T("��Ѷʵʱ��⵽�����ڻ�Ծ״̬");
        //gl_systemMessage.PushInnerSystemInformationMessage(str);
      }
    }
    else {
      str = pRTData->GetStockCode();
      str += _T("��Ч��Ʊ���루��Ѷʵʱ���ݣ�");
      gl_systemMessage.PushInnerSystemInformationMessage(str);
    }
  }
}

bool CChinaMarket::TaskProcessTengxunRTData(void) {
  CRTDataPtr pRTData = nullptr;
  size_t lTotalData = gl_RTDataContainer.GetTengxunRTDataSize();

  for (size_t i = 0; i < lTotalData; i++) {
    pRTData = gl_RTDataContainer.PopTengxunRTData();
    if (pRTData->IsActive()) {
      auto pStock = GetStock(pRTData->GetStockCode());
      pStock->SetTotalValue(pRTData->GetTotalValue());
      pStock->SetCurrentValue(pRTData->GetCurrentValue());
      pStock->SetHighLimit(pRTData->GetHighLimit());
      pStock->SetLowLimit(pRTData->GetLowLimit());
    }
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//
// ��Լÿ100�������һ��
//
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  // ץȡʵʱ����(���ˡ���Ѷ�����ף���ÿ400��������һ�Σ����ɱ�֤��3�����ڱ���һ��ȫ���Ծ��Ʊ��
  if (m_fGetRTData && (m_iCountDownSlowReadingRTData <= 0)) {
    TaskGetRTDataFromWeb();
    TaskProcessWebRTDataGetFromSinaServer();
    TaskProcessWebRTDataGetFromNeteaseServer();
    // ���Ҫ�����ٶ�ȡʵʱ���ݣ������ö�ȡ����Ϊÿ����һ��
    if (!m_fStartReceivingData && IsSystemReady()) m_iCountDownSlowReadingRTData = 300; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
    else m_iCountDownSlowReadingRTData = 3;  // ����4��,��ÿ400��������һ��ʵʱ����
  }
  m_iCountDownSlowReadingRTData--;

  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (GetMarketTime() > s_timeLast) {
    SchedulingTaskPerSecond(GetMarketTime() - s_timeLast);
    s_timeLast = GetMarketTime();
  }

  // ϵͳ׼������֮����Ҫ��ɵĸ����
  if (IsSystemReady()) {
    if (!m_fTodayTempDataLoaded) { // �˹���������һ�Ρ�
      LoadTodayTempDB();
      m_fTodayTempDataLoaded = true;
    }
    TaskProcessWebRTDataGetFromTengxunServer();
    TaskGetNeteaseDayLineFromWeb();
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////
//
// �����ˡ����׻�����Ѷʵʱ�������ݷ�������ȡʵʱ���ݡ�ʹ������֮һ���ɡ�
//
/////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskGetRTDataFromWeb(void) {
  switch (m_iRTDataServer) {
  case 0: // ʹ������ʵʱ���ݷ�����
  if (IsUsingSinaRTDataReceiver()) {
    gl_WebInquirer.GetSinaRTData(); // ÿ400����(100X4)����һ��ʵʱ���ݡ����˵�ʵʱ�����������Ӧʱ�䲻����100���루30-70֮�䣩����û�г��ֹ����ݴ���
  }
  break;
  case 1: // ʹ������ʵʱ���ݷ�����
    // ����ʵʱ�����д�����ȱʧ�ֶΣ���ǰ׺��׺Ҳ��ʱȱʧ��
    // ����ʵʱ������ʱ������û��Ҫ����Ĺ�Ʊ����֪Ϊ�Ρ�
  if (IsUsingNeteaseRTDataReceiver()) {
    // ��ȡ����ʵʱ�������ݡ���������ʵʱ���������˵�����Դ��ͬ���ʶ����߿ɻ�����ʹ����һ���ɡ�
    gl_WebInquirer.GetNeteaseRTData();
  }
  break;
  default: // ����
  break;
  }

  if (IsSystemReady()) {
    // ��ȡ��Ѷʵʱ�������ݡ� ������Ѷʵʱ����Ĺ�������Ϊ�֣�û�������Ϣ�������޷�������ʵʱ�������ݶԽӣ����˾���Ϊ�ɣ����ʶ���ʱ����
    if (IsUsingTengxunRTDataReceiver()) {
      if (m_iCountDownTengxunNumber <= 0) {
        gl_WebInquirer.GetTengxunRTData();// ֻ�е�ϵͳ׼����Ϻ󣬷���ִ�ж�ȡ��Ѷʵʱ�������ݵĹ�����Ŀǰ��ʹ�ô˹���
        m_iCountDownTengxunNumber = 5;
      }
      else m_iCountDownTengxunNumber--; // ����ʵʱ���ݶ�ȡ��Σ���Ѷ�Ŷ�ȡһ�Ρ���Ϊ��Ѷ�Ĺҵ��������õ���ÿ�ֱ�׼�����Ȳ���
    }
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ʱ���Ⱥ�����ÿ��һ�Ρ�
//
// ��������֮���п��ܳ��ֻ����������洢��ʱʵʱ���ݵĹ����߳������ʵʱ���ݵĹ����߳�֮��Ͳ�����ͬʱ���У�
// �ʶ����еĶ�ʱ����Ҫ����ʱ�����ӳ��������У�����ִ��ÿ����һ�ε�������ִ��ÿ����һ�ε����������ܹ���֤���������������ִ�С�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::SchedulingTaskPerSecond(long lSecondNumber) {
  static int s_iCountDownProcessWebRTData = 0;
  const long lCurrentTime = GetFormatedMarketTime();

  // �����ȳ��򰴼��ʱ���С˳�����У����ʱ�䳤�ı���λ�ڼ��ʱ��̵�֮ǰ��
  SchedulingTaskPerHour(lSecondNumber, lCurrentTime);
  SchedulingTaskPer5Minutes(lSecondNumber, lCurrentTime);
  SchedulingTaskPerMinute(lSecondNumber, lCurrentTime);
  SchedulingTaskPer10Seconds(lSecondNumber, lCurrentTime);

  CheckMarketReady(); // ����г��Ƿ���ɳ�ʼ��

  if ((GetDayLineNeedUpdateNumber() <= 0) && (GetDayLineNeedSaveNumber() <= 0)) {
    TaskChoice10RSStrongStockSet(lCurrentTime);
    TaskChoice10RSStrong1StockSet(lCurrentTime);
    TaskChoice10RSStrong2StockSet(lCurrentTime);
  }

  // �ж��Ƿ�ʼ�����ռ�����
  TaskCheckStartReceivingData(lCurrentTime);
  // �ж��й���Ʊ�г�����״̬
  TaskCheckMarketOpen(lCurrentTime);

  if (s_iCountDownProcessWebRTData <= 0) {
    // �����յ���ʵʱ���ݷַ�������ع�Ʊ��ʵʱ���ݶ����С�
    // �����ж������Դ���ʶ���Ҫ�ȴ�������Դ��ִ��һ�κ󣬷����Էַ�����ع�Ʊ�����ʶ���Ҫÿ����ִ��һ�Σ��Ա�֤������Դ���ٶ����ṩһ�����ݡ�
    TaskDistributeSinaRTDataToProperStock();
    // �ַ�����ʵʱ����������ع�Ʊ�С�
    TaskDistributeNeteaseRTDataToProperStock();

    TaskProcessTengxunRTData();

    TaskDiscardNeteaseRTData();
    TaskDiscardSinaRTData();
    TaskDiscardTengxunRTData();
    s_iCountDownProcessWebRTData = 0;
  }
  else s_iCountDownProcessWebRTData--;

  // ����ʵʱ���ݣ�ÿ����һ�Ρ�Ŀǰ����ʵʱ����Ϊÿ3����һ�θ��£��ʶ������ٿ��ˡ�
  // �˼�������Ҫ��DistributeRTDataReceivedFromWebToProperStock֮��ִ�У��Է�ֹ����ͬ�����⡣
  // ��ϵͳ�洢��ʱ����ʱ����ͬʱ����ʵʱ���ݣ��������׳���ͬ�����⡣���ϵͳ���ڴ洢��ʱʵʱ���ݣ���ȴ�һ������һ����ѯʱ�ټ���ʵʱ����
  if (IsSystemReady() && !gl_ThreadStatus.IsSavingTempData() && IsTodayTempRTDataLoaded()) {
    if (gl_ThreadStatus.IsRTDataNeedCalculate()) {
      gl_ThreadStatus.SetCalculatingRTData(true);
      TaskProcessRTData();
      gl_ThreadStatus.SetRTDataNeedCalculate(false);
      gl_ThreadStatus.SetCalculatingRTData(false);
    }
  }

  TaskShowCurrentTransaction();

  // װ�ص�ǰ��Ʊ��������
  TaskLoadCurrentStockDayLine();

  return true;
}

bool CChinaMarket::SchedulingTaskPerHour(long lSecondNumber, long lCurrentTime) {
  static int i1HourCounter = 3599; // һСʱһ�εļ�����

  // ����ÿһСʱһ�ε�����
  i1HourCounter -= lSecondNumber;
  if (i1HourCounter < 0) {
    i1HourCounter = 3599;

    return true;
  }
  else
    return false;
}

bool CChinaMarket::SchedulingTaskPer5Minutes(long lSecondNumber, long lCurrentTime) {
  static int i5MinuteCounter = 299; // �����һ�εļ�����

  // ����ÿ�����һ�ε�����
  i5MinuteCounter -= lSecondNumber;
  if (i5MinuteCounter < 0) {
    i5MinuteCounter = 299;

    if (IsSavingTempData()) {
      TaskSaveTempDataIntoDB(lCurrentTime);
    }

    return true;
  } // ÿ�����һ�ε�����
  else {
    return false;
  }
}

void CChinaMarket::TaskSaveTempDataIntoDB(long lCurrentTime) {
  // ����ʱÿ����Ӵ洢һ�ε�ǰ״̬������һ�����ô�ʩ����ֹ�˳�ϵͳ��Ͷ��������е����ݣ�����̫Ƶ����
  if (m_fSystemReady && m_fMarketOpened && !gl_ThreadStatus.IsCalculatingRTData()) {
    if (((lCurrentTime > 93000) && (lCurrentTime < 113600)) || ((lCurrentTime > 130000) && (lCurrentTime < 150600))) { // �洢��ʱ�����ϸ��ս���ʱ����ȷ��(�м������ڼ�ͱ��к��Ҫ�洢һ�Σ��ʶ���11:36��15:06����ֹ��
      CString str;
      str = _T("�洢��ʱ����");
      gl_systemMessage.PushDayLineInfoMessage(str);
      UpdateTempRTData();
    }
  }
}

bool CChinaMarket::AddChoicedStock(CChinaStockPtr pStock) {
  auto it = find(m_avChoicedStock[0].cbegin(), m_avChoicedStock[0].cend(), pStock);
  if (it == m_avChoicedStock[0].end()) {
    m_avChoicedStock[0].push_back(pStock);
    ASSERT(!pStock->IsSaveToChoicedStockDB());
    return true;
  }
  return false;
}

bool CChinaMarket::DeleteChoicedStock(CChinaStockPtr pStock) {
  auto it = find(m_avChoicedStock[0].cbegin(), m_avChoicedStock[0].cend(), pStock);
  if (it == m_avChoicedStock[0].end()) {
    return false;
  }
  else {
    (*it)->SetChoiced(false);
    (*it)->SetSaveToChoicedStockDB(false);
    m_avChoicedStock[0].erase(it);
    return true;
  }
}

void CChinaMarket::ClearDayLineNeedUpdaeStatus(void) {
  for (auto pStock : m_vChinaMarketStock) {
    if (pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(false);
  }
}

void CChinaMarket::IncreaseNeteaseDayLineNeedUpdateNumber(int iNumber) {
  ASSERT(m_iDayLineNeedUpdate < 12000);
  m_iDayLineNeedUpdate += iNumber;
}

void CChinaMarket::DecreaseNeteaseDayLineNeedUpdateNumber(int iNumber) {
  ASSERT(m_iDayLineNeedUpdate > 0);
  if (m_iDayLineNeedUpdate >= iNumber) m_iDayLineNeedUpdate -= iNumber;
}

bool CChinaMarket::SchedulingTaskPerMinute(long lSecondNumber, long lCurrentTime) {
  static int i1MinuteCounter = 59;  // һ����һ�εļ�����

  // ����ÿ����һ�ε��������еĶ�ʱ����Ҫ����ʱ�����ӳ��������У�����ִ��ÿ����һ�ε�������ִ��ÿ����һ�ε����������ܹ���֤���������������ִ�С�
  i1MinuteCounter -= lSecondNumber;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 59; // ���ü�����

    TaskResetMarket(lCurrentTime);
    TaskResetMarketAgain(lCurrentTime);

    // �ڿ���ǰ��������ͣʱ��ѯ���й�Ʊ�أ��ҵ������Ծ��Ʊ��
    TaskSetCheckActiveStockFlag(lCurrentTime);

    // �����������ֿ�ʼ������ʵʱ���ݡ�
    TaskProcessTodayStock(lCurrentTime);

    TaskSaveChoicedRTData();

    TaskClearChoicedRTDataSet(lCurrentTime);

    TaskUpdateStockCodeDB();
    TaskUpdateOptionDB();
    TaskUpdateChoicedStockDB();

    TaskCheckDayLineDB();

    return true;
  } // ÿһ����һ�ε�����
  else {
    return false;
  }
}

bool CChinaMarket::TaskSetCheckActiveStockFlag(long lCurrentTime) {
  if (((lCurrentTime >= 91500) && (lCurrentTime < 92900)) || ((lCurrentTime >= 113100) && (lCurrentTime < 125900))) {
    m_fCheckActiveStock = true;
    return true;
  }
  else {
    m_fCheckActiveStock = false;
    return false;
  }
}

bool CChinaMarket::TaskChoice10RSStrong1StockSet(long lCurrentTime) {
  if (IsSystemReady() && !m_fChoiced10RSStrong1StockSet && (lCurrentTime > 151100) && IsWorkingDay()) {
    RunningThreadChoice10RSStrong1StockSet();
    m_fChoiced10RSStrong1StockSet = true;
    return true;
  }
  return false;
}

bool CChinaMarket::TaskChoice10RSStrong2StockSet(long lCurrentTime) {
  if (IsSystemReady() && !m_fChoiced10RSStrong2StockSet && (lCurrentTime > 151200) && IsWorkingDay()) {
    RunningThreadChoice10RSStrong2StockSet();
    m_fChoiced10RSStrong2StockSet = true;
    return true;
  }
  return false;
}

bool CChinaMarket::TaskChoice10RSStrongStockSet(long lCurrentTime) {
  if (IsSystemReady() && !m_fChoiced10RSStrongStockSet && (lCurrentTime > 151000) && IsWorkingDay()) {
    RunningThreadChoice10RSStrongStockSet();
    m_fChoiced10RSStrongStockSet = true;
    return true;
  }
  return false;
}

bool CChinaMarket::TaskProcessTodayStock(long lCurrentTime) {
  if (IsSystemReady() && (lCurrentTime >= 150400) && !IsTodayStockProcessed() && IsWorkingDay()) {
    RunningThreadProcessTodayStock();
    SetTodayStockProcessed(true);
    return true;
  }
  return false;
}

bool CChinaMarket::TaskCheckDayLineDB(void) {
  if (m_fSaveDayLine && (m_iDayLineNeedSave <= 0) && (m_iDayLineNeedUpdate <= 0) && (m_iDayLineNeedProcess <= 0)) {
    if ((m_iDayLineNeedSave < 0) || (m_iDayLineNeedUpdate < 0) || (m_iDayLineNeedProcess < 0)) {
      gl_systemMessage.PushInnerSystemInformationMessage(_T("������ʷ���ݴ�������г�����覴�"));
    }
    m_fSaveDayLine = false;
    TRACE("������ʷ���ݸ������\n");
    CString str;
    str = _T("������ʷ���ݸ������");
    gl_systemMessage.PushInformationMessage(str);
    if (IsDayLineDBUpdated()) {
      // ���¹�Ʊ�����ݿ�
      SetUpdateStockCodeDB(true);
      ClearDayLineDBUpdatedFlag();
    }
  }
  return true;
}

bool CChinaMarket::TaskCheckStartReceivingData(long lCurrentTime) {
  if (!IsWorkingDay()) { //�����������ձ��С��ṹtm��0--6��ʾ��������������
    m_fStartReceivingData = false;
    return(m_fStartReceivingData);
  }
  else if ((lCurrentTime < 91200) || (lCurrentTime > 150630) || ((lCurrentTime > 113500) && (lCurrentTime < 125500))) { //��������������ʮ���г����׽�����Ϊ�˱�֤���һ����ʱ���ݵĴ洢��
    m_fStartReceivingData = false;

    return(m_fStartReceivingData);
  }
  else m_fStartReceivingData = true;
  return m_fStartReceivingData;
}

bool CChinaMarket::TaskCheckMarketOpen(long lCurrentTime) {
  if (!IsWorkingDay()) { //�����������ձ��С��ṹtm��0--6��ʾ��������������
    m_fMarketOpened = false;
  }
  else if ((lCurrentTime > 92800) && (lCurrentTime < 150600)) { // �г������������ݵ�ʱ�䣬�Զ�Ϊ150600����ֹͣ�洢��ʱ���ݵ�ʱ��һ����
    m_fMarketOpened = true;
  }
  else m_fMarketOpened = false;

  return m_fMarketOpened;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���������ÿ���ӵ���һ�Σ�����ʵ�ֻ��Ʋ�����
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskResetMarket(long lCurrentTime) {
  // �ŵ�ʮ��������ϵͳ
// �����ڴ�ʱ������������������Ļ����׳������ݲ�ȫ�����⡣
  if (IsPermitResetMarket()) { // �����������ϵͳ
    if ((lCurrentTime >= 91300) && (lCurrentTime < 91400) && IsWorkingDay()) { // �����վŵ�ʮ�������ϵͳ
      if (!TooManyStocksNeedUpdated()) { // ���й�������Ϊ�ݼ��պ����е��������ݶ���Ҫ���һ�飬��ʱ����0915ʱ����ϵͳ�Ա���������ߺ�������ִ�С�
        SetResetMarket(true);// ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
        m_fSystemReady = false;
      }
    }
  }
  return true;
}

bool CChinaMarket::TaskResetMarketAgain(long lCurrentTime) {
  // �ŵ��ʮ����ٴ�����ϵͳ
  if (IsPermitResetMarket()) { // �����������ϵͳ
    if ((lCurrentTime >= 92500)) {
      if ((lCurrentTime <= 93000) && IsWorkingDay()) { // �����վŵ��ʮ����ٴ�����ϵͳ
        SetResetMarket(true);// ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
        m_fSystemReady = false;
      }
      SetPermitResetMarket(false); // ���첻����������ϵͳ��
    }
  }
  return true;
}

bool CChinaMarket::TaskUpdateStockCodeDB(void) {
  if (IsUpdateStockCodeDB()) {
    RunningThreadUpdateStockCodeDB();
    SetUpdateStockCodeDB(false);
    return true;
  }
  return false;
}

bool CChinaMarket::TaskUpdateOptionDB(void) {
  if (IsUpdateOptionDB()) {
    RunningThreadUpdateOptionDB();
    SetUpdateOptionDB(false);
    return true;
  }
  return false;
}

bool CChinaMarket::TaskUpdateChoicedStockDB(void) {
  if (IsUpdateChoicedStockDB()) {
    RunningThreadAppendChoicedStockDB();
    return true;
  }
  return false;
}

bool CChinaMarket::TaskShowCurrentTransaction(void) {
  // ��ʾ��ǰ�������
  CChinaStockPtr pCurrentStock = GetCurrentStock();

  if (pCurrentStock != nullptr) {
    if (pCurrentStock->IsRTDataCalculated()) {
      if (pCurrentStock->GetCurrentTransationVolume() > 0) pCurrentStock->ReportGuadanTransaction();
      pCurrentStock->ReportGuadan();
      pCurrentStock->SetRTDataCalculated(false);
    }
  }

  return true;
}

bool CChinaMarket::TaskSaveChoicedRTData(void) {
  if (IsSystemReady() && m_fSaveRTData) {
    RunningThreadSaveChoicedRTData();
    return true;
  }
  else return false;
}

/////////////////////////////////////////////////////////////////////
//
// �ŵ��ʮ������ŵ���ʮ����������յ�ʵʱ���ݡ�
//
/////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskClearChoicedRTDataSet(long lCurrentTime) {
  if (lCurrentTime > 93100) {
    m_fRTDataSetCleared = true;
  }

  if (!m_fRTDataSetCleared) {
    if ((lCurrentTime > 92900) && (lCurrentTime < 93100)) {
      CSetRealTimeData setRTData;
      setRTData.Open();
      setRTData.m_pDatabase->BeginTrans();
      while (!setRTData.IsEOF()) {
        setRTData.Delete();
        setRTData.MoveNext();
      }
      setRTData.m_pDatabase->CommitTrans();
      setRTData.Close();

      m_fRTDataSetCleared = true;
    }
  }
  return true;
}

bool CChinaMarket::SchedulingTaskPer10Seconds(long lSecondNumber, long lCurrentTime) {
  static int i10SecondsCounter = 9; // ʮ��һ�εļ�����

  // ����ÿʮ����һ�ε�����
  i10SecondsCounter -= lSecondNumber;
  if (i10SecondsCounter < 0) {
    i10SecondsCounter = 9;
    // do something

    // ������������ʷ���ݵĺ�����Ϊ��ʱ��ѯ����ȡ�ʹ洢���ù������̡�
    if (m_iDayLineNeedProcess > 0) {
      TaskProcessDayLineGetFromNeeteaseServer();
    }

    // �ж��Ƿ�洢���߿�͹�Ʊ�����
    if ((m_iDayLineNeedSave > 0)) {
      m_fSaveDayLine = true;
      TaskSaveDayLineData();
    }
    TaskCheckDayLineDB();
    return true;
  } // ÿʮ����һ�ε�����
  else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �õ���Ʊ�ļ��
//
//////////////////////////////////////////////////////////////////////////////////////////////
CString CChinaMarket::GetStockName(CString strStockCode) {
  try {
    return (m_vChinaMarketStock.at(m_mapChinaMarketAStock.at(strStockCode))->GetStockName());
  }
  catch (exception&) {
    TRACE("GetStockName�����쳣\n");
    return _T("");
  }
}

////////////////////////////////////////////////////////////////////////////////
//
//	ͨ����Ʊ����õ���Ʊ��������
//	���û�ҵ��Ļ�����ֵΪ�١�
//
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::GetStockIndex(CString strStockCode, long& lIndex) {
  try {
    lIndex = m_mapChinaMarketAStock.at(strStockCode);
    return true;
  }
  catch (exception&) {
    TRACE("GetStockIndexԽ��\n");
    lIndex = -1;
    return false;
  }
}

////////////////////////////////////////////////////////////////////////////////
//
//	ͨ����Ʊ����õ���Ʊ��ָ�롣
//	���û�ҵ��Ļ����ؿ�ָ��
//
//
////////////////////////////////////////////////////////////////////////////////
CChinaStockPtr CChinaMarket::GetStock(CString strStockCode) {
  try {
    return (m_vChinaMarketStock.at(m_mapChinaMarketAStock.at(strStockCode)));
  }
  catch (exception&) {
    TRACE("GetStockԽ��, StockCode = %s\n", strStockCode.GetBuffer());
    return nullptr;
  }
}

CChinaStockPtr CChinaMarket::GetStock(long lIndex) {
  try {
    return m_vChinaMarketStock.at(lIndex);
  }
  catch (exception&) {
    TRACE("GetStockԽ��\n");
    return nullptr;
  }
}

void CChinaMarket::IncreaseActiveStockNumber(void) {
  m_lTotalActiveStock++;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//	ͨ����Ʊ������г��������õ�ǰѡ���Ʊ
//
//////////////////////////////////////////////////////////////////////////////////////
void CChinaMarket::SetCurrentStock(CString strStockCode) {
  CChinaStockPtr pStock = GetStock(strStockCode);
  SetCurrentStock(pStock);
  ASSERT(m_pCurrentStock != NULL);
}

//////////////////////////////////////////////////////////////////////////
//
// ���õ�ǰ�����Ĺ�Ʊ
//
// ������Ӧ�Ĺ�Ʊָ�룬װ�����������ݡ�
//
/////////////////////////////////////////////////////////////////////////
void CChinaMarket::SetCurrentStock(CChinaStockPtr pStock) {
  bool fSet = false;
  if (m_pCurrentStock != nullptr) {
    if (!m_pCurrentStock->IsSameStock(pStock)) {
      m_pCurrentStock->SetRecordRTData(false);
      fSet = true;
    }
  }
  else {
    fSet = true;
  }
  if (fSet) {
    pStock->SetRecordRTData(true);
    m_pCurrentStock = pStock;
    SetCurrentStockChanged(true);
    m_pCurrentStock->SetDayLineLoaded(false); // ����ֻ�����ñ�ʶ��ʵ��װ�������ɵ��ȳ���ִ�С�
  }
}

void CChinaMarket::ResetCurrentStock(void) {
  if (m_pCurrentStock != nullptr) {
    m_pCurrentStock->SetRecordRTData(false);
    m_pCurrentStock = nullptr;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	���������ݴ������ݿ⣮
//  �˺����ɹ����߳�ThreadDayLineSaveProc���ã�������Ҫʹ��ȫ�ֱ�����(Ŀǰʹ�����̵߳���֮��������ͬ������ĳ��֣�
//
// �����Ƿ�ִ���˴洢����������Ҫ�����ص�������ʷ����ɾ���������ܹ���ʡ�ڴ��ռ�á�����ʵ�ʴ洢����ʹ���߳�ģʽʵ�֣�
// �ʶ���ִ��ʱ������������̣߳��������߳�ɾ����������ʱ����ͬ�����⡣����ķ������ù����̶߳���ɾ���洢����������ݣ�
// ���̵߳�ɾ������ֻ�ڲ����ù����̣߳�����洢�������ݣ�������·���ִ�С�
//////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskSaveDayLineData(void) {
  CString str;

  for (auto pStock : m_vChinaMarketStock) {
    if (pStock->IsDayLineNeedSavingAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
      if (pStock->GetDayLineSize() > 0) {
        if (pStock->HaveNewDayLineData()) {
          RunningThreadSaveDayLineOfOneStock(pStock);
        }
        else pStock->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
      }
      else { // �������Ϊ�й�Ʊ���룬���˴�����δ����
        CString str1 = pStock->GetStockCode();
        str1 += _T(" Ϊδ���й�Ʊ����");
        gl_systemMessage.PushDayLineInfoMessage(str1);
      }
    }
    if (gl_fExitingSystem) {
      break; // ������������˳�����ֹͣ�洢��
    }
  }

  return(true);
}

bool CChinaMarket::UnloadDayLine(void) {
  for (auto pStock : m_vChinaMarketStock) {
    pStock->UnloadDayLine();
  }

  return true;
}

bool CChinaMarket::BuildWeekLine(void) {
  gl_systemMessage.PushInformationMessage(_T("��������������ʷ����"));
  for (auto pStock : m_vChinaMarketStock) {
    RunningThreadBuildWeekLineOfStock(pStock);
  }
  while (gl_ThreadStatus.HowManyBackGroundThreadsWorking() > 0) {
    Sleep(1000);
  }
  gl_systemMessage.PushInformationMessage(_T("������ʷ�����������"));

  return true;
}

CChinaStockPtr CChinaMarket::GetCurrentSelectedStock(void) {
  if (m_lCurrentSelectedStockSet >= 0) {
    return m_avChoicedStock[m_lCurrentSelectedStockSet][0];
  }
  else {
    return GetStock(0);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��ʵʱ���ݴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::SaveRTData(void) {
  CSetRealTimeData setRTData;
  CRTDataPtr pRTData = nullptr;
  size_t lTotal = m_qRTData.size();

  if (lTotal > 0) {
    setRTData.m_strFilter = _T("[ID] = 1");
    setRTData.Open();
    setRTData.m_pDatabase->BeginTrans();
    for (size_t i = 0; i < lTotal; i++) {
      pRTData = m_qRTData.front();
      m_qRTData.pop(); // �׵���ǰ���������
      pRTData->AppendData(setRTData);
    }
    setRTData.m_pDatabase->CommitTrans();
    setRTData.Close();
  }
  return(true);
}

bool CChinaMarket::IsDayLineNeedSaving(void) {
  for (auto pStock : m_vChinaMarketStock) {
    if (pStock->IsDayLineNeedSaving()) {
      ASSERT(pStock->IsActive());
      return true;
    }
  }
  return false;
}

bool CChinaMarket::Choice10RSStrong2StockSet(void) {
  vector<CChinaStockPtr> v10RSStrongStock;

  for (auto pStock : m_vChinaMarketStock) {
    RunningThreadCalculate10RSStrong2Stock(&v10RSStrongStock, pStock);

    /*
    if (IsAStock(pStock) && pStock->IsActive()) {
      if (!pStock->IsDayLineLoaded()) {
        pStock->LoadDayLine();
        pStock->SetDayLineLoaded(true);
      }
      if (pStock->Calculate10RSStrong2StockSet()) {
        v10RSStrongStock.push_back(pStock);
      }
      if (!pStock->IsSameStock(m_pCurrentStock)) {
        pStock->UnloadDayLine();
      }
    }
    if (gl_fExitingSystem) return false;
    */
  }
  while (gl_ThreadStatus.HowManyBackGroundThreadsWorking() > 0) {
    if (gl_fExitingSystem) return false;
    Sleep(100); // �ȴ������߳������������
  }

  CSetRSStrong2Stock setRSStrong2;

  setRSStrong2.Open();
  setRSStrong2.m_pDatabase->BeginTrans();
  while (!setRSStrong2.IsEOF()) {
    setRSStrong2.Delete();
    setRSStrong2.MoveNext();
  }
  setRSStrong2.m_pDatabase->CommitTrans();
  setRSStrong2.m_pDatabase->BeginTrans();
  for (auto pStock : v10RSStrongStock) {
    setRSStrong2.AddNew();
    setRSStrong2.m_Market = pStock->GetMarket();
    setRSStrong2.m_StockCode = pStock->GetStockCode();
    setRSStrong2.Update();
  }
  setRSStrong2.m_pDatabase->CommitTrans();
  setRSStrong2.Close();

  return true;
}

bool CChinaMarket::Choice10RSStrong1StockSet(void) {
  vector<CChinaStockPtr> v10RSStrongStock;

  for (auto pStock : m_vChinaMarketStock) {
    RunningThreadCalculate10RSStrong1Stock(&v10RSStrongStock, pStock);
    /*
    if (IsAStock(pStock) && pStock->IsActive()) {
      if (!pStock->IsDayLineLoaded()) {
        pStock->LoadDayLine();
        pStock->SetDayLineLoaded(true);
      }
      if (pStock->Calculate10RSStrong1StockSet()) {
        v10RSStrongStock.push_back(pStock);
      }
      if (!pStock->IsSameStock(m_pCurrentStock)) {
        pStock->UnloadDayLine();
      }
    }
    if (gl_fExitingSystem) return false;
    */
  }
  while (gl_ThreadStatus.HowManyBackGroundThreadsWorking() > 0) {
    if (gl_fExitingSystem) return false;
    Sleep(100); // �ȴ������߳������������
  }

  CSetRSStrong1Stock setRSStrong1;

  setRSStrong1.Open();
  setRSStrong1.m_pDatabase->BeginTrans();
  while (!setRSStrong1.IsEOF()) {
    setRSStrong1.Delete();
    setRSStrong1.MoveNext();
  }
  setRSStrong1.m_pDatabase->CommitTrans();
  setRSStrong1.m_pDatabase->BeginTrans();
  for (auto pStock : v10RSStrongStock) {
    setRSStrong1.AddNew();
    setRSStrong1.m_Market = pStock->GetMarket();
    setRSStrong1.m_StockCode = pStock->GetStockCode();
    setRSStrong1.Update();
  }
  setRSStrong1.m_pDatabase->CommitTrans();
  setRSStrong1.Close();

  return true;
}

bool CChinaMarket::Choice10RSStrongStockSet(CRSReference* pRef, int iIndex) {
  vector<CChinaStockPtr> v10RSStrongStock;

  for (auto pStock : m_vChinaMarketStock) {
    RunningThreadCalculate10RSStrongStock(&v10RSStrongStock, pRef, pStock);
    /*
    if (IsAStock(pStock) && pStock->IsActive()) {
      if (!pStock->IsDayLineLoaded()) {
        pStock->LoadDayLine();
        pStock->SetDayLineLoaded(true);
      }
      if (pStock->Calculate10RSStrongStockSet(pRef)) {
        v10RSStrongStock.push_back(pStock);
      }
      if (!pStock->IsSameStock(m_pCurrentStock)) {
        pStock->UnloadDayLine();
      }
    }
    if (gl_fExitingSystem) return false;
    */
  }

  while (gl_ThreadStatus.HowManyBackGroundThreadsWorking() > 0) {
    if (gl_fExitingSystem) return false;
    Sleep(100); // �ȴ������߳������������
  }

  m_lCurrentRSStrongIndex = iIndex; // CSetRSStrongStock��Ҫ��m_lCurrentRSStrongIndex��ѡ����ȷ�����ݱ�
  CSetRSStrongStock setRSStrong(iIndex);

  setRSStrong.Open();
  setRSStrong.m_pDatabase->BeginTrans();
  while (!setRSStrong.IsEOF()) {
    setRSStrong.Delete();
    setRSStrong.MoveNext();
  }
  setRSStrong.m_pDatabase->CommitTrans();
  setRSStrong.m_pDatabase->BeginTrans();
  for (auto pStock : v10RSStrongStock) {
    setRSStrong.AddNew();
    setRSStrong.m_Market = pStock->GetMarket();
    setRSStrong.m_StockCode = pStock->GetStockCode();
    setRSStrong.Update();
  }
  setRSStrong.m_pDatabase->CommitTrans();
  setRSStrong.Close();

  return true;
}

bool CChinaMarket::IsDayLineNeedUpdate(void) {
  for (auto pStock : m_vChinaMarketStock) {
    if (pStock->IsDayLineNeedUpdate()) return true;
  }
  return false;
}

bool CChinaMarket::TaskProcessDayLineGetFromNeeteaseServer(void) {
  for (auto pStock : m_vChinaMarketStock) {
    if (pStock->IsDayLineNeedProcess()) {
      pStock->ProcessNeteaseDayLineData();
      pStock->SetDayLineNeedProcess(false);
    }
  }
  return true;
}

bool CChinaMarket::TaskLoadCurrentStockDayLine(void) {
  if (m_pCurrentStock != nullptr) {
    if (!m_pCurrentStock->IsDayLineLoaded()) {
      RunningThreadLoadDayLine(m_pCurrentStock);
    }
  }
  return true;
}

bool CChinaMarket::RunningThreadSaveChoicedRTData(void) {
  thread thread1(ThreadSaveRTData, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CChinaMarket::RunningThreadProcessTodayStock(void) {
  thread thread1(ThreadProcessTodayStock, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CChinaMarket::RunningThreadBuildDayLineRS(long lStartCalculatingDay) {
  thread thread1(ThreadBuildDayLineRS, this, lStartCalculatingDay);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CChinaMarket::RunningThreadBuildThisDayRS(long lThisDay) {
  thread thread_calculateRS(ThreadBuildThisDayRS, this, lThisDay);
  thread_calculateRS.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CChinaMarket::RunningThreadSaveTempRTData(void) {
  thread thread1(ThreadSaveTempRTData, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CChinaMarket::RunningThreadSaveDayLineOfOneStock(CChinaStockPtr pStock) {
  thread thread1(ThreadSaveDayLineOfOneStock, pStock);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CChinaMarket::RunningThreadLoadDayLine(CChinaStockPtr pCurrentStock) {
  thread thread1(ThreadLoadDayLine, pCurrentStock);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CChinaMarket::RunningThreadUpdateStockCodeDB(void) {
  thread thread1(ThreadUpdateStockCodeDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CChinaMarket::RunningThreadUpdateOptionDB(void) {
  thread thread1(ThreadUpdateOptionDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CChinaMarket::RunningThreadAppendChoicedStockDB(void) {
  thread thread1(ThreadAppendChoicedStockDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CChinaMarket::RunningThreadChoice10RSStrong2StockSet(void) {
  thread thread1(ThreadChoice10RSStrong2StockSet, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CChinaMarket::RunningThreadChoice10RSStrong1StockSet(void) {
  thread thread1(ThreadChoice10RSStrong1StockSet, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CChinaMarket::RunningThreadChoice10RSStrongStockSet(void) {
  for (int i = 0; i < 10; i++) {
    if (m_aRSStrongOption[i].m_fActive) {
      thread thread1(ThreadChoice10RSStrongStockSet, this, &(m_aRSStrongOption[i]), i);
      thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
    }
  }
  SetUpdatedDayFor10DayRS(GetFormatedMarketDay());
  SetUpdateOptionDB(true); // ����ѡ�����ݿ�.��ʱ���㹤���߳�ֻ�Ǹո���������Ҫʱ��ȥ��ɡ�

  return true;
}

bool CChinaMarket::RunningThreadCalculate10RSStrong1Stock(vector<CChinaStockPtr>* pv10RSStrongStock, CChinaStockPtr pStock) {
  thread thread1(ThreadCalculate10RSStrong1Stock, pv10RSStrongStock, pStock);
  thread1.detach();

  return false;
}

bool CChinaMarket::RunningThreadCalculate10RSStrong2Stock(vector<CChinaStockPtr>* pv10RSStrongStock, CChinaStockPtr pStock) {
  thread thread1(ThreadCalculate10RSStrong2Stock, pv10RSStrongStock, pStock);
  thread1.detach();

  return false;
}

bool CChinaMarket::RunningThreadBuildWeekLine(void) {
  thread thread1(ThreadBuildWeekLine, this);
  thread1.detach();

  return true;
}

bool CChinaMarket::RunningThreadBuildWeekLineOfStock(CChinaStockPtr pStock) {
  thread thread1(ThreadBuildWeekLineOfStock, pStock);
  thread1.detach();

  return true;
}

bool CChinaMarket::RunningThreadBuildWeekLineRS(void) {
  thread thread1(ThreadBuildWeekLineRS, this);
  thread1.detach();

  return true;
}

bool CChinaMarket::RunningThreadCalculate10RSStrongStock(vector<CChinaStockPtr>* pv10RSStrongStock, CRSReference* pRef, CChinaStockPtr pStock) {
  thread thread1(ThreadCalculate10RSStrongStock, pv10RSStrongStock, pRef, pStock);
  thread1.detach();

  return false;
}

//////////////////////////////////////////////////////////////////////////////////
//
// ����ǰ�����յ�ʵʱ���ݣ��������߸��������ݣ����ǿ�ȡ������������̣���
//
// ֻ��������������ʷ���ݺ󣬷���ִ�д���ʵʱ���ݣ�����������й�Ʊ����������
//
// long lCurrentTradeDay ��ǰ�����ա����ڴ������������գ��ʶ������ڲ���һ�����ǵ�ǰ���ڣ�������ʱ����
//
//////////////////////////////////////////////////////////////////////////////////
long CChinaMarket::ProcessCurrentTradeDayStock(long lCurrentTradeDay) {
  char buffer[20];
  CString strDay;
  CSetDayLineBasicInfo setDayLineBasicInfo;
  CSetDayLineExtendInfo setDayLineExtendInfo;
  long iCount = 0;

  CString str;
  str = _T("��ʼ����");
  str += GetDayString(lCurrentTradeDay);
  str += _T("�յ�ʵʱ����");
  gl_systemMessage.PushInformationMessage(str);

  // �洢��ǰ�����յ�����
  _ltoa_s(lCurrentTradeDay, buffer, 10);
  strDay = buffer;
  setDayLineBasicInfo.m_strFilter = _T("[Day] =");
  setDayLineBasicInfo.m_strFilter += strDay;
  setDayLineBasicInfo.Open();
  setDayLineBasicInfo.m_pDatabase->BeginTrans();
  while (!setDayLineBasicInfo.IsEOF()) {
    setDayLineBasicInfo.Delete();
    setDayLineBasicInfo.MoveNext();
  }
  setDayLineBasicInfo.m_pDatabase->CommitTrans();

  setDayLineBasicInfo.m_pDatabase->BeginTrans();
  for (auto pStock : m_vChinaMarketStock) {
    if (!pStock->IsTodayDataActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    iCount++;
    pStock->SetDayLineEndDay(lCurrentTradeDay);
    pStock->SetIPOStatus(__STOCK_IPOED__); // ������һ�Ρ���ֹ�¹ɹ�Ʊ��������û����ʷ���ݶ�������Ϊ�����ڡ�
    setDayLineBasicInfo.AddNew();
    pStock->SaveTodayBasicInfo(&setDayLineBasicInfo);
    setDayLineBasicInfo.Update();
  }
  setDayLineBasicInfo.m_pDatabase->CommitTrans();
  setDayLineBasicInfo.Close();

  // �洢�������ɵ�������DayLineInfo���С�
  setDayLineExtendInfo.m_strFilter = _T("[Day] =");
  setDayLineExtendInfo.m_strFilter += strDay;
  setDayLineExtendInfo.Open();
  setDayLineExtendInfo.m_pDatabase->BeginTrans();
  while (!setDayLineExtendInfo.IsEOF()) {
    setDayLineExtendInfo.Delete();
    setDayLineExtendInfo.MoveNext();
  }
  setDayLineExtendInfo.m_pDatabase->CommitTrans();

  setDayLineExtendInfo.m_pDatabase->BeginTrans();
  for (auto pStock : m_vChinaMarketStock) {
    if (!pStock->IsTodayDataActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    setDayLineExtendInfo.AddNew();
    pStock->SaveTodayExtendInfo(&setDayLineExtendInfo);
    setDayLineExtendInfo.Update();
  }
  setDayLineExtendInfo.m_pDatabase->CommitTrans();
  setDayLineExtendInfo.Close();

  str = GetDayString(lCurrentTradeDay);
  str += _T("��ʵʱ���ݴ������");
  gl_systemMessage.PushInformationMessage(str);
  return iCount;
}

//////////////////////////////////////////////////////////////////////////////////
//
// �����մ���õ����ݴ��������ݿ��У��Ա���һϵͳ����ʱ����װ�롣
// �ƺ�Ӧ����һ����ʱ�����̵߳���ʽ���ڡ�
//
// �о�֮��
//
//////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::UpdateTodayTempDB(void) {
  CSetDayLineToday setDayLineToday;
  CString str;
  // �洢�������ɵ�������DayLineToday���С�
  setDayLineToday.Open();
  setDayLineToday.m_pDatabase->BeginTrans();
  while (!setDayLineToday.IsEOF()) {
    setDayLineToday.Delete();
    setDayLineToday.MoveNext();
  }
  setDayLineToday.m_pDatabase->CommitTrans();
  setDayLineToday.m_pDatabase->BeginTrans();
  for (auto pStock : m_vChinaMarketStock) {
    if (!pStock->IsTodayDataActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    if (pStock->IsNeedProcessRTData() && (!pStock->IsVolumeConsistence())) {
      str = pStock->GetStockCode();
      str += _T(" ��������ȷ");
      gl_systemMessage.PushInnerSystemInformationMessage(str);
    }
    setDayLineToday.AddNew();
    pStock->SaveTempInfo(setDayLineToday);
    setDayLineToday.Update();
  }
  setDayLineToday.m_pDatabase->CommitTrans();
  setDayLineToday.Close();

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// �����ݴ�Ľ��յ�ǰ����״̬�����÷����ĳ�ʼ̬���������ڿ���ʱϵͳ�˳�ʱ����������ʧ�������ѷ���������
//
// ������m_lUnknownVolumeΪ��¼���е�m_UnknownVolume - m_Volume��������Ϊ��һ�μ���ʱֻ�ǳ�ʼ��ϵͳ��
// ��Ҫ����m_lUnknownVolume = pRTData->m_lVolume - setDayLineToday.m_Volume + setDayLineToday.m_UnknownVolume
// ����һ��ִ�м���ʵʱ����ʱ��ֻ�ǳ�ʼ��ϵͳ��������������m_lUnknownVolume += pRTData->GetVolume
// �ʶ��˴��������㡣
/////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::LoadTodayTempDB(void) {
  CChinaStockPtr pStock = nullptr;
  CSetDayLineToday setDayLineToday;
  CRTDataPtr pRTData;

  ASSERT(!m_fTodayTempDataLoaded);
  ASSERT(!gl_ThreadStatus.IsCalculatingRTData());    // ִ�д˳�ʼ������ʱ������ʵʱ���ݵĹ����̱߳���û�����С�
  // ��ȡ�������ɵ�������DayLineToday���С�
  setDayLineToday.Open();
  if (!setDayLineToday.IsEOF()) {
    if (setDayLineToday.m_Day == GetFormatedMarketDay()) { // ����ǵ�������飬�����룬�������
      while (!setDayLineToday.IsEOF()) {
        if ((pStock = GetStock(setDayLineToday.m_StockCode)) != nullptr) {
          ASSERT(!pStock->HaveFirstRTData()); // ȷ��û�п�ʼ����ʵʱ����
          pStock->LoadTempInfo(setDayLineToday);
        }
        setDayLineToday.MoveNext();
      }
    }
  }
  setDayLineToday.Close();

  return true;
}

bool CChinaMarket::Load10DayRSStrong1StockSet(void) {
  CSetRSStrong1Stock setRSStrong1;
  CChinaStockPtr pStock = nullptr;

  m_v10RSStrong1Stock.clear();
  setRSStrong1.Open();
  while (!setRSStrong1.IsEOF()) {
    pStock = gl_pChinaStockMarket->GetStock(setRSStrong1.m_StockCode);
    if (pStock != nullptr) {
      m_v10RSStrong1Stock.push_back(pStock);
    }
    setRSStrong1.MoveNext();
  }
  setRSStrong1.Close();

  return true;
}

bool CChinaMarket::Load10DayRSStrong2StockSet(void) {
  CSetRSStrong2Stock setRSStrong2;
  CChinaStockPtr pStock = nullptr;

  m_v10RSStrong2Stock.clear();
  setRSStrong2.Open();
  while (!setRSStrong2.IsEOF()) {
    pStock = gl_pChinaStockMarket->GetStock(setRSStrong2.m_StockCode);
    if (pStock != nullptr) {
      m_v10RSStrong2Stock.push_back(pStock);
    }
    setRSStrong2.MoveNext();
  }
  setRSStrong2.Close();

  return true;
}

bool CChinaMarket::LoadCalculatingRSOption(void) {
  CSetRSOption setRSOption;

  setRSOption.Open();
  while (!setRSOption.IsEOF()) {
    m_aRSStrongOption[setRSOption.m_Index].m_fActive = setRSOption.m_Active;
    m_aRSStrongOption[setRSOption.m_Index].m_lDayLength[0] = setRSOption.m_DayLengthFirst;
    m_aRSStrongOption[setRSOption.m_Index].m_lDayLength[1] = setRSOption.m_DayLengthSecond;
    m_aRSStrongOption[setRSOption.m_Index].m_lDayLength[2] = setRSOption.m_DayLengthThird;
    m_aRSStrongOption[setRSOption.m_Index].m_lDayLength[3] = setRSOption.m_DayLengthFourth;
    m_aRSStrongOption[setRSOption.m_Index].m_lStrongDayLength[0] = setRSOption.m_StrongDayLengthFirst;
    m_aRSStrongOption[setRSOption.m_Index].m_lStrongDayLength[1] = setRSOption.m_StrongDayLengthSecond;
    m_aRSStrongOption[setRSOption.m_Index].m_lStrongDayLength[2] = setRSOption.m_StrongDayLengthThird;
    m_aRSStrongOption[setRSOption.m_Index].m_lStrongDayLength[3] = setRSOption.m_StrongDayLengthFourth;
    m_aRSStrongOption[setRSOption.m_Index].m_dRSStrong[0] = atof(setRSOption.m_RSStrongFirst);
    m_aRSStrongOption[setRSOption.m_Index].m_dRSStrong[1] = atof(setRSOption.m_RSStrongSecond);
    m_aRSStrongOption[setRSOption.m_Index].m_dRSStrong[2] = atof(setRSOption.m_RSStrongThird);
    m_aRSStrongOption[setRSOption.m_Index].m_dRSStrong[3] = atof(setRSOption.m_RSStrongFourth);
    setRSOption.MoveNext();
  }
  setRSOption.Close();
  return true;
}

void CChinaMarket::SaveCalculatingRSOption(void) {
  CSetRSOption setRSOption;

  setRSOption.Open();
  setRSOption.m_pDatabase->BeginTrans();
  while (!setRSOption.IsEOF()) {
    setRSOption.Delete();
    setRSOption.MoveNext();
  }
  setRSOption.m_pDatabase->CommitTrans();
  setRSOption.Close();

  setRSOption.Open();
  setRSOption.m_pDatabase->BeginTrans();
  for (int i = 0; i < 10; i++) {
    setRSOption.AddNew();
    setRSOption.m_Index = i;
    setRSOption.m_Active = m_aRSStrongOption[i].m_fActive;
    setRSOption.m_DayLengthFirst = m_aRSStrongOption[i].m_lDayLength[0];
    setRSOption.m_DayLengthSecond = m_aRSStrongOption[i].m_lDayLength[1];
    setRSOption.m_DayLengthThird = m_aRSStrongOption[i].m_lDayLength[2];
    setRSOption.m_DayLengthFourth = m_aRSStrongOption[i].m_lDayLength[3];
    setRSOption.m_StrongDayLengthFirst = m_aRSStrongOption[i].m_lStrongDayLength[0];
    setRSOption.m_StrongDayLengthSecond = m_aRSStrongOption[i].m_lStrongDayLength[1];
    setRSOption.m_StrongDayLengthThird = m_aRSStrongOption[i].m_lStrongDayLength[2];
    setRSOption.m_StrongDayLengthFourth = m_aRSStrongOption[i].m_lStrongDayLength[3];
    setRSOption.m_RSStrongFirst = ConvertValueToString(m_aRSStrongOption[i].m_dRSStrong[0]);
    setRSOption.m_RSStrongSecond = ConvertValueToString(m_aRSStrongOption[i].m_dRSStrong[1]);
    setRSOption.m_RSStrongThird = ConvertValueToString(m_aRSStrongOption[i].m_dRSStrong[2]);
    setRSOption.m_RSStrongFourth = ConvertValueToString(m_aRSStrongOption[i].m_dRSStrong[3]);
    setRSOption.Update();
  }
  setRSOption.m_pDatabase->CommitTrans();
  setRSOption.Close();
}

bool CChinaMarket::Load10DayRSStrongStockDB(void) {
  for (int i = 0; i < 10; i++) {
    LoadOne10DayRSStrongStockDB(i);
  }
  return true;
}

bool CChinaMarket::LoadOne10DayRSStrongStockDB(long lIndex) {
  m_lCurrentRSStrongIndex = lIndex;
  CSetRSStrongStock setRSStrongStock(lIndex);

  setRSStrongStock.Open();
  while (!setRSStrongStock.IsEOF()) {
    CChinaStockPtr pStock = gl_pChinaStockMarket->GetStock(setRSStrongStock.m_StockCode);
    if (pStock != nullptr) m_avChoicedStock[m_lCurrentRSStrongIndex + c_10DayRSStockSetStartPosition].push_back(pStock); // 10��RS��Ʊ����ʼλ��Ϊ��10����
    setRSStrongStock.MoveNext();
  }
  setRSStrongStock.Close();

  return true;
}

///////////////////////////////////////////////////////////////////////////////////
//
// ����lDay���������ǿ��, lDay�ĸ�ʽΪ��YYYYMMDD,�� 19990605.
// �����߰��ǵ����к�,�����ǿ�������ڶ����е�λ��.
// m_dRelativeStrongIndex���Ǽ������ָ�����ǵ�ǿ�ȡ�
//
//////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::BuildOneDayRelativeStrong(long lDay) {
  vector<CChinaStockPtr> vStock;
  vector<int> vIndex;
  vector<double> vRelativeStrong;
  int iTotalAShare = 0;
  CString strSQL;
  CString strDay;
  char  pch[30];
  int iStockNumber = 0;
  CTime ctTime;
  CSetDayLineBasicInfo setDayLineBasicInfo;
  double dShanghaiIndexUpDownRate = 0;
  double dShenzhenIndexUpDownRate = 0;
  double dIndexUpDownRate;
  double dRelativeStrongIndex;

  sprintf_s(pch, _T("%08d"), lDay);
  strDay = pch;
  setDayLineBasicInfo.m_strSort = _T("[UpDownRate]");
  setDayLineBasicInfo.m_strFilter = _T("[Day] =");
  setDayLineBasicInfo.m_strFilter += strDay;
  setDayLineBasicInfo.Open();
  if (setDayLineBasicInfo.IsEOF()) { // ���ݼ�Ϊ�գ���������û�н���
    setDayLineBasicInfo.Close();
    CString str = strDay;
    str += _T("�����ݼ�Ϊ�գ�û�м������ǿ��");
    gl_systemMessage.PushDayLineInfoMessage(str);    // ����ͬ�����Ʊ�����Ϣ
    return false;
  }
  setDayLineBasicInfo.m_pDatabase->BeginTrans();
  iStockNumber = 0;
  while (!setDayLineBasicInfo.IsEOF()) {
    if (strcmp(setDayLineBasicInfo.m_StockCode, _T("sh000001")) == 0) { // �Ϻ���ָ
      dShanghaiIndexUpDownRate = GetUpDownRate(setDayLineBasicInfo.m_Close, setDayLineBasicInfo.m_LastClose);
    }
    else if (strcmp(setDayLineBasicInfo.m_StockCode, _T("sz399001")) == 0) { // ���ڳ�ָ
      dShenzhenIndexUpDownRate = GetUpDownRate(setDayLineBasicInfo.m_Close, setDayLineBasicInfo.m_LastClose);
    }
    if (IsAStock(setDayLineBasicInfo.m_StockCode)) {
      long lIndex = m_mapChinaMarketAStock.at(setDayLineBasicInfo.m_StockCode);
      vStock.push_back(m_vChinaMarketStock.at(lIndex));
      vIndex.push_back(iStockNumber); // ��A�ɵ�������¼�������С�
      iTotalAShare++;
    }
    iStockNumber++;
    setDayLineBasicInfo.MoveNext();
  }
  dIndexUpDownRate = (dShanghaiIndexUpDownRate + dShenzhenIndexUpDownRate) / 2;

  setDayLineBasicInfo.MoveFirst();
  int iCount = 0;
  int iBefore = 0;
  while (iCount < vIndex.size()) { // ֻ�����Ծ��Ʊ�����ǿ��
    for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) { // ��������ȥ�������ݿ�,�������A�ɵĹ�Ʊ
      setDayLineBasicInfo.MoveNext();
    }
    setDayLineBasicInfo.Edit();
    double dLastClose = atof(setDayLineBasicInfo.m_LastClose);
    double dLow = atof(setDayLineBasicInfo.m_Low);
    double dHigh = atof(setDayLineBasicInfo.m_High);
    double dClose = atof(setDayLineBasicInfo.m_Close);
    double dUpDownRate = 0;
    // ����ָ�����ǿ��
    if (dLastClose < 0.001) { // �¹����еȣ��������̼۸�Ϊ��
      dRelativeStrongIndex = 50;
    }
    else {
      dUpDownRate = (dClose - dLastClose) / dLastClose;
      if ((dUpDownRate > 0.11) || (dUpDownRate < -0.11)) { // ��Ȩ�ȵ��¼۸�ͻ��
        dRelativeStrongIndex = 50;
      }
      else {
        dRelativeStrongIndex = (dUpDownRate - dIndexUpDownRate) * 500 + 50; // �Դ����ǵ�Ϊ��׼��50����
      }
    }
    setDayLineBasicInfo.m_RelativeStrongIndex = ConvertValueToString(dRelativeStrongIndex);

    // �����ǵ��������ǿ��
    if (dLastClose < 0.001) {
      setDayLineBasicInfo.m_RelativeStrong = ConvertValueToString(50); // �¹����л��߳�Ȩ��Ϣ��������˹�
    }
    else if (((dLow / dLastClose) < 0.88) || ((dHigh / dLastClose) > 1.12)) { // ��Ȩ���¹����е�
      setDayLineBasicInfo.m_RelativeStrong = ConvertValueToString(50); // �¹����л��߳�Ȩ��Ϣ��������˹�
    }
    else if ((fabs(dHigh - dClose) < 0.0001) && (((dClose / dLastClose)) > 1.095)) { // ��ͣ��
      setDayLineBasicInfo.m_RelativeStrong = ConvertValueToString(100);
    }
    else if ((fabs(dClose - dLow) < 0.0001) && ((dClose / dLastClose) < 0.905)) { // ��ͣ��
      setDayLineBasicInfo.m_RelativeStrong = ConvertValueToString(0);
    }
    else {
      setDayLineBasicInfo.m_RelativeStrong = ConvertValueToString((static_cast<double>(iCount) * 100) / iTotalAShare);
    }
    setDayLineBasicInfo.Update();
    iBefore = vIndex.at(iCount++);
    setDayLineBasicInfo.MoveNext(); // �Ƶ���һ�����ݡ�
    iBefore++; // ������Ҳͬʱ��һ��
  }
  setDayLineBasicInfo.m_pDatabase->CommitTrans();
  setDayLineBasicInfo.Close();

  vStock.clear();
  vIndex.clear();
  vRelativeStrong.clear();

  CString strDay2 = GetDayString(lDay);
  CString strTemp;
  strTemp = strDay2 + _T("�Ĺ�Ʊ���ǿ�ȼ������");
  gl_systemMessage.PushDayLineInfoMessage(strTemp);    // ����ͬ�����Ʊ�����Ϣ

  return(true);
}

bool CChinaMarket::BuildWeekLineRS(void) {
  return true;
}

double CChinaMarket::GetUpDownRate(CString strClose, CString strLastClose) {
  double lastClose = atof(strLastClose);
  if (lastClose < 0.001) return 0;
  double result = (atof(strClose) - lastClose) / lastClose;
  if ((result > 0.11) || (result < -0.11)) result = 0;
  return result;
}

bool CChinaMarket::UpdateStockCodeDB(void) {
  CSetStockCode setStockCode;

  setStockCode.Open();
  setStockCode.m_pDatabase->BeginTrans();
  while (!setStockCode.IsEOF()) {
    setStockCode.Delete();
    setStockCode.MoveNext();
  }
  setStockCode.m_pDatabase->CommitTrans();
  setStockCode.m_pDatabase->BeginTrans();
  for (auto pStock : m_vChinaMarketStock) {
    pStock->AppendStockCodeDB(setStockCode);
  }
  setStockCode.m_pDatabase->CommitTrans();
  setStockCode.Close();

  return true;
}

void CChinaMarket::LoadStockCodeDB(void) {
  CSetStockCode setStockCode;
  char buffer[30];
  CString str;

  setStockCode.Open();
  // װ���Ʊ�������ݿ�
  while (!setStockCode.IsEOF()) {
    CChinaStockPtr pStock = GetStock(setStockCode.m_StockCode);
    pStock->LoadStockCodeDB(setStockCode);
    setStockCode.MoveNext();
  }
  if (m_iDayLineNeedUpdate > 0) {
    int i = m_iDayLineNeedUpdate;
    m_lStockNeedUpdated = m_iDayLineNeedUpdate;
    if (GetDayOfWeek() == 1) gl_systemMessage.PushInformationMessage(_T("ÿ����һ�������й�Ʊ����"));
    TRACE("����%d����Ʊ��Ҫ�����������\n", i);
    _itoa_s(i, buffer, 10);
    str = buffer;
    str += _T("����Ʊ��Ҫ�����������");
    gl_systemMessage.PushInformationMessage(str);
  }
  setStockCode.Close();
}

///////////////////////////////////////////////////////////////////////////////////
//
// ����ѡ�����ݿ�
//
//
//
//////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::UpdateOptionDB(void) {
  CSetOption setOption;
  setOption.Open();
  setOption.m_pDatabase->BeginTrans();
  if (setOption.IsEOF()) {
    setOption.AddNew();
    setOption.m_RelativeStrongEndDay = GetRelativeStrongEndDay();
    setOption.m_RalativeStrongStartDay = GetRelativeStrongStartDay();
    setOption.m_LastLoginDay = GetFormatedMarketDay();
    setOption.m_UpdatedDayFor10DayRS1 = GetUpdatedDayFor10DayRS1();
    setOption.m_UpdatedDayFor10DayRS2 = GetUpdatedDayFor10DayRS2();
    setOption.m_UpdatedDayFor10DayRS = GetUpdatedDayFor10DayRS();
    setOption.Update();
  }
  else {
    setOption.Edit();
    setOption.m_RelativeStrongEndDay = GetRelativeStrongEndDay();
    setOption.m_RalativeStrongStartDay = GetRelativeStrongStartDay();
    setOption.m_LastLoginDay = GetFormatedMarketDay();
    setOption.m_UpdatedDayFor10DayRS1 = GetUpdatedDayFor10DayRS1();
    setOption.m_UpdatedDayFor10DayRS2 = GetUpdatedDayFor10DayRS2();
    setOption.m_UpdatedDayFor10DayRS = GetUpdatedDayFor10DayRS();
    setOption.Update();
  }
  setOption.m_pDatabase->CommitTrans();
  setOption.Close();
  return true;
}

void CChinaMarket::LoadOptionDB(void) {
  CSetOption setOption;
  setOption.Open();
  if (setOption.IsEOF()) {
    SetRelativeStrongStartDay(__CHINA_MARKET_BEGIN_DAY__);
    SetRelativeStrongEndDay(__CHINA_MARKET_BEGIN_DAY__);
    SetLastLoginDay(__CHINA_MARKET_BEGIN_DAY__);
    SetUpdatedDayFor10DayRS1(__CHINA_MARKET_BEGIN_DAY__);
    SetUpdatedDayFor10DayRS2(__CHINA_MARKET_BEGIN_DAY__);
  }
  else {
    if (setOption.m_RelativeStrongEndDay == 0) {
      SetRelativeStrongEndDay(__CHINA_MARKET_BEGIN_DAY__);
    }
    else {
      SetRelativeStrongEndDay(setOption.m_RelativeStrongEndDay);
      if (GetRelativeStrongEndDay() > __CHINA_MARKET_BEGIN_DAY__) {
        // ��������ʷ���ݿ��д��ھ�����ʱ�����õ��߳�ģʽ�洢�����ݡ�ʹ�ö��߳�ģʽʱ��MySQL����ֻ�����Exception�����������ݿ�����ʱ����ͬ�����⣩��
        // �ʶ��޲�����ʱͬʱֻ����һ���洢�̣߳���������������״̬���������ⲻ�����������������������ʱ���ʶ��½���������ʱ����ʹ�ö��̣߳�ĿǰΪ4������
        gl_SaveOneStockDayLine.SetMaxCount(1);
      }
    }
    if (setOption.m_RalativeStrongStartDay == 0) {
      SetRelativeStrongStartDay(__CHINA_MARKET_BEGIN_DAY__);
    }
    else {
      SetRelativeStrongStartDay(setOption.m_RalativeStrongStartDay);
    }
    if (setOption.m_LastLoginDay == 0) {
      SetLastLoginDay(__CHINA_MARKET_BEGIN_DAY__);
    }
    else {
      SetLastLoginDay(setOption.m_LastLoginDay);
    }
    SetUpdatedDayFor10DayRS1(setOption.m_UpdatedDayFor10DayRS1);
    SetUpdatedDayFor10DayRS2(setOption.m_UpdatedDayFor10DayRS2);
    SetUpdatedDayFor10DayRS(setOption.m_UpdatedDayFor10DayRS);
    if (setOption.m_UpdatedDayFor10DayRS1 < GetFormatedMarketDay())  m_fChoiced10RSStrong1StockSet = false;
    else m_fChoiced10RSStrong1StockSet = true;
    if (setOption.m_UpdatedDayFor10DayRS2 < GetFormatedMarketDay())  m_fChoiced10RSStrong2StockSet = false;
    else m_fChoiced10RSStrong2StockSet = true;
    if (setOption.m_UpdatedDayFor10DayRS < GetFormatedMarketDay())  m_fChoiced10RSStrongStockSet = false;
    else m_fChoiced10RSStrongStockSet = true;
  }

  setOption.Close();
}

void CChinaMarket::LoadOptionChinaStockMarketDB(void) {
  CSetOptionChinaStockMarket setOptionChinaStockMarket;

  setOptionChinaStockMarket.Open();
  if (!setOptionChinaStockMarket.IsEOF()) {
    m_iRTDataServer = setOptionChinaStockMarket.m_RTDataServerIndex;
  }
  else {
    m_iRTDataServer = 0; // Ĭ��ʹ������ʵʱ���ݷ�����
  }
}

bool CChinaMarket::UpdateOptionChinaStockMarketDB(void) {
  CSetOptionChinaStockMarket setOptionChinaStockMarket;

  setOptionChinaStockMarket.Open();
  setOptionChinaStockMarket.m_pDatabase->BeginTrans();
  if (setOptionChinaStockMarket.IsEOF()) {
    setOptionChinaStockMarket.AddNew();
    setOptionChinaStockMarket.m_RTDataServerIndex = m_iRTDataServer;
    setOptionChinaStockMarket.Update();
  }
  else {
    setOptionChinaStockMarket.Edit();
    setOptionChinaStockMarket.m_RTDataServerIndex = m_iRTDataServer;
    setOptionChinaStockMarket.Update();
  }
  setOptionChinaStockMarket.m_pDatabase->CommitTrans();
  setOptionChinaStockMarket.Close();

  return true;
}

bool CChinaMarket::UpdateChoicedStockDB(void) {
  CSetChoicedStock setChoicedStock;

  setChoicedStock.Open();
  setChoicedStock.m_pDatabase->BeginTrans();
  while (!setChoicedStock.IsEOF()) {
    setChoicedStock.Delete();
    setChoicedStock.MoveNext();
  }
  setChoicedStock.m_pDatabase->CommitTrans();
  setChoicedStock.m_pDatabase->BeginTrans();
  for (auto pStock : m_avChoicedStock[0]) {
    ASSERT(pStock->IsChoiced());
    setChoicedStock.AddNew();
    setChoicedStock.m_Market = pStock->GetMarket();
    setChoicedStock.m_StockCode = pStock->GetStockCode();
    setChoicedStock.Update();
    pStock->SetSaveToChoicedStockDB(true);
  }
  setChoicedStock.m_pDatabase->CommitTrans();
  setChoicedStock.Close();

  return true;
}

bool CChinaMarket::AppendChoicedStockDB(void) {
  CSetChoicedStock setChoicedStock;

  setChoicedStock.Open();
  setChoicedStock.m_pDatabase->BeginTrans();
  for (auto pStock : m_avChoicedStock[0]) {
    ASSERT(pStock->IsChoiced());
    if (!pStock->IsSaveToChoicedStockDB()) {
      setChoicedStock.AddNew();
      setChoicedStock.m_Market = pStock->GetMarket();
      setChoicedStock.m_StockCode = pStock->GetStockCode();
      setChoicedStock.Update();
      pStock->SetSaveToChoicedStockDB(true);
    }
  }
  setChoicedStock.m_pDatabase->CommitTrans();
  setChoicedStock.Close();

  return true;
}

void CChinaMarket::LoadChoicedStockDB(void) {
  CSetChoicedStock setChoicedStock;

  setChoicedStock.Open();
  // װ���Ʊ�������ݿ�
  while (!setChoicedStock.IsEOF()) {
    CChinaStockPtr pStock = GetStock(setChoicedStock.m_StockCode);
    auto it = find(m_avChoicedStock[0].cbegin(), m_avChoicedStock[0].cend(), pStock);
    if (it == m_avChoicedStock[0].end()) {
      m_avChoicedStock[0].push_back(pStock);
    }
    pStock->SetChoiced(true);
    pStock->SetSaveToChoicedStockDB(true);
    setChoicedStock.MoveNext();
  }
  setChoicedStock.Close();
}

bool CChinaMarket::UpdateTempRTData(void) {
  if (!gl_ThreadStatus.IsSavingTempData()) {
    gl_ThreadStatus.SetSavingTempData(true);
    RunningThreadSaveTempRTData();
    return true;
  }
  return false;
}