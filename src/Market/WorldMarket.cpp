#include"pch.h"

#include"systemData.h"

#include "WorldMarket.h"

#include "AccessoryDataSource.h"
#include"thread.h"

#include"FinnhubInaccessibleExchange.h"

#include"FinnhubDataSource.h"

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoCryptoWebSocket.h"

#include"SetFinnhubStockDayLine.h"

#include "ChinaMarket.h"
#include "InfoReport.h"
#include "QuandlDataSource.h"
#include "SetIndexNasdaq100.h"
#include "SetIndexNasdaq100MA200UpDownRate.h"
#include "ThreadStatus.h"
#include "TiingoDataSource.h"
#include "TiingoInaccessibleStock.h"
#include "TimeConvert.h"

CWorldMarket::CWorldMarket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE("CWorldMarket市场变量只允许存在一个实例\n");
	}

	m_strMarketId = _T("US");
	m_exchange = gl_dataContainerStockExchange.GetItem(m_strMarketId);
	ASSERT(m_exchange != nullptr);
	m_strLocalMarketTimeZone = _T("America/New_York");
	GetMarketLocalTimeOffset(m_strLocalMarketTimeZone);// 美国股市使用美东标准时间, 美国股市开市时间为九点三十分

	// 无需（也无法）每日更新的变量放在这里
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // 由于证券代码总数有二十万之多，无法在一天之内更新完，故不再重置此索引。
	m_bFinnhubWebSiteAccessible = true;

	CalculateTime();

	m_pvMarketStatus = make_shared<vector<CMarketStatusPtr>>();
	m_pvMarketHoliday = make_shared<vector<CMarketHolidayPtr>>();

	Reset();

	AddTask(WORLD_MARKET_CREATE_TASK__, 1);
}

CWorldMarket::~CWorldMarket() {
	PrepareToCloseMarket();
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

	m_lFinnhubDayLineUpdated = 0;
}

void CWorldMarket::ResetQuandl() {
}

void CWorldMarket::ResetTiingo() {
	if (IsEarlyThen(gl_tiingoInaccessibleStock.GetUpdateDate(), GetMarketDate(), 7)) {
		gl_tiingoInaccessibleStock.Clear(); // 不使用更新时间早于一周的数据。清除之，让系统自动查验新的状态。
		gl_tiingoInaccessibleStock.SetUpdateDate(GetMarketDate());
	}
	m_lTiingoStockDayLineUpdated = 0;
}

void CWorldMarket::ResetDataContainer() {
	gl_dataContainerFinnhubForexExchange.Reset();
	gl_dataContainerFinnhubCryptoExchange.Reset();
	gl_dataFinnhubForexSymbol.Reset();
	gl_dataFinnhubCryptoSymbol.Reset();
	gl_dataContainerFinnhubCountry.Reset();
	gl_dataContainerFinnhubEconomicCalendar.Reset();

	gl_dataContainerFinnhubStock.Reset();

	gl_dataContainerTiingoStock.Reset();
	gl_dataContainerTiingoNewSymbol.Reset();
	gl_dataContainerTiingoCryptoSymbol.Reset();

	gl_dataContainerChosenFinnhubStock.Reset();
	gl_dataContainerChosenWorldForex.Reset();
	gl_dataContainerChosenWorldCrypto.Reset();
}

void CWorldMarket::ResetMarket() {
	m_fResettingMarket = true;

	Reset();

	UpdateToken();

	gl_dataContainerFinnhubCountry.LoadDB();
	gl_dataContainerFinnhubStock.LoadDB();
	gl_dataContainerChosenFinnhubStock.LoadDB();
	gl_dataContainerFinnhubForexExchange.LoadDB();
	gl_dataFinnhubForexSymbol.LoadDB();
	gl_dataContainerChosenWorldForex.LoadDB();
	gl_dataContainerFinnhubCryptoExchange.LoadDB();
	gl_dataFinnhubCryptoSymbol.LoadDB();
	gl_dataContainerChosenWorldCrypto.LoadDB();
	gl_dataContainerFinnhubEconomicCalendar.LoadDB();

	gl_dataContainerTiingoStock.LoadDB();
	gl_dataContainerTiingoCryptoSymbol.LoadDB();
	gl_dataContainerTiingoFundamentalDefinition.LoadDB();

	for (const auto& pDataSource : m_vDataSource) {
		pDataSource->Reset();
	}

	string s = _T("重置World Market于美东标准时间：") + GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(s);

	m_fResettingMarket = false;
}

void CWorldMarket::PrepareToCloseMarket() {
	//ASSERT(gl_systemConfiguration.IsExitingSystem());
	DisconnectAllWebSocket();
}

//////////////////////////////////////////////////////////////////////////////////////
//
// 各任务调度函数
//
// todo 采用concurrencpp::timer后，可以将此函数的各项任务分配为定时器所驱动的工作线程任务。
//
//////////////////////////////////////////////////////////////////////////////////////
int CWorldMarket::ProcessTask(long lCurrentTime) {
	if (IsMarketTaskEmpty()) return false;
	const auto pTask = GetMarketTask();
	if (lCurrentTime >= pTask->GetTime()) {
		DiscardCurrentMarketTask();
		switch (pTask->GetType()) {
		case WORLD_MARKET_CREATE_TASK__: // 生成其他任务
			TaskCreateTask(lCurrentTime);
			break;
		case WORLD_MARKET_CHECK_SYSTEM_READY__: // 170000重启系统
			TaskCheckMarketReady(lCurrentTime);
			break;
		case WORLD_MARKET_RESET__: // 170000重启系统
			TaskResetMarket(lCurrentTime);
			break;
		case WORLD_MARKET_UPDATE_DB__:
			ASSERT(!IsTimeToResetSystem(lCurrentTime));// 下午五时重启系统，各数据库需要重新装入，故而此时不允许更新数据库。
			TaskUpdateWorldMarketDB(lCurrentTime);
			break;
		case WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__:
			TaskMonitorWebSocket(lCurrentTime);
			break;
		case WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__:
			TaskProcessWebSocketData(lCurrentTime);
			TaskPerSecond(lCurrentTime);
			break;
		case WORLD_MARKET_CALCULATE_NASDAQ100_200MA_UPDOWN_RATE:
			TaskCalculateNasdaq100MA200UpDownRate(lCurrentTime);
			break;
		case WORLD_MARKET_CONNECT_FINNHUB_WEB_SOCKET__:
			ASSERT(!gl_systemConfiguration.IsUsingFinnhubWebSocket());
			gl_systemConfiguration.SetUsingFinnhubWebSocket(true); // 只设置标识，实际启动由其他任务完成。
			break;
		case WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__:
			gl_pTiingoDataSource->SetUpdateIEXTopOfBook(true); //
			break;
		case WORLD_MARKET_TIINGO_INQUIRE_DAYlINE__:
			if (gl_pWorldMarket->GetMarketDate() == gl_pWorldMarket->GetCurrentTradeDate() && lCurrentTime < 180000) {
				AddTask(WORLD_MARKET_TIINGO_INQUIRE_DAYlINE__, 180000);
			}
			else { // 当日18时之后或者第二日交易时间前
				gl_pTiingoDataSource->SetUpdateStockDailyMeta(true);
				gl_pTiingoDataSource->SetUpdateDayLine(true);
			}
			break;
		case WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__: // Note 20点后执行，刚闭市时数据可能不全。
			ASSERT(!gl_systemConfiguration.IsPaidTypeTiingoAccount()); // 免费账户需要处理当日数据（付费账户下载所有股票的日线）
			gl_pWorldMarket->TaskCreateTiingoTradeDayDayLine(lCurrentTime);
			break;
		case WORLD_MARKET_TIINGO_PROCESS_DAYLINE__:
			gl_pWorldMarket->TaskProcessTiingoDayLine(lCurrentTime);
			break;
		default:
			break;
		}
		return pTask->GetType();
	}
	return 0;
}

int CWorldMarket::ProcessCurrentImmediateTask(long lMarketTime) {
	ASSERT(!m_marketImmediateTask.Empty());

	auto pTask = m_marketImmediateTask.GetTask();
	auto taskType = pTask->GetType();
	m_marketImmediateTask.DiscardCurrentTask();

	return taskType;
}

int CWorldMarket::XferMarketTimeToIndex() {
	return XferChinaMarketTimeToIndex(GetMarketTM());
}

void CWorldMarket::TaskCreateTask(long lCurrentTime) {
	const long lTimeMinute = (lCurrentTime / 100) * 100; // 当前小时和分钟

	while (!IsMarketTaskEmpty()) DiscardCurrentMarketTask();

	// 系统初始化检查
	AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, 1);

	// 市场重置
	if (lCurrentTime < gl_systemConfiguration.GetWorldMarketResettingTime()) {
		AddTask(WORLD_MARKET_RESET__, gl_systemConfiguration.GetWorldMarketResettingTime()); // 执行时间为170000之后
	}

	AddTask(WORLD_MARKET_UPDATE_DB__, lTimeMinute + 40);// 更新股票简介数据库的任务

	AddTask(WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__, lCurrentTime);

	AddTask(WORLD_MARKET_TIINGO_INQUIRE_DAYlINE__, GetNextTime(lCurrentTime, 0, 0, 20)); // 开始下载日线历史数据

	AddTask(WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__, GetNextTime(lTimeMinute, 0, 1, 0)); // 
	AddTask(WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__, GetNextTime(lTimeMinute, 0, 2, 0)); // 两分钟后开始监测WebSocket

	AddTask(WORLD_MARKET_CALCULATE_NASDAQ100_200MA_UPDOWN_RATE, GetNextTime(lTimeMinute, 0, 3, 0)); // 三分钟计算Nasdaq100 200MA比率

	AddTask(WORLD_MARKET_CREATE_TASK__, 240000); // 重启市场任务的任务于每日零时执行
}

void CWorldMarket::TaskProcessWebSocketData(long lCurrentTime) {
	ASSERT(!IsTimeToResetSystem(lCurrentTime));	// 下午五时重启系统，各数据库需要重新装入，故而此时不允许更新。

	ProcessWebSocketData();
	UpdateFinnhubStockFromWebSocket();

	long lNextTime = GetNextSecond(lCurrentTime);
	if (IsTimeToResetSystem(lNextTime)) lNextTime = GetNextTime(GetResetTime(), 0, 5, 1);
	AddTask(WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__, lNextTime);
}

void CWorldMarket::TaskMonitorWebSocket(long lCurrentTime) {
	AddTask(WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__, GetNextTime(lCurrentTime, 0, 1, 0));
	if (!IsSystemReady()) return;

	gl_pFinnhubWebSocket->MonitorWebSocket(GetFinnhubWebSocketSymbols());
	gl_pTiingoCryptoWebSocket->MonitorWebSocket(gl_dataContainerChosenWorldCrypto.GetSymbols());
	gl_pTiingoIEXWebSocket->MonitorWebSocket(gl_dataContainerChosenFinnhubStock.GetSymbols());
	gl_pTiingoForexWebSocket->MonitorWebSocket(gl_dataContainerChosenWorldForex.GetSymbols());
}

void CWorldMarket::TaskResetMarket(long lCurrentTime) {
	// 市场时间十七时重启系统
	ASSERT(!m_fResettingMarket);
	ResetMarket();
	SetSystemReady(false);
	ASSERT(!m_fResettingMarket);

	AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, lCurrentTime); // 每次重置系统时，必须设置系统状态检查任务
}

bool CWorldMarket::TaskUpdateTiingoIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) { // 更新tiingo stock profile与finnhub更新stock profile互斥
		gl_runtime.background_executor()->post([this] {
			TRACE("Finnhub update profile\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F profile"));
			auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			this->UpdateTiingoIndustry();
			auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			if ((end - start).count() > 2000) {
				string s = fmt::format("Finnhub update Profile  Saving time: {:Ld}ms", (end - start).count());
				gl_systemMessage.PushInnerSystemInformationMessage(s);
			}
			TRACE("Finnhub update profile ended\n");
		});
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateSicIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// 更新tiingo stock profile与finnhub更新stock profile互斥
		gl_runtime.background_executor()->post([this] {
			TRACE("Sic Industry\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F Sic Industry"));
			this->UpdateSicIndustry();
			TRACE("Sic Industry updated\n");
		});
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateNaicsIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// 更新tiingo stock profile与finnhub更新stock profile互斥
		gl_runtime.background_executor()->post([this] {
			TRACE("NaicsIndustry\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F Naics Industry"));
			this->UpdateNaicsIndustry();
		});
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将Tiingo日线数据存入数据库．
//
// 无论是否执行了存储函数，都需要将下载的日线历史数据删除，这样能够节省内存的占用。由于实际存储功能使用线程模式实现，
// 故而其执行时间可能晚于主线程，导致主线程删除日线数据时出现同步问题。解决的方法是让工作线程独立删除存储后的日线数据，
// 主线程的删除函数只在不调用工作线程（无需存储日线数据）的情况下方才执行。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
void CWorldMarket::TaskUpdateTiingoStockDayLineDB() {
	CTiingoStockPtr pTiingoStock = nullptr;
	const size_t symbolSize = gl_dataContainerTiingoStock.Size();

	for (int i = 0; i < symbolSize; i++) {
		if (gl_systemConfiguration.IsExitingSystem()) break;// 如果程序正在退出，则停止存储。
		pTiingoStock = gl_dataContainerTiingoStock.GetStock(i);
		if (pTiingoStock->IsUpdateDayLineDB()) {
			pTiingoStock->UpdateDayLineDB();
			SetTiingoStockDayLineUpdated(GetTiingoStockDayLineUpdated() + 1);
			pTiingoStock->UpdateDayLineStartEndDate();
			pTiingoStock->UnloadDayLine();
			pTiingoStock->SetUpdateDayLineDB(false);
			pTiingoStock->SetUpdateProfileDB(true);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将Forex日线数据存入数据库．
//
// 无论是否执行了存储函数，都需要将下载的日线历史数据删除，这样能够节省内存的占用。由于实际存储功能使用线程模式实现，
// 故而其执行时间可能晚于主线程，导致主线程删除日线数据时出现同步问题。解决的方法是让工作线程独立删除存储后的日线数据，
// 主线程的删除函数只在不调用工作线程（无需存储日线数据）的情况下方才执行。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskUpdateForexDayLineDB() {
	bool fUpdated = false;
	CForexSymbolPtr pSymbol = nullptr;
	const size_t symbolSize = gl_dataFinnhubForexSymbol.Size();

	//TRACE("Finnhub forex dayLine\n");
	for (int i = 0; i < symbolSize; i++) {
		pSymbol = gl_dataFinnhubForexSymbol.GetItem(i);
		if (pSymbol->IsUpdateDayLineDB()) {
			pSymbol->SetUpdateDayLineDB(false);
			pSymbol->SetUpdateDayLineDB(false);
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					gl_runtime.thread_executor()->post([pSymbol] {
						gl_UpdateWorldMarketDB.acquire();
						gl_systemMessage.SetWorldMarketSavingFunction(_T("F forex dayline"));
						auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
						if (!gl_systemConfiguration.IsExitingSystem()) {// 如果程序正在退出，则停止存储。
							pSymbol->UpdateDayLineDB();
							pSymbol->UpdateDayLineStartEndDate();
							pSymbol->SetUpdateProfileDB(true);
							pSymbol->UnloadDayLine();
							string str = pSymbol->GetSymbol();
							str += _T("日线资料存储完成");
							gl_systemMessage.PushDayLineInfoMessage(str);
						}
						auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
						if ((end - start).count() > 2000) {
							string s = fmt::format("FFinnhub Update forex dayLine  Saving time: {:Ld}ms", (end - start).count());
							gl_systemMessage.PushInnerSystemInformationMessage(s);
						}
						gl_UpdateWorldMarketDB.release();
					});
					fUpdated = true;
				}
				else pSymbol->UnloadDayLine(); // 当无需执行存储函数时，这里还要单独卸载日线数据。因存储日线数据线程稍后才执行，故而不能在此统一执行删除函数。
			}
			else { // 此种情况为有股票代码，但此代码尚未上市
				pSymbol->SetIPOStatus(_STOCK_NOT_YET_LIST_);
				string str1 = pSymbol->GetSymbol();
				str1 += _T(" 为未上市股票代码");
				gl_systemMessage.PushDayLineInfoMessage(str1);
			}
		}
	}

	return (fUpdated);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//
// 无论是否执行了存储函数，都需要将下载的日线历史数据删除，这样能够节省内存的占用。由于实际存储功能使用线程模式实现，
// 故而其执行时间可能晚于主线程，导致主线程删除日线数据时出现同步问题。解决的方法是让工作线程独立删除存储后的日线数据，
// 主线程的删除函数只在不调用工作线程（无需存储日线数据）的情况下方才执行。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskUpdateCryptoDayLineDB() {
	bool fUpdated = false;
	CFinnhubCryptoPtr pSymbol = nullptr;
	const size_t symbolSize = gl_dataFinnhubCryptoSymbol.Size();

	//TRACE("Finnhub Crypto dayLine\n");
	for (int i = 0; i < symbolSize; ++i) {
		pSymbol = gl_dataFinnhubCryptoSymbol.GetItem(i);
		if (pSymbol->IsUpdateDayLineDB()) {
			pSymbol->SetUpdateDayLineDB(false);
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					gl_runtime.thread_executor()->post([pSymbol] {
						gl_UpdateWorldMarketDB.acquire();
						gl_systemMessage.SetWorldMarketSavingFunction(_T("F crypto dayLine"));
						auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
						if (!gl_systemConfiguration.IsExitingSystem()) { // 如果程序正在退出，则停止存储。
							pSymbol->UpdateDayLineDB();
							pSymbol->UpdateDayLineStartEndDate();
							pSymbol->SetUpdateProfileDB(true);
							pSymbol->UnloadDayLine();
							string str2 = pSymbol->GetSymbol();
							str2 += _T("日线资料存储完成");
							gl_systemMessage.PushDayLineInfoMessage(str2);
						}
						auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
						if ((end - start).count() > 2000) {
							string s = fmt::format("Finnhub update Crypto dayLine Saving time: {:Ld}ms", (end - start).count());
							gl_systemMessage.PushInnerSystemInformationMessage(s);
						}
						gl_UpdateWorldMarketDB.release();
					});
					fUpdated = true;
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
					string str1 = pSymbol->GetSymbol();
					str1 += _T(" 为未上市股票代码");
					gl_systemMessage.PushDayLineInfoMessage(str1);
				}
			}
		}
	}
	return fUpdated;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Note 02/01/2005后，tiingo.com不再提供全部的IEX盘后数据，放弃使用。
// 如何处理当日数据的逻辑位于此函数中。
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWorldMarket::TaskCreateTiingoTradeDayDayLine(long lCurrentTime) {
	if (!gl_pTiingoDataSource->IsUpdateDayLine()) { // 当前交易日已经接收到日线数据？
		if (IsEndMarketIEXTopOfBookUpdated()) {// 已接收到了IEX TopOfBook数据？
			gl_systemMessage.PushInnerSystemInformationMessage("process Tiingo IEX data");
			gl_runtime.thread_executor()->post([] {
				gl_UpdateWorldMarketDB.acquire();
				TRACE("process IEX data\n");
				gl_systemMessage.SetWorldMarketSavingFunction(_T("T process IEX"));
				gl_dataContainerTiingoStock.BuildDayLine(gl_pWorldMarket->GetCurrentTradeDate());
				TRACE("process IEX data ended\n");
				gl_UpdateWorldMarketDB.release();
			});
			if (lCurrentTime < 233000) {
				SetEndMarketIEXTopOfBookUpdate(false);
				AddTask(WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__, GetNextTime(lCurrentTime, 0, 28, 0)); // 申请IEX数据
				AddTask(WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__, GetNextTime(lCurrentTime, 0, 30, 0));
			}
		}
		else { // 尚未接收IEX数据？这是初始状态
			AddTask(WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__, GetNextTime(lCurrentTime, 0, 0, 1)); // 申请IEX数据
			AddTask(WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__, GetNextTime(lCurrentTime, 0, 2, 0)); // 两分钟后处理
		}
	}
	else {
		AddTask(WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__, GetNextTime(lCurrentTime, 0, 1, 0)); // 一分钟后执行下一次
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理tiingo日线数据。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CWorldMarket::TaskProcessTiingoDayLine(long lCurrentTime) {
	if (gl_systemConfiguration.GetTiingoStockDayLineProcessedDate() >= GetCurrentTradeDate()) return; // 已更新完52WeekHighLow,不再自动更新。
	ASSERT(!gl_pTiingoDataSource->IsUpdateDayLine());// 接收完日线数据后方可处理
	ASSERT(!gl_pTiingoDataSource->IsUpdateIEXTopOfBook()); // 接收完IEX日线数据后方可处理
	gl_runtime.thread_executor()->post([] {
		gl_dataContainerTiingoStock.TaskProcessDayLine();
	});
	gl_runtime.thread_executor()->post([] {
		gl_dataContainerTiingoStock.TaskProcessDayLine();
	});
}

void CWorldMarket::TaskDeleteDelistedStock() {
	gl_runtime.thread_executor()->post([] {
		DeleteTiingoDelistedStock();
	});
}

void CWorldMarket::TaskPerSecond(long lCurrentTime) {
	static int m_iCountDownPerMinute = 59;
	static int m_iCountDownPerHour = 3599;
	static int m_iCountDownPerDay = 3500 * 24 - 1;

	if (--m_iCountDownPerMinute < 0) { // 每分钟一次
		m_iCountDownPerMinute = 59;
	}
	if (--m_iCountDownPerHour < 0) { // 每小时一次
		ASSERT(gl_systemConfiguration.GetTiingoHourLyRequestLimit() > 100);
		m_iCountDownPerHour = 3599;
	}
	if (--m_iCountDownPerDay < 0) { // 每天一次
		ASSERT(gl_systemConfiguration.GetTiingoDailyRequestLimit() > 10000);
		m_iCountDownPerDay = 3600 * 24 - 1;
	}
}

bool CWorldMarket::UpdateEPSSurpriseDB() {
	const size_t stockSize = gl_dataContainerFinnhubStock.Size();

	CFinnhubStockPtr pStock = nullptr;
	for (long l = 0; l < stockSize; ++l) {
		pStock = gl_dataContainerFinnhubStock.GetItem(l);
		if (pStock->IsUpdateEPSSurpriseDB()) {
			pStock->SetUpdateEPSSurpriseDB(false);
			pStock->UpdateEPSSurpriseDB();
		}
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}

	return (true);
}

void CWorldMarket::UpdateSECFilingsDB() {
	const size_t stockSize = gl_dataContainerFinnhubStock.Size();

	CFinnhubStockPtr pStock = nullptr;
	for (long l = 0; l < stockSize; ++l) {
		pStock = gl_dataContainerFinnhubStock.GetItem(l);
		if (pStock->IsUpdateSECFilingsDB()) {
			pStock->SetUpdateSECFilingsDB(false);
			ASSERT(pStock->UpdateSECFilingsDB());
		}
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}
}

void CWorldMarket::TaskCalculateNasdaq100MA200UpDownRate(long lCurrentTime) {
#ifndef DEBUG
	if (!gl_pAccessoryDataSource->IsUpdateIndexNasdaq100Stocks() || !gl_pTiingoDataSource->IsUpdateDayLine()) {
		AddTask(WORLD_MARKET_CALCULATE_NASDAQ100_200MA_UPDOWN_RATE, GetNextTime(lCurrentTime, 0, 10, 0)); // 十分钟继续计算Nasdaq100 200MA比率
		return;
	}
#endif
	ASSERT(!gl_pAccessoryDataSource->IsUpdateIndexNasdaq100Stocks()); // 每日更新Nasdaq100代码
	gl_runtime.thread_executor()->post([this] {
		this->LoadNasdaq100StocksDayLine().get();
		this->CalculateNasdaq100StocksMA(200);
		this->calculateNasdaq100MA200UpDownRate();
		gl_systemMessage.PushInformationMessage(_T("Nasdaq 100 200MA upDown rate calculated"));
	});
}

concurrencpp::result<bool> CWorldMarket::LoadNasdaq100StocksDayLine() {
	m_vNasdaq100TiingoStock.clear();
	CSetIndexNasdaq100 setIndexNasdaq100;

	setIndexNasdaq100.Open();
	while (!setIndexNasdaq100.IsEOF()) {
		string sSymbol = setIndexNasdaq100.m_Symbol.GetString();
		if (gl_dataContainerTiingoStock.IsSymbol(sSymbol)) {
			// 只计算代码集中的股票。目前GOOG代码不存在，只有GOOGL.
			auto pStock = gl_dataContainerTiingoStock.GetStock(sSymbol);
			m_vNasdaq100TiingoStock.push_back(pStock);
		}
		setIndexNasdaq100.MoveNext();
	}
	setIndexNasdaq100.Close();

	vector<result<bool>> results;

	bool succeed = true;
	/*
	for (auto pStock : m_vNasdaq100TiingoStock) {
		gl_BackgroundWorkingThread.acquire();
		auto result = gl_runtime.thread_executor()->submit([pStock] {
			if (!pStock->IsDayLineLoaded()) {
				pStock->LoadDayLineDB();
			}
			gl_BackgroundWorkingThread.release();
			return true;
		});
		results.emplace_back(std::move(result));
	}
	for (auto& r : results) {
		//auto a = r.resolve();
		succeed &= co_await r;
	}
	*/

	for (auto pStock : m_vNasdaq100TiingoStock) {
		if (!pStock->IsDayLineLoaded()) {
			pStock->LoadDayLineDB();
		}
	}

	co_return succeed;
}

void CWorldMarket::CalculateNasdaq100StocksMA(const int length) const {
	for (auto pStock : m_vNasdaq100TiingoStock) {
		if (pStock->IsDayLineLoaded()) {
			pStock->CalculateDayLineMA(length);
		}
	}
}

class upDownRate200MA {
public:
	long lDate;
	int Rate;
};

void CWorldMarket::calculateNasdaq100MA200UpDownRate() const {
	vector<upDownRate200MA> vUpDownRate;

	long lBeginDate = GetPrevDay(GetMarketDate(), 365); // 最多计算一年内的数据
	long lCurrentDate = lBeginDate;
	long lEndDate = GetMarketDate();
	while (lCurrentDate < lEndDate) {
		int iTotalStock = 0;
		int iAbove200MA = 0;
		for (auto& pStock : m_vNasdaq100TiingoStock) {
			if (pStock->HaveDayLine(lCurrentDate)) {
				iTotalStock++;
				auto pDayLine = pStock->GetDayLineAtDate(lCurrentDate);
				if (pDayLine->GetClose() > pDayLine->GetAverage(200)) iAbove200MA++;
			}
		}
		if (iTotalStock > 80) {
			upDownRate200MA upDown;
			upDown.lDate = lCurrentDate;
			upDown.Rate = iAbove200MA * 100 / iTotalStock;
			vUpDownRate.push_back(upDown);
		}
		lCurrentDate = GetNextDay(lCurrentDate);
	}

	CSetIndexNasdaq100MA200UpDownRate setIndex;
	lCurrentDate = 0;

	setIndex.m_strSort = _T("[Date]");
	setIndex.Open();
	setIndex.m_pDatabase->BeginTrans();
	if (!setIndex.IsEOF()) {
		setIndex.MoveLast();
		lCurrentDate = setIndex.m_Date;
		setIndex.MoveNext();
	}
	for (auto upDownRate : vUpDownRate) {
		if (upDownRate.lDate > lCurrentDate) {
			setIndex.AddNew();
			setIndex.m_Date = upDownRate.lDate;
			setIndex.m_Rate = upDownRate.Rate;
			setIndex.Update();
		}
	}
	setIndex.m_pDatabase->CommitTrans();
	setIndex.Close();
}

bool CWorldMarket::TaskCheckMarketReady(long lCurrentTime) {
	if (!IsSystemReady()) {
		if (!gl_pFinnhubDataSource->IsUpdateSymbol() && !gl_pFinnhubDataSource->IsUpdateForexExchange() && !gl_pFinnhubDataSource->IsUpdateForexSymbol()
			&& !gl_pFinnhubDataSource->IsUpdateCryptoExchange() && !gl_pFinnhubDataSource->IsUpdateCryptoSymbol()) {
			const string str = "世界市场初始化完毕";
			gl_systemMessage.PushInformationMessage(str);
			SetSystemReady(true);
		}
	}
	if (!IsSystemReady()) {
		AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, GetNextSecond(lCurrentTime));
	}
	return IsSystemReady();
}

void CWorldMarket::TaskUpdateWorldMarketDB(long lCurrentTime) {
	if (gl_dataContainerFinnhubCountry.GetLastTotalCountry() < gl_dataContainerFinnhubCountry.GetTotalCountry()) { // 国家名称
		gl_runtime.background_executor()->post([] {
			TRACE("finnhub Country\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F Country"));
			gl_dataContainerFinnhubCountry.UpdateDB();
			TRACE("finnhub Country updated\n");
		});
	}
	if (gl_dataContainerFinnhubForexExchange.IsNeedUpdate()) { // ForexExchange
		gl_runtime.background_executor()->post([] {
			TRACE("finnhub Forex Exchange\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F Forex Exchange"));
			gl_dataContainerFinnhubForexExchange.UpdateDB();
			TRACE("finnhub Forex Exchange updated\n");
		});
	}
	if (gl_dataFinnhubForexSymbol.IsUpdateProfileDB()) { // Forex symbol
		gl_runtime.background_executor()->post([] {
			TRACE("finnhub Forex Symbol\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F Forex Symbol"));
			gl_dataFinnhubForexSymbol.UpdateDB();
			TRACE("finnhub Forex Symbol updated\n");
		});
	}
	if (gl_dataContainerFinnhubCryptoExchange.IsNeedUpdate()) { // Crypto Exchange
		gl_runtime.background_executor()->post([] {
			TRACE("finnhub Crypto Exchange\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F Crypto Exchange"));
			gl_dataContainerFinnhubCryptoExchange.UpdateDB();
			TRACE("finnhub Crypto Exchange updated\n");
		});
	}
	if (gl_dataFinnhubCryptoSymbol.IsUpdateProfileDB()) { // crypto symbol
		gl_runtime.background_executor()->post([] {
			TRACE("finnhub Crypto Symbol\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F Crypto symbol"));
			gl_dataFinnhubCryptoSymbol.UpdateDB();
			TRACE("finnhub Crypto Symbol updated\n");
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateInsiderTransactionDB()) { // Insider Transaction
		gl_runtime.background_executor()->post([] {
			TRACE("finnhub Insider Transaction\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F Insider Transaction"));
			auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			gl_pWorldMarket->UpdateInsiderTransactionDB();
			auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			if ((end - start).count() > 2000) {
				string s = fmt::format("Finnhub insider transaction Saving time: {:Ld}ms", (end - start).count());
				gl_systemMessage.PushInnerSystemInformationMessage(s);
			}
			TRACE("finnhub Insider Transaction updated\n");
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateInsiderSentimentDB()) { // Insider Sentiment
		gl_runtime.background_executor()->post([] {
			TRACE("finnhub Insider Sentiment\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F Insider Sentiment"));
			gl_pWorldMarket->UpdateInsiderSentimentDB();
			TRACE("finnhub Insider Sentiment updated\n");
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateDayLineDB()) { // stock dayLine
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			TRACE("finnhub Stock DayLine\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F Stock dayLine"));
			gl_pWorldMarket->UpdateFinnhubStockDayLineDB();
			TRACE("finnhub Stock DayLine updated\n");
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataContainerFinnhubEconomicCalendar.IsUpdateDB()) { // Economic Calendar
		gl_runtime.background_executor()->post([] {
			TRACE("finnhub Economic calendar\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F Economic calendar"));
			gl_dataContainerFinnhubEconomicCalendar.UpdateDB();
			TRACE("finnhub Economic calendar updated\n");
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateCompanyNewsDB()) { // Company News
		gl_runtime.background_executor()->post([] {
			TRACE("finnhub News\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F News"));
			gl_pWorldMarket->UpdateCompanyNewsDB();
			TRACE("finnhub News updated\n");
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateBasicFinancialDB()) { // Basic financial
		gl_runtime.background_executor()->post([] {
			TRACE("finnhub Basic financial\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F Basic financial"));
			gl_dataContainerFinnhubStock.UpdateBasicFinancialDB(); // 此任务很费时，原因待查。目前先不使用此隔绝区
			TRACE("finnhub Basic financial updated\n");
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateEPSSurpriseDB()) { // stock EPS surprise
		gl_runtime.background_executor()->post([] {
			TRACE("finnhub EPS surprise\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F EPS surprise"));
			gl_pWorldMarket->UpdateEPSSurpriseDB();
			TRACE("finnhub EPS surprise updated\n");
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateSECFilingsDB()) { // stock EPS surprise
		gl_runtime.background_executor()->post([] {
			TRACE("finnhub SEC Filings\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("F SEC filings"));
			gl_pWorldMarket->UpdateSECFilingsDB();
			TRACE("finnhub SEC Filings updated\n");
		});
	}

	// Tiingo部分
	static bool s_bUpdatingTiingoStockProfile = false;
	if (gl_dataContainerTiingoStock.IsUpdateProfileDB() && !gl_ThreadStatus.IsSavingWorldMarketThreadRunning() && !s_bUpdatingTiingoStockProfile) { // Tiingo Stock
		gl_runtime.background_executor()->post([] {
			s_bUpdatingTiingoStockProfile = true;
			gl_UpdateWorldMarketDB.acquire();
			TRACE("Tiingo stock profile\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("T profile"));
			auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			gl_dataContainerTiingoStock.UpdateDB();
			auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			if ((end - start).count() > 2000) {
				string s = fmt::format("Tiingo stock Saving time: {:Ld}ms", (end - start).count());
				gl_systemMessage.PushInnerSystemInformationMessage(s);
			}
			TRACE("Tiingo stock profile updated\n");
			gl_UpdateWorldMarketDB.release();
			s_bUpdatingTiingoStockProfile = false;
		});
	}

	if (gl_dataContainerTiingoCryptoSymbol.IsUpdateProfileDB()) { // Tiingo crypto symbol
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			TRACE("Tiingo crypto symbol\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("T crypto symbol"));
			gl_dataContainerTiingoCryptoSymbol.UpdateDB();
			gl_UpdateWorldMarketDB.release();
		});
	}

	if (gl_dataContainerTiingoFundamentalDefinition.IsUpdateDB()) { // Tiingo crypto symbol
		gl_runtime.background_executor()->post([] {
			TRACE("Tiingo Definition\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("T definition"));
			gl_dataContainerTiingoFundamentalDefinition.UpdateDB();
		});
	}

	static bool s_updatingTiingoFinancialState = false;
	if (gl_dataContainerTiingoStock.IsUpdateFinancialStateDB() && !s_updatingTiingoFinancialState) {
		gl_runtime.background_executor()->post([] {
			s_updatingTiingoFinancialState = true;
			TRACE("Tiingo Financial State\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("T financial state"));
			auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			gl_dataContainerTiingoStock.UpdateFinancialStateDB();
			auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			if ((end - start).count() > 2000) {
				string s = fmt::format("Tiingo Financial statement Saving time: {:Ld}ms", (end - start).count());
				gl_systemMessage.PushInnerSystemInformationMessage(s);
			}
			TRACE("Tiingo Financial State updated\n");
			s_updatingTiingoFinancialState = true;
		});
	}

	static bool s_updatingTiingoStockDayLine = false;
	if (gl_dataContainerTiingoStock.IsUpdateDayLineDB() && !s_updatingTiingoStockDayLine) {
		gl_runtime.background_executor()->post([] {
			s_updatingTiingoStockDayLine = true;
			gl_UpdateWorldMarketDB.acquire();
			TRACE("Update Tiingo stock DayLine\n");
			gl_systemMessage.SetWorldMarketSavingFunction(_T("T stock dayline"));
			auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			gl_pWorldMarket->TaskUpdateTiingoStockDayLineDB();
			auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			if ((end - start).count() > 2000) {
				string s = fmt::format("Tiingo Stock dayLine Saving time: {:Ld}ms", (end - start).count());
				gl_systemMessage.PushInnerSystemInformationMessage(s);
			}
			TRACE("Tiingo stock DayLine updated\n");
			gl_UpdateWorldMarketDB.release();
			s_updatingTiingoStockDayLine = false;
		});
	}

	TaskUpdateForexDayLineDB(); // 这个函数内部继续生成工作线程
	TaskUpdateCryptoDayLineDB(); // 这个函数内部继续生成工作线程

	if (!gl_pFinnhubDataSource->IsUpdateSymbol() && gl_dataContainerFinnhubStock.IsUpdateProfileDB()) { // stock profile
		static int s_counter2 = 0;
		if (s_counter2 > 30) {
			gl_runtime.background_executor()->post([] {
				gl_UpdateWorldMarketDB.acquire();
				TRACE("Finnhub profile\n");
				gl_systemMessage.SetWorldMarketSavingFunction(_T("F stock profile"));
				auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
				gl_dataContainerFinnhubStock.UpdateProfileDB();
				auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
				if ((end - start).count() > 2000) {
					string s = fmt::format("Finnhub Profile  Saving time: {:Ld}ms", (end - start).count());
					gl_systemMessage.PushInnerSystemInformationMessage(s);
				}
				TRACE("Finnhub profile updated\n");
				gl_UpdateWorldMarketDB.release();
			});
			s_counter2 = 0;
		}
		s_counter2++;
	}

	if (gl_finnhubInaccessibleExchange.IsUpdateDB()) { // 更新禁止访问证券交易所名单
		gl_runtime.background_executor()->post([] {
			gl_finnhubInaccessibleExchange.UpdateDB();
			gl_finnhubInaccessibleExchange.SetUpdateDB(false);
		});
	}

	if (gl_tiingoInaccessibleStock.IsUpdateDB()) { // 更新禁止访问证券交易所名单
		gl_runtime.background_executor()->post([] {
			gl_tiingoInaccessibleStock.UpdateDB();
			gl_tiingoInaccessibleStock.SetUpdateDB(false);
		});
	}

	long lNextTime = GetNextTime(lCurrentTime, 0, 1, 0);
	if (IsTimeToResetSystem(lNextTime)) lNextTime = GetResetTime() + 510;
	ASSERT(!IsTimeToResetSystem(lNextTime));// 重启系统时各数据库需要重新装入，故而此时不允许更新数据库。
	AddTask(WORLD_MARKET_UPDATE_DB__, lNextTime); // 每五分钟更新一次
}

bool CWorldMarket::UpdateToken() {
	ASSERT(gl_systemConfiguration.IsInitialized());

	if (gl_systemConfiguration.GetFinnhubToken().length() > 5) {
		gl_pFinnhubDataSource->SetInquiryToken(gl_systemConfiguration.GetFinnhubToken().c_str());
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Finnhub Token Needed"));
	}
	if (gl_systemConfiguration.GetTiingoToken().length() > 5) {
		gl_pTiingoDataSource->SetInquiryToken(gl_systemConfiguration.GetTiingoToken().c_str());
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Tiingo Token Needed"));
	}
	if (gl_systemConfiguration.GetQuandlToken().length() > 5) {
		gl_pQuandlDataSource->SetInquiryToken(gl_systemConfiguration.GetQuandlToken().c_str());
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Quandl Token Needed"));
	}

	return true;
}

bool CWorldMarket::UpdateFinnhubStockDayLineDB() {
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(i);
		pStock->UpdateDayLineDB();
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}
	return true;
}

bool CWorldMarket::UpdateCompanyNewsDB() {
	for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
		const auto pStock = gl_dataContainerFinnhubStock.GetItem(l);
		if (pStock->IsUpdateCompanyNewsDB()) {
			pStock->SetUpdateCompanyNewsDB(false);
			pStock->UpdateCompanyNewsDB();
		}
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}

	return (true);
}

bool CWorldMarket::UpdateInsiderSentimentDB() {
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(i);
		if (pStock->IsUpdateInsiderSentimentDB()) {
			pStock->SetUpdateInsiderSentimentDB(false);
			if (pStock->HaveInsiderSentiment()) {
				pStock->UpdateInsiderSentimentDB();
			}
		}
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}
	return true;
}

bool CWorldMarket::UpdateTiingoIndustry() {
	// not implemented
	return false;
}

bool CWorldMarket::UpdateSicIndustry() {
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
		const string strFilterPrefix = _T("[Symbol] = '");
		CSetFinnhubStockDayLine setFinnhubStockDayLine;

		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetItem(l);
			setFinnhubStockDayLine.m_strFilter = (strFilterPrefix + pStock->GetSymbol() + _T("'")).c_str();
			setFinnhubStockDayLine.m_strSort = _T("[Date]");
			setFinnhubStockDayLine.Open();
			if (!setFinnhubStockDayLine.IsEOF()) {
				if (setFinnhubStockDayLine.m_Date < pStock->GetDayLineStartDate()) {
					pStock->SetDayLineStartDate(setFinnhubStockDayLine.m_Date);
					pStock->SetUpdateProfileDB(true);
				}
				setFinnhubStockDayLine.MoveLast();
				if (setFinnhubStockDayLine.m_Date > pStock->GetDayLineEndDate()) {
					pStock->SetDayLineEndDate(setFinnhubStockDayLine.m_Date);
					pStock->SetUpdateProfileDB(true);
				}
			}
			setFinnhubStockDayLine.Close();
		}
	} catch (CException& e) {
		ReportInformation(e);
	}
}

/// <summary>
/// Finnhub WebSocket的免费账户最多只能发送50个证券的数据
/// </summary>
vectorString CWorldMarket::GetFinnhubWebSocketSymbols() {
	vectorString vSymbol;

	vectorString vSymbolTemp = gl_dataContainerChosenFinnhubStock.GetSymbols();
	for (auto symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	vSymbolTemp = gl_dataContainerChosenWorldForex.GetSymbols();
	for (auto symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	vSymbolTemp = gl_dataContainerChosenWorldCrypto.GetSymbols();
	for (auto symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	return vSymbol;
}

/// <summary>
/// // 停止WebSocket。此函数等待其停止后方返回。是系统退出前的准备工作。
/// 这里直接调用Disconnect(),
/// </summary>
void CWorldMarket::DisconnectAllWebSocket() {
	//本函数只在系统退出时调用
	//ASSERT(gl_systemConfiguration.IsExitingSystem()); 
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket()) gl_pFinnhubWebSocket->Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket()) gl_pTiingoIEXWebSocket->Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket()) gl_pTiingoCryptoWebSocket->Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket()) gl_pTiingoForexWebSocket->Disconnect();
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
	for (size_t i = 0; i < total; i++) {
		const auto pString = gl_pFinnhubWebSocket->PopData();
		string strMessage = _T("Finnhub: ");
		strMessage += *pString;
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_pFinnhubWebSocket->ParseFinnhubWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedFinnhubWebSocket(iTotalDataSize);
}

void CWorldMarket::ProcessTiingoIEXWebSocketData() {
	const auto total = gl_pTiingoIEXWebSocket->DataSize();
	size_t iTotalDataSize = 0;
	for (size_t i = 0; i < total; i++) {
		const auto pString = gl_pTiingoIEXWebSocket->PopData();
		string strMessage = _T("Tiingo IEX: ");
		strMessage += *pString;
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_pTiingoIEXWebSocket->ParseTiingoIEXWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoIEXWebSocket(iTotalDataSize);
}

void CWorldMarket::ProcessTiingoCryptoWebSocketData() {
	const auto total = gl_pTiingoCryptoWebSocket->DataSize();

	size_t iTotalDataSize = 0;
	for (size_t i = 0; i < total; i++) {
		const auto pString = gl_pTiingoCryptoWebSocket->PopData();
		string strMessage = _T("Tiingo Crypto: ");
		strMessage += *pString;
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
		string strMessage = _T("Tiingo Forex: ");
		strMessage += *pString;
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
void CWorldMarket::UpdateFinnhubStockFromWebSocket() {
	auto total = gl_SystemData.GetTiingoIEXSocketSize();
	for (size_t i = 0; i < total; i++) {
		const CTiingoIEXSocketPtr pIEXData = gl_SystemData.PopTiingoIEXSocket();
		UpdateFinnhubStockFromTiingoIEXSocket(pIEXData);
		gl_systemMessage.SetCurrentTiingoWebSocketIEX(pIEXData->m_sSymbol);
	}

	total = gl_SystemData.GetTiingoCryptoSocketSize();
	for (size_t i = 0; i < total; i++) {
		const CTiingoCryptoSocketPtr pCryptoData = gl_SystemData.PopTiingoCryptoSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketCrypto(pCryptoData->m_sSymbol);
	}

	total = gl_SystemData.GetTiingoForexSocketSize();
	for (size_t i = 0; i < total; i++) {
		const CTiingoForexSocketPtr pForexData = gl_SystemData.PopTiingoForexSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketForex(pForexData->m_sSymbol);
	}

	total = gl_SystemData.GetFinnhubSocketSize();
	for (size_t i = 0; i < total; i++) {
		const CFinnhubSocketPtr pFinnhubData = gl_SystemData.PopFinnhubSocket();
		UpdateFinnhubStockFromFinnhubSocket(pFinnhubData);
		gl_systemMessage.SetCurrentFinnhubWebSocketStake(pFinnhubData->m_sSymbol);
	}
}

void CWorldMarket::UpdateFinnhubStockFromTiingoIEXSocket(const CTiingoIEXSocketPtr& pTiingoIEXbData) {
	if (gl_dataContainerFinnhubStock.IsSymbol(pTiingoIEXbData->m_sSymbol.c_str())) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(pTiingoIEXbData->m_sSymbol.c_str());
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

void CWorldMarket::UpdateFinnhubStockFromFinnhubSocket(const CFinnhubSocketPtr& pFinnhubData) {
	if (gl_dataContainerFinnhubStock.IsSymbol(pFinnhubData->m_sSymbol.c_str())) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(pFinnhubData->m_sSymbol.c_str());
		pStock->SetActive(true);
		pStock->SetNew(pFinnhubData->m_dLastPrice * 1000);
	}
}

void CWorldMarket::UpdateMarketStatus(const CMarketStatusesPtr& pv) const {
	for (auto p : *pv) {
		m_pvMarketStatus->push_back(p);
	}
}

void CWorldMarket::UpdateMarketHoliday(const CMarketHolidaysPtr& pv) const {
	for (auto p : *pv) {
		m_pvMarketHoliday->push_back(p);
	}
}

void CWorldMarket::DeleteTiingoDelistedStock() {
	auto Size = gl_dataContainerTiingoDelistedSymbol.Size();
	for (size_t index = 0; index < Size; index++) {
		auto pTiingoDelistedStock = gl_dataContainerTiingoDelistedSymbol.GetStock(index);
		ASSERT(gl_dataContainerTiingoStock.IsSymbol(pTiingoDelistedStock));
		DeleteTiingoDayLine(pTiingoDelistedStock); // 删除日线
		DeleteTiingoFinancialStatement(pTiingoDelistedStock); // 删除财经报告
		gl_dataContainerTiingoStock.Delete(pTiingoDelistedStock->GetSymbol()); // 删除代码
	}
}

void CWorldMarket::DeleteTiingoDayLine(const CTiingoStockPtr& pStock) {
	CSetTiingoStockDayLine setDayLine;
	setDayLine.m_strFilter = _T("[Symbol] = '");
	setDayLine.m_strFilter += pStock->GetSymbol().c_str();
	setDayLine.m_strFilter += _T("'");

	setDayLine.Open();
	setDayLine.m_pDatabase->BeginTrans();
	while (!setDayLine.IsEOF()) {
		setDayLine.Delete();
		setDayLine.MoveNext();
	}
	setDayLine.m_pDatabase->CommitTrans();
	setDayLine.Close();
}

void CWorldMarket::DeleteTiingoFinancialStatement(const CTiingoStockPtr& pStock) {
	CSetTiingoCompanyFinancialState setDayLine;
	setDayLine.m_strFilter = _T("[Symbol] = '");
	setDayLine.m_strFilter += pStock->GetSymbol().c_str();
	setDayLine.m_strFilter += _T("'");

	setDayLine.Open();
	setDayLine.m_pDatabase->BeginTrans();
	while (!setDayLine.IsEOF()) {
		setDayLine.Delete();
		setDayLine.MoveNext();
	}
	setDayLine.m_pDatabase->CommitTrans();
	setDayLine.Close();
}

bool CWorldMarket::IsTodayMarketEnded() {
	return false;
}
