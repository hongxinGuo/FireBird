#include"pch.h"

#include "WorldMarket.h"
#include"thread.h"

#include"WebInquirer.h"
#include"EPSSurprise.h"

#include"SetWorldMarketOption.h"
#include"SetFinnhubForexExchange.h"
#include"SetWorldStock.h"
#include"SetWorldChoicedStock.h"
#include"SetCountry.h"
#include"SetEconomicCalendar.h"
#include"SetEPSSurprise.h"
#include"SetTiingoStock.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

using namespace std;
#include<algorithm>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Semaphore gl_UpdateWorldMarketDB(1);  // ���ź�����������������Ʊ������ʷ���ݿ�

CWorldMarket::CWorldMarket() {
	static int siInstance = 0;

	if (++siInstance > 1) {
		TRACE("CWorldMarket�г�����ֻ�������һ��ʵ��\n");
	}

	// ����ÿ�ո��µı�����������
	m_fFinnhubEPSSurpriseUpdated = true;
	m_lCurrentUpdateEPSSurprisePos = 0;

	m_lCurrentUpdateDayLinePos = 0; // ����֤ȯ���������ж�ʮ��֮�࣬�޷���һ��֮�ڸ����꣬�ʲ������ô�������

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
	m_vFinnhubInquiringStr.at(__INSIDER_TRANSACTION__) = _T("https://finnhub.io/api/v1/stock/insider-transactions?symbol=");
	m_vFinnhubInquiringStr.at(__SEC_FILINGS__) = _T("https://finnhub.io/api/v1/stock/filings?symbol=");

	m_vFinnhubInquiringStr.at(__STOCK_EPS_SURPRISE__) = _T("https://finnhub.io/api/v1/stock/earnings?symbol=");
	m_vFinnhubInquiringStr.at(__STOCK_EARNING_CALENDAR__) = _T("https://finnhub.io/api/v1/calendar/earnings?");

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
	m_vCountry.resize(0);
	m_vEconomicCalendar.resize(0);
	m_vFinnhubExchange.resize(0);
	m_vForexExchange.resize(0);
	m_vForexSymbol.resize(0);
	m_vWorldStock.resize(0);
	m_vWorldChoicedStock.resize(0);

	if (m_FinnhubWebSocket.getReadyState() != ix::ReadyState::Closed) {
		m_FinnhubWebSocket.stop();
	}
	while (m_FinnhubWebSocket.getReadyState() != ix::ReadyState::Closed) Sleep(1);

	if (m_TiingoIEXWebSocket.getReadyState() != ix::ReadyState::Closed) {
		m_TiingoIEXWebSocket.stop();
	}
	while (m_TiingoIEXWebSocket.getReadyState() != ix::ReadyState::Closed) Sleep(1);

	if (m_TiingoCryptoWebSocket.getReadyState() != ix::ReadyState::Closed) {
		m_TiingoCryptoWebSocket.stop();
	}
	while (m_TiingoCryptoWebSocket.getReadyState() != ix::ReadyState::Closed) Sleep(1);

	if (m_TiingoForexWebSocket.getReadyState() != ix::ReadyState::Closed) {
		m_TiingoForexWebSocket.stop();
	}
	while (m_TiingoForexWebSocket.getReadyState() != ix::ReadyState::Closed) Sleep(1);
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
	m_fFinnhubStockProfileUpdated = false;
	m_fFinnhubDayLineUpdated = false;
	m_vWorldStock.resize(0);
	m_mapWorldStock.clear();
	m_vWorldChoicedStock.resize(0);
	m_mapWorldChoicedStock.clear();
	m_fFinnhubForexExchangeUpdated = false;
	m_vForexExchange.resize(0);
	m_mapForexExchange.clear();
	m_fFinnhubForexSymbolUpdated = false;
	m_vForexSymbol.resize(0);
	m_mapForexSymbol.clear();
	m_fFinnhubForexDayLineUpdated = false;
	m_vCountry.resize(0);
	m_mapCountry.clear();
	m_fCountryListUpdated = false;
	m_vEconomicCalendar.resize(0);
	m_mapEconomicCalendar.clear();
	m_fFinnhubEconomicCalendarUpdated = false;

	m_fFinnhubPeerUpdated = false;
	m_fFinnhubInsiderTransactionUpdated = false;
	m_lCurrentUpdatePeerPos = 0;
	m_lCurrentUpdateInsiderTransactionPos = 0;

	m_fFinnhubInquiring = false;
	m_fFinnhubDataReceived = true;

	m_fRebulidDayLine = false;
	SetSystemReady(false); // �г���ʼ״̬Ϊδ���úá�

	m_lLastTotalForexExchange = 0;
	m_lLastTotalForexSymbol = 0;
	m_lCurrentUpdateForexDayLinePos = 0;

	m_lLastTotalCountry = 0;
	m_lLastTotalEconomicCalendar = 0;

	m_lLastTotalTiingoIndustry = 0;
	m_lLastTotalSICIndustry = 0;
	m_lLastTotalNaicsIndustry = 0;

	if (GetDayOfWeek() == 6) { // ÿ�ܵ�����������һ��EPSSurprise
		m_lCurrentUpdateEPSSurprisePos = 0;
		SetFinnhubEPSSurpriseUpdated(false);
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
	LoadCountryDB();
	LoadStockDB();
	LoadWorldChoicedStock();
	LoadForexExchange();
	LoadForexSymbol();
	LoadEconomicCalendarDB();
	LoadTiingoStock();

	CString str = _T("����World Market��������׼ʱ�䣺");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);
}

bool CWorldMarket::SchedulingTask(void) {
	CVirtualMarket::SchedulingTask();

	static time_t s_timeLast = 0;
	static int s_iCountfinnhubLimit = 12; // Finnhub.ioÿ1.5����������һ�Σ��Է�ֹ����Ƶ�ʹ��ߵ������
	static int s_iCountTiingoLimit = 80; // ��֤ÿ80��ִ��һ�Σ���8��ÿ�Σ�.Tiingo����˻��ٶ�����ΪÿСʱ500�Σ� ÿ����9�Σ���ÿ��8�뼴�ɡ�
	const long lCurrentTime = GetFormatedMarketTime();

	TaskCheckSystemReady();

	if (--s_iCountfinnhubLimit < 0) {
		TaskInquiryFinnhub(lCurrentTime);
		if (IsFinnhubInquiring()) {
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

////////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub��ȡ������������ͽ����ֻ�ִ�з�ʽ���ʶ����ٵ������β����һ���ֻء�
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::ProcessFinnhubInquiringMessage(void) {
	CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
	CString strTemp;
	CWorldStockPtr pStock = nullptr;
	CForexSymbolPtr pSymbol = nullptr;
	bool fDone = false;

	if (m_qFinnhubWebInquiry.size() > 0) { // ������ȴ���
		ASSERT(IsFinnhubInquiring());
		if (IsFinnhubDataReceived()) { //�Ѿ�����������������Finnhub�����Ѿ����յ��ˣ�
			m_CurrentFinnhubInquiry = GetFinnhubInquiry();
			gl_pFinnhubWebInquiry->SetInquiryingStringPrefix(m_vFinnhubInquiringStr.at(m_CurrentFinnhubInquiry.m_lInquiryIndex)); // ����ǰ׺

			switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) { // ���ù�Ʊָ��
			case __COMPANY_PROFILE__: // Premium ����˻��޷���ȡ����Ϣ��sandboxģʽ�ܶ�ȡ���������Ǵ���ģ�ֻ�����ڲ��ԡ�
			case __COMPANY_PROFILE_CONCISE__:
			case __PEERS__:
			case __INSIDER_TRANSACTION__:
			case __STOCK_EPS_SURPRISE__:
			case __STOCK_QUOTE__:
			case __STOCK_CANDLES__:
				pStock = m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex);
				break;
			default:
				break;
			}
			switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) { // ���ݲ�ͬ��Ҫ��������׺�ַ���
			case __COMPANY_PROFILE__: // Premium ����˻��޷���ȡ����Ϣ��sandboxģʽ�ܶ�ȡ���������Ǵ���ģ�ֻ�����ڲ��ԡ�
				ASSERT(pStock != nullptr);
				gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(pStock->GetSymbol());
				pStock->SetProfileUpdated(true);
				break;
			case __COMPANY_PROFILE_CONCISE__:
				ASSERT(pStock != nullptr);
				gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(pStock->GetSymbol());
				pStock->SetProfileUpdated(true);
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
				ASSERT(pStock != nullptr);
				gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(pStock->GetSymbol());
				pStock->SetPeerUpdated(true);
				break;
			case __BASIC_FINANCIALS__:
				break;
			case __INSIDER_TRANSACTION__:
				ASSERT(pStock != nullptr);
				gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(pStock->GetSymbol());
				pStock->SetInsiderTransactionNeedUpdate(false);
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
			case __IPO_CALENDAR__:
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
				ASSERT(pStock != nullptr);
				gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(pStock->GetSymbol());
				break;
			case __STOCK_EARNING_CALENDAR__:
				break;
			case __STOCK_QUOTE__:
				ASSERT(pStock != nullptr);
				strMiddle = pStock->GetSymbol();
				gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
				break;
			case __STOCK_CANDLES__:
				ASSERT(pStock != nullptr);
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
			fDone = true;
		}
	}

	return fDone;
}

bool CompareWorldStock(CWorldStockPtr p1, CWorldStockPtr p2) { return (p1->GetSymbol().Compare(p2->GetSymbol()) < 0); }

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
	vector<CInsiderTransactionPtr> vInsiderTransaction;
	bool fFoundNewStock = false;
	char buffer[30];
	CString strNumber;
	bool fDone = false;

	ASSERT(gl_WebInquirer.GetFinnhubDataSize() <= 1);
	if (IsFinnhubDataReceived()) { // ����������ݽ������
		if (gl_WebInquirer.GetFinnhubDataSize() > 0) {  // ����ǰ��������
			ASSERT(IsFinnhubInquiring());
			pWebData = gl_WebInquirer.PopFinnhubData();

			switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) { // ���ݲ�ͬ��Ҫ��������׺�ַ���
			case __COMPANY_PROFILE__: // Premium ����˻��޷���ȡ����Ϣ��sandboxģʽ�ܶ�ȡ���������Ǵ���ģ�ֻ�����ڲ��ԡ�
			case __COMPANY_PROFILE_CONCISE__:
			case __PEERS__:
			case __INSIDER_TRANSACTION__:
			case __STOCK_EPS_SURPRISE__:
			case __STOCK_QUOTE__:
			case __STOCK_CANDLES__:
				pStock = m_vWorldStock.at(m_CurrentFinnhubInquiry.m_lStockIndex);
				break;
			default:
				break;
			}
			switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) {
			case __COMPANY_PROFILE__: // Ŀǰ����˻��޷�ʹ�ô˹��ܡ�
				ASSERT(pStock != nullptr);
				if (ProcessFinnhubStockProfile(pWebData, pStock)) {
					pStock->SetProfileUpdateDate(gl_pWorldMarket->GetFormatedMarketDate());
					pStock->SetProfileUpdated(true);
					pStock->SetUpdateProfileDB(true);
				}
				break;
			case __COMPANY_PROFILE_CONCISE__:
				ASSERT(pStock != nullptr);
				if (ProcessFinnhubStockProfileConcise(pWebData, pStock)) {
					pStock->SetProfileUpdateDate(gl_pWorldMarket->GetFormatedMarketDate());
					pStock->SetProfileUpdated(true);
					pStock->SetUpdateProfileDB(true);
				}
				break;
			case  __COMPANY_SYMBOLS__:
				if (ProcessFinnhubStockSymbol(pWebData, vStock)) {
					//TRACE("����%s Finnhub Symbol����Ϊ%d\n", m_vFinnhubExchange.at(m_lCurrentExchangePos)->m_strCode, vStock.size());
					sprintf_s(buffer, _T("%6d"), (int)vStock.size());
					strNumber = buffer;
					str = _T("����") + m_vFinnhubExchange.at(m_lCurrentExchangePos)->m_strCode + _T(" Finnhub Symbol����Ϊ") + strNumber;
					gl_systemMessage.PushInnerSystemInformationMessage(str);
					// ���Ͻ��������롣
					for (auto& pStock3 : vStock) {
						pStock3->SetExchangeCode(m_vFinnhubExchange.at(m_lCurrentExchangePos)->m_strCode);
					}
					for (auto& pStock2 : vStock) {
						if (!IsStock(pStock2->GetSymbol())) {
							AddStock(pStock2);
							str = _T("Finnhub�����´���:") + pStock2->GetSymbol();
							gl_systemMessage.PushInnerSystemInformationMessage(str);
						}
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
				ASSERT(pStock != nullptr);
				if (ProcessFinnhubStockPeer(pWebData, pStock)) {
					pStock->SetPeerUpdateDate(GetFormatedMarketDate());
					pStock->SetUpdateProfileDB(true);
				}
				break;
			case __BASIC_FINANCIALS__:
				break;
			case __INSIDER_TRANSACTION__:
				if (ProcessFinnhubStockInsiderTransaction(pWebData, vInsiderTransaction)) {
					pStock->SetInsiderTransactionUpdateDate(GetFormatedMarketDate());
					pStock->SetUpdateProfileDB(true);
					if (vInsiderTransaction.size() > 0) {
						pStock->UpdateInsiderTransaction(vInsiderTransaction);
						pStock->SetInsiderTransactionNeedSave(true);
					}
				}
				break;
			case __STOCK_EPS_SURPRISE__:
				ASSERT(pStock != nullptr);
				if (ProcessFinnhubEPSSurprise(pWebData, vEPSSurprise)) {
					if (vEPSSurprise.size() > 0) {
						pStock->UpdateEPSSurprise(vEPSSurprise);
					}
					else {
						pStock->SetLastEPSSurpriseUpdateDate(19700101); // ����������Ϊ���硣
						pStock->SetUpdateProfileDB(true);
					}
					pStock->m_fEPSSurpriseUpdated = true;
					pStock->m_fEPSSurpriseNeedSave = true;
				}
				break;
			case __STOCK_QUOTE__:
				ASSERT(pStock != nullptr);
				ProcessFinnhubStockQuote(pWebData, pStock);
				if ((pStock->GetTransactionTime() + 3600 * 12 - GetMarketTime()) > 0) { // ����ʱ�䲻����12Сʱ�������ô˹�ƱΪ��Ծ��Ʊ
					pStock->SetActive(true);
					if (!pStock->IsIPOed()) {
						pStock->SetIPOStatus(__STOCK_IPOED__);
						pStock->SetUpdateProfileDB(true);
					}
				}
				break;
			case __STOCK_CANDLES__:
				ASSERT(pStock != nullptr);
				if (ProcessFinnhubStockCandle(pWebData, pStock)) {
					if (pStock->GetDayLineSize() == 0) { // û���������ݣ�
						if (pStock->IsNotChecked()) { // ��δȷ��������Ч�ԣ�
							pStock->SetIPOStatus(__STOCK_NULL__);
						}
					}
					else if (IsEarlyThen(pStock->GetDayLine(pStock->GetDayLineSize() - 1)->GetFormatedMarketDate(), GetFormatedMarketDate(), 100)) {
						pStock->SetIPOStatus(__STOCK_DELISTED__);
					}
					else {
						pStock->SetIPOStatus(__STOCK_IPOED__);
					}
					TRACE("����%s��������\n", pStock->GetSymbol().GetBuffer());
				}
				break;
			case __FOREX_EXCHANGE__:
				if (ProcessFinnhubForexExchange(pWebData, vExchange)) {
					for (int i = 0; i < vExchange.size(); i++) {
						if (!IsForexExchange(vExchange.at(i))) {
							AddForexExchange(vExchange.at(i));
						}
					}
					SetFinnhubForexExchangeUpdated(true);
				}
				break;
			case __FOREX_SYMBOLS__:
				if (ProcessFinnhubForexSymbol(pWebData, vForexSymbol)) {
					for (auto& pSymbol : vForexSymbol) {
						if (!IsForexSymbol(pSymbol->GetSymbol())) {
							pSymbol->SetExchangeCode(m_vForexExchange.at(m_CurrentFinnhubInquiry.m_lStockIndex));
							AddForexSymbol(pSymbol);
						}
					}
				}
				break;
			case __FOREX_CANDLES__:
				if (ProcessFinnhubForexCandle(pWebData, m_vForexSymbol.at(m_CurrentFinnhubInquiry.m_lStockIndex))) {
					TRACE("����%s��������\n", m_vForexSymbol.at(m_CurrentFinnhubInquiry.m_lStockIndex)->GetSymbol().GetBuffer());
				}
				break;
			case __FOREX_ALL_RATES__:
				break;
			case __ECONOMIC_COUNTRY_LIST__:
				ProcessFinnhubCountryList(pWebData, vCountry);
				for (auto& pCountry : vCountry) {
					if (!IsCountry(pCountry)) {
						AddCountry(pCountry);
					}
				}
				SetCountryListUpdated(true);
				break;
			case __ECONOMIC_CALENDAR__:
				ProcessFinnhubEconomicCalendar(pWebData, vEconomicCalendar);
				UpdateEconomicCalendar(vEconomicCalendar);
				SetFinnhubEconomicCalendarUpdated(true);
				break;
			default:
				break;
			}
			gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(_T("")); // ��Щ��������û���õ��м��ַ��Σ���������֮ǰ���м��ַ��Σ�����еĻ���������������ַ����Ĵ���
			SetFinnhubInquiring(false);
			fDone = true;
		}
	}

	return fDone;
}

bool CWorldMarket::ProcessTiingoInquiringMessage(void) {
	CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
	CString strTemp;
	CWorldStockPtr pStock = nullptr;
	CForexSymbolPtr pSymbol = nullptr;
	bool fDone = false;

	if (m_qTiingoWebInquiry.size() > 0) { // ������ȴ���
		ASSERT(IsTiingoInquiring());
		if (IsTiingoDataReceived()) { //�Ѿ�����������������Tiingo�����Ѿ����յ��ˣ�
			m_CurrentTiingoInquiry = GetTiingoInquiry();
			gl_pTiingoWebInquiry->SetInquiryingStringPrefix(m_vTiingoInquiringStr.at(m_CurrentTiingoInquiry.m_lInquiryIndex)); // ����ǰ׺
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
			case __INSIDER_TRANSACTION__:
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
			case __IPO_CALENDAR__:
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
			case __STOCK_EARNING_CALENDAR__:
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
			fDone = true;
		}
	}

	return fDone;
}

bool CWorldMarket::ProcessTiingoWebDataReceived(void) {
	CWebDataPtr pWebData = nullptr;
	CWorldStockPtr pStock = nullptr;
	CString str = _T(""), strNumber = _T("");
	vector<CString> vExchange;
	vector<CForexSymbolPtr> vForexSymbol;
	vector<CEconomicCalendarPtr> vEconomicCalendar;
	vector<CEPSSurprisePtr> vEPSSurprise;
	vector<CWorldStockPtr> vStock;
	vector<CTiingoStockPtr> vTiingoStock;
	long lTemp = 0;
	const bool fFoundNewStock = false;
	bool fDone = false;
	char buffer[50];

	ASSERT(gl_WebInquirer.GetTiingoDataSize() <= 1);
	if (IsTiingoDataReceived()) { // ����������ݽ������
		if (gl_WebInquirer.GetTiingoDataSize() > 0) {  // ����ǰ��������
			ASSERT(IsTiingoInquiring());
			pWebData = gl_WebInquirer.PopTiingoData();
			switch (m_CurrentTiingoInquiry.m_lInquiryIndex) {
			case __COMPANY_PROFILE__: // Ŀǰ����˻��޷�ʹ�ô˹��ܡ�
				break;
			case __COMPANY_PROFILE_CONCISE__:
				break;
			case  __COMPANY_SYMBOLS__:
				if (ProcessTiingoStockSymbol(pWebData, vTiingoStock)) {
					lTemp = 0;
					for (auto& pTiingoStock : vTiingoStock) {
						if (!IsTiingoStock(pTiingoStock->m_strTicker)) {
							m_vTiingoStock.push_back(pTiingoStock);
						}
						if (IsStock(pTiingoStock->m_strTicker)) { // Tiingo��Symbol��Ϣֻ������Finnhub��һ�����䡣
							lTemp++;
							pStock = m_vWorldStock.at(m_mapWorldStock.at(pTiingoStock->m_strTicker));
							pStock->UpdateStockProfile(pTiingoStock);
							pStock->SetUpdateProfileDB(true);
						}
						else { // new stock��
							// do nothing now.
							int iiii = 0;
							iiii++; // for debug
						}
					}
					TRACE("����Tiingo��Ծ��Ʊ��Ϊ��%d\n", lTemp);
					sprintf_s(buffer, _T("%6d"), lTemp);
					strNumber = buffer;
					str = _T("����Tiingo Symbol��Ծ��Ʊ����Ϊ") + strNumber;
					gl_systemMessage.PushInnerSystemInformationMessage(str);
				}
				SetTiingoSymbolUpdated(true);
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
			case __INSIDER_TRANSACTION__:
				break;
			case __STOCK_EPS_SURPRISE__:
				break;
			case __STOCK_QUOTE__:
				break;
			case __STOCK_CANDLES__:
				pStock = m_vWorldStock.at(m_CurrentTiingoInquiry.m_lStockIndex);
				ProcessTiingoStockDayLine(pWebData, pStock);
				TRACE("����Tiingo %s��������\n", pStock->GetSymbol().GetBuffer());
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
			SetTiingoInquiring(false);
			fDone = true;
		}
	}

	return fDone;
}

bool CWorldMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
	static int s_iCount1Hour = 3599;
	static int s_iCount5Minute = 299;
	static int s_iCount1Minute = 59;
	static int s_iCount10Second = 9;

	s_iCount10Second -= lSecond;
	s_iCount1Minute -= lSecond;
	s_iCount5Minute -= lSecond;
	s_iCount1Hour -= lSecond;
	if (s_iCount1Hour < 0) {
		s_iCount1Hour = 3599;
		SchedulingTaskPerHour(lCurrentTime);
	}
	if (s_iCount5Minute < 0) {
		s_iCount5Minute = 299;
		SchedulingTaskPer5Minute(lCurrentTime);
	}
	if (s_iCount1Minute < 0) {
		s_iCount1Minute = 59;
		SchedulingTaskPerMinute(lCurrentTime);
	}
	if (s_iCount10Second < 0) {
		s_iCount10Second = 9;
		SchedulingTaskPer10Seconds(lCurrentTime);
	}

	TaskProcessWebSocketData();

	static bool sm_fConnectedFinnhubWebSocket = false;
	static bool sm_fSendFinnhubWebStocketMessage = false;

	static bool sm_fConnectedTiingoIEXWebSocket = false;
	static bool sm_fSendTiingoIEXWebStocketMessage = false;
	static bool sm_fConnectedTiingoCryptoWebSocket = false;
	static bool sm_fSendTiingoCryptoWebStocketMessage = false;
	static bool sm_fConnectedTiingoForexWebSocket = false;
	static bool sm_fSendTiingoForexWebStocketMessage = false;

	if (!sm_fConnectedFinnhubWebSocket) {
		if (m_FinnhubWebSocket.getReadyState() == ix::ReadyState::Closed) {
			sm_fConnectedFinnhubWebSocket = true;
			//ConnectFinnhubWebSocket();
		}
	}

	if (!sm_fSendFinnhubWebStocketMessage) {
		if (m_FinnhubWebSocket.getReadyState() == ix::ReadyState::Open) {
			sm_fSendFinnhubWebStocketMessage = true;
			//SendFinnhubWebSocketMessage();
		}
	}

	if (!sm_fConnectedTiingoIEXWebSocket) {
		if (m_TiingoIEXWebSocket.getReadyState() == ix::ReadyState::Closed) {
			sm_fConnectedTiingoIEXWebSocket = true;
			//ConnectTiingoIEXWebSocket();
		}
	}

	if (!sm_fSendTiingoIEXWebStocketMessage) {
		if (m_TiingoIEXWebSocket.getReadyState() == ix::ReadyState::Open) {
			sm_fSendTiingoIEXWebStocketMessage = true;
			//SendTiingoIEXWebSocketMessage();
		}
	}

	if (!sm_fConnectedTiingoCryptoWebSocket) {
		if (m_TiingoCryptoWebSocket.getReadyState() == ix::ReadyState::Closed) {
			sm_fConnectedTiingoCryptoWebSocket = true;
			//ConnectTiingoCryptoWebSocket();
		}
	}

	if (!sm_fSendTiingoCryptoWebStocketMessage) {
		if (m_TiingoCryptoWebSocket.getReadyState() == ix::ReadyState::Open) {
			sm_fSendTiingoCryptoWebStocketMessage = true;
			//SendTiingoCryptoWebSocketMessage();
		}
	}

	if (!sm_fConnectedTiingoForexWebSocket) {
		if (m_TiingoForexWebSocket.getReadyState() == ix::ReadyState::Closed) {
			sm_fConnectedTiingoForexWebSocket = true;
			//ConnectTiingoForexWebSocket();
		}
	}

	if (!sm_fSendTiingoForexWebStocketMessage) {
		if (m_TiingoForexWebSocket.getReadyState() == ix::ReadyState::Open) {
			sm_fSendTiingoForexWebStocketMessage = true;
			//SendTiingoForexWebSocketMessage();
		}
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
	TaskUpdateInsiderTransactionDB();
	TaskUpdateForexDayLineDB();
	TaskUpdateDayLineDB();
	TaskUpdateEPSSurpriseDB();
	TaskUpdateEconomicCalendarDB();

	return true;
}

bool CWorldMarket::SchedulingTaskPer5Minute(long lCurrentTime) {
	if (IsFinnhubSymbolUpdated() && IsStockProfileNeedUpdate()) {
		TaskUpdateStockProfileDB();
	}

	TaskUpdateTiingoStockDB();

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
		if ((lCurrentTime > 170000) && (lCurrentTime <= 170100)) { // ���г�ʱ���������ʱ(����ʱ��������ʱ�������г������������ڽ����������ݡ�
			SetResetMarket(true);// ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
			SetPermitResetMarket(false); // ���첻����������ϵͳ��
			SetSystemReady(false);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// �˺�����SchedulingTask���ȣ�ÿ1500��������ִ��һ�Ρ�
//
//////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskInquiryFinnhub(long lCurrentTime) {
	if (((lCurrentTime < 165700) || (lCurrentTime > 170200))) { // ������ʱ����ϵͳ���ʶ���ʱ���������������Ϣ��
		TaskInquiryFinnhubCountryList();
		TaskInquiryFinnhubForexExchange();
		TaskInquiryFinnhubCompanySymbol(); // ��һ���������������뵱��֤ȯ����
		TaskInquiryFinnhubForexSymbol();
		//TaskInquiryFinnhubEconomicCalendar();

		// ����Finnhub������Ϣ�����񣬽�Ҫ����������Ա�֤���г�ʱ���賿ʮ���Ӻ�ִ�С������ܹ���֤�������г�ʱû��ִ�в�ѯ����
		if (IsSystemReady()) {
			TaskInquiryFinnhubCompanyProfileConcise();
			TaskInquiryFinnhubPeer();
			TaskInquiryFinnhubInsiderTransaction();
			//TaskInquiryFinnhubEPSSurprise(); // �������ûʲô�ã���ʱֹͣ���¡�
			TaskInquiryFinnhubForexDayLine();
			TaskInquiryFinnhubDayLine();
			if (!IsFinnhubDayLineUpdated()) {
				//TaskInquiryFinnhubRTQuote();
			}
		}
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCountryList(void) {
	WebInquiry inquiry{ 0, 0, 0 };

	if (!IsCountryListUpdated() && !IsFinnhubInquiring()) {
		inquiry.m_lInquiryIndex = __ECONOMIC_COUNTRY_LIST__;
		inquiry.m_iPriority = 10;
		m_qFinnhubWebInquiry.push(inquiry);
		SetFinnhubInquiring(true);
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
	bool fHaveInquiry = false;

	if (!IsFinnhubSymbolUpdated() && !IsFinnhubInquiring()) {
		for (m_lCurrentExchangePos = 0; m_lCurrentExchangePos < lExchangeSize; m_lCurrentExchangePos++) {
			if (!m_vFinnhubExchange.at(m_lCurrentExchangePos)->IsUpdated()) {
				pExchange = m_vFinnhubExchange.at(m_lCurrentExchangePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			inquiry.m_lInquiryIndex = __COMPANY_SYMBOLS__;
			inquiry.m_lStockIndex = m_lCurrentExchangePos;
			inquiry.m_iPriority = 10;
			m_qFinnhubWebInquiry.push(inquiry);
			SetFinnhubInquiring(true);
			pExchange->SetUpdated(true);
			//TRACE("����%s������֤ȯ����\n", pExchange->m_strCode.GetBuffer());
		}
		else {
			fHaveInquiry = false;
			SetFinnhubSymbolUpdated(true);
			TRACE("Finnhub�������������ݲ�ѯ���\n");
			str = _T("�������������ݲ�ѯ���");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskUpdateForexSymbolDB(void) {
	CreatingThreadUpdateForexSymbolDB();
	return true;
}

bool CWorldMarket::TaskInquiryFinnhubCompanyProfileConcise(void) {
	bool fFound = false;
	WebInquiry inquiry{ 0, 0, 0 };
	long lStockSetSize = m_vWorldStock.size();
	CString str = _T("");
	bool fHaveInquiry = false;

	ASSERT(IsSystemReady());
	if (!IsFinnhubStockProfileUpdated() && !IsFinnhubInquiring()) {
		for (m_lCurrentProfilePos = 0; m_lCurrentProfilePos < lStockSetSize; m_lCurrentProfilePos++) {
			if (!m_vWorldStock.at(m_lCurrentProfilePos)->IsProfileUpdated()) {
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
			SetFinnhubInquiring(true);
			fHaveInquiry = true;
		}
		else {
			SetFinnhubStockProfileUpdated(true);
			TRACE("Finnhub��Ʊ���������\n");
			str = _T("Finnhub��Ʊ���������");
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubDayLine(void) {
	bool fFound = false;
	WebInquiry inquiry{ 0, 0, 0 };
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = m_vWorldStock.size();
	bool fHaveInquiry = false;

	ASSERT(IsSystemReady());
	if (!IsFinnhubDayLineUpdated() && !IsFinnhubInquiring()) {
		for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < lStockSetSize; m_lCurrentUpdateDayLinePos++) {
			pStock = m_vWorldStock.at(m_lCurrentUpdateDayLinePos);
			if (pStock->IsUSMarket() && pStock->IsDayLineNeedUpdate()) { // Ŀǰ����˻�ֻ�����������г��Ĺ�Ʊ���ߡ�
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			inquiry.m_lInquiryIndex = __STOCK_CANDLES__;
			inquiry.m_lStockIndex = m_lCurrentUpdateDayLinePos;
			inquiry.m_iPriority = 10;
			m_qFinnhubWebInquiry.push(inquiry);
			SetFinnhubInquiring(true);
			pStock->SetDayLineNeedUpdate(false);
			TRACE("����%s��������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			fHaveInquiry = false;
			SetFinnhubDayLineUpdated(true);
			m_lCurrentUpdateDayLinePos = 0; // ���ô����������е��������ݸ���һ������ʱ��Ҫ����24Сʱ���ʱ��ָ��¼��ɡ�
			TRACE("Finnhub���߸�����ϣ����¿�ʼ����\n");
			str = _T("US Market������ʷ���ݸ������");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubInsiderTransaction(void) {
	bool fFound = false;
	WebInquiry inquiry{ 0, 0, 0 };
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = m_vWorldStock.size();
	bool fHaveInquiry = false;

	ASSERT(IsSystemReady());
	if (!IsFinnhubInsiderTransactionUpdated() && !IsFinnhubInquiring()) {
		for (m_lCurrentUpdateInsiderTransactionPos = 0; m_lCurrentUpdateInsiderTransactionPos < lStockSetSize; m_lCurrentUpdateInsiderTransactionPos++) {
			pStock = m_vWorldStock.at(m_lCurrentUpdateInsiderTransactionPos);
			if (pStock->IsUSMarket()) {
				if (pStock->IsInsiderTransactionNeedUpdate()) { // Ŀǰ����˻�ֻ�����������г��Ĺ�Ʊ���ߡ�
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			inquiry.m_lInquiryIndex = __INSIDER_TRANSACTION__;
			inquiry.m_lStockIndex = m_lCurrentUpdateInsiderTransactionPos;
			inquiry.m_iPriority = 10;
			m_qFinnhubWebInquiry.push(inquiry);
			SetFinnhubInquiring(true);
			TRACE("����%s �ڲ���������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			fHaveInquiry = false;
			SetFinnhubInsiderTransactionUpdated(true);
			TRACE("FinnhubInsider Transaction�������\n");
			str = _T("US Market Insider Transaction���ݸ������");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubRTQuote(void) {
	WebInquiry inquiry{ 0, 0, 0 };

	ASSERT(IsSystemReady());
	if (!IsFinnhubInquiring()) {
		m_lCurrentRTDataQuotePos++;
		if (m_lCurrentRTDataQuotePos == m_vWorldStock.size()) m_lCurrentRTDataQuotePos = 0;
		inquiry.m_lInquiryIndex = __STOCK_QUOTE__;
		inquiry.m_lStockIndex = m_lCurrentRTDataQuotePos;
		inquiry.m_iPriority = 10;
		m_qFinnhubWebInquiry.push(inquiry);
		SetFinnhubInquiring(true);
		TRACE("����%sʵʱ����\n", m_vWorldStock.at(m_lCurrentRTDataQuotePos)->GetSymbol().GetBuffer());
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubPeer(void) {
	bool fFound = false;
	WebInquiry inquiry{ 0, 0, 0 };
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = m_vWorldStock.size();
	bool fHaveInquiry = false;

	ASSERT(IsSystemReady());
	if (!IsFinnhubPeerUpdated() && !IsFinnhubInquiring()) {
		for (m_lCurrentUpdatePeerPos = 0; m_lCurrentUpdatePeerPos < lStockSetSize; m_lCurrentUpdatePeerPos++) {
			if (!m_vWorldStock.at(m_lCurrentUpdatePeerPos)->IsPeerUpdated()) {
				pStock = m_vWorldStock.at(m_lCurrentUpdatePeerPos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			inquiry.m_lInquiryIndex = __PEERS__;
			inquiry.m_lStockIndex = m_lCurrentUpdatePeerPos;
			inquiry.m_iPriority = 10;
			m_qFinnhubWebInquiry.push(inquiry);
			SetFinnhubInquiring(true);
			TRACE("����%s Peer����\n", m_vWorldStock.at(m_lCurrentUpdatePeerPos)->GetSymbol().GetBuffer());
		}
		else {
			fHaveInquiry = false;
			SetFinnhubPeerUpdated(true);
			TRACE("Finnhub Peers�������\n");
			str = _T("Finnhub Peer Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubEconomicCalendar(void) {
	WebInquiry inquiry{ 0, 0, 0 };

	if (!IsFinnhubEconomicCalendarUpdated() && !IsFinnhubInquiring()) {
		inquiry.m_lInquiryIndex = __ECONOMIC_CALENDAR__;
		inquiry.m_iPriority = 10;
		m_qFinnhubWebInquiry.push(inquiry);
		SetFinnhubInquiring(true);
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
	bool fHaveInquiry = false;

	ASSERT(IsSystemReady());
	if (!IsFinnhubEPSSurpriseUpdated() && !IsFinnhubInquiring()) {
		for (m_lCurrentUpdateEPSSurprisePos = 0; m_lCurrentUpdateEPSSurprisePos < lStockSetSize; m_lCurrentUpdateEPSSurprisePos++) {
			if (!m_vWorldStock.at(m_lCurrentUpdateEPSSurprisePos)->IsEPSSurpriseUpdated()) {
				pStock = m_vWorldStock.at(m_lCurrentUpdateEPSSurprisePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			inquiry.m_lInquiryIndex = __STOCK_EPS_SURPRISE__;
			inquiry.m_lStockIndex = m_lCurrentUpdateEPSSurprisePos;
			inquiry.m_iPriority = 10;
			m_qFinnhubWebInquiry.push(inquiry);
			SetFinnhubInquiring(true);
			m_vWorldStock.at(m_lCurrentUpdateEPSSurprisePos)->SetEPSSurpriseUpdated(true);
			TRACE("����%s EPS Surprise����\n", m_vWorldStock.at(m_lCurrentUpdateEPSSurprisePos)->GetSymbol().GetBuffer());
		}
		else {
			SetFinnhubEPSSurpriseUpdated(true);
			TRACE("Finnhub EPS Surprise�������\n");
			str = _T("Finnhub EPS Surprise Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubForexExchange(void) {
	WebInquiry inquiry{ 0, 0, 0 };

	if (!IsFinnhubForexExchangeUpdated() && !IsFinnhubInquiring()) {
		inquiry.m_lInquiryIndex = __FOREX_EXCHANGE__;
		inquiry.m_iPriority = 10;
		m_qFinnhubWebInquiry.push(inquiry);
		SetFinnhubInquiring(true);
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubForexSymbol(void) {
	WebInquiry inquiry{ 0, 0, 0 };

	if (!IsFinnhubForexSymbolUpdated() && !IsFinnhubInquiring()) {
		inquiry.m_lInquiryIndex = __FOREX_SYMBOLS__;
		inquiry.m_lStockIndex = m_lCurrentForexExchangePos++;
		inquiry.m_iPriority = 10;
		m_qFinnhubWebInquiry.push(inquiry);
		SetFinnhubInquiring(true);
		if (m_lCurrentForexExchangePos >= m_vForexExchange.size()) {
			SetFinnhubForexSymbolUpdated(true);
			m_lCurrentForexExchangePos = 0;
		}
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubForexDayLine(void) {
	bool fFound = false;
	WebInquiry inquiry{ 0, 0, 0 };
	CForexSymbolPtr pForexSymbol;
	CString str = _T("");
	const long lStockSetSize = m_vForexSymbol.size();
	bool fHaveInquiry = false;

	ASSERT(IsSystemReady());
	if (!IsFinnhubForexDayLineUpdated() && !IsFinnhubInquiring()) {
		for (m_lCurrentUpdateForexDayLinePos = 0; m_lCurrentUpdateForexDayLinePos < lStockSetSize; m_lCurrentUpdateForexDayLinePos++) {
			if (GetForexSymbol(m_lCurrentUpdateForexDayLinePos)->IsDayLineNeedUpdate()) {
				pForexSymbol = GetForexSymbol(m_lCurrentUpdateForexDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			inquiry.m_lInquiryIndex = __FOREX_CANDLES__;
			inquiry.m_lStockIndex = m_lCurrentUpdateForexDayLinePos;
			inquiry.m_iPriority = 10;
			m_qFinnhubWebInquiry.push(inquiry);
			SetFinnhubInquiring(true);
			pForexSymbol->SetDayLineNeedUpdate(false);
			TRACE("����Forex%s��������\n", pForexSymbol->GetSymbol().GetBuffer());
		}
		else {
			SetFinnhubForexDayLineUpdated(true);
			TRACE("Finnhub Forex���߸������\n");
			str = _T("Forex DayLine Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryTiingo(void) {
	if (IsSystemReady()) {
		TaskInquiryTiingoCompanySymbol();
		// ����Tiingo�涨ÿ��ֻ�ܲ�ѯ500�����룬�ʲ��Գɹ�����ʱ��ʹ�á�
		TaskInquiryTiingoDayLine(); // ����������ϡ�
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryTiingoCompanySymbol(void) {
	WebInquiry inquiry{ 0, 0, 0 };
	CString str;

	if (!IsTiingoSymbolUpdated() && !IsTiingoInquiring()) {
		inquiry.m_lInquiryIndex = __COMPANY_SYMBOLS__;
		inquiry.m_iPriority = 10;
		PushTiingoInquiry(inquiry);
		SetTiingoInquiring(true);
		str = _T("Inquiry Tiingo Symbol");
		gl_systemMessage.PushInformationMessage(str);

		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo����������������Finnhub���������غ�����ֻ����ͬʱ��������֮һ��
// Ŀǰ�˹���ֻ����������ѡ��Ʊ���������ݡ�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskInquiryTiingoDayLine(void) {
	bool fFound = false;
	WebInquiry inquiry{ 0, 0, 0 };
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = m_vWorldChoicedStock.size();
	bool fHaveInquiry = false;

	ASSERT(IsSystemReady());
	if (!IsTiingoDayLineUpdated() && !IsTiingoInquiring()) {
		for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < lStockSetSize; m_lCurrentUpdateDayLinePos++) {
			if (m_vWorldChoicedStock.at(m_lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
				pStock = m_vWorldChoicedStock.at(m_lCurrentUpdateDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			inquiry.m_lInquiryIndex = __STOCK_CANDLES__;
			inquiry.m_lStockIndex = m_mapWorldStock.at(pStock->GetSymbol());
			inquiry.m_iPriority = 10;
			PushTiingoInquiry(inquiry);
			SetTiingoInquiring(true);
			pStock->SetDayLineNeedUpdate(false);
			TRACE("����Tiingo %s��������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			SetTiingoDayLineUpdated(true);
			TRACE("Tiingo���߸������\n");
			str = _T("�����г���ѡ��Ʊ������ʷ���ݸ������");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskUpdateTiingoIndustry(void) {
	if (IsFinnhubStockProfileUpdated()) {
		CreatingThreadUpdateTiingoIndustry();
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateSICIndustry(void) {
	if (IsFinnhubStockProfileUpdated()) {
		CreatingThreadUpdateSICIndustry();
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateNaicsIndustry(void) {
	if (IsFinnhubStockProfileUpdated()) {
		CreatingThreadUpdateNaicsIndustry();
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateForexExchangeDB(void) {
	CreatingThreadUpdateForexExchangeDB();

	return true;
}

bool CWorldMarket::UpdateForexExchangeDB(void) {
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
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateStockProfileDB(void) {
	CreatingThreadUpdateStockProfileDB();
	return true;
}

bool CWorldMarket::TaskUpdateDayLineDB(void) {
	CreatingThreadUpdateDayLineDB();

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
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskUpdateForexDayLineDB(void) {
	CString str;
	bool fUpdated = false;

	for (auto& pSymbol : m_vForexSymbol) {
		if (gl_fExitingSystem) {
			break; // ������������˳�����ֹͣ�洢��
		}
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					CreatingThreadUpdateForexDayLineDB(pSymbol.get());
					fUpdated = true;
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
	}

	return(fUpdated);
}

bool CWorldMarket::TaskUpdateCountryListDB(void) {
	CreatingThreadUpdateCountryListDB();
	return true;
}

bool CWorldMarket::TaskUpdateEPSSurpriseDB(void) {
	CString str;

	for (auto& pStock : m_vWorldStock) {
		if (pStock->IsEPSSurpriseNeedSaveAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			CreatingThreadUpdateEPSSurpriseDB(pStock.get());
			TRACE("����%s EPS surprise����\n", pStock->GetSymbol().GetBuffer());
		}
		if (gl_fExitingSystem) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}

	return(true);
}

bool CWorldMarket::TaskUpdateEconomicCalendarDB(void) {
	CreatingThreadUpdateEconomicCalendarDB();

	return true;
}

bool CWorldMarket::TaskUpdateInsiderTransactionDB(void) {
	CreatingThreadUpdateInsiderTransactionDB();
	return true;
}

bool CWorldMarket::TaskUpdateTiingoStockDB(void) {
	CreatingThreadUpdateTiingoStockDB();
	return true;
}

bool CWorldMarket::TaskCheckSystemReady(void) {
	CString str = _T("");

	if (!IsSystemReady()) {
		if (IsFinnhubSymbolUpdated() && IsFinnhubForexExchangeUpdated() && IsFinnhubForexSymbolUpdated()) {
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
	RunningthreadUpdateDayLneStartEndDate();
	return true;
}

bool CWorldMarket::RunningthreadUpdateDayLneStartEndDate(void) {
	thread thread1(ThreadUpdateWorldStockDayLineStartEndDate, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateDayLineDB() {
	thread thread1(ThreadUpdateWorldStockDayLineDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateStockProfileDB(void) {
	thread thread1(ThreadUpdateStockProfileDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateForexDayLineDB(CFinnhubForexSymbol* pSymbol) {
	thread thread1(ThreadUpdateForexDayLineDB, pSymbol);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateForexSymbolDB() {
	thread thread1(ThreadUpdateForexSymbolDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateCountryListDB(void) {
	thread thread1(ThreadUpdateCountryListDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateEPSSurpriseDB(CWorldStock* pStock) {
	thread thread1(ThreadUpdateEPSSurpriseDB, pStock);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateInsiderTransactionDB(void) {
	thread thread1(ThreadUpdateInsiderTransactionDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateTiingoStockDB(void) {
	thread thread1(ThreadUpdateTiingoStockDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateTiingoIndustry(void) {
	thread thread1(ThreadUpdateTiingoIndustry, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateSICIndustry(void) {
	thread thread1(ThreadUpdateSICIndustry, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateNaicsIndustry(void) {
	thread thread1(ThreadUpdateNaicsIndustry, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateForexExchangeDB(void) {
	thread thread1(ThreadUpdateForexExchangeDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateEconomicCalendarDB(void) {
	thread thread1(ThreadUpdateEconomicCalendarDB, this);
	thread1.detach();

	return true;
}

bool CWorldMarket::IsStockProfileNeedUpdate(void) {
	const int iTotal = m_vWorldStock.size();
	for (int i = 0; i < iTotal; i++) {
		if (m_vWorldStock.at(i)->IsUpdateProfileDB()) return true;
	}
	return false;
}

void CWorldMarket::AddStock(CWorldStockPtr pStock) {
	m_mapWorldStock[pStock->GetSymbol()] = m_vWorldStock.size();
	m_vWorldStock.push_back(pStock);
	pStock->SetTodayNewStock(true);
	pStock->SetUpdateProfileDB(true);
}

bool CWorldMarket::DeleteStock(CWorldStockPtr pStock) {
	if (pStock == nullptr) return false;
	if (!IsStock(pStock->GetSymbol())) return false;

	m_mapWorldStock.erase(pStock->GetSymbol());
	auto it = find(m_vWorldStock.begin(), m_vWorldStock.end(), pStock);
	m_vWorldStock.erase(it);

	return true;
}

void CWorldMarket::AddTiingoStock(CTiingoStockPtr pTiingoStock) {
	m_mapTiingoStock[pTiingoStock->m_strTicker] = m_vTiingoStock.size();
	m_vTiingoStock.push_back(pTiingoStock);
}

bool CWorldMarket::DeleteTiingoStock(CTiingoStockPtr pStock) {
	if (pStock == nullptr) return false;
	if (!IsTiingoStock(pStock->m_strTicker)) return false;

	m_mapTiingoStock.erase(pStock->m_strTicker);
	auto it = find(m_vTiingoStock.begin(), m_vTiingoStock.end(), pStock);
	m_vTiingoStock.erase(it);

	return true;
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

void CWorldMarket::AddForexExchange(CString strForexExchange) {
	m_mapForexExchange[strForexExchange] = m_vForexExchange.size();
	m_vForexExchange.push_back(strForexExchange);
}

bool CWorldMarket::DeleteForexExchange(CString strForexExchange) {
	if (!IsForexExchange(strForexExchange)) return false;

	auto it = find(m_vForexExchange.begin(), m_vForexExchange.end(), strForexExchange);
	m_vForexExchange.erase(it);
	m_mapForexExchange.erase(strForexExchange);

	return true;
}

void CWorldMarket::AddForexSymbol(CForexSymbolPtr pForexSymbol) {
	m_mapForexSymbol[pForexSymbol->GetSymbol()] = m_mapForexSymbol.size();
	m_vForexSymbol.push_back(pForexSymbol);
}

bool CWorldMarket::DeleteForexSymbol(CForexSymbolPtr pForexSymbol) {
	if (pForexSymbol == nullptr) return false;
	if (!IsForexSymbol(pForexSymbol->GetSymbol())) return false;

	m_mapForexSymbol.erase(pForexSymbol->GetSymbol());
	auto it = find(m_vForexSymbol.begin(), m_vForexSymbol.end(), pForexSymbol);
	m_vForexSymbol.erase(it);

	return true;
}

bool CWorldMarket::IsCountry(CString strCountry) {
	if (m_mapCountry.find(strCountry) == m_mapCountry.end()) {
		return false;
	}
	else return true;
}

bool CWorldMarket::IsCountry(CCountryPtr pCountry) {
	return IsCountry(pCountry->m_strCountry);
}

void CWorldMarket::AddCountry(CCountryPtr pCountry) {
	m_mapCountry[pCountry->m_strCountry] = m_vCountry.size();
	m_vCountry.push_back(pCountry);
}

bool CWorldMarket::DeleteCountry(CCountryPtr pCountry) {
	if (pCountry == nullptr) return false;
	if (!IsCountry(pCountry->m_strCountry)) return false;

	m_mapCountry.erase(pCountry->m_strCountry);
	auto it = find(m_vCountry.begin(), m_vCountry.end(), pCountry);
	m_vCountry.erase(it);

	return true;
}

WebInquiry CWorldMarket::GetFinnhubInquiry(void) {
	ASSERT(m_qFinnhubWebInquiry.size() > 0);

	WebInquiry inquiry = m_qFinnhubWebInquiry.top();
	m_qFinnhubWebInquiry.pop();

	return inquiry;
}

WebInquiry CWorldMarket::GetTiingoInquiry(void) {
	ASSERT(m_qTiingoWebInquiry.size() > 0);

	WebInquiry inquiry = m_qTiingoWebInquiry.top();
	m_qTiingoWebInquiry.pop();

	return inquiry;
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

bool CWorldMarket::LoadStockDB(void) {
	CSetWorldStock setWorldStock;
	CWorldStockPtr pWorldStock = nullptr;
	CString str;
	long lSymbolLength = 0;

	setWorldStock.m_strSort = _T("[Symbol]");
	setWorldStock.Open();
	setWorldStock.m_pDatabase->BeginTrans();
	while (!setWorldStock.IsEOF()) {
		pWorldStock = make_shared<CWorldStock>();
		pWorldStock->Load(setWorldStock);
		if (!IsStock(pWorldStock->GetSymbol())) {
			pWorldStock->CheckProfileUpdateStatus(GetFormatedMarketDate());
			pWorldStock->CheckDayLineUpdateStatus(GetFormatedMarketDate(), GetLastTradeDate(), GetFormatedMarketTime(), GetDayOfWeek());
			pWorldStock->CheckEPSSurpriseStatus(GetFormatedMarketDate());
			pWorldStock->CheckPeerStatus(GetFormatedMarketDate());
			pWorldStock->CheckInsiderTransactionStatus(GetFormatedMarketDate());
			m_mapWorldStock[setWorldStock.m_Symbol] = m_lLastTotalWorldStock++;
			m_vWorldStock.push_back(pWorldStock);
			if (pWorldStock->GetCurrency().GetLength() > lSymbolLength) {
				lSymbolLength = pWorldStock->GetCurrency().GetLength();
			}
		}
		else {
			str = _T("�����ظ����룺");
			str += pWorldStock->GetSymbol();
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

	char buffer[100];
	sprintf_s(buffer, _T("%d"), lSymbolLength);
	str = _T("�����Ϊ");
	str += buffer;
	gl_systemMessage.PushInnerSystemInformationMessage(str);

	return true;
}

bool CWorldMarket::LoadWorldChoicedStock(void) {
	CSetWorldChoicedStock setWorldChoicedStock;
	CWorldStockPtr pStock = nullptr;

	setWorldChoicedStock.Open();
	setWorldChoicedStock.m_pDatabase->BeginTrans();
	while (!setWorldChoicedStock.IsEOF()) {
		if (IsStock(setWorldChoicedStock.m_Symbol)) {
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

///////////////////////////////////////////////////////////////////////////
//
// ���ָ��·�������Ĭ���µĹ��Ҵ��븽�������
//
//////////////////////////////////////////////////////////////////////////
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
bool CWorldMarket::UpdateStockProfileDB(void) {
	static bool sm_fInProcess = false;
	CWorldStockPtr pStock = nullptr;
	CSetWorldStock setWorldStock;
	int iUpdatedStock = 0;
	int iCount = 0;
	time_t tt = GetTickCount64();

	if (sm_fInProcess) {
		TRACE("�����������֮��û�����\n");
		return false;
	}
	else {
		sm_fInProcess = true;
	}

	//����ԭ�еĴ��뼯״̬
	if (IsStockProfileNeedUpdate()) {
		for (auto& pStock2 : m_vWorldStock) {
			if (pStock2->IsUpdateProfileDB()) iUpdatedStock++;
		}
		setWorldStock.m_strSort = _T("[Symbol]");
		setWorldStock.Open();
		setWorldStock.m_pDatabase->BeginTrans();
		while (iCount < iUpdatedStock) {
			if (setWorldStock.IsEOF()) break;
			pStock = m_vWorldStock.at(m_mapWorldStock.at(setWorldStock.m_Symbol));
			if (pStock->IsUpdateProfileDB()) {
				iCount++;
				pStock->Update(setWorldStock);
				pStock->SetUpdateProfileDB(false);
			}
			setWorldStock.MoveNext();
		}
		if (iCount < iUpdatedStock) {
			for (auto& pStock3 : m_vWorldStock) {
				if (pStock3->IsUpdateProfileDB()) {
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
	sm_fInProcess = false;
	tt = GetTickCount64() - tt;
	return true;
}

bool CWorldMarket::UpdateStockDayLineDB(void) {
	for (long i = 0; i < GetTotalStock(); i++) {
		GetStock(i)->UpdateDayLineDB();
		if (gl_fExitingSystem) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}
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

	for (auto& pSymbol2 : m_vForexSymbol) {
		if (pSymbol2->IsUpdateProfileDB()) {
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
				if (pSymbol->IsUpdateProfileDB()) {
					pSymbol->Update(setForexSymbol);
					pSymbol->SetUpdateProfileDB(false);
				}
			}
			setForexSymbol.MoveNext();
		}
		setForexSymbol.m_pDatabase->CommitTrans();
		setForexSymbol.Close();
	}

	return true;
}

bool CWorldMarket::UpdateInsiderTransactionDB(void) {
	CString str;
	CWorldStockPtr pStock = nullptr;

	for (long i = 0; i < GetTotalStock(); i++) {
		pStock = GetStock(i);
		if (pStock->IsInsiderTransactionNeedSaveAndClearFlag()) {
			if (pStock->HaveInsiderTransaction()) {
				pStock->SaveInsiderTransaction();
				str = pStock->GetSymbol() + _T("�ڲ��������ϸ������");
				gl_systemMessage.PushDayLineInfoMessage(str);
				TRACE("����%s�ڲ���������\n", pStock->GetSymbol().GetBuffer());
			}
		}
		if (gl_fExitingSystem) {
			break; // ������������˳�����ֹͣ�洢��
		}
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

bool CWorldMarket::UpdateTiingoStockDB(void) {
	CTiingoStockPtr pTiingoStock = nullptr;
	CSetTiingoStock setTiingoStock;

	if (m_lLastTotalTiingoStock < m_vTiingoStock.size()) {
		setTiingoStock.Open();
		setTiingoStock.m_pDatabase->BeginTrans();
		for (long l = m_lLastTotalTiingoStock; l < m_vTiingoStock.size(); l++) {
			pTiingoStock = m_vTiingoStock.at(l);
			pTiingoStock->Append(setTiingoStock);
		}
		setTiingoStock.m_pDatabase->CommitTrans();
		setTiingoStock.Close();
		m_lLastTotalTiingoStock = m_vTiingoStock.size();
	}

	return true;
}

bool CWorldMarket::UpdateTiingoIndustry(void) {
	// ��δʵ��
	return false;
}

bool CWorldMarket::UpdateSICIndustry(void) {
	// ��δʵ��
	return false;
}

bool CWorldMarket::UpdateNaicsIndustry(void) {
	// ��δʵ��
	return false;
}

bool CWorldMarket::RebuildEPSSurprise(void) {
	for (auto& p : m_vWorldStock) {
		p->SetLastEPSSurpriseUpdateDate(19800101);
		p->m_fEPSSurpriseUpdated = false;
	}
	SetFinnhubEPSSurpriseUpdated(false);
	m_lCurrentUpdateEPSSurprisePos = 0;

	return true;
}

bool CWorldMarket::RebuildPeer(void) {
	for (auto& pStock : m_vWorldStock) {
		if (pStock->GetPeerUpdateDate() != 19800101) {
			pStock->SetPeerUpdateDate(19800101);
			pStock->SetPeerUpdated(false);
			pStock->SetUpdateProfileDB(true);
		}
	}
	SetFinnhubPeerUpdated(false);
	m_lCurrentUpdatePeerPos = 0;

	return true;
}

bool CWorldMarket::SortStockVector(void) {
	sort(m_vWorldStock.begin(), m_vWorldStock.end(), CompareWorldStock);
	m_mapWorldStock.clear();
	int j = 0;
	for (auto& pStock : m_vWorldStock) {
		m_mapWorldStock[pStock->GetSymbol()] = j++;
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
		m_mapForexSymbol[pSymbol->GetSymbol()] = i++;
		setForexSymbol.MoveNext();
	}
	setForexSymbol.Close();
	m_lLastTotalForexSymbol = m_vForexSymbol.size();

	return true;
}

bool CWorldMarket::LoadCountryDB(void) {
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

bool CWorldMarket::LoadTiingoStock(void) {
	CSetTiingoStock setTiingoStock;
	CTiingoStockPtr pTiingoStock = nullptr;
	CString strSymbol = _T("");

	setTiingoStock.Open();
	while (!setTiingoStock.IsEOF()) {
		pTiingoStock = make_shared<CTiingoStock>();
		pTiingoStock->Load(setTiingoStock);
		m_mapTiingoStock[pTiingoStock->m_strTicker] = m_vTiingoStock.size();
		m_vTiingoStock.push_back(pTiingoStock);
		setTiingoStock.MoveNext();
	}
	setTiingoStock.Close();
	m_lLastTotalTiingoStock = m_vTiingoStock.size();

	return true;
}

bool CWorldMarket::RebuildStockDayLineDB(void) {
	for (auto& pStock : m_vWorldStock) {
		pStock->SetIPOStatus(__STOCK_NOT_CHECKED__);
		pStock->SetDayLineStartDate(29900101);
		pStock->SetDayLineEndDate(19800101);
		pStock->SetDayLineNeedUpdate(true);
		pStock->SetUpdateProfileDB(true);
	}
	SetFinnhubStockProfileUpdated(false);

	return true;
}

bool CWorldMarket::UpdateStockDayLineStartEndDate(void) {
	CString strFilterPrefix = _T("[Symbol] = '");
	CString strFilter, str;
	CSetWorldStockDayLine setWorldStockDayLine;
	CWorldStockPtr pStock2 = nullptr;

	for (auto& pStock : m_vWorldStock) {
		setWorldStockDayLine.m_strFilter = strFilterPrefix + pStock->GetSymbol() + _T("'");
		setWorldStockDayLine.m_strSort = _T("[Date]");
		setWorldStockDayLine.Open();
		if (!setWorldStockDayLine.IsEOF()) {
			if (setWorldStockDayLine.m_Date < pStock->GetDayLineStartDate()) {
				pStock->SetDayLineStartDate(setWorldStockDayLine.m_Date);
				pStock->SetUpdateProfileDB(true);
			}
			setWorldStockDayLine.MoveLast();
			if (setWorldStockDayLine.m_Date > pStock->GetDayLineEndDate()) {
				pStock->SetDayLineEndDate(setWorldStockDayLine.m_Date);
				pStock->SetUpdateProfileDB(true);
			}
		}
		setWorldStockDayLine.Close();
	}

	return true;
}

string strMessage;

/// <summary>
/// finnhub����Դ�ĸ�ʽ��wss://ws.finnhub.io/?token=c1i57rv48v6vit20lrc0��
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CWorldMarket::ConnectFinnhubWebSocket(void) {
	// Connect to a server with encryption
	// See https://machinezone.github.io/IXWebSocket/usage/#tls-support-and-configuration
	std::string url("wss://ws.finnhub.io");
	CString strToken = gl_pFinnhubWebInquiry->GetInquiringStringSuffix();
	strToken = "/?" + strToken.Right(strToken.GetLength() - 1);
	url += strToken.GetBuffer();

	ix::SocketTLSOptions TLSOption;
	TLSOption.tls = true;
	m_FinnhubWebSocket.setTLSOptions(TLSOption);

	m_FinnhubWebSocket.setUrl(url); // wss://ws.finnhub.io/?c1i57rv48v6vit20lrc0

	// Optional heart beat, sent every 45 seconds when there is not any traffic
	// to make sure that load balancers do not kill an idle connection.
	m_FinnhubWebSocket.setPingInterval(45);

	// Per message deflate connection is enabled by default. You can tweak its parameters or disable it
	m_FinnhubWebSocket.disablePerMessageDeflate();

	// Setup a callback to be fired when a message or an event (open, close, error) is received
	m_FinnhubWebSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
		{
			switch (msg->type) {
			case ix::WebSocketMessageType::Message:
				//gl_WebInquirer.pushFinnhubWebSocketData(msg->str);
				break;
			case ix::WebSocketMessageType::Error:
				gl_systemMessage.PushInnerSystemInformationMessage(msg->errorInfo.reason.c_str());
				break;
			case ix::WebSocketMessageType::Open:
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Finnhub WebSocket������"));
				break;
			case ix::WebSocketMessageType::Close:
				break;
			case ix::WebSocketMessageType::Fragment:
				break;
			case ix::WebSocketMessageType::Ping:
				break;
			case ix::WebSocketMessageType::Pong:
				break;
			default: // error
				break;
			}
		}
	);

	// Now that our callback is setup, we can start our background thread and receive messages
	m_FinnhubWebSocket.start();

	return true;
}

bool CWorldMarket::SendFinnhubWebSocketMessage(void)
{
	ix::WebSocketSendInfo info;

	ASSERT(m_FinnhubWebSocket.getReadyState() == ix::ReadyState::Open);

	// Send a message to the server (default to TEXT mode)
	info = m_FinnhubWebSocket.send(_T("{\"type\":\"subscribe\",\"symbol\":\"AAPL\"}")); // {"type":"subscribe","symbol":"AAPL"}
	info = m_FinnhubWebSocket.send(_T("{\"type\":\"subscribe\",\"symbol\":\"RIG\"}"));
	m_FinnhubWebSocket.send("{\"type\":\"subscribe\",\"symbol\":\"BINANCE:BTCUSDT\"}"); //{"type":"subscribe","symbol":"BINANCE:BTCUSDT"}
	m_FinnhubWebSocket.send("{\"type\":\"subscribe\",\"symbol\":\"BINANCE:LTCBTC\"}"); //{"type":"subscribe","symbol":"BINANCE:LTCBTC"}
	m_FinnhubWebSocket.send("{\"type\":\"subscribe\",\"symbol\":\"IC MARKETS:1\"}"); //
	m_FinnhubWebSocket.send("{\"type\":\"subscribe\",\"symbol\":\"OANDA:AUD_SGD\"}"); // OANDA:AUD_SGD
	m_FinnhubWebSocket.send("{\"type\":\"subscribe\",\"symbol\":\"FXCM:USD/JPY\"}"); // FXCM:USD/JPY

	return false;
}

/// <summary>
/// Tiingo IEX������Դ��ʽ��wss://api.tiingo.com/iex������Կ������͵ġ�
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CWorldMarket::ConnectTiingoIEXWebSocket(void) {
	// Connect to a server with encryption
	// See https://machinezone.github.io/IXWebSocket/usage/#tls-support-and-configuration
	std::string url("wss://api.tiingo.com/iex");

	ix::SocketTLSOptions TLSOption;
	TLSOption.tls = true;
	m_TiingoIEXWebSocket.setTLSOptions(TLSOption);

	m_TiingoIEXWebSocket.setUrl(url);

	// Optional heart beat, sent every 30 seconds when there is not any traffic
	// to make sure that load balancers do not kill an idle connection.
	m_TiingoIEXWebSocket.setPingInterval(30);

	// Per message deflate connection is enabled by default. You can tweak its parameters or disable it
	m_TiingoIEXWebSocket.disablePerMessageDeflate();

	// Setup a callback to be fired when a message or an event (open, close, error) is received
	m_TiingoIEXWebSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
		{
			switch (msg->type) {
			case ix::WebSocketMessageType::Message:
				gl_WebInquirer.pushTiingoIEXWebSocketData(msg->str);
				//gl_systemMessage.PushInnerSystemInformationMessage(msg->str.c_str());
				break;
			case ix::WebSocketMessageType::Error:
				gl_systemMessage.PushInnerSystemInformationMessage(msg->errorInfo.reason.c_str());
				break;
			case ix::WebSocketMessageType::Open:
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX WebSocket������"));
				break;
			case ix::WebSocketMessageType::Close:
				break;
			case ix::WebSocketMessageType::Fragment:
				break;
			case ix::WebSocketMessageType::Ping:
				break;
			case ix::WebSocketMessageType::Pong:
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX heartbeat"));
				break;
			default: // error
				break;
			}
		}
	);

	// Now that our callback is setup, we can start our background thread and receive messages
	m_TiingoIEXWebSocket.start();

	return true;
}

/// <summary>
/// Tiingo Crypto������Դ��ʽ��wss://api.tiingo.com/crypto������Կ������͵ġ�
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CWorldMarket::ConnectTiingoCryptoWebSocket(void) {
	// Connect to a server with encryption
	// See https://machinezone.github.io/IXWebSocket/usage/#tls-support-and-configuration
	std::string url("wss://api.tiingo.com/crypto");

	ix::SocketTLSOptions TLSOption;
	TLSOption.tls = true;
	m_TiingoCryptoWebSocket.setTLSOptions(TLSOption);

	m_TiingoCryptoWebSocket.setUrl(url);

	// Optional heart beat, sent every 30 seconds when there is not any traffic
	// to make sure that load balancers do not kill an idle connection.
	m_TiingoCryptoWebSocket.setPingInterval(30);

	// Per message deflate connection is enabled by default. You can tweak its parameters or disable it
	m_TiingoCryptoWebSocket.disablePerMessageDeflate();

	// Setup a callback to be fired when a message or an event (open, close, error) is received
	m_TiingoCryptoWebSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
		{
			switch (msg->type) {
			case ix::WebSocketMessageType::Message:
				//gl_WebInquirer.pushTiingoWebSocketData(msg->str);
				gl_systemMessage.PushInnerSystemInformationMessage(msg->str.c_str());
				break;
			case ix::WebSocketMessageType::Error:
				gl_systemMessage.PushInnerSystemInformationMessage(msg->errorInfo.reason.c_str());
				break;
			case ix::WebSocketMessageType::Open:
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto WebSocket������"));
				break;
			case ix::WebSocketMessageType::Close:
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto Close"));
				break;
			case ix::WebSocketMessageType::Fragment:
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto Fragment"));
				break;
			case ix::WebSocketMessageType::Ping:
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto ping"));
				break;
			case ix::WebSocketMessageType::Pong:
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto heartbeat"));
				break;
			default: // error
				break;
			}
		}
	);

	// Now that our callback is setup, we can start our background thread and receive messages
	m_TiingoCryptoWebSocket.start();

	return true;
}

/// <summary>
/// Tiingo Forex������Դ��ʽ��wss://api.tiingo.com/fx������Կ������͵ġ�
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CWorldMarket::ConnectTiingoForexWebSocket(void) {
	// Connect to a server with encryption
	// See https://machinezone.github.io/IXWebSocket/usage/#tls-support-and-configuration
	std::string url("wss://api.tiingo.com/fx");

	ix::SocketTLSOptions TLSOption;
	TLSOption.tls = true;
	m_TiingoForexWebSocket.setTLSOptions(TLSOption);

	m_TiingoForexWebSocket.setUrl(url);

	// Optional heart beat, sent every 30 seconds when there is not any traffic
	// to make sure that load balancers do not kill an idle connection.
	m_TiingoForexWebSocket.setPingInterval(30);

	// Per message deflate connection is enabled by default. You can tweak its parameters or disable it
	m_TiingoForexWebSocket.disablePerMessageDeflate();

	// Setup a callback to be fired when a message or an event (open, close, error) is received
	m_TiingoForexWebSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
		{
			switch (msg->type) {
			case ix::WebSocketMessageType::Message:
				gl_WebInquirer.pushTiingoIEXWebSocketData(msg->str);
				gl_systemMessage.PushInnerSystemInformationMessage(msg->str.c_str());
				break;
			case ix::WebSocketMessageType::Error:
				gl_systemMessage.PushInnerSystemInformationMessage(msg->errorInfo.reason.c_str());
				break;
			case ix::WebSocketMessageType::Open:
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Forex WebSocket������"));
				break;
			case ix::WebSocketMessageType::Close:
				break;
			case ix::WebSocketMessageType::Fragment:
				break;
			case ix::WebSocketMessageType::Ping:
				break;
			case ix::WebSocketMessageType::Pong:
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Forex heartbeat"));
				break;
			default: // error
				break;
			}
		}
	);

	// Now that our callback is setup, we can start our background thread and receive messages
	m_TiingoForexWebSocket.start();

	return true;
}

bool CWorldMarket::SendTiingoIEXWebSocketMessage(void)
{
	static bool sm_fSendAuth = true;
	CString str = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	//CString strSuffix = _T("\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":[\"aapl\",\"rig\"]}}"); // 5����������ݸ�ʽ��1������ϸ���ݸ�ʽ��
	CString strSuffix = _T("\",\"eventData\":{\"thresholdLevel\":5}}"); // 5����������ݸ�ʽ��1������ϸ���ݸ�ʽ��
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();
	strAuth = strAuth.Right(strAuth.GetLength() - 7);
	str += strAuth + strSuffix;

	string messageAuth(str);
	ix::WebSocketSendInfo info;

	ASSERT(m_TiingoIEXWebSocket.getReadyState() == ix::ReadyState::Open);

	if (sm_fSendAuth) {
		info = m_TiingoIEXWebSocket.send(messageAuth);
	}
	return false;
}

bool CWorldMarket::SendTiingoCryptoWebSocketMessage(void)
{
	static bool sm_fSendAuth = true;
	CString str = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strSuffix = _T("\",\"eventData\":{\"thresholdLevel\":5}}"); // 5����������ݸ�ʽ��1������ϸ���ݸ�ʽ��
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();
	strAuth = strAuth.Right(strAuth.GetLength() - 7);
	str += strAuth + strSuffix;

	string messageAuth(str);
	ix::WebSocketSendInfo info;

	ASSERT(m_TiingoCryptoWebSocket.getReadyState() == ix::ReadyState::Open);

	if (sm_fSendAuth) {
		info = m_TiingoCryptoWebSocket.send(messageAuth);
	}

	return false;
}

bool CWorldMarket::SendTiingoForexWebSocketMessage(void)
{
	static bool sm_fSendAuth = true;
	CString str = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strSuffix = _T("\",\"eventData\":{\"thresholdLevel\":5}}"); // 5����������ݸ�ʽ��1������ϸ���ݸ�ʽ��
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();
	strAuth = strAuth.Right(strAuth.GetLength() - 7);
	str += strAuth + strSuffix;

	string messageAuth(str);
	ix::WebSocketSendInfo info;

	ASSERT(m_TiingoForexWebSocket.getReadyState() == ix::ReadyState::Open);

	if (sm_fSendAuth) {
		info = m_TiingoForexWebSocket.send(messageAuth);
	}

	return false;
}

bool CWorldMarket::TaskProcessWebSocketData(void) {
	ProcessFinnhubWebSocketData();
	ProcessTiingoIEXWebSocketData();
	ProcessTiingoCryptoWebSocketData();
	ProcessTiingoForexWebSocketData();

	return true;
}

bool CWorldMarket::ProcessFinnhubWebSocketData() {
	auto total = gl_WebInquirer.GetFinnhubWebSocketDataSize();

	for (auto i = 0; i < total; i++) {
		auto pData = gl_WebInquirer.PopFinnhubWebSocketData();
		ProcessOneFinnhubWebSocketData(pData);
	}
	return true;
}

bool CWorldMarket::ProcessTiingoIEXWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoIEXWebSocketDataSize();

	for (auto i = 0; i < total; i++) {
		auto pData = gl_WebInquirer.PopTiingoIEXWebSocketData();
		ProcessOneTiingoIEXWebSocketData(pData);
	}
	return true;
}

bool CWorldMarket::ProcessTiingoCryptoWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoCryptoWebSocketDataSize();

	for (auto i = 0; i < total; i++) {
		auto pData = gl_WebInquirer.PopTiingoCryptoWebSocketData();
		ProcessOneTiingoCryptoWebSocketData(pData);
	}
	return true;
}

bool CWorldMarket::ProcessTiingoForexWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoForexWebSocketDataSize();

	for (auto i = 0; i < total; i++) {
		auto pData = gl_WebInquirer.PopTiingoForexWebSocketData();
		ProcessOneTiingoForexWebSocketData(pData);
	}
	return true;
}