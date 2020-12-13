#include "AmericaStakeMarket.h"
#include"thread.h"

#include"WebInquirer.h"
#include"ProcessAmericaStake.h"

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
  m_lCurrentRTDataQuotePos = 0;

  m_vAmericaStake.resize(0);
  m_mapAmericaStake.clear();
  m_fSymbolUpdated = false; // 每日需要更新代码
  m_fSymbolProceeded = false;
  m_fAmericaStakeUpdated = false;
  m_fStakeDayLineUpdated = false;

  m_fFinnhubInquiring = false;
  m_fFinnhubDataReceived = true;
}

void CAmericaStakeMarket::ResetMarket(void) {
  Reset();

  LoadAmericaStake();

  CString str = _T("重置America Stake Market于美东标准时间：");
  str += GetStringOfMarketTime();
  gl_systemMessage.PushInformationMessage(str);
}

bool CAmericaStakeMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  static int s_iCountFinnhub = -1;
  const long lCurrentTime = GetFormatedMarketTime();

  // 第一个动作，首先申请当日证券代码
  TaskInquiryTodaySymbol();

  // Finnhub.io读取函数。
  if (s_iCountFinnhub-- < 0) { // 每0.3秒调用一次
    s_iCountFinnhub = -1; //目前采用1.5秒左右接收一次数据。最快不能达到每次1秒，故s_iCountFinnhub必须设置为4.这样能够保证第一次接收时
    ProcessFinnhubWebDataReceived(); // 要先处理收到的Finnhub网络数据
    ProcessFinnhubInquiringMessage(); // 然后再申请处理下一个
  }
  else s_iCountFinnhub--;

  //根据时间，调度各项定时任务.每秒调度一次
  if (GetMarketTime() > s_timeLast) {
    SchedulingTaskPerSecond(GetMarketTime() - s_timeLast, lCurrentTime);
    s_timeLast = GetMarketTime();
  }

  return true;
}

//////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////
bool CAmericaStakeMarket::ProcessFinnhubWebDataReceived(void) {
  CWebDataPtr pWebData = nullptr;

  ASSERT(gl_WebInquirer.GetFinnhubDataSize() <= 1);
  if (IsFinnhubDataReceived() && (gl_WebInquirer.GetFinnhubDataSize() > 0)) { // 如果网络数据接收到了
      // 处理当前网络数据
    pWebData = gl_WebInquirer.PopFinnhubData();
    switch (m_lPrefixIndex) {
    case __COMPANY_PROFILE__: // 目前免费账户无法使用此功能。
    ProcessAmericaStakeProfile(pWebData);
    break;
    case __COMPANY_PROFILE2__:
    ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentProfilePos);
    ProcessAmericaStakeProfile2(pWebData);
    break;
    case  __COMPANY_SYMBOLS__:
    ProcessAmericaStakeSymbol(pWebData);
    SetSystemReady(true);
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
    ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentRTDataQuotePos);
    ProcessAmericaStakeQuote(pWebData, m_vAmericaStake.at(m_lCurrentRTDataQuotePos));
    break;
    case __STAKE_CANDLES__:
    ASSERT(m_CurrentFinnhubInquiry.m_lStakeIndex == m_lCurrentUpdateDayLinePos);
    ProcessAmericaStakeCandle(pWebData, m_vAmericaStake.at(m_lCurrentUpdateDayLinePos));
    TRACE("处理%s日线数据\n", m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->m_strSymbol.GetBuffer());
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
    m_fFinnhubInquiring = false;
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

  if (IsFinnhubDataReceived()) { //已经发出了数据申请且Finnhub数据已经接收到了？
    if (m_qWebInquiry.size() > 0) { // 有申请等待？
      ASSERT(m_fFinnhubInquiring);
      m_CurrentFinnhubInquiry = m_qWebInquiry.top();
      m_lPrefixIndex = GetFinnInquiry();
      gl_pFinnhubWebInquiry->SetInquiryingStrPrefix(m_vFinnhubInquiringStr.at(m_lPrefixIndex)); // 设置前缀
      switch (m_lPrefixIndex) { // 根据不同的要求设置中缀字符串
      case __COMPANY_PROFILE__: // 免费账户无法读取此信息，sandbox模式能读取，但是错误的，只能用于测试。
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
      pStake = m_vAmericaStake.at(m_lCurrentRTDataQuotePos);
      strMiddle = pStake->GetSymbol();
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      break;
      case __STAKE_CANDLES__:
      pStake = m_vAmericaStake.at(m_lCurrentUpdateDayLinePos);
      strMiddle = pStake->GetDayLineInquiryString(GetMarketTime());
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      pStake->m_fDayLineNeedUpdate = false;
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
      SetFinnhubDataReceived(false);
    }
  }

  return true;
}

bool CAmericaStakeMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
  SchedulingTaskPer1Hour(lSecond, lCurrentTime);
  SchedulingTaskPer1Minute(lSecond, lCurrentTime);
  SchedulingTaskPer10Seconds(lSecond, lCurrentTime);

  TaskSaveStakeSymbolDB();

  static int s_iCount = 4;

  if (m_fSymbolProceeded) {
    if (s_iCount-- < 0) {
      s_iCount = 4;
      TaskInquiryAmericaStake();
      TaskInquiryDayLine();
    }
    TaskInquiryFinnhubRTQuote();
  }

  static int s_iCount2 = 1;
  if (m_fSymbolProceeded) {
    if (s_iCount2-- < 0) {
      s_iCount2 = 1;
    }
  }

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
    TaskResetMarket(lCurrentTime);

    return true;
  }
  else return false;
}

bool CAmericaStakeMarket::SchedulingTaskPer1Minute(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 59;  // 一分钟一次的计数器

  TaskResetMarket(lCurrentTime);

  // 自动查询crweber.com
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
  // 市场时间四点重启系统
  if (IsPermitResetMarket()) { // 如果允许重置系统
    if ((lCurrentTime >= 29000) && (lCurrentTime < 30000)) { // 本市场时间的一点重启本市场 // 东八区本地时间为下午五时（或夏令时的四时）。
      SetResetMarket(true);// 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
      SetPermitResetMarket(false); // 今天不再允许重启系统。
    }
  }
  return true;
}

bool CAmericaStakeMarket::TaskInquiryTodaySymbol(void) {
  FinnhubInquiry inquiry;
  if (!m_fSymbolUpdated) {
    inquiry.m_lInquiryIndex = __COMPANY_SYMBOLS__;
    inquiry.m_iPriority = 10;
    m_qWebInquiry.push(inquiry);
    m_fSymbolUpdated = true;
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
  if (!m_fAmericaStakeUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentProfilePos = 0; m_lCurrentProfilePos < m_vAmericaStake.size(); m_lCurrentProfilePos++) {
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
      m_fFinnhubInquiring = true;
    }
    else {
      m_fAmericaStakeUpdated = true;
      TRACE("Finnhub股票简介更新完毕\n");
    }
  }
  return false;
}

bool CAmericaStakeMarket::TaskInquiryDayLine(void) {
  bool fFound = false;
  FinnhubInquiry inquiry;
  CAmericaStakePtr pStake;

  ASSERT(m_fSymbolProceeded);
  if (!m_fStakeDayLineUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < m_vAmericaStake.size(); m_lCurrentUpdateDayLinePos++) {
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
    }
  }
  return false;
}

bool CAmericaStakeMarket::TaskInquiryFinnhubRTQuote(void) {
  bool fFound = false;
  FinnhubInquiry inquiry;

  ASSERT(m_fSymbolProceeded);
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
  FinnhubInquiry inquiry;
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