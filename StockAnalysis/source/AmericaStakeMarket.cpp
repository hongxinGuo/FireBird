#include "AmericaStakeMarket.h"
#include"thread.h"

#include"WebInquirer.h"
#include"ProcessCompanySymbol.h"
#include"ProcessCompanyProfile.h"

#include"SetCompanySymbol.h"

CAmericaStakeMarket::CAmericaStakeMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CAmericaStakeMarket市场变量只允许存在一个实例\n");
  }

  m_strMarketId = _T("FinnHub美股信息");
  m_lMarketTimeZone = 4 * 3600; // 美国股市使用美东标准时间。
  CalculateTime();

  m_vFinnHubInquiringStr.resize(1000);

  // FinnHub前缀字符串在此预设之
  m_vFinnHubInquiringStr.at(__COMPANY_PROFILE2__) = _T("https://finnhub.io/api/v1/stock/profile2?symbol="); // 公司简介
  m_vFinnHubInquiringStr.at(__COMPANY_SYMBOLS__) = _T("https://finnhub.io/api/v1/stock/symbol?exchange=US"); // 可用代码集
  m_vFinnHubInquiringStr.at(__MARKET_NEWS__) = _T("https://finnhub.io/api/v1/news?category=general");
  m_vFinnHubInquiringStr.at(__COMPANY_NEWS__) = _T("https://finnhub.io/api/v1/company-news?symbol=");
  m_vFinnHubInquiringStr.at(__NEWS_SETIMENTS__) = _T("https://finnhub.io/api/v1/news-sentiment?symbol=");
  m_vFinnHubInquiringStr.at(__PEERS__) = _T("https://finnhub.io/api/v1/stock/peers?symbol=");
  m_vFinnHubInquiringStr.at(__BASIC_FINANCIALS__) = _T("https://finnhub.io/api/v1/stock/metric?symbol=");
  m_vFinnHubInquiringStr.at(__SEC_FILINGS__) = _T("https://finnhub.io/api/v1/stock/filings?symbol=");

  m_vFinnHubInquiringStr.at(__QUOTE__) = _T("https://finnhub.io/api/v1/quote?symbol="); // 某个代码的交易
  m_vFinnHubInquiringStr.at(__CANDLES__) = _T("https://finnhub.io/api/v1/stock/candle?symbol="); // 历史蜡烛图

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
  m_fSymbolUpdated = false; // 每日需要更新代码
  m_fCompanyProfileUpdated = false;
  m_fInquiringComprofileData = false;
  m_lLastTotalCompanySymbol = 0;
  m_lTotalCompanySymbol = 0;
  m_vCompanySymbol.resize(0);
  m_mapConpanySymbol.clear();
}

bool CAmericaStakeMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  const long lCurrentTime = GetFormatedMarketTime();

  //根据时间，调度各项定时任务.每秒调度一次
  if (GetMarketTime() > s_timeLast) {
    SchedulingTaskPerSecond(GetMarketTime() - s_timeLast, lCurrentTime);
    s_timeLast = GetMarketTime();
  }

  return true;
}

void CAmericaStakeMarket::GetFinnHubDataFromWeb(void) {
  static bool s_fWaitData = false;
  CWebDataPtr pWebData = nullptr;

  if (s_fWaitData) {// 已经发出了数据申请？
    if (!IsWaitingFinHubData()) {
      s_fWaitData = false;
      if (gl_WebInquirer.GetFinnHubDataSize() > 0) { // 如果网络数据接收到了
        // 处理当前网络数据
        pWebData = gl_WebInquirer.PopFinnHubData();
        switch (m_lPrefixIndex) {
        case __COMPANY_PROFILE2__:
        ProcessCompanyProfile(pWebData);
        m_fInquiringComprofileData = false;
        break;
        case  __COMPANY_SYMBOLS__:
        ProcessCompanySymbolData(pWebData);
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
        case __QUOTE__:
        break;
        case __CANDLES__:
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
  else { // 没发出网络数据申请的话
    if (m_qWebInquiry.size() > 0) { // 有申请等待？
      m_lPrefixIndex = GetFinnInquiry();
      gl_pFinnhubWebInquiry->SetInquiryingStrPrefix(m_vFinnHubInquiringStr.at(m_lPrefixIndex)); // 设置前缀
      switch (m_lPrefixIndex) {
      case __COMPANY_PROFILE2__:
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vCompanySymbol.at(m_lCurrentProfilePos)->m_strSymbol);
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
      case __QUOTE__:
      break;
      case __CANDLES__:
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
      s_fWaitData = true;
      SetWaitingFinnHubData(true);
    }
  }
}

void CAmericaStakeMarket::ResetMarket(void) {
  Reset();

  LoadCompanySymbol();
  LoadCompanyProfile();

  CString str = _T("重置America Stake Market于美东标准时间：");
  str += GetStringOfMarketTime();
  gl_systemMessage.PushInformationMessage(str);
}

bool CAmericaStakeMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
  static int s_iCount = 1;

  SchedulingTaskPer1Hour(lSecond, lCurrentTime);
  SchedulingTaskPer1Minute(lSecond, lCurrentTime);

  if (s_iCount-- < 0) { // 每两秒申请一次FinnHub数据
    s_iCount = 1;
    GetFinnHubDataFromWeb();
  }

  //TaskUpdateTodaySymbol();
  //TaskSaveCompanySymbol();

  TaskUpdateComProfile();

  return true;
}

bool CAmericaStakeMarket::SchedulingTaskPer1Hour(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 3599;  // 一分钟一次的计数器

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
  static int i1MinuteCounter = 59;  // 一小时一次的计数器

  TaskResetMarket(lCurrentTime);

  // 自动查询crweber.com
  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 59;
    return true;
  }
  else {
    return false;
  }
}

bool CAmericaStakeMarket::TaskResetMarket(long lCurrentTime) {
  // 市场时间四点重启系统
  if (IsPermitResetMarket()) { // 如果允许重置系统
    if ((lCurrentTime >= 29000) && (lCurrentTime < 30000)) { // 本市场时间的一点重启本市场 // 东八区本地时间为下午五时（或夏令时的四时）。
      SetResetMarket(true);// 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
      SetPermitResetMarket(false); // 今天不再允许重启系统。
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

bool CAmericaStakeMarket::TaskSaveCompanySymbol(void) {
  CSetCompanySymbol setCompanySymbol;
  CCompanySymbolPtr pSymbol = nullptr;

  if (m_lLastTotalCompanySymbol < m_lTotalCompanySymbol) {
    setCompanySymbol.Open();
    setCompanySymbol.m_pDatabase->BeginTrans();
    for (long l = m_lLastTotalCompanySymbol; l < m_lTotalCompanySymbol; l++) {
      pSymbol = m_vCompanySymbol.at(l);
      pSymbol->Save(setCompanySymbol);
    }
    setCompanySymbol.m_pDatabase->CommitTrans();
    setCompanySymbol.Close();
    m_lLastTotalCompanySymbol = m_lTotalCompanySymbol;
  }
  return true;
}

bool CAmericaStakeMarket::TaskUpdateComProfile(void) {
  bool fFound = false;
  FinnHubInquiry inquiry;

  if (!m_fCompanyProfileUpdated && !m_fInquiringComprofileData) {
    for (m_lCurrentProfilePos = 0; m_lCurrentProfilePos < m_vCompanyProfile.size(); m_lCurrentProfilePos++) {
      if (IsEarlyThen(m_vCompanyProfile.at(m_lCurrentProfilePos)->m_lLastUpdateDate, GetFormatedMarketDate(), 365)) {
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

bool CAmericaStakeMarket::TaskSaveCompanyProfile(void) {
  for (auto& pProfile : m_vCompanyProfile) {
  }
  return false;
}

bool CAmericaStakeMarket::IsCompanySymbol(CString strSymbol) {
  if (m_mapConpanySymbol.find(strSymbol) == m_mapConpanySymbol.end()) { // 新代码？
    return false;
  }
  else return true;
}

void CAmericaStakeMarket::AddCompanySymbol(CCompanySymbolPtr pSymbol) {
  m_vCompanySymbol.push_back(pSymbol);
  m_mapConpanySymbol[pSymbol->m_strSymbol] = m_lLastTotalCompanySymbol++;
}

void CAmericaStakeMarket::AddCompanyProfile(CCompanyProfilePtr pProfile) {
  const long lIndex = m_mapConpanySymbol.at(pProfile->m_strName);
  m_vCompanyProfile.at(lIndex)->m_strCountry = pProfile->m_strCountry;
  m_vCompanyProfile.at(lIndex)->m_strCurrency = pProfile->m_strCurrency;
  m_vCompanyProfile.at(lIndex)->m_strExchange = pProfile->m_strExchange;
  m_vCompanyProfile.at(lIndex)->m_lIPODate = pProfile->m_lIPODate;
  m_vCompanyProfile.at(lIndex)->m_strLogo = pProfile->m_strLogo;
  m_vCompanyProfile.at(lIndex)->m_strShareOutstanding = pProfile->m_strShareOutstanding;
  m_vCompanyProfile.at(lIndex)->m_strName = pProfile->m_strName;
  m_vCompanyProfile.at(lIndex)->m_strPhone = pProfile->m_strPhone;
  m_vCompanyProfile.at(lIndex)->m_strShareOutstanding = pProfile->m_strShareOutstanding;
  m_vCompanyProfile.at(lIndex)->m_strTicker = pProfile->m_strTicker;
  m_vCompanyProfile.at(lIndex)->m_strWebURL = pProfile->m_strWebURL;
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
  case __QUOTE__: // 实时数据优先级最低
  case __CANDLES__: // 历史数据优先级低
  case __FOREX_CANDLES__: // 历史数据优先级低
  case __FOREX_ALL_RATES__:
  case __CRYPTO_CANDLES__:
  inquiry.m_iPriority = 10;
  break;
  default:
  TRACE("设置FinnHubInquiry优先级时出现未知指令%d\n", lOrder);
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

bool CAmericaStakeMarket::LoadCompanySymbol(void) {
  CSetCompanySymbol setCompanySymbol;
  CCompanySymbolPtr pCompanySymbol = nullptr;
  CCompanyProfilePtr pCompanyProfile = nullptr;

  setCompanySymbol.Open();
  setCompanySymbol.m_pDatabase->BeginTrans();
  while (!setCompanySymbol.IsEOF()) {
    pCompanySymbol = make_shared<CCompanySymbol>();
    pCompanySymbol->Load(setCompanySymbol);
    m_vCompanySymbol.push_back(pCompanySymbol);
    pCompanyProfile = make_shared<CCompanyProfile>();
    pCompanyProfile->Update(pCompanySymbol);
    m_vCompanyProfile.push_back(pCompanyProfile);
    m_mapConpanySymbol[setCompanySymbol.m_Symbol] = m_lLastTotalCompanySymbol++;
    setCompanySymbol.MoveNext();
  }
  setCompanySymbol.m_pDatabase->CommitTrans();
  setCompanySymbol.Close();
  m_lTotalCompanyProfile = m_vCompanyProfile.size();
  ASSERT(m_lLastTotalCompanySymbol == m_vCompanySymbol.size());
  return true;
}

bool CAmericaStakeMarket::SaveCompnaySymbol(void) {
  CSetCompanySymbol setCompanySymbol;
  CCompanySymbolPtr pCompanySymbol = nullptr;

  setCompanySymbol.Open();
  setCompanySymbol.m_pDatabase->BeginTrans();
  for (long l = m_lLastTotalCompanySymbol; l < m_vCompanySymbol.size(); l++) {
    pCompanySymbol = m_vCompanySymbol.at(l);
    pCompanySymbol->Save(setCompanySymbol);
  }
  setCompanySymbol.m_pDatabase->CommitTrans();
  setCompanySymbol.Close();
  m_lLastTotalCompanySymbol = m_vCompanySymbol.size();
  return true;
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
    setCompanyProfile.MoveNext();
  }
  setCompanyProfile.m_pDatabase->CommitTrans();
  setCompanyProfile.Close();
  return true;
}