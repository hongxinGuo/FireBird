#include "AmericaMarket.h"
#include"thread.h"

#include"WebInquirer.h"
#include"ProcessFinnhubWebData.h"
#include"ProcessTiingoWebData.h"
#include"EPSSurprise.h"

#include"SetAmericaMarketOption.h"
#include"SetForexExchange.h"
#include"SetAmericaStake.h"
#include"SetAmericaChoicedStock.h"
#include"SetCountry.h"
#include"SetEconomicCalendar.h"
#include"SetEPSSurprise.h"

Semaphore gl_SaveAmericaStockDayLine(1);  // ���ź�����������������Ʊ������ʷ���ݿ�
Semaphore gl_SaveEPSSurprise(1);  // ���ź������ڸ���EPSSurprise����
Semaphore gl_SaveForexDayLine(1);  // ���ź�����������Forex������ʷ���ݿ�

CAmericaMarket::CAmericaMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CAmericaMarket�г�����ֻ�������һ��ʵ��\n");
  }

  // ������ÿ������״̬�ı�������������ʱ��ҪԤ��״̬
  m_fFinnhubEPSSurpriseUpdated = true;
  m_fFinnhubPeerUpdated = true;
  m_lCurrentUpdatePeerPos = 0;
  m_lCurrentUpdateEPSSurprisePos = 0;

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

  m_vTiingoInquiringStr.at(__COMPANY_PROFILE__) = _T("https://api.tiingo.com/tiingo/fundamentals/");
  m_vTiingoInquiringStr.at(__COMPANY_SYMBOLS__) = _T("https://api.tiingo.com/tiingo/fundamentals/meta?"); // ���ô��뼯
  m_vTiingoInquiringStr.at(__STOCK_CANDLES__) = _T("https://api.tiingo.com/tiingo/daily/");
}

CAmericaMarket::~CAmericaMarket() {
}

void CAmericaMarket::Reset(void) {
  ResetFinnhub();
  ResetTiingo();
}

void CAmericaMarket::ResetFinnhub(void) {
  m_lLastTotalAmericaStake = 0;
  m_lCurrentProfilePos = 0;
  m_lCurrentUpdateDayLinePos = 0;
  m_lCurrentRTDataQuotePos = 0;
  m_lCurrentForexExchangePos = 0;
  m_lCurrentForexSymbolPos = 0;
  m_lChoicedStockPos = 0;

  m_CurrentFinnhubInquiry.Reset();
  while (m_qFinnhubWebInquiry.size() > 0) m_qFinnhubWebInquiry.pop();

  // Finnhub�������������ݱ�ʶ��ÿ����Ҫ���á�
  m_fFinnhubSymbolUpdated = false; // ÿ����Ҫ���´���
  m_fAmericaStakeUpdated = false;
  m_fFinnhubDayLineUpdated = false;
  m_vAmericaStake.resize(0);
  m_mapAmericaStake.clear();
  m_vAmericaChoicedStake.resize(0);
  m_mapAmericaChoicedStake.clear();
  m_fFinnhubForexExhangeUpdated = false;
  m_vForexExchange.resize(0);
  m_mapForexExchange.clear();
  m_fFinnhubForexSymbolUpdated = false;
  m_vForexSymbol.resize(0);
  m_mapForexSymbol.clear();
  m_fForexDayLineUpdated = false;
  m_vCountry.resize(0);
  m_mapCountry.clear();
  m_fCountryListUpdated = false;
  m_vEconomicCalendar.resize(0);
  m_mapEconomicCalendar.clear();
  m_fFinnhubEconomicCalendarUpdated = false;

  m_fFinnhubInquiring = false;
  m_fFinnhubDataReceived = true;

  m_fRebulidDayLine = false;
  SetSystemReady(false); // �г���ʼ״̬Ϊδ���úá�

  m_lLastTotalForexExchange = 0;
  m_lLastTotalForexSymbol = 0;
  m_lCurrentUpdateForexDayLinePos = 0;

  m_lLastTotalCountry = 0;
  m_lLastTotalEconomicCalendar = 0;

  if (GetDayOfWeek() == 6) { // ÿ�ܵ�����������һ��Peer��EPSSurprise
    m_lCurrentUpdatePeerPos = 0;
    m_lCurrentUpdateEPSSurprisePos = 0;
    m_fFinnhubEPSSurpriseUpdated = false;
    m_fFinnhubPeerUpdated = false;
  }
}

void CAmericaMarket::ResetQuandl(void) {
}

void CAmericaMarket::ResetTiingo(void) {
  m_fTiingoInquiring = false;
  m_fTiingoDataReceived = true;

  m_fTiingoSymbolUpdated = false;
  m_fTiingoDayLineUpdated = false;
}

void CAmericaMarket::ResetMarket(void) {
  Reset();

  LoadOption();
  LoadCountryList();
  LoadAmericaStake();
  LoadAmericaChoicedStock();
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
  static int s_iCountfinnhubLimit = 11; // Finnhub.ioÿ1.1����������һ�Σ��Է�ֹ����Ƶ�ʹ��ߵ������
  static int s_iCountTiingoLimit = 80; // ��֤ÿ80��ִ��һ�Σ���8��ÿ�Σ�.Tiingo����˻��ٶ�����ΪÿСʱ500�Σ� ÿ����9�Σ���ÿ��8�뼴�ɡ�
  const long lCurrentTime = GetFormatedMarketTime();

  TaskCheckSystemReady();

  if (--s_iCountfinnhubLimit < 0) {
    TaskInquiryFinnhub(lCurrentTime);
    if (m_fFinnhubInquiring) {
      s_iCountfinnhubLimit = 11; // ����������������ݣ������ü��������Ա�������һ�Ρ�
    }
  }

  ProcessFinnhubWebDataReceived(); // Ҫ�ȴ����յ���Finnhub��������
  ProcessFinnhubInquiringMessage(); // Ȼ�������봦����һ��

  if (--s_iCountTiingoLimit < 0) {
    s_iCountTiingoLimit = 80;
    TaskInquiryTiingo();
  }

  ProcessTiingoWebDataReceived(); // Ҫ�ȴ����յ���Tiingo��������
  ProcessTiingoInquiringMessage(); // Ȼ�������봦����һ��

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
  CAmericaStakePtr pStock = nullptr;
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
      m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_fFinnhubPeerUpdated = true;
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
      pStock = m_vAmericaStake.at(m_lCurrentRTDataQuotePos);
      strMiddle = pStock->GetSymbol();
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      break;
      case __STOCK_CANDLES__:
      pStock = m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      strMiddle = pStock->GetFinnhubDayLineInquiryString(GetMarketTime());
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      pStock->m_fDayLineNeedUpdate = false;
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
      SetFinnhubDataReceived(false); // ���ô˱�ʶ��Ҫ�������������̣߳�GetWebData��֮ǰ���������߳��еĶ������׳���
      gl_pFinnhubWebInquiry->GetWebData();
    }
  }

  return true;
}

bool CompareAmericaStake2(CAmericaStakePtr p1, CAmericaStakePtr p2) { return (p1->m_strSymbol.Compare(p2->m_strSymbol) < 0); }

//////////////////////////////////////////////
//
// �������߳̽��յ���Finnhub������Ϣ��
//
// Ŀǰֻ����ͬʱ����һ����Ϣ������Ϣ��������������֮ǰ������������һ����Ϣ�������ܹ���֤ͬһ�ԡ�������Finnhub��վ���ٶ����ƣ�
// ÿ����ֻ����60�����룬�ʶ�û�б�Ҫǿ�������ٶȡ�
//
//////////////////////////////////////////////
bool CAmericaMarket::ProcessFinnhubWebDataReceived(void) {
  CWebDataPtr pWebData = nullptr;
  CAmericaStakePtr pStock = nullptr;
  CString str = _T("");
  vector<CString> vExchange;
  vector<CForexSymbolPtr> vForexSymbol;
  vector<CEconomicCalendarPtr> vEconomicCalendar;
  vector<CEPSSurprisePtr> vEPSSurprise;
  vector<CAmericaStakePtr> vStake;
  vector<CDayLinePtr> vDayLine;
  vector<CCountryPtr> vCountry;
  long lTemp = 0;
  bool fFoundNewStock = false;

  ASSERT(gl_WebInquirer.GetFinnhubDataSize() <= 1);
  if (IsFinnhubDataReceived()) { // ����������ݽ������
    if (gl_WebInquirer.GetFinnhubDataSize() > 0) {  // ����ǰ��������
      ASSERT(m_fFinnhubInquiring);
      pWebData = gl_WebInquirer.PopFinnhubData();
      switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) {
      case __COMPANY_PROFILE__: // Ŀǰ����˻��޷�ʹ�ô˹��ܡ�
      ProcessFinnhubStockProfile(pWebData, m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex));
      break;
      case __COMPANY_PROFILE2__:
      pStock = m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      ProcessFinnhubStockProfile2(pWebData, pStock);
      pStock->m_lProfileUpdateDate = gl_pAmericaMarket->GetFormatedMarketDate();
      pStock->m_fUpdateDatabase = true;
      break;
      case  __COMPANY_SYMBOLS__:
      ProcessFinnhubStockSymbol(pWebData, vStake);
      for (auto& pStock2 : vStake) {
        if (!IsAmericaStake(pStock2->GetSymbol())) {
          m_vAmericaStake.push_back(pStock2);
          fFoundNewStock = true;
        }
      }
      if (fFoundNewStock) {
        sort(m_vAmericaStake.begin(), m_vAmericaStake.end(), CompareAmericaStake2);
        m_mapAmericaStake.clear();
        int j = 0;
        for (auto& pStake2 : m_vAmericaStake) {
          m_mapAmericaStake[pStake2->m_strSymbol] = j++;
        }
        gl_systemMessage.PushInformationMessage("Finnhub�����´��룬���´��뼯");
      }
      m_fFinnhubSymbolUpdated = true;
      break;
      case  __MARKET_NEWS__:
      break;
      case __COMPANY_NEWS__:
      break;
      case __NEWS_SENTIMENT__:
      break;
      case __PEERS__:
      pStock = m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      ProcessFinnhubStockPeer(pWebData, pStock);
      pStock->m_fUpdateDatabase = true;
      break;
      case __BASIC_FINANCIALS__:
      break;
      case __STOCK_EPS_SURPRISE__:
      pStock = m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      if (ProcessFinnhubEPSSurprise(pWebData, vEPSSurprise)) {
        if (vEPSSurprise.size() > 0) {
          pStock->UpdateEPSSurprise(vEPSSurprise);
        }
        else {
          pStock->m_lLastEPSSurpriseUpdateDate = 19700101; // ����������Ϊ���硣
          pStock->m_fUpdateDatabase = true; // ���´��뼯
        }
        pStock->m_fEPSSurpriseNeedUpdate = false;
        pStock->m_fEPSSurpriseNeedSave = true;
      }
      break;
      case __STOCK_QUOTE__:
      pStock = m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      ProcessFinnhubStockQuote(pWebData, pStock);
      if ((pStock->GetTransactionTime() + 3600 * 12 - GetMarketTime()) > 0) { // ����ʱ�䲻����12Сʱ�������ô˹�ƱΪ��Ծ��Ʊ
        pStock->SetActive(true);
        if (!pStock->IsIPOed()) {
          pStock->SetIPOStatus(__STAKE_IPOED__);
          pStock->m_fUpdateDatabase = true;
        }
      }
      break;
      case __STOCK_CANDLES__:
      pStock = m_vAmericaStake.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      if (ProcessFinnhubStockCandle(pWebData, pStock)) {
        if (pStock->GetDayLineSize() == 0) { // û���������ݣ�
          if (pStock->IsNotChecked()) { // ��δȷ��������Ч�ԣ�
            pStock->SetIPOStatus(__STAKE_NULL__);
          }
        }
        else if (IsEarlyThen(pStock->GetDayLine(pStock->GetDayLineSize() - 1)->GetFormatedMarketDate(), GetFormatedMarketDate(), 100)) {
          pStock->SetIPOStatus(__STAKE_DELISTED__);
        }
        else {
          pStock->SetIPOStatus(__STAKE_IPOED__);
        }
        TRACE("����%s��������\n", pStock->m_strSymbol.GetBuffer());
      }
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
      m_fFinnhubForexExhangeUpdated = true;
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
      break;
      case __FOREX_CANDLES__:
      ProcessFinnhubForexCandle(pWebData, m_vForexSymbol.at(m_CurrentFinnhubInquiry.m_lStockIndex));
      TRACE("����%s��������\n", m_vForexSymbol.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_strSymbol.GetBuffer());
      break;
      case __FOREX_ALL_RATES__:
      break;
      case __ECONOMIC_COUNTRY_LIST__:
      ProcessFinnhubCountryList(pWebData, vCountry);
      for (auto& pCountry : vCountry) {
        if (m_mapCountry.find(pCountry->m_strCountry) == m_mapCountry.end()) {
          m_mapCountry[pCountry->m_strCountry] = m_vCountry.size();
          m_vCountry.push_back(pCountry);
        }
      }
      m_fCountryListUpdated = true;
      break;
      case __ECONOMIC_CALENDAR__:
      ProcessFinnhubEconomicCalendar(pWebData, vEconomicCalendar);
      UpdateEconomicCalendar(vEconomicCalendar);
      m_fFinnhubEconomicCalendarUpdated = true;
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
  CAmericaStakePtr pStock = nullptr;
  CForexSymbolPtr pSymbol = nullptr;

  if (m_qTiingoWebInquiry.size() > 0) { // ������ȴ���
    ASSERT(m_fTiingoInquiring);
    if (IsTiingoDataReceived()) { //�Ѿ�����������������Tiingo�����Ѿ����յ��ˣ�
      m_CurrentTiingoInquiry = m_qTiingoWebInquiry.top();
      m_qTiingoWebInquiry.pop();
      gl_pTiingoWebInquiry->SetInquiryingStrPrefix(m_vTiingoInquiringStr.at(m_CurrentTiingoInquiry.m_lInquiryIndex)); // ����ǰ׺
      switch (m_CurrentTiingoInquiry.m_lInquiryIndex) { // ���ݲ�ͬ��Ҫ��������׺�ַ���
      case __COMPANY_PROFILE__: // Premium ����˻��޷���ȡ����Ϣ��sandboxģʽ�ܶ�ȡ�����Ǵ���ģ�ֻ�����ڲ��ԡ�
      break;
      case __COMPANY_PROFILE2__:
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
      break;
      case __STOCK_EARNING_CALENDER__:
      break;
      case __STOCK_QUOTE__:
      break;
      case __STOCK_CANDLES__:
      pStock = m_vAmericaStake.at(m_CurrentTiingoInquiry.m_lStockIndex);
      strMiddle = pStock->GetTiingoDayLineInquiryString(GetFormatedMarketDate());
      gl_pTiingoWebInquiry->SetInquiryingStringMiddle(strMiddle);
      pStock->m_fDayLineNeedUpdate = false;
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
      break;
      case __FOREX_CANDLES__:
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
      TRACE("Tiingoδ����ָ��%d\n", m_CurrentTiingoInquiry.m_lInquiryIndex);
      break;
      }
      SetTiingoDataReceived(false); // ���ô˱�ʶ��Ҫ�������������̣߳�GetWebData��֮ǰ���������߳��еĶ������׳���
      gl_pTiingoWebInquiry->GetWebData();
    }
  }

  return true;
}

bool CAmericaMarket::ProcessTiingoWebDataReceived(void) {
  CWebDataPtr pWebData = nullptr;
  CAmericaStakePtr pStock = nullptr;
  CString str = _T("");
  vector<CString> vExchange;
  vector<CForexSymbolPtr> vForexSymbol;
  vector<CEconomicCalendarPtr> vEconomicCalendar;
  vector<CEPSSurprisePtr> vEPSSurprise;
  vector<CAmericaStakePtr> vStake;
  long lTemp = 0;
  bool fFoundNewStock = false;

  ASSERT(gl_WebInquirer.GetTiingoDataSize() <= 1);
  if (IsTiingoDataReceived()) { // ����������ݽ������
    if (gl_WebInquirer.GetTiingoDataSize() > 0) {  // ����ǰ��������
      ASSERT(m_fTiingoInquiring);
      pWebData = gl_WebInquirer.PopTiingoData();
      switch (m_CurrentTiingoInquiry.m_lInquiryIndex) {
      case __COMPANY_PROFILE__: // Ŀǰ����˻��޷�ʹ�ô˹��ܡ�
      break;
      case __COMPANY_PROFILE2__:
      break;
      case  __COMPANY_SYMBOLS__:
      if (ProcessTiingoStockSymbol(pWebData, vStake)) {
        for (auto& pStock2 : vStake) {
          if (pStock2->m_fIsActive && (IsAmericaStake(pStock2->m_strSymbol))) { // Tiingo��Symbol��Ϣֻ������Finnhub��һ�����䡣
            lTemp++;
            pStock = m_vAmericaStake.at(m_mapAmericaStake.at(pStock2->m_strSymbol));
            pStock->m_strTiingoPermaTicker = pStock2->m_strTiingoPermaTicker;
            pStock->m_fIsActive = pStock2->m_fIsActive;
            pStock->m_fIsADR = pStock2->m_fIsADR;
            pStock->m_strTiingoIndustry = pStock2->m_strTiingoIndustry;
            pStock->m_strTiingoSector = pStock2->m_strTiingoSector;
            pStock->m_strSICIndustry = pStock2->m_strSICIndustry;
            pStock->m_strSICSector = pStock2->m_strSICSector;
            pStock->m_iSICCode = pStock2->m_iSICCode;
            pStock->m_strCompanyWebSite = pStock2->m_strCompanyWebSite;
            pStock->m_strSECFilingWebSite = pStock2->m_strSECFilingWebSite;
            pStock->m_lDailyDataUpdateDate = pStock2->m_lDailyDataUpdateDate;
            pStock->m_lStatementUpdateDate = pStock2->m_lStatementUpdateDate;
            pStock->m_fUpdateDatabase = true;
          }
        }
      }
      m_fTiingoSymbolUpdated = true;
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
      case __STOCK_EPS_SURPRISE__:
      break;
      case __STOCK_QUOTE__:
      break;
      case __STOCK_CANDLES__:
      pStock = m_vAmericaStake.at(m_CurrentTiingoInquiry.m_lStockIndex);
      ProcessTiingoStockDayLine(pWebData, pStock);
      TRACE("����Tiingo %s��������\n", pStock->m_strSymbol.GetBuffer());
      break;
      case __FOREX_EXCHANGE__:
      break;
      case __FOREX_SYMBOLS__:
      break;
      case __FOREX_CANDLES__:
      break;
      case __FOREX_ALL_RATES__:
      break;
      case __ECONOMIC_COUNTRY_LIST__:
      break;
      case __ECONOMIC_CALENDAR__:
      default:
      break;
      }
      m_fTiingoInquiring = false;
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

  return true;
}

bool CAmericaMarket::SchedulingTaskPer10Seconds(long lCurrentTime) {
  return true;
}

bool CAmericaMarket::SchedulingTaskPer1Minute(long lCurrentTime) {
  TaskResetMarket(lCurrentTime);

  if (m_lLastTotalCountry < m_vCountry.size()) {
    TaskUpdateCountryListDB();
  }
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
// �˺�����SchedulingTask���ȣ�ÿ1500��������ִ��һ�Ρ�
//
//////////////////////////////////////////////////////////////////////////////////////
bool CAmericaMarket::TaskInquiryFinnhub(long lCurrentTime) {
  if (((lCurrentTime < 165700) || (lCurrentTime > 170300))) { // ������ʱ����ϵͳ���ʶ���ʱ���������������Ϣ��
    TaskInquiryFinnhubCompanySymbol(); // ��һ���������������뵱��֤ȯ����
    TaskInquiryFinnhubCountryList();
    TaskInquiryFinnhubForexExchange();
    TaskInquiryFinnhubForexSymbol();
    //TaskInquiryFinnhubEconomicCalender();

    // ����Finnhub������Ϣ�����񣬽�Ҫ����������Ա�֤���г�ʱ���賿ʮ���Ӻ�ִ�С������ܹ���֤�������г�ʱû��ִ�в�ѯ����
    if (IsSystemReady() && !m_fFinnhubInquiring) {
      TaskInquiryFinnhubCompanyProfile2();
      TaskInquiryFinnhubDayLine();
      TaskInquiryFinnhubEPSSurprise();
      TaskInquiryFinnhubPeer();
      TaskInquiryFinnhubForexDayLine();
      if (m_fFinnhubDayLineUpdated) {
        //TaskInquiryFinnhubRTQuote();
      }
    }
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

  if (!m_fFinnhubSymbolUpdated && !m_fFinnhubInquiring) {
    inquiry.m_lInquiryIndex = __COMPANY_SYMBOLS__;
    inquiry.m_iPriority = 10;
    m_qFinnhubWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    str = _T("��ѯFinnhub Company Symbol");
    gl_systemMessage.PushInformationMessage(str);

    return true;
  }
  return false;
}

bool CAmericaMarket::TaskUpdateForexSymbolDB(void) {
  RunningThreadUpdateForexSymbolDB();
  return false;
}

bool CAmericaMarket::TaskUpdateForexExchangeDB(void) {
  CSetForexExchange setForexExchange;

  if (m_lLastTotalForexExchange < m_vForexExchange.size()) {
    setForexExchange.Open();
    setForexExchange.m_pDatabase->BeginTrans();
    for (long l = m_lLastTotalForexExchange; l < m_vForexExchange.size(); l++) {
      setForexExchange.AddNew();
      setForexExchange.m_Exchange = m_vForexExchange.at(l);
      setForexExchange.Update();
    }
    setForexExchange.m_pDatabase->CommitTrans();
    setForexExchange.Close();
    m_lLastTotalForexExchange = m_vForexExchange.size();
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
  CAmericaStakePtr pStock;
  CString str = _T("");
  long lStakeSetSize = m_vAmericaStake.size();

  ASSERT(IsSystemReady());
  if (!m_fFinnhubDayLineUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < lStakeSetSize; m_lCurrentUpdateDayLinePos++) {
      if (m_vAmericaStake.at(m_lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
        pStock = m_vAmericaStake.at(m_lCurrentUpdateDayLinePos);
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
      m_fFinnhubDayLineUpdated = true;
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
  CAmericaStakePtr pStock;
  CString str = _T("");
  long lStakeSetSize = m_vAmericaStake.size();

  ASSERT(IsSystemReady());
  if (!m_fFinnhubPeerUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentUpdatePeerPos = 0; m_lCurrentUpdatePeerPos < lStakeSetSize; m_lCurrentUpdatePeerPos++) {
      if (!m_vAmericaStake.at(m_lCurrentUpdatePeerPos)->m_fFinnhubPeerUpdated) {
        pStock = m_vAmericaStake.at(m_lCurrentUpdatePeerPos);
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
      TRACE("����%s Peer����\n", m_vAmericaStake.at(m_lCurrentUpdatePeerPos)->m_strSymbol.GetBuffer());
    }
    else {
      m_fFinnhubPeerUpdated = true;
      TRACE("Finnhub Peers�������\n");
      str = _T("�����г�ͬҵ��˾���ݸ������");
      gl_systemMessage.PushInformationMessage(str);
    }
  }
  return true;
}

bool CAmericaMarket::TaskInquiryFinnhubEconomicCalender(void) {
  WebInquiry inquiry{ 0, 0, 0 };
  if (!m_fFinnhubEconomicCalendarUpdated && !m_fFinnhubInquiring) {
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
  CAmericaStakePtr pStock;
  CString str = _T("");
  long lStakeSetSize = m_vAmericaStake.size();

  ASSERT(IsSystemReady());
  if (!m_fFinnhubEPSSurpriseUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentUpdateEPSSurprisePos = 0; m_lCurrentUpdateEPSSurprisePos < lStakeSetSize; m_lCurrentUpdateEPSSurprisePos++) {
      if (m_vAmericaStake.at(m_lCurrentUpdateEPSSurprisePos)->IsEPSSurpriseNeedUpdate()) {
        pStock = m_vAmericaStake.at(m_lCurrentUpdateEPSSurprisePos);
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
      m_fFinnhubEPSSurpriseUpdated = true;
      TRACE("Finnhub EPS Surprise�������\n");
      str = _T("Finnhub EPS Surprise�������");
      gl_systemMessage.PushInformationMessage(str);
    }
  }
  return true;
}

bool CAmericaMarket::TaskInquiryFinnhubForexExchange(void) {
  WebInquiry inquiry{ 0, 0, 0 };
  if (!m_fFinnhubForexExhangeUpdated && !m_fFinnhubInquiring) {
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
  if (!m_fFinnhubForexSymbolUpdated && !m_fFinnhubInquiring) {
    inquiry.m_lInquiryIndex = __FOREX_SYMBOLS__;
    inquiry.m_lStockIndex = m_lCurrentForexExchangePos++;
    inquiry.m_iPriority = 10;
    m_qFinnhubWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    if (m_lCurrentForexExchangePos >= m_vForexExchange.size()) m_fFinnhubForexSymbolUpdated = true;
    return true;
  }
  return false;
}

bool CAmericaMarket::TaskInquiryFinnhubForexDayLine(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
  CForexSymbolPtr pStock;
  CString str = _T("");
  const long lStakeSetSize = m_vForexSymbol.size();

  ASSERT(IsSystemReady());
  if (!m_fForexDayLineUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentUpdateForexDayLinePos = 0; m_lCurrentUpdateForexDayLinePos < lStakeSetSize; m_lCurrentUpdateForexDayLinePos++) {
      if (m_vForexSymbol.at(m_lCurrentUpdateForexDayLinePos)->IsDayLineNeedUpdate()) {
        pStock = m_vForexSymbol.at(m_lCurrentUpdateForexDayLinePos);
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
      m_vForexSymbol.at(m_lCurrentUpdateForexDayLinePos)->SetDayLineNeedUpdate(false);
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

void CAmericaMarket::TaskInquiryTiingo(void) {
  if (IsSystemReady() && !m_fTiingoInquiring) {
    TaskInquiryTiingoCompanySymbol();
    // ����Tiingo�涨ÿ��ֻ�ܲ�ѯ500�����룬�ʲ��Գɹ�����ʱ��ʹ�á�
    TaskInquiryTiingoDayLine(); // ����������ϡ�
  }
}

bool CAmericaMarket::TaskInquiryTiingoCompanySymbol(void) {
  WebInquiry inquiry{ 0, 0, 0 };
  CString str;

  if (!m_fTiingoSymbolUpdated && !m_fTiingoInquiring) {
    inquiry.m_lInquiryIndex = __COMPANY_SYMBOLS__;
    inquiry.m_iPriority = 10;
    m_qTiingoWebInquiry.push(inquiry);
    m_fTiingoInquiring = true;
    str = _T("��ѯTiingo Company Symbol");
    gl_systemMessage.PushInformationMessage(str);

    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo����������������Finnhub���������غ�����ֻ����ͬʱ��������֮һ��
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CAmericaMarket::TaskInquiryTiingoDayLine(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
  CAmericaStakePtr pStock;
  CString str = _T("");
  long lStakeSetSize = m_vAmericaChoicedStake.size();

  ASSERT(IsSystemReady());
  if (!m_fTiingoDayLineUpdated && !m_fTiingoInquiring) {
    for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < lStakeSetSize; m_lCurrentUpdateDayLinePos++) {
      if (m_vAmericaChoicedStake.at(m_lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
        pStock = m_vAmericaChoicedStake.at(m_lCurrentUpdateDayLinePos);
        fFound = true;
        break;
      }
    }
    if (fFound) {
      inquiry.m_lInquiryIndex = __STOCK_CANDLES__;
      inquiry.m_lStockIndex = m_mapAmericaStake.at(pStock->GetSymbol());
      inquiry.m_iPriority = 10;
      m_qTiingoWebInquiry.push(inquiry);
      m_fTiingoInquiring = true;
      m_vAmericaChoicedStake.at(m_lCurrentUpdateDayLinePos)->SetDayLineNeedUpdate(false);
      TRACE("����Tiingo %s��������\n", m_vAmericaChoicedStake.at(m_lCurrentUpdateDayLinePos)->m_strSymbol.GetBuffer());
    }
    else {
      m_fTiingoDayLineUpdated = true;
      TRACE("Tiingo���߸������\n");
      str = _T("�����г���ѡ��Ʊ������ʷ���ݸ������");
      gl_systemMessage.PushInformationMessage(str);
    }
  }
  return true;
}

bool CAmericaMarket::TaskUpdateDayLineDB(void) {
  RunningThreadUpdateDayLineDB();

  return true;
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
          RunningThreadUpdateForexDayLineDB(pSymbol.get());
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

  for (auto& pStock : m_vAmericaStake) {
    if (pStock->IsEPSSurpriseNeedSaveAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
      RunningThreadUpdateEPSSurpriseDB(pStock.get());
      TRACE("����%s EPS surprise����\n", pStock->GetSymbol().GetBuffer());
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

  if (m_lLastTotalEconomicCalendar < m_vEconomicCalendar.size()) {
    setEconomicCalendar.Open();
    setEconomicCalendar.m_pDatabase->BeginTrans();
    for (long l = m_lLastTotalEconomicCalendar; l < m_vEconomicCalendar.size(); l++) {
      pCalendar = m_vEconomicCalendar.at(l);
      pCalendar->Append(setEconomicCalendar);
    }
    setEconomicCalendar.m_pDatabase->CommitTrans();
    setEconomicCalendar.Close();
    m_lLastTotalEconomicCalendar = m_vEconomicCalendar.size();
  }
  return true;
}

bool CAmericaMarket::TaskCheckSystemReady(void) {
  CString str = _T("");

  if (!IsSystemReady()) {
    if (m_fFinnhubSymbolUpdated && m_fFinnhubForexExhangeUpdated && m_fFinnhubForexSymbolUpdated) {
      str = _T("�����г���ʼ�����");
      gl_systemMessage.PushInformationMessage(str);
      SetSystemReady(true);
      return true;
    }
    return false;
  }
  return true;
}

bool CAmericaMarket::TaskUpdateDayLineStartEndDate(void) {
  RunningthreadUpdateDayLneStartEndDate(this);
  return false;
}

bool CAmericaMarket::RunningthreadUpdateDayLneStartEndDate(CAmericaMarket* pMarket) {
  thread thread1(ThreadUpdateAmericaStakeDayLineStartEndDate, pMarket);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CAmericaMarket::RunningThreadUpdateDayLineDB() {
  thread thread1(ThreadUpdateAmericaStakeDayLineDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CAmericaMarket::RunningTaskThreadUpdateStakeDB(void) {
  thread thread1(ThreadUpdateStakeDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CAmericaMarket::RunningThreadUpdateForexDayLineDB(CForexSymbol* pSymbol) {
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

bool CAmericaMarket::RunningThreadUpdateEPSSurpriseDB(CAmericaStake* pStock) {
  thread thread1(ThreadUpdateEPSSurpriseDB, pStock);
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
  if (IsAmericaStake(strTicker)) {
    return m_vAmericaStake.at(m_mapAmericaStake.at(strTicker));
  }
  else return nullptr;
}

void CAmericaMarket::AddAmericaStake(CAmericaStakePtr pStock) {
  m_mapAmericaStake[pStock->m_strSymbol] = m_vAmericaStake.size();
  m_vAmericaStake.push_back(pStock);
}

bool CAmericaMarket::UpdateEconomicCalendar(vector<CEconomicCalendarPtr> vEconomicCalendar) {
  CString strSymbol = _T("");

  for (auto& pEconomicCalendar : vEconomicCalendar) {
    strSymbol = pEconomicCalendar->m_strCountry + pEconomicCalendar->m_strEvent + pEconomicCalendar->m_strTime;
    if (m_mapEconomicCalendar.find(strSymbol) == m_mapEconomicCalendar.end()) { // ���¼���
      m_mapEconomicCalendar[strSymbol] = m_vEconomicCalendar.size();
      m_vEconomicCalendar.push_back(pEconomicCalendar);
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

bool CAmericaMarket::LoadOption(void) {
  CSetAmericaMarketOption setOption;

  setOption.Open();
  if (setOption.m_FinnhubToken.GetLength() > 5) {
    gl_pFinnhubWebInquiry->SetInquiryingStringSuffix(setOption.m_FinnhubToken);
  }
  if (setOption.m_TiingoToken.GetLength() > 5) {
    gl_pTiingoWebInquiry->SetInquiryingStringSuffix(setOption.m_TiingoToken);
  }
  if (setOption.m_QuandlToken.GetLength() > 5) {
    gl_pQuandlWebInquiry->SetInquiryingStringSuffix(setOption.m_QuandlToken);
  }
  setOption.Close();

  return true;
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
    pAmericaStake->CheckDayLineUpdateStatus(GetFormatedMarketDate(), GetLastTradeDate(), GetFormatedMarketTime(), GetDayOfWeek());
    pAmericaStake->CheckEPSSurpriseStatus(GetFormatedMarketDate());
    m_vAmericaStake.push_back(pAmericaStake);
    m_mapAmericaStake[setAmericaStake.m_Symbol] = m_lLastTotalAmericaStake++;
    setAmericaStake.MoveNext();
  }
  setAmericaStake.m_pDatabase->CommitTrans();
  setAmericaStake.Close();
  m_lLastTotalAmericaStake = m_vAmericaStake.size();
  ASSERT(m_lLastTotalAmericaStake == m_vAmericaStake.size());
  return true;
}

bool CAmericaMarket::LoadAmericaChoicedStock(void) {
  CSetAmericaChoicedStock setAmericaChoicedStock;
  CAmericaStakePtr pStock = nullptr;

  setAmericaChoicedStock.Open();
  setAmericaChoicedStock.m_pDatabase->BeginTrans();
  while (!setAmericaChoicedStock.IsEOF()) {
    if (IsAmericaStake(setAmericaChoicedStock.m_Symbol)) {
      pStock = GetStock(setAmericaChoicedStock.m_Symbol);
      m_mapAmericaChoicedStake[setAmericaChoicedStock.m_Symbol] = m_mapAmericaChoicedStake.size();
      m_vAmericaChoicedStake.push_back(pStock);
    }
    else {
      setAmericaChoicedStock.Delete(); // ����Ƿ���Ʊ����
    }
    setAmericaChoicedStock.MoveNext();
  }
  setAmericaChoicedStock.m_pDatabase->CommitTrans();
  setAmericaChoicedStock.Close();

  return true;
}

bool CAmericaMarket::UpdateCountryListDB(void) {
  CCountryPtr pCountry = nullptr;
  CSetCountry setCountry;

  if (m_lLastTotalCountry < m_vCountry.size()) {
    setCountry.Open();
    setCountry.m_pDatabase->BeginTrans();
    for (long l = m_lLastTotalCountry; l < m_vCountry.size(); l++) {
      pCountry = m_vCountry.at(l);
      pCountry->Append(setCountry);
    }
    setCountry.m_pDatabase->CommitTrans();
    setCountry.Close();
    m_lLastTotalCountry = m_vCountry.size();
  }
  return true;
}

bool CAmericaMarket::UpdateStakeDB(void) {
  const long lTotalAmericaStake = m_vAmericaStake.size();
  CAmericaStakePtr pStock = nullptr;
  CSetAmericaStake setAmericaStake;

  if (m_lLastTotalAmericaStake < m_vAmericaStake.size()) {
    DeleteStakeSymbolDB();
    m_lLastTotalAmericaStake = m_vAmericaStake.size();

    setAmericaStake.Open();
    setAmericaStake.m_pDatabase->BeginTrans();
    for (long l = 0; l < m_lLastTotalAmericaStake; l++) {
      pStock = m_vAmericaStake.at(l);
      pStock->Append(setAmericaStake);
      pStock->m_fUpdateDatabase = false;
    }
    setAmericaStake.m_pDatabase->CommitTrans();
    setAmericaStake.Close();
  }
  else {
    if (IsAmericaStakeUpdated()) {
      setAmericaStake.Open();
      setAmericaStake.m_pDatabase->BeginTrans();
      while (!setAmericaStake.IsEOF()) {
        if (IsAmericaStake(setAmericaStake.m_Symbol)) {
          pStock = m_vAmericaStake.at(m_mapAmericaStake.at(setAmericaStake.m_Symbol));
          if (pStock->m_fUpdateDatabase) {
            pStock->Update(setAmericaStake);
            pStock->m_fUpdateDatabase = false;
          }
        }
        setAmericaStake.MoveNext();
      }
      setAmericaStake.m_pDatabase->CommitTrans();
      setAmericaStake.Close();
    }
  }
  return true;
}

bool CAmericaMarket::UpdateForexSymbolDB(void) {
  const long lTotalForexSymbol = m_vForexSymbol.size();
  CForexSymbolPtr pSymbol = nullptr;
  CSetForexSymbol setForexSymbol;
  bool fUpdateSymbol = false;

  if (m_lLastTotalForexSymbol < lTotalForexSymbol) {
    setForexSymbol.Open();
    setForexSymbol.m_pDatabase->BeginTrans();
    for (long l = m_lLastTotalForexSymbol; l < lTotalForexSymbol; l++) {
      pSymbol = m_vForexSymbol.at(l);
      pSymbol->Append(setForexSymbol);
    }
    setForexSymbol.m_pDatabase->CommitTrans();
    setForexSymbol.Close();
    m_lLastTotalForexSymbol = lTotalForexSymbol;
  }

  for (auto& pSymbol : m_vForexSymbol) {
    if (pSymbol->m_fUpdateDatabase) {
      fUpdateSymbol = true;
      break;
    }
  }
  if (fUpdateSymbol) {
    setForexSymbol.Open();
    setForexSymbol.m_pDatabase->BeginTrans();
    while (!setForexSymbol.IsEOF()) {
      if (m_mapForexSymbol.find(setForexSymbol.m_Symbol) != m_mapForexSymbol.end()) {
        pSymbol = m_vForexSymbol.at(m_mapForexSymbol.at(setForexSymbol.m_Symbol));
        if (pSymbol->m_fUpdateDatabase) {
          pSymbol->Update(setForexSymbol);
          pSymbol->m_fUpdateDatabase = false;
        }
      }
      setForexSymbol.MoveNext();
    }
    setForexSymbol.m_pDatabase->CommitTrans();
    setForexSymbol.Close();
  }

  return true;
}

bool CAmericaMarket::UpdateEconomicCalendarDB(void) {
  const long lTotalEconomicCalendar = m_vEconomicCalendar.size();
  CEconomicCalendarPtr pEconomicCalendar = nullptr;
  CSetEconomicCalendar setEconomicCalendar;

  if (m_lLastTotalEconomicCalendar < m_vEconomicCalendar.size()) {
    setEconomicCalendar.Open();
    setEconomicCalendar.m_pDatabase->BeginTrans();
    for (long l = m_lLastTotalEconomicCalendar; l < m_vEconomicCalendar.size(); l++) {
      pEconomicCalendar = m_vEconomicCalendar.at(l);
      pEconomicCalendar->Append(setEconomicCalendar);
    }
    setEconomicCalendar.m_pDatabase->CommitTrans();
    setEconomicCalendar.Close();
    m_lLastTotalEconomicCalendar = m_vEconomicCalendar.size();
  }

  return true;
}

bool CAmericaMarket::RebulidFinnhubDayLine(void) {
  for (auto& pStock : m_vAmericaStake) {
    pStock->SetDayLineStartDate(29900101);
    pStock->SetDayLineEndDate(19800101);
    pStock->m_fUpdateDatabase = true;
  }

  m_fFinnhubDayLineUpdated = false;

  return true;
}

bool CAmericaMarket::RebuildEPSSurprise(void) {
  for (auto& p : m_vAmericaStake) {
    p->m_lLastEPSSurpriseUpdateDate = 19800101;
    p->m_fEPSSurpriseNeedUpdate = true;
  }
  m_fFinnhubEPSSurpriseUpdated = false;
  m_lCurrentUpdateEPSSurprisePos = 0;

  return true;
}

bool CAmericaMarket::ReBuildPeer(void) {
  m_fFinnhubPeerUpdated = false;
  m_lCurrentUpdatePeerPos = 0;

  return true;
}

void CAmericaMarket::DeleteEPSSurpriseDB(void) {
  CDatabase database;

  if (gl_fTestMode) {
    ASSERT(0); // ���ڴ���ʵ�����ݿ⣬�ʲ�������Դ˺���
    exit(1);
  }

  database.Open(_T("AmericaMarket"), FALSE, FALSE, _T("ODBC;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
  database.BeginTrans();
  database.ExecuteSQL(_T("TRUNCATE `americamarket`.`eps_surprise`;"));
  database.CommitTrans();
  database.Close();
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
  m_lLastTotalForexExchange = m_vForexExchange.size();

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
  m_lLastTotalForexSymbol = m_vForexSymbol.size();

  return true;
}

bool CAmericaMarket::LoadCountryList(void) {
  CSetCountry setCountry;
  CCountryPtr pCountry = nullptr;

  setCountry.m_strSort = _T("[Country]");
  setCountry.Open();
  while (!setCountry.IsEOF()) {
    pCountry = make_shared<CCountry>();
    pCountry->Load(setCountry);
    m_mapCountry[pCountry->m_strCountry] = m_vCountry.size();
    m_vCountry.push_back(pCountry);
    setCountry.MoveNext();
  }
  setCountry.Close();
  m_lLastTotalCountry = m_vCountry.size();

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
    m_mapEconomicCalendar[strSymbol] = m_vEconomicCalendar.size();
    m_vEconomicCalendar.push_back(pEconomicCalendar);
    setEconomicCalendar.MoveNext();
  }
  setEconomicCalendar.Close();
  m_lLastTotalEconomicCalendar = m_vEconomicCalendar.size();

  return true;
}

bool CAmericaMarket::DeleteStakeSymbolDB(void) {
  CDatabase database;

  if (gl_fTestMode) {
    ASSERT(0); // ���ڴ���ʵ�����ݿ⣬�ʲ�������Դ˺���
    exit(1);
  }

  database.Open(_T("AmericaMarket"), FALSE, FALSE, _T("ODBC;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
  database.BeginTrans();
  database.ExecuteSQL(_T("TRUNCATE `americamarket`.`companyprofile`;"));
  database.CommitTrans();
  database.Close();

  return true;
}

bool CAmericaMarket::DeleteStakeDayLineDB(void) {
  CDatabase database;

  if (gl_fTestMode) {
    ASSERT(0); // ���ڴ���ʵ�����ݿ⣬�ʲ�������Դ˺���
    exit(1);
  }

  database.Open(_T("AmericaMarket"), FALSE, FALSE, _T("ODBC;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
  database.BeginTrans();
  database.ExecuteSQL(_T("TRUNCATE `americamarket`.`dayline`;"));
  database.CommitTrans();
  database.Close();

  return true;
}

bool CAmericaMarket::RebuildStakeDayLineDB(void) {
  for (auto& pStock : m_vAmericaStake) {
    pStock->m_lIPOStatus = __STAKE_NOT_CHECKED__;
    pStock->m_fUpdateDatabase = true;
  }
  m_fAmericaStakeUpdated = false;

  return true;
}

bool CAmericaMarket::UpdateDayLineStartEndDate(void) {
  CString strFilterPrefix = _T("[Symbol] = '");
  CString strFilter, str;
  CSetAmericaStakeDayLine setAmericaStakeDayLine;
  CAmericaStakePtr pStock2 = nullptr;
  bool fSavedStatus = m_fFinnhubDayLineUpdated;

  m_fFinnhubDayLineUpdated = true;
  for (auto& pStock : m_vAmericaStake) {
    setAmericaStakeDayLine.m_strFilter = strFilterPrefix + pStock->m_strSymbol + _T("'");
    setAmericaStakeDayLine.m_strSort = _T("[Date]");
    setAmericaStakeDayLine.Open();
    if (!setAmericaStakeDayLine.IsEOF()) {
      if (setAmericaStakeDayLine.m_Date < pStock->m_lDayLineStartDate) {
        pStock->m_lDayLineStartDate = setAmericaStakeDayLine.m_Date;
        pStock->m_fUpdateDatabase = true;
      }
      setAmericaStakeDayLine.MoveLast();
      if (setAmericaStakeDayLine.m_Date > pStock->m_lDayLineEndDate) {
        pStock->m_lDayLineEndDate = setAmericaStakeDayLine.m_Date;
        pStock->m_fUpdateDatabase = true;
      }
    }
    setAmericaStakeDayLine.Close();
  }

  m_fFinnhubDayLineUpdated = fSavedStatus;

  return true;
}