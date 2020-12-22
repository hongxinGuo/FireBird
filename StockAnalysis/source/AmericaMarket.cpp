#include "AmericaMarket.h"
#include"thread.h"

#include"WebInquirer.h"
#include"ProcessAmericaStake.h"
#include"EPSSurprise.h"

#include"SetForexExchange.h"
#include"SetAmericaStake.h"
#include"SetCountry.h"
#include"SetEconomicCalendar.h"
#include"SetEPSSurprise.h"

Semaphore gl_SaveAmericaOneStockDayLine(1);  // 此信号量用于生成美国股票日线历史数据库
Semaphore gl_SaveEPSSurprise(1);  // 此信号量用于更新EPSSurprise数据
Semaphore gl_SaveForexDayLine(1);  // 此信号量用于生成Forex日线历史数据库

CAmericaMarket::CAmericaMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CAmericaMarket市场变量只允许存在一个实例\n");
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

  m_vFinnhubInquiringStr.at(__STOCK_EPS_SURPRISE__) = _T("https://finnhub.io/api/v1/stock/earnings?symbol=");
  m_vFinnhubInquiringStr.at(__STOCK_EARNING_CALENDER__) = _T("https://finnhub.io/api/v1/calendar/earnings?");

  m_vFinnhubInquiringStr.at(__STOCK_QUOTE__) = _T("https://finnhub.io/api/v1/quote?symbol="); // 某个代码的交易
  m_vFinnhubInquiringStr.at(__STOCK_CANDLES__) = _T("https://finnhub.io/api/v1/stock/candle?symbol="); // 历史蜡烛图

  m_vFinnhubInquiringStr.at(__FOREX_EXCHANGE__) = _T("https://finnhub.io/api/v1/forex/exchange?");
  m_vFinnhubInquiringStr.at(__FOREX_SYMBOLS__) = _T("https://finnhub.io/api/v1/forex/symbol?exchange=");
  m_vFinnhubInquiringStr.at(__FOREX_CANDLES__) = _T("https://finnhub.io/api/v1/forex/candle?symbol=");
  m_vFinnhubInquiringStr.at(__FOREX_ALL_RATES__) = _T("https://finnhub.io/api/v1/forex/rates?base=USD");

  m_vFinnhubInquiringStr.at(__ECONOMIC_COUNTRY_LIST__) = _T("https://finnhub.io/api/v1/country?");
  m_vFinnhubInquiringStr.at(__ECONOMIC_CALENDAR__) = _T("https://finnhub.io/api/v1/calendar/economic?");

  Reset();
}

CAmericaMarket::~CAmericaMarket() {
}

void CAmericaMarket::Reset(void) {
  m_lTotalAmericaStake = 0;
  m_lLastTotalAmericaStake = 0;
  m_lCurrentProfilePos = 0;
  m_lCurrentUpdateDayLinePos = 0;
  m_lCurrentRTDataQuotePos = 0;
  m_lCurrentForexExchangePos = 0;
  m_lCurrentForexSymbolPos = 0;
  m_lCurrentUpdatePeerPos = 0;
  m_lCurrentUpdateEPSSurprisePos = 0;

  m_CurrentFinnhubInquiry.Reset();
  while (m_qFinnhubWebInquiry.size() > 0) m_qFinnhubWebInquiry.pop();

  // Finnhub各申请网络数据标识，每日需要重置。
  m_fSymbolUpdated = false; // 每日需要更新代码
  m_fAmericaStakeUpdated = false;
  m_fStakeDayLineUpdated = false;
  m_vAmericaStake.resize(0);
  m_mapAmericaStake.clear();
  m_fForexExhangeUpdated = false;
  m_vForexExchange.resize(0);
  m_mapForexExchange.clear();
  m_fForexSymbolUpdated = false;
  m_vForexSymbol.resize(0);
  m_mapForexSymbol.clear();
  m_fForexDayLineUpdated = false;
  m_vCountry.resize(0);
  m_mapCountry.clear();
  m_fCountryListUpdated = false;
  m_vEconomicCalendar.resize(0);
  m_mapEconomicCalendar.clear();
  m_fEconomicCalendarUpdated = false;
  m_fEPSSurpriseUpdated = false;

  m_fPeerUpdated = false;

  m_fFinnhubInquiring = false;
  m_fFinnhubDataReceived = true;

  m_fRebulidDayLine = false;
  SetSystemReady(false); // 市场初始状态为未设置好。

  m_lLastTotalForexExchange = 0;
  m_lTotalForexExchange = 0;
  m_lLastTotalForexSymbol = 0;
  m_lTotalForexSymbol = 0;
  m_lCurrentUpdateForexDayLinePos = 0;

  m_lLastTotalCountry = 0;
  m_lTotalCountry = 0;
  m_lTotalEconomicCalendar = 0;
  m_lLastTotalEconomicCalendar = 0;
}

void CAmericaMarket::ResetMarket(void) {
  Reset();

  LoadCountryList();
  LoadAmericaStake();
  LoadForexExchange();
  LoadForexSymbol();
  LoadEconomicCalendarDB();

  CString str = _T("重置America Stake Market于美东标准时间：");
  str += GetStringOfMarketTime();
  gl_systemMessage.PushInformationMessage(str);
}

bool CAmericaMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  static int s_iCountfinnhubLimit = 11; // 每1.2秒左右执行一次，以防止出现频率过高的情况。
  const long lCurrentTime = GetFormatedMarketTime();

  TaskCheckSystemReady();

  TaskInquiryFinnhub(lCurrentTime);

  ProcessFinnhubWebDataReceived(); // 要先处理收到的Finnhub网络数据
  ProcessFinnhubInquiringMessage(); // 然后再申请处理下一个

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
bool CAmericaMarket::ProcessFinnhubWebDataReceived(void) {
  CWebDataPtr pWebData = nullptr;
  CAmericaStakePtr pStake = nullptr;
  CString str = _T("");
  vector<CString> vExchange;
  vector<CForexSymbolPtr> vForexSymbol;
  vector<CEconomicCalendarPtr> vEconomicCalendar;
  vector<CEPSSurprisePtr> vEPSSurprise;
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
      pStake = m_vAmericaStake.at(m_lCurrentProfilePos);
      ProcessAmericaStakeProfile2(pWebData, pStake);
      pStake->m_lProfileUpdateDate = gl_pAmericaMarket->GetFormatedMarketDate();
      pStake->m_fUpdateDatabase = true;
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
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentUpdatePeerPos);
      pStake = m_vAmericaStake.at(m_lCurrentUpdatePeerPos);
      ProcessPeer(pWebData, pStake);
      pStake->m_fUpdateDatabase = true;
      break;
      case __BASIC_FINANCIALS__:
      break;
      case __STOCK_EPS_SURPRISE__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentUpdateEPSSurprisePos);
      pStake = m_vAmericaStake.at(m_lCurrentUpdateEPSSurprisePos);
      ProcessEPSSurprise(pWebData, vEPSSurprise);
      pStake->UpdateEPSSurprise(vEPSSurprise);
      pStake->m_fEPSSurpriseNeedUpdate = false;
      pStake->m_fEPSSurpriseNeedSave = true;
      break;
      case __STOCK_QUOTE__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentRTDataQuotePos);
      ProcessAmericaStakeQuote(pWebData, m_vAmericaStake.at(m_lCurrentRTDataQuotePos));
      break;
      case __STOCK_CANDLES__:
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
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentUpdateForexDayLinePos);
      ProcessForexCandle(pWebData, m_vForexSymbol.at(m_lCurrentUpdateForexDayLinePos));
      TRACE("处理%s日线数据\n", m_vForexSymbol.at(m_lCurrentUpdateForexDayLinePos)->m_strSymbol.GetBuffer());
      break;
      case __FOREX_ALL_RATES__:
      break;
      case __ECONOMIC_COUNTRY_LIST__:
      ProcessCountryList(pWebData, m_vCountry);
      m_lTotalCountry = m_vCountry.size();
      m_fCountryListUpdated = true;
      break;
      case __ECONOMIC_CALENDAR__:
      ProcessEconomicCalendar(pWebData, vEconomicCalendar);
      UpdateEconomicCalendar(vEconomicCalendar);
      m_fEconomicCalendarUpdated = true;
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
bool CAmericaMarket::ProcessFinnhubInquiringMessage(void) {
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  CAmericaStakePtr pStake = nullptr;
  CForexSymbolPtr pSymbol = nullptr;

  if (m_qFinnhubWebInquiry.size() > 0) { // 有申请等待？
    ASSERT(m_fFinnhubInquiring);
    if (IsFinnhubDataReceived()) { //已经发出了数据申请且Finnhub数据已经接收到了？
      m_CurrentFinnhubInquiry = m_qFinnhubWebInquiry.top();
      m_qFinnhubWebInquiry.pop();
      gl_pFinnhubWebInquiry->SetInquiryingStrPrefix(m_vFinnhubInquiringStr.at(m_CurrentFinnhubInquiry.m_lInquiryIndex)); // 设置前缀
      switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) { // 根据不同的要求设置中缀字符串
      case __COMPANY_PROFILE__: // Premium 免费账户无法读取此信息，sandbox模式能读取，但是错误的，只能用于测试。
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentUpdateDayLinePos);
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_lCurrentProfilePos)->m_strSymbol);
      m_vAmericaStake.at(m_lCurrentProfilePos)->m_fInquiryAmericaStake = false;
      break;
      case __COMPANY_PROFILE2__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentProfilePos);
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_lCurrentProfilePos)->m_strSymbol);
      m_vAmericaStake.at(m_lCurrentProfilePos)->m_fInquiryAmericaStake = false;
      break;
      case  __COMPANY_SYMBOLS__:
      // do nothing
      break;
      case __COMPANY_EXECTIVE__: // Premium
      break;
      case __MARKET_NEWS__:
      break;
      case __COMPANY_NEWS__:
      break;
      case __PRESS_RELEASE__: // Premium
      break;
      case __NEWS_SENTIMENT__:
      break;
      case __PEERS__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentUpdatePeerPos);
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_lCurrentUpdatePeerPos)->m_strSymbol);
      m_vAmericaStake.at(m_lCurrentUpdatePeerPos)->m_fPeerUpdated = true;
      break;
      case __BASIC_FINANCIALS__:
      break;
      case __OWNERSHIP__: // Premium
      break;
      case __FUND_OWNERSHIP__: // jPremium
      break;
      case __FINANCIAL__: // Premium
      break;
      case __FINAICIAL_AS_REPORT__:
      break;
      case __SEC_FILINGS__:
      break;
      case __INTERNATIONAL_FILINGS__: // Premium
      break;
      case __SEC_SENTIMENT_ANALYSIS__: // Premium
      break;
      case __SIMILARITY_INDEX__: // Premium
      break;
      case __IPO_CALENDER__:
      break;
      case __DIVIDENDS__: // Premium
      break;
      case __STOCK_RECOMMENDATION_TRENDS__:
      break;
      case __STOCK_PRICE_TARGET__:
      break;
      case __STOCK_UPGRADE_DOWNGRADE__: // Premium
      break;
      case __STOCK_REVENUE_EXTIMATES__: // Premium
      break;
      case __STOCK_EPS_EXTIMATES__:// Premium
      break;
      case __STOCK_EPS_SURPRISE__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentUpdateEPSSurprisePos);
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_lCurrentUpdateEPSSurprisePos)->m_strSymbol);
      break;
      case __STOCK_EARNING_CALENDER__:
      break;
      case __STOCK_QUOTE__:
      pStake = m_vAmericaStake.at(m_lCurrentRTDataQuotePos);
      strMiddle = pStake->GetSymbol();
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      break;
      case __STOCK_CANDLES__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentUpdateDayLinePos);
      pStake = m_vAmericaStake.at(m_lCurrentUpdateDayLinePos);
      strMiddle = pStake->GetDayLineInquiryString(GetMarketTime());
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      pStake->m_fDayLineNeedUpdate = false;
      break;
      case __STOCK_TICK_DATA__: // Premium
      break;
      case __STOCK_LAST_BID_ASK__: // Premium
      break;
      case __STOCK_SPLITS__:
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
      ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentUpdateForexDayLinePos);
      pSymbol = m_vForexSymbol.at(m_lCurrentUpdateForexDayLinePos);
      strMiddle = pSymbol->GetDayLineInquiryString(GetMarketTime());
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      pSymbol->m_fDayLineNeedUpdate = false;
      break;
      case __FOREX_ALL_RATES__:
      break;
      case __ECONOMIC_COUNTRY_LIST__:
      // do nothing
      break;
      case __ECONOMIC_CALENDAR__:
      // do nothing
      break;
      default:
      TRACE("未处理指令%d\n", m_CurrentFinnhubInquiry.m_lInquiryIndex);
      break;
      }
      SetFinnhubDataReceived(false); // 重置此标识需要放在启动工作线程（GetWebData）之前，否则工作线程中的断言容易出错。
      gl_pFinnhubWebInquiry->GetWebData();
    }
  }

  return true;
}

bool CAmericaMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
  SchedulingTaskPer1Hour(lSecond, lCurrentTime);
  SchedulingTaskPer1Minute(lSecond, lCurrentTime);
  SchedulingTaskPer10Seconds(lSecond, lCurrentTime);

  TaskSaveStakeSymbolDB();
  static int s_iCount = -1;

  return true;
}

bool CAmericaMarket::SchedulingTaskPer10Seconds(long lSecond, long lCurrentTime) {
  static int i10SecondsCounter = 9;  // 十秒钟一次的计数器

  i10SecondsCounter -= lSecond;
  if (i10SecondsCounter < 0) {
    i10SecondsCounter = 9;

    return true;
  }
  else return false;
}

bool CAmericaMarket::SchedulingTaskPer1Hour(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 3599;  // 一小时一次的计数器

  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 3599;

    return true;
  }
  else return false;
}

bool CAmericaMarket::SchedulingTaskPer1Minute(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 59;  // 一分钟一次的计数器

  // 自动查询crweber.com
  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 59;
    TaskResetMarket(lCurrentTime);

    TaskUpdateCountryListDB();
    TaskUpdateForexExchangeDB();
    TaskUpdateForexSymbolDB();
    TaskUpdateForexDayLineDB();
    TaskUpdateDayLineDB();
    TaskUpdateEPSSurpriseDB();
    TaskUpdateEconomicCalendar();
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
bool CAmericaMarket::TaskResetMarket(long lCurrentTime) {
  // 市场时间十七时重启系统
  if (IsSystemReady() && IsPermitResetMarket()) { // 如果允许重置系统
    if ((lCurrentTime > 170000) && (lCurrentTime <= 170100)) { // 本市场时间的下午五时(北京时间上午五时重启本市场。这样有利于接收日线数据。
      SetResetMarket(true);// 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
      SetPermitResetMarket(false); // 今天不再允许重启系统。
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// 此函数由SchedulingTask调度，每100毫秒左右执行一次。
//
//////////////////////////////////////////////////////////////////////////////////////
bool CAmericaMarket::TaskInquiryFinnhub(long lCurrentTime) {
  static int s_iCountfinnhubLimit = 11; // 保证每12次执行一次（即1.2秒每次）
  if (((lCurrentTime < 165700) || (lCurrentTime > 170300))) { // 下午五时重启系统，故而此时不允许接收网络信息。
    TaskInquiryFinnhubCountryList();
    TaskInquiryFinnhubCompanySymbol(); // 第一个动作，首先申请当日证券代码
    TaskInquiryFinnhubForexExchange();
    TaskInquiryFinnhubForexSymbol();
    //TaskInquiryFinnhubEconomicCalender();

    // 申请Finnhub网络信息的任务，皆要放置在这里，以保证在市场时间凌晨十分钟后执行。这样能够保证在重启市场时没有执行查询任务
    if (IsSystemReady() && !m_fFinnhubInquiring && (s_iCountfinnhubLimit < 0)) {
      s_iCountfinnhubLimit = 11;
      TaskInquiryFinnhubCompanyProfile2();
      TaskInquiryFinnhubEPSSurprise();
      TaskInquiryFinnhubPeer();
      TaskInquiryFinnhubForexDayLine();
      TaskInquiryFinnhubDayLine();
      if (m_fStakeDayLineUpdated) {
        //TaskInquiryFinnhubRTQuote();
      }
    }
    else s_iCountfinnhubLimit--;
  }

  return true;
}

bool CAmericaMarket::TaskInquiryFinnhubCountryList(void) {
  WebInquiry inquiry{ 0, 0, 0 };
  if (!m_fCountryListUpdated && !m_fFinnhubInquiring) {
    inquiry.m_lInquiryIndex = __ECONOMIC_COUNTRY_LIST__;
    inquiry.m_iPriority = 10;
    m_qFinnhubWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    return true;
  }
  return false;
}

bool CAmericaMarket::TaskInquiryFinnhubCompanySymbol(void) {
  WebInquiry inquiry{ 0, 0, 0 };
  if (!m_fSymbolUpdated && !m_fFinnhubInquiring) {
    inquiry.m_lInquiryIndex = __COMPANY_SYMBOLS__;
    inquiry.m_iPriority = 10;
    m_qFinnhubWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    return true;
  }
  return false;
}

bool CAmericaMarket::TaskSaveStakeSymbolDB(void) {
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

bool CAmericaMarket::TaskUpdateForexSymbolDB(void) {
  RunningThreadUpdateForexSymbolDB();
  return false;
}

bool CAmericaMarket::TaskUpdateForexExchangeDB(void) {
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

bool CAmericaMarket::TaskUpdateStakeDB(void) {
  RunningTaskThreadUpdateStakeDB();
  return true;
}

bool CAmericaMarket::TaskInquiryFinnhubCompanyProfile2(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
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
      m_qFinnhubWebInquiry.push(inquiry);
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

bool CAmericaMarket::TaskInquiryFinnhubDayLine(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
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
      inquiry.m_lInquiryIndex = __STOCK_CANDLES__;
      inquiry.m_lStakeIndex = m_lCurrentUpdateDayLinePos;
      inquiry.m_iPriority = 10;
      m_qFinnhubWebInquiry.push(inquiry);
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

bool CAmericaMarket::TaskInquiryFinnhubRTQuote(void) {
  WebInquiry inquiry{ 0, 0, 0 };

  ASSERT(IsSystemReady());
  if (!m_fFinnhubInquiring) {
    m_lCurrentRTDataQuotePos++;
    if (m_lCurrentRTDataQuotePos == m_vAmericaStake.size()) m_lCurrentRTDataQuotePos = 0;
    while (!m_vAmericaStake.at(m_lCurrentRTDataQuotePos)->IsIPOed()) {
      m_lCurrentRTDataQuotePos++;
      if (m_lCurrentRTDataQuotePos == m_vAmericaStake.size()) m_lCurrentRTDataQuotePos = 0;
    }
    inquiry.m_lInquiryIndex = __STOCK_QUOTE__;
    inquiry.m_lStakeIndex = m_lCurrentRTDataQuotePos;
    inquiry.m_iPriority = 10;
    m_qFinnhubWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    TRACE("申请%s实时数据\n", m_vAmericaStake.at(m_lCurrentRTDataQuotePos)->m_strSymbol.GetBuffer());
  }
  return true;
}

bool CAmericaMarket::TaskInquiryFinnhubPeer(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
  CAmericaStakePtr pStake;
  CString str = _T("");
  long lStakeSetSize = m_vAmericaStake.size();

  ASSERT(IsSystemReady());
  if (!m_fPeerUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentUpdatePeerPos = 0; m_lCurrentUpdatePeerPos < lStakeSetSize; m_lCurrentUpdatePeerPos++) {
      if (!m_vAmericaStake.at(m_lCurrentUpdatePeerPos)->m_fPeerUpdated) {
        pStake = m_vAmericaStake.at(m_lCurrentUpdatePeerPos);
        fFound = true;
        break;
      }
    }
    if (fFound) {
      inquiry.m_lInquiryIndex = __PEERS__;
      inquiry.m_lStakeIndex = m_lCurrentUpdatePeerPos;
      inquiry.m_iPriority = 10;
      m_qFinnhubWebInquiry.push(inquiry);
      m_fFinnhubInquiring = true;
      m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->SetDayLineNeedUpdate(false);
      TRACE("申请%s Peer数据\n", m_vAmericaStake.at(m_lCurrentUpdatePeerPos)->m_strSymbol.GetBuffer());
    }
    else {
      m_fPeerUpdated = true;
      TRACE("Finnhub Peers更新完毕\n");
      str = _T("美国市场同业公司数据更新完毕");
      gl_systemMessage.PushInformationMessage(str);
    }
  }
  return true;
}

bool CAmericaMarket::TaskInquiryFinnhubEconomicCalender(void) {
  WebInquiry inquiry{ 0, 0, 0 };
  if (!m_fEconomicCalendarUpdated && !m_fFinnhubInquiring) {
    inquiry.m_lInquiryIndex = __ECONOMIC_CALENDAR__;
    inquiry.m_iPriority = 10;
    m_qFinnhubWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    return true;
  }
  return false;
}

bool CAmericaMarket::TaskInquiryFinnhubEPSSurprise(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
  CAmericaStakePtr pStake;
  CString str = _T("");
  long lStakeSetSize = m_vAmericaStake.size();

  ASSERT(IsSystemReady());
  if (!m_fEPSSurpriseUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentUpdateEPSSurprisePos = 0; m_lCurrentUpdateEPSSurprisePos < lStakeSetSize; m_lCurrentUpdateEPSSurprisePos++) {
      if (m_vAmericaStake.at(m_lCurrentUpdateEPSSurprisePos)->IsEPSSurpriseNeedUpdate()) {
        pStake = m_vAmericaStake.at(m_lCurrentUpdateEPSSurprisePos);
        fFound = true;
        break;
      }
    }
    if (fFound) {
      inquiry.m_lInquiryIndex = __STOCK_EPS_SURPRISE__;
      inquiry.m_lStakeIndex = m_lCurrentUpdateEPSSurprisePos;
      inquiry.m_iPriority = 10;
      m_qFinnhubWebInquiry.push(inquiry);
      m_fFinnhubInquiring = true;
      m_vAmericaStake.at(m_lCurrentUpdateEPSSurprisePos)->SetEPSSurpriseNeedUpdate(false);
      TRACE("申请%s EPS Surprise数据\n", m_vAmericaStake.at(m_lCurrentUpdateEPSSurprisePos)->m_strSymbol.GetBuffer());
    }
    else {
      m_fEPSSurpriseUpdated = true;
      TRACE("Finnhub EPS Surprise更新完毕\n");
      str = _T("Finnhub EPS Surprise更新完毕");
      gl_systemMessage.PushInformationMessage(str);
    }
  }
  return true;
}

bool CAmericaMarket::TaskInquiryFinnhubForexExchange(void) {
  WebInquiry inquiry{ 0, 0, 0 };
  if (!m_fForexExhangeUpdated && !m_fFinnhubInquiring) {
    inquiry.m_lInquiryIndex = __FOREX_EXCHANGE__;
    inquiry.m_iPriority = 10;
    m_qFinnhubWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    return true;
  }
  return false;
}

bool CAmericaMarket::TaskInquiryFinnhubForexSymbol(void) {
  WebInquiry inquiry{ 0, 0, 0 };
  if (!m_fForexSymbolUpdated && !m_fFinnhubInquiring) {
    inquiry.m_lInquiryIndex = __FOREX_SYMBOLS__;
    inquiry.m_lStakeIndex = m_lCurrentForexExchangePos++;
    inquiry.m_iPriority = 10;
    m_qFinnhubWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    if (m_lCurrentForexExchangePos >= m_vForexExchange.size()) m_fForexSymbolUpdated = true;
    return true;
  }
  return false;
}

bool CAmericaMarket::TaskInquiryFinnhubForexDayLine(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
  CForexSymbolPtr pStake;
  CString str = _T("");
  const long lStakeSetSize = m_vForexSymbol.size();

  ASSERT(IsSystemReady());
  if (!m_fForexDayLineUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentUpdateForexDayLinePos = 0; m_lCurrentUpdateForexDayLinePos < lStakeSetSize; m_lCurrentUpdateForexDayLinePos++) {
      if (m_vForexSymbol.at(m_lCurrentUpdateForexDayLinePos)->IsDayLineNeedUpdate()) {
        pStake = m_vForexSymbol.at(m_lCurrentUpdateDayLinePos);
        fFound = true;
        break;
      }
    }
    if (fFound) {
      inquiry.m_lInquiryIndex = __FOREX_CANDLES__;
      inquiry.m_lStakeIndex = m_lCurrentUpdateForexDayLinePos;
      inquiry.m_iPriority = 10;
      m_qFinnhubWebInquiry.push(inquiry);
      m_fFinnhubInquiring = true;
      m_vAmericaStake.at(m_lCurrentUpdateForexDayLinePos)->SetDayLineNeedUpdate(false);
      TRACE("申请Forex%s日线数据\n", m_vForexSymbol.at(m_lCurrentUpdateForexDayLinePos)->m_strSymbol.GetBuffer());
    }
    else {
      m_fForexDayLineUpdated = true;
      TRACE("Finnhub Forex日线更新完毕\n");
      str = _T("美国市场Forex日线历史数据更新完毕");
      gl_systemMessage.PushInformationMessage(str);
    }
  }
  return true;
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
bool CAmericaMarket::TaskUpdateDayLineDB(void) {
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

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将Forex日线数据存入数据库．
//  此函数由工作线程ThreadForexDayLineSaveProc调用，尽量不要使用全局变量。(目前使用主线程调用之，以消除同步问题的出现）
//
// 无论是否执行了存储函数，都需要将下载的日线历史数据删除，这样能够节省内存的占用。由于实际存储功能使用线程模式实现，
// 故而其执行时间可能晚于主线程，导致主线程删除日线数据时出现同步问题。解决的方法是让工作线程独立删除存储后的日线数据，
// 主线程的删除函数只在不调用工作线程（无需存储日线数据）的情况下方才执行。
//////////////////////////////////////////////////////////////////////////////////////////
bool CAmericaMarket::TaskUpdateForexDayLineDB(void) {
  CString str;

  for (auto& pSymbol : m_vForexSymbol) {
    if (pSymbol->IsDayLineNeedSavingAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
      if (pSymbol->GetDayLineSize() > 0) {
        if (pSymbol->HaveNewDayLineData()) {
          RunningThreadUpdateForexDayLineDB(pSymbol);
          TRACE("更新%s日线数据\n", pSymbol->GetSymbol().GetBuffer());
        }
        else pSymbol->UnloadDayLine(); // 当无需执行存储函数时，这里还要单独卸载日线数据。因存储日线数据线程稍后才执行，故而不能在此统一执行删除函数。
      }
      else { // 此种情况为有股票代码，但此代码尚未上市
        CString str1 = pSymbol->GetSymbol();
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

bool CAmericaMarket::TaskUpdateCountryListDB(void) {
  RunningThreadUpdateCountryListDB();
  return false;
}

bool CAmericaMarket::TaskUpdateEPSSurpriseDB(void) {
  CString str;

  for (auto& pStake : m_vAmericaStake) {
    if (pStake->IsEPSSurpriseNeedSaveAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
      RunningThreadUpdateEPSSurpriseDB(pStake);
      TRACE("更新%s EPS surprise数据\n", pStake->GetSymbol().GetBuffer());
    }
    if (gl_fExitingSystem) {
      break; // 如果程序正在退出，则停止存储。
    }
  }

  return(true);
}

bool CAmericaMarket::TaskUpdateEconomicCalendar(void) {
  CSetEconomicCalendar setEconomicCalendar;
  CEconomicCalendarPtr pCalendar = nullptr;

  if (m_lLastTotalEconomicCalendar < m_lTotalEconomicCalendar) {
    setEconomicCalendar.Open();
    setEconomicCalendar.m_pDatabase->BeginTrans();
    for (long l = m_lLastTotalEconomicCalendar; l < m_lTotalEconomicCalendar; l++) {
      pCalendar = m_vEconomicCalendar.at(l);
      pCalendar->Append(setEconomicCalendar);
    }
    setEconomicCalendar.m_pDatabase->CommitTrans();
    setEconomicCalendar.Close();
    m_lLastTotalEconomicCalendar = m_lTotalEconomicCalendar;
  }
  return true;
}

bool CAmericaMarket::TaskCheckSystemReady(void) {
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

bool CAmericaMarket::RunningThreadUpdateDayLineDB(CAmericaStakePtr pStake) {
  thread thread1(ThreadUpdateAmericaStakeDayLineDB, pStake);
  thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
  return true;
}

bool CAmericaMarket::RunningTaskThreadUpdateStakeDB(void) {
  thread thread1(ThreadUpdateStakeDB, this);
  thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
  return true;
}

bool CAmericaMarket::RunningThreadUpdateForexDayLineDB(CForexSymbolPtr pSymbol) {
  thread thread1(ThreadUpdateForexDayLineDB, pSymbol);
  thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
  return true;
}

bool CAmericaMarket::RunningThreadUpdateForexSymbolDB() {
  thread thread1(ThreadUpdateForexSymbolDB, this);
  thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
  return true;
}

bool CAmericaMarket::RunningThreadUpdateCountryListDB(void) {
  thread thread1(ThreadUpdateCountryListDB, this);
  thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
  return true;
}

bool CAmericaMarket::RunningThreadUpdateEPSSurpriseDB(CAmericaStakePtr pStake) {
  thread thread1(ThreadUpdateEPSSurpriseDB, pStake);
  thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
  return true;
}

bool CAmericaMarket::IsAmericaStake(CString strSymbol) {
  if (m_mapAmericaStake.find(strSymbol) == m_mapAmericaStake.end()) { // 新代码？
    return false;
  }
  else return true;
}

bool CAmericaMarket::IsAmericaStakeUpdated(void) {
  const int iTotal = m_vAmericaStake.size();
  for (int i = 0; i < iTotal; i++) {
    if (m_vAmericaStake.at(i)->m_fUpdateDatabase) return true;
  }
  return false;
}

CAmericaStakePtr CAmericaMarket::GetAmericaStake(CString strTicker) {
  if (m_mapAmericaStake.find(strTicker) != m_mapAmericaStake.end()) {
    return m_vAmericaStake.at(m_mapAmericaStake.at(strTicker));
  }
  else return nullptr;
}

void CAmericaMarket::AddAmericaStake(CAmericaStakePtr pStake) {
  m_vAmericaStake.push_back(pStake);
  m_mapAmericaStake[pStake->m_strSymbol] = m_lTotalAmericaStake++;
}

bool CAmericaMarket::UpdateEconomicCalendar(vector<CEconomicCalendarPtr> vEconomicCalendar) {
  CString strSymbol = _T("");

  for (auto& pEconomicCalendar : vEconomicCalendar) {
    strSymbol = pEconomicCalendar->m_strCountry + pEconomicCalendar->m_strEvent + pEconomicCalendar->m_strTime;
    if (m_mapEconomicCalendar.find(strSymbol) == m_mapEconomicCalendar.end()) { // 新事件？
      m_vEconomicCalendar.push_back(pEconomicCalendar);
      m_mapEconomicCalendar[strSymbol] = m_lTotalEconomicCalendar++;
    }
  }
  return true;
}

long CAmericaMarket::GetFinnInquiry(void) {
  WebInquiry inquiry{ 0, 0, 0 };
  if (m_qFinnhubWebInquiry.size() > 0) {
    inquiry = m_qFinnhubWebInquiry.top();
    m_qFinnhubWebInquiry.pop();
    return inquiry.m_lInquiryIndex;
  }
  return -1;
}

bool CAmericaMarket::LoadAmericaStake(void) {
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

bool CAmericaMarket::SaveCompnayProfile(void) {
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

bool CAmericaMarket::UpdateCountryListDB(void) {
  CCountryPtr pCountry = nullptr;
  CSetCountry setCountry;

  if (m_lLastTotalCountry < m_lTotalCountry) {
    setCountry.Open();
    setCountry.m_pDatabase->BeginTrans();
    for (long l = m_lLastTotalCountry; l < m_lTotalCountry; l++) {
      pCountry = m_vCountry.at(l);
      pCountry->Append(setCountry);
    }
    setCountry.m_pDatabase->CommitTrans();
    setCountry.Close();
    m_lLastTotalCountry = m_lTotalCountry;
  }
  return true;
}

bool CAmericaMarket::UpdateStakeDB(void) {
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

bool CAmericaMarket::UpdateForexSymbolDB(void) {
  const long lTotalForexSymbol = m_vForexSymbol.size();
  CForexSymbolPtr pSymbol = nullptr;
  CSetForexSymbol setForexSymbol;

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

  setForexSymbol.Open();
  setForexSymbol.m_pDatabase->BeginTrans();
  while (!setForexSymbol.IsEOF()) {
    pSymbol = m_vForexSymbol.at(m_mapForexSymbol.at(setForexSymbol.m_Symbol));
    if (pSymbol->m_fUpdateDatabase) {
      pSymbol->Update(setForexSymbol);
      pSymbol->m_fUpdateDatabase = false;
    }
    setForexSymbol.MoveNext();
  }
  setForexSymbol.m_pDatabase->CommitTrans();
  setForexSymbol.Close();
  return true;
}

bool CAmericaMarket::UpdateEconomicCalendarDB(void) {
  const long lTotalEconomicCalendar = m_vEconomicCalendar.size();
  CEconomicCalendarPtr pEconomicCalendar = nullptr;
  CSetEconomicCalendar setEconomicCalendar;

  if (m_lLastTotalEconomicCalendar < m_lTotalEconomicCalendar) {
    setEconomicCalendar.Open();
    setEconomicCalendar.m_pDatabase->BeginTrans();
    for (long l = m_lLastTotalEconomicCalendar; l < m_lTotalEconomicCalendar; l++) {
      pEconomicCalendar = m_vEconomicCalendar.at(l);
      pEconomicCalendar->Append(setEconomicCalendar);
    }
    setEconomicCalendar.m_pDatabase->CommitTrans();
    setEconomicCalendar.Close();
    m_lLastTotalEconomicCalendar = m_lTotalEconomicCalendar;
  }

  return true;
}

bool CAmericaMarket::RebulidFinnhubDayLine(void) {
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

bool CAmericaMarket::SortStakeTable(void) {
  CSetAmericaStake setAmericaStake;
  vector<CAmericaStakePtr> vStake;
  CAmericaStakePtr pStake = nullptr;

  setAmericaStake.m_strSort = _T("[Symbol]");
  setAmericaStake.Open();
  setAmericaStake.m_pDatabase->BeginTrans();
  while (!setAmericaStake.IsEOF()) {
    pStake = make_shared<CAmericaStake>();
    pStake->Load(setAmericaStake);
    pStake->SetCheckingDayLineStatus();
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

bool CAmericaMarket::LoadForexExchange(void) {
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

bool CAmericaMarket::LoadForexSymbol(void) {
  CSetForexSymbol setForexSymbol;
  CForexSymbolPtr pSymbol = nullptr;
  int i = 0;

  setForexSymbol.Open();
  while (!setForexSymbol.IsEOF()) {
    pSymbol = make_shared<CForexSymbol>();
    pSymbol->Load(setForexSymbol);
    pSymbol->SetCheckingDayLineStatus();
    m_vForexSymbol.push_back(pSymbol);
    m_mapForexSymbol[pSymbol->m_strSymbol] = i++;
    setForexSymbol.MoveNext();
  }
  setForexSymbol.Close();
  m_lLastTotalForexSymbol = m_lTotalForexSymbol = m_vForexSymbol.size();

  return true;
}

bool CAmericaMarket::LoadCountryList(void) {
  CSetCountry setCountry;
  CCountryPtr pCountry = nullptr;

  setCountry.Open();
  while (!setCountry.IsEOF()) {
    pCountry = make_shared<CCountry>();
    pCountry->Load(setCountry);
    m_vCountry.push_back(pCountry);
    setCountry.MoveNext();
  }
  setCountry.Close();
  m_lLastTotalCountry = m_lTotalCountry = m_vCountry.size();

  return true;
}

bool CAmericaMarket::LoadEconomicCalendarDB(void) {
  CSetEconomicCalendar setEconomicCalendar;
  CEconomicCalendarPtr pEconomicCalendar = nullptr;
  CString strSymbol = _T("");

  setEconomicCalendar.Open();
  while (!setEconomicCalendar.IsEOF()) {
    pEconomicCalendar = make_shared<CEconomicCalendar>();
    pEconomicCalendar->Load(setEconomicCalendar);
    strSymbol = pEconomicCalendar->m_strCountry + pEconomicCalendar->m_strEvent + pEconomicCalendar->m_strTime;
    m_vEconomicCalendar.push_back(pEconomicCalendar);
    m_mapEconomicCalendar[strSymbol] = m_lTotalEconomicCalendar++;
    setEconomicCalendar.MoveNext();
  }
  setEconomicCalendar.Close();
  m_lLastTotalEconomicCalendar = m_lTotalEconomicCalendar = m_vEconomicCalendar.size();

  return true;
}