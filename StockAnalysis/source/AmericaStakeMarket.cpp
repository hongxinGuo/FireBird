#include "AmericaStakeMarket.h"
#include"thread.h"

#include"WebInquirer.h"
#include"ProcessAmericaStake.h"

#include"SetAmericaStake.h"

CAmericaStakeMarket::CAmericaStakeMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CAmericaStakeMarket�г�����ֻ�������һ��ʵ��\n");
  }

  m_strMarketId = _T("Finnhub������Ϣ");
  m_lMarketTimeZone = 4 * 3600; // ��������ʹ��������׼ʱ�䡣
  CalculateTime();

  m_vFinnhubInquiringStr.resize(1000);

  // Finnhubǰ׺�ַ����ڴ�Ԥ��֮
  m_vFinnhubInquiringStr.at(__COMPANY_PROFILE__) = _T("https://finnhub.io/api/v1/stock/profile?symbol="); // ��˾��顣
  m_vFinnhubInquiringStr.at(__COMPANY_PROFILE2__) = _T("https://finnhub.io/api/v1/stock/profile2?symbol="); // ��˾��飨��棩
  m_vFinnhubInquiringStr.at(__COMPANY_SYMBOLS__) = _T("https://finnhub.io/api/v1/stock/symbol?exchange=US"); // ���ô��뼯
  m_vFinnhubInquiringStr.at(__MARKET_NEWS__) = _T("https://finnhub.io/api/v1/news?category=general");
  m_vFinnhubInquiringStr.at(__COMPANY_NEWS__) = _T("https://finnhub.io/api/v1/company-news?symbol=");
  m_vFinnhubInquiringStr.at(__NEWS_SENTIMENT__) = _T("https://finnhub.io/api/v1/news-sentiment?symbol=");
  m_vFinnhubInquiringStr.at(__PEERS__) = _T("https://finnhub.io/api/v1/stock/peers?symbol=");
  m_vFinnhubInquiringStr.at(__BASIC_FINANCIALS__) = _T("https://finnhub.io/api/v1/stock/metric?symbol=");
  m_vFinnhubInquiringStr.at(__SEC_FILINGS__) = _T("https://finnhub.io/api/v1/stock/filings?symbol=");

  m_vFinnhubInquiringStr.at(__STAKE_QUOTE__) = _T("https://finnhub.io/api/v1/quote?symbol="); // ĳ������Ľ���
  m_vFinnhubInquiringStr.at(__STAKE_CANDLES__) = _T("https://finnhub.io/api/v1/stock/candle?symbol="); // ��ʷ����ͼ

  m_vFinnhubInquiringStr.at(__FOREX_EXCHANGE__) = _T("https://finnhub.io/api/v1/forex/exchange?");
  m_vFinnhubInquiringStr.at(__FOREX_SYMBOLS__) = _T("https://finnhub.io/api/v1/forex/symbol?exchange=oanda");
  m_vFinnhubInquiringStr.at(__FOREX_CANDLES__) = _T("https://finnhub.io/api/v1/forex/candle?symbol=OANDA:EUR_USD&resolution=D");
  m_vFinnhubInquiringStr.at(__FOREX_ALL_RATES__) = _T("https://finnhub.io/api/v1/forex/rates?base=USD");

  m_lPrefixIndex = -1; //
  Reset();
}

CAmericaStakeMarket::~CAmericaStakeMarket() {
}

void CAmericaStakeMarket::Reset(void) {
  m_lTotalAmericaStake = 0;
  m_lLastTotalAmericaStake = 0;
  m_lCurrentProfilePos = 0;
  m_lCurrentUpdateDayLinePos = 0;
  m_vAmericaStake.resize(0);
  m_mapAmericaStake.clear();
  m_fSymbolUpdated = false; // ÿ����Ҫ���´���
  m_fSymbolProceeded = false;
  m_fAmericaStakeUpdated = false;
  m_fStakeDayLineUpdated = false;

  m_fInquiringStakeProfileData = false;
  m_fInquiringStakeCandle = false;
}

void CAmericaStakeMarket::ResetMarket(void) {
  Reset();

  LoadAmericaStake();

  CString str = _T("����America Stake Market��������׼ʱ�䣺");
  str += GetStringOfMarketTime();
  gl_systemMessage.PushInformationMessage(str);
}

bool CAmericaStakeMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  static int s_iCountFinnhub = 4;
  const long lCurrentTime = GetFormatedMarketTime();

  // Finnhub.io��ȡ������
  if (s_iCountFinnhub-- < 0) { // ÿ0.5�����һ��
    s_iCountFinnhub = 4; //Ŀǰ����1.5�����ҽ���һ�����ݡ���첻�ܴﵽÿ��1�룬��s_iCountFinnhub��������Ϊ4.�����ܹ���֤��һ�ν���ʱ
  }
  else s_iCountFinnhub--;

  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (GetMarketTime() > s_timeLast) {
    SchedulingTaskPerSecond(GetMarketTime() - s_timeLast, lCurrentTime);
    s_timeLast = GetMarketTime();
  }

  return true;
}

/// <summary>
/// //////////////////////////////////////////////////////////////////////////////////////////
/// </summary>
/// finnhub��ȡ������������ͽ����ֻ�ִ�з�ʽ���ʶ����ٵ������β����һ���ֻء�
/// <param name=""></param>
///
/////////////////////////////////////////////////////////////////////////////////////////////
void CAmericaStakeMarket::GetFinnhubDataFromWeb(void) {
  static bool s_fWaitingData = false;
  CWebDataPtr pWebData = nullptr;
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  char buffer[50];

  if (s_fWaitingData) { // �Ѿ��������������룿
    if (!IsWaitingFinHubData()) { //Finnhub�����Ѿ����յ��ˣ�
      s_fWaitingData = false;
      if (gl_WebInquirer.GetFinnhubDataSize() > 0) { // ����������ݽ��յ���
        // ����ǰ��������
        pWebData = gl_WebInquirer.PopFinnhubData();
        switch (m_lPrefixIndex) {
        case __COMPANY_PROFILE__: // Ŀǰ����˻��޷�ʹ�ô˹��ܡ�
        ProcessAmericaStakeProfile(pWebData);
        m_fInquiringStakeProfileData = false;
        break;
        case __COMPANY_PROFILE2__:
        ProcessAmericaStakeProfile2(pWebData);
        m_fInquiringStakeProfileData = false;
        break;
        case  __COMPANY_SYMBOLS__:
        ProcessAmericaStakeSymbol(pWebData);
        m_fSymbolProceeded = true;
        break;
        case  __MARKET_NEWS__:
        break;
        case __COMPANY_NEWS__:
        break;
        case __NEWS_SENTIMENT__:
        break;
        case __PEERS__:
        break;
        case __BASIC_FINANCIALS__:
        break;
        case __STAKE_QUOTE__:
        break;
        case __STAKE_CANDLES__:
        ProcessAmericaStakeCandle(pWebData, m_vAmericaStake.at(m_lCurrentUpdateDayLinePos));
        m_fInquiringStakeCandle = false;
        TRACE("����%s��������\n", m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->m_strSymbol.GetBuffer());
        break;
        case __FOREX_EXCHANGE__:
        break;
        case __FOREX_SYMBOLS__:
        break;
        case __FOREX_CANDLES__:
        break;
        case __FOREX_ALL_RATES__:
        break;
        default:
        break;
        }
      }
    }
  }
  else { // û����������������Ļ�
    if (m_qWebInquiry.size() > 0) { // ������ȴ���
      m_lPrefixIndex = GetFinnInquiry();
      gl_pFinnhubWebInquiry->SetInquiryingStrPrefix(m_vFinnhubInquiringStr.at(m_lPrefixIndex)); // ����ǰ׺
      switch (m_lPrefixIndex) { // ���ݲ�ͬ��Ҫ��������׺�ַ���
      case __COMPANY_PROFILE__: // ����˻��޷���ȡ����Ϣ��sandboxģʽ�ܶ�ȡ�����Ǵ���ģ�ֻ�����ڲ��ԡ�
      while (!m_vAmericaStake.at(m_lCurrentProfilePos)->m_fInquiryAmericaStake) m_lCurrentProfilePos++;
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_lCurrentProfilePos)->m_strSymbol);
      m_vAmericaStake.at(m_lCurrentProfilePos)->m_fInquiryAmericaStake = false;
      break;
      case __COMPANY_PROFILE2__:
      while (!m_vAmericaStake.at(m_lCurrentProfilePos)->m_fInquiryAmericaStake) m_lCurrentProfilePos++;
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_lCurrentProfilePos)->m_strSymbol);
      m_vAmericaStake.at(m_lCurrentProfilePos)->m_fInquiryAmericaStake = false;
      break;
      case  __COMPANY_SYMBOLS__:
      // do nothing
      break;
      case  __MARKET_NEWS__:
      break;
      case __COMPANY_NEWS__:
      break;
      case __NEWS_SENTIMENT__:
      break;
      case __PEERS__:
      break;
      case __BASIC_FINANCIALS__:
      break;
      case __STAKE_QUOTE__:
      break;
      case __STAKE_CANDLES__:
      strMiddle += m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->m_strSymbol;
      strMiddle += _T("&resolution=D");
      strMiddle += _T("&from=");
      sprintf_s(buffer, _T("%I64i"), (INT64)(GetMarketTime() - (time_t)(365) * 24 * 3600)); // ����˻�ֻ�ܶ�ȡһ�����ڵ��������ݡ�
      strTemp = buffer;
      strMiddle += strTemp;
      strMiddle += _T("&to=");
      sprintf_s(buffer, _T("%I64i"), GetMarketTime());
      strTemp = buffer;
      strMiddle += strTemp;
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->m_fDayLineNeedUpdate = false;
      break;
      case __FOREX_EXCHANGE__:
      break;
      case __FOREX_SYMBOLS__:
      break;
      case __FOREX_CANDLES__:
      break;
      case __FOREX_ALL_RATES__:
      break;
      default:
      break;
      }
      gl_pFinnhubWebInquiry->GetWebData();
      s_fWaitingData = true;
      SetWaitingFinnhubData(true);
    }
  }
}

bool CAmericaStakeMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
  SchedulingTaskPer1Hour(lSecond, lCurrentTime);
  SchedulingTaskPer1Minute(lSecond, lCurrentTime);
  SchedulingTaskPer10Seconds(lSecond, lCurrentTime);

  TaskInquiryTodaySymbol();
  TaskSaveStakeSymbolDB();

  GetFinnhubDataFromWeb();

  if (m_fSymbolProceeded) {
    TaskInquiryAmericaStake();
    TaskInquiryDayLine();
  }

  static int s_iCount = 1;
  if (m_fSymbolProceeded) {
    if (s_iCount-- < 0) {
      s_iCount = 1;
    }
  }

  return true;
}

bool CAmericaStakeMarket::SchedulingTaskPer10Seconds(long lSecond, long lCurrentTime) {
  static int i10SecondsCounter = 9;  // ʮ����һ�εļ�����

  i10SecondsCounter -= lSecond;
  if (i10SecondsCounter < 0) {
    i10SecondsCounter = 9;

    return true;
  }
  else return false;
}

bool CAmericaStakeMarket::SchedulingTaskPer1Hour(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 3599;  // һСʱһ�εļ�����

  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 3599;
    TaskResetMarket(lCurrentTime);

    return true;
  }
  else return false;
}

bool CAmericaStakeMarket::SchedulingTaskPer1Minute(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 59;  // һ����һ�εļ�����

  TaskResetMarket(lCurrentTime);

  // �Զ���ѯcrweber.com
  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 59;

    TaskUpdateDayLineDB();
    if (IsAmericaStakeUpdated()) {
      TaskUpdateStakeDB();
    }

    return true;
  }
  else {
    return false;
  }
}

bool CAmericaStakeMarket::TaskResetMarket(long lCurrentTime) {
  // �г�ʱ���ĵ�����ϵͳ
  if (IsPermitResetMarket()) { // �����������ϵͳ
    if ((lCurrentTime >= 29000) && (lCurrentTime < 30000)) { // ���г�ʱ���һ���������г� // ����������ʱ��Ϊ������ʱ��������ʱ����ʱ����
      SetResetMarket(true);// ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
      SetPermitResetMarket(false); // ���첻����������ϵͳ��
    }
  }
  return true;
}

bool CAmericaStakeMarket::TaskInquiryTodaySymbol(void) {
  if (!m_fSymbolUpdated) {
    SetFinnInquiry(__COMPANY_SYMBOLS__);
    //SetFinnInquiry(__MARKET_NEWS__);
    //SetFinnInquiry(__FOREX_EXCHANGE__);
    //SetFinnInquiry(__FOREX_SYMBOLS__);
    //SetFinnInquiry(__FOREX_CANDLES__);
    //SetFinnInquiry(__FOREX_ALL_RATES__);
    //SetFinnInquiry(__MARKET_NEWS__);
    m_fSymbolUpdated = true;
    return true;
  }
  return false;
}

bool CAmericaStakeMarket::TaskSaveStakeSymbolDB(void) {
  CSetAmericaStake setAmericaStake;
  CAmericaStakePtr pStake = nullptr;

  if (m_lLastTotalAmericaStake < m_lTotalAmericaStake) {
    setAmericaStake.Open();
    setAmericaStake.m_pDatabase->BeginTrans();
    for (long l = m_lLastTotalAmericaStake; l < m_lTotalAmericaStake; l++) {
      pStake = m_vAmericaStake.at(l);
      pStake->Save(setAmericaStake);
    }
    setAmericaStake.m_pDatabase->CommitTrans();
    setAmericaStake.Close();
    m_lLastTotalAmericaStake = m_lTotalAmericaStake;
  }
  return true;
}

bool CAmericaStakeMarket::TaskUpdateStakeDB(void) {
  RunningTaskThreadUpdateStakeDB();
  return true;
}

bool CAmericaStakeMarket::TaskInquiryAmericaStake(void) {
  bool fFound = false;
  FinnhubInquiry inquiry;

  ASSERT(m_fSymbolProceeded);
  if (!m_fAmericaStakeUpdated && !m_fInquiringStakeProfileData) {
    for (m_lCurrentProfilePos = 0; m_lCurrentProfilePos < m_vAmericaStake.size(); m_lCurrentProfilePos++) {
      if (IsEarlyThen(m_vAmericaStake.at(m_lCurrentProfilePos)->m_lProfileUpdateDate, GetFormatedMarketDate(), 365)) {
        fFound = true;
        break;
      }
    }
    if (fFound) {
      inquiry.m_iIndex = __COMPANY_PROFILE2__;
      inquiry.m_iPriority = 10;
      m_qWebInquiry.push(inquiry);
      m_fInquiringStakeProfileData = true;
    }
    else {
      m_fAmericaStakeUpdated = true;
    }
  }
  return false;
}

bool CAmericaStakeMarket::TaskInquiryDayLine(void) {
  bool fFound = false;
  FinnhubInquiry inquiry;

  ASSERT(m_fSymbolProceeded);
  if (!m_fStakeDayLineUpdated && !m_fInquiringStakeCandle) {
    for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < m_vAmericaStake.size(); m_lCurrentUpdateDayLinePos++) {
      if (m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
        fFound = true;
        break;
      }
    }
    if (fFound) {
      inquiry.m_iIndex = __STAKE_CANDLES__;
      inquiry.m_iPriority = 10;
      m_qWebInquiry.push(inquiry);
      m_fInquiringStakeCandle = true;
      m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->SetDayLineNeedUpdate(false);
      TRACE("����%s��������\n", m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->m_strSymbol.GetBuffer());
    }
    else {
      m_fStakeDayLineUpdated = true;
    }
  }
  return false;
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
bool CAmericaStakeMarket::TaskUpdateDayLineDB(void) {
  CString str;

  for (auto& pStake : m_vAmericaStake) {
    if (pStake->IsDayLineNeedSavingAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
      if (pStake->GetDayLineSize() > 0) {
        if (pStake->HaveNewDayLineData()) {
          RunningThreadUpdateDayLineDB(pStake);
          TRACE("����%s��������\n", pStake->GetSymbol().GetBuffer());
        }
        else pStake->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
      }
      else { // �������Ϊ�й�Ʊ���룬���˴�����δ����
        CString str1 = pStake->GetSymbol();
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

bool CAmericaStakeMarket::RunningThreadUpdateDayLineDB(CAmericaStakePtr pStake) {
  thread thread1(ThreadUpdateAmericaStakeDayLineDB, pStake);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CAmericaStakeMarket::RunningTaskThreadUpdateStakeDB(void) {
  thread thread1(ThreadUpdateStakeDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool TaskUpdateDayLineDB(void) {
  return true;
}

bool CAmericaStakeMarket::IsAmericaStake(CString strSymbol) {
  if (m_mapAmericaStake.find(strSymbol) == m_mapAmericaStake.end()) { // �´��룿
    return false;
  }
  else return true;
}

bool CAmericaStakeMarket::IsAmericaStakeUpdated(void) {
  const int iTotal = m_vAmericaStake.size();
  for (int i = 0; i < iTotal; i++) {
    if (m_vAmericaStake.at(i)->m_fUpdateDatabase) return true;
  }
  return false;
}

CAmericaStakePtr CAmericaStakeMarket::GetAmericaStake(CString strTicker) {
  if (m_mapAmericaStake.find(strTicker) != m_mapAmericaStake.end()) {
    return m_vAmericaStake.at(m_mapAmericaStake.at(strTicker));
  }
  else return nullptr;
}

void CAmericaStakeMarket::AddAmericaStake(CAmericaStakePtr pStake) {
  m_vAmericaStake.push_back(pStake);
  m_mapAmericaStake[pStake->m_strSymbol] = m_lTotalAmericaStake++;
}

void CAmericaStakeMarket::SetFinnInquiry(long lOrder) {
  FinnhubInquiry inquiry;
  inquiry.m_iIndex = lOrder;
  switch (lOrder) {
  case __COMPANY_PROFILE2__:
  case __COMPANY_SYMBOLS__:
  case __MARKET_NEWS__:
  case __FOREX_EXCHANGE__:
  case __FOREX_SYMBOLS__:
  case __CRYPTO_EXCHANGE__:
  case __CRYPTO_SYMBOL__:
  inquiry.m_iPriority = 100;
  break;
  case __COMPANY_NEWS__:
  case __NEWS_SENTIMENT__:
  case __PEERS__:
  case __BASIC_FINANCIALS__:
  case __SEC_FILINGS__:
  inquiry.m_iPriority = 1;
  break;
  case __STAKE_QUOTE__: // ʵʱ�������ȼ����
  case __STAKE_CANDLES__: // ��ʷ�������ȼ���
  case __FOREX_CANDLES__: // ��ʷ�������ȼ���
  case __FOREX_ALL_RATES__:
  case __CRYPTO_CANDLES__:
  inquiry.m_iPriority = 10;
  break;
  default:
  TRACE("����FinnhubInquiry���ȼ�ʱ����δָ֪��%d\n", lOrder);
  inquiry.m_iPriority = 0;
  break;
  }
  m_qWebInquiry.push(inquiry);
}

long CAmericaStakeMarket::GetFinnInquiry(void) {
  FinnhubInquiry inquiry;
  if (m_qWebInquiry.size() > 0) {
    inquiry = m_qWebInquiry.top();
    m_qWebInquiry.pop();
    return inquiry.m_iIndex;
  }
  return -1;
}

bool CAmericaStakeMarket::LoadAmericaStake(void) {
  CSetAmericaStake setAmericaStake;
  CAmericaStakePtr pAmericaStake = nullptr;

  //setAmericaStake.m_strSort = _T("[Symbol]");
  setAmericaStake.Open();
  setAmericaStake.m_pDatabase->BeginTrans();
  while (!setAmericaStake.IsEOF()) {
    pAmericaStake = make_shared<CAmericaStake>();
    pAmericaStake->Load(setAmericaStake);
    m_vAmericaStake.push_back(pAmericaStake);
    m_mapAmericaStake[setAmericaStake.m_Symbol] = m_lLastTotalAmericaStake++;
    setAmericaStake.MoveNext();
  }
  setAmericaStake.m_pDatabase->CommitTrans();
  setAmericaStake.Close();
  m_lTotalAmericaStake = m_vAmericaStake.size();
  ASSERT(m_lLastTotalAmericaStake == m_vAmericaStake.size());
  return true;
}

bool CAmericaStakeMarket::SaveCompnayProfile(void) {
  CSetAmericaStake setAmericaStake;
  CAmericaStakePtr pAmericaStake = nullptr;
  long lTotalAmericaStake = m_lTotalAmericaStake;

  setAmericaStake.Open();
  setAmericaStake.m_pDatabase->BeginTrans();
  for (long l = m_lLastTotalAmericaStake; l < lTotalAmericaStake; l++) {
    pAmericaStake = m_vAmericaStake.at(l);
    pAmericaStake->Save(setAmericaStake);
  }
  setAmericaStake.m_pDatabase->CommitTrans();
  setAmericaStake.Close();
  m_lLastTotalAmericaStake = m_vAmericaStake.size();
  return true;
}

bool CAmericaStakeMarket::UpdateStakeDB(void) {
  const long lTotalAmericaStake = m_vAmericaStake.size();
  CAmericaStakePtr pStake = nullptr;
  CSetAmericaStake setAmericaStake;

  setAmericaStake.Open();
  setAmericaStake.m_pDatabase->BeginTrans();
  while (!setAmericaStake.IsEOF()) {
    pStake = m_vAmericaStake.at(m_mapAmericaStake.at(setAmericaStake.m_Symbol));
    if (pStake->m_fUpdateDatabase) {
      pStake->Update(setAmericaStake);
      pStake->m_fUpdateDatabase = false;
    }
    setAmericaStake.MoveNext();
  }
  setAmericaStake.m_pDatabase->CommitTrans();
  setAmericaStake.Close();
  return true;
}