#include "AmericaStakeMarket.h"
#include"thread.h"

#include"WebInquirer.h"
#include"ProcessAmericaStake.h"

#include"SetForexExchange.h"
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
  m_fSymbolUpdated = false; // ÿ����Ҫ���´���
  m_fAmericaStakeUpdated = false;
  m_fStakeDayLineUpdated = false;
  m_fForexExhangeUpdated = false;
  m_fForexSymbolUpdated = false;

  m_fFinnhubInquiring = false;
  m_fFinnhubDataReceived = true;

  m_fRebulidDayLine = false;
  SetSystemReady(false); // �г���ʼ״̬Ϊδ���úá�

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

  CString str = _T("����America Stake Market��������׼ʱ�䣺");
  str += GetStringOfMarketTime();
  gl_systemMessage.PushInformationMessage(str);
}

bool CAmericaStakeMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  static int s_iCountfinnhubLimit = 11; // ÿ1.2������ִ��һ�Σ��Է�ֹ����Ƶ�ʹ��ߵ������
  const long lCurrentTime = GetFormatedMarketTime();

  TaskCheckSystemReady();

  TaskInquiryTodaySymbol(); // ��һ���������������뵱��֤ȯ����
  TaskInquiryFinnhubForexExchange();
  TaskInquiryFinnhubForexSymbol();

  ProcessFinnhubWebDataReceived(); // Ҫ�ȴ����յ���Finnhub��������
  ProcessFinnhubInquiringMessage(); // Ȼ�������봦����һ��

  // ����Finnhub������Ϣ�����񣬽�Ҫ����������Ա�֤���г�ʱ���賿ʮ���Ӻ�ִ�С������ܹ���֤�������г�ʱû��ִ�в�ѯ����
  if (IsSystemReady() && (lCurrentTime > 1000) && !m_fFinnhubInquiring && (s_iCountfinnhubLimit < 0)) {
    s_iCountfinnhubLimit = 11;
    TaskInquiryAmericaStake();
    TaskInquiryDayLine();
    if (m_fStakeDayLineUpdated) {
      //TaskInquiryFinnhubRTQuote();
    }
  }
  else s_iCountfinnhubLimit--;

  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (GetMarketTime() > s_timeLast) {
    SchedulingTaskPerSecond(GetMarketTime() - s_timeLast, lCurrentTime);
    s_timeLast = GetMarketTime();
  }

  return true;
}

//////////////////////////////////////////////
//
// �������߳̽��յ���Finnhub������Ϣ��
//
// Ŀǰֻ����ͬʱ����һ����Ϣ������Ϣ��������������֮ǰ������������һ����Ϣ�������ܹ���֤ͬһ�ԡ�������Finnhub��վ���ٶ����ƣ�
// ÿ����ֻ����60�����룬�ʶ�û�б�Ҫǿ�������ٶȡ�
//
//////////////////////////////////////////////
bool CAmericaStakeMarket::ProcessFinnhubWebDataReceived(void) {
  CWebDataPtr pWebData = nullptr;
  CString str = _T("");
  vector<CString> vExchange;
  vector<CForexSymbolPtr> vForexSymbol;
  long lTemp = 0;

  ASSERT(gl_WebInquirer.GetFinnhubDataSize() <= 1);
  if (IsFinnhubDataReceived()) { // ����������ݽ������
    if (gl_WebInquirer.GetFinnhubDataSize() > 0) {  // ����ǰ��������
      ASSERT(m_fFinnhubInquiring);
      pWebData = gl_WebInquirer.PopFinnhubData();
      switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) {
      case __COMPANY_PROFILE__: // Ŀǰ����˻��޷�ʹ�ô˹��ܡ�
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
      TRACE("����%s��������\n", m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->m_strSymbol.GetBuffer());
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
/// finnhub��ȡ������������ͽ����ֻ�ִ�з�ʽ���ʶ����ٵ������β����һ���ֻء�
/// <param name=""></param>
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool CAmericaStakeMarket::ProcessFinnhubInquiringMessage(void) {
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  CAmericaStakePtr pStake = nullptr;

  if (m_qWebInquiry.size() > 0) { // ������ȴ���
    ASSERT(m_fFinnhubInquiring);
    if (IsFinnhubDataReceived()) { //�Ѿ�����������������Finnhub�����Ѿ����յ��ˣ�
      m_CurrentFinnhubInquiry = m_qWebInquiry.top();
      m_qWebInquiry.pop();
      gl_pFinnhubWebInquiry->SetInquiryingStrPrefix(m_vFinnhubInquiringStr.at(m_CurrentFinnhubInquiry.m_lInquiryIndex)); // ����ǰ׺
      switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) { // ���ݲ�ͬ��Ҫ��������׺�ַ���
      case __COMPANY_PROFILE__: // ����˻��޷���ȡ����Ϣ��sandboxģʽ�ܶ�ȡ�����Ǵ���ģ�ֻ�����ڲ��ԡ�
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
      TRACE("δ����ָ��%d\n", m_CurrentFinnhubInquiry.m_lInquiryIndex);
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

    return true;
  }
  else return false;
}

bool CAmericaStakeMarket::SchedulingTaskPer1Minute(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 59;  // һ����һ�εļ�����

  // �Զ���ѯcrweber.com
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
/// ��������������ÿ����ִ��һ�εĵ����С�
/// <returns></returns>
///
bool CAmericaStakeMarket::TaskResetMarket(long lCurrentTime) {
  // �г�ʱ���������ϵͳ
  if (IsSystemReady() && IsPermitResetMarket()) { // �����������ϵͳ
    if (lCurrentTime <= 100) { // ���г�ʱ�����ʱ�������г� // ����������ʱ��Ϊ����ʮ��ʱ��������ʱ����ʱ����
      SetResetMarket(true);// ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
      SetPermitResetMarket(false); // ���첻����������ϵͳ��
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
      TRACE("����%s���\n", m_vAmericaStake.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
      m_fFinnhubInquiring = true;
    }
    else {
      m_fAmericaStakeUpdated = true;
      TRACE("Finnhub��Ʊ���������\n");
      str = _T("Finnhub��Ʊ���������");
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
      TRACE("����%s��������\n", m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->m_strSymbol.GetBuffer());
    }
    else {
      m_fStakeDayLineUpdated = true;
      TRACE("Finnhub���߸������\n");
      str = _T("�����г�������ʷ���ݸ������");
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
    TRACE("����%sʵʱ����\n", m_vAmericaStake.at(m_lCurrentRTDataQuotePos)->m_strSymbol.GetBuffer());
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

bool CAmericaStakeMarket::TaskCheckSystemReady(void) {
  CString str = _T("");

  if (!IsSystemReady()) {
    if (m_fSymbolUpdated && m_fForexExhangeUpdated && m_fForexSymbolUpdated) {
      str = _T("�����г���ʼ�����");
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
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CAmericaStakeMarket::RunningTaskThreadUpdateStakeDB(void) {
  thread thread1(ThreadUpdateStakeDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
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