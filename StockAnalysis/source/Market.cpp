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

  time_t ttime;
  time(&ttime);
  tm tm_;
  localtime_s(&tm_, &ttime);
  if (tm_.tm_hour >= 15) { // �й���Ʊ�г��Ѿ�����
    m_fTodayStockCompiled = true; // ���к��ִ�б�ϵͳ������Ϊ�Ѿ���������չ�Ʊ�����ˡ�
  }
  else m_fTodayStockCompiled = false;

  m_lRelativeStrongEndDay = m_lRelativeStrongStartDay = m_lLastLoginDay = 19900101;

  m_fGetDayLineData = m_fSaveDayLine = false;

  m_fTodayTempDataLoaded = false;

  m_fCheckTodayActiveStock = true;  //��鵱�ջ�Ծ��Ʊ������Ϊ�档

  m_fUpdatedStockCodeDataBase = false;

  m_fCalculatingRS = false;

  m_fGetRTStockData = true;
  m_fReadingTengxunRTData = true; // Ĭ��״̬�²���ȡ��Ѷʵʱ����
  m_iCountDownDayLine = 3;    // 400ms��ʱ��100msÿ�Σ�
  m_iCountDownSlowReadingRTData = 3; // 400����ÿ��

  m_fUsingSinaRTDataReceiver = true; // ʹ������ʵʱ������ȡ��
  m_fUsingNeteaseRTDataReceiver = false; // ʹ������ʵʱ������ȡ��
  m_fUsingNeteaseRTDataReceiverAsTester = false;
  m_fUsingTengxunRTDataReceiverAsTester = true;

  // ���ɹ�Ʊ�����
  CreateTotalStockContainer();

  // ���ô�ȫ�ֱ���
  gl_CrweberIndex.Reset();
  ResetIT(); // ����ʵʱ��Ʊ�����ѯ������
}

#ifdef _DEBUG
void CMarket::AssertValid() const {
  CObject::AssertValid();
}

void CMarket::Dump(CDumpContext& dc) const {
  CObject::Dump(dc);
}
#endif //_DEBUG

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
  }

  return true;
}

void CMarket::ResetIT(void) {
  m_itSinaStock = m_vChinaMarketAStock.begin();
  m_itTengxunStock = m_vChinaMarketAStock.begin();
  m_itNeteaseStock = m_vChinaMarketAStock.begin();
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
bool CMarket::CreateNeteaseDayLineInquiringStr(CString& str, CString& strStartDay) {
  static int iStarted = 0;
  static int siCounter = 0;

  long lTotalStock;

  lTotalStock = m_vChinaMarketAStock.size();

  bool fFound = false;
  int iCount = 0;
  CString strTemp;
  while (!fFound && (iCount < 1000)) {
    if (!m_vChinaMarketAStock.at(siCounter)->IsDayLineNeedUpdate()) { // �������ݲ���Ҫ���¡���ϵͳ��ʼʱ�����ô�m_fDayLineNeedUpdate��ʶ
      // TRACE("%S ���������������\n", static_cast<LPCWSTR>(m_vChinaMarketAStock.at(siCounter)->m_strStockCode));
      siCounter++;
      iCount++;
      if (siCounter == lTotalStock) {
        siCounter = 0;
      }
    }
    else if (m_vChinaMarketAStock.at(siCounter)->GetIPOStatus() == __STOCK_NULL__) {	// ��δʹ�ù��Ĺ�Ʊ���������ѯ��������
      m_vChinaMarketAStock.at(siCounter)->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
      // TRACE("��Ч��Ʊ���룺%S, �����ѯ��������\n", static_cast<LPCWSTR>(m_vChinaMarketAStock.at(siCounter)->m_strStockCode));
      iCount++;
      siCounter++;
      if (siCounter == lTotalStock) {
        siCounter = 0;
      }
    }
    else if (m_vChinaMarketAStock.at(siCounter)->GetDayLineEndDay() >= gl_systemTime.GetLastTradeDay()) { // ��һ�����յ����������Ѿ��洢����ʱ�Ѿ������һ�����������ˣ������ٴδ���
      m_vChinaMarketAStock.at(siCounter)->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
      // TRACE("%S �������ݱ����Ѹ���\n", static_cast<LPCWSTR>(m_vChinaMarketAStock.at(siCounter)->m_strStockCode));
      siCounter++;
      iCount++;
      if (siCounter == lTotalStock) {
        siCounter = 0;
      }
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
  CStockPtr pID = m_vChinaMarketAStock.at(siCounter);
  pID->SetInquiringOnce(true);
  switch (pID->GetMarket()) { // ת����������������������ʽ���Ϻ�Ϊ��0��������Ϊ��1����
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
  case __SHENZHEN_3BAN__: // ɽ����С��
  case __SHENZHEN_B_SHARE__: // ����B��
  case __SHENZHEN_CHUANGYE__: // ���ڴ�ҵ��
  case __SHENZHEN_MAIN__: // ��������
  str += '1'; // �����г���ʶ
  break;
  default: // Խ��
  ASSERT(0);
  }
  char buffer[30];
  str += pID->GetStockCode().Right(6); // ȡ��Ʊ������ұ���λ���֡�
  tm tm_;
  tm_.tm_year = pID->GetDayLineEndDay() / 10000 - 1900;
  tm_.tm_mon = pID->GetDayLineEndDay() / 100 - (tm_.tm_year + 1900) * 100 - 1;
  tm_.tm_mday = pID->GetDayLineEndDay() - (tm_.tm_year + 1900) * 10000 - (tm_.tm_mon + 1) * 100;
  tm_.tm_hour = 12;
  tm_.tm_min = 0;
  tm_.tm_sec = 0;
  time_t tTime;
  tTime = mktime(&tm_);
  tTime += 3600 * 24; // ����һ�졣
  localtime_s(&tm_, &tTime);
  const long lDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday; // ���ߵ���ʼ�������������ڵĺ�һ�졣
  sprintf_s(buffer, "%8d", lDay);
  strStartDay = buffer;
  siCounter++;
  if (siCounter == lTotalStock) {
    siCounter = 0;
  }
  return true;
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
bool CMarket::DistributeSinaRTDataToProperStock(void) {
  CSingleLock sl(&gl_ProcessSinaRTDataQueue);
  if (sl.IsLocked()) {
    CStockPtr pStock;
    const long lTotalNumber = gl_QueueSinaRTData.GetRTDataSize();
    CString strVolume;

    for (int iCount = 0; iCount < lTotalNumber; iCount++) {
      CRTDataPtr pRTData = gl_QueueSinaRTData.PopRTData();
      if (pRTData->GetDataSource() == __INVALID_RT_WEB_DATA__) {
        gl_systemMessage.PushInnerSystemInformationMessage(_T("ʵʱ����Դ��������"));
      }
      if (pRTData->IsActive()) { // ��ʵʱ������Ч��
        long lIndex = m_mapChinaMarketAStock.at(pRTData->GetStockCode());
        pStock = m_vChinaMarketAStock.at(lIndex);
        if (!pStock->IsActive()) {
          if (pRTData->IsDataTimeAtCurrentTradingDay()) {
            pStock->SetActive(true);
            pStock->SetStockName(pRTData->GetStockName());
            pStock->SetStockCode(pRTData->GetStockCode());
            pStock->UpdateStatus(pRTData);
            pStock->SetTransactionTime(pRTData->GetTransactionTime());
            pStock->SetDayLineNeedUpdate(true);
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
  }
  sl.Unlock();

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ÿ�β�ѯ����ʵʱ��Ʊ���ݵ��ַ���
//
//////////////////////////////////////////////////////////////////////////////////////////
int CMarket::GetSinaInquiringStockStr(CString& str, long lTotalNumber, bool fSkipUnactiveStock) {
  //ASSERT(SystemReady());
  return GetInquiringStr(str, m_itSinaStock, _T(","), lTotalNumber, fSkipUnactiveStock);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ÿ�β�ѯ��Ѷʵʱ��Ʊ���ݵ��ַ���
//
//////////////////////////////////////////////////////////////////////////////////////////
int CMarket::GetTengxunInquiringStockStr(CString& str, long lTotalNumber, bool fSkipUnactiveStock) {
  ASSERT(SystemReady());
  return GetInquiringStr(str, m_itTengxunStock, _T(","), lTotalNumber, fSkipUnactiveStock);
}

int CMarket::GetNeteaseInquiringStockStr(CString& str, long lTotalNumber, bool fSkipUnactiveStock) {
  CString strStockCode, strRight6, strLeft2, strPrefix;
  if (fSkipUnactiveStock) StepToNextActiveStockIT(m_itNeteaseStock);
  if (m_itNeteaseStock == m_vChinaMarketAStock.end()) {
    m_itNeteaseStock = m_vChinaMarketAStock.begin();
  }
  strStockCode = (*m_itNeteaseStock++)->GetStockCode();
  strRight6 = strStockCode.Right(6);
  strLeft2 = strStockCode.Left(2);
  if (strLeft2.Compare(_T("sh")) == 0) {
    strPrefix = _T("0");
  }
  else strPrefix = _T("1");
  str += strPrefix + strRight6;  // �õ���һ����Ʊ����
  int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
  while ((m_itNeteaseStock != m_vChinaMarketAStock.end()) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
    if (fSkipUnactiveStock) StepToNextActiveStockIT(m_itNeteaseStock);
    if (m_itNeteaseStock != m_vChinaMarketAStock.end()) {
      iCount++;
      str += _T(",");
      strStockCode = (*m_itNeteaseStock++)->GetStockCode();
      strRight6 = strStockCode.Right(6);
      strLeft2 = strStockCode.Left(2);
      if (strLeft2.Compare(_T("sh")) == 0) {
        strPrefix = _T("0");
      }
      else strPrefix = _T("1");
      str += strPrefix + strRight6;  // �õ���һ����Ʊ����
    }
  }
  if (m_itNeteaseStock == m_vChinaMarketAStock.end()) {
    m_itNeteaseStock = m_vChinaMarketAStock.begin();
  }
  else {
    m_itNeteaseStock--;    // ��һ�������ܹ�����ס��Ե
  }
  return iCount;
}

int CMarket::GetInquiringStr(CString& str, vector<CStockPtr>::iterator& itStock, CString strPostfix, long lTotalNumber, bool fSkipUnactiveStock) {
  if (fSkipUnactiveStock) StepToNextActiveStockIT(itStock);
  if (itStock == m_vChinaMarketAStock.end()) {
    itStock = m_vChinaMarketAStock.begin();
  }
  str += (*itStock++)->GetStockCode();  // �õ���һ����Ʊ����
  int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
  while ((itStock != m_vChinaMarketAStock.end()) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
    if (fSkipUnactiveStock) StepToNextActiveStockIT(itStock);
    if (itStock != m_vChinaMarketAStock.end()) {
      iCount++;
      str += strPostfix;
      str += (*itStock++)->GetStockCode();
    }
  }
  if (itStock == m_vChinaMarketAStock.end()) {
    itStock = m_vChinaMarketAStock.begin();
  }
  else {
    itStock--;    // ��һ�������ܹ�����ס��Ե
  }
  return iCount;
}

bool CMarket::StepToNextActiveStockIT(vector<CStockPtr>::iterator& itStock) {
  while (!(*itStock)->IsActive()) {
    itStock++;
    if (itStock == m_vChinaMarketAStock.end()) {
      return true;
    }
  }
  return false;
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

bool CMarket::ProcessTengxunRTData(void) {
  return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// ������������߷������϶�ȡ�Ĺ�Ʊ�������ݡ�
// ������ʽΪ�� ����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ProcessNeteaseDayLineData(CNeteaseDayLineWebData* pWebData) {
  long iCount = 0;
  char* pCurrentPos = pWebData->GetBufferAddr();
  char* pTestPos = pWebData->GetBufferAddr();
  vector<CDayLinePtr> vTempDayLine;
  long lLength = pWebData->GetByteReaded();

  if (lLength == 0) { // û�����ݶ��룿����״̬�ǲ�ѯ�Ĺ�ƱΪ��Ч�������ڣ�����
    return false;
  }

  while (*pCurrentPos != 0X0d) {
    if ((*pCurrentPos == 0x0a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    pCurrentPos++;
    iCount++;
  }
  pCurrentPos++;
  if (*pCurrentPos != 0x0a) return false;
  iCount++;
  pCurrentPos++;
  iCount++;
  shared_ptr<CDayLine> pDayLine;
  long lIndex = -1;

  pTestPos = pWebData->GetBufferAddr();
  pTestPos += iCount;
  ASSERT(*pTestPos == *pCurrentPos);
  if (iCount == lLength) {// ��Ч��Ʊ���룬����ֻ��ǰ׺˵����û��ʵ����Ϣ�����������ˣ������Ѿ������ˣ��������¹����еĵ�һ��
    lIndex = m_mapChinaMarketAStock.at(pWebData->GetDownLoadingStockCode());
    // ASSERT(!m_vChinaMarketAStock[lIndex]->m_fActive); ��һ����ƱIPO����δ����ʱ����Ʊ������ڵ�û���������ݡ�ȡ���˶����жϡ�
    // ��Щ��Ʊ�����к���ֱ��չ�����������������������ݲ��ٸ��¡���������������ô˹�ƱΪ��Ч����
    if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() == 19900101) { // �����ʼ���߽������ڴ���û�б��������˹�Ʊ������δ��ʹ�ù�
      m_vChinaMarketAStock.at(lIndex)->SetIPOStatus(__STOCK_NULL__);   // �˹�Ʊ������δʹ�á�
      //TRACE("��Ч��Ʊ���룺%s\n", static_cast<LPCWSTR>(m_vChinaMarketAStock.at(Index)->GetStockCode()));
    }
    else { // �Ѿ����еĹ�Ʊ
      if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() + 100 < gl_systemTime.GetDay()) {
        m_vChinaMarketAStock.at(lIndex)->SetIPOStatus(__STOCK_DELISTED__);   // �˹�Ʊ�����Ѿ����С�
      }
      //TRACE("%S û�пɸ��µ���������\n", static_cast<LPCWSTR>(m_vChinaMarketAStock.at(Index)->GetStockCode()));
    }
    m_vChinaMarketAStock.at(lIndex)->SetDayLineNeedUpdate(false); // ������Ҫ������������
    return false;
  }

  lIndex = 0;
  long iTemp = 0;
  CString strTemp;
  CStockPtr pStock = nullptr;
  pTestPos = pWebData->GetBufferAddr();
  pTestPos += iCount;
  ASSERT(*pTestPos == *pCurrentPos);
  while (iCount < lLength) {
    pDayLine = make_shared<CDayLine>();
    if (!ProcessOneItemDayLineData(pWebData->GetDownLoadingStockCode(), pDayLine, pCurrentPos, iTemp)) { // ����һ����������
      TRACE(_T("%s �������ݳ���\n"), pDayLine->GetStockCode());
      // ������ݴ����������
      vTempDayLine.clear();
      return false; // ���ݳ�����������
    }
    lIndex = m_mapChinaMarketAStock.at(pDayLine->GetStockCode());
    pStock = m_vChinaMarketAStock.at(lIndex);
    iCount += iTemp;
    pTestPos = pWebData->GetBufferAddr();
    pTestPos += iCount;
    ASSERT(*pTestPos == *pCurrentPos);
    if (!pStock->IsActive()) { // �µĹ�Ʊ���룿
      // �����¹�Ʊ
      pStock->SetActive(true);
      pStock->SetDayLineLoaded(false);
      pStock->SetMarket(pDayLine->GetMarket());
      pStock->SetStockCode(pDayLine->GetStockCode()); // ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
      pStock->SetStockName(pDayLine->GetStockName());// ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
      strTemp = pStock->GetStockCode().Right(6); // ��ȡ��Ʊ�����ұߵ���������
      m_lTotalActiveStock++;
    }
    vTempDayLine.push_back(pDayLine); // �ݴ�����ʱvector�У���Ϊ�����������ݵ�ʱ��˳���ǵߵ��ģ����µ�����ǰ��
  }
  strTemp = pDayLine->GetStockCode();
  strTemp += _T("�����������.");
  gl_systemMessage.PushDayLineInfoMessage(strTemp);
  pStock->SetDayLineNeedUpdate(false); // ��������������ϣ�����Ҫ�����������ˡ�
  if ((vTempDayLine.at(0)->GetDay() + 100) < gl_systemTime.GetDay()) { // ��ȡ���Ĺ�Ʊ���������������ղ����ϸ��µ���������գ������˻��������������һ���µ�ʱ��۲졣
    pStock->SetIPOStatus(__STOCK_DELISTED__); // �����л���ͣ���ס�
  }
  else {
    pStock->SetIPOStatus(__STOCK_IPOED__); // �������׹�Ʊ
  }
  ASSERT(lIndex >= 0);
  pStock->m_vDayLine.clear(); // �����������������ݣ�����еĻ���
  // ������������ʱ��Ϊ�������
  for (int i = vTempDayLine.size() - 1; i >= 0; i--) {
    pDayLine = vTempDayLine.at(i);
    if (pDayLine->GetDay() < gl_systemTime.GetDay()) { // ��Ҫ�洢�����������ݣ���������������ʵʱ�������ɣ�.
      // ���¹ɵ�һ������ʱ��������ֻ��һ�죬�����������ӵ��ˣ���������������Ϊ�ա�����ʱע�⡣
      // �����ǵ�ȡgl_lLastTradeDay��֮ǰ���������ݣ��ʶ��¹ɵ����������϶�Ϊ�ա�
      pStock->m_vDayLine.push_back(pDayLine);
    }
  }
  vTempDayLine.clear();
  pStock->SetDayLineLoaded(true);
  pStock->SetDayLineNeedSavingFlag(true); // ���ô洢���߱�ʶ

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ȡһ���������ݡ���������������ʷ���ݸ�ʽ��
//
// ��ʵʱ���������ƣ����ּ۸�ԷŴ�һǧ�����Գ����ʹ洢���������ݿ�ʱ��DECIMAL(10,3)���ʹ洢��
//
// �ַ�������ʽΪ��2019-07-10,600000,�ֶ�����,���̼�,��߼�,��ͼ�,���̼�,ǰ���̼�,�ǵ�ֵ,�ǵ�����,������,�ɽ�����,�ɽ����,����ֵ,��ͨ��ֵ\r\n
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ProcessOneItemDayLineData(CString strStockCode, CDayLinePtr pDayLine, char*& pCurrentPos, long& lLength) {
  long iCount = 0;
  static char buffer2[200], buffer3[100];
  long i = 0;
  tm tm_;
  int year = 0, month = 0, day = 0;
  long lDay = 0;
  CString str;
  i = 0;
  while (*pCurrentPos != 0x2c) {
    if ((*pCurrentPos == 0x0d) || (*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false; // ���ݳ�����������
    }
    buffer3[i++] = *pCurrentPos++;
    iCount++;
  }
  pCurrentPos++;
  iCount++;
  buffer3[i] = 0x00;
  sscanf_s(buffer3, "%04d-%02d-%02d", &year, &month, &day);
  tm_.tm_year = year - 1900;
  tm_.tm_mon = month - 1;
  tm_.tm_mday = day;
  tm_.tm_hour = 15;
  tm_.tm_min = 0;
  tm_.tm_sec = 0;
  tm_.tm_isdst = 0;
  pDayLine->SetTime(mktime(&tm_));
  lDay = year * 10000 + month * 100 + day;
  pDayLine->SetDay(lDay);
  //TRACE("%d %d %d\n", year, month, day);

  if (*pCurrentPos != 0x27) return(false); // ���Ƕ��ţ����ݳ�����������
  pCurrentPos++;
  iCount++;

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  str = buffer2;
  pDayLine->SetStockCode(strStockCode);
  str = strStockCode.Left(2);
  if (str == _T("sh")) {
    pDayLine->SetMarket(__SHANGHAI_MARKET__);
  }
  else if (str == _T("sz")) {
    pDayLine->SetMarket(__SHENZHEN_MARKET__);
  }
  else {
    ASSERT(0); // ��Ʊ������ʽ����
    return false;
  }
  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  str = buffer2;
  pDayLine->SetStockName(str);

  if (!ReadOneValueExceptPeriod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetClose(buffer2);

  if (!ReadOneValueExceptPeriod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetHigh(buffer2);

  if (!ReadOneValueExceptPeriod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetLow(buffer2);

  if (!ReadOneValueExceptPeriod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetOpen(buffer2);

  if (!ReadOneValueExceptPeriod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetLastClose(buffer2);

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  if (pDayLine->GetOpen() == 0) {
    //ASSERT(strcmp(buffer2, "None") == 0);
    pDayLine->SetUpDown(0.0);
  }
  else pDayLine->SetUpDown(buffer2);

  if (pDayLine->GetLastClose() == 0) { // �����ǵ�����
    pDayLine->SetUpDownRate(0.0); // ����������̼�Ϊ�㣨û���ף������ǵ���Ҳ��Ϊ�㡣
  }
  else {
    // ��Ҫ�Ŵ�1000 * 100�������̼۱�ʵ��ֵ��1000������¼���ǰٷֱȣ�ҲҪ����100����
    pDayLine->SetUpDownRate(((double)(pDayLine->GetUpDown() * 100000.0)) / pDayLine->GetLastClose());
  }

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetChangeHandRate(buffer2);

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetVolume(buffer2); // ������ǹ���

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetAmount(buffer2);

  // ����ֵ��������������ʽ����Ҫ�����ж�
  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetTotalValue(buffer2); // ����ֵ�ĵ�λΪ��Ԫ

  // ��ͨ��ֵ�����ö��Ž������ʶ�����ʹ��ReadOneValue����
  // ��ͨ��ֵ��������ʽ�����֣��ʶ���Ҫ�����ж���
  i = 0;
  while (*pCurrentPos != 0x00d) {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) return false; // ���ݳ�����������
    buffer2[i++] = *pCurrentPos++;
    iCount++;
  }
  pCurrentPos++;
  iCount++;
  buffer2[i] = 0x000;
  pDayLine->SetCurrentValue(buffer2); // ��ͨ��ֵ�ĵ�λΪ��Ԫ��
  // \r���������Ӧ����\n
  if (*pCurrentPos++ != 0x0a) return false; // ���ݳ�����������
  iCount++;

  lLength = iCount;

  return true;
}

bool CMarket::ReadOneValue(char*& pCurrentPos, char* buffer, long& iReadNumber) {
  int i = 0;

  while (*pCurrentPos != 0x2c) { // ����һ������ǰ���ַ����뻺����. 0x2c���Ƕ��š�
    if ((*pCurrentPos == 0x0d) || (*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false; // ���ݳ�����������
    }
    buffer[i++] = *pCurrentPos++;
  }
  buffer[i] = 0x000;
  pCurrentPos++;
  i++;
  iReadNumber += i;
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���븡������С���������λ������ͼ���0.��������λ���������������ֵ�Ŵ�һǧ����
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ReadOneValueExceptPeriod(char*& pCurrentPos, char* buffer, long& lCounter) {
  int i = 0;
  bool fFoundPoint = false;
  int iCount = 0;
  while ((*pCurrentPos != ',') && (iCount < 3)) {
    if (fFoundPoint) iCount++;
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x00d) || (*pCurrentPos == 0x000)) return false;
    if (*pCurrentPos == '.') {
      fFoundPoint = true;
      pCurrentPos++;
    }
    else buffer[i++] = *pCurrentPos++;
  }

  if (fFoundPoint && (iCount < 3)) {
    int jCount = i;
    for (int j = iCount; j < 3; j++) {
      buffer[jCount++] = '0';
    }
    buffer[jCount] = 0x000;
  }
  else {
    buffer[i] = 0x000;
  }

  while (*pCurrentPos != ',') {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x00d) || (*pCurrentPos == 0x000)) return false;
    i++;
    pCurrentPos++;
  }
  pCurrentPos++;
  i++;
  if (fFoundPoint) i++;
  lCounter += i; // ���1������Ҫ��������Ķ���

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

  gl_systemTime.CalculateTime();      // ����ϵͳ����ʱ��

  // ץȡʵʱ����(���ˡ���Ѷ�����ף���ÿ400��������һ�Σ����ɱ�֤��3�����ڱ���һ��ȫ���Ծ��Ʊ��
  if (!gl_ExitingSystem.IsTrue() && m_fGetRTStockData && (m_iCountDownSlowReadingRTData <= 0)) {
    GetRTDataFromWeb();

    // ���Ҫ�����ٶ�ȡʵʱ���ݣ������ö�ȡ����Ϊÿ����һ��
    if (!m_fMarketOpened && SystemReady()) m_iCountDownSlowReadingRTData = 10; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
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
    LoadTodayTempDataSaved();
    GetNeteaseDayLineWebData();
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////
//
// �����ˡ����׻�����Ѷʵʱ�������ݷ�������ȡʵʱ���ݡ�ʹ������֮һ���ɡ�
//
/////////////////////////////////////////////////////////////////////////////////
bool CMarket::GetRTDataFromWeb(void) {
  static int siCountDownTengxunNumber = 3;
  static int siCountDownNeteaseNumber = 300;

  if (m_fUsingSinaRTDataReceiver) {
    gl_SinaRTWebData.GetWebData(); // ÿ400����(100X4)����һ��ʵʱ���ݡ����˵�ʵʱ�����������Ӧʱ�䲻����100���루30-70֮�䣩����û�г��ֹ����ݴ���
  }

  if (SystemReady()) {
    // ����ʵʱ�����д�����ȱʧ�ֶΣ���ǰ׺��׺Ҳ��ʱȱʧ����ʱֹͣʹ�á�
    // ����ʵʱ������ʱ������û��Ҫ����Ĺ�Ʊ����֪Ϊ�Ρ�
    ASSERT(m_fUsingNeteaseRTDataReceiver == false);
    if (m_fUsingNeteaseRTDataReceiver) {
      if (siCountDownNeteaseNumber <= 0) {
        // ��ȡ����ʵʱ�������ݡ���������ʵʱ���������˵�����Դ��ͬ���ʶ����߿ɻ�����ʹ����һ���ɡ�
        gl_NeteaseRTWebData.GetWebData(); // Ŀǰ��ʹ�ô˹��ܡ�
        siCountDownNeteaseNumber = 3;
      }
      else siCountDownNeteaseNumber--;
    }

    // ��ȡ��Ѷʵʱ�������ݡ� ������Ѷʵʱ����Ĺ�������Ϊ�֣�û�������Ϣ�������޷�������ʵʱ�������ݶԽӣ����˾���Ϊ�ɣ����ʶ���ʱ����
    if (m_fReadingTengxunRTData) {
      if (siCountDownTengxunNumber <= 0) {
        gl_TengxunRTWebData.GetWebData();// ֻ�е�ϵͳ׼����Ϻ󣬷���ִ�ж�ȡ��Ѷʵʱ�������ݵĹ�����Ŀǰ��ʹ�ô˹���
        siCountDownTengxunNumber = 3;
      }
      else siCountDownTengxunNumber--; // ����ʵʱ���ݶ�ȡ���Σ���Ѷ�Ŷ�ȡһ�Ρ���Ϊ��Ѷ�Ĺҵ��������õ���ÿ�ֱ�׼�����Ȳ���
    }
  }

  return true;
}

bool CMarket::LoadTodayTempDataSaved(void) {
  // װ��֮ǰ�洢��ϵͳ�������ݣ�����еĻ���
  if (!m_fTodayTempDataLoaded) { // �˹���������һ�Ρ�
    LoadTodayTempDB();
    m_fTodayTempDataLoaded = true;
    return true;
  }
  return false;
}

bool CMarket::GetNeteaseDayLineWebData(void) {
  // ץȡ��������.
  // ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
  if (!gl_ExitingSystem.IsTrue() && m_fGetDayLineData) {
    switch (gl_cMaxSavingOneDayLineThreads) {
    case 8: case 7: case 6:case 5: case 4:
    gl_NeteaseDayLineWebDataFourth.GetWebData();
    case 3:
    gl_NeteaseDayLineWebDataThird.GetWebData();
    case 2:
    gl_NeteaseDayLineWebDataSecond.GetWebData();
    case 1: case 0:
    gl_NeteaseDayLineWebData.GetWebData();
    break;
    default:
    gl_NeteaseDayLineWebData.GetWebData();
    TRACE(_T("Out of range in Get Newease DayLine Web Data\n"));
    break;
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
bool CMarket::SchedulingTaskPerSecond(long lSecondNumber) {
  static int s_iCountDownProcessRTWebData = 0;
  const long lCurrentTime = gl_systemTime.GetTime();

  // �����ȳ��򰴼��ʱ���С˳�����У����ʱ�䳤�ı���λ�ڼ��ʱ��̵�֮ǰ��
  SchedulingTaskPerHour(lSecondNumber, lCurrentTime);
  SchedulingTaskPer5Minutes(lSecondNumber, lCurrentTime);
  SchedulingTaskPer1Minute(lSecondNumber, lCurrentTime);
  SchedulingTaskPer10Seconds(lSecondNumber, lCurrentTime);

  if (s_iCountDownProcessRTWebData <= 0) {
    // �����յ���ʵʱ���ݷַ�������ع�Ʊ��ʵʱ���ݶ����С�
    // �����ж������Դ���ʶ���Ҫ�ȴ�������Դ��ִ��һ�κ󣬷����Էַ�����ع�Ʊ�����ʶ���Ҫÿ����ִ��һ�Σ��Ա�֤������Դ���ٶ����ṩһ�����ݡ�
    DistributeSinaRTDataToProperStock();
    s_iCountDownProcessRTWebData = 0;
  }
  else s_iCountDownProcessRTWebData--;

  // ����ʵʱ���ݣ�ÿ����һ�Ρ�Ŀǰ����ʵʱ����Ϊÿ3����һ�θ��£��ʶ������ٿ��ˡ�
  // �˼�������Ҫ��DistributeRTDataReceivedFromWebToProperStock֮��ִ�У��Է�ֹ����ͬ�����⡣
  if (SystemReady() && !gl_ThreadStatus.IsSavingTempData() && IsTodayTempRTDataLoaded()) { // ��ϵͳ�洢��ʱ����ʱ����ͬʱ����ʵʱ���ݣ��������׳���ͬ�����⡣
    if (gl_ThreadStatus.IsRTDataNeedCalculate()) {
      gl_ThreadStatus.SetCalculatingRTData(true);
      AfxBeginThread(ThreadCalculateRTData, nullptr);
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

    // �ŵ�ʮ��������ϵͳ
    // �����ڴ�ʱ������������������Ļ����׳������ݲ�ȫ�����⡣
    if (m_fPermitResetSystem) { // �����������ϵͳ
      if ((lCurrentTime >= 91300) && (lCurrentTime <= 91400) && ((gl_systemTime.GetDayOfWeek() > 0) && (gl_systemTime.GetDayOfWeek() < 6))) { // �����վŵ�ʮ�������ϵͳ
        gl_fResetSystem = true;     // ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
        m_fSystemReady = false;
      }
    }

    // �ŵ��ʮ����ٴ�����ϵͳ
    if (m_fPermitResetSystem) { // �����������ϵͳ
      if ((lCurrentTime >= 92500) && (lCurrentTime <= 93000) && ((gl_systemTime.GetDayOfWeek() > 0) && (gl_systemTime.GetDayOfWeek() < 6))) { // �����վŵ�ʮ�������ϵͳ
        gl_fResetSystem = true;     // ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
        m_fSystemReady = false;
        m_fPermitResetSystem = false; // ���첻����������ϵͳ��
      }
    }

    // �ж��й���Ʊ�г�����״̬
    if ((lCurrentTime < 91500) || (lCurrentTime > 150630) || ((lCurrentTime > 113500) && (lCurrentTime < 125500))) { //��������������ʮ���г����׽�����Ϊ�˱�֤���һ����ʱ���ݵĴ洢��
      m_fMarketOpened = false;
    }
    else if ((gl_systemTime.GetDayOfWeek() == 0) || (gl_systemTime.GetDayOfWeek() == 6)) { //�����������ձ��С��ṹtm��0--6��ʾ��������������
      m_fMarketOpened = false;
    }
    else m_fMarketOpened = true;

    // �ڿ���ǰ��������ͣʱ��ѯ���й�Ʊ�أ��ҵ������Ծ��Ʊ��
    if (((lCurrentTime >= 91500) && (lCurrentTime < 92900)) || ((lCurrentTime >= 113100) && (lCurrentTime < 125900))) {
      m_fCheckTodayActiveStock = true;
    }
    else m_fCheckTodayActiveStock = false;

    // ��������һ�ֿ�ʼ������ʵʱ���ݡ�
    if ((lCurrentTime >= 150100) && !IsTodayStockCompiled()) {
      if (SystemReady()) {
        AfxBeginThread(ThreadCompileCurrentTradeDayStock, nullptr);
        SetTodayStockCompiledFlag(true);
      }
    }

    // �����ȡ������ʷ���ݵ������Ƿ�����ˡ�
    if (m_fGetDayLineData) {
      m_fSaveDayLine = true;
      if (IsDayLineDataInquiringOnce()) {
        m_fGetDayLineData = false;
      }
    }

    // �ж��Ƿ�洢���߿�͹�Ʊ�����
    if (m_fSaveDayLine) {
      gl_ChinaStockMarket.SaveDayLineData();
      if (!m_fGetDayLineData && (!IsDayLineNeedSaving() && !gl_ThreadStatus.IsSavingDayLine())) {
        m_fSaveDayLine = false;
        m_fUpdatedStockCodeDataBase = true;
        TRACE("������ʷ���ݸ������\n");
        CString str;
        str = _T("������ʷ���ݸ������");
        gl_systemMessage.PushInformationMessage(str);
        UpdateStockCodeDB();  // ���¹�Ʊ�����ݿ�
      }
    }
  } // ÿһ����һ�ε�����
  else i1MinuteCounter -= lSecondNumber;

  return true;
}

bool CMarket::SchedulingTaskPer10Seconds(long lSecondNumber, long lCurrentTime) {
  static int i10SecondsCounter = 9; // ʮ��һ�εļ�����

  // ����ÿʮ����һ�ε�����
  if (i10SecondsCounter <= 0) {
    i10SecondsCounter = 9;
    // do something
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
  catch (exception & e) {
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
  catch (exception & e) {
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
  catch (exception & e) {
    TRACE("GetStockPtr�����쳣\n");
    return nullptr;
  }
}

CStockPtr CMarket::GetStockPtr(long lIndex) {
  try {
    return m_vChinaMarketAStock.at(lIndex);
  }
  catch (exception & e) {
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
//	�����ߴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//  �˺����������̵߳��ã���Ҫע������ͬ�����⡣
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveDayLine(CStockPtr pStock) {
  CSetStockCode setStockCode;
  CSetDayLine setDayLine;

  long lIndex = 0;
  long lSize = 0;
  CStockPtr pStockID;
  CDayLinePtr pDayLine;

  CCriticalSection cs;
  CSingleLock s(&cs);
  s.Lock();
  if (s.IsLocked()) {
    lIndex = m_mapChinaMarketAStock.at(pStock->GetStockCode());
    pStockID = m_vChinaMarketAStock.at(lIndex);
    lSize = pStock->m_vDayLine.size();
    setDayLine.m_strFilter = _T("[ID] = 1"); // ����������Ϊ����Index.�������ã����������е����ݶ��룬�˷�ʱ��
    s.Unlock();
  }

  setDayLine.Open();
  setDayLine.m_pDatabase->BeginTrans();
  for (int i = 0; i < lSize; i++) { // ����������洢�ģ���Ҫ��ͷ����ʼ�洢
    pDayLine = pStock->m_vDayLine.at(i);
    if (pStockID->GetDayLineEndDay() >= pDayLine->GetDay()) continue; // �洢�����������ݲ��ô洢
    setDayLine.AddNew();
    setDayLine.m_Time = pDayLine->GetDay();
    setDayLine.m_Market = pDayLine->GetMarket();
    setDayLine.m_StockCode = pDayLine->GetStockCode();
    setDayLine.m_StockName = pDayLine->GetStockName();
    setDayLine.m_LastClose = ConvertValueToString(pDayLine->GetLastClose(), 1000);
    setDayLine.m_High = ConvertValueToString(pDayLine->GetHigh(), 1000);
    setDayLine.m_Low = ConvertValueToString(pDayLine->GetLow(), 1000);
    setDayLine.m_Open = ConvertValueToString(pDayLine->GetOpen(), 1000);
    setDayLine.m_Close = ConvertValueToString(pDayLine->GetClose(), 1000);
    setDayLine.m_Volume = ConvertValueToString(pDayLine->GetVolume());
    setDayLine.m_Amount = ConvertValueToString(pDayLine->GetAmount());
    setDayLine.m_UpAndDown = ConvertValueToString(pDayLine->GetUpDown());
    setDayLine.m_UpDownRate = ConvertValueToString(pDayLine->GetUpDownRate());
    setDayLine.m_ChangeHandRate = ConvertValueToString(pDayLine->GetChangeHandRate());
    setDayLine.m_TotalValue = ConvertValueToString(pDayLine->GetTotalValue());
    setDayLine.m_CurrentValue = ConvertValueToString(pDayLine->GetCurrentValue());

    setDayLine.m_RelativeStrong = ConvertValueToString(pDayLine->GetRelativeStrong());

    setDayLine.Update();
  }
  setDayLine.m_pDatabase->CommitTrans();
  setDayLine.Close();

  // ���������������ں���ʼ��������
  s.Lock();
  if (s.IsLocked()) {
    if (pStockID->GetDayLineEndDay() < pStock->m_vDayLine.at(pStock->m_vDayLine.size() - 1)->GetDay()) {
      pStockID->SetDayLineStartDay(pStock->m_vDayLine.at(0)->GetDay());
      pStockID->SetDayLineEndDay(pStock->m_vDayLine.at(pStock->m_vDayLine.size() - 1)->GetDay());
    }
    s.Unlock();
  }

  return true;
}

bool CMarket::SaveOneRecord(CSetDayLine* psetDayLine, CDayLinePtr pDayLine) {
  psetDayLine->AddNew();
  psetDayLine->m_Time = pDayLine->GetDay();
  psetDayLine->m_Market = pDayLine->GetMarket();
  psetDayLine->m_StockCode = pDayLine->GetStockCode();
  psetDayLine->m_StockName = pDayLine->GetStockName();
  psetDayLine->m_LastClose = ConvertValueToString(pDayLine->GetLastClose(), 1000);
  psetDayLine->m_High = ConvertValueToString(pDayLine->GetHigh(), 1000);
  psetDayLine->m_Low = ConvertValueToString(pDayLine->GetLow(), 1000);
  psetDayLine->m_Open = ConvertValueToString(pDayLine->GetOpen(), 1000);
  psetDayLine->m_Close = ConvertValueToString(pDayLine->GetClose(), 1000);
  psetDayLine->m_Volume = ConvertValueToString(pDayLine->GetVolume());
  psetDayLine->m_Amount = ConvertValueToString(pDayLine->GetAmount());
  psetDayLine->m_UpAndDown = ConvertValueToString(pDayLine->GetUpDown());
  psetDayLine->m_UpDownRate = ConvertValueToString(pDayLine->GetUpDownRate());
  psetDayLine->m_ChangeHandRate = ConvertValueToString(pDayLine->GetChangeHandRate());
  psetDayLine->m_TotalValue = ConvertValueToString(pDayLine->GetTotalValue());
  psetDayLine->m_CurrentValue = ConvertValueToString(pDayLine->GetCurrentValue());

  psetDayLine->m_RelativeStrong = ConvertValueToString(pDayLine->GetRelativeStrong());

  psetDayLine->Update();

  return true;
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
      if (pStock->m_vDayLine.size() > 0) { // �¹ɵ�һ������ʱ������ֻ�洢���ڽ�����������ݣ������������ǿյģ��ʶ���Ҫ�ж�һ��
        pTransfer = new strTransferSharedPtr; // �˴����ɣ����̸߳���delete
        pTransfer->m_pStock = pStock;
        AfxBeginThread(ThreadSaveDayLineOfOneStock, (LPVOID)pTransfer, THREAD_PRIORITY_LOWEST);
      }
      else {
        CString str1 = pStock->GetStockCode();
        str1 += _T(" �¹�����,û����������");
        gl_systemMessage.PushDayLineInfoMessage(str1);
      }
      str = pStock->GetStockCode();
      str += _T("�������ϴ洢���");
      gl_systemMessage.PushDayLineInfoMessage(str);
    }
    if (gl_ExitingSystem.IsTrue()) {
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
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsActive()) {
      pStock->SaveRealTimeData(&setRTData);
    }
  }
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
  setIndex.AddNew();
  gl_CrweberIndex.SaveData(&setIndex);
  setIndex.Update();
  setIndex.m_pDatabase->CommitTrans();
  setIndex.Close();
  return(true);
}

bool CMarket::IsDayLineNeedUpdate(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    if (pStock->IsDayLineNeedUpdate()) return true;
  }
  return false;
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

bool CMarket::IsDayLineDataInquiringOnce(void) {
  for (auto pStock : m_vChinaMarketAStock) {
    if (!pStock->IsInquiringOnce()) return false;
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
  long lClose = 0, lLastClose = 0;
  char buffer[20];
  CString strDay;
  CSetDayLine setDayKLine;
  CSetDayLineInfo setDayLineInfo;
  long lIndex = 0;
  long iCount = 0;

  CString str;
  str = _T("��ʼ�������½����յ�ʵʱ����");
  gl_systemMessage.PushInformationMessage(str);

  // �洢��ǰ�����յ�����
  _ltoa_s(lCurrentTradeDay, buffer, 10);
  strDay = buffer;
  setDayKLine.m_strFilter = _T("[Time] =");
  setDayKLine.m_strFilter += strDay;
  setDayKLine.Open();
  setDayKLine.m_pDatabase->BeginTrans();
  while (!setDayKLine.IsEOF()) {
    setDayKLine.Delete();
    setDayKLine.MoveNext();
  }
  setDayKLine.m_pDatabase->CommitTrans();

  setDayKLine.m_pDatabase->BeginTrans();
  for (auto pStock : m_vChinaMarketAStock) {
    if (!pStock->TodayDataIsActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    iCount++;
    lIndex = m_mapChinaMarketAStock.at(pStock->GetStockCode());
    m_vChinaMarketAStock.at(lIndex)->SetDayLineEndDay(lCurrentTradeDay);
    m_vChinaMarketAStock.at(lIndex)->SetIPOStatus(__STOCK_IPOED__); // ������һ�Ρ���ֹ�¹ɹ�Ʊ��������û����ʷ���ݶ�������Ϊ�����ڡ�
    setDayKLine.AddNew();
    setDayKLine.m_Time = lCurrentTradeDay;
    setDayKLine.m_Market = pStock->GetMarket();
    setDayKLine.m_StockName = pStock->GetStockName();
    setDayKLine.m_StockCode = pStock->GetStockCode();
    lLastClose = pStock->GetLastClose();
    setDayKLine.m_LastClose = ConvertValueToString(pStock->GetLastClose(), 1000);
    setDayKLine.m_Open = ConvertValueToString(pStock->GetOpen(), 1000);
    setDayKLine.m_High = ConvertValueToString(pStock->GetHigh(), 1000);
    setDayKLine.m_Low = ConvertValueToString(pStock->GetLow(), 1000);
    lClose = pStock->GetNew();
    setDayKLine.m_Close = ConvertValueToString(pStock->GetNew(), 1000);
    setDayKLine.m_UpAndDown = ConvertValueToString((lClose - lLastClose), 1000);
    if (lLastClose == 0) { // �����е�һ��Ĺ�Ʊ
      setDayKLine.m_UpDownRate = ConvertValueToString(0);
    }
    else {
      setDayKLine.m_UpDownRate = ConvertValueToString(((double)(lClose - lLastClose)) * 100.0 / lLastClose);
    }

    setDayKLine.m_Volume = ConvertValueToString(pStock->GetVolume());
    setDayKLine.m_Amount = ConvertValueToString(pStock->GetAmount());
    setDayKLine.m_TotalValue = ConvertValueToString(pStock->GetTotalValue());
    setDayKLine.m_CurrentValue = ConvertValueToString(pStock->GetCurrentValue());
    setDayKLine.m_RelativeStrong = ConvertValueToString(pStock->GetRelativeStrong());

    setDayKLine.Update();
  }
  setDayKLine.m_pDatabase->CommitTrans();
  setDayKLine.Close();

  // �洢�������ɵ�������DayLineInfo���С�
  setDayLineInfo.m_strFilter = _T("[Time] =");
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
    if (!pStock->TodayDataIsActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    setDayLineInfo.AddNew();
    setDayLineInfo.m_Day = lCurrentTradeDay;
    setDayLineInfo.m_StockCode = pStock->GetStockCode();

    pStock->StoreCalculatedInfo(setDayLineInfo);
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
    if (!pStock->TodayDataIsActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    ASSERT(pStock->GetVolume() == pStock->GetOrdinaryBuyVolume() + pStock->GetOrdinarySellVolume() + pStock->GetAttackBuyVolume()
           + pStock->GetAttackSellVolume() + pStock->GetStrongBuyVolume() + pStock->GetStrongSellVolume() + pStock->GetUnknownVolume());
    setDayLineToday.AddNew();
    setDayLineToday.m_Time = gl_systemTime.GetDay();
    setDayLineToday.m_Market = pStock->GetMarket();
    setDayLineToday.m_StockName = pStock->GetStockName();
    setDayLineToday.m_StockCode = pStock->GetStockCode();

    setDayLineToday.m_Open = ConvertValueToString(pStock->GetOpen(), 1000);
    setDayLineToday.m_Volume = ConvertValueToString(pStock->GetVolume());
    pStock->StoreTempInfo(setDayLineToday);
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
    if (setDayLineToday.m_Time == gl_systemTime.GetDay()) { // ����ǵ�������飬�����룬�������
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
  CSetDayLine setDayKLine;
  const long lYear = lDay / 10000;
  const long lMonth = lDay / 100 - lYear * 100;
  const long lDayOfMonth = lDay - lYear * 10000 - lMonth * 100;
  char buffer[100];

  for (j = 0; j < 30; j++) pch[j] = 0x000;

  _ltoa_s(lDay, pch, 10);
  strDay = pch;
  setDayKLine.m_strSort = _T("[UpDownRate]");
  setDayKLine.m_strFilter = _T("[Time] =");
  setDayKLine.m_strFilter += strDay;
  setDayKLine.Open();
  if (setDayKLine.IsEOF()) { // ���ݼ�Ϊ�գ���������û�н���
    setDayKLine.Close();
    return false;
  }
  setDayKLine.m_pDatabase->BeginTrans();
  iStockNumber = 0;
  while (!setDayKLine.IsEOF()) {
    if (gl_ChinaStockMarket.IsAStock(setDayKLine.m_StockCode)) {
      long lIndex = m_mapChinaMarketAStock.at(setDayKLine.m_StockCode);
      vStock.push_back(m_vChinaMarketAStock.at(lIndex));
      vIndex.push_back(iStockNumber); // ��A�ɵ�������¼�������С�
      iTotalAShare++;
    }
    iStockNumber++;
    setDayKLine.MoveNext();
  }

  setDayKLine.MoveFirst();
  int iCount = 0;
  int iBefore = 0;
  while (iCount < vIndex.size()) { // ֻ�����Ծ��Ʊ�����ǿ��
    for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) { // ��������ȥ�������ݿ�,�������A�ɵĹ�Ʊ
      setDayKLine.MoveNext();
    }
    setDayKLine.Edit();
    double dLastClose = atof(setDayKLine.m_LastClose);
    double dLow = atof(setDayKLine.m_Low);
    double dHigh = atof(setDayKLine.m_High);
    double dClose = atof(setDayKLine.m_Close);
    if (((dLow / dLastClose) < 0.88)
        || ((dHigh / dLastClose) > 1.12)) { // ��Ȩ���¹����е�
      setDayKLine.m_RelativeStrong = ConvertValueToString(50); // �¹����л��߳�Ȩ��Ϣ��������˹�
    }
    else if ((fabs(dHigh - dClose) < 0.0001)
             && (((dClose / dLastClose)) > 1.095)) { // ��ͣ��
      setDayKLine.m_RelativeStrong = ConvertValueToString(100);
    }
    else if ((fabs(dClose - dLow) < 0.0001)
             && ((dClose / dLastClose) < 0.905)) { // ��ͣ��
      setDayKLine.m_RelativeStrong = ConvertValueToString(0);
    }
    else {
      setDayKLine.m_RelativeStrong = ConvertValueToString((static_cast<double>(iCount) * 100) / iTotalAShare);
    }
    setDayKLine.Update();
    iBefore = vIndex.at(iCount++);
    setDayKLine.MoveNext(); // �Ƶ���һ�����ݡ�
    iBefore++; // ������Ҳͬʱ��һ��
  }
  setDayKLine.m_pDatabase->CommitTrans();
  setDayKLine.Close();

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
    setStockCode.AddNew();
    CString str;
    setStockCode.m_Counter = pStock->GetOffset();
    setStockCode.m_StockType = pStock->GetMarket();
    setStockCode.m_StockCode = pStock->GetStockCode();
    if (pStock->GetStockName() != _T("")) {   // ����˹�ƱID�����µ����֣�
      setStockCode.m_StockName = pStock->GetStockName(); // ��洢�µ�����
    }
    if (pStock->GetIPOStatus() == __STOCK_IPOED__) { // ����˹�Ʊ�ǻ�Ծ��Ʊ
      if (pStock->GetDayLineEndDay() < (gl_systemTime.GetDay() - 100)) { // ����˹�Ʊ��������ʷ�����Ѿ�����һ�����ˣ������ô˹�Ʊ״̬Ϊ������
        setStockCode.m_IPOed = __STOCK_DELISTED__;
      }
      else {
        setStockCode.m_IPOed = pStock->GetIPOStatus();
      }
    }
    else {
      setStockCode.m_IPOed = pStock->GetIPOStatus();
    }
    setStockCode.m_DayLineStartDay = pStock->GetDayLineStartDay();
    setStockCode.m_DayLineEndDay = pStock->GetDayLineEndDay();
    setStockCode.Update();
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
    long lIndex = 1;
    lIndex = m_mapChinaMarketAStock.at(setStockCode.m_StockCode);
    CStockPtr pStock = m_vChinaMarketAStock.at(lIndex);
    if (setStockCode.m_StockCode != _T("")) {
      pStock->SetStockCode(setStockCode.m_StockCode);
    }
    if (setStockCode.m_StockName != _T("")) {
      CString str = setStockCode.m_StockName; // ��str�м����һ�£��Ϳ��Զ�ȡUniCode��ʽ��m_StockName�ˡ�
      pStock->SetStockName(str);
    }
    if (setStockCode.m_IPOed != __STOCK_NOT_CHECKED__) { // ����˹�Ʊ�����Ѿ��������������ù�ƱĿǰ״̬���������á�
      pStock->SetIPOStatus(setStockCode.m_IPOed);
    }
    pStock->SetDayLineStartDay(setStockCode.m_DayLineStartDay);
    if (pStock->GetDayLineEndDay() < setStockCode.m_DayLineEndDay) { // ��ʱһ����Ʊ���ж����¼������������Ϊ׼��
      pStock->SetDayLineEndDay(setStockCode.m_DayLineEndDay);
    }
    // ���ٸ����������ݱ��ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
    if (gl_systemTime.GetLastTradeDay() <= pStock->GetDayLineEndDay()) { // ������������Ϊ���ջ�����һ�������յ����ݡ�
      pStock->SetDayLineNeedUpdate(false); // �������ݲ���Ҫ����
    }
    if (setStockCode.m_IPOed == __STOCK_NULL__) { // ��Ч���벻�������������
      pStock->SetDayLineNeedUpdate(false);
    }
    if (setStockCode.m_IPOed == __STOCK_DELISTED__) { // ���й�Ʊ���������������
      pStock->SetDayLineNeedUpdate(false);
    }
    setStockCode.MoveNext();
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

  // �ж��Ƿ���Ҫ��ȡ������ʷ����
  if (m_lLastLoginDay >= gl_systemTime.GetLastTradeDay()) {
    m_fGetDayLineData = false;
    m_fSaveDayLine = false;
  }
  else {
    m_fGetDayLineData = true;
    m_fSaveDayLine = true;
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