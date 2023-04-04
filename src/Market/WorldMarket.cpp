// ReSharper disable All
#include"pch.h"

#include "WorldMarket.h"
#include"thread.h"

#include"FinnhubInaccessibleExchange.h"

#include"FinnhubDataSource.h"
//#include"TiingoDataSource.h"

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoCryptoWebSocket.h"

#include"SetWorldStockDayLine.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXUserAgent.h>

#include<thread>
#include<memory>

#include "InfoReport.h"
#include "QuandlDataSource.h"
#include "TiingoDataSource.h"
#include "TimeConvert.h"
using std::thread;
using std::make_shared;

CWorldMarket::CWorldMarket() {
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE("CWorldMarket市场变量只允许存在一个实例\n");
	}

	// 无需（也无法）每日更新的变量放在这里
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // 由于证券代码总数有二十万之多，无法在一天之内更新完，故不再重置此索引。
	m_bFinnhubWebSiteAccessible = true;

	m_strMarketId = _T("美国市场");
	m_lMarketTimeZone = 4 * 3600; // 美国股市使用美东标准时间, GMT + 4
	CalculateTime();

	Reset();

	AddTask(CREATE_TASK__, 1);
}

CWorldMarket::~CWorldMarket() {
	PreparingExitMarket();
}

void CWorldMarket::Reset() {
	ResetFinnhub();
	ResetTiingo();
	ResetDataClass();
}

void CWorldMarket::ResetFinnhub() {
	SetSystemReady(false); // 市场初始状态为未设置好。
	if (IsEarlyThen(gl_finnhubInaccessibleExchange.GetUpdateDate(), GetMarketDate(), 7)) {
		gl_finnhubInaccessibleExchange.Clear(); // 不使用更新时间早于一周的数据。清除之，让系统自动查验新的状态。
		gl_finnhubInaccessibleExchange.SetUpdateDate(GetMarketDate());
	}
}

void CWorldMarket::ResetQuandl() { }

void CWorldMarket::ResetTiingo() { }

void CWorldMarket::ResetDataClass() {
	m_dataFinnhubStockExchange.Reset();
	m_dataFinnhubForexExchange.Reset();
	m_dataFinnhubCryptoExchange.Reset();
	m_dataFinnhubForexSymbol.Reset();
	m_dataFinnhubCryptoSymbol.Reset();
	m_dataFinnhubCountry.Reset();
	m_dataFinnhubEconomicCalendar.Reset();

	m_containerStock.Reset();

	m_dataTiingoStock.Reset();
	m_dataTiingoCryptoSymbol.Reset();

	m_containerChosenStock.Reset();
	m_containerChosenForex.Reset();
	m_containerChosenCrypto.Reset();
}

void CWorldMarket::ResetMarket() {
	Reset();

	UpdateToken();

	LoadWorldExchangeDB(); // 装入世界交易所信息
	LoadCountryDB();
	LoadStockDB();
	LoadWorldChosenStock();
	LoadForexExchange();
	LoadFinnhubForexSymbol();
	LoadWorldChosenForex();
	LoadCryptoExchange();
	LoadFinnhubCryptoSymbol();
	LoadWorldChosenCrypto();
	LoadEconomicCalendarDB();
	LoadTiingoStock();
	LoadTiingoCryptoSymbol();

	for (const auto& pDataSource : m_vDataSource) {
		pDataSource->Reset();
	}

	CString str = _T("重置World Market于美东标准时间：");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);
}

void CWorldMarket::PreparingExitMarket() {
	//ASSERT(gl_systemStatus.IsExitingSystem());
	DisconnectAllWebSocket();
}

bool CWorldMarket::ProcessTask(long lCurrentTime) {
	if (IsMarketTaskEmpty()) return false;
	const auto pTask = GetMarketTask();
	ASSERT(pTask->GetTime() < 240000);
	if (lCurrentTime >= pTask->GetTime()) {
		DiscardMarketTask();
		switch (pTask->GetType()) {
		case CREATE_TASK__: // 生成其他任务
			TaskCreateTask(lCurrentTime);
			break;
		case WORLD_MARKET_CHECK_SYSTEM_READY__: // 170000重启系统
			TaskCheckMarketReady(lCurrentTime);
			break;
		case WORLD_MARKET_RESET__: // 170000重启系统
			TaskResetMarket(lCurrentTime);
			break;
		case WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__:
			TaskUpdateStockProfileDB(lCurrentTime);
			break;
		case WORLD_MARKET_MONITORING_WEB_SOCKET_STATUS__:
			TaskMonitoringWebSocketStatus(lCurrentTime);
			break;
		case WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__:
			TaskProcessWebSocketData(lCurrentTime);
			break;
		default:
			ASSERT(0); // 非法任务
			break;
		}
		return true;
	}
	return false;
}

void CWorldMarket::TaskCreateTask(long lCurrentTime) {
	CMarketTaskPtr pTask;
	const long lTimeMinute = (lCurrentTime / 100) * 100; // 当前小时和分钟

	while (!IsMarketTaskEmpty()) DiscardMarketTask();
	SetResetMarketPermission(false);

	// 系统初始化检查
	AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, 1);

	// 重置系统
	if (lCurrentTime < 170000) {
		AddTask(WORLD_MARKET_RESET__, 170000); // 执行时间为：170000
	}

	AddTask(WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__, lTimeMinute + 40);// 更新股票简介数据库的任务

	AddTask(WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__, lCurrentTime);
	AddTask(WORLD_MARKET_MONITORING_WEB_SOCKET_STATUS__, lCurrentTime);
}

void CWorldMarket::TaskProcessWebSocketData(long lCurrentTime) {
	if (!IsTimeToResetSystem(lCurrentTime)) {	// 下午五时重启系统，各数据库需要重新装入，故而此时不允许更新。
		ProcessWebSocketData();
		UpdateWorldStockFromWebSocket();
	}
	AddTask(WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__, GetNextSecond(lCurrentTime));
}

void CWorldMarket::TaskMonitoringWebSocketStatus(long lCurrentTime) {
	StartAllWebSocket();
	AddTask(WORLD_MARKET_MONITORING_WEB_SOCKET_STATUS__, GetNextTime(lCurrentTime, 0, 1, 0));
}

void CWorldMarket::TaskResetMarket(long lCurrentTime) {
	// 市场时间十七时重启系统
	SetResetMarket(true); // 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
	SetSystemReady(false);

	AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, lCurrentTime); // 每次重置系统时，必须设置系统状态检查任务
}

bool CWorldMarket::CreateThreadUpdateTiingoIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) { // 更新tiingo stock profile与finnhub更新stock profile互斥
		thread thread1(ThreadUpdateTiingoIndustry, this);
		thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
		return true;
	}
	return false;
}

bool CWorldMarket::CreateThreadUpdateSICIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// 更新tiingo stock profile与finnhub更新stock profile互斥
		thread thread1(ThreadUpdateSICIndustry, this);
		thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
		return true;
	}
	return false;
}

bool CWorldMarket::CreateThreadUpdateNaicsIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// 更新tiingo stock profile与finnhub更新stock profile互斥
		thread thread1(ThreadUpdateNaicsIndustry, this);
		thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
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
bool CWorldMarket::UpdateForexDayLineDB() {
	CString str;
	bool fUpdated = false;
	CForexSymbolPtr pSymbol = nullptr;
	size_t symbolSize = m_dataFinnhubForexSymbol.Size();

	for (int i = 0; i < symbolSize; i++) {
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
		pSymbol = m_dataFinnhubForexSymbol.GetSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) {	// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					thread thread1(ThreadUpdateForexDayLineDB, pSymbol.get());
					thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
					fUpdated = true;
					TRACE("更新%s日线数据\n", pSymbol->GetSymbol().GetBuffer());
				}
				else pSymbol->UnloadDayLine(); // 当无需执行存储函数时，这里还要单独卸载日线数据。因存储日线数据线程稍后才执行，故而不能在此统一执行删除函数。
			}
			else { // 此种情况为有股票代码，但此代码尚未上市
				pSymbol->SetIPOStatus(_STOCK_NOT_YET_LIST_);
				CString str1 = pSymbol->GetSymbol();
				str1 += _T(" 为未上市股票代码");
				gl_systemMessage.PushDayLineInfoMessage(str1);
			}
		}
	}

	return (fUpdated);
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
bool CWorldMarket::UpdateCryptoDayLineDB() {
	CString str;
	bool fUpdated = false;
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;
	size_t symbolSize = m_dataFinnhubCryptoSymbol.Size();

	for (int i = 0; i < symbolSize; ++i) {
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
		pSymbol = m_dataFinnhubCryptoSymbol.GetSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) {	// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					thread thread1(ThreadUpdateCryptoDayLineDB, pSymbol.get());
					thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
					fUpdated = true;
					//TRACE("更新%s日线数据\n", pSymbol->GetSymbol().GetBuffer());
				}
				else pSymbol->UnloadDayLine(); // 当无需执行存储函数时，这里还要单独卸载日线数据。因存储日线数据线程稍后才执行，故而不能在此统一执行删除函数。
			}
			else {// 此种情况为有股票代码，但此代码尚未上市；或者是已退市股票
				if (pSymbol->GetDayLineEndDate() > 19800101) {// 已退市
					pSymbol->SetIPOStatus(_STOCK_DELISTED_);
					pSymbol->SetUpdateProfileDB(true);
				}
				else {// 此种情况为有股票代码，但此代码尚未上市
					pSymbol->SetIPOStatus(_STOCK_NOT_YET_LIST_);
					CString str1 = pSymbol->GetSymbol();
					str1 += _T(" 为未上市股票代码");
					gl_systemMessage.PushDayLineInfoMessage(str1);
				}
			}
		}
	}

	return (fUpdated);
}

bool CWorldMarket::UpdateEPSSurpriseDB() {
	CString str;
	size_t stockSize = m_containerStock.Size();

	CWorldStockPtr pStock = nullptr;
	for (long l = 0; l < stockSize; ++l) {
		pStock = m_containerStock.GetStock(l);
		if (pStock->IsEPSSurpriseNeedSaveAndClearFlag()) {// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			thread thread1(ThreadUpdateEPSSurpriseDB, pStock.get());
			thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
			TRACE("更新%s EPS surprise数据\n", pStock->GetSymbol().GetBuffer());
		}
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}

	return (true);
}

bool CWorldMarket::TaskCheckMarketReady(long lCurrentTime) {
	if (!IsSystemReady()) {
		if (!gl_pFinnhubDataSource->IsUpdateSymbol() && !gl_pFinnhubDataSource->IsUpdateForexExchange() && !gl_pFinnhubDataSource->IsUpdateForexSymbol()
			&& !gl_pFinnhubDataSource->IsUpdateCryptoExchange() && !gl_pFinnhubDataSource->IsUpdateCryptoSymbol()) {
			const CString str = "世界市场初始化完毕";
			gl_systemMessage.PushInformationMessage(str);
			SetSystemReady(true);
		}
	}
	//SetSystemReady(true);
	if (!IsSystemReady()) {
		AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, GetNextSecond(lCurrentTime));
	}
	return IsSystemReady();
}

void CWorldMarket::CreateThreadUpdateDayLineStartEndDate() {
	thread thread1(ThreadUpdateWorldStockDayLineStartEndDate, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateDayLineDB() {
	thread thread1(ThreadUpdateWorldStockDayLineDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::TaskUpdateStockProfileDB(long lCurrentTime) {
	if (!IsTimeToResetSystem(lCurrentTime)) {	// 下午五时重启系统，各数据库需要重新装入，故而此时不允许更新数据库。
		if (m_dataFinnhubCountry.GetLastTotalCountry() < m_dataFinnhubCountry.GetTotalCountry()) {
			CreateThreadUpdateCountryListDB();
		}
		if (IsUpdateForexExchangeDB()) CreateThreadUpdateForexExchangeDB();
		if (IsUpdateForexSymbolDB()) CreateThreadUpdateForexSymbolDB();
		if (IsUpdateCryptoExchangeDB()) CreateThreadUpdateCryptoExchangeDB();
		if (IsUpdateCryptoSymbolDB()) CreateThreadUpdateFinnhubCryptoSymbolDB();
		if (IsUpdateInsiderTransactionDB()) CreateThreadUpdateInsiderTransactionDB();
		if (IsUpdateInsiderSentimentDB()) CreateThreadUpdateInsiderSentimentDB();
		if (IsSaveStockDayLineDB()) CreateThreadUpdateDayLineDB();
		if (IsUpdateEconomicCalendarDB()) CreateThreadUpdateEconomicCalendarDB();
		if (IsUpdateCompanyNewsDB()) CreateThreadUpdateCompanyNewsDB();
		if (IsUpdateBasicFinancialDB()) CreateThreadUpdateBasicFinancialDB();
		if (IsNeedUpdateTiingoStock()) CreateThreadUpdateTiingoStockDB();
		if (IsNeedUpdateTiingoCryptoSymbol()) CreateThreadUpdateTiingoCryptoSymbolDB();

		UpdateForexDayLineDB();
		UpdateCryptoDayLineDB();
		UpdateEPSSurpriseDB();

		if (!gl_pFinnhubDataSource->IsUpdateSymbol() && IsUpdateStockProfileDB()) {
			CreateThreadUpdateStockProfileDB();
		}
	}
	AddTask(WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__, GetNextTime(lCurrentTime, 0, 5, 0)); // 每五分钟更新一次
}

void CWorldMarket::CreateThreadUpdateStockProfileDB() {
	thread thread1(ThreadUpdateWorldMarketStockProfileDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateCompanyNewsDB() {
	thread thread1(ThreadUpdateCompanyNewsDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateBasicFinancialDB() {
	thread thread1(ThreadUpdateBasicFinancialDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateCountryListDB() {
	thread thread1(ThreadUpdateCountryListDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateInsiderTransactionDB() {
	thread thread1(ThreadUpdateInsiderTransactionDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateInsiderSentimentDB() {
	thread thread1(ThreadUpdateInsiderSentimentDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateTiingoStockDB() {
	thread thread1(ThreadUpdateTiingoStockDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateTiingoCryptoSymbolDB() {
	thread thread1(ThreadUpdateTiingoCryptoSymbolDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateForexExchangeDB() {
	thread thread1(ThreadUpdateForexExchangeDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateForexSymbolDB() {
	thread thread1(ThreadUpdateForexSymbolDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateCryptoExchangeDB() {
	thread thread1(ThreadUpdateCryptoExchangeDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateFinnhubCryptoSymbolDB() {
	thread thread1(ThreadUpdateFinnhubCryptoSymbolDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateEconomicCalendarDB() {
	thread thread1(ThreadUpdateEconomicCalendarDB, this);
	thread1.detach();
}

bool CWorldMarket::UpdateToken() {
	ASSERT(gl_systemConfiguration.IsInitialized());

	if (gl_systemConfiguration.GetFinnhubToken().GetLength() > 5) {
		gl_pFinnhubDataSource->SetInquiryToken(gl_systemConfiguration.GetFinnhubToken());
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Finnhub Token Needed"));
	}
	if (gl_systemConfiguration.GetTiingoToken().GetLength() > 5) {
		gl_pTiingoDataSource->SetInquiryToken(gl_systemConfiguration.GetTiingoToken());
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Tiingo Token Needed"));
	}
	if (gl_systemConfiguration.GetQuandlToken().GetLength() > 5) {
		gl_pQuandlDataSource->SetInquiryToken(gl_systemConfiguration.GetQuandlToken());
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Quandl Token Needed"));
	}

	return true;
}

bool CWorldMarket::UpdateStockDayLineDB() {
	for (long i = 0; i < GetStockSize(); i++) {
		const CWorldStockPtr pStock = GetStock(i);
		pStock->UpdateDayLineDB();
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}
	return true;
}

bool CWorldMarket::UpdateCompanyNewsDB() {
	for (long l = 0; l < m_containerStock.Size(); l++) {
		const auto pStock = m_containerStock.GetStock(l);
		if (pStock->IsUpdateCompanyNewsDBAndClearFlag()) {// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			pStock->UpdateCompanyNewsDB();
		}
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}

	return (true);
}

bool CWorldMarket::UpdateInsiderTransactionDB() {
	for (long i = 0; i < GetStockSize(); i++) {
		const CWorldStockPtr pStock = GetStock(i);
		if (pStock->IsSaveInsiderTransactionAndClearFlag()) {
			if (pStock->HaveInsiderTransaction()) {
				pStock->SaveInsiderTransaction();
				const CString str = pStock->GetSymbol() + _T("内部交易资料更新完成");
				gl_systemMessage.PushDayLineInfoMessage(str);
				//TRACE("更新%s内部交易数据\n", pStock->GetSymbol().GetBuffer());
			}
		}
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}
	return true;
}

bool CWorldMarket::UpdateInsiderSentimentDB() {
	for (long i = 0; i < GetStockSize(); i++) {
		const CWorldStockPtr pStock = GetStock(i);
		if (pStock->IsSaveInsiderSentimentAndClearFlag()) {
			if (pStock->HaveInsiderSentiment()) {
				pStock->SaveInsiderSentiment();
				const CString str = pStock->GetSymbol() + _T("内部交易情绪资料更新完成");
				gl_systemMessage.PushDayLineInfoMessage(str);
			}
		}
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}
	return true;
}

bool CWorldMarket::UpdateTiingoIndustry() {
	// not implemented
	return false;
}

bool CWorldMarket::UpdateSICIndustry() {
	// not implemented
	return false;
}

bool CWorldMarket::UpdateNaicsIndustry() {
	// not implemented
	return false;
}

void CWorldMarket::RebuildEPSSurprise() {
	m_containerStock.ResetEPSSurprise();
	gl_pFinnhubDataSource->SetUpdateEPSSurprise(true);
}

void CWorldMarket::RebuildPeer() {
	m_containerStock.ResetPeer();
	gl_pFinnhubDataSource->SetUpdatePeer(true);
}

void CWorldMarket::RebuildBasicFinancial() {
	m_containerStock.ResetBasicFinancial();
	gl_pFinnhubDataSource->SetUpdateStockBasicFinancial(true);
}

void CWorldMarket::RebuildStockDayLineDB() {
	m_containerStock.ResetDayLine();
	gl_pFinnhubDataSource->SetUpdateStockProfile(true);
}

void CWorldMarket::UpdateStockDayLineStartEndDate() {
	try {
		const CString strFilterPrefix = _T("[Symbol] = '");
		CSetWorldStockDayLine setWorldStockDayLine;

		for (long l = 0; l < m_containerStock.Size(); l++) {
			const auto pStock = m_containerStock.GetStock(l);
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
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

/// <summary>
/// Finnhub WebSocket的免费账户最多只能发送50个证券的数据
/// </summary>
vectorString CWorldMarket::GetFinnhubWebSocketSymbolVector() {
	vectorString vSymbol;

	for (long l = 0; l < m_containerChosenStock.Size(); l++) {
		const CWorldStockPtr pStock = m_containerChosenStock.GetStock(l);
		vSymbol.push_back(pStock->GetSymbol().GetBuffer());
	}

	for (long l = 0; l < m_containerChosenCrypto.Size(); l++) {
		const CFinnhubCryptoSymbolPtr pCrypto = m_containerChosenCrypto.GetCryptoSymbol(l);
		vSymbol.push_back(pCrypto->GetSymbol().GetBuffer());
	}

	for (long l = 0; l < m_containerChosenForex.Size(); l++) {
		const CForexSymbolPtr pForex = m_containerChosenForex.GetForexSymbol(l);
		vSymbol.push_back(pForex->GetSymbol().GetBuffer());
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
vectorString CWorldMarket::GetTiingoIEXWebSocketSymbolVector() {
	vectorString vSymbol;
	for (long l = 0; l < m_containerChosenStock.Size(); l++) {
		const CWorldStockPtr pStock = m_containerChosenStock.GetStock(l);
		vSymbol.push_back(pStock->GetSymbol().GetBuffer());
	}

	return vSymbol;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 接收所有的Crypto数据时，每秒数据量大致在50-100K附近。
//
// thresholdLevel 2: Top-of-Book AND Last Trade updates.
// thresholdLevel 5: only Last Trade updates.
//
//////////////////////////////////////////////////////////////////////////////////////////////
vectorString CWorldMarket::GetTiingoCryptoWebSocketSymbolVector() {
	vectorString vSymbol;
	for (long l = 0; l < m_containerChosenCrypto.Size(); l++) {
		const CFinnhubCryptoSymbolPtr pCrypto = m_containerChosenCrypto.GetCryptoSymbol(l);
		vSymbol.push_back(pCrypto->GetSymbol().GetBuffer());
	}

	return vSymbol;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 5状态下每秒接收100K左右。
//
// thresholdLevel 5: ALL Top-of-Book updates
// thresholdLevel 7: A top-of-book update that is due to a change in either the bid/ask price or size.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
vectorString CWorldMarket::GetTiingoForexWebSocketSymbolVector() {
	vectorString vSymbol;
	for (long l = 0; l < m_containerChosenForex.Size(); l++) {
		const CForexSymbolPtr pForex = m_containerChosenForex.GetForexSymbol(l);
		vSymbol.push_back(pForex->GetSymbol().GetBuffer());
	}

	return vSymbol;
}

void CWorldMarket::StartAllWebSocket() {
	if (IsSystemReady()) {
		if (!gl_pFinnhubDataSource->IsWebError() && gl_finnhubWebSocket.IsIdle()) StartFinnhubWebSocket();
		if (!gl_pTiingoDataSource->IsWebError()) {
			if (gl_tiingoIEXWebSocket.IsIdle()) StartTiingoIEXWebSocket();
			if (gl_tiingoCryptoWebSocket.IsIdle()) StartTiingoCryptoWebSocket();
			if (gl_tiingoForexWebSocket.IsIdle()) StartTiingoForexWebSocket();
		}
	}
}

void CWorldMarket::StartFinnhubWebSocket() {
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket() && !gl_pFinnhubDataSource->IsTimeout()) {
		gl_finnhubWebSocket.CreateThreadConnectWebSocketAndSendMessage(GetFinnhubWebSocketSymbolVector());
		gl_finnhubWebSocket.SetHeartbeatTime(GetUTCTime());
	}
}

void CWorldMarket::StartTiingoIEXWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		gl_tiingoIEXWebSocket.CreateThreadConnectWebSocketAndSendMessage(GetTiingoIEXWebSocketSymbolVector());
		gl_tiingoIEXWebSocket.SetHeartbeatTime(GetUTCTime());
	}
}

void CWorldMarket::StartTiingoCryptoWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		gl_tiingoCryptoWebSocket.CreateThreadConnectWebSocketAndSendMessage(GetTiingoCryptoWebSocketSymbolVector());
		gl_tiingoCryptoWebSocket.SetHeartbeatTime(GetUTCTime());
	}
}

void CWorldMarket::StartTiingoForexWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		gl_tiingoForexWebSocket.CreateThreadConnectWebSocketAndSendMessage(GetTiingoForexWebSocketSymbolVector());
		gl_tiingoForexWebSocket.SetHeartbeatTime(GetUTCTime());
	}
}

/// <summary>
/// // 停止WebSocket。此函数等待其停止后方返回。是系统退出前的准备工作。
/// </summary>
void CWorldMarket::DisconnectAllWebSocket() {
	//本函数只在系统退出时调用
	//ASSERT(gl_systemStatus.IsExitingSystem()); 
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket()) gl_finnhubWebSocket.Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket()) gl_tiingoIEXWebSocket.Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket()) gl_tiingoCryptoWebSocket.Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket()) gl_tiingoForexWebSocket.Disconnect();
}

/// <summary>
/// 停止WebSocket。此函数是生成工作线程来停止WebSocket，不用等待其停止即返回。用于系统运行中的停止动作。
/// </summary>
void CWorldMarket::StopWebSocketsIfTimeOut() {
	if (IsSystemReady()) {
		StopFinnhubWebSocketIfTimeOut();
		StopTiingoIEXWebSocketIfTimeOut();
		StopTiingoCryptoWebSocketIfTimeOut();
		StopTiingoForexWebSocketIfTimeOut();
	}
}

void CWorldMarket::StopFinnhubWebSocketIfTimeOut() {
	if ((gl_pFinnhubDataSource->IsTimeout()) || gl_finnhubWebSocket.IsIdle()) {
		gl_finnhubWebSocket.CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Finnhub web故障#12002，关闭Web socket服务"));
	}
}

void CWorldMarket::StopTiingoIEXWebSocketIfTimeOut() {
	if ((gl_pTiingoDataSource->IsTimeout()) && !gl_tiingoIEXWebSocket.IsClosed()) {
		gl_tiingoIEXWebSocket.CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX web故障#12002，关闭Web socket服务"));
	}
}

void CWorldMarket::StopTiingoCryptoWebSocketIfTimeOut() {
	if ((gl_pTiingoDataSource->IsTimeout()) && !gl_tiingoCryptoWebSocket.IsClosed()) {
		gl_tiingoCryptoWebSocket.CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto web故障#12002，关闭Web socket服务"));
	}
}

void CWorldMarket::StopTiingoForexWebSocketIfTimeOut() {
	if ((gl_pTiingoDataSource->IsTimeout()) && !gl_tiingoForexWebSocket.IsClosed()) {
		gl_tiingoForexWebSocket.CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Forex web故障#12002，关闭Web socket服务"));
	}
}

void CWorldMarket::ProcessWebSocketData() {
	if (gl_finnhubWebSocket.DataSize() > 0) ProcessFinnhubWebSocketData();
	if (gl_tiingoIEXWebSocket.DataSize() > 0) ProcessTiingoIEXWebSocketData();
	if (gl_tiingoCryptoWebSocket.DataSize() > 0) ProcessTiingoCryptoWebSocketData();
	if (gl_tiingoForexWebSocket.DataSize() > 0) ProcessTiingoForexWebSocketData();
}

void CWorldMarket::ProcessFinnhubWebSocketData() {
	const auto total = gl_finnhubWebSocket.DataSize();
	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		auto pString = gl_finnhubWebSocket.PopData();
		CString strMessage = _T("Finnhub: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_finnhubWebSocket.ParseFinnhubWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedFinnhubWebSocket(iTotalDataSize);
}

void CWorldMarket::ProcessTiingoIEXWebSocketData() {
	const auto total = gl_tiingoIEXWebSocket.DataSize();
	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		auto pString = gl_tiingoIEXWebSocket.PopData();
		CString strMessage = _T("Tiingo IEX: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_tiingoIEXWebSocket.ParseTiingoIEXWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoIEXWebSocket(iTotalDataSize);
}

void CWorldMarket::ProcessTiingoCryptoWebSocketData() {
	const auto total = gl_tiingoCryptoWebSocket.DataSize();

	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		auto pString = gl_tiingoCryptoWebSocket.PopData();
		CString strMessage = _T("Tiingo Crypto: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_tiingoCryptoWebSocket.ParseTiingoCryptoWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoCryptoWebSocket(iTotalDataSize);
}

void CWorldMarket::ProcessTiingoForexWebSocketData() {
	const auto total = gl_tiingoForexWebSocket.DataSize();
	size_t iTotalDataSize = 0;
	for (size_t i = 0; i < total; i++) {
		auto pString = gl_tiingoForexWebSocket.PopData();
		CString strMessage = _T("Tiingo Forex: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_tiingoForexWebSocket.ParseTiingoForexWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoForexWebSocket(iTotalDataSize);
}

/// <summary>
/// 更新证券的实时信息
/// 目前只使用IEX和Crypto的数据，其他数据抛弃掉
/// </summary>
/// <returns></returns>
void CWorldMarket::UpdateWorldStockFromWebSocket() {
	auto total = gl_SystemData.GetTiingoIEXSocketSize();
	for (auto i = 0; i < total; i++) {
		const CTiingoIEXSocketPtr pIEXData = gl_SystemData.PopTiingoIEXSocket();
		UpdateWorldStockFromTiingoIEXSocket(pIEXData);
		gl_systemMessage.SetCurrentTiingoWebSocketIEX(pIEXData->m_sSymbol.c_str());
	}

	total = gl_SystemData.GetTiingoCryptoSocketSize();
	for (auto i = 0; i < total; i++) {
		const CTiingoCryptoSocketPtr pCryptoData = gl_SystemData.PopTiingoCryptoSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketCrypto(pCryptoData->m_sSymbol.c_str());
	}

	total = gl_SystemData.GetTiingoForexSocketSize();
	for (auto i = 0; i < total; i++) {
		const CTiingoForexSocketPtr pForexData = gl_SystemData.PopTiingoForexSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketForex(pForexData->m_sSymbol.c_str());
	}

	total = gl_SystemData.GetFinnhubSocketSize();
	for (auto i = 0; i < total; i++) {
		const CFinnhubSocketPtr pFinnhubData = gl_SystemData.PopFinnhubSocket();
		UpdateWorldStockFromFinnhubSocket(pFinnhubData);
		gl_systemMessage.SetCurrentFinnhubWebSocketStake(pFinnhubData->m_sSymbol.c_str());
	}
}

void CWorldMarket::UpdateWorldStockFromTiingoIEXSocket(CTiingoIEXSocketPtr pTiingoIEXbData) {
	if (IsStock(pTiingoIEXbData->m_sSymbol.c_str())) {
		const CWorldStockPtr pStock = GetStock(pTiingoIEXbData->m_sSymbol.c_str());
		pStock->SetActive(true);
		switch (pTiingoIEXbData->m_chMessageType) {
		case 'T':
			pStock->SetNew(pTiingoIEXbData->m_dLastPrice * 1000);
			break;
		case 'Q':
			// do nothing
			break;
		default:
			// do nothing
			break;
		}
	}
}

void CWorldMarket::UpdateWorldStockFromFinnhubSocket(CFinnhubSocketPtr pFinnhubData) {
	if (IsStock(pFinnhubData->m_sSymbol.c_str())) {
		const CWorldStockPtr pStock = GetStock(pFinnhubData->m_sSymbol.c_str());
		pStock->SetActive(true);
		pStock->SetNew(pFinnhubData->m_dLastPrice * 1000);
	}
}
