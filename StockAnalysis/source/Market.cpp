//////////////////////////////////////////////////////////////////////////////////////////////////

//#include"stdafx.h"

#include"globedef.h"
#include"Thread.h"

#include"accessory.h"
#include"TransferSharedPtr.h"

#include"stock.h"
#include"Market.h"

#include"SetDayLineInfo.h"
#include"SetDayLineToday.h"
#include"SetOption.h"
#include"SetCrweberIndex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMarket::CMarket(void) : CObject() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("�г�����ֻ�������һ��ʵ��\n");
    ASSERT(0);
  }

  m_fPermitResetSystem = true; // ����ϵͳ�����ñ�ʶ��Ψ���˱�ʶ������ϵͳ���á���ʼʱ����Ϊ�棺��������ϵͳ��

  Reset();
}

CMarket::~CMarket() {
}

void CMarket::Reset(void) {
  m_lTotalActiveStock = 0; // ��ʼʱ��Ʊ���еĹ�Ʊ����Ϊ��

  m_fLoadedSelectedStock = false;
  m_fSystemReady = false;    // �г���ʼ״̬Ϊδ���úá�
  m_fCurrentStockChanged = false;
  m_fCurrentEditStockChanged = false;

  m_pCurrentStock = nullptr;

  m_lTotalMarketBuy = m_lTotalMarketSell = 0;

  m_ttNewestTransactionTime = 0;

  time_t ttime;
  time(&ttime);
  tm tm_;
  localtime_s(&tm_, &ttime);
  if (tm_.tm_hour >= 15) { // �й���Ʊ�г��Ѿ�����
    m_fTodayStockCompiled = true; // ���к��ִ�б�ϵͳ������Ϊ�Ѿ���������չ�Ʊ�����ˡ�
  }
  else m_fTodayStockCompiled = false;

  m_lRelativeStrongEndDay = m_lRelativeStrongStartDay = m_lLastLoginDay = 19900101;

  m_fSaveDayLine = false;

  m_fTodayTempDataLoaded = false;

  m_fCheckTodayActiveStock = true;  //��鵱�ջ�Ծ��Ʊ������Ϊ�档

  m_fCalculatingRS = false;

  m_fGetRTStockData = true;
  m_fReadingTengxunRTData = true; // Ĭ��״̬�²���ȡ��Ѷʵʱ����
  m_iCountDownDayLine = 3;    // 400ms��ʱ��100msÿ�Σ�
  m_iCountDownSlowReadingRTData = 3; // 400����ÿ��

  m_fUsingSinaRTDataReceiver = true; // ʹ������ʵʱ������ȡ��
  m_fUsingNeteaseRTDataReceiver = true; // ʹ������ʵʱ������ȡ��
  m_fUsingNeteaseRTDataReceiverAsTester = false;
  m_fUsingTengxunRTDataReceiverAsTester = true;

  m_iDayLineNeedProcess = 0;
  m_iDayLineNeedSave = 0;
  m_iDayLineNeedUpdate = 0;

  // ���ɹ�Ʊ�����
  CreateTotalStockContainer();

  // ���ô�ȫ�ֱ���
  gl_CrweberIndex.Reset();
}

#ifdef _DEBUG
void CMarket::AssertValid() const {
  CObject::AssertValid();
}

void CMarket::Dump(CDumpContext& dc) const {
  CObject::Dump(dc);
}
#endif //_DEBUG

void CMarket::TaskGetNeteaseDayLineFromWeb(void) {
  ASSERT(SystemReady());
  if (m_iDayLineNeedUpdate > 0) {
    GetNeteaseWebDayLineData();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ʼ�����п��ܵĹ�Ʊ����أ�ֻ��CMarket�ĳ�ʼ��������һ�Ρ�
// ���������Ҫ����ȫ�ֱ�����ʼ����֧�֣��ʶ�gl_ChinaStockMarketʵ����Ҫ��������ȫ�ֱ��������
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::CreateTotalStockContainer(void) {
  char buffer[10]{};

  CStockPtr pStock = nullptr;
  int iCount = 0;

  // ���֮ǰ�����ݣ�����еĻ�����Resetʱ�������������оʹ������ݣ���
  m_vChinaMarketAStock.clear();
  m_mapChinaMarketAStock.clear();

  // �����Ϻ���Ʊ����
  for (int i = 600000; i < 602000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStock = make_shared<CStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // �Ϻ�����
    pStock->SetOffset(iCount);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
    ASSERT(pStock->IsDayLineNeedUpdate());
    m_iDayLineNeedUpdate++;
  }

  // ���������Ʊ����
  for (int i = 603000; i < 604000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStock = make_shared<CStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // �Ϻ�����
    pStock->SetOffset(iCount);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // ���ɿƴ����Ʊ����
  for (int i = 688000; i < 689000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStock = make_shared<CStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // �Ϻ��ƴ���
    pStock->SetOffset(iCount);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // ����B�ɹ�Ʊ����
  for (int i = 900000; i < 901000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStock = make_shared<CStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // �Ϻ�B��
    pStock->SetOffset(iCount);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // �����Ϻ�ָ������
  for (int i = 0; i < 1000; i++) {
    CString str = _T("sh");
    sprintf_s(buffer, 10, "%06d", i);
    pStock = make_shared<CStock>();
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHANGHAI_MARKET__); // �Ϻ�ָ��
    pStock->SetOffset(iCount);
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  ///////////////////////////////////////////////
  // �������������Ʊ����
  for (int i = 0; i < 2000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStock = make_shared<CStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // ��������
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // ����������С���Ʊ����
  for (int i = 2000; i < 3000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStock = make_shared<CStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // ������С��
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // ����B�ɹ�Ʊ����
  for (int i = 200000; i < 201000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStock = make_shared<CStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // ����B��
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // ���ɴ�ҵ���Ʊ����
  for (int i = 300000; i < 301000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStock = make_shared<CStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // ���ڴ�ҵ��
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }

  // ��������ָ��
  for (int i = 399000; i < 400000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStock = make_shared<CStock>();
    pStock->SetOffset(iCount);
    str += buffer;
    pStock->SetStockCode(str);
    pStock->SetMarket(__SHENZHEN_MARKET__); // ����ָ��
    m_vChinaMarketAStock.push_back(pStock);
    m_mapChinaMarketAStock[pStock->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
    m_iDayLineNeedUpdate++;
  }
  m_lTotalStock = m_vChinaMarketAStock.size();
  ASSERT(m_iDayLineNeedUpdate == 12000); // Ŀǰ�ܲ�ѯ��Ʊ��Ϊ12000����
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����������߹�Ʊ������ַ��������ڲ�ѯ�˹�Ʊ�ڵ�ǰ�г��Ƿ��ڻ�Ծ״̬�������Ƿ���ڴ˹�Ʊ���룩
//
//  �˺����Ǽ��m_vChinaMarketAStock��Ʊ��
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::CreateNeteaseDayLineInquiringStr(CString& str) {
  static int siCounter = 0;

  bool fFound = false;
  int iCount = 0;
  CString strTemp;
  while (!fFound && (iCount++ < 1000)) {
    CStockPtr pStock = m_vChinaMarketAStock.at(siCounter);
    if (!pStock->IsDayLineNeedUpdate()) { // �������ݲ���Ҫ���¡���ϵͳ��ʼʱ�����ô�m_fDayLineNeedUpdate��ʶ
      // TRACE("%S ���������������\n", static_cast<LPCWSTR>(pStock->m_strStockCode));
      IncreaseStockInquiringIndex(siCounter);
    }
    else if (pStock->GetIPOStatus() == __STOCK_NULL__) {	// ��δʹ�ù��Ĺ�Ʊ���������ѯ��������
      pStock->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
      // TRACE("��Ч��Ʊ���룺%S, �����ѯ��������\n", static_cast<LPCWSTR>(pStock->m_strStockCode));
      IncreaseStockInquiringIndex(siCounter);
    }
    else if (pStock->GetDayLineEndDay() >= gl_systemTime.GetLastTradeDay()) { // ��һ�����յ����������Ѿ��洢����ʱ�Ѿ������һ�����������ˣ������ٴδ���
      pStock->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
      // TRACE("%S �������ݱ����Ѹ���\n", static_cast<LPCWSTR>(pStock->m_strStockCode));
      IncreaseStockInquiringIndex(siCounter);
    }
    else if (pStock->IsDayLineNeedProcess()) { // �������������ص���δ����һ������������֣�
      IncreaseStockInquiringIndex(siCounter);
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
  CStockPtr pStock = m_vChinaMarketAStock.at(siCounter);
  ASSERT(!pStock->IsDayLineNeedSaving());
  ASSERT(!pStock->IsDayLineNeedProcess());
  pStock->SetDayLineNeedUpdate(false);
  switch (pStock->GetMarket()) { // ת����������������������ʽ���Ϻ�Ϊ��0��������Ϊ��1����
  case __SHANGHAI_MARKET__: // �Ϻ��г���
  case __SHANGHAI_MAIN__: // �Ϻ����壿
  case __SHANGHAI_INDEX__: // �Ϻ�ָ��
  case __SHANGHAI_3BAN__: // �Ϻ�3��
  case __SHANGHAI_KECHUANG__: // �Ϻ��ƴ���
  case __SHANGHAI_B_SHARE__: // �Ϻ�B��
  str += '0'; // �Ϻ��г���ʶ
  break;
  case __SHENZHEN_MARKET__: // �����г���
  case __SHENZHEN_INDEX__: // ����ָ��
  case __SHENZHEN_3BAN__: // ������С��
  case __SHENZHEN_B_SHARE__: // ����B��
  case __SHENZHEN_CHUANGYE__: // ���ڴ�ҵ��
  case __SHENZHEN_MAIN__: // ��������
  str += '1'; // �����г���ʶ
  break;
  default: // Խ��
  ASSERT(0);
  }
  str += pStock->GetStockCode().Right(6); // ȡ��Ʊ������ұ���λ���֡�
  IncreaseStockInquiringIndex(siCounter);
  return true;
}

int CMarket::IncreaseStockInquiringIndex(int& iIndex) {
  if (++iIndex == m_lTotalStock) {
    iIndex = 0;
  }
  return iIndex;
}

/////////////////////////////////////////////////////////////////////////
//
//	�õ���ʱ��ƫ������09:30Ϊ0��15:00Ϊ240,����Ϊ1����
//
//
////////////////////////////////////////////////////////////////////////
long CMarket::GetMinLineOffset(CStock sID, time_t Time) {
  ASSERT(Time >= 0);
  tm tmTemp{};
  time_t t = 0;
  long lIndex = 0;

  localtime_s(&tmTemp, &Time);
  tmTemp.tm_hour = (9 - 8);			// time_t, tmʹ�õ��ǹ��ʱ�׼ʱ(UTC),�ʱ���ʱ��09��30��UTC��01��30��Ҫ��ȥ8Сʱ
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
bool CMarket::IsAStock(CStockPtr pStock) {
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
bool CMarket::IsAStock(CString strStockCode) {
  if ((strStockCode[0] == 's') && (strStockCode[1] == 'h') && (strStockCode[2] == '6') && (strStockCode[3] == '0')) {
    if ((strStockCode[4] == '0') || (strStockCode[4] == '1')) {
      return true;
    }
  }
  else {
    if ((strStockCode[0] == 's') && (strStockCode[1] == 'z') && (strStockCode[2] == '0') && (strStockCode[3] == '0')) {
      if ((strStockCode[4] == '0') || (strStockCode[4] == '2')) {
        return true;
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
bool CMarket::IsStock(CString strStockCode, CStockPtr& pStock) {
  if ((pStock = GetStockPtr(strStockCode)) != nullptr) {
    return(true);
  }
  else {
    pStock = NULL;
    return(false);
  }
}

bool CMarket::IsUpdateStockCodeDB(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsDayLineDBUpdated()) return true;
  }
  return false;
}

void CMarket::ClearUpdateStockCodeDBFlag(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    pStock->SetDayLineDBUpdated(false);
  }
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////
INT64 CMarket::GetTotalAttackBuyAmount(void) {
  INT64 lAmount = 0;
  for (auto pStock : m_vChinaMarketAStock) {
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
INT64 CMarket::GetTotalAttackSellAmount(void) {
  INT64 lAmount = 0;
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsActive() && IsAStock(pStock)) {
      lAmount += pStock->GetAttackSellAmount();
    }
  }
  return(lAmount);
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
bool CMarket::TaskDistributeSinaRTDataToProperStock(void) {
  gl_ProcessSinaRTDataQueue.Wait();
  CStockPtr pStock;
  const long lTotalNumber = gl_QueueSinaRTData.GetRTDataSize();
  CString strVolume;

  for (int iCount = 0; iCount < lTotalNumber; iCount++) {
    CRTDataPtr pRTData = gl_QueueSinaRTData.PopRTData();
    if (pRTData->GetDataSource() == __INVALID_RT_WEB_DATA__) {
      gl_systemMessage.PushInnerSystemInformationMessage(_T("ʵʱ����Դ��������"));
    }
    if (pRTData->IsActive()) { // ��ʵʱ������Ч��
      if (m_ttNewestTransactionTime < pRTData->GetTransactionTime()) {
        m_ttNewestTransactionTime = pRTData->GetTransactionTime();
      }
      long lIndex = m_mapChinaMarketAStock.at(pRTData->GetStockCode());
      pStock = m_vChinaMarketAStock.at(lIndex);
      if (!pStock->IsActive()) {
        if (pRTData->IsValidTime()) {
          pStock->SetActive(true);
          pStock->SetStockName(pRTData->GetStockName());
          pStock->SetStockCode(pRTData->GetStockCode());
          pStock->UpdateStatus(pRTData);
          pStock->SetTransactionTime(pRTData->GetTransactionTime());
          pStock->SetIPOStatus(__STOCK_IPOED__);
          m_lTotalActiveStock++;
        }
      }
      if (pRTData->GetTransactionTime() > pStock->GetTransactionTime()) { // �µ����ݣ�
        pStock->PushRTData(pRTData); // �洢�µ����������ݳ�
        pStock->SetTransactionTime(pRTData->GetTransactionTime());   // �������½��ܵ�ʵʱ���ݵ�ʱ��
      }
    }
  }
  gl_ThreadStatus.SetRTDataNeedCalculate(true); // ���ý��յ�ʵʱ���ݱ�ʶ
  ASSERT(gl_QueueSinaRTData.GetRTDataSize() == 0); // ����һ�δ���ȫ�����ݡ�
  gl_ProcessSinaRTDataQueue.Signal();

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ÿ�β�ѯ����ʵʱ��Ʊ���ݵ��ַ���
//
//////////////////////////////////////////////////////////////////////////////////////////
int CMarket::GetSinaInquiringStockStr(CString& str, long lTotalNumber, bool fSkipUnactiveStock) {
  static int siCounter = 0;

  return GetInquiringStr(str, siCounter, _T(","), lTotalNumber, fSkipUnactiveStock);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ÿ�β�ѯ��Ѷʵʱ��Ʊ���ݵ��ַ���
//
//////////////////////////////////////////////////////////////////////////////////////////
int CMarket::GetTengxunInquiringStockStr(CString& str, long lTotalNumber, bool fSkipUnactiveStock) {
  static int siCounter = 0;

  ASSERT(SystemReady());
  return GetInquiringStr(str, siCounter, _T(","), lTotalNumber, fSkipUnactiveStock);
}

int CMarket::GetNeteaseInquiringStockStr(CString& str, long lTotalNumber, bool fSkipUnactiveStock) {
  static int siCounter = 0;

  CString strStockCode, strRight6, strLeft2, strPrefix;
  if (fSkipUnactiveStock) StepToActiveStockIndex(siCounter);
  strStockCode = m_vChinaMarketAStock.at(siCounter)->GetStockCode();
  IncreaseStockInquiringIndex(siCounter);
  strRight6 = strStockCode.Right(6);
  strLeft2 = strStockCode.Left(2);
  if (strLeft2.Compare(_T("sh")) == 0) {
    strPrefix = _T("0");
  }
  else strPrefix = _T("1");
  str += strPrefix + strRight6;  // �õ���һ����Ʊ����
  int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
  while ((siCounter < m_vChinaMarketAStock.size()) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
    if (fSkipUnactiveStock) StepToActiveStockIndex(siCounter);
    iCount++;
    str += _T(",");
    strStockCode = m_vChinaMarketAStock.at(siCounter)->GetStockCode();
    strRight6 = strStockCode.Right(6);
    strLeft2 = strStockCode.Left(2);
    if (strLeft2.Compare(_T("sh")) == 0) {
      strPrefix = _T("0");
    }
    else strPrefix = _T("1");
    str += strPrefix + strRight6;  // �õ���һ����Ʊ����
    if (siCounter == 0) break;
    IncreaseStockInquiringIndex(siCounter);
  }
  if (siCounter > 0) siCounter--;

  return iCount;
}

int CMarket::GetInquiringStr(CString& str, int& iStockIndex, CString strPostfix, long lTotalNumber, bool fSkipUnactiveStock) {
  if (fSkipUnactiveStock) StepToActiveStockIndex(iStockIndex);
  str += m_vChinaMarketAStock.at(iStockIndex)->GetStockCode();  // �õ���һ����Ʊ����
  IncreaseStockInquiringIndex(iStockIndex);
  int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
  while ((iStockIndex < m_vChinaMarketAStock.size()) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
    if (fSkipUnactiveStock) StepToActiveStockIndex(iStockIndex);
    iCount++;
    str += strPostfix;
    str += m_vChinaMarketAStock.at(iStockIndex)->GetStockCode();
    if (iStockIndex == 0) break;
    IncreaseStockInquiringIndex(iStockIndex);
  }
  if (iStockIndex > 0) iStockIndex--;

  return iCount;
}

bool CMarket::StepToActiveStockIndex(int& iStockIndex) {
  while (!m_vChinaMarketAStock.at(iStockIndex)->IsActive()) {
    IncreaseStockInquiringIndex(iStockIndex);
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// �ɹ����߳�ThreadCalculatingRTDataProc���ã�ע��ȫ�ֱ�����ʹ��
//
//
/////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ProcessRTData(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsActive()) {
      pStock->ProcessRTData();
    }
  }
  return true;
}

bool CMarket::TaskProcessWebRTDataGetFromSinaServer(void) {
  CWebDataReceivedPtr pWebDataReceived = nullptr;
  long lTotalData = gl_QueueSinaWebRTData.GetWebRTDataSize();
  for (int i = 0; i < lTotalData; i++) {
    pWebDataReceived = gl_QueueSinaWebRTData.PopWebRTData();
    pWebDataReceived->m_pCurrentPos = pWebDataReceived->m_pDataBuffer;
    pWebDataReceived->m_lCurrentPos = 0;
    while (pWebDataReceived->m_lCurrentPos < pWebDataReceived->m_lBufferLength) {
      CRTDataPtr pRTData = make_shared<CRTData>();
      if (pRTData->ReadSinaData(pWebDataReceived)) {
        gl_QueueSinaRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
        //gl_QueueSinaRTDataForSave.PushRTData(pRTData); // ͬʱ������洢ʵʱ���ݶ���
      }
      else return false;  // ��������ݳ����⣬�׵����á�
    }
  }
  return true;
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
// ����ʵʱ����ȱ�ٹؼ��Եĳɽ����һ��ʶ��޷���Ϊ�������ݣ�ֻ����Ϊ�����á�
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::TaskProcessWebRTDataGetFromNeteaseServer(void) {
  CWebDataReceivedPtr pWebDataReceived = nullptr;
  int iCount = 0;

  long lTotalData = gl_QueueNeteaseWebRTData.GetWebRTDataSize();
  for (int i = 0; i < lTotalData; i++) {
    pWebDataReceived = gl_QueueNeteaseWebRTData.PopWebRTData();
    pWebDataReceived->ResetCurrentPos();

    if (!IsValidNeteaseRTDataPrefix(pWebDataReceived)) return false;

    iCount = 0;
    while (!((*pWebDataReceived->m_pCurrentPos == ' ') || (pWebDataReceived->m_lCurrentPos >= (pWebDataReceived->m_lBufferLength - 4)))) {
      CRTDataPtr pRTData = make_shared<CRTData>();
      if (pRTData->ReadNeteaseData(pWebDataReceived)) {
        iCount++;
        gl_QueueNeteaseRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
        //gl_QueueNeteaseRTDataForSave.PushRTData(pRTData); // ͬʱ������洢ʵʱ���ݶ���
        //TRACE(_T("����ʵʱ���ݽ��յ�%s \n"), pRTData->GetStockCode());
        // ���һ��
        CheckNeteaseRTData(pRTData);
      }
      else return false;  // ��������ݳ����⣬�׵����á�
    }
    TRACE(_T("ReadNetease��������,��������%d������\n", iCount));
  }
  return true;
}

bool CMarket::IsValidNeteaseRTDataPrefix(CWebDataReceivedPtr pWebDataReceived) {
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

void CMarket::CheckNeteaseRTData(CRTDataPtr pRTData) {
  // ���һ��
  CString str;
  ASSERT(pRTData->GetDataSource() == __NETEASE_RT_WEB_DATA__);
  if (pRTData->IsActive()) {
    CStockPtr pStock = nullptr;
    if ((pStock = gl_ChinaStockMarket.GetStockPtr(pRTData->GetStockCode())) != nullptr) {
      if (!pStock->IsActive()) {
        str = pStock->GetStockCode();
        str += _T(" ����ʵʱ��⵽�����ڻ�Ծ״̬");
        //gl_systemMessage.PushInnerSystemInformationMessage(str);
      }
    }
    else {
      str = pRTData->GetStockCode();
      str += _T(" ��Ч��Ʊ���루����ʵʱ���ݣ�");
      gl_systemMessage.PushInnerSystemInformationMessage(str);
    }
  }
}

bool CMarket::TaskProcessNeteaseRTData(void) {
  CRTDataPtr pRTData = nullptr;
  long lTotalData = gl_QueueNeteaseRTData.GetRTDataSize();

  for (long i = 0; i < lTotalData; i++) {
    // Ŀǰ��ʹ������ʵʱ���ݣ�����ֻ�Ǽ򵥵�ȡ�����ӵ���
    pRTData = gl_QueueNeteaseRTData.PopRTData();
    pRTData = nullptr;
  }
  if (lTotalData > 0) TRACE(_T("������%d������ʵʱ����\n"), lTotalData);
  return true;
}

bool CMarket::TaskProcessWebRTDataGetFromCrweberdotcom(void) {
  CWebDataReceivedPtr pWebData = nullptr;
  long lTotalData = gl_QueueSinaWebRTData.GetWebRTDataSize();
  for (int i = 0; i < lTotalData; i++) {
    pWebData = gl_QueueCrweberdotcomWebData.PopWebRTData();
    pWebData->m_pCurrentPos = pWebData->m_pDataBuffer;
    pWebData->m_lCurrentPos = 0;
    if (gl_CrweberIndex.ReadData(pWebData)) {
      if (gl_CrweberIndex.IsTodayUpdated() || gl_CrweberIndex.IsDataChanged()) {
        gl_CrweberIndexLast = gl_CrweberIndex;
        gl_ChinaStockMarket.SaveCrweberIndexData();
        gl_systemMessage.PushInformationMessage(_T("crweber����ָ���Ѹ���"));
        gl_CrweberIndex.m_fTodayUpdated = false;
      }
      TRACE("crweber.com���ֽ���Ϊ%d\n", pWebData->m_lBufferLength);
    }
    else return false;  // ��������ݳ����⣬�׵����á�
  }
  return true;
}

bool CMarket::TaskProcessWebRTDataGetFromTengxunServer(void) {
  CWebDataReceivedPtr pWebDataReceived = nullptr;

  long lTotalData = gl_QueueTengxunWebRTData.GetWebRTDataSize();
  for (int i = 0; i < lTotalData; i++) {
    pWebDataReceived = gl_QueueTengxunWebRTData.PopWebRTData();
    pWebDataReceived->ResetCurrentPos();

    if (!IsValidTengxunRTDataPrefix(pWebDataReceived)) return false;

    while (pWebDataReceived->m_lCurrentPos < pWebDataReceived->m_lBufferLength) {
      CRTDataPtr pRTData = make_shared<CRTData>();
      if (pRTData->ReadTengxunData(pWebDataReceived)) {
        gl_QueueTengxunRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
        //gl_QueueSinaRTDataForSave.PushRTData(pRTData); // ͬʱ������洢ʵʱ���ݶ���

        // ���һ��
        CheckTengxunRTData(pRTData);
      }
      else return false;  // ��������ݳ����⣬�׵����á�
    }
  }
  return true;
}

bool CMarket::IsValidTengxunRTDataPrefix(CWebDataReceivedPtr pWebDataReceived) {
  char buffer[50];
  CString strInvalidStock = _T("v_pv_none_match=\"1\";\n"); // ��Ϊ��Ч��Ʊ��ѯ�������ݸ�ʽ����21���ַ�
  strncpy_s(buffer, pWebDataReceived->m_pCurrentPos, 21);
  buffer[21] = 0x000;
  CString str1 = buffer;

  if (str1.Compare(strInvalidStock) == 0) {
    pWebDataReceived->IncreaseCurrentPos(21);
    return true;
  }
  else return false;
}

void CMarket::CheckTengxunRTData(CRTDataPtr pRTData) {
  CString str;
  ASSERT(pRTData->GetDataSource() == __TENGXUN_RT_WEB_DATA__);
  if (pRTData->IsActive()) {
    CStockPtr pStock = nullptr;
    if ((pStock = gl_ChinaStockMarket.GetStockPtr(pRTData->GetStockCode())) != nullptr) {
      if (!pStock->IsActive()) {
        str = pStock->GetStockCode();
        str += _T(" ��Ѷʵʱ��⵽�����ڻ�Ծ״̬");
        //gl_systemMessage.PushInnerSystemInformationMessage(str);
      }
    }
    else {
      str = pRTData->GetStockCode();
      str += _T(" ��Ч��Ʊ���루��Ѷʵʱ���ݣ�");
      gl_systemMessage.PushInnerSystemInformationMessage(str);
    }
  }
}

bool CMarket::TaskProcessTengxunRTData(void) {
  CRTDataPtr pRTData = nullptr;
  long lTotalData = gl_QueueTengxunRTData.GetRTDataSize();

  for (long i = 0; i < lTotalData; i++) {
    pRTData = gl_QueueTengxunRTData.PopRTData();
    if (pRTData->IsActive()) {
      auto pStock = gl_ChinaStockMarket.GetStockPtr(pRTData->GetStockCode());
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
bool CMarket::SchedulingTask(void) {
  static time_t s_timeLast = 0;
#ifdef DEBUG
#define __NumberOfCount__ 10
#else
#define __NumberOfCount__ 1000
#endif
  gl_systemTime.CalculateTime();      // ����ϵͳ����ʱ��

  // ץȡʵʱ����(���ˡ���Ѷ�����ף���ÿ400��������һ�Σ����ɱ�֤��3�����ڱ���һ��ȫ���Ծ��Ʊ��
  if (m_fGetRTStockData && (m_iCountDownSlowReadingRTData <= 0)) {
    TaskGetRTDataFromWeb();
    TaskProcessWebRTDataGetFromSinaServer();
    // ���Ҫ�����ٶ�ȡʵʱ���ݣ������ö�ȡ����Ϊÿ����һ��
    if (!m_fMarketOpened && SystemReady()) m_iCountDownSlowReadingRTData = __NumberOfCount__; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
    else m_iCountDownSlowReadingRTData = 3;  // ����4��,��ÿ400��������һ��ʵʱ����
  }
  m_iCountDownSlowReadingRTData--;

  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (gl_systemTime.Gett_time() > s_timeLast) {
    SchedulingTaskPerSecond(gl_systemTime.Gett_time() - s_timeLast);
    s_timeLast = gl_systemTime.Gett_time();
  }

  // ϵͳ׼������֮����Ҫ��ɵĸ����
  if (SystemReady()) {
    if (!m_fTodayTempDataLoaded) { // �˹���������һ�Ρ�
      LoadTodayTempDB();
      m_fTodayTempDataLoaded = true;
    }
    TaskProcessWebRTDataGetFromTengxunServer();
    TaskProcessWebRTDataGetFromNeteaseServer();
    TaskGetNeteaseDayLineFromWeb();
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////
//
// �����ˡ����׻�����Ѷʵʱ�������ݷ�������ȡʵʱ���ݡ�ʹ������֮һ���ɡ�
//
/////////////////////////////////////////////////////////////////////////////////
bool CMarket::TaskGetRTDataFromWeb(void) {
  static int siCountDownTengxunNumber = 5;
  static int siCountDownNeteaseNumber = 5;

  if (m_fUsingSinaRTDataReceiver) {
    gl_SinaWebRTData.GetWebData(); // ÿ400����(100X4)����һ��ʵʱ���ݡ����˵�ʵʱ�����������Ӧʱ�䲻����100���루30-70֮�䣩����û�г��ֹ����ݴ���
  }

  if (SystemReady()) {
    // ����ʵʱ�����д�����ȱʧ�ֶΣ���ǰ׺��׺Ҳ��ʱȱʧ����ʱֹͣʹ�á�
    // ����ʵʱ������ʱ������û��Ҫ����Ĺ�Ʊ����֪Ϊ�Ρ�
    if (m_fUsingNeteaseRTDataReceiver) {
      if (siCountDownNeteaseNumber <= 0) {
        // ��ȡ����ʵʱ�������ݡ���������ʵʱ���������˵�����Դ��ͬ���ʶ����߿ɻ�����ʹ����һ���ɡ�
        gl_NeteaseWebRTData.GetWebData(); // Ŀǰ��ʹ�ô˹��ܡ�
        siCountDownNeteaseNumber = 5;
      }
      else siCountDownNeteaseNumber--;
    }
    // ��ȡ��Ѷʵʱ�������ݡ� ������Ѷʵʱ����Ĺ�������Ϊ�֣�û�������Ϣ�������޷�������ʵʱ�������ݶԽӣ����˾���Ϊ�ɣ����ʶ���ʱ����
    if (m_fReadingTengxunRTData) {
      if (siCountDownTengxunNumber <= 0) {
        gl_TengxunWebRTData.GetWebData();// ֻ�е�ϵͳ׼����Ϻ󣬷���ִ�ж�ȡ��Ѷʵʱ�������ݵĹ�����Ŀǰ��ʹ�ô˹���
        siCountDownTengxunNumber = 5;
      }
      else siCountDownTengxunNumber--; // ����ʵʱ���ݶ�ȡ��Σ���Ѷ�Ŷ�ȡһ�Ρ���Ϊ��Ѷ�Ĺҵ��������õ���ÿ�ֱ�׼�����Ȳ���
    }
  }
  return true;
}

bool CMarket::GetNeteaseWebDayLineData(void) {
  // ץȡ��������.
  // ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
  switch (gl_cMaxSavingOneDayLineThreads) {
  case 8: case 7: case 6:
  gl_NeteaseWebDayLineDataSix.GetWebData(); // ����������ʷ����
  case 5:
  gl_NeteaseWebDayLineDataFive.GetWebData();
  case 4:
  gl_NeteaseWebDayLineDataFourth.GetWebData();
  case 3:
  gl_NeteaseWebDayLineDataThird.GetWebData();
  case 2:
  gl_NeteaseWebDayLineDataSecond.GetWebData();
  case 1: case 0:
  gl_NeteaseWebDayLineData.GetWebData();
  break;
  default:
  gl_NeteaseWebDayLineData.GetWebData();
  TRACE(_T("Out of range in Get Newease DayLine Web Data\n"));
  break;
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
bool CMarket::SchedulingTaskPerSecond(long lSecondNumber) {
  static int s_iCountDownProcessWebRTData = 0;
  const long lCurrentTime = gl_systemTime.GetTime();

  // �����ȳ��򰴼��ʱ���С˳�����У����ʱ�䳤�ı���λ�ڼ��ʱ��̵�֮ǰ��
  SchedulingTaskPerHour(lSecondNumber, lCurrentTime);
  SchedulingTaskPer5Minutes(lSecondNumber, lCurrentTime);
  SchedulingTaskPer1Minute(lSecondNumber, lCurrentTime);
  SchedulingTaskPer10Seconds(lSecondNumber, lCurrentTime);

  if (s_iCountDownProcessWebRTData <= 0) {
    // �����յ���ʵʱ���ݷַ�������ع�Ʊ��ʵʱ���ݶ����С�
    // �����ж������Դ���ʶ���Ҫ�ȴ�������Դ��ִ��һ�κ󣬷����Էַ�����ع�Ʊ�����ʶ���Ҫÿ����ִ��һ�Σ��Ա�֤������Դ���ٶ����ṩһ�����ݡ�
    TaskDistributeSinaRTDataToProperStock();
    TaskProcessTengxunRTData();
    TaskProcessNeteaseRTData();
    s_iCountDownProcessWebRTData = 0;
  }
  else s_iCountDownProcessWebRTData--;

  // ����ʵʱ���ݣ�ÿ����һ�Ρ�Ŀǰ����ʵʱ����Ϊÿ3����һ�θ��£��ʶ������ٿ��ˡ�
  // �˼�������Ҫ��DistributeRTDataReceivedFromWebToProperStock֮��ִ�У��Է�ֹ����ͬ�����⡣
  if (SystemReady() && !gl_ThreadStatus.IsSavingTempData() && IsTodayTempRTDataLoaded()) { // ��ϵͳ�洢��ʱ����ʱ����ͬʱ����ʵʱ���ݣ��������׳���ͬ�����⡣
    if (gl_ThreadStatus.IsRTDataNeedCalculate()) {
      gl_ThreadStatus.SetCalculatingRTData(true);
      if (gl_ThreadStatus.IsRTDataNeedCalculate()) { // ֻ���г���ʼ̬���úú󣬲�������ʵʱ���ݡ�
        gl_ChinaStockMarket.ProcessRTData();
        gl_ThreadStatus.SetRTDataNeedCalculate(false);
      }
      gl_ThreadStatus.SetCalculatingRTData(false);
    }
  }

  return true;
}

bool CMarket::SchedulingTaskPerHour(long lSecondNumber, long lCurrentTime) {
  static int i1HourCounter = 3599; // һСʱһ�εļ�����

  // ����ÿһСʱһ�ε�����
  if (i1HourCounter <= 0) {
    i1HourCounter = 3599;
  }
  else i1HourCounter -= lSecondNumber;

  return true;
}

bool CMarket::SchedulingTaskPer5Minutes(long lSecondNumber, long lCurrentTime) {
  static int i5MinuteCounter = 299; // �����һ�εļ�����

  // ����ÿ�����һ�ε�����
  if (i5MinuteCounter <= 0) {
    i5MinuteCounter = 299;

    // �Զ���ѯcrweber.com
    gl_CrweberIndexWebData.GetWebData();
    TaskProcessWebRTDataGetFromCrweberdotcom();

    ResetSystemFlagAtMidnight(lCurrentTime);
    SaveTempDataIntoDB(lCurrentTime);
  } // ÿ�����һ�ε�����
  else i5MinuteCounter -= lSecondNumber;

  return true;
}

void CMarket::ResetSystemFlagAtMidnight(long lCurrentTime) {
  // ��ҹ�������ø��ֱ�ʶ
  if (lCurrentTime <= 1500 && !m_fPermitResetSystem) {  // ����㵽���ʮ��֣�����ϵͳ��ʶ
    m_fPermitResetSystem = true;
    CString str;
    str = _T(" ����ϵͳ���ñ�ʶ");
    gl_systemMessage.PushInformationMessage(str);
  }
}

void CMarket::SaveTempDataIntoDB(long lCurrentTime) {
  // ����ʱÿ����Ӵ洢һ�ε�ǰ״̬������һ�����ô�ʩ����ֹ�˳�ϵͳ��Ͷ��������е����ݣ�����̫Ƶ����
  if (m_fSystemReady) {
    if (m_fMarketOpened && !gl_ThreadStatus.IsCalculatingRTData()) {
      if (((lCurrentTime > 93000) && (lCurrentTime < 113600)) || ((lCurrentTime > 130000) && (lCurrentTime < 150600))) { // �洢��ʱ�����ϸ��ս���ʱ����ȷ��(�м������ڼ�ͱ��к��Ҫ�洢һ�Σ��ʶ���11:36��15:06����ֹ��
        CString str;
        str = _T(" �洢��ʱ����");
        gl_systemMessage.PushInformationMessage(str);
        UpdateTempRTData();
      }
    }
  }
}

bool CMarket::SchedulingTaskPer1Minute(long lSecondNumber, long lCurrentTime) {
  static int i1MinuteCounter = 59;  // һ����һ�εļ�����

  // ����ÿ����һ�ε��������еĶ�ʱ����Ҫ����ʱ�����ӳ��������У�����ִ��ÿ����һ�ε�������ִ��ÿ����һ�ε����������ܹ���֤���������������ִ�С�
  if (i1MinuteCounter <= 0) {
    i1MinuteCounter = 59; // ���ü�����

    // �����á�ÿСʱ�Զ���ѯcrweber.com
    //gl_CrweberIndexWebData.GetWebData();

    TaskResetSystem(lCurrentTime);
    TaskResetSystemAgain(lCurrentTime);

    // �ж��й���Ʊ�г�����״̬
    TaskCheckMarketOpen(lCurrentTime);

    // �ڿ���ǰ��������ͣʱ��ѯ���й�Ʊ�أ��ҵ������Ծ��Ʊ��
    TaskSetCheckTodayActiveStockFlag(lCurrentTime);

    // �����������ֿ�ʼ������ʵʱ���ݡ�
    TaskCompileTodayStock(lCurrentTime);
  } // ÿһ����һ�ε�����
  else i1MinuteCounter -= lSecondNumber;

  return true;
}

void CMarket::TaskSetCheckTodayActiveStockFlag(long lCurrentTime) {
  if (((lCurrentTime >= 91500) && (lCurrentTime < 92900)) || ((lCurrentTime >= 113100) && (lCurrentTime < 125900))) {
    m_fCheckTodayActiveStock = true;
  }
  else m_fCheckTodayActiveStock = false;
}

bool CMarket::TaskCompileTodayStock(long lCurrentTime) {
  if ((lCurrentTime >= 150300) && !IsTodayStockCompiled()) {
    if (SystemReady()) {
      AfxBeginThread(ThreadCompileCurrentTradeDayStock, nullptr);
      SetTodayStockCompiledFlag(true);
      return true;
    }
  }
  return false;
}

bool CMarket::TaskUpdateStockCodeDB(void) {
  if (m_fSaveDayLine && (m_iDayLineNeedSave <= 0) && (m_iDayLineNeedUpdate <= 0) && (m_iDayLineNeedProcess <= 0)) {
    if ((m_iDayLineNeedSave < 0) || (m_iDayLineNeedUpdate < 0) || (m_iDayLineNeedProcess < 0)) {
      gl_systemMessage.PushInnerSystemInformationMessage("������ʷ���ݴ�������г�����覴�");
    }
    m_fSaveDayLine = false;
    TRACE("������ʷ���ݸ������\n");
    CString str;
    str = _T("������ʷ���ݸ������");
    gl_systemMessage.PushInformationMessage(str);
    if (IsUpdateStockCodeDB()) {
      // ���¹�Ʊ�����ݿ�
      AfxBeginThread(ThreadUpdateStockCodeDB, nullptr);
      ClearUpdateStockCodeDBFlag();
    }
  }
  return true;
}

bool CMarket::TaskCheckMarketOpen(long lCurrentTime) {
  if (!gl_systemTime.IsWorkingDay()) { //�����������ձ��С��ṹtm��0--6��ʾ��������������
    m_fMarketOpened = false;
    return(m_fMarketOpened);
  }
  else if ((lCurrentTime < 91500) || (lCurrentTime > 150630) || ((lCurrentTime > 113500) && (lCurrentTime < 125500))) { //��������������ʮ���г����׽�����Ϊ�˱�֤���һ����ʱ���ݵĴ洢��
    m_fMarketOpened = false;
    return(m_fMarketOpened);
  }
  else m_fMarketOpened = true;
  return m_fMarketOpened;
}

bool CMarket::TaskResetSystem(long lCurrentTime) {
  // �ŵ�ʮ��������ϵͳ
// �����ڴ�ʱ������������������Ļ����׳������ݲ�ȫ�����⡣
  if (m_fPermitResetSystem) { // �����������ϵͳ
    if ((lCurrentTime >= 91300) && (lCurrentTime <= 91400) && gl_systemTime.IsWorkingDay()) { // �����վŵ�ʮ�������ϵͳ
      gl_fResetSystem = true;     // ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
      m_fSystemReady = false;
    }
  }
  return true;
}

bool CMarket::TaskResetSystemAgain(long lCurrentTime) {
  // �ŵ��ʮ����ٴ�����ϵͳ
  if (m_fPermitResetSystem) { // �����������ϵͳ
    if ((lCurrentTime >= 92500) && (lCurrentTime <= 93000) && gl_systemTime.IsWorkingDay()) { // �����վŵ�ʮ�������ϵͳ
      gl_fResetSystem = true;     // ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
      m_fSystemReady = false;
      m_fPermitResetSystem = false; // ���첻����������ϵͳ��
    }
  }
  return true;
}

bool CMarket::SchedulingTaskPer10Seconds(long lSecondNumber, long lCurrentTime) {
  static int i10SecondsCounter = 9; // ʮ��һ�εļ�����

  // ����ÿʮ����һ�ε�����
  if (i10SecondsCounter <= 0) {
    i10SecondsCounter = 9;
    // do something

    // ������������ʷ���ݵĺ�����Ϊ��ʱ��ѯ����ȡ�ʹ洢���ù������̡�
    if (m_iDayLineNeedProcess > 0) {
      ProcessDayLineGetFromNeeteaseServer();
    }

    // �ж��Ƿ�洢���߿�͹�Ʊ�����
    if ((m_iDayLineNeedSave > 0)) {
      m_fSaveDayLine = true;
      gl_ChinaStockMarket.SaveDayLineData();
    }

    TaskUpdateStockCodeDB();
  } // ÿʮ����һ�ε�����
  else i10SecondsCounter -= lSecondNumber;

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �õ���Ʊ�ļ��
//
//////////////////////////////////////////////////////////////////////////////////////////////
CString CMarket::GetStockName(CString strStockCode) {
  try {
    return (m_vChinaMarketAStock.at(m_mapChinaMarketAStock.at(strStockCode))->GetStockName());
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
bool CMarket::GetStockIndex(CString strStockCode, long& lIndex) {
  try {
    lIndex = m_mapChinaMarketAStock.at(strStockCode);
    return true;
  }
  catch (exception&) {
    TRACE("GetStockIndex�����쳣\n");
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
CStockPtr CMarket::GetStockPtr(CString strStockCode) {
  try {
    return (m_vChinaMarketAStock.at(m_mapChinaMarketAStock.at(strStockCode)));
  }
  catch (exception&) {
    TRACE("GetStockPtr�����쳣\n");
    return nullptr;
  }
}

CStockPtr CMarket::GetStockPtr(long lIndex) {
  try {
    return m_vChinaMarketAStock.at(lIndex);
  }
  catch (exception&) {
    TRACE("GetStockPtr�����쳣\n");
    return nullptr;
  }
}

void CMarket::IncreaseActiveStockNumber(void) {
  m_lTotalActiveStock++;
}

//////////////////////////////////////////////////////////////////////////
//
// ���õ�ǰ�����Ĺ�Ʊ
//
// ������Ӧ�Ĺ�Ʊָ�룬װ�����������ݡ�
//
/////////////////////////////////////////////////////////////////////////
void CMarket::SetShowStock(CStockPtr pStock) {
  if (m_pCurrentStock != pStock) {
    m_pCurrentStock = pStock;
    m_fCurrentStockChanged = true;
    m_pCurrentStock->SetDayLineLoaded(false);
    AfxBeginThread(ThreadLoadDayLine, 0);
  }
}

bool CMarket::IsCurrentStockChanged(void) {
  if (m_fCurrentStockChanged) {
    m_fCurrentStockChanged = false;
    return true;
  }
  else return false;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//	ͨ����Ʊ������г��������õ�ǰѡ���Ʊ
//
//////////////////////////////////////////////////////////////////////////////////////
void CMarket::SetShowStock(CString strStockCode) {
  m_pCurrentStock = GetStockPtr(strStockCode);
  ASSERT(m_pCurrentStock != NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	���������ݴ������ݿ⣮
//  �˺����ɹ����߳�ThreadDayLineSaveProc���ã�������Ҫʹ��ȫ�ֱ�����
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveDayLineData(void) {
  CString str;
  strTransferSharedPtr* pTransfer = nullptr;

  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsDayLineNeedSavingAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
      if (pStock->m_vDayLine.size() > 0) {
        pTransfer = new strTransferSharedPtr; // �˴����ɣ����̸߳���delete
        pTransfer->m_pStock = pStock;
        AfxBeginThread(ThreadSaveDayLineOfOneStock, (LPVOID)pTransfer, THREAD_PRIORITY_LOWEST);
      }
      else { // �������Ϊ�й�Ʊ���룬���˴�����δ����
        CString str1 = pStock->GetStockCode();
        str1 += _T(" Ϊδ���й�Ʊ����");
        gl_systemMessage.PushDayLineInfoMessage(str1);
      }
    }
    if (gl_ExitingSystem) {
      break; // ������������˳�����ֹͣ�洢��
    }
  }

  return(true);
}

bool CMarket::ClearAllDayLineVector(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    pStock->m_vDayLine.clear();
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��ʵʱ���ݴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveRTData(void) {
  CSetRealTimeData setRTData;
  setRTData.m_strFilter = _T("[ID] = 1");

  setRTData.Open();
  setRTData.m_pDatabase->BeginTrans();
  setRTData.m_pDatabase->CommitTrans();
  setRTData.Close();
  return(true);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��crweber.com����ָ�����ݴ������ݿ⡣
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveCrweberIndexData(void) {
  CSetCrweberIndex setIndex;
  setIndex.m_strFilter = _T("[ID] = 1");

  // �洢�������ɵ�������CrweberIndex���С�
  setIndex.Open();
  setIndex.m_pDatabase->BeginTrans();
  gl_CrweberIndex.AppendData(setIndex);
  setIndex.m_pDatabase->CommitTrans();
  setIndex.Close();
  return(true);
}

bool CMarket::IsDayLineNeedSaving(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsDayLineNeedSaving()) {
      ASSERT(pStock->IsActive());
      return true;
    }
  }
  return false;
}

bool CMarket::IsDayLineNeedUpdate(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsDayLineNeedUpdate()) return true;
  }
  return false;
}

bool CMarket::ProcessDayLineGetFromNeeteaseServer(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsDayLineNeedProcess()) {
      pStock->ProcessNeteaseDayLineData();
      pStock->SetDayLineNeedProcess(false);
    }
  }
  return true;
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
long CMarket::CompileCurrentTradeDayStock(long lCurrentTradeDay) {
  char buffer[20];
  CString strDay;
  CSetDayLine setDayLine;
  CSetDayLineInfo setDayLineInfo;
  long iCount = 0;

  CString str;
  str = _T("��ʼ�������½����յ�ʵʱ����");
  gl_systemMessage.PushInformationMessage(str);

  // �洢��ǰ�����յ�����
  _ltoa_s(lCurrentTradeDay, buffer, 10);
  strDay = buffer;
  setDayLine.m_strFilter = _T("[Day] =");
  setDayLine.m_strFilter += strDay;
  setDayLine.Open();
  setDayLine.m_pDatabase->BeginTrans();
  while (!setDayLine.IsEOF()) {
    setDayLine.Delete();
    setDayLine.MoveNext();
  }
  setDayLine.m_pDatabase->CommitTrans();

  setDayLine.m_pDatabase->BeginTrans();
  for (auto pStock : m_vChinaMarketAStock) {
    if (!pStock->IsTodayDataActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    iCount++;
    pStock->SetDayLineEndDay(lCurrentTradeDay);
    pStock->SetIPOStatus(__STOCK_IPOED__); // ������һ�Ρ���ֹ�¹ɹ�Ʊ��������û����ʷ���ݶ�������Ϊ�����ڡ�
    setDayLine.AddNew();
    pStock->SaveBasicInfo(setDayLine);
    setDayLine.Update();
  }
  setDayLine.m_pDatabase->CommitTrans();
  setDayLine.Close();

  // �洢�������ɵ�������DayLineInfo���С�
  setDayLineInfo.m_strFilter = _T("[Day] =");
  setDayLineInfo.m_strFilter += strDay;
  setDayLineInfo.Open();
  setDayLineInfo.m_pDatabase->BeginTrans();
  while (!setDayLineInfo.IsEOF()) {
    setDayLineInfo.Delete();
    setDayLineInfo.MoveNext();
  }
  setDayLineInfo.m_pDatabase->CommitTrans();

  setDayLineInfo.m_pDatabase->BeginTrans();
  for (auto pStock : m_vChinaMarketAStock) {
    if (!pStock->IsTodayDataActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    setDayLineInfo.AddNew();
    pStock->SaveCalculatedInfo(setDayLineInfo);
    setDayLineInfo.Update();
  }
  setDayLineInfo.m_pDatabase->CommitTrans();
  setDayLineInfo.Close();

  gl_systemMessage.PushInformationMessage(_T("���½�����ʵʱ���ݴ������"));
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
bool CMarket::UpdateTodayTempDB(void) {
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
  for (auto pStock : m_vChinaMarketAStock) {
    if (!pStock->IsTodayDataActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    if (pStock->GetVolume() != pStock->GetOrdinaryBuyVolume() + pStock->GetOrdinarySellVolume() + pStock->GetAttackBuyVolume()
        + pStock->GetAttackSellVolume() + pStock->GetStrongBuyVolume() + pStock->GetStrongSellVolume() + pStock->GetUnknownVolume()) {
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
bool CMarket::LoadTodayTempDB(void) {
  CStockPtr pStock = nullptr;
  CSetDayLineToday setDayLineToday;
  CRTDataPtr pRTData;

  ASSERT(!m_fTodayTempDataLoaded);
  ASSERT(!gl_ThreadStatus.IsCalculatingRTData());    // ִ�д˳�ʼ������ʱ������ʵʱ���ݵĹ����̱߳���û�����С�
  // ��ȡ�������ɵ�������DayLineToday���С�
  setDayLineToday.Open();
  if (!setDayLineToday.IsEOF()) {
    if (setDayLineToday.m_Day == gl_systemTime.GetDay()) { // ����ǵ�������飬�����룬�������
      while (!setDayLineToday.IsEOF()) {
        if ((pStock = GetStockPtr(setDayLineToday.m_StockCode)) != nullptr) {
          ASSERT(!pStock->HaveFirstRTData()); // ȷ��û�п�ʼ����ʵʱ����
          // ��Ҫ����m_lUnknownVolume = pRTData->m_lVolume - setDayLineToday.m_Volume + setDayLineToday.m_UnknownVolume
          // ����һ��ִ�м���ʵʱ����ʱ��ֻ�ǳ�ʼ��ϵͳ��������������m_lUnknownVolume += pRTData->GetVolume
          // �ʶ�LoadAndCalculateTempInfo��Ҫ�ر���
          pStock->LoadAndCalculateTempInfo(setDayLineToday);
        }
        setDayLineToday.MoveNext();
      }
    }
  }
  setDayLineToday.Close();

  return true;
}

///////////////////////////////////////////////////////////////////////////////////
//
// ����lDay���������ǿ��, lDay�ĸ�ʽΪ��YYYYMMDD,�� 19990605.
// �����߰��ǵ����к�,�����ǿ�������ڶ����е�λ��
//
//////////////////////////////////////////////////////////////////////////////////
bool CMarket::CalculateOneDayRelativeStrong(long lDay) {
  vector<CStockPtr> vStock;
  vector<int> vIndex;
  vector<double> vRelativeStrong;
  int iTotalAShare = 0;
  CString strSQL;
  CString strDay;
  char  pch[30];
  int iStockNumber = 0, j = 0;
  CTime ctTime;
  CSetDayLine setDayLine;
  const long lYear = lDay / 10000;
  const long lMonth = lDay / 100 - lYear * 100;
  const long lDayOfMonth = lDay - lYear * 10000 - lMonth * 100;
  char buffer[100];

  for (j = 0; j < 30; j++) pch[j] = 0x000;

  _ltoa_s(lDay, pch, 10);
  strDay = pch;
  setDayLine.m_strSort = _T("[UpDownRate]");
  setDayLine.m_strFilter = _T("[Day] =");
  setDayLine.m_strFilter += strDay;
  setDayLine.Open();
  if (setDayLine.IsEOF()) { // ���ݼ�Ϊ�գ���������û�н���
    setDayLine.Close();
    return false;
  }
  setDayLine.m_pDatabase->BeginTrans();
  iStockNumber = 0;
  while (!setDayLine.IsEOF()) {
    if (gl_ChinaStockMarket.IsAStock(setDayLine.m_StockCode)) {
      long lIndex = m_mapChinaMarketAStock.at(setDayLine.m_StockCode);
      vStock.push_back(m_vChinaMarketAStock.at(lIndex));
      vIndex.push_back(iStockNumber); // ��A�ɵ�������¼�������С�
      iTotalAShare++;
    }
    iStockNumber++;
    setDayLine.MoveNext();
  }

  setDayLine.MoveFirst();
  int iCount = 0;
  int iBefore = 0;
  while (iCount < vIndex.size()) { // ֻ�����Ծ��Ʊ�����ǿ��
    for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) { // ��������ȥ�������ݿ�,�������A�ɵĹ�Ʊ
      setDayLine.MoveNext();
    }
    setDayLine.Edit();
    double dLastClose = atof(setDayLine.m_LastClose);
    double dLow = atof(setDayLine.m_Low);
    double dHigh = atof(setDayLine.m_High);
    double dClose = atof(setDayLine.m_Close);
    if (((dLow / dLastClose) < 0.88)
        || ((dHigh / dLastClose) > 1.12)) { // ��Ȩ���¹����е�
      setDayLine.m_RelativeStrong = ConvertValueToString(50); // �¹����л��߳�Ȩ��Ϣ��������˹�
    }
    else if ((fabs(dHigh - dClose) < 0.0001)
             && (((dClose / dLastClose)) > 1.095)) { // ��ͣ��
      setDayLine.m_RelativeStrong = ConvertValueToString(100);
    }
    else if ((fabs(dClose - dLow) < 0.0001)
             && ((dClose / dLastClose) < 0.905)) { // ��ͣ��
      setDayLine.m_RelativeStrong = ConvertValueToString(0);
    }
    else {
      setDayLine.m_RelativeStrong = ConvertValueToString((static_cast<double>(iCount) * 100) / iTotalAShare);
    }
    setDayLine.Update();
    iBefore = vIndex.at(iCount++);
    setDayLine.MoveNext(); // �Ƶ���һ�����ݡ�
    iBefore++; // ������Ҳͬʱ��һ��
  }
  setDayLine.m_pDatabase->CommitTrans();
  setDayLine.Close();

  vStock.clear();
  vIndex.clear();
  vRelativeStrong.clear();

  sprintf_s(buffer, "%4d��%2d��%2d�յĹ�Ʊ���ǿ�ȼ������", lYear, lMonth, lDayOfMonth);
  CString strTemp;
  strTemp = buffer;
  gl_systemMessage.PushDayLineInfoMessage(strTemp);    // ����ͬ�����Ʊ�����Ϣ

  return(true);
}

bool CMarket::UpdateStockCodeDB(void) {
  CSetStockCode setStockCode;

  setStockCode.Open();
  setStockCode.m_pDatabase->BeginTrans();
  while (!setStockCode.IsEOF()) {
    setStockCode.Delete();
    setStockCode.MoveNext();
  }
  setStockCode.m_pDatabase->CommitTrans();
  setStockCode.m_pDatabase->BeginTrans();
  for (auto pStock : m_vChinaMarketAStock) {
    pStock->AppendStockCodeDB(setStockCode);
  }
  setStockCode.m_pDatabase->CommitTrans();
  setStockCode.Close();

  return true;
}

void CMarket::LoadStockCodeDB(void) {
  CSetStockCode setStockCode;

  setStockCode.Open();
  // װ���Ʊ�������ݿ�
  while (!setStockCode.IsEOF()) {
    CStockPtr pStock = GetStockPtr(setStockCode.m_StockCode);
    pStock->LoadStockCodeDB(setStockCode);
    setStockCode.MoveNext();
  }
  if (gl_ChinaStockMarket.m_iDayLineNeedUpdate > 0) {
    int i = gl_ChinaStockMarket.m_iDayLineNeedUpdate;
    TRACE("����%d����Ʊ��Ҫ������������\n", i);
    //gl_systemMessage.PushInformationMessage(_T("�����������.."));
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
bool CMarket::UpdateOptionDB(void) {
  CSetOption setOption;
  setOption.Open();
  setOption.m_pDatabase->BeginTrans();
  if (setOption.IsEOF()) {
    setOption.AddNew();
    setOption.m_RelativeStrongEndDay = gl_ChinaStockMarket.GetRelativeStrongEndDay();
    setOption.m_RalativeStrongStartDay = gl_ChinaStockMarket.GetRelativeStrongStartDay();
    setOption.m_LastLoginDay = gl_systemTime.GetDay();
    setOption.Update();
  }
  else {
    setOption.Edit();
    setOption.m_RelativeStrongEndDay = gl_ChinaStockMarket.GetRelativeStrongEndDay();
    setOption.m_RalativeStrongStartDay = gl_ChinaStockMarket.GetRelativeStrongStartDay();
    setOption.m_LastLoginDay = gl_systemTime.GetDay();
    setOption.Update();
  }
  setOption.m_pDatabase->CommitTrans();
  setOption.Close();
  return false;
}

void CMarket::LoadOptionDB(void) {
  CSetOption setOption;
  setOption.Open();
  if (setOption.IsEOF()) {
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900101);
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900101);
    gl_ChinaStockMarket.SetLastLoginDay(19900101);
  }
  else {
    if (setOption.m_RelativeStrongEndDay == 0) {
      gl_ChinaStockMarket.SetRelativeStrongEndDay(19900101);
    }
    else {
      gl_ChinaStockMarket.SetRelativeStrongEndDay(setOption.m_RelativeStrongEndDay);
      if (gl_ChinaStockMarket.GetRelativeStrongEndDay() > 19900101) {
        // ��������ʷ���ݿ��д��ھ�����ʱ�����õ��߳�ģʽ�洢�����ݡ�ʹ�ö��߳�ģʽʱ��MySQL����ֻ�����Exception�����������ݿ�����ʱ����ͬ�����⣩��
        gl_SaveOneStockDayLine.SetMaxCount(1);
      }
    }
    if (setOption.m_RalativeStrongStartDay == 0) {
      gl_ChinaStockMarket.SetRelativeStrongStartDay(19900101);
    }
    else {
      gl_ChinaStockMarket.SetRelativeStrongStartDay(setOption.m_RalativeStrongStartDay);
    }
    if (setOption.m_LastLoginDay == 0) {
      gl_ChinaStockMarket.SetLastLoginDay(19900101);
    }
    else {
      gl_ChinaStockMarket.SetLastLoginDay(setOption.m_LastLoginDay);
    }
  }

  setOption.Close();
}

bool CMarket::UpdateTempRTData(void) {
  if (!gl_ThreadStatus.IsSavingTempData()) {
    gl_ThreadStatus.SetSavingTempData(true);
    AfxBeginThread(ThreadSaveTempRTData, nullptr);
  }

  return false;
}