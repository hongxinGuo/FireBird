#include "AmericaStakeMarket.h"
#include"thread.h"

#include"WebInquirer.h"
#include"ProcessCompanyProfile.h"

#include"SetCompanyProfile.h"

CAmericaStakeMarket::CAmericaStakeMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CAmericaStakeMarket�г�����ֻ�������һ��ʵ��\n");
  }

  m_strMarketId = _T("FinnHub������Ϣ");
  m_lMarketTimeZone = 4 * 3600; // ��������ʹ��������׼ʱ�䡣
  CalculateTime();

  m_vFinnHubInquiringStr.resize(1000);

  // FinnHubǰ׺�ַ����ڴ�Ԥ��֮
  m_vFinnHubInquiringStr.at(__COMPANY_PROFILE2__) = _T("https://finnhub.io/api/v1/stock/profile2?symbol="); // ��˾���
  m_vFinnHubInquiringStr.at(__COMPANY_SYMBOLS__) = _T("https://finnhub.io/api/v1/stock/symbol?exchange=US"); // ���ô��뼯
  m_vFinnHubInquiringStr.at(__MARKET_NEWS__) = _T("https://finnhub.io/api/v1/news?category=general");
  m_vFinnHubInquiringStr.at(__COMPANY_NEWS__) = _T("https://finnhub.io/api/v1/company-news?symbol=");
  m_vFinnHubInquiringStr.at(__NEWS_SETIMENTS__) = _T("https://finnhub.io/api/v1/news-sentiment?symbol=");
  m_vFinnHubInquiringStr.at(__PEERS__) = _T("https://finnhub.io/api/v1/stock/peers?symbol=");
  m_vFinnHubInquiringStr.at(__BASIC_FINANCIALS__) = _T("https://finnhub.io/api/v1/stock/metric?symbol=");
  m_vFinnHubInquiringStr.at(__SEC_FILINGS__) = _T("https://finnhub.io/api/v1/stock/filings?symbol=");

  m_vFinnHubInquiringStr.at(__STAKE_QUOTE__) = _T("https://finnhub.io/api/v1/quote?symbol="); // ĳ������Ľ���
  m_vFinnHubInquiringStr.at(__STAKE_CANDLES__) = _T("https://finnhub.io/api/v1/stock/candle?symbol="); // ��ʷ����ͼ

  m_vFinnHubInquiringStr.at(__FOREX_EXCHANGE__) = _T("https://finnhub.io/api/v1/forex/exchange?");
  m_vFinnHubInquiringStr.at(__FOREX_SYMBOLS__) = _T("https://finnhub.io/api/v1/forex/symbol?exchange=oanda");
  m_vFinnHubInquiringStr.at(__FOREX_CANDLES__) = _T("https://finnhub.io/api/v1/forex/candle?symbol=OANDA:EUR_USD&resolution=D");
  m_vFinnHubInquiringStr.at(__FOREX_ALL_RATES__) = _T("https://finnhub.io/api/v1/forex/rates?base=USD");

  m_lPrefixIndex = -1; //
  Reset();
}

CAmericaStakeMarket::~CAmericaStakeMarket() {
}

void CAmericaStakeMarket::Reset(void) {
  m_lTotalCompanyProfile = 0;
  m_lLastTotalCompanyProfile = 0;
  m_lCurrentProfilePos = 0;
  m_lCurrentUpdateDayLinePos = 0;
  m_vCompanyProfile.resize(0);
  m_mapCompanyProfile.clear();
  m_fSymbolUpdated = false; // ÿ����Ҫ���´���
  m_fSymbolProceeded = false;
  m_fCompanyProfileUpdated = false;
  m_fStakeDayLineUpdated = false;

  m_fInquiringComprofileData = false;
  m_fInquiringStakeCandle = false;
}

bool CAmericaStakeMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  const long lCurrentTime = GetFormatedMarketTime();

  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (GetMarketTime() > s_timeLast) {
    SchedulingTaskPerSecond(GetMarketTime() - s_timeLast, lCurrentTime);
    s_timeLast = GetMarketTime();
  }

  return true;
}

void CAmericaStakeMarket::GetFinnHubDataFromWeb(void) {
  static bool s_fWaitingData = false;
  CWebDataPtr pWebData = nullptr;
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  char buffer[50];

  if (s_fWaitingData) {
    if (!IsWaitingFinHubData()) {// �Ѿ��������������룿
      s_fWaitingData = false;
      if (gl_WebInquirer.GetFinnHubDataSize() > 0) { // ����������ݽ��յ���
        // ����ǰ��������
        pWebData = gl_WebInquirer.PopFinnHubData();
        switch (m_lPrefixIndex) {
        case __COMPANY_PROFILE2__:
        ProcessCompanyProfile(pWebData);
        m_fInquiringComprofileData = false;
        break;
        case  __COMPANY_SYMBOLS__:
        ProcessCompanySymbol(pWebData);
        m_fSymbolProceeded = true;
        break;
        case  __MARKET_NEWS__:
        break;
        case __COMPANY_NEWS__:
        break;
        case __NEWS_SETIMENTS__:
        break;
        case __PEERS__:
        break;
        case __BASIC_FINANCIALS__:
        break;
        case __STAKE_QUOTE__:
        break;
        case __STAKE_CANDLES__:
        ProcessStakeCandle(pWebData, m_vCompanyProfile.at(m_lCurrentUpdateDayLinePos));
        m_fInquiringStakeCandle = false;
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
      gl_pFinnhubWebInquiry->SetInquiryingStrPrefix(m_vFinnHubInquiringStr.at(m_lPrefixIndex)); // ����ǰ׺
      switch (m_lPrefixIndex) { // ���ݲ�ͬ��Ҫ��������׺�ַ���
      case __COMPANY_PROFILE2__:
      while (!m_vCompanyProfile.at(m_lCurrentProfilePos)->m_fInquiryCompanyProfile) m_lCurrentProfilePos++;
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vCompanyProfile.at(m_lCurrentProfilePos)->m_strSymbol);
      m_vCompanyProfile.at(m_lCurrentProfilePos)->m_fInquiryCompanyProfile = false;
      break;
      case  __COMPANY_SYMBOLS__:
      // do nothing
      break;
      case  __MARKET_NEWS__:
      break;
      case __COMPANY_NEWS__:
      break;
      case __NEWS_SETIMENTS__:
      break;
      case __PEERS__:
      break;
      case __BASIC_FINANCIALS__:
      break;
      case __STAKE_QUOTE__:
      break;
      case __STAKE_CANDLES__:
      strMiddle += m_vCompanyProfile.at(m_lCurrentUpdateDayLinePos)->m_strSymbol;
      strMiddle += _T("&resolution=D");
      strMiddle += _T("&from=");
      sprintf_s(buffer, _T("%I64i"), (INT64)(GetMarketTime() - (time_t)(360) * 24 * 3600));
      strTemp = buffer;
      strMiddle += strTemp;
      strMiddle += _T("&to=");
      sprintf_s(buffer, _T("%I64i"), GetMarketTime());
      strTemp = buffer;
      strMiddle += strTemp;
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      m_vCompanyProfile.at(m_lCurrentUpdateDayLinePos)->m_fDayLineNeedUpdate = false;
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
      SetWaitingFinnHubData(true);
    }
  }
}

void CAmericaStakeMarket::ResetMarket(void) {
  Reset();

  LoadCompanyProfile();

  CString str = _T("����America Stake Market��������׼ʱ�䣺");
  str += GetStringOfMarketTime();
  gl_systemMessage.PushInformationMessage(str);
}

bool CAmericaStakeMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
  static int s_iCount = 1;

  SchedulingTaskPer1Hour(lSecond, lCurrentTime);
  SchedulingTaskPer1Minute(lSecond, lCurrentTime);

  GetFinnHubDataFromWeb();

  TaskUpdateTodaySymbol();
  TaskSaveCompanySymbolDB();

  if (m_fSymbolProceeded) {
    TaskUpdateCompanyProfile();
    //TaskUpdateDayLine();
  }

  return true;
}

bool CAmericaStakeMarket::SchedulingTaskPer1Hour(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 3599;  // һ����һ�εļ�����

  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 3599;
    TaskResetMarket(lCurrentTime);
    //TaskMaintainDatabase(lCurrentTime);

    return true;
  }
  else return false;
}

bool CAmericaStakeMarket::SchedulingTaskPer1Minute(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 59;  // һСʱһ�εļ�����

  TaskResetMarket(lCurrentTime);

  // �Զ���ѯcrweber.com
  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 59;
    if (IsCompanyProfileUpdated()) {
      TaskUpdateCompanyProfileDB();
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

bool CAmericaStakeMarket::TaskUpdateTodaySymbol(void) {
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

bool CAmericaStakeMarket::TaskSaveCompanySymbolDB(void) {
  CSetCompanyProfile setCompanyProfile;
  CCompanyProfilePtr pProfile = nullptr;

  if (m_lLastTotalCompanyProfile < m_lTotalCompanyProfile) {
    setCompanyProfile.Open();
    setCompanyProfile.m_pDatabase->BeginTrans();
    for (long l = m_lLastTotalCompanyProfile; l < m_lTotalCompanyProfile; l++) {
      pProfile = m_vCompanyProfile.at(l);
      pProfile->Save(setCompanyProfile);
    }
    setCompanyProfile.m_pDatabase->CommitTrans();
    setCompanyProfile.Close();
    m_lLastTotalCompanyProfile = m_lTotalCompanyProfile;
  }
  return true;
}

bool CAmericaStakeMarket::TaskUpdateCompanyProfile(void) {
  bool fFound = false;
  FinnHubInquiry inquiry;

  ASSERT(m_fSymbolProceeded);
  if (!m_fCompanyProfileUpdated && !m_fInquiringComprofileData) {
    for (m_lCurrentProfilePos = 0; m_lCurrentProfilePos < m_vCompanyProfile.size(); m_lCurrentProfilePos++) {
      if (IsEarlyThen(m_vCompanyProfile.at(m_lCurrentProfilePos)->m_lCompanyProfileUpdateDate, GetFormatedMarketDate(), 365)) {
        fFound = true;
        break;
      }
    }
    if (fFound) {
      inquiry.m_iIndex = __COMPANY_PROFILE2__;
      inquiry.m_iPriority = 10;
      m_qWebInquiry.push(inquiry);
      m_fInquiringComprofileData = true;
    }
    else {
      m_fCompanyProfileUpdated = true;
    }
  }
  return false;
}

bool CAmericaStakeMarket::TaskUpdateCompanyProfileDB(void) {
  const long lTotalCompanyProfile = m_vCompanyProfile.size();
  CCompanyProfilePtr pProfile = nullptr;
  CSetCompanyProfile setCompanyProfile;

  setCompanyProfile.Open();
  setCompanyProfile.m_pDatabase->BeginTrans();
  for (long l = 0; l < lTotalCompanyProfile; l++) {
    pProfile = m_vCompanyProfile.at(l);
    if (pProfile->m_fUpdateDatabase) {
      while ((setCompanyProfile.m_Symbol.Compare(pProfile->m_strSymbol) != 0) && !setCompanyProfile.IsEOF()) {
        setCompanyProfile.MoveNext();
      }
      if (setCompanyProfile.IsEOF()) break;
      pProfile->Update(setCompanyProfile);
      pProfile->m_fUpdateDatabase = false;
    }
  }
  setCompanyProfile.m_pDatabase->CommitTrans();
  setCompanyProfile.Close();
  return true;
}

bool CAmericaStakeMarket::TaskUpdateDayLine(void) {
  bool fFound = false;
  FinnHubInquiry inquiry;

  ASSERT(m_fSymbolProceeded);
  if (!m_fStakeDayLineUpdated && !m_fInquiringStakeCandle) {
    for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < m_vCompanyProfile.size(); m_lCurrentUpdateDayLinePos++) {
      if (m_vCompanyProfile.at(m_lCurrentUpdateDayLinePos)->m_fDayLineNeedUpdate) {
        fFound = true;
        break;
      }
    }
    if (fFound) {
      inquiry.m_iIndex = __STAKE_CANDLES__;
      inquiry.m_iPriority = 10;
      m_qWebInquiry.push(inquiry);
      m_fInquiringStakeCandle = true;
    }
    else {
      m_fStakeDayLineUpdated = true;
    }
  }
  return false;
}

bool TaskUpdateDayLineDB(void) {
  return true;
}

bool CAmericaStakeMarket::IsCompanyProfile(CString strSymbol) {
  if (m_mapCompanyProfile.find(strSymbol) == m_mapCompanyProfile.end()) { // �´��룿
    return false;
  }
  else return true;
}

bool CAmericaStakeMarket::IsCompanyProfileUpdated(void) {
  const int iTotal = m_vCompanyProfile.size();
  for (int i = 0; i < iTotal; i++) {
    if (m_vCompanyProfile.at(i)->m_fUpdateDatabase) return true;
  }
  return false;
}

CCompanyProfilePtr CAmericaStakeMarket::GetCompanyProfile(CString strTicker) {
  if (m_mapCompanyProfile.find(strTicker) != m_mapCompanyProfile.end()) {
    return m_vCompanyProfile.at(m_mapCompanyProfile.at(strTicker));
  }
  else return nullptr;
}

void CAmericaStakeMarket::AddCompanyProfile(CCompanyProfilePtr pProfile) {
  m_vCompanyProfile.push_back(pProfile);
  m_mapCompanyProfile[pProfile->m_strSymbol] = m_lTotalCompanyProfile++;
}

void CAmericaStakeMarket::SetFinnInquiry(long lOrder) {
  FinnHubInquiry inquiry;
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
  case __NEWS_SETIMENTS__:
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
  TRACE("����FinnHubInquiry���ȼ�ʱ����δָ֪��%d\n", lOrder);
  inquiry.m_iPriority = 0;
  break;
  }
  m_qWebInquiry.push(inquiry);
}

long CAmericaStakeMarket::GetFinnInquiry(void) {
  FinnHubInquiry inquiry;
  if (m_qWebInquiry.size() > 0) {
    inquiry = m_qWebInquiry.top();
    m_qWebInquiry.pop();
    return inquiry.m_iIndex;
  }
  return -1;
}

bool CAmericaStakeMarket::LoadCompanyProfile(void) {
  CSetCompanyProfile setCompanyProfile;
  CCompanyProfilePtr pCompanyProfile = nullptr;

  setCompanyProfile.Open();
  setCompanyProfile.m_pDatabase->BeginTrans();
  while (!setCompanyProfile.IsEOF()) {
    pCompanyProfile = make_shared<CCompanyProfile>();
    pCompanyProfile->Load(setCompanyProfile);
    m_vCompanyProfile.push_back(pCompanyProfile);
    m_mapCompanyProfile[setCompanyProfile.m_Symbol] = m_lLastTotalCompanyProfile++;
    setCompanyProfile.MoveNext();
  }
  setCompanyProfile.m_pDatabase->CommitTrans();
  setCompanyProfile.Close();
  m_lTotalCompanyProfile = m_vCompanyProfile.size();
  ASSERT(m_lLastTotalCompanyProfile == m_vCompanyProfile.size());
  return true;
}

bool CAmericaStakeMarket::SaveCompnayProfile(void) {
  CSetCompanyProfile setCompanyProfile;
  CCompanyProfilePtr pCompanyProfile = nullptr;
  long lTotalCompanyProfile = m_lTotalCompanyProfile;

  setCompanyProfile.Open();
  setCompanyProfile.m_pDatabase->BeginTrans();
  for (long l = m_lLastTotalCompanyProfile; l < lTotalCompanyProfile; l++) {
    pCompanyProfile = m_vCompanyProfile.at(l);
    pCompanyProfile->Save(setCompanyProfile);
  }
  setCompanyProfile.m_pDatabase->CommitTrans();
  setCompanyProfile.Close();
  m_lLastTotalCompanyProfile = m_vCompanyProfile.size();
  return true;
}