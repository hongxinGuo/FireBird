#include"pch.h"

#include"systemData.h"

#include "WorldMarket.h"
#include"thread.h"

#include"FinnhubInaccessibleExchange.h"

#include"FinnhubDataSource.h"

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoCryptoWebSocket.h"

#include"SetWorldStockDayLine.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXUserAgent.h>

#include "InfoReport.h"
#include "QuandlDataSource.h"
#include "TiingoDataSource.h"
#include "TimeConvert.h"
using std::thread;
using std::make_shared;

CWorldMarket::CWorldMarket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
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

	m_pvMarketStatus = make_shared<vector<CMarketStatusPtr>>();
	m_pvMarketHoliday = make_shared<vector<CMarketHolidayPtr>>();

	Reset();

	AddTask(CREATE_TASK__, 1);
}

CWorldMarket::~CWorldMarket() {
	PreparingExitMarket();
}

void CWorldMarket::Reset() {
	ResetFinnhub();
	ResetTiingo();
	ResetDataContainer();
}

void CWorldMarket::ResetFinnhub() {
	SetSystemReady(false); // 市场初始状态为未设置好。
	if (IsEarlyThen(gl_finnhubInaccessibleExchange.GetUpdateDate(), GetMarketDate(), 7)) {
		gl_finnhubInaccessibleExchange.Clear(); // 不使用更新时间早于一周的数据。清除之，让系统自动查验新的状态。
		gl_finnhubInaccessibleExchange.SetUpdateDate(GetMarketDate());
	}
	m_pvMarketStatus->clear();
	m_pvMarketHoliday->clear();
}

void CWorldMarket::ResetQuandl() {}

void CWorldMarket::ResetTiingo() {}

void CWorldMarket::ResetDataContainer() {
	gl_dataContainerFinnhubStockExchange.Reset();
	gl_dataContainerFinnhubForexExchange.Reset();
	gl_dataContainerFinnhubCryptoExchange.Reset();
	gl_dataFinnhubForexSymbol.Reset();
	gl_dataFinnhubCryptoSymbol.Reset();
	gl_dataContainerFinnhubCountry.Reset();
	gl_dataContainerFinnhubEconomicCalendar.Reset();

	gl_dataContainerFinnhubStock.Reset();

	gl_dataContainerTiingoStock.Reset();
	gl_dataContainerTiingoCryptoSymbol.Reset();

	gl_dataContainerChosenWorldStock.Reset();
	gl_dataContainerChosenWorldForex.Reset();
	gl_dataContainerChosenWorldCrypto.Reset();
}

void CWorldMarket::ResetMarket() {
	gl_bWorldMarketResetting = true;
	Reset();

	UpdateToken();

	gl_dataContainerFinnhubStockExchange.LoadDB(); // 装入世界交易所信息
	gl_dataContainerFinnhubCountry.LoadDB();
	gl_dataContainerFinnhubStock.LoadDB();
	gl_dataContainerChosenWorldStock.LoadDB();
	gl_dataContainerFinnhubForexExchange.LoadDB();
	gl_dataFinnhubForexSymbol.LoadDB();
	gl_dataContainerChosenWorldForex.LoadDB();
	gl_dataContainerFinnhubCryptoExchange.LoadDB();
	gl_dataFinnhubCryptoSymbol.LoadDB();
	gl_dataContainerChosenWorldCrypto.LoadDB();
	gl_dataContainerFinnhubEconomicCalendar.LoadDB();

	gl_dataContainerTiingoStock.LoadDB();
	gl_dataContainerTiingoCryptoSymbol.LoadDB();

	for (const auto& pDataSource : m_vDataSource) {
		pDataSource->Reset();
	}

	CString str = _T("重置World Market于美东标准时间：");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);

	gl_bWorldMarketResetting = false;
}

void CWorldMarket::PreparingExitMarket() {
	//ASSERT(gl_systemConfiguration.IsExitingSystem());
	DisconnectAllWebSocket();
}

bool CWorldMarket::ProcessTask(long lCurrentTime) {
	if (IsMarketTaskEmpty()) return false;
	const auto pTask = GetMarketTask();
	if (lCurrentTime >= pTask->GetTime()) {
		DiscardCurrentMarketTask();
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
		case WORLD_MARKET_START_ALL_WEB_SOCKET__:
			TaskStartAllWebSocket(lCurrentTime);
			break;
		case WORLD_MARKET_STOP_ALL_WEB_SOCKET__:
			TaskStopAllWebSocket(lCurrentTime);
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
	const long lTimeMinute = (lCurrentTime / 100) * 100; // 当前小时和分钟

	while (!IsMarketTaskEmpty()) DiscardCurrentMarketTask();

	// 系统初始化检查
	AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, 1);

	// 市场重置
	if (lCurrentTime < 170000) {
		AddTask(WORLD_MARKET_RESET__, 170000); // 执行时间为：170000
	}

	AddTask(WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__, lTimeMinute + 40);// 更新股票简介数据库的任务

	AddTask(WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__, lCurrentTime);

	AddTask(WORLD_MARKET_STOP_ALL_WEB_SOCKET__, GetNextTime(lTimeMinute + 30, 0, 1, 0)); // 停止WebSocket任务要先于启动WebSocket任务
	AddTask(WORLD_MARKET_START_ALL_WEB_SOCKET__, GetNextTime(lTimeMinute + 60, 0, 1, 0));

	AddTask(CREATE_TASK__, 240000); // 重启市场任务的任务于每日零时执行
}

void CWorldMarket::TaskProcessWebSocketData(long lCurrentTime) {
	ASSERT(!IsTimeToResetSystem(lCurrentTime));	// 下午五时重启系统，各数据库需要重新装入，故而此时不允许更新。

	ProcessWebSocketData();
	UpdateWorldStockFromWebSocket();

	long lNextTime = GetNextSecond(lCurrentTime);
	if (IsTimeToResetSystem(lNextTime)) lNextTime = 170501;
	AddTask(WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__, lNextTime);
}

void CWorldMarket::TaskStartAllWebSocket(long lCurrentTime) {
	StartAllWebSocket();
	AddTask(WORLD_MARKET_START_ALL_WEB_SOCKET__, GetNextTime(lCurrentTime, 0, 1, 0));
}

void CWorldMarket::TaskStopAllWebSocket(long lCurrentTime) {
	StopAllWebSocketIfTimeOut();
	AddTask(WORLD_MARKET_STOP_ALL_WEB_SOCKET__, GetNextTime(lCurrentTime, 0, 1, 0));
}

void CWorldMarket::TaskResetMarket(long lCurrentTime) {
	// 市场时间十七时重启系统
	SetResetMarket(true); // 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
	SetSystemReady(false);

	AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, lCurrentTime); // 每次重置系统时，必须设置系统状态检查任务
}

bool CWorldMarket::CreateThreadUpdateTiingoIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) { // 更新tiingo stock profile与finnhub更新stock profile互斥
		thread thread1(ThreadUpdateTiingoIndustry, gl_pWorldMarket);
		thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
		return true;
	}
	return false;
}

bool CWorldMarket::CreateThreadUpdateSICIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// 更新tiingo stock profile与finnhub更新stock profile互斥
		thread thread1(ThreadUpdateSICIndustry, gl_pWorldMarket);
		thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
		return true;
	}
	return false;
}

bool CWorldMarket::CreateThreadUpdateNaicsIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// 更新tiingo stock profile与finnhub更新stock profile互斥
		thread thread1(ThreadUpdateNaicsIndustry, gl_pWorldMarket);
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
	const size_t symbolSize = gl_dataFinnhubForexSymbol.Size();

	for (int i = 0; i < symbolSize; i++) {
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
		pSymbol = gl_dataFinnhubForexSymbol.GetSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) {	// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					CreateThreadUpdateForexDayLineDB(pSymbol);
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
	const size_t symbolSize = gl_dataFinnhubCryptoSymbol.Size();

	for (int i = 0; i < symbolSize; ++i) {
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
		pSymbol = gl_dataFinnhubCryptoSymbol.GetSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) {	// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					CreateThreadUpdateCryptoDayLineDB(pSymbol);
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
	const size_t stockSize = gl_dataContainerFinnhubStock.Size();

	CWorldStockPtr pStock = nullptr;
	for (long l = 0; l < stockSize; ++l) {
		pStock = gl_dataContainerFinnhubStock.GetStock(l);
		if (pStock->IsEPSSurpriseNeedSaveAndClearFlag()) {// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			//CreateThreadUpdateEPSSurpriseDB(pStock);
			pStock->UpdateEPSSurpriseDB();
			TRACE("更新%s EPS surprise数据\n", pStock->GetSymbol().GetBuffer());
		}
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}

	return (true);
}

void CWorldMarket::CreateThreadUpdateForexDayLineDB(CForexSymbolPtr pSymbol) {
	thread thread1(ThreadUpdateForexDayLineDB, pSymbol);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateCryptoDayLineDB(CFinnhubCryptoSymbolPtr pSymbol) {
	thread thread1(ThreadUpdateCryptoDayLineDB, pSymbol);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateEPSSurpriseDB(CWorldStockPtr pStock) {
	thread thread1(ThreadUpdateEPSSurpriseDB, pStock);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateEPSSurpriseDB2() {
	thread thread1(ThreadUpdateEPSSurpriseDB2, gl_pWorldMarket);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
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
	thread thread1(ThreadUpdateWorldStockDayLineStartEndDate, gl_pWorldMarket);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateDayLineDB() {
	thread thread1(ThreadUpdateWorldStockDayLineDB, gl_pWorldMarket);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::TaskUpdateStockProfileDB(long lCurrentTime) {
	if (gl_dataContainerFinnhubCountry.GetLastTotalCountry() < gl_dataContainerFinnhubCountry.GetTotalCountry()) {
		CreateThreadUpdateCountryListDB();
	}
	if (gl_dataContainerFinnhubForexExchange.IsNeedUpdate()) CreateThreadUpdateForexExchangeDB();
	if (gl_dataFinnhubForexSymbol.IsUpdateProfileDB()) CreateThreadUpdateForexSymbolDB();
	if (gl_dataContainerFinnhubCryptoExchange.IsNeedUpdate()) CreateThreadUpdateCryptoExchangeDB();
	if (gl_dataFinnhubCryptoSymbol.IsUpdateProfileDB()) CreateThreadUpdateFinnhubCryptoSymbolDB();
	if (gl_dataContainerFinnhubStock.IsSaveInsiderTransaction()) CreateThreadUpdateInsiderTransactionDB();
	if (gl_dataContainerFinnhubStock.IsSaveInsiderSentiment()) CreateThreadUpdateInsiderSentimentDB();
	if (gl_dataContainerFinnhubStock.IsDayLineNeedSaving()) CreateThreadUpdateDayLineDB();
	if (gl_dataContainerFinnhubEconomicCalendar.IsNeedUpdate()) CreateThreadUpdateEconomicCalendarDB();
	if (gl_dataContainerFinnhubStock.IsUpdateCompanyNewsDB()) CreateThreadUpdateCompanyNewsDB();
	if (gl_dataContainerFinnhubStock.IsUpdateBasicFinancialDB()) CreateThreadUpdateBasicFinancialDB();
	if (gl_dataContainerTiingoStock.IsNeedUpdate()) CreateThreadUpdateTiingoStockDB();
	if (gl_dataContainerTiingoCryptoSymbol.IsNeedUpdate()) CreateThreadUpdateTiingoCryptoSymbolDB();
	if (gl_dataContainerFinnhubStock.IsSaveEPSSurpriseDB()) CreateThreadUpdateEPSSurpriseDB2();

	UpdateForexDayLineDB();
	UpdateCryptoDayLineDB();

	if (!gl_pFinnhubDataSource->IsUpdateSymbol() && gl_dataContainerFinnhubStock.IsUpdateProfileDB()) {
		CreateThreadUpdateStockProfileDB();
	}

	if (gl_finnhubInaccessibleExchange.IsNeedUpdate()) { // 更新禁止访问证券交易所名单
		gl_finnhubInaccessibleExchange.UpdateDB();
		gl_finnhubInaccessibleExchange.SetUpdate(false);
	}

	long lNextTime = GetNextTime(lCurrentTime, 0, 5, 0);
	if (IsTimeToResetSystem(lNextTime)) lNextTime = 170510;
	ASSERT(!IsTimeToResetSystem(lNextTime));// 下午五时重启系统，各数据库需要重新装入，故而此时不允许更新数据库。
	AddTask(WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__, lNextTime); // 每五分钟更新一次
}

void CWorldMarket::CreateThreadUpdateStockProfileDB() {
	thread thread1(ThreadUpdateWorldMarketStockProfileDB);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateCompanyNewsDB() {
	thread thread1(ThreadUpdateCompanyNewsDB, gl_pWorldMarket);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateBasicFinancialDB() {
	thread thread1(ThreadUpdateBasicFinancialDB);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateCountryListDB() {
	thread thread1(ThreadUpdateCountryListDB);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateInsiderTransactionDB() {
	thread thread1(ThreadUpdateInsiderTransactionDB, gl_pWorldMarket);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateInsiderSentimentDB() {
	thread thread1(ThreadUpdateInsiderSentimentDB, gl_pWorldMarket);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateTiingoStockDB() {
	thread thread1(ThreadUpdateTiingoStockDB);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateTiingoCryptoSymbolDB() {
	thread thread1(ThreadUpdateTiingoCryptoSymbolDB);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateForexExchangeDB() {
	thread thread1(ThreadUpdateForexExchangeDB);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateForexSymbolDB() {
	thread thread1(ThreadUpdateForexSymbolDB);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateCryptoExchangeDB() {
	thread thread1(ThreadUpdateCryptoExchangeDB);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateFinnhubCryptoSymbolDB() {
	thread thread1(ThreadUpdateFinnhubCryptoSymbolDB);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CWorldMarket::CreateThreadUpdateEconomicCalendarDB() {
	thread thread1(ThreadUpdateEconomicCalendarDB);
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
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
		pStock->UpdateDayLineDB();
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}
	return true;
}

bool CWorldMarket::UpdateCompanyNewsDB() {
	for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
		const auto pStock = gl_dataContainerFinnhubStock.GetStock(l);
		if (pStock->IsUpdateCompanyNewsDBAndClearFlag()) {// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			pStock->UpdateCompanyNewsDB();
		}
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}

	return (true);
}

bool CWorldMarket::UpdateInsiderTransactionDB() {
	int iCounter = 0;
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
		if (pStock->IsSaveInsiderTransactionAndClearFlag()) {
			if (pStock->HaveInsiderTransaction()) {
				iCounter++;
				pStock->UpdateInsiderTransactionDB();
			}
		}
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}
	if (iCounter > 0) {
		char buffer[30];
		sprintf_s(buffer, _T("%i"), iCounter);
		const CString str2 = buffer;
		gl_systemMessage.PushDayLineInfoMessage(_T("本次更新了") + str2 + _T("个股票的内部交易数据"));
	}
	return true;
}

bool CWorldMarket::UpdateInsiderSentimentDB() {
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
		if (pStock->IsSaveInsiderSentimentAndClearFlag()) {
			if (pStock->HaveInsiderSentiment()) {
				pStock->UpdateInsiderSentimentDB();
			}
		}
		if (gl_systemConfiguration.IsExitingSystem()) {
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
	gl_dataContainerFinnhubStock.ResetEPSSurprise();
	gl_pFinnhubDataSource->SetUpdateEPSSurprise(true);
}

void CWorldMarket::RebuildPeer() {
	gl_dataContainerFinnhubStock.ResetPeer();
	gl_pFinnhubDataSource->SetUpdatePeer(true);
}

void CWorldMarket::RebuildBasicFinancial() {
	gl_dataContainerFinnhubStock.ResetBasicFinancial();
	gl_pFinnhubDataSource->SetUpdateStockBasicFinancial(true);
}

void CWorldMarket::RebuildStockDayLineDB() {
	gl_dataContainerFinnhubStock.ResetDayLine();
	gl_pFinnhubDataSource->SetUpdateStockProfile(true);
}

void CWorldMarket::UpdateStockDayLineStartEndDate() {
	try {
		const CString strFilterPrefix = _T("[Symbol] = '");
		CSetWorldStockDayLine setWorldStockDayLine;

		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetStock(l);
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

	vectorString vSymbolTemp = gl_dataContainerChosenWorldStock.GetSymbolVector();
	for (auto symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	vSymbolTemp = gl_dataContainerChosenWorldForex.GetSymbolVector();
	for (auto symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	vSymbolTemp = gl_dataContainerChosenWorldCrypto.GetSymbolVector();
	for (auto symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	return vSymbol;
}

void CWorldMarket::StartAllWebSocket() {
	if (IsSystemReady()) {
		if (!gl_pFinnhubDataSource->IsWebError()) {
			if (gl_pFinnhubWebSocket->IsError() || gl_pFinnhubWebSocket->IsIdle()) StartFinnhubWebSocket();
		}
		if (!gl_pTiingoDataSource->IsWebError()) {
			if (gl_pTiingoIEXWebSocket->IsError() || gl_pTiingoIEXWebSocket->IsIdle()) StartTiingoIEXWebSocket();
			if (gl_pTiingoCryptoWebSocket->IsError() || gl_pTiingoCryptoWebSocket->IsIdle()) StartTiingoCryptoWebSocket();
			if (gl_pTiingoForexWebSocket->IsError() || gl_pTiingoForexWebSocket->IsIdle()) StartTiingoForexWebSocket();
		}
	}
}

void CWorldMarket::StartFinnhubWebSocket() {
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket() && !gl_pFinnhubDataSource->IsTimeout()) {
		gl_pFinnhubWebSocket->CreateThreadConnectAndSendMessage(GetFinnhubWebSocketSymbolVector());
		gl_pFinnhubWebSocket->SetHeartbeatTime(GetUTCTime());
	}
}

void CWorldMarket::StartTiingoIEXWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		gl_pTiingoIEXWebSocket->CreateThreadConnectAndSendMessage(gl_dataContainerChosenWorldStock.GetSymbolVector());
		gl_pTiingoIEXWebSocket->SetHeartbeatTime(GetUTCTime());
	}
}

void CWorldMarket::StartTiingoCryptoWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		gl_pTiingoCryptoWebSocket->CreateThreadConnectAndSendMessage(gl_dataContainerChosenWorldCrypto.GetSymbolVector());
		gl_pTiingoCryptoWebSocket->SetHeartbeatTime(GetUTCTime());
	}
}

void CWorldMarket::StartTiingoForexWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		gl_pTiingoForexWebSocket->CreateThreadConnectAndSendMessage(gl_dataContainerChosenWorldForex.GetSymbolVector());
		gl_pTiingoForexWebSocket->SetHeartbeatTime(GetUTCTime());
	}
}

/// <summary>
/// // 停止WebSocket。此函数等待其停止后方返回。是系统退出前的准备工作。
/// </summary>
void CWorldMarket::DisconnectAllWebSocket() {
	//本函数只在系统退出时调用
	//ASSERT(gl_systemConfiguration.IsExitingSystem()); 
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket()) gl_pFinnhubWebSocket->Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket()) gl_pTiingoIEXWebSocket->Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket()) gl_pTiingoCryptoWebSocket->Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket()) gl_pTiingoForexWebSocket->Disconnect();
}

void CWorldMarket::StopAllWebSocketIfTimeOut() const {
	if (IsSystemReady()) {
		if (gl_systemConfiguration.IsUsingFinnhubWebSocket()) {
			StopFinnhubWebSocketIfTimeOut();
		}
		if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket()) {
			StopTiingoCryptoWebSocketIfTimeOut();
		}
		if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket()) {
			StopTiingoIEXWebSocketIfTimeOut();
		}
		if (gl_systemConfiguration.IsUsingTiingoForexWebSocket()) {
			StopTiingoForexWebSocketIfTimeOut();
		}
	}
}

void CWorldMarket::StopFinnhubWebSocketIfTimeOut() {
	if (gl_pFinnhubDataSource->IsTimeout() || gl_pFinnhubWebSocket->IsIdle()) {
		gl_pFinnhubWebSocket->CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Finnhub web timeout，关闭Web socket服务"));
	}
}

void CWorldMarket::StopTiingoIEXWebSocketIfTimeOut() {
	if (gl_pTiingoDataSource->IsTimeout() && !gl_pTiingoIEXWebSocket->IsIdle()) {
		gl_pTiingoIEXWebSocket->CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX web timeout，关闭Web socket服务"));
	}
}

void CWorldMarket::StopTiingoCryptoWebSocketIfTimeOut() {
	if (gl_pTiingoDataSource->IsTimeout() || gl_pTiingoCryptoWebSocket->IsIdle()) {
		gl_pTiingoCryptoWebSocket->CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto web timeout，关闭Web socket服务"));
	}
}

void CWorldMarket::StopTiingoForexWebSocketIfTimeOut() {
	if (gl_pTiingoDataSource->IsTimeout() && !gl_pTiingoForexWebSocket->IsIdle()) {
		gl_pTiingoForexWebSocket->CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Forex web timeout，关闭Web socket服务"));
	}
}

void CWorldMarket::ProcessWebSocketData() {
	if (gl_pFinnhubWebSocket->DataSize() > 0) ProcessFinnhubWebSocketData();
	if (gl_pTiingoIEXWebSocket->DataSize() > 0) ProcessTiingoIEXWebSocketData();
	if (gl_pTiingoCryptoWebSocket->DataSize() > 0) ProcessTiingoCryptoWebSocketData();
	if (gl_pTiingoForexWebSocket->DataSize() > 0) ProcessTiingoForexWebSocketData();
}

void CWorldMarket::ProcessFinnhubWebSocketData() {
	const auto total = gl_pFinnhubWebSocket->DataSize();
	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		const auto pString = gl_pFinnhubWebSocket->PopData();
		CString strMessage = _T("Finnhub: ");
		strMessage += pString->c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_pFinnhubWebSocket->ParseFinnhubWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedFinnhubWebSocket(iTotalDataSize);
}

void CWorldMarket::ProcessTiingoIEXWebSocketData() {
	const auto total = gl_pTiingoIEXWebSocket->DataSize();
	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		const auto pString = gl_pTiingoIEXWebSocket->PopData();
		CString strMessage = _T("Tiingo IEX: ");
		strMessage += pString->c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_pTiingoIEXWebSocket->ParseTiingoIEXWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoIEXWebSocket(iTotalDataSize);
}

void CWorldMarket::ProcessTiingoCryptoWebSocketData() {
	const auto total = gl_pTiingoCryptoWebSocket->DataSize();

	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		const auto pString = gl_pTiingoCryptoWebSocket->PopData();
		CString strMessage = _T("Tiingo Crypto: ");
		strMessage += pString->c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_pTiingoCryptoWebSocket->ParseTiingoCryptoWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoCryptoWebSocket(iTotalDataSize);
}

void CWorldMarket::ProcessTiingoForexWebSocketData() {
	const auto total = gl_pTiingoForexWebSocket->DataSize();
	size_t iTotalDataSize = 0;
	for (size_t i = 0; i < total; i++) {
		const auto pString = gl_pTiingoForexWebSocket->PopData();
		CString strMessage = _T("Tiingo Forex: ");
		strMessage += pString->c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_pTiingoForexWebSocket->ParseTiingoForexWebSocketData(pString);
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

void CWorldMarket::UpdateWorldStockFromTiingoIEXSocket(const CTiingoIEXSocketPtr& pTiingoIEXbData) {
	if (gl_dataContainerFinnhubStock.IsSymbol(pTiingoIEXbData->m_sSymbol.c_str())) {
		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(pTiingoIEXbData->m_sSymbol.c_str());
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

void CWorldMarket::UpdateWorldStockFromFinnhubSocket(const CFinnhubSocketPtr& pFinnhubData) {
	if (gl_dataContainerFinnhubStock.IsSymbol(pFinnhubData->m_sSymbol.c_str())) {
		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(pFinnhubData->m_sSymbol.c_str());
		pStock->SetActive(true);
		pStock->SetNew(pFinnhubData->m_dLastPrice * 1000);
	}
}

void CWorldMarket::UpdateMarketStatus(const CMarketStatusVectorPtr& pv) const {
	for (auto p : *pv) {
		m_pvMarketStatus->push_back(p);
	}
}

void CWorldMarket::UpdateMarketHoliday(const CMarketHolidayVectorPtr& pv) const {
	for (auto p : *pv) {
		m_pvMarketHoliday->push_back(p);
	}
}
