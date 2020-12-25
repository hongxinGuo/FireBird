#include "AmericaMarket.h"
#include"thread.h"

#include"WebInquirer.h"
#include"ProcessFinnhubWebData.h"
#include"ProcessTiingoWebData.h"
#include"EPSSurprise.h"

#include"SetForexExchange.h"
#include"SetAmericaStake.h"
#include"SetCountry.h"
#include"SetEconomicCalendar.h"
#include"SetEPSSurprise.h"

Semaphore gl_SaveAmericaOneStockDayLine(1);  // ���ź�����������������Ʊ������ʷ���ݿ�
Semaphore gl_SaveEPSSurprise(1);  // ���ź������ڸ���EPSSurprise����
Semaphore gl_SaveForexDayLine(1);  // ���ź�����������Forex������ʷ���ݿ�

CAmericaMarket::CAmericaMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CAmericaMarket�г�����ֻ��������һ��ʵ��\n");
  }

  m_strMarketId = _T("Finnhub������Ϣ");
  m_lMarketTimeZone = 4 * 3600; // ��������ʹ��������׼ʱ�䡣
  CalculateTime();

  InitialFinnhubInquiryStr();
  InitialTiingoInquiryStr();

  Reset();
}

void CAmericaMarket::InitialFinnhubInquiryStr(void) {
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

  m_vFinnhubInquiringStr.at(__STOCK_EPS_SURPRISE__) = _T("https://finnhub.io/api/v1/stock/earnings?symbol=");
  m_vFinnhubInquiringStr.at(__STOCK_EARNING_CALENDER__) = _T("https://finnhub.io/api/v1/calendar/earnings?");

  m_vFinnhubInquiringStr.at(__STOCK_QUOTE__) = _T("https://finnhub.io/api/v1/quote?symbol="); // ĳ������Ľ���
  m_vFinnhubInquiringStr.at(__STOCK_CANDLES__) = _T("https://finnhub.io/api/v1/stock/candle?symbol="); // ��ʷ����ͼ

  m_vFinnhubInquiringStr.at(__FOREX_EXCHANGE__) = _T("https://finnhub.io/api/v1/forex/exchange?");
  m_vFinnhubInquiringStr.at(__FOREX_SYMBOLS__) = _T("https://finnhub.io/api/v1/forex/symbol?exchange=");
  m_vFinnhubInquiringStr.at(__FOREX_CANDLES__) = _T("https://finnhub.io/api/v1/forex/candle?symbol=");
  m_vFinnhubInquiringStr.at(__FOREX_ALL_RATES__) = _T("https://finnhub.io/api/v1/forex/rates?base=USD");

  m_vFinnhubInquiringStr.at(__ECONOMIC_COUNTRY_LIST__) = _T("https://finnhub.io/api/v1/country?");
  m_vFinnhubInquiringStr.at(__ECONOMIC_CALENDAR__) = _T("https://finnhub.io/api/v1/calendar/economic?");
}

void CAmericaMarket::InitialTiingoInquiryStr(void) {
  m_vTiingoInquiringStr.resize(1000);

  m_vTiingoInquiringStr.at(__STOCK_CANDLES__) = _T("https://api.tiingo.com/tiingo/daily/");
}

CAmericaMarket::~CAmericaMarket() {
}

void CAmericaMarket::Reset(void) {
  ResetFinnhub();
}

void CAmericaMarket::ResetFinnhub(void) {
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

  // Finnhub�������������ݱ�ʶ��ÿ����Ҫ���á�
  m_fSymbolUpdated = false; // ÿ����Ҫ���´���
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
  SetSystemReady(false); // �г���ʼ״̬Ϊδ���úá�

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

void CAmericaMarket::ResetQuandl(void) {
}

void CAmericaMarket::ResetTiingo(void) {
}

void CAmericaMarket::ResetMarket(void) {
  Reset();

  LoadCountryList();
  LoadAmericaStake();
  LoadForexExchange();
  LoadForexSymbol();
  LoadEconomicCalendarDB();

  CString str = _T("����America Stake Market��������׼ʱ�䣺");
  str += GetStringOfMarketTime();
  gl_systemMessage.PushInformationMessage(str);
}

bool CAmericaMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  static int s_iCountfinnhubLimit = 11; // ÿ1.2������ִ��һ�Σ��Է�ֹ����Ƶ�ʹ��ߵ������
  const long lCurrentTime = GetFormatedMarketTime();

  TaskCheckSystemReady();

  TaskInquiryFinnhub(lCurrentTime);

  ProcessFinnhubWebDataReceived(); // Ҫ�ȴ����յ���Finnhub��������
  ProcessFinnhubInquiringMessage(); // Ȼ�������봦����һ��

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
bool CAmericaMarket::ProcessFinnhubInquiringMessage(void) {
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  CAmericaStakePtr pStake = nullptr;
  CForexSymbolPtr pSymbol = nullptr;

  if (m_qFinnhubWebInquiry.size() > 0) { // ������ȴ���
    ASSERT(m_fFinnhubInquiring);
    if (IsFinnhubDataReceived()) { //�Ѿ�����������������Finnhub�����Ѿ����յ��ˣ�
      m_CurrentFinnhubInquiry = m_qFinnhubWebInquiry.top();
      m_qFinnhubWebInquiry.pop();
      gl_pFinnhubWebInquiry->SetInquiryingStrPrefix(m_vFinnhubInquiringStr.at(m_CurrentFinnhubInquiry.m_lInquiryIndex)); // ����ǰ׺
      switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) { // ���ݲ�ͬ��Ҫ��������׺�ַ���
      case __COMPANY_PROFILE__: // Premium ����˻��޷���ȡ����Ϣ��sandboxģʽ�ܶ�ȡ�����Ǵ���ģ�ֻ�����ڲ��ԡ�
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_strSymbol);
      m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_fInquiryAmericaStake = false;
      break;
      case __COMPANY_PROFILE2__:
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_strSymbol);
      m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_fInquiryAmericaStake = false;
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
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_strSymbol);
      m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_fPeerUpdated = true;
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
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_strSymbol);
      break;
      case __STOCK_EARNING_CALENDER__:
      break;
      case __STOCK_QUOTE__:
      pStake = m_vAmericaStake.at(m_lCurrentRTDataQuotePos);
      strMiddle = pStake->GetSymbol();
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      break;
      case __STOCK_CANDLES__:
      pStake = m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex);
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
      strMiddle = m_vForexExchange.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      break;
      case __FOREX_CANDLES__:
      pSymbol = m_vForexSymbol.at(m_CurrentFinnhubInquiry.m_lStockIndex);
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
      TRACE("δ����ָ��%d\n", m_CurrentFinnhubInquiry.m_lInquiryIndex);
      break;
      }
      SetFinnhubDataReceived(false); // ���ô˱�ʶ��Ҫ�������������̣߳�GetWebData��֮ǰ���������߳��еĶ������׳�����
      gl_pFinnhubWebInquiry->GetWebData();
    }
  }

  return true;
}

bool CompareAmericaStake2(CAmericaStakePtr p1, CAmericaStakePtr p2) { return (p1->m_strSymbol.Compare(p2->m_strSymbol) < 0); }

//////////////////////////////////////////////
//
// ���������߳̽��յ���Finnhub������Ϣ��
//
// Ŀǰֻ����ͬʱ����һ����Ϣ������Ϣ��������������֮ǰ��������������һ����Ϣ�������ܹ���֤ͬһ�ԡ�������Finnhub��վ���ٶ����ƣ�
// ÿ����ֻ����60�����룬�ʶ�û�б�Ҫǿ�������ٶȡ�
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
  vector<CAmericaStakePtr> vStake;
  long lTemp = 0;

  ASSERT(gl_WebInquirer.GetFinnhubDataSize() <= 1);
  if (IsFinnhubDataReceived()) { // ����������ݽ������
    if (gl_WebInquirer.GetFinnhubDataSize() > 0) {  // ������ǰ��������
      ASSERT(m_fFinnhubInquiring);
      pWebData = gl_WebInquirer.PopFinnhubData();
      switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) {
      case __COMPANY_PROFILE__: // Ŀǰ����˻��޷�ʹ�ô˹��ܡ�
      ProcessFinnhubStockProfile(pWebData, m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex));
      break;
      case __COMPANY_PROFILE2__:
      pStake = m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      ProcessFinnhubStockProfile2(pWebData, pStake);
      pStake->m_lProfileUpdateDate = gl_pAmericaMarket->GetFormatedMarketDate();
      pStake->m_fUpdateDatabase = true;
      break;
      case  __COMPANY_SYMBOLS__:
      if (ProcessFinnhubStockSymbol(pWebData, vStake)) {
        m_lTotalAmericaStake += vStake.size();
        for (auto& pStake : vStake) {
          m_vAmericaStake.push_back(pStake);
        }
        sort(m_vAmericaStake.begin(), m_vAmericaStake.end(), CompareAmericaStake2);
        m_mapAmericaStake.clear();
        int j = 0;
        for (auto& pStake : m_vAmericaStake) {
          m_mapAmericaStake[pStake->m_strSymbol] = j++;
        }
        gl_systemMessage.PushInformationMessage("�����´��룬���´��뼯");
      }
      m_fSymbolUpdated = true;
      break;
      case  __MARKET_NEWS__:
      break;
      case __COMPANY_NEWS__:
      break;
      case __NEWS_SENTIMENT__:
      break;
      case __PEERS__:
      pStake = m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      ProcessFinnhubStockPeer(pWebData, pStake);
      pStake->m_fUpdateDatabase = true;
      break;
      case __BASIC_FINANCIALS__:
      break;
      case __STOCK_EPS_SURPRISE__:
      pStake = m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      if (ProcessFinnhubEPSSurprise(pWebData, vEPSSurprise)) {
        if (vEPSSurprise.size() > 0) {
          pStake->UpdateEPSSurprise(vEPSSurprise);
        }
        else {
          pStake->m_lLastEPSSurpriseUpdateDate = 19700101; // ����������Ϊ���硣
          pStake->m_fUpdateDatabase = true; // ���´��뼯
        }
        pStake->m_fEPSSurpriseNeedUpdate = false;
        pStake->m_fEPSSurpriseNeedSave = true;
      }
      break;
      case __STOCK_QUOTE__:
      ProcessFinnhubStockQuote(pWebData, m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex));
      break;
      case __STOCK_CANDLES__:
      ProcessFinnhubStockCandle(pWebData, m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex));
      TRACE("����%s��������\n", m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_strSymbol.GetBuffer());
      break;
      case __FOREX_EXCHANGE__:
      ProcessFinnhubForexExchange(pWebData, vExchange);
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
      ProcessFinnhubForexSymbol(pWebData, vForexSymbol);
      for (auto& pSymbol : vForexSymbol) {
        if (m_mapForexSymbol.find(pSymbol->m_strSymbol) == m_mapForexSymbol.end()) {
          pSymbol->m_strExchange = m_vForexExchange.at(m_CurrentFinnhubInquiry.m_lStockIndex);
          lTemp = m_mapForexSymbol.size();
          m_mapForexSymbol[pSymbol->m_strSymbol] = lTemp;
          m_vForexSymbol.push_back(pSymbol);
        }
      }
      m_lTotalForexSymbol = m_vForexSymbol.size();
      break;
      case __FOREX_CANDLES__:
      ProcessFinnhubForexCandle(pWebData, m_vForexSymbol.at(m_CurrentFinnhubInquiry.m_lStockIndex));
      TRACE("����%s��������\n", m_vForexSymbol.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_strSymbol.GetBuffer());
      break;
      case __FOREX_ALL_RATES__:
      break;
      case __ECONOMIC_COUNTRY_LIST__:
      ProcessFinnhubCountryList(pWebData, m_vCountry);
      m_lTotalCountry = m_vCountry.size();
      m_fCountryListUpdated = true;
      break;
      case __ECONOMIC_CALENDAR__:
      ProcessFinnhubEconomicCalendar(pWebData, vEconomicCalendar);
      UpdateEconomicCalendar(vEconomicCalendar);
      m_fEconomicCalendarUpdated = true;
      break;
      default:
      break;
      }
      m_fFinnhubInquiring = false;
    }
  }

  return true;
}

bool CAmericaMarket::ProcessTiingoInquiringMessage(void) {
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  CAmericaStakePtr pStake = nullptr;
  CForexSymbolPtr pSymbol = nullptr;

  if (m_qTiingoWebInquiry.size() > 0) { // ������ȴ���
    ASSERT(m_fTiingoInquiring);
    if (IsTiingoDataReceived()) { //�Ѿ�����������������Tiingo�����Ѿ����յ��ˣ�
      m_CurrentTiingoInquiry = m_qTiingoWebInquiry.top();
      m_qTiingoWebInquiry.pop();
      gl_pTiingoWebInquiry->SetInquiryingStrPrefix(m_vTiingoInquiringStr.at(m_CurrentTiingoInquiry.m_lInquiryIndex)); // ����ǰ׺
      switch (m_CurrentTiingoInquiry.m_lInquiryIndex) { // ���ݲ�ͬ��Ҫ��������׺�ַ���
      case __COMPANY_PROFILE__: // Premium ����˻��޷���ȡ����Ϣ��sandboxģʽ�ܶ�ȡ�����Ǵ���ģ�ֻ�����ڲ��ԡ�
      ASSERT(m_CurrentTiingoInquiry.m_lStockIndex == m_lCurrentUpdateDayLinePos);
      gl_pTiingoWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_lCurrentProfilePos)->m_strSymbol);
      m_vAmericaStake.at(m_lCurrentProfilePos)->m_fInquiryAmericaStake = false;
      break;
      case __COMPANY_PROFILE2__:
      ASSERT(m_CurrentTiingoInquiry.m_lStockIndex == m_lCurrentProfilePos);
      gl_pTiingoWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_lCurrentProfilePos)->m_strSymbol);
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
      ASSERT(m_CurrentTiingoInquiry.m_lStockIndex == m_lCurrentUpdatePeerPos);
      gl_pTiingoWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_lCurrentUpdatePeerPos)->m_strSymbol);
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
      ASSERT(m_CurrentTiingoInquiry.m_lStockIndex == m_lCurrentUpdateEPSSurprisePos);
      gl_pTiingoWebInquiry->SetInquiryingStringMiddle(m_vAmericaStake.at(m_lCurrentUpdateEPSSurprisePos)->m_strSymbol);
      break;
      case __STOCK_EARNING_CALENDER__:
      break;
      case __STOCK_QUOTE__:
      pStake = m_vAmericaStake.at(m_lCurrentRTDataQuotePos);
      strMiddle = pStake->GetSymbol();
      gl_pTiingoWebInquiry->SetInquiryingStringMiddle(strMiddle);
      break;
      case __STOCK_CANDLES__:
      pStake = m_vAmericaStake.at(m_CurrentTiingoInquiry.m_lStockIndex);
      strMiddle = pStake->GetDayLineInquiryString(GetMarketTime());
      gl_pTiingoWebInquiry->SetInquiryingStringMiddle(strMiddle);
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
      ASSERT(m_CurrentTiingoInquiry.m_lStockIndex == (m_lCurrentForexExchangePos - 1));
      strMiddle = m_vForexExchange.at(m_CurrentTiingoInquiry.m_lStockIndex);
      gl_pTiingoWebInquiry->SetInquiryingStringMiddle(strMiddle);
      break;
      case __FOREX_CANDLES__:
      ASSERT(m_CurrentTiingoInquiry.m_lStockIndex == m_lCurrentUpdateForexDayLinePos);
      pSymbol = m_vForexSymbol.at(m_lCurrentUpdateForexDayLinePos);
      strMiddle = pSymbol->GetDayLineInquiryString(GetMarketTime());
      gl_pTiingoWebInquiry->SetInquiryingStringMiddle(strMiddle);
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
      TRACE("δ����ָ��%d\n", m_CurrentTiingoInquiry.m_lInquiryIndex);
      break;
      }
      SetTiingoDataReceived(false); // ���ô˱�ʶ��Ҫ�������������̣߳�GetWebData��֮ǰ���������߳��еĶ������׳�����
      gl_pTiingoWebInquiry->GetWebData();
    }
  }

  return true;
}

bool CAmericaMarket::ProcessTiingoWebDataReceived(void) {
  CWebDataPtr pWebData = nullptr;
  CAmericaStakePtr pStake = nullptr;
  CString str = _T("");
  vector<CString> vExchange;
  vector<CForexSymbolPtr> vForexSymbol;
  vector<CEconomicCalendarPtr> vEconomicCalendar;
  vector<CEPSSurprisePtr> vEPSSurprise;
  vector<CAmericaStakePtr> vStake;
  long lTemp = 0;

  ASSERT(gl_WebInquirer.GetFinnhubDataSize() <= 1);
  if (IsFinnhubDataReceived()) { // ����������ݽ������
    if (gl_WebInquirer.GetFinnhubDataSize() > 0) {  // ������ǰ��������
      ASSERT(m_fFinnhubInquiring);
      pWebData = gl_WebInquirer.PopFinnhubData();
      switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) {
      case __COMPANY_PROFILE__: // Ŀǰ����˻��޷�ʹ�ô˹��ܡ�
      ASSERT(m_CurrentFinnhubInquiry.m_lStockIndex == m_lCurrentProfilePos);
      ProcessFinnhubStockProfile(pWebData, m_vAmericaStake.at(m_lCurrentProfilePos));
      break;
      case __COMPANY_PROFILE2__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStockIndex == m_lCurrentProfilePos);
      pStake = m_vAmericaStake.at(m_lCurrentProfilePos);
      ProcessFinnhubStockProfile2(pWebData, pStake);
      pStake->m_lProfileUpdateDate = gl_pAmericaMarket->GetFormatedMarketDate();
      pStake->m_fUpdateDatabase = true;
      break;
      case  __COMPANY_SYMBOLS__:
      if (ProcessFinnhubStockSymbol(pWebData, vStake)) {
        m_lTotalAmericaStake += vStake.size();
        for (auto& pStake : vStake) {
          m_vAmericaStake.push_back(pStake);
        }
        sort(m_vAmericaStake.begin(), m_vAmericaStake.end(), CompareAmericaStake2);
        m_mapAmericaStake.clear();
        int j = 0;
        for (auto& pStake : m_vAmericaStake) {
          m_mapAmericaStake[pStake->m_strSymbol] = j++;
        }
        gl_systemMessage.PushInformationMessage("�����´��룬���´��뼯");
      }
      m_fSymbolUpdated = true;
      break;
      case  __MARKET_NEWS__:
      break;
      case __COMPANY_NEWS__:
      break;
      case __NEWS_SENTIMENT__:
      break;
      case __PEERS__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStockIndex == m_lCurrentUpdatePeerPos);
      pStake = m_vAmericaStake.at(m_lCurrentUpdatePeerPos);
      ProcessFinnhubStockPeer(pWebData, pStake);
      pStake->m_fUpdateDatabase = true;
      break;
      case __BASIC_FINANCIALS__:
      break;
      case __STOCK_EPS_SURPRISE__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStockIndex == m_lCurrentUpdateEPSSurprisePos);
      pStake = m_vAmericaStake.at(m_lCurrentUpdateEPSSurprisePos);
      if (ProcessFinnhubEPSSurprise(pWebData, vEPSSurprise)) {
        if (vEPSSurprise.size() > 0) {
          pStake->UpdateEPSSurprise(vEPSSurprise);
        }
        else {
          pStake->m_lLastEPSSurpriseUpdateDate = 19700101; // ����������Ϊ���硣
          pStake->m_fUpdateDatabase = true; // ���´��뼯
        }
        pStake->m_fEPSSurpriseNeedUpdate = false;
        pStake->m_fEPSSurpriseNeedSave = true;
      }
      break;
      case __STOCK_QUOTE__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStockIndex == m_lCurrentRTDataQuotePos);
      ProcessFinnhubStockQuote(pWebData, m_vAmericaStake.at(m_lCurrentRTDataQuotePos));
      break;
      case __STOCK_CANDLES__:
      ProcessTiingoStockDayLine(pWebData, m_vAmericaStake.at(m_CurrentTiingoInquiry.m_lStockIndex));
      TRACE("����Tiingo %s��������\n", m_vAmericaStake.at(m_CurrentTiingoInquiry.m_lStockIndex)->m_strSymbol.GetBuffer());
      break;
      case __FOREX_EXCHANGE__:
      ProcessFinnhubForexExchange(pWebData, vExchange);
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
      ProcessFinnhubForexSymbol(pWebData, vForexSymbol);
      for (auto& pSymbol : vForexSymbol) {
        if (m_mapForexSymbol.find(pSymbol->m_strSymbol) == m_mapForexSymbol.end()) {
          pSymbol->m_strExchange = m_vForexExchange.at(m_CurrentFinnhubInquiry.m_lStockIndex);
          lTemp = m_mapForexSymbol.size();
          m_mapForexSymbol[pSymbol->m_strSymbol] = lTemp;
          m_vForexSymbol.push_back(pSymbol);
        }
      }
      m_lTotalForexSymbol = m_vForexSymbol.size();
      break;
      case __FOREX_CANDLES__:
      ASSERT(m_CurrentFinnhubInquiry.m_lStockIndex == m_lCurrentUpdateForexDayLinePos);
      ProcessFinnhubForexCandle(pWebData, m_vForexSymbol.at(m_lCurrentUpdateForexDayLinePos));
      TRACE("����%s��������\n", m_vForexSymbol.at(m_lCurrentUpdateForexDayLinePos)->m_strSymbol.GetBuffer());
      break;
      case __FOREX_ALL_RATES__:
      break;
      case __ECONOMIC_COUNTRY_LIST__:
      ProcessFinnhubCountryList(pWebData, m_vCountry);
      m_lTotalCountry = m_vCountry.size();
      m_fCountryListUpdated = true;
      break;
      case __ECONOMIC_CALENDAR__:
      ProcessFinnhubEconomicCalendar(pWebData, vEconomicCalendar);
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

bool CAmericaMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
  static int s_iCount1Hour = 3599;
  static int s_iCount10Minute = 599;
  static int s_iCount1Minute = 59;
  static int s_iCount10Second = 9;

  s_iCount10Second -= lSecond;
  s_iCount1Minute -= lSecond;
  s_iCount10Minute -= lSecond;
  s_iCount1Hour -= lSecond;
  if (s_iCount1Hour < 0) {
    s_iCount1Hour = 3599;
    SchedulingTaskPer1Hour(lCurrentTime);
  }
  if (s_iCount10Minute < 0) {
    s_iCount10Minute = 599;
    SchedulingTaskPer10Minute(lCurrentTime);
  }
  if (s_iCount1Minute < 0) {
    s_iCount1Minute = 59;
    SchedulingTaskPer1Minute(lCurrentTime);
  }
  if (s_iCount10Second < 0) {
    s_iCount10Second = 9;
    SchedulingTaskPer10Seconds(lCurrentTime);
  }

  TaskSaveStakeSymbolDB();
  static int s_iCount = -1;

  return true;
}

bool CAmericaMarket::SchedulingTaskPer10Seconds(long lCurrentTime) {
  return true;
}

bool CAmericaMarket::SchedulingTaskPer1Minute(long lCurrentTime) {
  TaskResetMarket(lCurrentTime);

  TaskUpdateCountryListDB();
  TaskUpdateForexExchangeDB();
  TaskUpdateForexSymbolDB();
  TaskUpdateForexDayLineDB();
  TaskUpdateDayLineDB();
  TaskUpdateEPSSurpriseDB();
  TaskUpdateEconomicCalendar();

  return true;
}

bool CAmericaMarket::SchedulingTaskPer10Minute(long lCurrentTime) {
  if (IsAmericaStakeUpdated()) {
    TaskUpdateStakeDB();
  }

  return true;
}

bool CAmericaMarket::SchedulingTaskPer1Hour(long lCurrentTime) {
  return true;
}

/// <summary>
/// /////////////////////////////////////////////////////////////////////////////////
///
/// ��������������ÿ����ִ��һ�εĵ����С�
/// <returns></returns>
///
bool CAmericaMarket::TaskResetMarket(long lCurrentTime) {
  // �г�ʱ��ʮ��ʱ����ϵͳ
  if (IsSystemReady() && IsPermitResetMarket()) { // �����������ϵͳ
    if ((lCurrentTime > 170000) && (lCurrentTime <= 170100)) { // ���г�ʱ���������ʱ(����ʱ��������ʱ�������г������������ڽ����������ݡ�
      SetResetMarket(true);// ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
      SetPermitResetMarket(false); // ���첻����������ϵͳ��
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// �˺�����SchedulingTask���ȣ�ÿ100��������ִ��һ�Ρ�
//
//////////////////////////////////////////////////////////////////////////////////////
bool CAmericaMarket::TaskInquiryFinnhub(long lCurrentTime) {
  static int s_iCountfinnhubLimit = 11; // ��֤ÿ12��ִ��һ�Σ���1.2��ÿ�Σ�
  if (((lCurrentTime < 165700) || (lCurrentTime > 170300))) { // ������ʱ����ϵͳ���ʶ���ʱ����������������Ϣ��
    TaskInquiryFinnhubCountryList();
    TaskInquiryFinnhubCompanySymbol(); // ��һ���������������뵱��֤ȯ����
    TaskInquiryFinnhubForexExchange();
    TaskInquiryFinnhubForexSymbol();
    //TaskInquiryFinnhubEconomicCalender();

    // ����Finnhub������Ϣ�����񣬽�Ҫ����������Ա�֤���г�ʱ���賿ʮ���Ӻ�ִ�С������ܹ���֤�������г�ʱû��ִ�в�ѯ����
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
  CString str;

  if (!m_fSymbolUpdated && !m_fFinnhubInquiring) {
    inquiry.m_lInquiryIndex = __COMPANY_SYMBOLS__;
    inquiry.m_iPriority = 10;
    m_qFinnhubWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    str = _T("��ѯFinnhub Symbol");
    gl_systemMessage.PushInformationMessage(str);

    return true;
  }
  return false;
}

bool CAmericaMarket::TaskSaveStakeSymbolDB(void) {
  CSetAmericaStake setAmericaStake;
  CAmericaStakePtr pStake = nullptr;

  if (m_lLastTotalAmericaStake < m_lTotalAmericaStake) {
    DeleteStakeSymbolDB();
    m_lLastTotalAmericaStake = m_lTotalAmericaStake;

    setAmericaStake.Open();
    setAmericaStake.m_pDatabase->BeginTrans();
    for (long l = 0; l < m_lTotalAmericaStake; l++) {
      pStake = m_vAmericaStake.at(l);
      pStake->Append(setAmericaStake);
    }
    setAmericaStake.m_pDatabase->CommitTrans();
    setAmericaStake.Close();
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
      inquiry.m_lStockIndex = m_lCurrentProfilePos;
      inquiry.m_iPriority = 10;
      m_qFinnhubWebInquiry.push(inquiry);
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
      inquiry.m_lStockIndex = m_lCurrentUpdateDayLinePos;
      inquiry.m_iPriority = 10;
      m_qFinnhubWebInquiry.push(inquiry);
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
    inquiry.m_lStockIndex = m_lCurrentRTDataQuotePos;
    inquiry.m_iPriority = 10;
    m_qFinnhubWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    TRACE("����%sʵʱ����\n", m_vAmericaStake.at(m_lCurrentRTDataQuotePos)->m_strSymbol.GetBuffer());
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
      inquiry.m_lStockIndex = m_lCurrentUpdatePeerPos;
      inquiry.m_iPriority = 10;
      m_qFinnhubWebInquiry.push(inquiry);
      m_fFinnhubInquiring = true;
      m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->SetDayLineNeedUpdate(false);
      TRACE("����%s Peer����\n", m_vAmericaStake.at(m_lCurrentUpdatePeerPos)->m_strSymbol.GetBuffer());
    }
    else {
      m_fPeerUpdated = true;
      TRACE("Finnhub Peers�������\n");
      str = _T("�����г�ͬҵ��˾���ݸ������");
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
      inquiry.m_lStockIndex = m_lCurrentUpdateEPSSurprisePos;
      inquiry.m_iPriority = 10;
      m_qFinnhubWebInquiry.push(inquiry);
      m_fFinnhubInquiring = true;
      m_vAmericaStake.at(m_lCurrentUpdateEPSSurprisePos)->SetEPSSurpriseNeedUpdate(false);
      TRACE("����%s EPS Surprise����\n", m_vAmericaStake.at(m_lCurrentUpdateEPSSurprisePos)->m_strSymbol.GetBuffer());
    }
    else {
      m_fEPSSurpriseUpdated = true;
      TRACE("Finnhub EPS Surprise�������\n");
      str = _T("Finnhub EPS Surprise�������");
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
    inquiry.m_lStockIndex = m_lCurrentForexExchangePos++;
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
      inquiry.m_lStockIndex = m_lCurrentUpdateForexDayLinePos;
      inquiry.m_iPriority = 10;
      m_qFinnhubWebInquiry.push(inquiry);
      m_fFinnhubInquiring = true;
      m_vAmericaStake.at(m_lCurrentUpdateForexDayLinePos)->SetDayLineNeedUpdate(false);
      TRACE("����Forex%s��������\n", m_vForexSymbol.at(m_lCurrentUpdateForexDayLinePos)->m_strSymbol.GetBuffer());
    }
    else {
      m_fForexDayLineUpdated = true;
      TRACE("Finnhub Forex���߸������\n");
      str = _T("�����г�Forex������ʷ���ݸ������");
      gl_systemMessage.PushInformationMessage(str);
    }
  }
  return true;
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
bool CAmericaMarket::TaskUpdateDayLineDB(void) {
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

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��Forex�������ݴ������ݿ⣮
//  �˺����ɹ����߳�ThreadForexDayLineSaveProc���ã�������Ҫʹ��ȫ�ֱ�����(Ŀǰʹ�����̵߳���֮��������ͬ������ĳ��֣�
//
// �����Ƿ�ִ���˴洢����������Ҫ�����ص�������ʷ����ɾ���������ܹ���ʡ�ڴ��ռ�á�����ʵ�ʴ洢����ʹ���߳�ģʽʵ�֣�
// �ʶ���ִ��ʱ������������̣߳��������߳�ɾ����������ʱ����ͬ�����⡣����ķ������ù����̶߳���ɾ���洢����������ݣ�
// ���̵߳�ɾ������ֻ�ڲ����ù����̣߳�����洢�������ݣ�������·���ִ�С�
//////////////////////////////////////////////////////////////////////////////////////////
bool CAmericaMarket::TaskUpdateForexDayLineDB(void) {
  CString str;

  for (auto& pSymbol : m_vForexSymbol) {
    if (pSymbol->IsDayLineNeedSavingAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
      if (pSymbol->GetDayLineSize() > 0) {
        if (pSymbol->HaveNewDayLineData()) {
          RunningThreadUpdateForexDayLineDB(pSymbol);
          TRACE("����%s��������\n", pSymbol->GetSymbol().GetBuffer());
        }
        else pSymbol->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
      }
      else { // �������Ϊ�й�Ʊ���룬���˴�����δ����
        CString str1 = pSymbol->GetSymbol();
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

bool CAmericaMarket::TaskUpdateCountryListDB(void) {
  RunningThreadUpdateCountryListDB();
  return false;
}

bool CAmericaMarket::TaskUpdateEPSSurpriseDB(void) {
  CString str;

  for (auto& pStake : m_vAmericaStake) {
    if (pStake->IsEPSSurpriseNeedSaveAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
      RunningThreadUpdateEPSSurpriseDB(pStake);
      TRACE("����%s EPS surprise����\n", pStake->GetSymbol().GetBuffer());
    }
    if (gl_fExitingSystem) {
      break; // ������������˳�����ֹͣ�洢��
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
      str = _T("�����г���ʼ�����");
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
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CAmericaMarket::RunningTaskThreadUpdateStakeDB(void) {
  thread thread1(ThreadUpdateStakeDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CAmericaMarket::RunningThreadUpdateForexDayLineDB(CForexSymbolPtr pSymbol) {
  thread thread1(ThreadUpdateForexDayLineDB, pSymbol);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CAmericaMarket::RunningThreadUpdateForexSymbolDB() {
  thread thread1(ThreadUpdateForexSymbolDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CAmericaMarket::RunningThreadUpdateCountryListDB(void) {
  thread thread1(ThreadUpdateCountryListDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CAmericaMarket::RunningThreadUpdateEPSSurpriseDB(CAmericaStakePtr pStake) {
  thread thread1(ThreadUpdateEPSSurpriseDB, pStake);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CAmericaMarket::IsAmericaStake(CString strSymbol) {
  if (m_mapAmericaStake.find(strSymbol) == m_mapAmericaStake.end()) { // �´��룿
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
    if (m_mapEconomicCalendar.find(strSymbol) == m_mapEconomicCalendar.end()) { // ���¼���
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

  setAmericaStake.m_strSort = _T("[Symbol]");
  setAmericaStake.Open();
  setAmericaStake.m_pDatabase->BeginTrans();
  while (!setAmericaStake.IsEOF()) {
    pAmericaStake = make_shared<CAmericaStake>();
    pAmericaStake->Load(setAmericaStake);
    pAmericaStake->CheckEPSSurpriseStatus(GetFormatedMarketDate());
    m_vAmericaStake.push_back(pAmericaStake);
    m_mapAmericaStake[setAmericaStake.m_Symbol] = m_lLastTotalAmericaStake++;
    setAmericaStake.MoveNext();
  }
  setAmericaStake.m_pDatabase->CommitTrans();
  setAmericaStake.Close();
  m_lLastTotalAmericaStake = m_lTotalAmericaStake = m_vAmericaStake.size();
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
  m_lLastTotalAmericaStake = lTotalAmericaStake;
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

bool CAmericaMarket::RebuildEPSSurprise(void) {
  DeleteEPSSurpriseDB();

  for (auto& p : m_vAmericaStake) {
    p->m_lLastEPSSurpriseUpdateDate = 19800101;
    p->m_fEPSSurpriseNeedUpdate = true;
  }
  m_fEPSSurpriseUpdated = false;
  return true;
}

void CAmericaMarket::DeleteEPSSurpriseDB(void) {
  CDatabase database;

  if (gl_fTestMode) {
    ASSERT(0); // ���ڴ���ʵ�����ݿ⣬�ʲ��������Դ˺���
    exit(1);
  }

  database.Open(_T("AmericaMarket"), FALSE, FALSE, _T("ODBC;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
  database.BeginTrans();
  database.ExecuteSQL(_T("TRUNCATE `americamarket`.`eps_surprise`;"));
  database.CommitTrans();
  database.Close();
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

bool CAmericaMarket::DeleteStakeSymbolDB(void) {
  CDatabase database;

  if (gl_fTestMode) {
    ASSERT(0); // ���ڴ���ʵ�����ݿ⣬�ʲ��������Դ˺���
    exit(1);
  }

  database.Open(_T("AmericaMarket"), FALSE, FALSE, _T("ODBC;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
  database.BeginTrans();
  database.ExecuteSQL(_T("TRUNCATE `americamarket`.`companyprofile`;"));
  database.CommitTrans();
  database.Close();

  return true;
}