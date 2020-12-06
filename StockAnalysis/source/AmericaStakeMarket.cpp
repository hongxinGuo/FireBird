#include "AmericaStakeMarket.h"
#include"thread.h"

#include"WebInquirer.h"

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
  m_lLastTotalCompanySymbol = 0;
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
      break;
      case  __COMPANY_SYMBOLS__:
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

  TaskUpdateTodaySymbol();

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

bool CAmericaStakeMarket::ProcessCompanySymbolData(CWebDataPtr pWebData) {
  CCompanySymbolPtr pSymbol = nullptr;

  if (pWebData->GetCurrentPosData() != '[') return false;
  pWebData->IncreaseCurrentPos();
  while (pWebData->GetCurrentPos() < pWebData->GetBufferLength()) {
    if ((pSymbol = ReadOneSymbol(pWebData)) != nullptr) {
      if (m_mapConpanySymbol.find(pSymbol->m_strSymbol) == m_mapConpanySymbol.end()) { // 新代码？
        m_vCompanySymbol.push_back(pSymbol);
        m_mapConpanySymbol[pSymbol->m_strSymbol] = m_lTotalCompanySymbol++;
      }
    }
    else return false;
    if (!(pWebData->GetCurrentPosData() == ',')) { // 读完了？
      if (pWebData->GetCurrentPosData() == ']') return true; // 读完了
    }
    else {
      pWebData->IncreaseCurrentPos();
    }
  }
  return false;
}

map<CString, long> gl_mapCompanySymbol{ {"description", 0 }, {"displaySymbol", 1}, {"symbol", 2}, {"type", 3}, {"currency", 4} };

CCompanySymbolPtr CAmericaStakeMarket::ReadOneSymbol(CWebDataPtr pWebData) {
  CString strDescription = _T("");
  CString strSymbol;
  CString strDisplaySymbol;
  CString strType;
  CString strCurrency;
  CString strTemp, strTemp2;
  CCompanySymbolPtr pSymbol = make_shared<CCompanySymbol>();
  char buffer[1000];
  int i1 = 0;
  long lCurrentPos = pWebData->GetCurrentPos();
  while (pWebData->GetData(lCurrentPos) != '}') buffer[i1++] = pWebData->GetData(lCurrentPos++);
  buffer[i1] = '}';
  buffer[i1 + 1] = 0x000;

  while (pWebData->GetCurrentPosData() != '{') pWebData->IncreaseCurrentPos();
  pWebData->IncreaseCurrentPos(); // 跨过字符{
  for (int i = 0; i < 5; i++) { // 共五个数据
    strTemp = ReadString(pWebData);
    if (gl_mapCompanySymbol.find(strTemp) != gl_mapCompanySymbol.end()) {
      while (pWebData->GetCurrentPosData() != '"') pWebData->IncreaseCurrentPos();
      strTemp2 = ReadString(pWebData);
      switch (gl_mapCompanySymbol.at(strTemp)) {
      case 0:
      pSymbol->m_strDescription = strTemp2;
      break;
      case 1:
      pSymbol->m_strDisplaySymbol = strTemp2;
      break;
      case 2:
      pSymbol->m_strSymbol = strTemp2;
      break;
      case 3:
      pSymbol->m_strType = strTemp2;
      break;
      case 4:
      pSymbol->m_strCurrency = strTemp2;
      break;
      default:
      break;
      }
      while (pWebData->GetCurrentPosData() != '"') pWebData->IncreaseCurrentPos();
    }
  }
  while (pWebData->GetCurrentPosData() != '}') pWebData->IncreaseCurrentPos();
  pWebData->IncreaseCurrentPos();
  return pSymbol;
}

CString CAmericaStakeMarket::ReadString(CWebDataPtr pWebData) {
  char buffer[100];
  int i = 0;
  CString str = _T("");

  if (pWebData->GetCurrentPosData() != '"') return str; // error
  pWebData->IncreaseCurrentPos();
  while (pWebData->GetCurrentPosData() != '"') {
    buffer[i++] = pWebData->GetCurrentPosData();
    pWebData->IncreaseCurrentPos();
  }
  pWebData->IncreaseCurrentPos();
  buffer[i] = 0x000;
  str = buffer;
  return str;
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

  setCompanySymbol.Open();
  setCompanySymbol.m_pDatabase->BeginTrans();
  while (!setCompanySymbol.IsEOF()) {
    pCompanySymbol = make_shared<CCompanySymbol>();
    pCompanySymbol->Load(setCompanySymbol);
    m_vCompanySymbol.push_back(pCompanySymbol);
    m_mapConpanySymbol[setCompanySymbol.m_Symbol] = m_lLastTotalCompanySymbol++;
    setCompanySymbol.MoveNext();
  }
  setCompanySymbol.m_pDatabase->CommitTrans();
  setCompanySymbol.Close();
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