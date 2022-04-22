#include"pch.h"

#include "WorldMarket.h"
#include"SystemData.h"

#include "WorldMarket.h"
#include"FinnhubInquiryType.h"
#include"thread.h"
#include"Callablefunction.h"
#include"WebInquirer.h"

#include"FinnhubInquiryType.h"

#include"ProductFinnhubStockPriceQuote.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"

#include"SetWorldMarketOption.h"
#include"SetWorldStockDayLine.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include<ixwebsocket/IXWebSocketMessage.h>

using namespace std;
#include<algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CWorldMarket, CVirtualMarket)

CWorldMarket::CWorldMarket() {
	static int siInstance = 0;

	if (++siInstance > 1) {
		TRACE("CWorldMarket市场变量只允许存在一个实例\n");
	}

	// 无需（也无法）每日更新的变量放在这里
	m_fFinnhubEPSSurpriseUpdated = true;
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // 由于证券代码总数有二十万之多，无法在一天之内更新完，故不再重置此索引。

	m_strMarketId = _T("美国市场");
	m_lMarketTimeZone = 4 * 3600; // 美国股市使用美东标准时间。
	CalculateTime();

	Reset();

	m_fRecordFinnhubWebSocket = true;
	m_fRecordTiingoCryptoWebSocket = true;
	m_fRecordTiingoForexWebSocket = true;
	m_fRecordTiingoIEXWebSocket = true;

	thread thread1(ThreadWorldMarketBackground);
	thread1.detach();
}

CWorldMarket::~CWorldMarket() {
	PreparingExitMarket();

	if (!gl_fExitingSystem) {
		gl_fExitingSystem = true;
		while (gl_ThreadStatus.IsWorldMarketBackgroundThreadRunning()) Sleep(1);
		gl_fExitingSystem = false;
	}
	else {
		while (gl_ThreadStatus.IsWorldMarketBackgroundThreadRunning()) Sleep(1);
	}
}

void CWorldMarket::Reset(void) {
	ResetFinnhub();
	ResetTiingo();
	ResetDataClass();
}

void CWorldMarket::ResetFinnhub(void) {
	m_pCurrentFinnhubProduct = nullptr;

	m_dataFinnhubStockExchange.Reset();
	m_dataFinnhubForexExchange.Reset();

	// Finnhub各申请网络数据标识，每日需要重置。
	m_fFinnhubSymbolUpdated = false; // 每日需要更新代码
	m_fFinnhubStockProfileUpdated = false;
	m_fFinnhubStockBasicFinancialUpdated = false;
	m_fFinnhubDayLineUpdated = false;
	m_fFinnhubForexExchangeUpdated = false;
	m_fFinnhubForexSymbolUpdated = false;
	m_fFinnhubForexDayLineUpdated = false;
	m_fFinnhubCryptoExchangeUpdated = false;
	m_fFinnhubCryptoSymbolUpdated = false;
	m_fFinnhubCryptoDayLineUpdated = false;
	m_fCountryListUpdated = false;
	m_fFinnhubEconomicCalendarUpdated = false;

	m_fFinnhubPeerUpdated = false;
	m_fFinnhubInsiderTransactionUpdated = false;

	m_fFinnhubInquiring = false;
	m_fFinnhubDataReceived = true;

	m_fRebulidDayLine = false;
	SetSystemReady(false); // 市场初始状态为未设置好。

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

	m_fTiingoStockSymbolUpdated = false;
	m_fTiingoCryptoSymbolUpdated = false;
	m_fTiingoDayLineUpdated = false;
}

void CWorldMarket::ResetDataClass(void) {
	m_dataFinnhubStockExchange.Reset();
	m_dataFinnhubForexExchange.Reset();
	m_dataFinnhubCryptoExchange.Reset();
	m_dataFinnhubForexSymbol.Reset();
	m_dataFinnhubCryptoSymbol.Reset();
	m_dataFinnhubCountry.Reset();
	m_dataFinnhubEconomicCalendar.Reset();

	m_dataWorldStock.Reset();
	m_dataTiingoStock.Reset();

	m_dataChoicedStock.Reset();
	m_dataChoicedForex.Reset();
	m_dataChoicedCrypto.Reset();
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
	LoadFinnhubCryptoSymbol();
	LoadWorldChoicedCrypto();
	LoadEconomicCalendarDB();
	LoadTiingoStock();
	LoadTiingoCryptoSymbol();

	CString str = _T("重置World Market于美东标准时间：");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);
}

bool CWorldMarket::PreparingExitMarket(void)
{
	ASSERT(gl_fExitingSystem);
	StopReceivingWebSocket();
	while (gl_ThreadStatus.IsWorldMarketBackgroundThreadRunning()) Sleep(1);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// 默认每100毫秒执行一次。
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::SchedulingTask(void) {
	CVirtualMarket::SchedulingTask();

	static time_t s_lastTimeSchedulingTask = 0;
	static int s_iCountfinnhubLimit = 12; // Finnhub.io每1.5秒左右申请一次，以防止出现频率过高的情况。
	static int s_iCountTiingoLimit = 80; // 保证每80次执行一次（即8秒每次）.Tiingo免费账户速度限制为每小时500次， 每分钟9次，故每次8秒即可。
	const long lCurrentTime = GetMarketTime();

	TaskCheckSystemReady();

	if (--s_iCountfinnhubLimit < 0) {
		TaskInquiryFinnhub(lCurrentTime);
		if (IsFinnhubInquiring()) {
			ASSERT(gl_pFinnhubWebInquiry->GetShortestInquiringInterval() > 100);
			s_iCountfinnhubLimit = gl_pFinnhubWebInquiry->GetShortestInquiringInterval() / 100; // 如果申请了网络数据，则重置计数器，以便申请下一次。
		}
		if (gl_pFinnhubWebInquiry->IsWebError()) { // finnhub.io有时被屏蔽，故而出现错误时暂停一会儿。
			gl_pFinnhubWebInquiry->SetWebError(false);
			s_iCountfinnhubLimit = 6000; // 如果出现错误，则每10分钟重新申请一次。
		}
	}
	ProcessFinnhubWebDataReceived(); // 要先处理收到的Finnhub网络数据
	ProcessFinnhubInquiringMessage(); // 然后再申请处理下一个

	if (--s_iCountTiingoLimit < 0) {
		ASSERT(gl_pTiingoWebInquiry->GetShortestInquiringInterval() > 100);
		s_iCountTiingoLimit = gl_pTiingoWebInquiry->GetShortestInquiringInterval() / 100;
		TaskInquiryTiingo();
		if (gl_pTiingoWebInquiry->IsWebError()) {
			gl_pTiingoWebInquiry->SetWebError(false);
			s_iCountTiingoLimit = 6000; // 如果出现错误，则每10分钟重新申请一次。
		}
	}
	ProcessTiingoWebDataReceived(); // 要先处理收到的Tiingo网络数据
	ProcessTiingoInquiringMessage(); // 然后再申请处理下一个

//根据时间，调度各项定时任务.每秒调度一次
	if (GetUTCTime() > s_lastTimeSchedulingTask) {
		SchedulingTaskPerSecond(GetUTCTime() - s_lastTimeSchedulingTask, lCurrentTime);
		s_lastTimeSchedulingTask = GetUTCTime();
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
	CFinnhubCryptoSymbolPtr pCryptoSymbol = nullptr;
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
	static int s_iCount1Hour = 3576; // 与五分钟每次的错开11秒钟，与一分钟每次的错开22秒钟
	static int s_iCount5Minute = 287; // 与一分钟每次的错开11秒钟
	static int s_iCount1Minute = 58; // 与10秒每次的错开1秒钟
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

	if (!IsTimeToResetSystem(lCurrentTime)) { // 下午五时重启系统，各数据库需要重新装入，故而此时不允许更新。
		TaskProcessWebSocketData();
		TaskUpdateWorldStockFromWebSocket();
	}

	return true;
}

bool CWorldMarket::SchedulingTaskPer10Seconds(long lCurrentTime) {
	return true;
}

bool CWorldMarket::SchedulingTaskPerMinute(long lCurrentTime) {
	TaskResetMarket(lCurrentTime);
	// 建立WebSocket连接
	TaskActivateWebSocket();

	// 这个必须是最后一个任务。因其在执行完毕后返回了。
	if (!IsTimeToResetSystem(lCurrentTime)) { // 下午五时重启系统，各数据库需要重新装入，故而此时不允许更新数据库。
		if (m_dataFinnhubCountry.GetLastTotalCountry() < m_dataFinnhubCountry.GetTotalCountry()) {
			TaskUpdateCountryListDB();
		}
		TaskUpdateForexExchangeDB();
		TaskUpdateForexSymbolDB();
		TaskUpdateCryptoExchangeDB();
		TaskUpdateFinnhubCryptoSymbolDB();
		TaskUpdateInsiderTransactionDB();
		TaskUpdateForexDayLineDB();
		TaskUpdateCryptoDayLineDB();
		TaskUpdateDayLineDB();
		TaskUpdateEPSSurpriseDB();
		TaskUpdateEconomicCalendarDB();
		return true;
	}

	return false;
}

bool CWorldMarket::SchedulingTaskPer5Minute(long lCurrentTime) {
	if (IsFinnhubSymbolUpdated() && IsStockProfileNeedUpdate()) {
		TaskUpdateStockProfileDB();
	}

	if (IsBasicFinancialNeedUpdate()) {
		TaskUpdateBasicFinancialDB();
	}

	TaskUpdateTiingoStockDB();
	TaskUpdateTiingoCryptoSymbolDB();

	return true;
}

bool CWorldMarket::SchedulingTaskPerHour(long lCurrentTime) {
	TaskReActivateWebSocket();

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
	if (((lCurrentTime < 165700) || (lCurrentTime > 170500))) { // 下午五时重启系统，故而此时不允许接收网络信息。
		TaskInquiryFinnhubCountryList();
		TaskInquiryFinnhubForexExchange();
		TaskInquiryFinnhubCryptoExchange();
		TaskInquiryFinnhubCompanySymbol(); // 第一个动作，首先申请当日证券代码
		TaskInquiryFinnhubForexSymbol();
		TaskInquiryFinnhubCryptoSymbol();
		//TaskInquiryFinnhubEconomicCalendar();

		// 申请Finnhub网络信息的任务，皆要放置在这里，以保证在市场时间凌晨十分钟后执行。这样能够保证在重启市场时不会执行查询任务
		if (IsSystemReady()) {
			TaskInquiryFinnhubCompanyProfileConcise();
			TaskInquiryFinnhubCompanyBasicFinancial();
			TaskInquiryFinnhubPeer();
			TaskInquiryFinnhubInsiderTransaction();
			TaskInquiryFinnhubCryptoDayLine();
			TaskInquiryFinnhubStockDayLine();
			//TaskInquiryFinnhubForexDayLine(); // Forex dayline目前只限于付费用户使用
			//TaskInquiryFinnhubEPSSurprise(); // 这个现在没什么用，暂时停止更新。
			if (!IsFinnhubStockDayLineUpdated()) {
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
		SetCurrentFunction(_T("Finnhub country List"));
		SetFinnhubInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub economic country List"));
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCompanySymbol(void) {
	static bool s_fInquiringFinnhubStockSymbol = false;
	bool fFound = false;
	CFinnhubStockExchangePtr pExchange;
	CString str = _T("");
	long lExchangeSize = m_dataFinnhubStockExchange.GetExchangeSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentStockExchangePos = 0;

	if (!IsFinnhubSymbolUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubStockSymbol) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock symbol..."));
			s_fInquiringFinnhubStockSymbol = true;
		}
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
			SetCurrentFunction(_T("Finnhub交易所代码:") + pExchange->m_strCode);
			SetFinnhubInquiring(true);
			pExchange->SetUpdated(true);
			//TRACE("申请%s交易所证券代码\n", pExchange->m_strCode.GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockSymbol = false;
			fHaveInquiry = false;
			SetFinnhubSymbolUpdated(true);
			TRACE("Finnhub交易所代码数据查询完毕\n");
			str = _T("Finnhub交易所代码数据查询完毕");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubCompanyProfileConcise(void) {
	static bool s_fInquiringFinnhubStockProfile = false;
	bool fFound = false;
	long lStockSetSize = GetStockSize();
	CString str = _T("");
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentProfilePos;

	ASSERT(IsSystemReady());
	if (!IsFinnhubStockProfileUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubStockProfile) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock profile..."));
			s_fInquiringFinnhubStockProfile = true;
		}
		for (lCurrentProfilePos = 0; lCurrentProfilePos < lStockSetSize; lCurrentProfilePos++) {
			if (!GetStock(lCurrentProfilePos)->IsProfileUpdated()) {
				fFound = true;
				break;
			}
		}
		if (fFound) {
			p = m_FinnhubFactory.CreateProduct(this, __COMPANY_PROFILE_CONCISE__);
			p->SetIndex(lCurrentProfilePos);
			m_qFinnhubProduct.push(p);
			SetCurrentFunction(_T("简介:") + m_dataWorldStock.GetStock(lCurrentProfilePos)->GetSymbol());
			//TRACE("更新%s简介\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetFinnhubInquiring(true);
			fHaveInquiry = true;
		}
		else {
			s_fInquiringFinnhubStockProfile = false;
			SetFinnhubStockProfileUpdated(true);
			TRACE("Finnhub股票简介更新完毕\n");
			str = _T("Finnhub股票简介更新完毕");
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubCompanyBasicFinancial(void) {
	static bool s_fInquiringFinnhubCompanyBasicFinancial = false;
	bool fFound = false;
	long lStockSetSize = GetStockSize();
	CString str = _T("");
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentBasicFinncialsPos;

	ASSERT(IsSystemReady());
	if (!IsFinnhubStockBasicFinancialUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubCompanyBasicFinancial) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock basic financials..."));
			s_fInquiringFinnhubCompanyBasicFinancial = true;
		}
		for (lCurrentBasicFinncialsPos = 0; lCurrentBasicFinncialsPos < lStockSetSize; lCurrentBasicFinncialsPos++) {
			if (!GetStock(lCurrentBasicFinncialsPos)->IsBasicFinancialUpdated()) {
				fFound = true;
				break;
			}
		}
		if (fFound) {
			p = m_FinnhubFactory.CreateProduct(this, __BASIC_FINANCIALS__);
			p->SetIndex(lCurrentBasicFinncialsPos);
			m_qFinnhubProduct.push(p);
			SetCurrentFunction(_T("基本财务:") + m_dataWorldStock.GetStock(lCurrentBasicFinncialsPos)->GetSymbol());
			//TRACE("更新%s简介\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetFinnhubInquiring(true);
			fHaveInquiry = true;
		}
		else {
			s_fInquiringFinnhubCompanyBasicFinancial = false;
			SetFinnhubStockBasicFinancialUpdated(true);
			TRACE("Finnhub股票基本财务情况更新完毕\n");
			str = _T("Finnhub股票基本财务情况更新完毕");
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubStockDayLine(void) {
	static bool s_fInquiringFinnhubStockDayLine = false;
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = GetStockSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

	ASSERT(IsSystemReady());
	if (!IsFinnhubStockDayLineUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubStockDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock day line..."));
			s_fInquiringFinnhubStockDayLine = true;
		}
		for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < lStockSetSize; m_lCurrentUpdateDayLinePos++) {
			pStock = GetStock(m_lCurrentUpdateDayLinePos);
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
			SetCurrentFunction(_T("日线:") + pStock->GetSymbol());
			//TRACE("申请%s日线数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockDayLine = false;
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
	static bool s_fInquiringFinnhubStockInsiderTransaction = false;
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = GetStockSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentUpdateInsiderTransactionPos = 0;

	ASSERT(IsSystemReady());
	if (!IsFinnhubInsiderTransactionUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubStockInsiderTransaction) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock insider transaction..."));
			s_fInquiringFinnhubStockInsiderTransaction = true;
		}
		for (lCurrentUpdateInsiderTransactionPos = 0; lCurrentUpdateInsiderTransactionPos < lStockSetSize; lCurrentUpdateInsiderTransactionPos++) {
			pStock = GetStock(lCurrentUpdateInsiderTransactionPos);
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
			p->SetIndex(lCurrentUpdateInsiderTransactionPos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			SetCurrentFunction(_T("内部交易:") + pStock->GetSymbol());
			//TRACE("申请%s 内部交易数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockInsiderTransaction = false;
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
	static long s_lCurrentRTDataQuotePos = 0;
	CWebSourceDataProductPtr p = nullptr;

	ASSERT(IsSystemReady());
	if (!IsFinnhubInquiring()) {
		p = make_shared<CProductFinnhubStockPriceQuote>();
		p->SetIndex(s_lCurrentRTDataQuotePos);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		s_lCurrentRTDataQuotePos++;
		if (s_lCurrentRTDataQuotePos == GetStockSize()) s_lCurrentRTDataQuotePos = 0;
		SetCurrentFunction(_T("Finnhub stock RT: ") + GetStock(s_lCurrentRTDataQuotePos)->GetSymbol());
		TRACE("申请%s实时数据\n", GetStock(s_lCurrentRTDataQuotePos)->GetSymbol().GetBuffer());
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubPeer(void) {
	static bool s_fInquiringFinnhubStockPeer = false;
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = GetStockSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentUpdatePeerPos = 0;

	ASSERT(IsSystemReady());
	if (!IsFinnhubPeerUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubStockPeer) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock peer..."));
			s_fInquiringFinnhubStockPeer = true;
		}
		for (lCurrentUpdatePeerPos = 0; lCurrentUpdatePeerPos < lStockSetSize; lCurrentUpdatePeerPos++) {
			if (!GetStock(lCurrentUpdatePeerPos)->IsPeerUpdated()) {
				pStock = GetStock(lCurrentUpdatePeerPos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __PEERS__);
			p->SetIndex(lCurrentUpdatePeerPos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			SetCurrentFunction(_T("Peer:") + GetStock(lCurrentUpdatePeerPos)->GetSymbol());
			//TRACE("申请%s Peer数据\n", GetStock(lCurrentUpdatePeerPos)->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockPeer = false;
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
		SetCurrentFunction(_T("Finnhub ecomomic calendar"));
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubEPSSurprise(void) {
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = GetStockSize();
	bool fHaveInquiry = false;

	ASSERT(IsSystemReady());
	if (!IsFinnhubEPSSurpriseUpdated() && !IsFinnhubInquiring()) {
		for (m_lCurrentUpdateEPSSurprisePos = 0; m_lCurrentUpdateEPSSurprisePos < lStockSetSize; m_lCurrentUpdateEPSSurprisePos++) {
			if (!GetStock(m_lCurrentUpdateEPSSurprisePos)->IsEPSSurpriseUpdated()) {
				pStock = GetStock(m_lCurrentUpdateEPSSurprisePos);
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
			GetStock(m_lCurrentUpdateEPSSurprisePos)->SetEPSSurpriseUpdated(true);
			TRACE("申请%s EPS Surprise数据\n", GetStock(m_lCurrentUpdateEPSSurprisePos)->GetSymbol().GetBuffer());
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
		SetCurrentFunction(_T("Finnhub forex exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub forex exchange"));
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubForexSymbol(void) {
	static long s_lCurrentForexExchangePos = 0;
	CWebSourceDataProductPtr p = nullptr;

	if (!IsFinnhubForexSymbolUpdated() && !IsFinnhubInquiring()) {
		p = m_FinnhubFactory.CreateProduct(this, __FOREX_SYMBOLS__);
		p->SetIndex(s_lCurrentForexExchangePos++);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		if (s_lCurrentForexExchangePos >= m_dataFinnhubForexExchange.GetForexExchangeSize()) {
			SetFinnhubForexSymbolUpdated(true);
			s_lCurrentForexExchangePos = 0;
			SetCurrentFunction(_T("Finnhub forex synmbol"));
			gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub Forex sysbols"));
		}
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubForexDayLine(void) {
	static bool s_fInquiringFinnhubForexDayLine = false;
	bool fFound = false;
	CForexSymbolPtr pForexSymbol;
	CString str = _T("");
	const long lStockSetSize = m_dataFinnhubForexSymbol.GetForexSymbolSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentUpdateForexDayLinePos = 0;

	ASSERT(IsSystemReady());
	if (!IsFinnhubForexDayLineUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubForexDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			s_fInquiringFinnhubForexDayLine = true;
		}
		for (lCurrentUpdateForexDayLinePos = 0; lCurrentUpdateForexDayLinePos < lStockSetSize; lCurrentUpdateForexDayLinePos++) {
			if (GetForexSymbol(lCurrentUpdateForexDayLinePos)->IsDayLineNeedUpdate()) {
				pForexSymbol = GetForexSymbol(lCurrentUpdateForexDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __FOREX_CANDLES__);
			p->SetIndex(lCurrentUpdateForexDayLinePos);
			m_qFinnhubProduct.push(p);
			SetCurrentFunction(_T("Finnhub Forex日线：") + pForexSymbol->GetSymbol());
			SetFinnhubInquiring(true);
			pForexSymbol->SetDayLineNeedUpdate(false);
			SetCurrentFunction(_T("日线:") + pForexSymbol->GetSymbol());
			TRACE("申请%s日线数据\n", pForexSymbol->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubForexDayLine = false;
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
		SetCurrentFunction(_T("Finnhub crypto exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub crypto exchange"));
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCryptoSymbol(void) {
	static long s_lCurrentCryptoExchangePos = 0;
	CWebSourceDataProductPtr p = nullptr;

	if (!IsFinnhubCryptoSymbolUpdated() && !IsFinnhubInquiring()) {
		p = m_FinnhubFactory.CreateProduct(this, __CRYPTO_SYMBOLS__);
		p->SetIndex(s_lCurrentCryptoExchangePos++);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		if (s_lCurrentCryptoExchangePos >= m_dataFinnhubCryptoExchange.GetCryptoExchangeSize()) {
			SetFinnhubCryptoSymbolUpdated(true);
			SetCurrentFunction(_T("Finnhub crypto synmbol"));
			gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub Crypto sysbols"));
			s_lCurrentCryptoExchangePos = 0;
		}
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCryptoDayLine(void) {
	static bool s_fInquiringFinnhubCryptoDayLine = false;
	bool fFound = false;
	CFinnhubCryptoSymbolPtr pCryptoSymbol;
	CString str = _T("");
	const long lStockSetSize = m_dataFinnhubCryptoSymbol.GetCryptoSymbolSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentUpdateCryptoDayLinePos = 0;

	ASSERT(IsSystemReady());
	if (!IsFinnhubCryptoDayLineUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubCryptoDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			s_fInquiringFinnhubCryptoDayLine = true;
		}
		for (lCurrentUpdateCryptoDayLinePos = 0; lCurrentUpdateCryptoDayLinePos < lStockSetSize; lCurrentUpdateCryptoDayLinePos++) {
			if (GetFinnhubCryptoSymbol(lCurrentUpdateCryptoDayLinePos)->IsDayLineNeedUpdate()) {
				pCryptoSymbol = GetFinnhubCryptoSymbol(lCurrentUpdateCryptoDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __CRYPTO_CANDLES__);
			p->SetIndex(lCurrentUpdateCryptoDayLinePos);
			m_qFinnhubProduct.push(p);
			SetCurrentFunction(_T("Finnhub Crypto日线：") + pCryptoSymbol->GetSymbol());
			SetFinnhubInquiring(true);
			pCryptoSymbol->SetDayLineNeedUpdate(false);
			SetCurrentFunction(_T("日线:") + pCryptoSymbol->GetSymbol());
			//TRACE("申请%s日线数据\n", pCryptoSymbol->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubCryptoDayLine = false;
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
		TaskInquiryTiingoCryptoSymbol();
		// 由于Tiingo规定每月只能查询500个代码，故测试成功后即暂时不使用。
		TaskInquiryTiingoDayLine(); // 初步测试完毕。
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryTiingoCompanySymbol(void) {
	if (!IsTiingoStockSymbolUpdated() && !IsTiingoInquiring()) {
		CWebSourceDataProductPtr p = m_TiingoFactory.CreateProduct(this, __STOCK_SYMBOLS__);
		m_qTiingoProduct.push(p);
		SetTiingoInquiring(true);
		SetCurrentFunction(_T("Tiingo stock synmbol"));
		gl_systemMessage.PushInformationMessage(_T("Tiingo stock symbol已更新"));

		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryTiingoCryptoSymbol(void) {
	if (!IsTiingoCryptoSymbolUpdated() && !IsTiingoInquiring()) {
		CWebSourceDataProductPtr p = m_TiingoFactory.CreateProduct(this, __CRYPTO_SYMBOLS__);
		m_qTiingoProduct.push(p);
		SetTiingoInquiring(true);
		SetCurrentFunction(_T("Tiingo crypto synmbol"));
		gl_systemMessage.PushInformationMessage(_T("Tiingo crypto symbol已更新"));

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
	long lStockSetSize = m_dataChoicedStock.GetSize();
	bool fHaveInquiry = false;

	ASSERT(IsSystemReady());
	if (!IsTiingoDayLineUpdated() && !IsTiingoInquiring()) {
		for (long lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			if (m_dataChoicedStock.GetStock(lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
				pStock = m_dataChoicedStock.GetStock(lCurrentUpdateDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			CWebSourceDataProductPtr p = m_TiingoFactory.CreateProduct(this, __STOCK_PRICE_CANDLES__);
			p->SetIndex(m_dataWorldStock.GetIndex(pStock->GetSymbol()));
			m_qTiingoProduct.push(p);
			SetCurrentFunction(_T("Tiingo Stock日线：") + pStock->GetSymbol());
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
		thread thread1(ThreadUpdateTiingoIndustry, this);
		thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateSICIndustry(void) {
	if (IsFinnhubStockProfileUpdated()) {
		thread thread1(ThreadUpdateSICIndustry, this);
		thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateNaicsIndustry(void) {
	if (IsFinnhubStockProfileUpdated()) {
		thread thread1(ThreadUpdateNaicsIndustry, this);
		thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
		return true;
	}
	return false;
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
					thread thread1(ThreadUpdateForexDayLineDB, pSymbol.get());
					thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
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
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;

	for (int i = 0; i < m_dataFinnhubCryptoSymbol.GetCryptoSymbolSize(); i++) {
		if (gl_fExitingSystem) {
			break; // 如果程序正在退出，则停止存储。
		}
		pSymbol = m_dataFinnhubCryptoSymbol.GetCryptoSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					thread thread1(ThreadUpdateCryptoDayLineDB, pSymbol.get());
					thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
					fUpdated = true;
					//TRACE("更新%s日线数据\n", pSymbol->GetSymbol().GetBuffer());
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

bool CWorldMarket::TaskUpdateEPSSurpriseDB(void) {
	CString str;

	CWorldStockPtr pStock = nullptr;
	for (long l = 0; l < m_dataWorldStock.GetStockSize(); l++) {
		pStock = m_dataWorldStock.GetStock(l);
		if (pStock->IsEPSSurpriseNeedSaveAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			thread thread1(ThreadUpdateEPSSurpriseDB, pStock.get());
			thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
			TRACE("更新%s EPS surprise数据\n", pStock->GetSymbol().GetBuffer());
		}
		if (gl_fExitingSystem) {
			break; // 如果程序正在退出，则停止存储。
		}
	}

	return(true);
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
	thread thread1(ThreadUpdateWorldStockDayLineStartEndDate, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateDayLineDB() {
	thread thread1(ThreadUpdateWorldStockDayLineDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateStockProfileDB(void) {
	thread thread1(ThreadUpdateStockProfileDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateBasicFinancialDB(void) {
	thread thread1(ThreadUpdateBasicFinancialDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateCountryListDB(void) {
	thread thread1(ThreadUpdateCountryListDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateInsiderTransactionDB(void) {
	thread thread1(ThreadUpdateInsiderTransactionDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateTiingoStockDB(void) {
	thread thread1(ThreadUpdateTiingoStockDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateTiingoCryptoSymbolDB(void) {
	thread thread1(ThreadUpdateTiingoCryptoSymbolDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateForexExchangeDB(void) {
	thread thread1(ThreadUpdateForexExchangeDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateForexSymbolDB() {
	thread thread1(ThreadUpdateForexSymbolDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateCryptoExchangeDB(void) {
	thread thread1(ThreadUpdateCryptoExchangeDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateFinnhubCryptoSymbolDB() {
	thread thread1(ThreadUpdateFinnhubCryptoSymbolDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateEconomicCalendarDB(void) {
	thread thread1(ThreadUpdateEconomicCalendarDB, this);
	thread1.detach();

	return true;
}

void CWorldMarket::StopReceivingWebSocket(void) {
	m_finnhubWebSocket.Deconnecting();
	m_tiingoIEXWebSocket.Deconnecting();
	m_tiingoCryptoWebSocket.Deconnecting();
	m_tiingoForexWebSocket.Deconnecting();
}

bool CWorldMarket::LoadOption(void) {
	CSetWorldMarketOption setOption;

	setOption.Open();
	if (setOption.m_FinnhubToken.GetLength() > 5) {
		gl_pFinnhubWebInquiry->SetInquiryingStringSuffix(setOption.m_FinnhubToken);
		if (setOption.m_FinnhubMaxPerHour > 0) {
			gl_pFinnhubWebInquiry->SetShortestINquiringInterval(3600000 / setOption.m_FinnhubMaxPerHour);
		}
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Finnhub Token Needed"));
	}
	if (setOption.m_TiingoToken.GetLength() > 5) {
		gl_pTiingoWebInquiry->SetInquiryingStringSuffix(setOption.m_TiingoToken);
		if (setOption.m_TiingoMaxPerHour > 0) {
			gl_pTiingoWebInquiry->SetShortestINquiringInterval(3600000 / setOption.m_TiingoMaxPerHour);
		}
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Tiingo Token Needed"));
	}
	if (setOption.m_QuandlToken.GetLength() > 5) {
		gl_pQuandlWebInquiry->SetInquiryingStringSuffix(setOption.m_QuandlToken);
	}
	setOption.Close();

	return true;
}

bool CWorldMarket::UpdateStockDayLineDB(void) {
	CWorldStockPtr pStock = nullptr;

	for (long i = 0; i < GetStockSize(); i++) {
		pStock = GetStock(i);
		pStock->UpdateDayLineDB();
		if (gl_fExitingSystem) {
			break; // 如果程序正在退出，则停止存储。
		}
	}
	return true;
}

bool CWorldMarket::UpdateInsiderTransactionDB(void) {
	CString str;
	CWorldStockPtr pStock = nullptr;

	for (long i = 0; i < GetStockSize(); i++) {
		pStock = GetStock(i);
		if (pStock->IsInsiderTransactionNeedSaveAndClearFlag()) {
			if (pStock->HaveInsiderTransaction()) {
				pStock->SaveInsiderTransaction();
				str = pStock->GetSymbol() + _T("内部交易资料更新完成");
				gl_systemMessage.PushDayLineInfoMessage(str);
				//TRACE("更新%s内部交易数据\n", pStock->GetSymbol().GetBuffer());
			}
		}
		if (gl_fExitingSystem) {
			break; // 如果程序正在退出，则停止存储。
		}
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
	m_dataWorldStock.ResetEPSSurprise();
	SetFinnhubEPSSurpriseUpdated(false);

	return true;
}

bool CWorldMarket::RebuildPeer(void) {
	m_dataWorldStock.ResetPeer();
	SetFinnhubPeerUpdated(false);

	return true;
}

bool CWorldMarket::RebuildStockDayLineDB(void) {
	m_dataWorldStock.ResetDayLine();
	SetFinnhubStockProfileUpdated(false);

	return true;
}

bool CWorldMarket::UpdateStockDayLineStartEndDate(void) {
	CString strFilterPrefix = _T("[Symbol] = '");
	CString strFilter, str;
	CSetWorldStockDayLine setWorldStockDayLine;
	CWorldStockPtr pStock2 = nullptr;

	CWorldStockPtr pStock = nullptr;
	for (long l = 0; l < m_dataWorldStock.GetStockSize(); l++) {
		pStock = m_dataWorldStock.GetStock(l);
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
/// Finnhub WebSocket的免费账户最多只能发送50个证券的数据
/// </summary>
/// <param name=""></param>
/// <returns></returns>
vector<CString> CWorldMarket::GetFinnhubWebSocketSymbolVector(void) {
	vector<CString> vSymbol;

	CWorldStockPtr pStock = nullptr;
	for (long l = 0; l < m_dataChoicedStock.GetSize(); l++) {
		pStock = m_dataChoicedStock.GetStock(l);
		vSymbol.push_back(pStock->GetSymbol());
	}

	CFinnhubCryptoSymbolPtr pCrypto = nullptr;
	for (long l = 0; l < m_dataChoicedCrypto.GetSize(); l++) {
		pCrypto = m_dataChoicedCrypto.GetCrypto(l);
		vSymbol.push_back(pCrypto->GetSymbol());
	}

	CForexSymbolPtr pForex = nullptr;
	for (long l = 0; l < m_dataChoicedForex.GetSize(); l++) {
		pForex = m_dataChoicedForex.GetForex(l);
		vSymbol.push_back(pForex->GetSymbol());
	}

	// Send a message to the server (default to TEXT mode)
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"BINANCE:BTCUSDT\"}"); //{"type":"subscribe","symbol":"BINANCE:BTCUSDT"}
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"BINANCE:LTCBTC\"}"); //{"type":"subscribe","symbol":"BINANCE:LTCBTC"}
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"IC MARKETS:1\"}"); //
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"OANDA:AUD_SGD\"}"); // OANDA:AUD_SGD
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"FXCM:USD/JPY\"}"); // FXCM:USD/JPY

	return vSymbol;
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
vector<CString> CWorldMarket::GetTiingoIEXWebSocketSymbolVector(void) {
	CWorldStockPtr pStock = nullptr;
	vector<CString> vSymbol;
	for (long l = 0; l < m_dataChoicedStock.GetSize(); l++) {
		pStock = m_dataChoicedStock.GetStock(l);
		vSymbol.push_back(pStock->GetSymbol());
	}

	return vSymbol;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 接收所有的Crypto数据时，每秒数据量大致在50-100K附近。
//
// thresholdlevel 2: Top-of-Book AND Last Trade updates.
// thresholdlevel 5: only Last Trade updates.
//
//////////////////////////////////////////////////////////////////////////////////////////////
vector<CString> CWorldMarket::GetTiingoCryptoWebSocketSymbolVector(void) {
	CFinnhubCryptoSymbolPtr pCrypto = nullptr;
	vector<CString> vSymbol;
	for (long l = 0; l < m_dataChoicedCrypto.GetSize(); l++) {
		pCrypto = m_dataChoicedCrypto.GetCrypto(l);
		vSymbol.push_back(pCrypto->GetSymbol());
	}

	return vSymbol;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 5状态下每秒接收100K左右。
//
// threshlodlevel 5: ALL Top-of-Book updates
// thresholdlevel 7: A top-of-book update that is due to a change in either the bid/ask price or size.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
vector<CString> CWorldMarket::GetTiingoForexWebSocketSymbolVector(void) {
	CForexSymbolPtr pForex = nullptr;
	vector<CString> vSymbol;
	for (long l = 0; l < m_dataChoicedForex.GetSize(); l++) {
		pForex = m_dataChoicedForex.GetForex(l);
		vSymbol.push_back(pForex->GetSymbol());
	}

	return vSymbol;
}

bool CWorldMarket::TaskReActivateWebSocket(void) {
	if (IsSystemReady()) {
		if (!m_finnhubWebSocket.IsReceivingData()) {
			m_finnhubWebSocket.DeconnectingWithoutWaitingSucceed();
			gl_systemMessage.PushInnerSystemInformationMessage(_T("停止Finnhub web socket服务"));
		}
		else {
			m_finnhubWebSocket.SetReceivingData(false);
		}
		if (!m_tiingoIEXWebSocket.IsReceivingData()) {
			m_tiingoIEXWebSocket.DeconnectingWithoutWaitingSucceed();
			gl_systemMessage.PushInnerSystemInformationMessage(_T("停止Tiingo IEX web socket服务"));
		}
		else {
			m_tiingoIEXWebSocket.SetReceivingData(false);
		}
		if (!m_tiingoCryptoWebSocket.IsReceivingData()) {
			m_tiingoCryptoWebSocket.DeconnectingWithoutWaitingSucceed();
			gl_systemMessage.PushInnerSystemInformationMessage(_T("停止Tiingo Crypto web socket服务"));
		}
		else {
			m_tiingoCryptoWebSocket.SetReceivingData(false);
		}
		if (!m_tiingoForexWebSocket.IsReceivingData()) {
			m_tiingoForexWebSocket.DeconnectingWithoutWaitingSucceed();
			gl_systemMessage.PushInnerSystemInformationMessage(_T("停止Tiingo Forex web socket服务"));
		}
		else {
			m_tiingoForexWebSocket.SetReceivingData(false);
		}
	}
	return true;
}

bool CWorldMarket::TaskActivateWebSocket(void) {
	if (IsSystemReady()) {
		if (m_finnhubWebSocket.IsClosed()) {
			m_finnhubWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetFinnhubWebSocketSymbolVector());
			gl_systemMessage.PushInnerSystemInformationMessage(_T("开启Finnhub web socket服务"));
		}
		if (m_tiingoIEXWebSocket.IsClosed()) {
			m_tiingoIEXWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetTiingoIEXWebSocketSymbolVector());
			gl_systemMessage.PushInnerSystemInformationMessage(_T("开启Tiingo IEX web socket服务"));
		}
		if (m_tiingoCryptoWebSocket.IsClosed()) {
			m_tiingoCryptoWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetTiingoCryptoWebSocketSymbolVector());
			gl_systemMessage.PushInnerSystemInformationMessage(_T("开启Tiingo Crypto web socket服务"));
		}
		if (m_tiingoForexWebSocket.IsClosed()) {
			m_tiingoForexWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetTiingoForexWebSocketSymbolVector());
			gl_systemMessage.PushInnerSystemInformationMessage(_T("开启Tiingo Forex web socket服务"));
		}
	}
	return true;
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
	CString strMessage;
	shared_ptr<string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopFinnhubWebSocketData();
		if (m_fRecordFinnhubWebSocket) {
			strMessage = _T("Finnhub: ");
			strMessage += (*pString).c_str();
			gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		}
		iTotalDataSize += pString->size();
		m_finnhubWebSocket.ParseFinnhubWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedFinnhubWebSocket(iTotalDataSize);

	return true;
}

bool CWorldMarket::ProcessTiingoIEXWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoIEXWebSocketDataSize();
	CString strMessage;
	shared_ptr<string> pString;
	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoIEXWebSocketData();
		if (m_fRecordTiingoIEXWebSocket) {
			strMessage = _T("Tiingo IEX: ");
			strMessage += (*pString).c_str();
			gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		}
		iTotalDataSize += pString->size();
		m_tiingoIEXWebSocket.ParseTiingoIEXWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoIEXWebSocket(iTotalDataSize);
	return true;
}

bool CWorldMarket::ProcessTiingoCryptoWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoCryptoWebSocketDataSize();
	CString strMessage;

	shared_ptr<string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoCryptoWebSocketData();
		if (m_fRecordTiingoCryptoWebSocket) {
			strMessage = _T("Tiingo Crypto: ");
			strMessage += (*pString).c_str();
			gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		}
		iTotalDataSize += pString->size();
		m_tiingoCryptoWebSocket.ParseTiingoCryptoWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoCryptoWebSocket(iTotalDataSize);
	return true;
}

bool CWorldMarket::ProcessTiingoForexWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoForexWebSocketDataSize();
	CString strMessage;
	shared_ptr<string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoForexWebSocketData();
		if (m_fRecordTiingoForexWebSocket) {
			strMessage = _T("Tiingo Forex: ");
			strMessage += (*pString).c_str();
			gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		}
		iTotalDataSize += pString->size();
		m_tiingoForexWebSocket.ParseTiingoForexWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoForexWebSocket(iTotalDataSize);
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
		gl_systemMessage.SetCurrentTiingoWebSocketIEX(pIEXData->m_strSymbol);
	}

	total = gl_SystemData.GetTiingoCryptoSocketSize();
	for (auto i = 0; i < total; i++) {
		pCryptoData = gl_SystemData.PopTiingoCryptoSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketCrypto(pCryptoData->m_strSymbol);
	}

	total = gl_SystemData.GetTiingoForexSocketSize();
	for (auto i = 0; i < total; i++) {
		pForexData = gl_SystemData.PopTiingoForexSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketForex(pForexData->m_strSymbol);
	}

	total = gl_SystemData.GetFinnhubSocketSize();
	for (auto i = 0; i < total; i++) {
		pFinnhubData = gl_SystemData.PopFinnhubSocket();
		UpdateWorldStockFromFinnhubSocket(pFinnhubData);
		gl_systemMessage.SetCurrentFinnhubWebSocketStake(pFinnhubData->m_strSymbol);
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
			// do nothing
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