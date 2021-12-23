#include"pch.h"

#include "WorldMarket.h"
#include"thread.h"
#include"Callablefunction.h"

#include"ProductFinnhubStockPriceQuote.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"
#include"ProductFinnhubForexSymbol.h"
#include"ProductFinnhubCryptoSymbol.h"

#include"WebInquirer.h"
#include"EPSSurprise.h"

#include"SetWorldMarketOption.h"
#include"SetFinnhubForexExchange.h"
#include"SetFinnhubCryptoExchange.h"
#include"SetWorldStock.h"
#include"SetWorldChoicedStock.h"
#include"SetWorldChoicedForex.h"
#include"SetWorldChoicedCrypto.h"
#include"SetWorldStockDayLine.h"
#include"SetCountry.h"
#include"SetEconomicCalendar.h"
#include"SetEPSSurprise.h"
#include"SetTiingoStock.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include<ixwebsocket/IXWebSocketMessage.h>

using namespace std;
#include<algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Semaphore gl_UpdateWorldMarketDB(1);  // 此信号量用于生成美国股票日线历史数据库

IMPLEMENT_DYNCREATE(CWorldMarket, CVirtualMarket)

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

	m_FinnhubWebSocket.SetSubscriptionStatus(false); // finnhub WebSocket没有注册ID

	Reset();
}

CWorldMarket::~CWorldMarket() {
	PreparingExitMarket();

	m_vCountry.resize(0);
	m_vWorldStock.resize(0);
	m_vWorldChoicedStock.resize(0);
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
	m_lCurrentCryptoExchangePos = 0;
	m_lCurrentCryptoSymbolPos = 0;
	m_lChoicedStockPos = 0;

	m_pCurrentFinnhubProduct = nullptr;

	m_dataFinnhubStockExchange.Reset();
	m_dataFinnhubForexExchange.Reset();

	// Finnhub各申请网络数据标识，每日需要重置。
	m_fFinnhubSymbolUpdated = false; // 每日需要更新代码
	m_fFinnhubStockProfileUpdated = false;
	m_fFinnhubDayLineUpdated = false;
	m_vWorldStock.resize(0);
	m_mapWorldStock.clear();
	m_vWorldChoicedStock.resize(0);
	m_mapWorldChoicedStock.clear();
	m_fFinnhubForexExchangeUpdated = false;
	m_fFinnhubForexSymbolUpdated = false;
	m_fFinnhubForexDayLineUpdated = false;
	m_fFinnhubCryptoExchangeUpdated = false;
	m_fFinnhubCryptoSymbolUpdated = false;
	m_vCryptoSymbol.resize(0);
	m_mapCryptoSymbol.clear();
	m_fFinnhubCryptoDayLineUpdated = false;
	m_vCountry.resize(0);
	m_mapCountry.clear();
	m_fCountryListUpdated = false;
	m_fFinnhubEconomicCalendarUpdated = false;

	m_fFinnhubPeerUpdated = false;
	m_fFinnhubInsiderTransactionUpdated = false;
	m_lCurrentUpdatePeerPos = 0;
	m_lCurrentUpdateInsiderTransactionPos = 0;

	m_fFinnhubInquiring = false;
	m_fFinnhubDataReceived = true;

	m_fRebulidDayLine = false;
	SetSystemReady(false); // 市场初始状态为未设置好。

	m_lCurrentUpdateForexDayLinePos = 0;
	m_lLastTotalCryptoSymbol = 0;
	m_lCurrentUpdateCryptoDayLinePos = 0;

	m_lLastTotalCountry = 0;

	m_lLastTotalTiingoIndustry = 0;
	m_lLastTotalSICIndustry = 0;
	m_lLastTotalNaicsIndustry = 0;

	m_iProcessedFinnhubWebSocket = 0;
	m_iProcessedTiingoIEXWebSocket = 0;
	m_iProcessedTiingoCryptoWebSocket = 0;
	m_iProcessedTiingoForexWebSocket = 0;

	if (GetDayOfWeek() == 6) { // 每周的星期六更新一次EPSSurprise
		m_lCurrentUpdateEPSSurprisePos = 0;
		SetFinnhubEPSSurpriseUpdated(false);
	}
}

void CWorldMarket::ResetQuandl(void) {
	m_pCurrentQuandlProduct = nullptr;
}

void CWorldMarket::ResetTiingo(void) {
	m_pCurrentTiingoProduct = nullptr;

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
	LoadWorldChoicedForex();
	LoadCryptoExchange();
	LoadCryptoSymbol();
	LoadWorldChoicedCrypto();
	LoadEconomicCalendarDB();
	LoadTiingoStock();

	CString str = _T("重置World Market于美东标准时间：");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);
}

bool CWorldMarket::PreparingExitMarket(void)
{
	StopReceivingWebSocket();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// 默认每100毫秒执行一次。
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::SchedulingTask(void) {
	CVirtualMarket::SchedulingTask();

	static time_t s_timeLast = 0;
	static int s_iCountfinnhubLimit = 12; // Finnhub.io每1.5秒左右申请一次，以防止出现频率过高的情况。
	static int s_iCountTiingoLimit = 80; // 保证每80次执行一次（即8秒每次）.Tiingo免费账户速度限制为每小时500次， 每分钟9次，故每次8秒即可。
	const long lCurrentTime = GetMarketTime();

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
	if (GetUTCTime() > s_timeLast) {
		SchedulingTaskPerSecond(GetUTCTime() - s_timeLast, lCurrentTime);
		s_timeLast = GetUTCTime();
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
	CForexSymbolPtr pForexSymbol = nullptr;
	CCryptoSymbolPtr pCryptoSymbol = nullptr;
	bool fDone = false;

	if (m_qFinnhubProduct.size() > 0) { // 有申请等待？
		ASSERT(IsFinnhubInquiring());
		if (IsFinnhubDataReceived()) { //已经发出了数据申请且Finnhub数据已经接收到了？
			m_pCurrentFinnhubProduct = m_qFinnhubProduct.front();
			m_qFinnhubProduct.pop();

			gl_pFinnhubWebInquiry->SetInquiryingStringPrefix(m_pCurrentFinnhubProduct->CreatMessage()); // 设置前缀

			SetFinnhubDataReceived(false); // 重置此标识需要放在启动工作线程（GetWebData）之前，否则工作线程中的断言容易出错。
			gl_pFinnhubWebInquiry->GetWebData();
			fDone = true;
		}
	}

	return fDone;
}

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
	bool fDone = false;

	if (m_pCurrentFinnhubProduct == nullptr) return false;

	if (IsFinnhubDataReceived()) { // 如果网络数据接收完成
		if (gl_WebInquirer.GetFinnhubDataSize() > 0) {  // 处理当前网络数据
			ASSERT(IsFinnhubInquiring());
			pWebData = gl_WebInquirer.PopFinnhubData();

			m_pCurrentFinnhubProduct->ProcessWebData(pWebData);

			gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(_T("")); // 有些网络申请没有用到中间字符段，如果不清除之前的中间字符段（如果有的话），会造成申请字符串的错误。
			SetFinnhubInquiring(false);
			fDone = true;
			m_pCurrentFinnhubProduct = nullptr;
		}
	}

	return fDone;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理Tiingo各申请数据，使用线程读取Tiingo网络数据
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWorldMarket::ProcessTiingoInquiringMessage(void) {
	bool fDone = false;
	CString strMessage;

	if (m_qTiingoProduct.size() > 0) { // 有申请等待？
		ASSERT(IsTiingoInquiring());
		if (IsTiingoDataReceived()) { //已经发出了数据申请且Tiingo数据已经接收到了？
			m_pCurrentTiingoProduct = m_qTiingoProduct.front();
			m_qTiingoProduct.pop();

			strMessage = m_pCurrentTiingoProduct->CreatMessage();
			gl_pTiingoWebInquiry->SetInquiryingStringPrefix(strMessage); // 采用此种形式时，只设置StringPrefix即可，其他两个字符串皆为空。

			SetTiingoDataReceived(false); // 重置此标识需要放在启动工作线程（GetWebData）之前，否则工作线程中的断言容易出错。
			gl_pTiingoWebInquiry->GetWebData(); // 调用读取网络数据的线程
			fDone = true;
		}
	}

	return fDone;
}

bool CWorldMarket::ProcessTiingoWebDataReceived(void) {
	CWebDataPtr pWebData = nullptr;
	bool fDone = false;

	if (m_pCurrentTiingoProduct == nullptr) return false;

	if (IsTiingoDataReceived()) { // 如果网络数据接收完成
		if (gl_WebInquirer.GetTiingoDataSize() > 0) {  // 处理当前网络数据
			ASSERT(IsTiingoInquiring());
			pWebData = gl_WebInquirer.PopTiingoData();

			m_pCurrentTiingoProduct->ProcessWebData(pWebData);

			SetTiingoInquiring(false);
			fDone = true;
			m_pCurrentTiingoProduct = nullptr;
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
	TaskUpdateWorldStockFromWebSocket();

	static bool sm_fConnectedFinnhubWebSocket = false;
	static bool sm_fSendFinnhubWebStocketMessage = false;

	static bool sm_fConnectedTiingoIEXWebSocket = false;
	static bool sm_fSendTiingoIEXWebStocketMessage = false;
	static bool sm_fConnectedTiingoCryptoWebSocket = false;
	static bool sm_fSendTiingoCryptoWebStocketMessage = false;
	static bool sm_fConnectedTiingoForexWebSocket = false;
	static bool sm_fSendTiingoForexWebStocketMessage = false;

	if (IsSystemReady()) {
		if (!sm_fConnectedFinnhubWebSocket) {
			if (m_FinnhubWebSocket.IsClosed()) {
				sm_fConnectedFinnhubWebSocket = true;
				ConnectFinnhubWebSocket();
			}
		}

		if (!sm_fSendFinnhubWebStocketMessage) {
			if (m_FinnhubWebSocket.IsOpen()) {
				sm_fSendFinnhubWebStocketMessage = true;
				SendFinnhubWebSocketMessage();
			}
		}

		if (!sm_fConnectedTiingoIEXWebSocket) {
			if (m_TiingoIEXWebSocket.IsClosed()) {
				sm_fConnectedTiingoIEXWebSocket = true;
				ConnectTiingoIEXWebSocket();
			}
		}

		if (!sm_fSendTiingoIEXWebStocketMessage) {
			if (m_TiingoIEXWebSocket.IsOpen()) {
				sm_fSendTiingoIEXWebStocketMessage = true;
				SendTiingoIEXWebSocketMessage();
			}
		}

		if (!sm_fConnectedTiingoCryptoWebSocket) {
			if (m_TiingoCryptoWebSocket.IsClosed()) {
				sm_fConnectedTiingoCryptoWebSocket = true;
				ConnectTiingoCryptoWebSocket();
			}
		}

		if (!sm_fSendTiingoCryptoWebStocketMessage) {
			if (m_TiingoCryptoWebSocket.IsOpen()) {
				sm_fSendTiingoCryptoWebStocketMessage = true;
				SendTiingoCryptoWebSocketMessage();
			}
		}

		if (!sm_fConnectedTiingoForexWebSocket) {
			if (m_TiingoForexWebSocket.IsClosed()) {
				sm_fConnectedTiingoForexWebSocket = true;
				ConnectTiingoForexWebSocket();
			}
		}

		if (!sm_fSendTiingoForexWebStocketMessage) {
			if (m_TiingoForexWebSocket.IsOpen()) {
				sm_fSendTiingoForexWebStocketMessage = true;
				SendTiingoForexWebSocketMessage();
			}
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
	TaskUpdateCryptoExchangeDB();
	TaskUpdateCryptoSymbolDB();
	TaskUpdateInsiderTransactionDB();
	TaskUpdateForexDayLineDB();
	TaskUpdateCryptoDayLineDB();
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
		TaskInquiryFinnhubCryptoExchange();
		TaskInquiryFinnhubCompanySymbol(); // 第一个动作，首先申请当日证券代码
		TaskInquiryFinnhubForexSymbol();
		TaskInquiryFinnhubCryptoSymbol();
		//TaskInquiryFinnhubEconomicCalendar();

		// 申请Finnhub网络信息的任务，皆要放置在这里，以保证在市场时间凌晨十分钟后执行。这样能够保证在重启市场时没有执行查询任务
		if (IsSystemReady()) {
			TaskInquiryFinnhubCompanyProfileConcise();
			TaskInquiryFinnhubPeer();
			TaskInquiryFinnhubInsiderTransaction();
			//TaskInquiryFinnhubEPSSurprise(); // 这个现在没什么用，暂时停止更新。
			TaskInquiryFinnhubForexDayLine();
			TaskInquiryFinnhubCryptoDayLine();
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
	if (!IsCountryListUpdated() && !IsFinnhubInquiring()) {
		CWebSourceDataProductPtr p = m_FinnhubFactory.CreateProduct(this, __ECONOMIC_COUNTRY_LIST__);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("查询Finnhub economic country List"));
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCompanySymbol(void) {
	bool fFound = false;
	CFinnhubStockExchangePtr pExchange;
	CString str = _T("");
	long lExchangeSize = m_dataFinnhubStockExchange.GetExchangeSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentStockExchangePos = 0;

	if (!IsFinnhubSymbolUpdated() && !IsFinnhubInquiring()) {
		for (lCurrentStockExchangePos = 0; lCurrentStockExchangePos < lExchangeSize; lCurrentStockExchangePos++) {
			if (!m_dataFinnhubStockExchange.GetExchange(lCurrentStockExchangePos)->IsUpdated()) {
				pExchange = m_dataFinnhubStockExchange.GetExchange(lCurrentStockExchangePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __STOCK_SYMBOLS__);
			p->SetIndex(lCurrentStockExchangePos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			pExchange->SetUpdated(true);
			//TRACE("申请%s交易所证券代码\n", pExchange->m_strCode.GetBuffer());
		}
		else {
			fHaveInquiry = false;
			SetFinnhubSymbolUpdated(true);
			TRACE("Finnhub交易所代码数据查询完毕\n");
			str = _T("Finnhub交易所代码数据查询完毕");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskUpdateForexSymbolDB(void) {
	CreatingThreadUpdateForexSymbolDB();
	return true;
}

bool CWorldMarket::TaskUpdateCryptoSymbolDB(void) {
	CreatingThreadUpdateCryptoSymbolDB();
	return true;
}

bool CWorldMarket::TaskInquiryFinnhubCompanyProfileConcise(void) {
	bool fFound = false;
	long lStockSetSize = m_vWorldStock.size();
	CString str = _T("");
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

	ASSERT(IsSystemReady());
	if (!IsFinnhubStockProfileUpdated() && !IsFinnhubInquiring()) {
		for (m_lCurrentProfilePos = 0; m_lCurrentProfilePos < lStockSetSize; m_lCurrentProfilePos++) {
			if (!m_vWorldStock.at(m_lCurrentProfilePos)->IsProfileUpdated()) {
				fFound = true;
				break;
			}
		}
		if (fFound) {
			p = m_FinnhubFactory.CreateProduct(this, __COMPANY_PROFILE_CONCISE__);
			p->SetIndex(m_lCurrentProfilePos);
			m_qFinnhubProduct.push(p);
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
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = m_vWorldStock.size();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

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
			p = m_FinnhubFactory.CreateProduct(this, __STOCK_PRICE_CANDLES__);
			p->SetIndex(m_lCurrentUpdateDayLinePos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			pStock->SetDayLineNeedUpdate(false);
			//TRACE("申请%s日线数据\n", pStock->GetSymbol().GetBuffer());
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
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = m_vWorldStock.size();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

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
			p = m_FinnhubFactory.CreateProduct(this, __INSIDER_TRANSACTION__);
			p->SetIndex(m_lCurrentUpdateInsiderTransactionPos);
			m_qFinnhubProduct.push(p);
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
	CWebSourceDataProductPtr p = nullptr;
	ASSERT(IsSystemReady());
	if (!IsFinnhubInquiring()) {
		m_lCurrentRTDataQuotePos++;
		if (m_lCurrentRTDataQuotePos == m_vWorldStock.size()) m_lCurrentRTDataQuotePos = 0;
		p = make_shared<CProductFinnhubStockPriceQuote>();
		p->SetIndex(m_lCurrentRTDataQuotePos);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		TRACE("申请%s实时数据\n", m_vWorldStock.at(m_lCurrentRTDataQuotePos)->GetSymbol().GetBuffer());
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubPeer(void) {
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = m_vWorldStock.size();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

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
			p = m_FinnhubFactory.CreateProduct(this, __PEERS__);
			p->SetIndex(m_lCurrentUpdatePeerPos);
			m_qFinnhubProduct.push(p);
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
	CWebSourceDataProductPtr p = nullptr;

	if (!IsFinnhubEconomicCalendarUpdated() && !IsFinnhubInquiring()) {
		p = m_FinnhubFactory.CreateProduct(this, __ECONOMIC_CALENDAR__);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubEPSSurprise(void) {
	bool fFound = false;
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
			CWebSourceDataProductPtr p = make_shared<CProductFinnhubStockEstimatesEPSSurprise>();
			p->SetIndex(m_lCurrentUpdateEPSSurprisePos);
			m_qFinnhubProduct.push(p);
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
	if (!IsFinnhubForexExchangeUpdated() && !IsFinnhubInquiring()) {
		m_qFinnhubProduct.push(m_FinnhubFactory.CreateProduct(this, __FOREX_EXCHANGE__));
		SetFinnhubInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub forex exchange"));
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubForexSymbol(void) {
	CWebSourceDataProductPtr p = nullptr;

	if (!IsFinnhubForexSymbolUpdated() && !IsFinnhubInquiring()) {
		p = m_FinnhubFactory.CreateProduct(this, __FOREX_SYMBOLS__);
		p->SetIndex(m_lCurrentForexExchangePos++);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		if (m_lCurrentForexExchangePos >= m_dataFinnhubForexExchange.GetForexExchangeSize()) {
			SetFinnhubForexSymbolUpdated(true);
			m_lCurrentForexExchangePos = 0;
			gl_systemMessage.PushInformationMessage(_T("Finnhub Forex sysbols已更新"));
		}
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubForexDayLine(void) {
	bool fFound = false;
	CForexSymbolPtr pForexSymbol;
	CString str = _T("");
	const long lStockSetSize = m_dataFinnhubForexSymbol.GetForexSymbolSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

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
			p = m_FinnhubFactory.CreateProduct(this, __FOREX_CANDLES__);
			p->SetIndex(m_lCurrentUpdateForexDayLinePos);
			m_qFinnhubProduct.push(p);
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

bool CWorldMarket::TaskInquiryFinnhubCryptoExchange(void) {
	if (!IsFinnhubCryptoExchangeUpdated() && !IsFinnhubInquiring()) {
		m_qFinnhubProduct.push(m_FinnhubFactory.CreateProduct(this, __CRYPTO_EXCHANGE__));
		SetFinnhubInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub crypto exchange"));
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCryptoSymbol(void) {
	CWebSourceDataProductPtr p = nullptr;

	if (!IsFinnhubCryptoSymbolUpdated() && !IsFinnhubInquiring()) {
		p = m_FinnhubFactory.CreateProduct(this, __CRYPTO_SYMBOLS__);
		p->SetIndex(m_lCurrentCryptoExchangePos++);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		if (m_lCurrentCryptoExchangePos >= m_dataFinnhubCryptoExchange.GetCryptoExchangeSize()) {
			SetFinnhubCryptoSymbolUpdated(true);
			gl_systemMessage.PushInformationMessage(_T("Finnhub Crypto sysbols已更新"));
			m_lCurrentCryptoExchangePos = 0;
		}
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCryptoDayLine(void) {
	bool fFound = false;
	CCryptoSymbolPtr pCryptoSymbol;
	CString str = _T("");
	const long lStockSetSize = m_vCryptoSymbol.size();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

	ASSERT(IsSystemReady());
	if (!IsFinnhubCryptoDayLineUpdated() && !IsFinnhubInquiring()) {
		for (m_lCurrentUpdateCryptoDayLinePos = 0; m_lCurrentUpdateCryptoDayLinePos < lStockSetSize; m_lCurrentUpdateCryptoDayLinePos++) {
			if (GetCryptoSymbol(m_lCurrentUpdateCryptoDayLinePos)->IsDayLineNeedUpdate()) {
				pCryptoSymbol = GetCryptoSymbol(m_lCurrentUpdateCryptoDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __CRYPTO_CANDLES__);
			p->SetIndex(m_lCurrentUpdateCryptoDayLinePos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			pCryptoSymbol->SetDayLineNeedUpdate(false);
			//TRACE("申请Crypto %s日线数据\n", pCryptoSymbol->GetSymbol().GetBuffer());
		}
		else {
			SetFinnhubCryptoDayLineUpdated(true);
			TRACE("Finnhub Crypto日线更新完毕\n");
			str = _T("Crypto DayLine Updated");
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
	CString str;

	if (!IsTiingoSymbolUpdated() && !IsTiingoInquiring()) {
		CWebSourceDataProductPtr p = m_TiingoFactory.CreateProduct(this, __STOCK_SYMBOLS__);
		m_qTiingoProduct.push(p);
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
			CWebSourceDataProductPtr p = m_TiingoFactory.CreateProduct(this, __STOCK_PRICE_CANDLES__);
			p->SetIndex(m_mapWorldStock.at(pStock->GetSymbol()));
			m_qTiingoProduct.push(p);
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

bool CWorldMarket::TaskUpdateCryptoExchangeDB(void) {
	CreatingThreadUpdateCryptoExchangeDB();

	return true;
}

bool CWorldMarket::UpdateForexExchangeDB(void) {
	return m_dataFinnhubForexExchange.UpdateDB();
}

bool CWorldMarket::UpdateCryptoExchangeDB(void) {
	return m_dataFinnhubCryptoExchange.UpdateDB();
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
	CForexSymbolPtr pSymbol = nullptr;

	for (int i = 0; i < m_dataFinnhubForexSymbol.GetForexSymbolSize(); i++) {
		if (gl_fExitingSystem) {
			break; // 如果程序正在退出，则停止存储。
		}
		pSymbol = m_dataFinnhubForexSymbol.GetForexSymbol(i);
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

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将Crypto日线数据存入数据库．
//  此函数由工作线程ThreadCryptoDayLineSaveProc调用，尽量不要使用全局变量。(目前使用主线程调用之，以消除同步问题的出现）
//
// 无论是否执行了存储函数，都需要将下载的日线历史数据删除，这样能够节省内存的占用。由于实际存储功能使用线程模式实现，
// 故而其执行时间可能晚于主线程，导致主线程删除日线数据时出现同步问题。解决的方法是让工作线程独立删除存储后的日线数据，
// 主线程的删除函数只在不调用工作线程（无需存储日线数据）的情况下方才执行。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskUpdateCryptoDayLineDB(void) {
	CString str;
	bool fUpdated = false;

	for (auto& pSymbol : m_vCryptoSymbol) {
		if (gl_fExitingSystem) {
			break; // 如果程序正在退出，则停止存储。
		}
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					CreatingThreadUpdateCryptoDayLineDB(pSymbol.get());
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
		if (IsFinnhubSymbolUpdated() && IsFinnhubForexExchangeUpdated() && IsFinnhubForexSymbolUpdated() && IsFinnhubCryptoExchangeUpdated() && IsFinnhubCryptoSymbolUpdated()) {
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
	CreatingthreadUpdateDayLneStartEndDate();
	return true;
}

bool CWorldMarket::CreatingthreadUpdateDayLneStartEndDate(void) {
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

bool CWorldMarket::CreatingThreadUpdateForexSymbolDB() {
	thread thread1(ThreadUpdateForexSymbolDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateForexDayLineDB(CProductFinnhubForexSymbol* pSymbol) {
	thread thread1(ThreadUpdateForexDayLineDB, pSymbol);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateCryptoExchangeDB(void) {
	thread thread1(ThreadUpdateCryptoExchangeDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateCryptoSymbolDB() {
	thread thread1(ThreadUpdateCryptoSymbolDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::CreatingThreadUpdateCryptoDayLineDB(CFinnhubCryptoSymbol* pSymbol) {
	thread thread1(ThreadUpdateCryptoDayLineDB, pSymbol);
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
	return m_dataFinnhubEconomicCalendar.Update(vEconomicCalendar);
}

void CWorldMarket::AddCryptoSymbol(CCryptoSymbolPtr pCryptoSymbol) {
	m_mapCryptoSymbol[pCryptoSymbol->GetSymbol()] = m_mapCryptoSymbol.size();
	m_vCryptoSymbol.push_back(pCryptoSymbol);
}

bool CWorldMarket::DeleteCryptoSymbol(CCryptoSymbolPtr pCryptoSymbol) {
	if (pCryptoSymbol == nullptr) return false;
	if (!IsCryptoSymbol(pCryptoSymbol->GetSymbol())) return false;

	m_mapCryptoSymbol.erase(pCryptoSymbol->GetSymbol());
	auto it = find(m_vCryptoSymbol.begin(), m_vCryptoSymbol.end(), pCryptoSymbol);
	m_vCryptoSymbol.erase(it);

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

CWebSourceDataProductPtr CWorldMarket::GetFinnhubInquiry(void) {
	ASSERT(m_qFinnhubProduct.size() > 0);

	CWebSourceDataProductPtr p = m_qFinnhubProduct.front();
	m_qFinnhubProduct.pop();

	return p;
}

CWebSourceDataProductPtr CWorldMarket::GetTiingoInquiry(void) {
	ASSERT(m_qTiingoProduct.size() > 0);

	CWebSourceDataProductPtr p = m_qTiingoProduct.front();
	m_qTiingoProduct.pop();

	return p;
}

void CWorldMarket::StopReceivingWebSocket(void) {
	m_FinnhubWebSocket.Deconnecting();
	m_TiingoIEXWebSocket.Deconnecting();
	m_TiingoCryptoWebSocket.Deconnecting();
	m_TiingoForexWebSocket.Deconnecting();
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
	return m_dataFinnhubStockExchange.LoadDB();
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
			pWorldStock->CheckProfileUpdateStatus(GetMarketDate());
			pWorldStock->CheckDayLineUpdateStatus(GetMarketDate(), GetLastTradeDate(), GetMarketTime(), GetDayOfWeek());
			pWorldStock->CheckEPSSurpriseStatus(GetMarketDate());
			pWorldStock->CheckPeerStatus(GetMarketDate());
			pWorldStock->CheckInsiderTransactionStatus(GetMarketDate());
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
	str = _T("WorldMarket股票代码最长长度为");
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
		gl_systemMessage.PushErrorMessage(_T("UpdateStockProfileDB任务用时超过五分钟"));
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
	TRACE("更新WorldMarket stock用时%lld毫秒\n", tt);

	return true;
}

bool CWorldMarket::UpdateStockDayLineDB(void) {
	CWorldStockPtr pStock = nullptr;

	for (long i = 0; i < GetTotalStock(); i++) {
		pStock = GetStock(i);
		pStock->UpdateDayLineDB();
		if (gl_fExitingSystem) {
			break; // 如果程序正在退出，则停止存储。
		}
	}
	return true;
}

bool CWorldMarket::UpdateForexSymbolDB(void) {
	return m_dataFinnhubForexSymbol.UpdateDB();
}

bool CWorldMarket::UpdateCryptoSymbolDB(void) {
	const long lTotalCryptoSymbol = m_vCryptoSymbol.size();
	CCryptoSymbolPtr pSymbol = nullptr;
	CSetFinnhubCryptoSymbol setCryptoSymbol;
	bool fUpdateSymbol = false;

	if (m_lLastTotalCryptoSymbol < lTotalCryptoSymbol) {
		setCryptoSymbol.Open();
		setCryptoSymbol.m_pDatabase->BeginTrans();
		for (long l = m_lLastTotalCryptoSymbol; l < lTotalCryptoSymbol; l++) {
			pSymbol = m_vCryptoSymbol.at(l);
			pSymbol->AppendSymbol(setCryptoSymbol);
		}
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();
		m_lLastTotalCryptoSymbol = lTotalCryptoSymbol;
	}

	for (auto& pSymbol2 : m_vCryptoSymbol) {
		if (pSymbol2->IsUpdateProfileDB()) {
			fUpdateSymbol = true;
			break;
		}
	}
	if (fUpdateSymbol) {
		setCryptoSymbol.Open();
		setCryptoSymbol.m_pDatabase->BeginTrans();
		while (!setCryptoSymbol.IsEOF()) {
			if (m_mapCryptoSymbol.find(setCryptoSymbol.m_Symbol) != m_mapCryptoSymbol.end()) {
				pSymbol = m_vCryptoSymbol.at(m_mapCryptoSymbol.at(setCryptoSymbol.m_Symbol));
				if (pSymbol->IsUpdateProfileDB()) {
					pSymbol->UpdateSymbol(setCryptoSymbol);
					pSymbol->SetUpdateProfileDB(false);
				}
			}
			setCryptoSymbol.MoveNext();
		}
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();
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
	return m_dataFinnhubEconomicCalendar.UpdateDB();
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
	return m_dataFinnhubForexExchange.LoadDB();
}

bool CWorldMarket::LoadForexSymbol(void) {
	return m_dataFinnhubForexSymbol.LoadDB();
}

bool CWorldMarket::LoadCryptoExchange(void) {
	return m_dataFinnhubCryptoExchange.LoadDB();
}

bool CWorldMarket::LoadCryptoSymbol(void) {
	return m_dataFinnhubForexSymbol.LoadDB();
}

bool CWorldMarket::LoadWorldChoicedForex(void)
{
	CSetWorldChoicedForex setWorldChoicedForex;
	CForexSymbolPtr pForex = nullptr;

	setWorldChoicedForex.Open();
	setWorldChoicedForex.m_pDatabase->BeginTrans();
	while (!setWorldChoicedForex.IsEOF()) {
		if (IsForexSymbol(setWorldChoicedForex.m_Symbol)) {
			pForex = GetForexSymbol(setWorldChoicedForex.m_Symbol);
			m_mapWorldChoicedForex[setWorldChoicedForex.m_Symbol] = m_mapWorldChoicedForex.size();
			m_vWorldChoicedForex.push_back(pForex);
		}
		else {
			setWorldChoicedForex.Delete(); // 清除非法股票代码
		}
		setWorldChoicedForex.MoveNext();
	}
	setWorldChoicedForex.m_pDatabase->CommitTrans();
	setWorldChoicedForex.Close();

	return true;
}

bool CWorldMarket::LoadWorldChoicedCrypto(void)
{
	CSetWorldChoicedCrypto setWorldChoicedCrypto;
	CCryptoSymbolPtr pCrypto = nullptr;

	setWorldChoicedCrypto.Open();
	setWorldChoicedCrypto.m_pDatabase->BeginTrans();
	while (!setWorldChoicedCrypto.IsEOF()) {
		if (IsCryptoSymbol(setWorldChoicedCrypto.m_Symbol)) {
			pCrypto = GetCryptoSymbol(setWorldChoicedCrypto.m_Symbol);
			m_mapWorldChoicedCrypto[setWorldChoicedCrypto.m_Symbol] = m_mapWorldChoicedCrypto.size();
			m_vWorldChoicedCrypto.push_back(pCrypto);
		}
		else {
			setWorldChoicedCrypto.Delete(); // 清除非法股票代码
		}
		setWorldChoicedCrypto.MoveNext();
	}
	setWorldChoicedCrypto.m_pDatabase->CommitTrans();
	setWorldChoicedCrypto.Close();

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
	return m_dataFinnhubEconomicCalendar.LoadDB();
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

/// <summary>
/// finnhub数据源的格式：wss://ws.finnhub.io/?token=c1i57rv48v6vit20lrc0。
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CWorldMarket::ConnectFinnhubWebSocket(void) {
	std::string url("wss://ws.finnhub.io");
	CString strToken = gl_pFinnhubWebInquiry->GetInquiringStringSuffix();
	strToken = "/?" + strToken.Right(strToken.GetLength() - 1);
	url += strToken.GetBuffer();

	m_FinnhubWebSocket.Connecting(url, FunctionProcessFinnhubWebSocket);

	return true;
}

/// <summary>
/// Finnhub WebSocket的免费账户最多只能发送50个证券的数据
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CWorldMarket::SendFinnhubWebSocketMessage(void) {
	ix::WebSocketSendInfo info;
	string preffix = _T("{\"type\":\"subscribe\",\"symbol\":\"");
	string suffix = _T("\"}");
	string strSymbol, strMessage;
	CString symbol;
	ASSERT(m_FinnhubWebSocket.IsOpen());

	for (auto pStock : m_vWorldChoicedStock) {
		strMessage = CreateFinnhubWebSocketString(pStock->GetSymbol());
		m_FinnhubWebSocket.Send(strMessage);
	}

	for (auto pCrypto : m_vWorldChoicedCrypto) {
		strMessage = CreateFinnhubWebSocketString(pCrypto->GetSymbol());
		m_FinnhubWebSocket.Send(strMessage);
	}

	for (auto pForex : m_vWorldChoicedForex) {
		strMessage = CreateFinnhubWebSocketString(pForex->GetSymbol());
		m_FinnhubWebSocket.Send(strMessage);
	}

	// Send a message to the server (default to TEXT mode)
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"BINANCE:BTCUSDT\"}"); //{"type":"subscribe","symbol":"BINANCE:BTCUSDT"}
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"BINANCE:LTCBTC\"}"); //{"type":"subscribe","symbol":"BINANCE:LTCBTC"}
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"IC MARKETS:1\"}"); //
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"OANDA:AUD_SGD\"}"); // OANDA:AUD_SGD
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"FXCM:USD/JPY\"}"); // FXCM:USD/JPY

	return false;
}

/// <summary>
/// Finnhub web socket格式： {"type":"subscribe","symbol":"符号串"},IEX， Crypto, Forex皆使用此格式
/// 如{"type":"subscribe","symbol":"MSFT"}, {"type":"subscribe","symbol":"BINANCE:LTCBTC"}, {"type":"subscribe","symbol":"OANDA:AUD_SGD"}
/// </summary>
/// <param name="strSymbol"></param>
/// <returns></returns>
string CWorldMarket::CreateFinnhubWebSocketString(CString strSymbol) {
	string sPreffix = _T("{\"type\":\"subscribe\",\"symbol\":\"");
	string sSuffix = _T("\"}");
	string sSymbol = strSymbol.GetBuffer();

	return sPreffix + sSymbol + sSuffix;
}

/// <summary>
/// Tiingo IEX的数据源格式：wss://api.tiingo.com/iex，其密钥是随后发送的。
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CWorldMarket::ConnectTiingoIEXWebSocket(void) {
	std::string url("wss://api.tiingo.com/iex");

	m_TiingoIEXWebSocket.Connecting(url, FunctionProcessTiingoIEXWebSocket);

	return true;
}

/// <summary>
/// Tiingo Crypto的数据源格式：wss://api.tiingo.com/crypto，其密钥是随后发送的。
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CWorldMarket::ConnectTiingoCryptoWebSocket(void) {
	std::string url("wss://api.tiingo.com/crypto");

	m_TiingoCryptoWebSocket.Connecting(url, FunctionProcessTiingoCryptoWebSocket);

	return true;
}

/// <summary>
/// Tiingo Forex的数据源格式：wss://api.tiingo.com/fx，其密钥是随后发送的。
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CWorldMarket::ConnectTiingoForexWebSocket(void) {
	std::string url("wss://api.tiingo.com/fx");

	m_TiingoForexWebSocket.Connecting(url, FunctionProcessTiingoForexWebSocket);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo对免费账户的流量限制，为500次/小时， 20000次/天， 5GB/月。
//
// thresholdlevel 0接收所有的IEX数据时，每秒数据量为1M-9M;thresholdlevel5接收所有IEX数据时，每秒数据量为10-50K。
//
// thresholdlevel 5：all Last Trade updates and only Quote updates that are deemed major updates by our system.
// thresholdlevel 0: ALL Top-of-Book AND Last Trade updates.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::SendTiingoIEXWebSocketMessage(void) {
	static bool sm_fSendAuth = true;
	CString str;
	CString strSymbols;
	CString strPreffix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":[");
	CString strSuffix = _T("]}}");
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();
	strAuth = strAuth.Right(strAuth.GetLength() - 7);

	strSymbols = CreateTiingoIEXWebSocketSymbolString(); // 去除最后多余的字符','

	str = strPreffix + strAuth + strMiddle + strSymbols + strSuffix;

	string messageAuth(str);
	ix::WebSocketSendInfo info;

	ASSERT(m_TiingoIEXWebSocket.IsOpen());

	if (sm_fSendAuth) {
		info = m_TiingoIEXWebSocket.Send(messageAuth);
	}

	return true;
}

CString CWorldMarket::CreateTiingoIEXWebSocketSymbolString(void) {
	CString strSymbols;
	CString strSymbol;

	for (auto pStock : m_vWorldChoicedStock) {
		strSymbol = _T("\"") + pStock->GetSymbol() + _T("\"") + _T(",");
		strSymbols += strSymbol;
	}
	strSymbols = strSymbols.Left(strSymbols.GetLength() - 1); // 去除最后多余的字符','

	return strSymbols;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 接收所有的Crypto数据时，每秒数据量大致在50-100K附近。
//
// thresholdlevel 2: Top-of-Book AND Last Trade updates.
// thresholdlevel 5: only Last Trade updates.
//
//////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::SendTiingoCryptoWebSocketMessage(void) {
	static bool sm_fSendAuth = true;
	CString str;
	CString strPreffix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":2,\"tickers\":["); // 5：Trade Updates per-exchange.2：Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	CString strSuffix = _T("]}}"); // 5：Trade Updates per-exchange.2：Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();
	strAuth = strAuth.Right(strAuth.GetLength() - 7);
	CString strSymbols = CreateTiingoCryptoWebSocketSymbolString();

	str = strPreffix + strAuth + strMiddle + strSymbols + strSuffix;

	string messageAuth(str);
	ix::WebSocketSendInfo info;

	ASSERT(m_TiingoCryptoWebSocket.IsOpen());

	if (sm_fSendAuth) {
		info = m_TiingoCryptoWebSocket.Send(messageAuth);
	}

	return true;
}

CString CWorldMarket::CreateTiingoCryptoWebSocketSymbolString(void) {
	CString strSymbols;
	CString strSymbol;

	for (auto pCrypto : m_vWorldChoicedCrypto) {
		strSymbol = _T("\"") + pCrypto->GetSymbol() + _T("\"") + _T(",");
		strSymbols += strSymbol;
	}
	strSymbols = strSymbols.Left(strSymbols.GetLength() - 1); // 去除最后多余的字符','

	return strSymbols;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 5状态下每秒接收100K左右。
//
// threshlodlevel 5: ALL Top-of-Book updates
// thresholdlevel 7: A top-of-book update that is due to a change in either the bid/ask price or size.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::SendTiingoForexWebSocketMessage(void) {
	static bool sm_fSendAuth = true;
	CString str;
	CString strPreffix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":["); //7：A top - of - book update that is due to a change in either the bid / ask price or size.
	CString strSuffix = _T("]}}"); //7：A top - of - book update that is due to a change in either the bid / ask price or size.
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();
	strAuth = strAuth.Right(strAuth.GetLength() - 7);
	CString strSymbols = CreateTiingoForexWebSocketSymbolString();

	str = strPreffix + strAuth + strMiddle + strSymbols + strSuffix;

	string messageAuth(str);
	ix::WebSocketSendInfo info;

	ASSERT(m_TiingoForexWebSocket.IsOpen());

	if (sm_fSendAuth) {
		info = m_TiingoForexWebSocket.Send(messageAuth);
	}

	return true;
}

CString CWorldMarket::CreateTiingoForexWebSocketSymbolString(void) {
	CString strSymbols;
	CString strSymbol;

	for (auto pForex : m_vWorldChoicedForex) {
		strSymbol = _T("\"") + pForex->GetSymbol() + _T("\"") + _T(",");
		strSymbols += strSymbol;
	}
	strSymbols = strSymbols.Left(strSymbols.GetLength() - 1); // 去除最后多余的字符','

	return strSymbols;
}

bool CWorldMarket::TaskProcessWebSocketData(void) {
	m_iWebSocketReceivedNumberPerSecond = gl_WebInquirer.GetFinnhubWebSocketDataSize()
		+ gl_WebInquirer.GetTiingoIEXWebSocketDataSize()
		+ gl_WebInquirer.GetTiingoCryptoWebSocketDataSize()
		+ gl_WebInquirer.GetTiingoForexWebSocketDataSize();

	ProcessFinnhubWebSocketData();
	ProcessTiingoIEXWebSocketData();
	ProcessTiingoCryptoWebSocketData();
	ProcessTiingoForexWebSocketData();

	return true;
}

bool CWorldMarket::ProcessFinnhubWebSocketData() {
	auto total = gl_WebInquirer.GetFinnhubWebSocketDataSize();
	shared_ptr<string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopFinnhubWebSocketData();
		iTotalDataSize += pString->size();
		ParseFinnhubWebSocketData(pString);
	}
	m_iProcessedFinnhubWebSocket = iTotalDataSize;

	return true;
}

bool CWorldMarket::ProcessTiingoIEXWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoIEXWebSocketDataSize();

	shared_ptr<string> pString;
	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoIEXWebSocketData();
		iTotalDataSize += pString->size();
		ParseTiingoIEXWebSocketData(pString);
	}
	m_iProcessedTiingoIEXWebSocket = iTotalDataSize;
	return true;
}

bool CWorldMarket::ProcessTiingoCryptoWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoCryptoWebSocketDataSize();

	shared_ptr<string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoCryptoWebSocketData();
		iTotalDataSize += pString->size();
		ParseTiingoCryptoWebSocketData(pString);
	}
	m_iProcessedTiingoCryptoWebSocket = iTotalDataSize;
	return true;
}

bool CWorldMarket::ProcessTiingoForexWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoForexWebSocketDataSize();
	shared_ptr<string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoForexWebSocketData();
		iTotalDataSize += pString->size();
		ParseTiingoForexWebSocketData(pString);
	}
	m_iProcessedTiingoForexWebSocket = iTotalDataSize;
	return true;
}

/// <summary>
/// 更新证券的实时信息
/// 目前只使用IEX和Finnhub的数据，其他数据抛弃掉
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CWorldMarket::TaskUpdateWorldStockFromWebSocket(void) {
	CTiingoIEXSocketPtr pIEXData;
	CTiingoCryptoSocketPtr pCryptoData;
	CTiingoForexSocketPtr pForexData;
	CFinnhubSocketPtr pFinnhubData;
	CWorldStockPtr pStock = nullptr;

	auto total = gl_SystemData.GetTiingoIEXSocketSize();

	for (auto i = 0; i < total; i++) {
		pIEXData = gl_SystemData.PopTiingoIEXSocket();
		UpdateWorldStockFromTiingoIEXSocket(pIEXData);
	}

	total = gl_SystemData.GetTiingoCryptoSocketSize();
	for (auto i = 0; i < total; i++) {
		pCryptoData = gl_SystemData.PopTiingoCryptoSocket();
	}

	total = gl_SystemData.GetTiingoForexSocketSize();
	for (auto i = 0; i < total; i++) {
		pForexData = gl_SystemData.PopTiingoForexSocket();
	}

	total = gl_SystemData.GetFinnhubSocketSize();
	for (auto i = 0; i < total; i++) {
		pFinnhubData = gl_SystemData.PopFinnhubSocket();
		UpdateWorldStockFromFinnhubSocket(pFinnhubData);
	}

	return true;
}

bool CWorldMarket::UpdateWorldStockFromTiingoIEXSocket(CTiingoIEXSocketPtr pTiingoIEXbData) {
	CWorldStockPtr pStock = nullptr;

	if (IsStock(pTiingoIEXbData->m_strSymbol)) {
		pStock = GetStock(pTiingoIEXbData->m_strSymbol);
		pStock->SetActive(true);
		switch (pTiingoIEXbData->m_chMessageType) {
		case 'T':
			pStock->SetNew(pTiingoIEXbData->m_dLastPrice * 1000);
			break;
		case 'Q':
			break;
		}
	}

	return true;
}

bool CWorldMarket::UpdateWorldStockFromFinnhubSocket(CFinnhubSocketPtr pFinnhubData) {
	CWorldStockPtr pStock = nullptr;

	if (IsStock(pFinnhubData->m_strSymbol)) {
		pStock = GetStock(pFinnhubData->m_strSymbol);
		pStock->SetActive(true);
		pStock->SetNew(pFinnhubData->m_dLastPrice * 1000);
	}

	return true;
}