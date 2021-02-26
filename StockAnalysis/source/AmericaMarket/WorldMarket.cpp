#include "WorldMarket.h"
#include"thread.h"

#include"WebInquirer.h"
#include"ProcessFinnhubWebData.h"
#include"ProcessTiingoWebData.h"
#include"EPSSurprise.h"

#include"SetWorldMarketOption.h"
#include"SetFinnhubForexExchange.h"
#include"SetWorldStock.h"
#include"SetWorldChoicedStock.h"
#include"SetCountry.h"
#include"SetEconomicCalendar.h"
#include"SetEPSSurprise.h"

Semaphore gl_UpdateWorldMarketDB(1);  // ���ź�����������������Ʊ������ʷ���ݿ�

CWorldMarket::CWorldMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CWorldMarket�г�����ֻ�������һ��ʵ��\n");
  }

  // ����ÿ�ո��µı�����������
  m_fFinnhubEPSSurpriseUpdated = true;
  m_lCurrentUpdateEPSSurprisePos = 0;

  m_lCurrentUpdateDayLinePos = 0; // ����֤ȯ����������ʮ��֮�࣬�޷���һ��֮�ڸ����꣬�ʲ������ô�������

  m_strMarketId = _T("�����г�");
  m_lMarketTimeZone = 4 * 3600; // ��������ʹ��������׼ʱ�䡣
  CalculateTime();

  InitialFinnhubInquiryStr();
  InitialTiingoInquiryStr();

  Reset();
}

void CWorldMarket::InitialFinnhubInquiryStr(void) {
  m_vFinnhubInquiringStr.resize(1000);

  // Finnhubǰ׺�ַ����ڴ�Ԥ��֮
  m_vFinnhubInquiringStr.at(__COMPANY_PROFILE__) = _T("https://finnhub.io/api/v1/stock/profile?symbol="); // ��˾��顣
  m_vFinnhubInquiringStr.at(__COMPANY_PROFILE_CONCISE__) = _T("https://finnhub.io/api/v1/stock/profile2?symbol="); // ��˾��飨��棩
  m_vFinnhubInquiringStr.at(__COMPANY_SYMBOLS__) = _T("https://finnhub.io/api/v1/stock/symbol?exchange="); // ���ô��뼯
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

void CWorldMarket::InitialTiingoInquiryStr(void) {
  m_vTiingoInquiringStr.resize(1000);

  m_vTiingoInquiringStr.at(__COMPANY_PROFILE__) = _T("https://api.tiingo.com/tiingo/fundamentals/");
  m_vTiingoInquiringStr.at(__COMPANY_SYMBOLS__) = _T("https://api.tiingo.com/tiingo/fundamentals/meta?"); // ���ô��뼯
  m_vTiingoInquiringStr.at(__STOCK_CANDLES__) = _T("https://api.tiingo.com/tiingo/daily/");
}

CWorldMarket::~CWorldMarket() {
}

void CWorldMarket::Reset(void) {
  ResetFinnhub();
  ResetTiingo();
}

void CWorldMarket::ResetFinnhub(void) {
  m_lLastTotalWorldStock = 0;
  m_lCurrentProfilePos = 0;
  m_lCurrentRTDataQuotePos = 0;
  m_lCurrentForexExchangePos = 0;
  m_lCurrentForexSymbolPos = 0;
  m_lChoicedStockPos = 0;

  m_CurrentFinnhubInquiry.Reset();
  while (m_qFinnhubWebInquiry.size() > 0) m_qFinnhubWebInquiry.pop();

  m_vFinnhubExchange.resize(0);
  m_mapFinnhubExchange.clear();
  m_lCurrentExchangePos = 0;

  // Finnhub�������������ݱ�ʶ��ÿ����Ҫ���á�
  m_fFinnhubSymbolUpdated = false; // ÿ����Ҫ���´���
  m_fWorldStockUpdated = false;
  m_fFinnhubDayLineUpdated = false;
  m_vWorldStock.resize(0);
  m_mapWorldStock.clear();
  m_vWorldChoicedStock.resize(0);
  m_mapWorldChoicedStock.clear();
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

  m_fFinnhubPeerUpdated = false;
  m_lCurrentUpdatePeerPos = 0;

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
    m_lCurrentUpdateEPSSurprisePos = 0;
    m_fFinnhubEPSSurpriseUpdated = false;
  }
}

void CWorldMarket::ResetQuandl(void) {
}

void CWorldMarket::ResetTiingo(void) {
  m_fTiingoInquiring = false;
  m_fTiingoDataReceived = true;

  m_fTiingoSymbolUpdated = false;
  m_fTiingoDayLineUpdated = false;
}

void CWorldMarket::ResetMarket(void) {
  Reset();

  LoadOption();
  LoadWorldExchangeDB(); // װ�����罻������Ϣ
  LoadCountryList();
  LoadWorldStock();
  LoadWorldChoicedStock();
  LoadForexExchange();
  LoadForexSymbol();
  LoadEconomicCalendarDB();

  CString str = _T("����World Market��������׼ʱ�䣺");
  str += GetStringOfMarketTime();
  gl_systemMessage.PushInformationMessage(str);
}

bool CWorldMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  static int s_iCountfinnhubLimit = 12; // Finnhub.ioÿ1.2����������һ�Σ��Է�ֹ����Ƶ�ʹ��ߵ������
  static int s_iCountTiingoLimit = 80; // ��֤ÿ80��ִ��һ�Σ���8��ÿ�Σ�.Tiingo����˻��ٶ�����ΪÿСʱ500�Σ� ÿ����9�Σ���ÿ��8�뼴�ɡ�
  const long lCurrentTime = GetFormatedMarketTime();

  TaskCheckSystemReady();

  if (--s_iCountfinnhubLimit < 0) {
    TaskInquiryFinnhub(lCurrentTime);
    if (m_fFinnhubInquiring) {
      s_iCountfinnhubLimit = 12; // ����������������ݣ������ü��������Ա�������һ�Ρ�
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
bool CWorldMarket::ProcessFinnhubInquiringMessage(void) {
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  CWorldStockPtr pStock = nullptr;
  CForexSymbolPtr pSymbol = nullptr;

  if (m_qFinnhubWebInquiry.size() > 0) { // ������ȴ���
    ASSERT(m_fFinnhubInquiring);
    if (IsFinnhubDataReceived()) { //�Ѿ�����������������Finnhub�����Ѿ����յ��ˣ�
      m_CurrentFinnhubInquiry = m_qFinnhubWebInquiry.top();
      m_qFinnhubWebInquiry.pop();
      gl_pFinnhubWebInquiry->SetInquiryingStrPrefix(m_vFinnhubInquiringStr.at(m_CurrentFinnhubInquiry.m_lInquiryIndex)); // ����ǰ׺
      switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) { // ���ݲ�ͬ��Ҫ��������׺�ַ���
      case __COMPANY_PROFILE__: // Premium ����˻��޷���ȡ����Ϣ��sandboxģʽ�ܶ�ȡ���������Ǵ���ģ�ֻ�����ڲ��ԡ�
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_strSymbol);
      m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_fInquiryStockProfile = false;
      break;
      case __COMPANY_PROFILE_CONCISE__:
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_strSymbol);
      m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_fInquiryStockProfile = false;
      break;
      case  __COMPANY_SYMBOLS__:
      strMiddle = m_vFinnhubExchange.at(m_lCurrentExchangePos)->m_strCode;
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
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
      pStock = m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(pStock->m_strSymbol);
      pStock->m_fFinnhubPeerUpdated = true;
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
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex)->m_strSymbol);
      break;
      case __STOCK_EARNING_CALENDER__:
      break;
      case __STOCK_QUOTE__:
      pStock = m_vWorldStock.at(m_lCurrentRTDataQuotePos);
      strMiddle = pStock->GetSymbol();
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      break;
      case __STOCK_CANDLES__:
      pStock = m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      strMiddle = pStock->GetFinnhubDayLineInquiryString(GetMarketTime());
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      pStock->SetDayLineNeedUpdate(false);
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
      strMiddle = pSymbol->GetFinnhubDayLineInquiryString(GetMarketTime());
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      pSymbol->SetDayLineNeedUpdate(false);
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

bool CompareWorldStock(CWorldStockPtr p1, CWorldStockPtr p2) { return (p1->m_strSymbol.Compare(p2->m_strSymbol) < 0); }

//////////////////////////////////////////////
//
// �������߳̽��յ���Finnhub������Ϣ��
//
// Ŀǰֻ����ͬʱ����һ����Ϣ������Ϣ��������������֮ǰ������������һ����Ϣ�������ܹ���֤ͬһ�ԡ�������Finnhub��վ���ٶ����ƣ�
// ÿ����ֻ����60�����룬�ʶ�û�б�Ҫǿ�������ٶȡ�
//
//////////////////////////////////////////////
bool CWorldMarket::ProcessFinnhubWebDataReceived(void) {
  CWebDataPtr pWebData = nullptr;
  CWorldStockPtr pStock = nullptr;
  CString str = _T("");
  vector<CString> vExchange;
  vector<CForexSymbolPtr> vForexSymbol;
  vector<CEconomicCalendarPtr> vEconomicCalendar;
  vector<CEPSSurprisePtr> vEPSSurprise;
  vector<CWorldStockPtr> vStock;
  vector<CDayLinePtr> vDayLine;
  vector<CCountryPtr> vCountry;
  long lTemp = 0;
  bool fFoundNewStock = false;
  char buffer[30];
  CString strNumber;

  ASSERT(gl_WebInquirer.GetFinnhubDataSize() <= 1);
  if (IsFinnhubDataReceived()) { // ����������ݽ������
    if (gl_WebInquirer.GetFinnhubDataSize() > 0) {  // ����ǰ��������
      ASSERT(m_fFinnhubInquiring);
      pWebData = gl_WebInquirer.PopFinnhubData();
      switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) {
      case __COMPANY_PROFILE__: // Ŀǰ����˻��޷�ʹ�ô˹��ܡ�
      ProcessFinnhubStockProfile(pWebData, m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex));
      break;
      case __COMPANY_PROFILE_CONCISE__:
      pStock = m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      if (ProcessFinnhubStockProfileConcise(pWebData, pStock)) {
        pStock->m_lProfileUpdateDate = gl_pWorldMarket->GetFormatedMarketDate();
        pStock->SetUpdateStockProfileDB(true);
      }
      break;
      case  __COMPANY_SYMBOLS__:
      if (ProcessFinnhubStockSymbol(pWebData, vStock)) {
        //TRACE("����%s Finnhub Symbol����Ϊ%d\n", m_vFinnhubExchange.at(m_lCurrentExchangePos)->m_strCode, vStock.size());
        sprintf_s(buffer, _T("%6d"), vStock.size());
        strNumber = buffer;
        str = _T("����") + m_vFinnhubExchange.at(m_lCurrentExchangePos)->m_strCode + _T(" Finnhub Symbol����Ϊ") + strNumber;
        gl_systemMessage.PushInnerSystemInformationMessage(str);
        // ���Ͻ��������롣
        for (auto& pStock3 : vStock) {
          pStock3->m_strExchangeCode = m_vFinnhubExchange.at(m_lCurrentExchangePos)->m_strCode;
        }
        for (auto& pStock2 : vStock) {
          if (!IsWorldStock(pStock2->GetSymbol())) {
            CreateNewStock(pStock2);
            fFoundNewStock = true;
            TRACE("�����´��룺%s\n", pStock2->GetSymbol().GetBuffer());
          }
        }
        if (fFoundNewStock) {
          SortStockVector();
          gl_systemMessage.PushInnerSystemInformationMessage("Finnhub�����´��룬���´��뼯");
        }
      }
      break;
      case  __MARKET_NEWS__:
      break;
      case __COMPANY_NEWS__:
      break;
      case __NEWS_SENTIMENT__:
      break;
      case __PEERS__:
      pStock = m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      if (ProcessFinnhubStockPeer(pWebData, pStock)) {
        pStock->SetPeerUpdateDate(GetFormatedMarketDate());
        pStock->SetUpdateStockProfileDB(true);
      }
      break;
      case __BASIC_FINANCIALS__:
      break;
      case __STOCK_EPS_SURPRISE__:
      pStock = m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      if (ProcessFinnhubEPSSurprise(pWebData, vEPSSurprise)) {
        if (vEPSSurprise.size() > 0) {
          pStock->UpdateEPSSurprise(vEPSSurprise);
        }
        else {
          pStock->m_lLastEPSSurpriseUpdateDate = 19700101; // ����������Ϊ���硣
          pStock->SetUpdateStockProfileDB(true);
        }
        pStock->m_fEPSSurpriseNeedUpdate = false;
        pStock->m_fEPSSurpriseNeedSave = true;
      }
      break;
      case __STOCK_QUOTE__:
      pStock = m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex);
      ProcessFinnhubStockQuote(pWebData, pStock);
      if ((pStock->GetTransactionTime() + 3600 * 12 - GetMarketTime()) > 0) { // ����ʱ�䲻����12Сʱ�������ô˹�ƱΪ��Ծ��Ʊ
        pStock->SetActive(true);
        if (!pStock->IsIPOed()) {
          pStock->SetIPOStatus(__STAKE_IPOED__);
          pStock->SetUpdateStockProfileDB(true);
        }
      }
      break;
      case __STOCK_CANDLES__:
      pStock = m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex);
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
      if (ProcessFinnhubForexExchange(pWebData, vExchange)) {
        for (int i = 0; i < vExchange.size(); i++) {
          if (m_mapForexExchange.find(vExchange.at(i)) == m_mapForexExchange.end()) {
            lTemp = m_vForexExchange.size();
            m_vForexExchange.push_back(vExchange.at(i));
            m_mapForexExchange[vExchange.at(i)] = lTemp;
          }
        }
        m_fFinnhubForexExhangeUpdated = true;
      }
      break;
      case __FOREX_SYMBOLS__:
      if (ProcessFinnhubForexSymbol(pWebData, vForexSymbol)) {
        for (auto& pSymbol : vForexSymbol) {
          if (m_mapForexSymbol.find(pSymbol->m_strSymbol) == m_mapForexSymbol.end()) {
            pSymbol->m_strExchange = m_vForexExchange.at(m_CurrentFinnhubInquiry.m_lStockIndex);
            lTemp = m_mapForexSymbol.size();
            m_mapForexSymbol[pSymbol->m_strSymbol] = lTemp;
            m_vForexSymbol.push_back(pSymbol);
          }
        }
      }
      break;
      case __FOREX_CANDLES__:
      if (ProcessFinnhubForexCandle(pWebData, m_vForexSymbol.at(m_CurrentFinnhubInquiry.m_lStockIndex))) {
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
      }
      break;
      case __ECONOMIC_CALENDAR__:
      ProcessFinnhubEconomicCalendar(pWebData, vEconomicCalendar);
      UpdateEconomicCalendar(vEconomicCalendar);
      m_fFinnhubEconomicCalendarUpdated = true;
      break;
      default:
      break;
      }
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(_T("")); // ��Щ��������û���õ��м��ַ��Σ���������֮ǰ���м��ַ��Σ�����еĻ���������������ַ����Ĵ���
      m_fFinnhubInquiring = false;
    }
  }

  return true;
}

bool CWorldMarket::ProcessTiingoInquiringMessage(void) {
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  CWorldStockPtr pStock = nullptr;
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
      case __COMPANY_PROFILE_CONCISE__:
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
      pStock = m_vWorldStock.at(m_CurrentTiingoInquiry.m_lStockIndex);
      strMiddle = pStock->GetTiingoDayLineInquiryString(GetFormatedMarketDate());
      gl_pTiingoWebInquiry->SetInquiryingStringMiddle(strMiddle);
      pStock->SetDayLineNeedUpdate(false);
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

bool CWorldMarket::ProcessTiingoWebDataReceived(void) {
  CWebDataPtr pWebData = nullptr;
  CWorldStockPtr pStock = nullptr;
  CString str = _T("");
  vector<CString> vExchange;
  vector<CForexSymbolPtr> vForexSymbol;
  vector<CEconomicCalendarPtr> vEconomicCalendar;
  vector<CEPSSurprisePtr> vEPSSurprise;
  vector<CWorldStockPtr> vStock;
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
      case __COMPANY_PROFILE_CONCISE__:
      break;
      case  __COMPANY_SYMBOLS__:
      if (ProcessTiingoStockSymbol(pWebData, vStock)) {
        for (auto& pStock2 : vStock) {
          if (pStock2->m_fIsActive && (IsWorldStock(pStock2->m_strSymbol))) { // Tiingo��Symbol��Ϣֻ������Finnhub��һ�����䡣
            lTemp++;
            pStock = m_vWorldStock.at(m_mapWorldStock.at(pStock2->m_strSymbol));
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
            pStock->SetUpdateStockProfileDB(true);
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
      pStock = m_vWorldStock.at(m_CurrentTiingoInquiry.m_lStockIndex);
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
      gl_pTiingoWebInquiry->SetInquiryingStringMiddle(_T("")); // ��Щ��������û���õ��м��ַ��Σ���������֮ǰ���м��ַ��Σ�����еĻ���������������ַ����Ĵ���
      m_fTiingoInquiring = false;
    }
  }

  return true;
}

bool CWorldMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
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
    SchedulingTaskPerHour(lCurrentTime);
  }
  if (s_iCount10Minute < 0) {
    s_iCount10Minute = 599;
    SchedulingTaskPer10Minute(lCurrentTime);
  }
  if (s_iCount1Minute < 0) {
    s_iCount1Minute = 59;
    SchedulingTaskPerMinute(lCurrentTime);
  }
  if (s_iCount10Second < 0) {
    s_iCount10Second = 9;
    SchedulingTaskPer10Seconds(lCurrentTime);
  }

  return true;
}

bool CWorldMarket::SchedulingTaskPer10Seconds(long lCurrentTime) {
  return true;
}

bool CWorldMarket::SchedulingTaskPerMinute(long lCurrentTime) {
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

bool CWorldMarket::SchedulingTaskPer10Minute(long lCurrentTime) {
  if (m_fFinnhubSymbolUpdated && IsWorldStockUpdated()) {
    TaskUpdateStockDB();
  }

  return true;
}

bool CWorldMarket::SchedulingTaskPerHour(long lCurrentTime) {
  return true;
}

/// <summary>
/// /////////////////////////////////////////////////////////////////////////////////
///
/// ��������������ÿ����ִ��һ�εĵ����С�
/// <returns></returns>
///
bool CWorldMarket::TaskResetMarket(long lCurrentTime) {
  // �г�ʱ��ʮ��ʱ����ϵͳ
  if (IsPermitResetMarket()) { // �����������ϵͳ
    if ((lCurrentTime > 175000) && (lCurrentTime <= 175100)) { // ���г�ʱ���������ʱ(����ʱ��������ʱ�������г������������ڽ����������ݡ�
      SetResetMarket(true);// ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
      SetPermitResetMarket(false); // ���첻����������ϵͳ��
      SetSystemReady(false);
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// �˺�����SchedulingTask���ȣ�ÿ1500��������ִ��һ�Ρ�
//
//////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskInquiryFinnhub(long lCurrentTime) {
  if (((lCurrentTime < 174700) || (lCurrentTime > 175100))) { // ������ʱ����ϵͳ���ʶ���ʱ���������������Ϣ��
    TaskInquiryFinnhubCountryList();
    TaskInquiryFinnhubForexExchange();
    TaskInquiryFinnhubCompanySymbol(); // ��һ���������������뵱��֤ȯ����
    TaskInquiryFinnhubForexSymbol();
    //TaskInquiryFinnhubEconomicCalender();

    // ����Finnhub������Ϣ�����񣬽�Ҫ����������Ա�֤���г�ʱ���賿ʮ���Ӻ�ִ�С������ܹ���֤�������г�ʱû��ִ�в�ѯ����
    if (IsSystemReady() && !m_fFinnhubInquiring) {
      TaskInquiryFinnhubCompanyProfile2();
      TaskInquiryFinnhubPeer();
      //TaskInquiryFinnhubEPSSurprise(); // �������ûʲô�ã���ʱֹͣ���¡�
      TaskInquiryFinnhubForexDayLine();
      TaskInquiryFinnhubDayLine();
      if (m_fFinnhubDayLineUpdated) {
        //TaskInquiryFinnhubRTQuote();
      }
    }
  }

  return true;
}

bool CWorldMarket::TaskInquiryFinnhubCountryList(void) {
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

bool CWorldMarket::TaskInquiryFinnhubCompanySymbol(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
  CFinnhubExchangePtr pExchange;
  CString str = _T("");
  long lExchangeSize = m_vFinnhubExchange.size();

  if (!m_fFinnhubSymbolUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentExchangePos = 0; m_lCurrentExchangePos < lExchangeSize; m_lCurrentExchangePos++) {
      if (m_vFinnhubExchange.at(m_lCurrentExchangePos)->m_fIsActive && !m_vFinnhubExchange.at(m_lCurrentExchangePos)->IsUpdated()) {
        pExchange = m_vFinnhubExchange.at(m_lCurrentExchangePos);
        fFound = true;
        break;
      }
    }
    if (fFound) {
      inquiry.m_lInquiryIndex = __COMPANY_SYMBOLS__;
      inquiry.m_lStockIndex = m_lCurrentExchangePos;
      inquiry.m_iPriority = 10;
      m_qFinnhubWebInquiry.push(inquiry);
      m_fFinnhubInquiring = true;
      pExchange->SetUpdated(true);
      //TRACE("����%s������֤ȯ����\n", pExchange->m_strCode.GetBuffer());
    }
    else {
      m_fFinnhubSymbolUpdated = true;
      TRACE("Finnhub�������������ݲ�ѯ���\n");
      str = _T("�������������ݲ�ѯ���");
      gl_systemMessage.PushInformationMessage(str);
    }
  }
  return true;
}

bool CWorldMarket::TaskUpdateForexSymbolDB(void) {
  RunningThreadUpdateForexSymbolDB();
  return false;
}

bool CWorldMarket::TaskUpdateForexExchangeDB(void) {
  CSetFinnhubForexExchange setForexExchange;

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

bool CWorldMarket::TaskUpdateStockDB(void) {
  RunningTaskThreadUpdateStockDB();
  return true;
}

bool CWorldMarket::TaskInquiryFinnhubCompanyProfile2(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
  long lStockSetSize = m_vWorldStock.size();
  CString str = _T("");

  ASSERT(IsSystemReady());
  if (!m_fWorldStockUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentProfilePos = 0; m_lCurrentProfilePos < lStockSetSize; m_lCurrentProfilePos++) {
      if (IsEarlyThen(m_vWorldStock.at(m_lCurrentProfilePos)->m_lProfileUpdateDate, GetFormatedMarketDate(), 365)) {
        fFound = true;
        break;
      }
    }
    if (fFound) {
      inquiry.m_lInquiryIndex = __COMPANY_PROFILE_CONCISE__;
      inquiry.m_lStockIndex = m_lCurrentProfilePos;
      inquiry.m_iPriority = 10;
      m_qFinnhubWebInquiry.push(inquiry);
      //TRACE("����%s���\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
      m_fFinnhubInquiring = true;
    }
    else {
      m_fWorldStockUpdated = true;
      TRACE("Finnhub��Ʊ���������\n");
      str = _T("Finnhub��Ʊ���������");
      gl_systemMessage.PushInformationMessage(str);
    }
  }
  return false;
}

bool CWorldMarket::TaskInquiryFinnhubDayLine(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
  CWorldStockPtr pStock;
  CString str = _T("");
  long lStockSetSize = m_vWorldStock.size();

  ASSERT(IsSystemReady());
  if (!m_fFinnhubDayLineUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < lStockSetSize; m_lCurrentUpdateDayLinePos++) {
      pStock = m_vWorldStock.at(m_lCurrentUpdateDayLinePos);
      if (pStock->IsUSMarket() && pStock->IsDayLineNeedUpdate()) { // Ŀǰ����˻�ֻ�����������г��Ĺ�Ʊ���ߡ�
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
      pStock->SetDayLineNeedUpdate(false);
      TRACE("����%s��������\n", pStock->m_strSymbol.GetBuffer());
    }
    else {
      m_fFinnhubDayLineUpdated = true;
      m_lCurrentUpdateDayLinePos = 0; // ���ô����������е��������ݸ���һ������ʱ��Ҫ����24Сʱ���ʱ��ָ��¼��ɡ�
      TRACE("Finnhub���߸�����ϣ����¿�ʼ����\n");
      str = _T("�����г�������ʷ���ݸ�����ϣ����¿�ʼ����");
      gl_systemMessage.PushInformationMessage(str);
    }
  }
  return true;
}

bool CWorldMarket::TaskInquiryFinnhubRTQuote(void) {
  WebInquiry inquiry{ 0, 0, 0 };

  ASSERT(IsSystemReady());
  if (!m_fFinnhubInquiring) {
    m_lCurrentRTDataQuotePos++;
    if (m_lCurrentRTDataQuotePos == m_vWorldStock.size()) m_lCurrentRTDataQuotePos = 0;
    inquiry.m_lInquiryIndex = __STOCK_QUOTE__;
    inquiry.m_lStockIndex = m_lCurrentRTDataQuotePos;
    inquiry.m_iPriority = 10;
    m_qFinnhubWebInquiry.push(inquiry);
    m_fFinnhubInquiring = true;
    TRACE("����%sʵʱ����\n", m_vWorldStock.at(m_lCurrentRTDataQuotePos)->m_strSymbol.GetBuffer());
  }
  return true;
}

bool CWorldMarket::TaskInquiryFinnhubPeer(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
  CWorldStockPtr pStock;
  CString str = _T("");
  long lStockSetSize = m_vWorldStock.size();

  ASSERT(IsSystemReady());
  if (!m_fFinnhubPeerUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentUpdatePeerPos = 0; m_lCurrentUpdatePeerPos < lStockSetSize; m_lCurrentUpdatePeerPos++) {
      if (!m_vWorldStock.at(m_lCurrentUpdatePeerPos)->m_fFinnhubPeerUpdated) {
        pStock = m_vWorldStock.at(m_lCurrentUpdatePeerPos);
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
      TRACE("����%s Peer����\n", m_vWorldStock.at(m_lCurrentUpdatePeerPos)->m_strSymbol.GetBuffer());
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

bool CWorldMarket::TaskInquiryFinnhubEconomicCalender(void) {
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

bool CWorldMarket::TaskInquiryFinnhubEPSSurprise(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
  CWorldStockPtr pStock;
  CString str = _T("");
  long lStockSetSize = m_vWorldStock.size();

  ASSERT(IsSystemReady());
  if (!m_fFinnhubEPSSurpriseUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentUpdateEPSSurprisePos = 0; m_lCurrentUpdateEPSSurprisePos < lStockSetSize; m_lCurrentUpdateEPSSurprisePos++) {
      if (m_vWorldStock.at(m_lCurrentUpdateEPSSurprisePos)->IsEPSSurpriseNeedUpdate()) {
        pStock = m_vWorldStock.at(m_lCurrentUpdateEPSSurprisePos);
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
      m_vWorldStock.at(m_lCurrentUpdateEPSSurprisePos)->SetEPSSurpriseNeedUpdate(false);
      TRACE("����%s EPS Surprise����\n", m_vWorldStock.at(m_lCurrentUpdateEPSSurprisePos)->m_strSymbol.GetBuffer());
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

bool CWorldMarket::TaskInquiryFinnhubForexExchange(void) {
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

bool CWorldMarket::TaskInquiryFinnhubForexSymbol(void) {
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

bool CWorldMarket::TaskInquiryFinnhubForexDayLine(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
  CForexSymbolPtr pStock;
  CString str = _T("");
  const long lStockSetSize = m_vForexSymbol.size();

  ASSERT(IsSystemReady());
  if (!m_fForexDayLineUpdated && !m_fFinnhubInquiring) {
    for (m_lCurrentUpdateForexDayLinePos = 0; m_lCurrentUpdateForexDayLinePos < lStockSetSize; m_lCurrentUpdateForexDayLinePos++) {
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

void CWorldMarket::TaskInquiryTiingo(void) {
  if (IsSystemReady() && !m_fTiingoInquiring) {
    TaskInquiryTiingoCompanySymbol();
    // ����Tiingo�涨ÿ��ֻ�ܲ�ѯ500�����룬�ʲ��Գɹ�����ʱ��ʹ�á�
    TaskInquiryTiingoDayLine(); // ����������ϡ�
  }
}

bool CWorldMarket::TaskInquiryTiingoCompanySymbol(void) {
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
bool CWorldMarket::TaskInquiryTiingoDayLine(void) {
  bool fFound = false;
  WebInquiry inquiry{ 0, 0, 0 };
  CWorldStockPtr pStock;
  CString str = _T("");
  long lStockSetSize = m_vWorldChoicedStock.size();

  ASSERT(IsSystemReady());
  if (!m_fTiingoDayLineUpdated && !m_fTiingoInquiring) {
    for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < lStockSetSize; m_lCurrentUpdateDayLinePos++) {
      if (m_vWorldChoicedStock.at(m_lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
        pStock = m_vWorldChoicedStock.at(m_lCurrentUpdateDayLinePos);
        fFound = true;
        break;
      }
    }
    if (fFound) {
      inquiry.m_lInquiryIndex = __STOCK_CANDLES__;
      inquiry.m_lStockIndex = m_mapWorldStock.at(pStock->GetSymbol());
      inquiry.m_iPriority = 10;
      m_qTiingoWebInquiry.push(inquiry);
      m_fTiingoInquiring = true;
      m_vWorldChoicedStock.at(m_lCurrentUpdateDayLinePos)->SetDayLineNeedUpdate(false);
      TRACE("����Tiingo %s��������\n", m_vWorldChoicedStock.at(m_lCurrentUpdateDayLinePos)->m_strSymbol.GetBuffer());
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

bool CWorldMarket::TaskUpdateDayLineDB(void) {
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
bool CWorldMarket::TaskUpdateForexDayLineDB(void) {
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

bool CWorldMarket::TaskUpdateCountryListDB(void) {
  RunningThreadUpdateCountryListDB();
  return false;
}

bool CWorldMarket::TaskUpdateEPSSurpriseDB(void) {
  CString str;

  for (auto& pStock : m_vWorldStock) {
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

bool CWorldMarket::TaskUpdateEconomicCalendar(void) {
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

bool CWorldMarket::TaskCheckSystemReady(void) {
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

bool CWorldMarket::TaskUpdateDayLineStartEndDate(void) {
  RunningthreadUpdateDayLneStartEndDate(this);
  return false;
}

bool CWorldMarket::RunningthreadUpdateDayLneStartEndDate(CWorldMarket* pMarket) {
  thread thread1(ThreadUpdateWorldStockDayLineStartEndDate, pMarket);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CWorldMarket::RunningThreadUpdateDayLineDB() {
  thread thread1(ThreadUpdateWorldStockDayLineDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CWorldMarket::RunningTaskThreadUpdateStockDB(void) {
  thread thread1(ThreadUpdateStockDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CWorldMarket::RunningThreadUpdateForexDayLineDB(CFinnhubForexSymbol* pSymbol) {
  thread thread1(ThreadUpdateForexDayLineDB, pSymbol);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CWorldMarket::RunningThreadUpdateForexSymbolDB() {
  thread thread1(ThreadUpdateForexSymbolDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CWorldMarket::RunningThreadUpdateCountryListDB(void) {
  thread thread1(ThreadUpdateCountryListDB, this);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CWorldMarket::RunningThreadUpdateEPSSurpriseDB(CWorldStock* pStock) {
  thread thread1(ThreadUpdateEPSSurpriseDB, pStock);
  thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
  return true;
}

bool CWorldMarket::IsWorldStock(CString strSymbol) {
  if (m_mapWorldStock.find(strSymbol) == m_mapWorldStock.end()) { // �´��룿
    return false;
  }
  else return true;
}

bool CWorldMarket::IsWorldStockUpdated(void) {
  const int iTotal = m_vWorldStock.size();
  for (int i = 0; i < iTotal; i++) {
    if (m_vWorldStock.at(i)->IsUpdateStockProfileDB()) return true;
  }
  return false;
}

CWorldStockPtr CWorldMarket::GetWorldStock(CString strTicker) {
  if (IsWorldStock(strTicker)) {
    return m_vWorldStock.at(m_mapWorldStock.at(strTicker));
  }
  else return nullptr;
}

void CWorldMarket::CreateNewStock(CWorldStockPtr pStock) {
  m_mapWorldStock[pStock->GetSymbol()] = m_vWorldStock.size();
  m_vWorldStock.push_back(pStock);
  pStock->SetTodayNewStock(true);
  pStock->SetUpdateStockProfileDB(true);
}

bool CWorldMarket::UpdateEconomicCalendar(vector<CEconomicCalendarPtr> vEconomicCalendar) {
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

long CWorldMarket::GetFinnInquiry(void) {
  WebInquiry inquiry{ 0, 0, 0 };
  if (m_qFinnhubWebInquiry.size() > 0) {
    inquiry = m_qFinnhubWebInquiry.top();
    m_qFinnhubWebInquiry.pop();
    return inquiry.m_lInquiryIndex;
  }
  return -1;
}

bool CWorldMarket::LoadOption(void) {
  CSetWorldMarketOption setOption;

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

bool CWorldMarket::LoadWorldExchangeDB(void) {
  CSetFinnhubExchange setExchange;
  CFinnhubExchangePtr pExchange = nullptr;

  if (m_vFinnhubExchange.size() == 0) {
    setExchange.m_strSort = _T("[Code]");
    setExchange.Open();
    while (!setExchange.IsEOF()) {
      pExchange = make_shared<CFinnhubExchange>();
      pExchange->Load(setExchange);
      m_vFinnhubExchange.push_back(pExchange);
      m_mapFinnhubExchange[pExchange->m_strCode] = m_vFinnhubExchange.size();
      setExchange.MoveNext();
    }
    setExchange.Close();
  }

  return true;
}

bool CWorldMarket::LoadWorldStock(void) {
  CSetWorldStock setWorldStock;
  CWorldStockPtr pWorldStock = nullptr;
  CString str;

  setWorldStock.m_strSort = _T("[Symbol]");
  setWorldStock.Open();
  setWorldStock.m_pDatabase->BeginTrans();
  while (!setWorldStock.IsEOF()) {
    pWorldStock = make_shared<CWorldStock>();
    pWorldStock->Load(setWorldStock);
    if (!IsWorldStock(pWorldStock->m_strSymbol)) {
      pWorldStock->CheckDayLineUpdateStatus(GetFormatedMarketDate(), GetLastTradeDate(), GetFormatedMarketTime(), GetDayOfWeek());
      pWorldStock->CheckEPSSurpriseStatus(GetFormatedMarketDate());
      pWorldStock->CheckPeerStatus(GetFormatedMarketDate());
      m_mapWorldStock[setWorldStock.m_Symbol] = m_lLastTotalWorldStock++;
      m_vWorldStock.push_back(pWorldStock);
    }
    else {
      str = _T("�����ظ����룺");
      str += pWorldStock->m_strSymbol;
      gl_systemMessage.PushInnerSystemInformationMessage(str);
      setWorldStock.Delete(); // ɾ�����ظ�����
    }
    setWorldStock.MoveNext();
  }
  setWorldStock.m_pDatabase->CommitTrans();
  setWorldStock.Close();
  SortStockVector();
  m_lLastTotalWorldStock = m_vWorldStock.size();
  TRACE("��װ��%d Finnhub Symbol\n", m_lLastTotalWorldStock);
  return true;
}

bool CWorldMarket::LoadWorldChoicedStock(void) {
  CSetWorldChoicedStock setWorldChoicedStock;
  CWorldStockPtr pStock = nullptr;

  setWorldChoicedStock.Open();
  setWorldChoicedStock.m_pDatabase->BeginTrans();
  while (!setWorldChoicedStock.IsEOF()) {
    if (IsWorldStock(setWorldChoicedStock.m_Symbol)) {
      pStock = GetStock(setWorldChoicedStock.m_Symbol);
      m_mapWorldChoicedStock[setWorldChoicedStock.m_Symbol] = m_mapWorldChoicedStock.size();
      m_vWorldChoicedStock.push_back(pStock);
    }
    else {
      setWorldChoicedStock.Delete(); // ����Ƿ���Ʊ����
    }
    setWorldChoicedStock.MoveNext();
  }
  setWorldChoicedStock.m_pDatabase->CommitTrans();
  setWorldChoicedStock.Close();

  return true;
}

bool CWorldMarket::UpdateCountryListDB(void) {
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

/// <summary>
/// ���ֲ�ѯ��ʽ�Ƚϻ�ɬ���������ȷ��Ŀǰʹ�ô˺�����(���ܳ��ִ洢�����ͬ��������⣬�о�֮��
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CWorldMarket::UpdateStockDB(void) {
  CWorldStockPtr pStock = nullptr;
  CSetWorldStock setWorldStock;
  int iUpdatedStock = 0;
  int iCount = 0;

  //����ԭ�еĴ��뼯״̬
  if (IsWorldStockUpdated()) {
    for (auto& pStock2 : m_vWorldStock) {
      if (pStock2->IsUpdateStockProfileDB()) iUpdatedStock++;
    }
    setWorldStock.m_strSort = _T("[Symbol]");
    setWorldStock.Open();
    setWorldStock.m_pDatabase->BeginTrans();
    while (iCount < iUpdatedStock) {
      if (setWorldStock.IsEOF()) break;
      pStock = m_vWorldStock.at(m_mapWorldStock.at(setWorldStock.m_Symbol));
      if (pStock->IsUpdateStockProfileDB()) {
        iCount++;
        pStock->Update(setWorldStock);
        pStock->SetUpdateStockProfileDB(false);
      }
      setWorldStock.MoveNext();
    }
    if (iCount < iUpdatedStock) {
      for (auto& pStock3 : m_vWorldStock) {
        if (pStock3->IsUpdateStockProfileDB()) {
          //ASSERT(pStock3->IsTodayNewStock()); // ���е��¹�Ʊ�����ǽ��������ɵ�
          iCount++;
          pStock3->Append(setWorldStock);
          pStock3->SetTodayNewStock(false);
          TRACE("�洢��Ʊ��%s\n", pStock3->GetSymbol().GetBuffer());
        }
        if (iCount >= iUpdatedStock) break;
      }
    }
    setWorldStock.m_pDatabase->CommitTrans();
    setWorldStock.Close();
    m_lLastTotalWorldStock = m_vWorldStock.size();
  }
  ASSERT(iCount == iUpdatedStock);
  return true;
}

bool CWorldMarket::UpdateForexSymbolDB(void) {
  const long lTotalForexSymbol = m_vForexSymbol.size();
  CForexSymbolPtr pSymbol = nullptr;
  CSetFinnhubForexSymbol setForexSymbol;
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
    if (pSymbol->IsUpdateStockProfileDB()) {
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
        if (pSymbol->IsUpdateStockProfileDB()) {
          pSymbol->Update(setForexSymbol);
          pSymbol->SetUpdateStockProfileDB(false);
        }
      }
      setForexSymbol.MoveNext();
    }
    setForexSymbol.m_pDatabase->CommitTrans();
    setForexSymbol.Close();
  }

  return true;
}

bool CWorldMarket::UpdateEconomicCalendarDB(void) {
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

bool CWorldMarket::RebuildEPSSurprise(void) {
  for (auto& p : m_vWorldStock) {
    p->m_lLastEPSSurpriseUpdateDate = 19800101;
    p->m_fEPSSurpriseNeedUpdate = true;
  }
  m_fFinnhubEPSSurpriseUpdated = false;
  m_lCurrentUpdateEPSSurprisePos = 0;

  return true;
}

bool CWorldMarket::ReBuildPeer(void) {
  for (auto& pStock : m_vWorldStock) {
    if (pStock->GetPeerUpdateDate() != 19800101) {
      pStock->SetPeerUpdateDate(19800101);
      pStock->m_fFinnhubPeerUpdated = false;
      pStock->SetUpdateStockProfileDB(true);
    }
  }
  m_fFinnhubPeerUpdated = false;
  m_lCurrentUpdatePeerPos = 0;

  return true;
}

bool CWorldMarket::SortStockVector(void) {
  sort(m_vWorldStock.begin(), m_vWorldStock.end(), CompareWorldStock);
  m_mapWorldStock.clear();
  int j = 0;
  for (auto& pStock : m_vWorldStock) {
    m_mapWorldStock[pStock->m_strSymbol] = j++;
  }

  return true;
}

bool CWorldMarket::LoadForexExchange(void) {
  CSetFinnhubForexExchange setForexExchange;
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

bool CWorldMarket::LoadForexSymbol(void) {
  CSetFinnhubForexSymbol setForexSymbol;
  CForexSymbolPtr pSymbol = nullptr;
  int i = 0;

  setForexSymbol.Open();
  while (!setForexSymbol.IsEOF()) {
    pSymbol = make_shared<CFinnhubForexSymbol>();
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

bool CWorldMarket::LoadCountryList(void) {
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

bool CWorldMarket::LoadEconomicCalendarDB(void) {
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

bool CWorldMarket::RebuildStockDayLineDB(void) {
  for (auto& pStock : m_vWorldStock) {
    pStock->SetIPOStatus(__STAKE_NOT_CHECKED__);
    pStock->SetDayLineStartDate(29900101);
    pStock->SetDayLineEndDate(19800101);
    pStock->SetDayLineNeedUpdate(true);
    pStock->SetUpdateStockProfileDB(true);
  }
  m_fWorldStockUpdated = false;

  return true;
}

bool CWorldMarket::UpdateDayLineStartEndDate(void) {
  CString strFilterPrefix = _T("[Symbol] = '");
  CString strFilter, str;
  CSetWorldStockDayLine setWorldStockDayLine;
  CWorldStockPtr pStock2 = nullptr;

  for (auto& pStock : m_vWorldStock) {
    setWorldStockDayLine.m_strFilter = strFilterPrefix + pStock->m_strSymbol + _T("'");
    setWorldStockDayLine.m_strSort = _T("[Date]");
    setWorldStockDayLine.Open();
    if (!setWorldStockDayLine.IsEOF()) {
      if (setWorldStockDayLine.m_Date < pStock->GetDayLineStartDate()) {
        pStock->SetDayLineStartDate(setWorldStockDayLine.m_Date);
        pStock->SetUpdateStockProfileDB(true);
      }
      setWorldStockDayLine.MoveLast();
      if (setWorldStockDayLine.m_Date > pStock->GetDayLineEndDate()) {
        pStock->SetDayLineEndDate(setWorldStockDayLine.m_Date);
        pStock->SetUpdateStockProfileDB(true);
      }
    }
    setWorldStockDayLine.Close();
  }

  return true;
}