#include "AmericaStakeMarket.h"
#include"thread.h"

#include"WebInquirer.h"
#include"ProcessAmericaStake.h"

#include"SetForexExchange.h"
#include"SetAmericaStake.h"

CAmericaStakeMarket::CAmericaStakeMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CAmericaStakeMarket市场变量只允许存在一个实例\n");
  }

  m_strMarketId = _T("Finnhub美股信息");
  m_lMarketTimeZone = 4 * 3600; // 美国股市使用美东标准时间。
  CalculateTime();

  m_vFinnhubInquiringStr.resize(1000);

  // Finnhub前缀字符串在此预设之
  m_vFinnhubInquiringStr.at(__COMPANY_PROFILE__) = _T("https://finnhub.io/api/v1/stock/profile?symbol="); // 公司简介。
  m_vFinnhubInquiringStr.at(__COMPANY_PROFILE2__) = _T("https://finnhub.io/api/v1/stock/profile2?symbol="); // 公司简介（简版）
  m_vFinnhubInquiringStr.at(__COMPANY_SYMBOLS__) = _T("https://finnhub.io/api/v1/stock/symbol?exchange=US"); // 可用代码集
  m_vFinnhubInquiringStr.at(__MARKET_NEWS__) = _T("https://finnhub.io/api/v1/news?category=general");
  m_vFinnhubInquiringStr.at(__COMPANY_NEWS__) = _T("https://finnhub.io/api/v1/company-news?symbol=");
  m_vFinnhubInquiringStr.at(__NEWS_SENTIMENT__) = _T("https://finnhub.io/api/v1/news-sentiment?symbol=");
  m_vFinnhubInquiringStr.at(__PEERS__) = _T("https://finnhub.io/api/v1/stock/peers?symbol=");
  m_vFinnhubInquiringStr.at(__BASIC_FINANCIALS__) = _T("https://finnhub.io/api/v1/stock/metric?symbol=");
  m_vFinnhubInquiringStr.at(__SEC_FILINGS__) = _T("https://finnhub.io/api/v1/stock/filings?symbol=");

  m_vFinnhubInquiringStr.at(__STAKE_QUOTE__) = _T("https://finnhub.io/api/v1/quote?symbol="); // 某个代码的交易
  m_vFinnhubInquiringStr.at(__STAKE_CANDLES__) = _T("https://finnhub.io/api/v1/stock/candle?symbol="); // 历史蜡烛图

  m_vFinnhubInquiringStr.at(__FOREX_EXCHANGE__) = _T("https://finnhub.io/api/v1/forex/exchange?");
  m_vFinnhubInquiringStr.at(__FOREX_SYMBOLS__) = _T("https://finnhub.io/api/v1/forex/symbol?exchange=");
  m_vFinnhubInquiringStr.at(__FOREX_CANDLES__) = _T("https://finnhub.io/api/v1/forex/candle?symbol=OANDA:EUR_USD&resolution=D");
  m_vFinnhubInquiringStr.at(__FOREX_ALL_RATES__) = _T("https://finnhub.io/api/v1/forex/rates?base=USD");

  Reset();
}

CAmericaStakeMarket::~CAmericaStakeMarket() {
}

void CAmericaStakeMarket::Reset(void) {
  m_lTotalAmericaStake = 0;
  m_lLastTotalAmericaStake = 0;
  m_lCurrentProfilePos = 0;
  m_lCurrentUpdateDayLinePos = 0;
  m_lCurrentRTDataQuotePos = 0;
  m_lCurrentForexExchangePos = 0;
  m_lCurrentForexSymbolPos = 0;

  m_vAmericaStake.resize(0);
  m_mapAmericaStake.clear();
  m_fSymbolUpdated = false; // 每日需要更新代码
  m_fAmericaStakeUpdated = false;
  m_fStakeDayLineUpdated = false;
  m_fForexExhangeUpdated = false;
  m_fForexSymbolUpdated = false;

  m_fFinnhubInquiring = false;
  m_fFinnhubDataReceived = true;

  m_fRebulidDayLine = false;
  SetSystemReady(false); // 市场初始状态为未设置好。

  m_vForexExchange.resize(0);
  m_mapForexExchange.clear();
  m_lLastTotalForexExchange = 0;
  m_lTotalForexExchange = 0;
  m_vForexSymbol.resize(0);
  m_mapForexSymbol.clear();
  m_lLastTotalForexSymbol = 0;
  m_lTotalForexSymbol = 0;
}

void CAmericaStakeMarket::ResetMarket(void) {
  Reset();

  LoadAmericaStake();
  LoadForexExchange();
  LoadForexSymbol();

  CString str = _T("重置America Stake Market于美东标准时间：");
  str += GetStringOfMarketTime();
  gl_systemMessage.PushInformationMessage(str);
}

bool CAmericaStakeMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  static int s_iCountfinnhubLimit = 11; // 每1.2秒左右执行一次，以防止出现频率过高的情况。
  const long lCurrentTime = GetFormatedMarketTime();

  TaskCheckSystemReady();

  TaskInquiryTodaySymbol(); // 第一个动作，首先申请当日证券代码
  TaskInquiryFinnhubForexExchange();
  TaskInquiryFinnhubForexSymbol();

  ProcessFinnhubWebDataReceived(); // 要先处理收到的Finnhub网络数据
  ProcessFinnhubInquiringMessage(); // 然后再申请处理下一个

  // 申请Finnhub网络信息的任务，皆要放置在这里，以保证在市场时间凌晨十分钟后执行。这样能够保证在重启市场时没有执行查询任务
  if (IsSystemReady() && (lCurrentTime > 1000) && !m_fFinnhubInquiring && (s_iCountfinnhubLimit < 0)) {
    s_iCountfinnhubLimit = 11;
    TaskInquiryAmericaStake();
    TaskInquiryDayLine();
    if (m_fStakeDayLineUpdated) {
      //TaskInquiryFinnhubRTQuote();
    }
  }
  else s_iCountfinnhubLimit--;

  //根据时间，调度各项定时任务.每秒调度一次
  if (GetMarketTime() > s_timeLast) {
    SchedulingTaskPerSecond(GetMarketTime() - s_timeLast, lCurrentTime);
    s_timeLast = GetMarketTime();
  }

  return true;
}

//////////////////////////////////////////////
//
// 处理工作线程接收到的Finnhub网络信息。
//
// 目前只允许同时处理一条信息。即信息从申请至处理完之前，不允许处理下一条信息。这样能够保证同一性。且由于Finnhub网站有速度限制，
// 每分钟只允许60次申请，故而没有必要强调处理速度。
//
//////////////////////////////////////////////
bool CAmericaStakeMarket::ProcessFinnhubWebDataReceived(void) {
  CWebDataPtr pWebData = nullptr;
  CString str = _T("");
  vector<CString> vExchange;
  vector<CForexSymbolPtr> vForexSymbol;
  long lTemp = 0;

  ASSERT(gl_WebInquirer.GetFinnhubDataSize() <= 1);
  if (IsFinnhubDataReceived()) { // 如果网络数据接收完成
    if (gl_WebInquirer.GetFinnhubDataSize() > 0) {  // 处理当前网络数据
      ASSERT(m_fFinnhubInquiring);
      pWebData = gl_WebInquirer.PopFinnhubData();
      switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) {
      case __COMPANY_PROFILE__: // 目前免费账户无法使用此功能。
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentProfilePos);
      ProcessAmericaStakeProfile(pWebData, m_vAmericaStake.at(m_lCurrentProfilePos));
      break;
      case __COMPANY_PROFILE2__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentProfilePos);
      ProcessAmericaStakeProfile2(pWebData, m_vAmericaStake.at(m_lCurrentProfilePos));
      break;
      case  __COMPANY_SYMBOLS__:
      ProcessAmericaStakeSymbol(pWebData);
      m_fSymbolUpdated = true;
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
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentRTDataQuotePos);
      ProcessAmericaStakeQuote(pWebData, m_vAmericaStake.at(m_lCurrentRTDataQuotePos));
      break;
      case __STAKE_CANDLES__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentUpdateDayLinePos);
      ProcessAmericaStakeCandle(pWebData, m_vAmericaStake.at(m_lCurrentUpdateDayLinePos));
      TRACE("处理%s日线数据\n", m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->m_strSymbol.GetBuffer());
      break;
      case __FOREX_EXCHANGE__:
      ProcessAmericaForexExchange(pWebData, vExchange);
      for (int i = 0; i < vExchange.size(); i++) {
        if (m_mapForexExchange.find(vExchange.at(i)) == m_mapForexExchange.end()) {
          lTemp = m_vForexExchange.size();
          m_vForexExchange.push_back(vExchange.at(i));
          m_mapForexExchange[vExchange.at(i)] = lTemp;
        }
      }
      m_lTotalForexExchange = m_vForexExchange.size();
      m_fForexExhangeUpdated = true;
      break;
      case __FOREX_SYMBOLS__:
      ProcessAmericaForexSymbol(pWebData, vForexSymbol);
      for (auto& pSymbol : vForexSymbol) {
        if (m_mapForexSymbol.find(pSymbol->m_strSymbol) == m_mapForexSymbol.end()) {
          pSymbol->m_strExchange = m_vForexExchange.at(m_CurrentFinnhubInquiry.m_lStakeIndex);
          lTemp = m_mapForexSymbol.size();
          m_mapForexSymbol[pSymbol->m_strSymbol] = lTemp;
          m_vForexSymbol.push_back(pSymbol);
        }
      }
      m_lTotalForexSymbol = m_vForexSymbol.size();
      break;
      case __FOREX_CANDLES__:
      break;
      case __FOREX_ALL_RATES__:
      break;
      default:
      break;
      }
      m_fFinnhubInquiring = false;
    }
  }

  return true;
}

/// <summary>
/// //////////////////////////////////////////////////////////////////////////////////////////
/// </summary>
/// finnhub读取函数采用申请和接收轮换执行方式，故而至少调用两次才完成一个轮回。
/// <param name=""></param>
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool CAmericaStakeMarket::ProcessFinnhubInquiringMessage(void) {
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  CAmericaStakePtr pStake = nullptr;

  if (m_qWebInquiry.size() > 0) { // 有申请等待？
    ASSERT(m_fFinnhubInquiring);
    if (IsFinnhubDataReceived()) { //已经发出了数据申请且Finnhub数据已经接收到了？
      m_CurrentFinnhubInquiry = m_qWebInquiry.top();
      m_qWebInquiry.pop();
      gl_pFinnhubWebInquiry->SetInquiryingStrPrefix(m_vFinnhubInquiringStr.at(m_CurrentFinnhubInquiry.m_lInquiryIndex)); // 设置前缀
      switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) { // 根据不同的要求设置中缀字符串
      case __COMPANY_PROFILE__: // 免费账户无法读取此信息，sandbox模式能读取，但是错误的，只能用于测试。
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_lCurrentProfilePos)->m_strSymbol);
      m_vAmericaStake.at(m_lCurrentProfilePos)->m_fInquiryAmericaStake = false;
      break;
      case __COMPANY_PROFILE2__:
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
      pStake = m_vAmericaStake.at(m_lCurrentRTDataQuotePos);
      strMiddle = pStake->GetSymbol();
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      break;
      case __STAKE_CANDLES__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentUpdateDayLinePos);
      pStake = m_vAmericaStake.at(m_lCurrentUpdateDayLinePos);
      strMiddle = pStake->GetDayLineInquiryString(GetMarketTime());
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      pStake->m_fDayLineNeedUpdate = false;
      break;
      case __FOREX_EXCHANGE__:
      // do nothing
      break;
      case __FOREX_SYMBOLS__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == (m_lCurrentForexExchangePos - 1));
      strMiddle = m_vForexExchange.at(m_CurrentFinnhubInquiry.m_lStakeIndex);
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      break;
      case __FOREX_CANDLES__:
      break;
      case __FOREX_ALL_RATES__:
      break;
      default:
      TRACE("未处理指令%d\n", m_CurrentFinnhubInquiry.m_lInquiryIndex);
      break;
      }
      SetFinnhubDataReceived(false);
      gl_pFinnhubWebInquiry->GetWebData();
    }
  }

  return true;
}

bool CAmericaStakeMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
  SchedulingTaskPer1Hour(lSecond, lCurrentTime);
  SchedulingTaskPer1Minute(lSecond, lCurrentTime);
  SchedulingTaskPer10Seconds(lSecond, lCurrentTime);

  TaskSaveStakeSymbolDB();
  static int s_iCount = -1;

  return true;
}

bool CAmericaStakeMarket::SchedulingTaskPer10Seconds(long lSecond, long lCurrentTime) {
  static int i10SecondsCounter = 9;  // 十秒钟一次的计数器

  i10SecondsCounter -= lSecond;
  if (i10SecondsCounter < 0) {
    i10SecondsCounter = 9;

    return true;
  }
  else return false;
}

bool CAmericaStakeMarket::SchedulingTaskPer1Hour(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 3599;  // 一小时一次的计数器

  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 3599;

    return true;
  }
  else return false;
}

bool CAmericaStakeMarket::SchedulingTaskPer1Minute(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 59;  // 一分钟一次的计数器

  // 自动查询crweber.com
  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 59;
    TaskResetMarket(lCurrentTime);

    TaskUpdateForexExchangeDB();
    TaskUpdateForexSymbolDB();
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

/// <summary>
/// /////////////////////////////////////////////////////////////////////////////////
///
/// 此任务必须放置于每分钟执行一次的调度中。
/// <returns></returns>
///
bool CAmericaStakeMarket::TaskResetMarket(long lCurrentTime) {
  // 市场时间零点重启系统
  if (IsSystemReady() && IsPermitResetMarket()) { // 如果允许重置系统
    if (lCurrentTime <= 100) { // 本市场时间的零时重启本市场 // 东八区本地时间为中午十二时（或夏令时的四时）。
      SetResetMarket(true);// 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
      SetPermitResetMarket(false); // 今天不再允许重启系统。
    }
  }
  return true;
}

bool CAmericaStakeMarket::TaskInquiryTodaySymbol(void) {
  FinnhubInquiry inquiry{ 0, 0, 0 };
  if (!m_fSymbolUpdated && !m_fFinnhubInquiring) {
    inquiry.m_lInquiryIndex = __COMPANY_SYMBOLS__;
    inquiry.m_iPriority = 10;
    m_qWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
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
      pStake->Append(setAmericaStake);
    }
    setAmericaStake.m_pDatabase->CommitTrans();
    setAmericaStake.Close();
    m_lLastTotalAmericaStake = m_lTotalAmericaStake;
  }
  return true;
}

bool CAmericaStakeMarket::TaskUpdateForexSymbolDB(void) {
  CSetForexSymbol setForexSymbol;
  CForexSymbolPtr pSymbol = nullptr;

  if (m_lLastTotalForexSymbol < m_lTotalForexSymbol) {
    setForexSymbol.Open();
    setForexSymbol.m_pDatabase->BeginTrans();
    for (long l = m_lLastTotalForexSymbol; l < m_lTotalForexSymbol; l++) {
      pSymbol = m_vForexSymbol.at(l);
      pSymbol->Append(setForexSymbol);
    }
    setForexSymbol.m_pDatabase->CommitTrans();
    setForexSymbol.Close();
    m_lLastTotalForexSymbol = m_lTotalForexSymbol;
  }
  return true;
}

bool CAmericaStakeMarket::TaskUpdateForexExchangeDB(void) {
  CSetForexExchange setForexExchange;

  if (m_lLastTotalForexExchange < m_lTotalForexExchange) {
    setForexExchange.Open();
    setForexExchange.m_pDatabase->BeginTrans();
    for (long l = m_lLastTotalForexExchange; l < m_lTotalForexExchange; l++) {
      setForexExchange.AddNew();
      setForexExchange.m_Exchange = m_vForexExchange.at(l);
      setForexExchange.Update();
    }
    setForexExchange.m_pDatabase->CommitTrans();
    setForexExchange.Close();
    m_lLastTotalForexExchange = m_lTotalForexExchange;
  }
  return true;
}

bool CAmericaStakeMarket::TaskUpdateStakeDB(void) {
  RunningTaskThreadUpdateStakeDB();
  return true;
}

bool CAmericaStakeMarket::TaskInquiryAmericaStake(void) {
  bool fFound = false;
  FinnhubInquiry inquiry{ 0, 0, 0 };
  long lStakeSetSize = m_vAmericaStake.size();
  CString str = _T("");

  ASSERT(IsSystemReady());
  if (!m_fAmericaStakeUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentProfilePos = 0; m_lCurrentProfilePos < lStakeSetSize; m_lCurrentProfilePos++) {
      if (IsEarlyThen(m_vAmericaStake.at(m_lCurrentProfilePos)->m_lProfileUpdateDate, GetFormatedMarketDate(), 365)) {
        fFound = true;
        break;
      }
    }
    if (fFound) {
      inquiry.m_lInquiryIndex = __COMPANY_PROFILE2__;
      inquiry.m_lStakeIndex = m_lCurrentProfilePos;
      inquiry.m_iPriority = 10;
      m_qWebInquiry.push(inquiry);
      TRACE("更新%s简介\n", m_vAmericaStake.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
      m_fFinnhubInquiring = true;
    }
    else {
      m_fAmericaStakeUpdated = true;
      TRACE("Finnhub股票简介更新完毕\n");
      str = _T("Finnhub股票简介更新完毕");
      gl_systemMessage.PushInformationMessage(str);
    }
  }
  return false;
}

bool CAmericaStakeMarket::TaskInquiryDayLine(void) {
  bool fFound = false;
  FinnhubInquiry inquiry{ 0, 0, 0 };
  CAmericaStakePtr pStake;
  CString str = _T("");
  long lStakeSetSize = m_vAmericaStake.size();

  ASSERT(IsSystemReady());
  if (!m_fStakeDayLineUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < lStakeSetSize; m_lCurrentUpdateDayLinePos++) {
      if (m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
        pStake = m_vAmericaStake.at(m_lCurrentUpdateDayLinePos);
        fFound = true;
        break;
      }
    }
    if (fFound) {
      inquiry.m_lInquiryIndex = __STAKE_CANDLES__;
      inquiry.m_lStakeIndex = m_lCurrentUpdateDayLinePos;
      inquiry.m_iPriority = 10;
      m_qWebInquiry.push(inquiry);
      m_fFinnhubInquiring = true;
      m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->SetDayLineNeedUpdate(false);
      TRACE("申请%s日线数据\n", m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->m_strSymbol.GetBuffer());
    }
    else {
      m_fStakeDayLineUpdated = true;
      TRACE("Finnhub日线更新完毕\n");
      str = _T("美国市场日线历史数据更新完毕");
      gl_systemMessage.PushInformationMessage(str);
    }
  }
  return true;
}

bool CAmericaStakeMarket::TaskInquiryFinnhubRTQuote(void) {
  FinnhubInquiry inquiry{ 0, 0, 0 };

  ASSERT(IsSystemReady());
  if (!m_fFinnhubInquiring) {
    m_lCurrentRTDataQuotePos++;
    if (m_lCurrentRTDataQuotePos == m_vAmericaStake.size()) m_lCurrentRTDataQuotePos = 0;
    while (!m_vAmericaStake.at(m_lCurrentRTDataQuotePos)->IsIPOed()) {
      m_lCurrentRTDataQuotePos++;
      if (m_lCurrentRTDataQuotePos == m_vAmericaStake.size()) m_lCurrentRTDataQuotePos = 0;
    }
    inquiry.m_lInquiryIndex = __STAKE_QUOTE__;
    inquiry.m_lStakeIndex = m_lCurrentRTDataQuotePos;
    inquiry.m_iPriority = 10;
    m_qWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    TRACE("申请%s实时数据\n", m_vAmericaStake.at(m_lCurrentRTDataQuotePos)->m_strSymbol.GetBuffer());
  }
  return true;
}

bool CAmericaStakeMarket::TaskInquiryFinnhubForexExchange(void) {
  FinnhubInquiry inquiry{ 0, 0, 0 };
  if (!m_fForexExhangeUpdated && !m_fFinnhubInquiring) {
    inquiry.m_lInquiryIndex = __FOREX_EXCHANGE__;
    inquiry.m_iPriority = 10;
    m_qWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    return true;
  }
  return false;
}

bool CAmericaStakeMarket::TaskInquiryFinnhubForexSymbol(void) {
  FinnhubInquiry inquiry{ 0, 0, 0 };
  if (!m_fForexSymbolUpdated && !m_fFinnhubInquiring) {
    inquiry.m_lInquiryIndex = __FOREX_SYMBOLS__;
    inquiry.m_lStakeIndex = m_lCurrentForexExchangePos++;
    inquiry.m_iPriority = 10;
    m_qWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    if (m_lCurrentForexExchangePos >= m_vForexExchange.size()) m_fForexSymbolUpdated = true;
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线数据存入数据库．
//  此函数由工作线程ThreadDayLineSaveProc调用，尽量不要使用全局变量。(目前使用主线程调用之，以消除同步问题的出现）
//
// 无论是否执行了存储函数，都需要将下载的日线历史数据删除，这样能够节省内存的占用。由于实际存储功能使用线程模式实现，
// 故而其执行时间可能晚于主线程，导致主线程删除日线数据时出现同步问题。解决的方法是让工作线程独立删除存储后的日线数据，
// 主线程的删除函数只在不调用工作线程（无需存储日线数据）的情况下方才执行。
//////////////////////////////////////////////////////////////////////////////////////////
bool CAmericaStakeMarket::TaskUpdateDayLineDB(void) {
  CString str;

  for (auto& pStake : m_vAmericaStake) {
    if (pStake->IsDayLineNeedSavingAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
      if (pStake->GetDayLineSize() > 0) {
        if (pStake->HaveNewDayLineData()) {
          RunningThreadUpdateDayLineDB(pStake);
          TRACE("更新%s日线数据\n", pStake->GetSymbol().GetBuffer());
        }
        else pStake->UnloadDayLine(); // 当无需执行存储函数时，这里还要单独卸载日线数据。因存储日线数据线程稍后才执行，故而不能在此统一执行删除函数。
      }
      else { // 此种情况为有股票代码，但此代码尚未上市
        CString str1 = pStake->GetSymbol();
        str1 += _T(" 为未上市股票代码");
        gl_systemMessage.PushDayLineInfoMessage(str1);
      }
    }
    if (gl_fExitingSystem) {
      break; // 如果程序正在退出，则停止存储。
    }
  }

  return(true);
}

bool CAmericaStakeMarket::TaskCheckSystemReady(void) {
  CString str = _T("");

  if (!IsSystemReady()) {
    if (m_fSymbolUpdated && m_fForexExhangeUpdated && m_fForexSymbolUpdated) {
      str = _T("美国市场初始化完毕");
      gl_systemMessage.PushInformationMessage(str);
      SetSystemReady(true);
      return true;
    }
    return false;
  }
  return true;
}

bool CAmericaStakeMarket::RunningThreadUpdateDayLineDB(CAmericaStakePtr pStake) {
  thread thread1(ThreadUpdateAmericaStakeDayLineDB, pStake);
  thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
  return true;
}

bool CAmericaStakeMarket::RunningTaskThreadUpdateStakeDB(void) {
  thread thread1(ThreadUpdateStakeDB, this);
  thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
  return true;
}

bool CAmericaStakeMarket::IsAmericaStake(CString strSymbol) {
  if (m_mapAmericaStake.find(strSymbol) == m_mapAmericaStake.end()) { // 新代码？
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

long CAmericaStakeMarket::GetFinnInquiry(void) {
  FinnhubInquiry inquiry{ 0, 0, 0 };
  if (m_qWebInquiry.size() > 0) {
    inquiry = m_qWebInquiry.top();
    m_qWebInquiry.pop();
    return inquiry.m_lInquiryIndex;
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
    pAmericaStake->Append(setAmericaStake);
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

bool CAmericaStakeMarket::RebulidFinnhubDayLine(void) {
  CSetAmericaStake setAmericaStake;
  for (auto& pStake : m_vAmericaStake) {
    pStake->SetDayLineStartDate(29900101);
    pStake->SetDayLineEndDate(19800101);
  }

  //setAmericaStake.m_strSort = _T("[Symbol]");
  setAmericaStake.Open();
  setAmericaStake.m_pDatabase->BeginTrans();
  while (!setAmericaStake.IsEOF()) {
    setAmericaStake.Edit();
    setAmericaStake.m_DayLineStartDate = 29900101;
    setAmericaStake.m_DayLineEndDate = 19800101;
    setAmericaStake.Update();
  }
  setAmericaStake.m_pDatabase->CommitTrans();
  setAmericaStake.Close();

  m_fStakeDayLineUpdated = false;

  return true;
}

bool CAmericaStakeMarket::SortStakeTable(void) {
  CSetAmericaStake setAmericaStake;
  vector<CAmericaStakePtr> vStake;
  CAmericaStakePtr pStake = nullptr;

  setAmericaStake.m_strSort = _T("[Symbol]");
  setAmericaStake.Open();
  setAmericaStake.m_pDatabase->BeginTrans();
  while (!setAmericaStake.IsEOF()) {
    pStake = make_shared<CAmericaStake>();
    pStake->Load(setAmericaStake);
    vStake.push_back(pStake);
    setAmericaStake.MoveNext();
  }
  setAmericaStake.m_pDatabase->CommitTrans();
  setAmericaStake.Close();

  setAmericaStake.Open();
  setAmericaStake.m_pDatabase->BeginTrans();
  while (!setAmericaStake.IsEOF()) {
    setAmericaStake.Delete();
    setAmericaStake.MoveNext();
  }
  setAmericaStake.m_pDatabase->CommitTrans();
  setAmericaStake.Close();

  setAmericaStake.Open();
  setAmericaStake.m_pDatabase->BeginTrans();
  for (auto& pStake1 : vStake) {
    pStake1->Append(setAmericaStake);
    setAmericaStake.AddNew();

    setAmericaStake.MoveNext();
  }
  setAmericaStake.m_pDatabase->CommitTrans();
  setAmericaStake.Close();

  return true;
}

bool CAmericaStakeMarket::LoadForexExchange(void) {
  CSetForexExchange setForexExchange;
  int i = 0;

  setForexExchange.Open();
  while (!setForexExchange.IsEOF()) {
    m_vForexExchange.push_back(setForexExchange.m_Exchange);
    m_mapForexExchange[setForexExchange.m_Exchange] = i++;
    setForexExchange.MoveNext();
  }
  setForexExchange.Close();
  m_lLastTotalForexExchange = m_lTotalForexExchange = m_vForexExchange.size();

  return true;
}

bool CAmericaStakeMarket::LoadForexSymbol(void) {
  CSetForexSymbol setForexSymbol;
  CForexSymbolPtr pSymbol = nullptr;
  int i = 0;

  setForexSymbol.Open();
  while (!setForexSymbol.IsEOF()) {
    pSymbol = make_shared<CForexSymbol>();
    pSymbol->Load(setForexSymbol);
    m_vForexSymbol.push_back(pSymbol);
    m_mapForexSymbol[pSymbol->m_strSymbol] = i++;
    setForexSymbol.MoveNext();
  }
  setForexSymbol.Close();
  m_lLastTotalForexSymbol = m_lTotalForexSymbol = m_vForexSymbol.size();

  return true;
}