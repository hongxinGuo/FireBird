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

Semaphore gl_UpdateWorldMarketDB(1);  // 此信号量用于生成美国股票日线历史数据库

CWorldMarket::CWorldMarket() {
	static int siInstance = 0;

	if (++siInstance > 1) {
		TRACE("CWorldMarket市场变量只允许存在一个实例\n");
	}

	// 无需每日更新的变量放在这里
	m_fFinnhubEPSSurpriseUpdated = true;
	m_lCurrentUpdateEPSSurprisePos = 0;

	m_lCurrentUpdateDayLinePos = 0; // 由于证券代码总数有二十万之多，无法在一天之内更新完，故不再重置此索引。

	m_strMarketId = _T("美国市场");
	m_lMarketTimeZone = 4 * 3600; // 美国股市使用美东标准时间。
	CalculateTime();

	InitialFinnhubInquiryStr();
	InitialTiingoInquiryStr();

	Reset();
}

void CWorldMarket::InitialFinnhubInquiryStr(void) {
	m_vFinnhubInquiringStr.resize(1000);

	// Finnhub前缀字符串在此预设之
	m_vFinnhubInquiringStr.at(__COMPANY_PROFILE__) = _T("https://finnhub.io/api/v1/stock/profile?symbol="); // 公司简介。
	m_vFinnhubInquiringStr.at(__COMPANY_PROFILE_CONCISE__) = _T("https://finnhub.io/api/v1/stock/profile2?symbol="); // 公司简介（简版）
	m_vFinnhubInquiringStr.at(__COMPANY_SYMBOLS__) = _T("https://finnhub.io/api/v1/stock/symbol?exchange="); // 可用代码集
	m_vFinnhubInquiringStr.at(__MARKET_NEWS__) = _T("https://finnhub.io/api/v1/news?category=general");
	m_vFinnhubInquiringStr.at(__COMPANY_NEWS__) = _T("https://finnhub.io/api/v1/company-news?symbol=");
	m_vFinnhubInquiringStr.at(__NEWS_SENTIMENT__) = _T("https://finnhub.io/api/v1/news-sentiment?symbol=");
	m_vFinnhubInquiringStr.at(__PEERS__) = _T("https://finnhub.io/api/v1/stock/peers?symbol=");
	m_vFinnhubInquiringStr.at(__BASIC_FINANCIALS__) = _T("https://finnhub.io/api/v1/stock/metric?symbol=");
	m_vFinnhubInquiringStr.at(__INSIDER_TRANSACTION__) = _T("https://finnhub.io/api/v1/stock/insider-transactions?symbol=");
	m_vFinnhubInquiringStr.at(__SEC_FILINGS__) = _T("https://finnhub.io/api/v1/stock/filings?symbol=");

	m_vFinnhubInquiringStr.at(__STOCK_EPS_SURPRISE__) = _T("https://finnhub.io/api/v1/stock/earnings?symbol=");
	m_vFinnhubInquiringStr.at(__STOCK_EARNING_CALENDAR__) = _T("https://finnhub.io/api/v1/calendar/earnings?");

	m_vFinnhubInquiringStr.at(__STOCK_QUOTE__) = _T("https://finnhub.io/api/v1/quote?symbol="); // 某个代码的交易
	m_vFinnhubInquiringStr.at(__STOCK_CANDLES__) = _T("https://finnhub.io/api/v1/stock/candle?symbol="); // 历史蜡烛图

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
	m_vTiingoInquiringStr.at(__COMPANY_SYMBOLS__) = _T("https://api.tiingo.com/tiingo/fundamentals/meta?"); // 可用代码集
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

	// Finnhub各申请网络数据标识，每日需要重置。
	m_fFinnhubSymbolUpdated = false; // 每日需要更新代码
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
	SetSystemReady(false); // 市场初始状态为未设置好。

	m_lLastTotalForexExchange = 0;
	m_lLastTotalForexSymbol = 0;
	m_lCurrentUpdateForexDayLinePos = 0;

	m_lLastTotalCountry = 0;
	m_lLastTotalEconomicCalendar = 0;

	m_lLastTotalTiingoIndustry = 0;
	m_lLastTotalSICIndustry = 0;
	m_lLastTotalNaicsIndustry = 0;

	if (GetDayOfWeek() == 6) { // 每周的星期六更新一次EPSSurprise
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
	LoadWorldExchangeDB(); // 装入世界交易所信息
	LoadCountryDB();
	LoadStockDB();
	LoadWorldChoicedStock();
	LoadForexExchange();
	LoadForexSymbol();
	LoadEconomicCalendarDB();
	LoadTiingoStock();

	CString str = _T("重置World Market于美东标准时间：");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);
}

bool CWorldMarket::SchedulingTask(void) {
	CVirtualMarket::SchedulingTask();

	static time_t s_timeLast = 0;
	static int s_iCountfinnhubLimit = 12; // Finnhub.io每1.5秒左右申请一次，以防止出现频率过高的情况。
	static int s_iCountTiingoLimit = 80; // 保证每80次执行一次（即8秒每次）.Tiingo免费账户速度限制为每小时500次， 每分钟9次，故每次8秒即可。
	const long lCurrentTime = GetFormatedMarketTime();

	TaskCheckSystemReady();

	if (--s_iCountfinnhubLimit < 0) {
		TaskInquiryFinnhub(lCurrentTime);
		if (IsFinnhubInquiring()) {
			s_iCountfinnhubLimit = 12; // 如果申请了网络数据，则重置计数器，以便申请下一次。
		}
	}

	ProcessFinnhubWebDataReceived(); // 要先处理收到的Finnhub网络数据
	ProcessFinnhubInquiringMessage(); // 然后再申请处理下一个

	if (--s_iCountTiingoLimit < 0) {
		s_iCountTiingoLimit = 80;
		TaskInquiryTiingo();
	}

	ProcessTiingoWebDataReceived(); // 要先处理收到的Tiingo网络数据
	ProcessTiingoInquiringMessage(); // 然后再申请处理下一个

	//根据时间，调度各项定时任务.每秒调度一次
	if (GetMarketTime() > s_timeLast) {
		SchedulingTaskPerSecond(GetMarketTime() - s_timeLast, lCurrentTime);
		s_timeLast = GetMarketTime();
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub读取函数采用申请和接收轮换执行方式，故而至少调用两次才完成一个轮回。
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::ProcessFinnhubInquiringMessage(void) {
	CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
	CString strTemp;
	CWorldStockPtr pStock = nullptr;
	CForexSymbolPtr pSymbol = nullptr;
	bool fDone = false;

	if (m_qFinnhubWebInquiry.size() > 0) { // 有申请等待？
		ASSERT(IsFinnhubInquiring());
		if (IsFinnhubDataReceived()) { //已经发出了数据申请且Finnhub数据已经接收到了？
			m_CurrentFinnhubInquiry = GetFinnhubInquiry();
			gl_pFinnhubWebInquiry->SetInquiryingStringPrefix(m_vFinnhubInquiringStr.at(m_CurrentFinnhubInquiry.m_lInquiryIndex)); // 设置前缀

			switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) { // 设置股票指针
			case __COMPANY_PROFILE__: // Premium 免费账户无法读取此信息，sandbox模式能读取，但数据是错误的，只能用于测试。
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
			switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) { // 根据不同的要求设置中缀字符串
			case __COMPANY_PROFILE__: // Premium 免费账户无法读取此信息，sandbox模式能读取，但数据是错误的，只能用于测试。
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
				TRACE("未处理指令%d\n", m_CurrentFinnhubInquiry.m_lInquiryIndex);
				break;
			}
			SetFinnhubDataReceived(false); // 重置此标识需要放在启动工作线程（GetWebData）之前，否则工作线程中的断言容易出错。
			gl_pFinnhubWebInquiry->GetWebData();
			fDone = true;
		}
	}

	return fDone;
}

bool CompareWorldStock(CWorldStockPtr p1, CWorldStockPtr p2) { return (p1->GetSymbol().Compare(p2->GetSymbol()) < 0); }

//////////////////////////////////////////////
//
// 处理工作线程接收到的Finnhub网络信息。
//
// 目前只允许同时处理一条信息。即信息从申请至处理完之前，不允许处理下一条信息。这样能够保证同一性。且由于Finnhub网站有速度限制，
// 每分钟只允许60次申请，故而没有必要强调处理速度。
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
	if (IsFinnhubDataReceived()) { // 如果网络数据接收完成
		if (gl_WebInquirer.GetFinnhubDataSize() > 0) {  // 处理当前网络数据
			ASSERT(IsFinnhubInquiring());
			pWebData = gl_WebInquirer.PopFinnhubData();

			switch (m_CurrentFinnhubInquiry.m_lInquiryIndex) { // 根据不同的要求设置中缀字符串
			case __COMPANY_PROFILE__: // Premium 免费账户无法读取此信息，sandbox模式能读取，但数据是错误的，只能用于测试。
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
			case __COMPANY_PROFILE__: // 目前免费账户无法使用此功能。
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
					//TRACE("今日%s Finnhub Symbol总数为%d\n", m_vFinnhubExchange.at(m_lCurrentExchangePos)->m_strCode, vStock.size());
					sprintf_s(buffer, _T("%6d"), (int)vStock.size());
					strNumber = buffer;
					str = _T("今日") + m_vFinnhubExchange.at(m_lCurrentExchangePos)->m_strCode + _T(" Finnhub Symbol总数为") + strNumber;
					gl_systemMessage.PushInnerSystemInformationMessage(str);
					// 加上交易所代码。
					for (auto& pStock3 : vStock) {
						pStock3->SetExchangeCode(m_vFinnhubExchange.at(m_lCurrentExchangePos)->m_strCode);
					}
					for (auto& pStock2 : vStock) {
						if (!IsStock(pStock2->GetSymbol())) {
							AddStock(pStock2);
							str = _T("Finnhub发现新代码:") + pStock2->GetSymbol();
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
						pStock->SetLastEPSSurpriseUpdateDate(19700101); // 将日期设置为更早。
						pStock->SetUpdateProfileDB(true);
					}
					pStock->m_fEPSSurpriseUpdated = true;
					pStock->m_fEPSSurpriseNeedSave = true;
				}
				break;
			case __STOCK_QUOTE__:
				ASSERT(pStock != nullptr);
				ProcessFinnhubStockQuote(pWebData, pStock);
				if ((pStock->GetTransactionTime() + 3600 * 12 - GetMarketTime()) > 0) { // 交易时间不早于12小时，则设置此股票为活跃股票
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
					if (pStock->GetDayLineSize() == 0) { // 没有日线数据？
						if (pStock->IsNotChecked()) { // 尚未确定代码有效性？
							pStock->SetIPOStatus(__STOCK_NULL__);
						}
					}
					else if (IsEarlyThen(pStock->GetDayLine(pStock->GetDayLineSize() - 1)->GetFormatedMarketDate(), GetFormatedMarketDate(), 100)) {
						pStock->SetIPOStatus(__STOCK_DELISTED__);
					}
					else {
						pStock->SetIPOStatus(__STOCK_IPOED__);
					}
					TRACE("处理%s日线数据\n", pStock->GetSymbol().GetBuffer());
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
					TRACE("处理%s日线数据\n", m_vForexSymbol.at(m_CurrentFinnhubInquiry.m_lStockIndex)->GetSymbol().GetBuffer());
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
			gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(_T("")); // 有些网络申请没有用到中间字符段，如果不清除之前的中间字符段（如果有的话），会造成申请字符串的错误。
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

	if (m_qTiingoWebInquiry.size() > 0) { // 有申请等待？
		ASSERT(IsTiingoInquiring());
		if (IsTiingoDataReceived()) { //已经发出了数据申请且Tiingo数据已经接收到了？
			m_CurrentTiingoInquiry = GetTiingoInquiry();
			gl_pTiingoWebInquiry->SetInquiryingStringPrefix(m_vTiingoInquiringStr.at(m_CurrentTiingoInquiry.m_lInquiryIndex)); // 设置前缀
			switch (m_CurrentTiingoInquiry.m_lInquiryIndex) { // 根据不同的要求设置中缀字符串
			case __COMPANY_PROFILE__: // Premium 免费账户无法读取此信息，sandbox模式能读取，但是错误的，只能用于测试。
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
				TRACE("Tiingo未处理指令%d\n", m_CurrentTiingoInquiry.m_lInquiryIndex);
				break;
			}
			SetTiingoDataReceived(false); // 重置此标识需要放在启动工作线程（GetWebData）之前，否则工作线程中的断言容易出错。
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
	if (IsTiingoDataReceived()) { // 如果网络数据接收完成
		if (gl_WebInquirer.GetTiingoDataSize() > 0) {  // 处理当前网络数据
			ASSERT(IsTiingoInquiring());
			pWebData = gl_WebInquirer.PopTiingoData();
			switch (m_CurrentTiingoInquiry.m_lInquiryIndex) {
			case __COMPANY_PROFILE__: // 目前免费账户无法使用此功能。
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
						if (IsStock(pTiingoStock->m_strTicker)) { // Tiingo的Symbol信息只是用于Finnhub的一个补充。
							lTemp++;
							pStock = m_vWorldStock.at(m_mapWorldStock.at(pTiingoStock->m_strTicker));
							pStock->UpdateStockProfile(pTiingoStock);
							pStock->SetUpdateProfileDB(true);
						}
						else { // new stock，
							// do nothing now.
							int iiii = 0;
							iiii++; // for debug
						}
					}
					TRACE("今日Tiingo活跃股票数为：%d\n", lTemp);
					sprintf_s(buffer, _T("%6d"), lTemp);
					strNumber = buffer;
					str = _T("今日Tiingo Symbol活跃股票总数为") + strNumber;
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
				TRACE("处理Tiingo %s日线数据\n", pStock->GetSymbol().GetBuffer());
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
			gl_pTiingoWebInquiry->SetInquiryingStringMiddle(_T("")); // 有些网络申请没有用到中间字符段，如果不清除之前的中间字符段（如果有的话），会造成申请字符串的错误。
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
/// 此任务必须放置于每分钟执行一次的调度中。
/// <returns></returns>
///
bool CWorldMarket::TaskResetMarket(long lCurrentTime) {
	// 市场时间十七时重启系统
	if (IsPermitResetMarket()) { // 如果允许重置系统
		if ((lCurrentTime > 170000) && (lCurrentTime <= 170100)) { // 本市场时间的下午五时(北京时间上午五时重启本市场。这样有利于接收日线数据。
			SetResetMarket(true);// 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
			SetPermitResetMarket(false); // 今天不再允许重启系统。
			SetSystemReady(false);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// 此函数由SchedulingTask调度，每1500毫秒左右执行一次。
//
//////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskInquiryFinnhub(long lCurrentTime) {
	if (((lCurrentTime < 165700) || (lCurrentTime > 170200))) { // 下午五时重启系统，故而此时不允许接收网络信息。
		TaskInquiryFinnhubCountryList();
		TaskInquiryFinnhubForexExchange();
		TaskInquiryFinnhubCompanySymbol(); // 第一个动作，首先申请当日证券代码
		TaskInquiryFinnhubForexSymbol();
		//TaskInquiryFinnhubEconomicCalendar();

		// 申请Finnhub网络信息的任务，皆要放置在这里，以保证在市场时间凌晨十分钟后执行。这样能够保证在重启市场时没有执行查询任务
		if (IsSystemReady()) {
			TaskInquiryFinnhubCompanyProfileConcise();
			TaskInquiryFinnhubPeer();
			TaskInquiryFinnhubInsiderTransaction();
			//TaskInquiryFinnhubEPSSurprise(); // 这个现在没什么用，暂时停止更新。
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
			//TRACE("申请%s交易所证券代码\n", pExchange->m_strCode.GetBuffer());
		}
		else {
			fHaveInquiry = false;
			SetFinnhubSymbolUpdated(true);
			TRACE("Finnhub交易所代码数据查询完毕\n");
			str = _T("交易所代码数据查询完毕");
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
			//TRACE("更新%s简介\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetFinnhubInquiring(true);
			fHaveInquiry = true;
		}
		else {
			SetFinnhubStockProfileUpdated(true);
			TRACE("Finnhub股票简介更新完毕\n");
			str = _T("Finnhub股票简介更新完毕");
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
			if (pStock->IsUSMarket() && pStock->IsDayLineNeedUpdate()) { // 目前免费账户只能下载美国市场的股票日线。
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
			TRACE("申请%s日线数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			fHaveInquiry = false;
			SetFinnhubDayLineUpdated(true);
			m_lCurrentUpdateDayLinePos = 0; // 重置此索引。所有的日线数据更新一次所需时间要超过24小时，故保持更新即可。
			TRACE("Finnhub日线更新完毕，从新开始更新\n");
			str = _T("US Market日线历史数据更新完毕");
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
				if (pStock->IsInsiderTransactionNeedUpdate()) { // 目前免费账户只能下载美国市场的股票日线。
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
			TRACE("申请%s 内部交易数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			fHaveInquiry = false;
			SetFinnhubInsiderTransactionUpdated(true);
			TRACE("FinnhubInsider Transaction更新完毕\n");
			str = _T("US Market Insider Transaction数据更新完毕");
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
		TRACE("申请%s实时数据\n", m_vWorldStock.at(m_lCurrentRTDataQuotePos)->GetSymbol().GetBuffer());
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
			TRACE("申请%s Peer数据\n", m_vWorldStock.at(m_lCurrentUpdatePeerPos)->GetSymbol().GetBuffer());
		}
		else {
			fHaveInquiry = false;
			SetFinnhubPeerUpdated(true);
			TRACE("Finnhub Peers更新完毕\n");
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
			TRACE("申请%s EPS Surprise数据\n", m_vWorldStock.at(m_lCurrentUpdateEPSSurprisePos)->GetSymbol().GetBuffer());
		}
		else {
			SetFinnhubEPSSurpriseUpdated(true);
			TRACE("Finnhub EPS Surprise更新完毕\n");
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
			TRACE("申请Forex%s日线数据\n", pForexSymbol->GetSymbol().GetBuffer());
		}
		else {
			SetFinnhubForexDayLineUpdated(true);
			TRACE("Finnhub Forex日线更新完毕\n");
			str = _T("Forex DayLine Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryTiingo(void) {
	if (IsSystemReady()) {
		TaskInquiryTiingoCompanySymbol();
		// 由于Tiingo规定每月只能查询500个代码，故测试成功后即暂时不使用。
		TaskInquiryTiingoDayLine(); // 初步测试完毕。
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
// Tiingo的下载日线数据与Finnhub的日线下载函数，只允许同时运行其中之一。
// 目前此功能只用于下载自选股票的日线数据。
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
			TRACE("申请Tiingo %s日线数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			SetTiingoDayLineUpdated(true);
			TRACE("Tiingo日线更新完毕\n");
			str = _T("美国市场自选股票日线历史数据更新完毕");
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
//	将Forex日线数据存入数据库．
//  此函数由工作线程ThreadForexDayLineSaveProc调用，尽量不要使用全局变量。(目前使用主线程调用之，以消除同步问题的出现）
//
// 无论是否执行了存储函数，都需要将下载的日线历史数据删除，这样能够节省内存的占用。由于实际存储功能使用线程模式实现，
// 故而其执行时间可能晚于主线程，导致主线程删除日线数据时出现同步问题。解决的方法是让工作线程独立删除存储后的日线数据，
// 主线程的删除函数只在不调用工作线程（无需存储日线数据）的情况下方才执行。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskUpdateForexDayLineDB(void) {
	CString str;
	bool fUpdated = false;

	for (auto& pSymbol : m_vForexSymbol) {
		if (gl_fExitingSystem) {
			break; // 如果程序正在退出，则停止存储。
		}
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					CreatingThreadUpdateForexDayLineDB(pSymbol.get());
					fUpdated = true;
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
		if (pStock->IsEPSSurpriseNeedSaveAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			CreatingThreadUpdateEPSSurpriseDB(pStock.get());
			TRACE("更新%s EPS surprise数据\n", pStock->GetSymbol().GetBuffer());
		}
		if (gl_fExitingSystem) {
			break; // 如果程序正在退出，则停止存储。
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
			str = _T("世界市场初始化完毕");
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
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateDayLineDB() {
	thread thread1(ThreadUpdateWorldStockDayLineDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateStockProfileDB(void) {
	thread thread1(ThreadUpdateStockProfileDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateForexDayLineDB(CFinnhubForexSymbol* pSymbol) {
	thread thread1(ThreadUpdateForexDayLineDB, pSymbol);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateForexSymbolDB() {
	thread thread1(ThreadUpdateForexSymbolDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateCountryListDB(void) {
	thread thread1(ThreadUpdateCountryListDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateEPSSurpriseDB(CWorldStock* pStock) {
	thread thread1(ThreadUpdateEPSSurpriseDB, pStock);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateInsiderTransactionDB(void) {
	thread thread1(ThreadUpdateInsiderTransactionDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateTiingoStockDB(void) {
	thread thread1(ThreadUpdateTiingoStockDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateTiingoIndustry(void) {
	thread thread1(ThreadUpdateTiingoIndustry, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateSICIndustry(void) {
	thread thread1(ThreadUpdateSICIndustry, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateNaicsIndustry(void) {
	thread thread1(ThreadUpdateNaicsIndustry, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateForexExchangeDB(void) {
	thread thread1(ThreadUpdateForexExchangeDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
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
		if (m_mapEconomicCalendar.find(strSymbol) == m_mapEconomicCalendar.end()) { // 新事件？
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
			str = _T("发现重复代码：");
			str += pWorldStock->GetSymbol();
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			setWorldStock.Delete(); // 删除此重复代码
		}
		setWorldStock.MoveNext();
	}
	setWorldStock.m_pDatabase->CommitTrans();
	setWorldStock.Close();
	SortStockVector();
	m_lLastTotalWorldStock = m_vWorldStock.size();
	TRACE("共装入%d Finnhub Symbol\n", m_lLastTotalWorldStock);

	char buffer[100];
	sprintf_s(buffer, _T("%d"), lSymbolLength);
	str = _T("最长长度为");
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
			setWorldChoicedStock.Delete(); // 清除非法股票代码
		}
		setWorldChoicedStock.MoveNext();
	}
	setWorldChoicedStock.m_pDatabase->CommitTrans();
	setWorldChoicedStock.Close();

	return true;
}

///////////////////////////////////////////////////////////////////////////
//
// 此种更新方法，是默认新的国家代码附加在最后。
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
/// 这种查询方式比较晦涩，但结果正确。目前使用此函数。(可能出现存储多个相同代码的问题，研究之）
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
		TRACE("此任务五分钟之内没有完成\n");
		return false;
	}
	else {
		sm_fInProcess = true;
	}

	//更新原有的代码集状态
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
					//ASSERT(pStock3->IsTodayNewStock()); // 所有的新股票，都是今天新生成的
					iCount++;
					pStock3->Append(setWorldStock);
					pStock3->SetTodayNewStock(false);
					TRACE("存储股票：%s\n", pStock3->GetSymbol().GetBuffer());
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
			break; // 如果程序正在退出，则停止存储。
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
				str = pStock->GetSymbol() + _T("内部交易资料更新完成");
				gl_systemMessage.PushDayLineInfoMessage(str);
				TRACE("更新%s内部交易数据\n", pStock->GetSymbol().GetBuffer());
			}
		}
		if (gl_fExitingSystem) {
			break; // 如果程序正在退出，则停止存储。
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
	// 尚未实现
	return false;
}

bool CWorldMarket::UpdateSICIndustry(void) {
	// 尚未实现
	return false;
}

bool CWorldMarket::UpdateNaicsIndustry(void) {
	// 尚未实现
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
/// finnhub数据源的格式：wss://ws.finnhub.io/?token=c1i57rv48v6vit20lrc0。
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
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Finnhub WebSocket已连接"));
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
/// Tiingo IEX的数据源格式：wss://api.tiingo.com/iex，其密钥是随后发送的。
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
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX WebSocket已连接"));
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
/// Tiingo Crypto的数据源格式：wss://api.tiingo.com/crypto，其密钥是随后发送的。
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
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto WebSocket已连接"));
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
/// Tiingo Forex的数据源格式：wss://api.tiingo.com/fx，其密钥是随后发送的。
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
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Forex WebSocket已连接"));
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
	//CString strSuffix = _T("\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":[\"aapl\",\"rig\"]}}"); // 5：最简略数据格式。1：最详细数据格式。
	CString strSuffix = _T("\",\"eventData\":{\"thresholdLevel\":5}}"); // 5：最简略数据格式。1：最详细数据格式。
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
	CString strSuffix = _T("\",\"eventData\":{\"thresholdLevel\":5}}"); // 5：最简略数据格式。1：最详细数据格式。
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
	CString strSuffix = _T("\",\"eventData\":{\"thresholdLevel\":5}}"); // 5：最简略数据格式。1：最详细数据格式。
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