#include"pch.h"

#include"systemData.h"

#include "WorldMarket.h"

#include "AccessoryDataSource.h"
#include "AlphaVantageDataSource.h"
#include"thread.h"

#include"InaccessibleSymbol.h"

#include"FinnhubDataSource.h"

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoCryptoWebSocket.h"

#include "ChinaMarket.h"
#include "dataBaseConnector.h"
#include "QuandlDataSource.h"
#include "ThreadStatus.h"
#include "TiingoDataSource.h"
#include "TimeConvert.h"

namespace {
	template <typename TWebSocket>
	void ProcessWebSocketDataGeneric(const TWebSocket& pWebSocket, const std::string& prefix, const std::function<void(size_t)>& setProcessedFunc) {
		const auto total = pWebSocket->DataSize();
		size_t iTotalDataSize = 0;
		for (size_t i = 0; i < total; i++) {
			const auto pString = pWebSocket->PopData();
			std::string strMessage = prefix;
			strMessage += *pString;
			gl_systemMessage.PushWebSocketInfoMessage(strMessage);
			iTotalDataSize += pString->size();
			(pWebSocket->ParseWebSocketData)(pString);
		}
		setProcessedFunc(iTotalDataSize);
	}
}

CWorldMarket::CWorldMarket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE(_T("CWorldMarket市场变量只允许存在一个实例\n"));
	}

	gl_dataContainerStockExchange.LoadDB(); // 交易所信息只需装载一次。

	m_strMarketId = "US";
	m_exchange = gl_dataContainerStockExchange.GetItem(m_strMarketId);
	ASSERT(m_exchange != nullptr);
	m_strLocalMarketTimeZone = "America/New_York";
	GetMarketLocalTimeOffset(m_strLocalMarketTimeZone);// 美国股市使用美东标准时间, 美国股市开市时间为九点三十分

	// 无需（也无法）每日更新的变量放在这里
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // 由于证券代码总数有二十万之多，无法在一天之内更新完，故不再重置此索引。
	m_bFinnhubWebSiteAccessible = true;

	CalculateTime();

	m_pvMarketStatus = make_shared<vector<CMarketStatus>>();
	m_pvMarketHoliday = make_shared<vector<CMarketHoliday>>();

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
	gl_dataContainerFinnhubStock.LoadProfileDB();
	gl_dataContainerChosenFinnhubStock.LoadDB();
	gl_dataContainerFinnhubForexExchange.LoadDB();
	gl_dataFinnhubForexSymbol.LoadProfileDB();
	gl_dataContainerChosenWorldForex.LoadDB();
	gl_dataContainerFinnhubCryptoExchange.LoadDB();
	gl_dataFinnhubCryptoSymbol.LoadProfileDB();
	gl_dataContainerChosenWorldCrypto.LoadDB();
	gl_dataContainerFinnhubEconomicCalendar.LoadDB();

	gl_dataContainerTiingoStock.LoadProfileDB();
	gl_dataContainerTiingoCryptoSymbol.LoadDB();
	gl_dataContainerTiingoFundamentalDefinition.LoadDB();

	for (const auto& pDataSource : m_vDataSource) {
		pDataSource->Reset();
	}

	string s = "重置World Market于美东标准时间：" + GetStringOfMarketTime();
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
		case WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__:
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
	switch (taskType) {
	default:
		ASSERT(0); // 错误的任务号
		break;
	}
	return pTask->GetType();
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
			gl_systemMessage.SetWorldMarketSavingFunction("F profile");
			auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			this->UpdateTiingoIndustry();
			auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			if ((end - start).count() > 2000) {
				string s = fmt::format("Finnhub update Profile  Saving time: {:Ld}ms", (end - start).count());
				gl_systemMessage.PushInnerSystemInformationMessage(s);
			}
		});
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateSicIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// 更新tiingo stock profile与finnhub更新stock profile互斥
		gl_runtime.background_executor()->post([this] {
			gl_systemMessage.SetWorldMarketSavingFunction("F Sic Industry");
			this->UpdateSicIndustry();
		});
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateNaicsIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// 更新tiingo stock profile与finnhub更新stock profile互斥
		gl_runtime.background_executor()->post([this] {
			gl_systemMessage.SetWorldMarketSavingFunction("F Naics Industry");
			this->UpdateNaicsIndustry();
		});
		return true;
	}
	return false;
}

bool CWorldMarket::TaskRebuildTiingoStockSplitDB() {
	gl_runtime.background_executor()->post([this] {
		gl_systemMessage.SetWorldMarketSavingFunction("T Rebuild Stock Split DB");
		this->RebuildTiingoStockSplitDB();
	});
	return true;
}

bool CWorldMarket::TaskRebuildTiingoIndustryRS() {
	gl_runtime.background_executor()->post([this] {
		gl_systemMessage.SetWorldMarketSavingFunction("T Rebuild Industry RS");
		this->RebuildIndustryRS();
	});
	return true;
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
int CWorldMarket::TaskUpdateTiingoStockDayLineDB() {
	CTiingoStockPtr pTiingoStock = nullptr;
	const size_t symbolSize = gl_dataContainerTiingoStock.Size();
	int iUpdatedCount = 0;
	for (size_t i = 0; i < symbolSize; i++) {
		if (gl_systemConfiguration.IsExitingSystem()) break;// 如果程序正在退出，则停止存储。
		pTiingoStock = gl_dataContainerTiingoStock.GetStock(i);
		if (pTiingoStock->IsUpdateDayLineDB()) {
			pTiingoStock->UpdateDayLineDB();
			SetTiingoStockDayLineUpdated(GetTiingoStockDayLineUpdated() + 1);
			pTiingoStock->UpdateDayLineStartEndDate();
			pTiingoStock->UnloadDayLine();
			pTiingoStock->SetUpdateDayLineDB(false);
			pTiingoStock->SetUpdateProfileDB(true);
			iUpdatedCount++;
		}
	}
	return iUpdatedCount;
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
	for (size_t i = 0; i < symbolSize; i++) {
		pSymbol = gl_dataFinnhubForexSymbol.GetItem(i);
		if (pSymbol->IsUpdateDayLineDB()) {
			pSymbol->SetUpdateDayLineDB(false);// Only call once
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					gl_runtime.thread_executor()->post([pSymbol] {
						gl_UpdateWorldMarketDB.acquire();
						gl_systemMessage.SetWorldMarketSavingFunction("F forex dayLine");
						auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
						if (!gl_systemConfiguration.IsExitingSystem()) {// 如果程序正在退出，则停止存储。
							pSymbol->UpdateDayLineDB();
							pSymbol->UpdateDayLineStartEndDate();
							pSymbol->SetUpdateProfileDB(true);
							pSymbol->UnloadDayLine();
							string str = pSymbol->GetSymbol();
							str += "日线资料存储完成";
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
				str1 += " 为未上市股票代码";
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
	int iUpdated = 0;
	CFinnhubCryptoPtr pSymbol = nullptr;
	const size_t symbolSize = gl_dataFinnhubCryptoSymbol.Size();

	//TRACE("Finnhub Crypto dayLine\n");
	for (size_t i = 0; i < symbolSize; ++i) {
		pSymbol = gl_dataFinnhubCryptoSymbol.GetItem(i);
		if (pSymbol->IsUpdateDayLineDB()) {
			pSymbol->SetUpdateDayLineDB(false);
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					gl_runtime.thread_executor()->post([pSymbol] {
						gl_UpdateWorldMarketDB.acquire();
						gl_systemMessage.SetWorldMarketSavingFunction("F crypto dayLine");
						auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
						pSymbol->UpdateDayLineDB();
						string str2 = pSymbol->GetSymbol();
						str2 += "日线资料存储完成";
						gl_systemMessage.PushDayLineInfoMessage(str2);
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
					str1 += " 为未上市股票代码";
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
				gl_systemMessage.SetWorldMarketSavingFunction("T process IEX");
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
	for (size_t l = 0; l < stockSize; ++l) {
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
	for (size_t l = 0; l < stockSize; ++l) {
		pStock = gl_dataContainerFinnhubStock.GetItem(l);
		if (pStock->IsUpdateSECFilingsDB()) {
			pStock->SetUpdateSECFilingsDB(false);
			pStock->UpdateSECFilingsDB();
			pStock->ClearSECFilings(); // 存储完毕后清除数据，节省内存占用。
		}
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}
}

void CWorldMarket::TaskCalculateNasdaq100MA200UpDownRate(long lCurrentTime) {
	if (!gl_pAccessoryDataSource->IsUpdateIndexNasdaq100Stocks() || !gl_pTiingoDataSource->IsUpdateDayLine()) {
		AddTask(WORLD_MARKET_CALCULATE_NASDAQ100_200MA_UPDOWN_RATE, GetNextTime(lCurrentTime, 0, 10, 0)); // 十分钟继续计算Nasdaq100 200MA比率
		return;
	}
	gl_runtime.thread_executor()->post([this] {
		this->LoadNasdaq100StocksDayLine().get();
		this->CalculateNasdaq100StocksMA(200);
		this->calculateNasdaq100MA200UpDownRate();
		gl_systemMessage.PushStockMarketInformationMessage("Nasdaq 100 200MA upDown rate calculated");
	});
}

concurrencpp::result<bool> CWorldMarket::LoadNasdaq100StocksDayLine() {
	m_vNasdaq100TiingoStock.clear();
	using namespace StockMarket;
	const auto& t = IndexNasdaq100{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);
	auto rows = db(select(all_of(t)).from(t).unconditionally());
	for (const auto& row : rows) {
		if (gl_dataContainerTiingoStock.IsSymbol(row.Symbol)) {
			auto pStock = gl_dataContainerTiingoStock.GetStock(row.Symbol);
			m_vNasdaq100TiingoStock.push_back(pStock);
		}
	}
	tx.commit();

	vector<result<bool>> results;

	bool succeed = true;

	for (auto& pStock : m_vNasdaq100TiingoStock) {
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

	co_return succeed;
}

void CWorldMarket::CalculateNasdaq100StocksMA(int length) const {
	for (auto& pStock : m_vNasdaq100TiingoStock) {
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

void CWorldMarket::calculateNasdaq100MA200UpDownRate() {
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

	if (!vUpDownRate.empty()) {
		if (vUpDownRate.at(vUpDownRate.size() - 1).lDate == GetMarketDate()) {
			if (vUpDownRate.at(vUpDownRate.size() - 1).Rate >= 80) {
				gl_systemMessage.PushStockMarketInformationMessage("Nasdaq 100 upDown rate > 80%");
			}
			if (vUpDownRate.at(vUpDownRate.size() - 1).Rate < 20) {
				gl_systemMessage.PushStockMarketInformationMessage("Nasdaq 100 upDown rate < 20%");
			}
		}
		string str = std::format("Nasdaq 100 200MA upDown rate calculated, current at {:d}", vUpDownRate.at(vUpDownRate.size() - 1).Rate);
		gl_systemMessage.PushStockMarketInformationMessage(str);
	}

	using namespace StockMarket;
	const auto& t = IndexNasdaq100_200MaUpdownRate{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);
	auto multi_insert = insert_into(t).columns(t.Date, t.Rate);

	auto result = db(select(all_of(t)).from(t).unconditionally());
	int rows = result.size();
	if (rows > 0) {
		for (int i = 0; i < rows - 1; i++) {
			result.pop_front();
		}
		auto& row = result.front();
		lCurrentDate = row.Date;
	}

	int nValues = 0;
	for (auto upDownRate : vUpDownRate) {
		if (upDownRate.lDate > lCurrentDate) {
			multi_insert.values.add(t.Date = upDownRate.lDate, t.Rate = upDownRate.Rate);
			nValues++;
		}
	}
	if (nValues > 0) {
		db(multi_insert);
	}
	tx.commit();
}

void CWorldMarket::calculateStockYearHigherRate() {
}

bool CWorldMarket::TaskCheckMarketReady(long lCurrentTime) {
	if (!IsSystemReady()) {
		if (!gl_pFinnhubDataSource->IsUpdateSymbol() && !gl_pFinnhubDataSource->IsUpdateForexExchange() && !gl_pFinnhubDataSource->IsUpdateForexSymbol()
			&& !gl_pFinnhubDataSource->IsUpdateCryptoExchange() && !gl_pFinnhubDataSource->IsUpdateCryptoSymbol()) {
			gl_systemMessage.PushInformationMessage("世界市场初始化完毕");
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
			gl_systemMessage.SetWorldMarketSavingFunction("F Country");
			gl_dataContainerFinnhubCountry.UpdateDB();
		});
	}
	if (gl_dataContainerFinnhubForexExchange.IsNeedUpdate()) { // ForexExchange
		gl_runtime.background_executor()->post([] {
			gl_systemMessage.SetWorldMarketSavingFunction("F Forex Exchange");
			gl_dataContainerFinnhubForexExchange.UpdateDB();
		});
	}
	if (gl_dataFinnhubForexSymbol.IsUpdateProfileDB()) { // Forex symbol
		gl_runtime.background_executor()->post([] {
			gl_systemMessage.SetWorldMarketSavingFunction("F Forex Symbol");
			gl_dataFinnhubForexSymbol.UpdateProfileDB();
		});
	}
	if (gl_dataContainerFinnhubCryptoExchange.IsNeedUpdate()) { // Crypto Exchange
		gl_runtime.background_executor()->post([] {
			gl_systemMessage.SetWorldMarketSavingFunction("F Crypto Exchange");
			gl_dataContainerFinnhubCryptoExchange.UpdateDB();
		});
	}
	if (gl_dataFinnhubCryptoSymbol.IsUpdateProfileDB()) { // crypto symbol
		gl_runtime.background_executor()->post([] {
			gl_systemMessage.SetWorldMarketSavingFunction("F Crypto symbol");
			gl_dataFinnhubCryptoSymbol.UpdateProfileDB();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateInsiderTransactionDB()) { // Insider Transaction
		gl_runtime.background_executor()->post([] {
			gl_systemMessage.SetWorldMarketSavingFunction("F Insider Transaction");
			auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			gl_pWorldMarket->UpdateInsiderTransactionDB();
			auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			if ((end - start).count() > 2000) {
				string s = fmt::format("Finnhub insider transaction Saving time: {:Ld}ms", (end - start).count());
				gl_systemMessage.PushInnerSystemInformationMessage(s);
			}
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateInsiderSentimentDB()) { // Insider Sentiment
		gl_runtime.background_executor()->post([] {
			gl_systemMessage.SetWorldMarketSavingFunction("F Insider Sentiment");
			gl_pWorldMarket->UpdateInsiderSentimentDB();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateDayLineDB()) { // stock dayLine
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_systemMessage.SetWorldMarketSavingFunction("F Stock dayLine");
			gl_pWorldMarket->UpdateFinnhubStockDayLineDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataContainerFinnhubEconomicCalendar.IsUpdateDB()) { // Economic Calendar
		gl_runtime.background_executor()->post([] {
			gl_systemMessage.SetWorldMarketSavingFunction("F Economic calendar");
			gl_dataContainerFinnhubEconomicCalendar.UpdateDB();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateCompanyNewsDB()) { // Company News
		gl_runtime.background_executor()->post([] {
			gl_systemMessage.SetWorldMarketSavingFunction("F News");
			gl_pWorldMarket->UpdateCompanyNewsDB();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateEPSSurpriseDB()) { // stock EPS surprise
		gl_runtime.background_executor()->post([] {
			gl_systemMessage.SetWorldMarketSavingFunction("F EPS surprise");
			gl_pWorldMarket->UpdateEPSSurpriseDB();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateSECFilingsDB()) { // stock EPS surprise
		gl_runtime.background_executor()->post([] {
			gl_systemMessage.SetWorldMarketSavingFunction("F SEC filings");
			gl_pWorldMarket->UpdateSECFilingsDB();
		});
	}

	// Tiingo部分
	static std::atomic<bool> s_bUpdatingTiingoStockProfile = false;
	if (gl_dataContainerTiingoStock.IsUpdateProfileDB() && !gl_ThreadStatus.IsSavingWorldMarketThreadRunning() && !s_bUpdatingTiingoStockProfile) { // Tiingo Stock
		gl_runtime.background_executor()->post([] {
			s_bUpdatingTiingoStockProfile = true;
			gl_UpdateWorldMarketDB.acquire();
			gl_systemMessage.SetWorldMarketSavingFunction("T profile");
			auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			gl_dataContainerTiingoStock.UpdateProfileDB();
			auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			if ((end - start).count() > 2000) {
				string s = fmt::format("Tiingo stock Saving time: {:Ld}ms", (end - start).count());
				gl_systemMessage.PushInnerSystemInformationMessage(s);
			}
			gl_UpdateWorldMarketDB.release();
			s_bUpdatingTiingoStockProfile = false;
		});
	}

	if (gl_dataContainerTiingoCryptoSymbol.IsUpdateProfileDB()) { // Tiingo crypto symbol
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_systemMessage.SetWorldMarketSavingFunction("T crypto symbol");
			gl_dataContainerTiingoCryptoSymbol.UpdateDB();
			gl_UpdateWorldMarketDB.release();
		});
	}

	if (gl_dataContainerTiingoFundamentalDefinition.IsUpdateDB()) { // Tiingo crypto symbol
		gl_runtime.background_executor()->post([] {
			gl_systemMessage.SetWorldMarketSavingFunction("T definition");
			gl_dataContainerTiingoFundamentalDefinition.UpdateDB();
		});
	}

	static bool s_updatingTiingoFinancialState = false;
	if (gl_dataContainerTiingoStock.IsUpdateFinancialStateDB() && !s_updatingTiingoFinancialState) {
		gl_runtime.background_executor()->post([] {
			s_updatingTiingoFinancialState = true;
			gl_systemMessage.SetWorldMarketSavingFunction("T financial state");
			auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			gl_dataContainerTiingoStock.UpdateFinancialStateDB();
			auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			if ((end - start).count() > 2000) {
				string s = fmt::format("Tiingo Financial statement Saving time: {:Ld}ms", (end - start).count());
				gl_systemMessage.PushInnerSystemInformationMessage(s);
			}
			s_updatingTiingoFinancialState = true;
		});
	}

	static bool s_updatingTiingoStockDayLine = false;
	if (gl_dataContainerTiingoStock.IsUpdateDayLineDB() && !s_updatingTiingoStockDayLine) {
		gl_runtime.background_executor()->post([] {
			s_updatingTiingoStockDayLine = true;
			gl_UpdateWorldMarketDB.acquire();
			gl_systemMessage.SetWorldMarketSavingFunction("T stock dayline");
			auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			auto iUpdatedCount = gl_pWorldMarket->TaskUpdateTiingoStockDayLineDB();
			auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			if ((end - start).count() > 2000) {
				string s = fmt::format("{:d} Tiingo Stock dayLine Saving time: {:Ld}ms", iUpdatedCount, (end - start).count());
				gl_systemMessage.PushInnerSystemInformationMessage(s);
			}
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
				gl_systemMessage.SetWorldMarketSavingFunction("F stock profile");
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
		gl_pFinnhubDataSource->SetInquiryToken(gl_systemConfiguration.GetFinnhubToken());
	}
	else {
		gl_systemMessage.PushInformationMessage("Finnhub Token Needed");
	}
	if (gl_systemConfiguration.GetTiingoToken().length() > 5) {
		gl_pTiingoDataSource->SetInquiryToken(gl_systemConfiguration.GetTiingoToken());
	}
	else {
		gl_systemMessage.PushInformationMessage("Tiingo Token Needed");
	}
	if (gl_systemConfiguration.GetQuandlToken().length() > 5) {
		gl_pQuandlDataSource->SetInquiryToken(gl_systemConfiguration.GetQuandlToken());
	}
	else {
		gl_systemMessage.PushInformationMessage("Quandl Token Needed");
	}

	if (gl_systemConfiguration.GetAlphaVantageToken().length() > 5) {
		gl_pAlphaVantageDataSource->SetInquiryToken(gl_systemConfiguration.GetAlphaVantageToken());
	}
	else {
		gl_systemMessage.PushInformationMessage("AlphaVantage Token Needed");
	}

	return true;
}

bool CWorldMarket::UpdateFinnhubStockDayLineDB() {
	for (size_t i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(i);
		pStock->UpdateDayLineDB();
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}
	return true;
}

bool CWorldMarket::UpdateCompanyNewsDB() {
	for (size_t l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
		const auto pStock = gl_dataContainerFinnhubStock.GetItem(l);
		if (pStock->IsUpdateCompanyNewsDB()) {
			pStock->UpdateCompanyNewsDB();
			pStock->ClearCompanyNews();
			pStock->SetUpdateCompanyNewsDB(false);
		}
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}

	return (true);
}

bool CWorldMarket::UpdateInsiderSentimentDB() {
	for (size_t i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(i);
		if (pStock->IsUpdateInsiderSentimentDB()) {
			pStock->SetUpdateInsiderSentimentDB(false);
			if (pStock->HaveInsiderSentiment()) {
				pStock->UpdateInsiderSentimentDB();
				pStock->ClearInsiderSentiment();
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

void CWorldMarket::RebuildTiingoStockSplitDB() {
	for (size_t index = 0; index < gl_dataContainerTiingoStock.Size(); index++) {
		auto pStock = gl_dataContainerTiingoStock.GetStock(index);
		gl_BackgroundWorkingThread.acquire();
		gl_runtime.background_executor()->post([pStock] {
			pStock->RebuildStockSplitDB();
			pStock->SetUpdateProfileDB(true);
			gl_BackgroundWorkingThread.release();
		});
	}
}

void CWorldMarket::UpdateOneYearStockDayLine() {
	long lBeginDate = GetPrevDay(GetMarketDate(), 365);
	for (size_t index = 0; index < gl_dataContainerTiingoStock.Size(); index++) {
		auto pStock = gl_dataContainerTiingoStock.GetStock(index);
		pStock->SetDayLineEndDate(lBeginDate);
		pStock->SetUpdateDayLine(true);
	}
}

void CWorldMarket::UpdateAllStockDayLine() {
	for (size_t index = 0; index < gl_dataContainerTiingoStock.Size(); index++) {
		auto pStock = gl_dataContainerTiingoStock.GetStock(index);
		pStock->SetDayLineEndDate(19800101); // 从1980年开始更新日线数据
		pStock->SetUpdateDayLine(true);
	}
}

void CWorldMarket::RebuildIndustryRS() {
	BuildIndustry();
	CalculateIndustryTotalValue();
}

void CWorldMarket::BuildIndustry() {
	for (size_t index = 0; index < 1000; index++) {
		m_aTiingoIndustryCode[index].clear();
	}
	for (size_t index = 0; index < gl_dataContainerTiingoStock.Size(); index++) {
		auto pStock = gl_dataContainerTiingoStock.GetStock(index);
		auto sicCode = pStock->GetSicCode() / 10;
		if (sicCode != 0) {
			m_aTiingoIndustryCode[sicCode].push_back(pStock);
		}
	}
}

void CWorldMarket::CalculateIndustryTotalValue() {
	for (const auto& vStocks : m_aTiingoIndustryCode) {
		CalculateStockTotalValue(vStocks);
	}
}

struct dayLineValue {
	long lDate;
	double dTotalValue;
};

using dayLineValuePtr = std::shared_ptr<dayLineValue>;

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 计算某一行业的每日总市值。
///
///Todo:: 目前仅支持SIC行业分类。
///
////////////////////////////////////////////////////////////////////////////////////////////////////
void CWorldMarket::CalculateStockTotalValue(const vector<CTiingoStockPtr>& vStocks) {
	vector<dayLineValuePtr> vDayLineValue;

	for (const auto& pStock : vStocks) {
		if (!pStock->IsDayLineLoaded()) pStock->LoadDayLineDB();
	}
	size_t totalDayLineSize = 0;
	for (const auto& pStock : vStocks) {
		totalDayLineSize += pStock->GetDayLineSize();
	}
	vDayLineValue.resize(totalDayLineSize + 128);
	for (const auto& pStock : vStocks) {
		CFinnhubStockPtr pFinnhubStock = gl_dataContainerFinnhubStock.GetItem(pStock->GetSymbol());
		if (pFinnhubStock != nullptr) { // 如果没有对应的Finnhub股票资料，则无法计算总市值
			const double shareOutstanding = pFinnhubStock->GetShareOutstanding() * 1000000;
			for (size_t index = 0; index < pStock->GetDayLineSize(); index++) {
				auto dayLine = pStock->GetDayLine(index);
				dayLineValuePtr pValue = make_shared<dayLineValue>();
				pValue->lDate = dayLine->GetDate();
				pValue->dTotalValue = dayLine->GetClose() * shareOutstanding / dayLine->GetRatio();
				vDayLineValue.push_back(pValue);
			}
		}
		else {
			string str = "无法计算 ";
			str += pStock->GetSymbol();
			str += " 的行业总市值，因找不到对应的Finnhub股数";
			gl_systemMessage.PushErrorMessage(str);
		}
	}
	ranges::sort(vDayLineValue, [](const dayLineValuePtr& p1, const dayLineValuePtr& p2) { return p1->lDate < p2->lDate; });

	vector<dayLineValuePtr> vResultValue;
	size_t index = 0;
	while (index < vDayLineValue.size()) {
		auto pValue2 = make_shared<dayLineValue>();
		double value = 0.0;
		const auto& pValue = vDayLineValue[index];
		auto date = pValue->lDate;
		pValue2->lDate = pValue->lDate;
		do {
			value += pValue->dTotalValue;
			index++;
		} while (index < vDayLineValue.size() - 1 && vDayLineValue[index + 1]->lDate == date);
		pValue2->dTotalValue = value;
	}
}

void CWorldMarket::RebuildStockDayLineDB() {
	gl_dataContainerFinnhubStock.ResetDayLine();
	gl_pFinnhubDataSource->SetUpdateStockProfile(true);
}

/// <summary>
/// Finnhub WebSocket的免费账户最多只能发送50个证券的数据
/// </summary>
vectorString CWorldMarket::GetFinnhubWebSocketSymbols() {
	vectorString vSymbol;

	vectorString vSymbolTemp = gl_dataContainerChosenFinnhubStock.GetSymbols();
	for (const auto& symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	vSymbolTemp = gl_dataContainerChosenWorldForex.GetSymbols();
	for (const auto& symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	vSymbolTemp = gl_dataContainerChosenWorldCrypto.GetSymbols();
	for (const auto& symbol : vSymbolTemp) {
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

/*
void CWorldMarket::ProcessFinnhubWebSocketData() {
	const auto total = gl_pFinnhubWebSocket->DataSize();
	size_t iTotalDataSize = 0;
	for (size_t i = 0; i < total; i++) {
		const auto pString = gl_pFinnhubWebSocket->PopData();
		string strMessage = "Finnhub: ");
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
		string strMessage = "Tiingo IEX: ");
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
		string strMessage = "Tiingo Crypto: ");
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
		string strMessage = "Tiingo Forex: ");
		strMessage += *pString;
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_pTiingoForexWebSocket->ParseTiingoForexWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoForexWebSocket(iTotalDataSize);
}*/

void CWorldMarket::ProcessFinnhubWebSocketData() {
	ProcessWebSocketDataGeneric(
		gl_pFinnhubWebSocket,
		"Finnhub: ",
		[](const size_t size) { gl_systemMessage.SetProcessedFinnhubWebSocket(size); });
}

void CWorldMarket::ProcessTiingoIEXWebSocketData() {
	ProcessWebSocketDataGeneric(
		gl_pTiingoIEXWebSocket,
		"Tiingo IEX: ",
		[](const size_t size) { gl_systemMessage.SetProcessedTiingoIEXWebSocket(size); }
	);
}

void CWorldMarket::ProcessTiingoCryptoWebSocketData() {
	ProcessWebSocketDataGeneric(
		gl_pTiingoCryptoWebSocket,
		"Tiingo Crypto: ",
		[](const size_t size) { gl_systemMessage.SetProcessedTiingoCryptoWebSocket(size); }
	);
}

void CWorldMarket::ProcessTiingoForexWebSocketData() {
	ProcessWebSocketDataGeneric(
		gl_pTiingoForexWebSocket,
		"Tiingo Forex: ",
		[](const size_t size) { gl_systemMessage.SetProcessedTiingoForexWebSocket(size); }
	);
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
	if (gl_dataContainerFinnhubStock.IsSymbol(pTiingoIEXbData->m_sSymbol)) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(pTiingoIEXbData->m_sSymbol);
		pStock->SetActive(true);
		pStock->SetNew(pTiingoIEXbData->m_dLastPrice * 1000);
	}
}

void CWorldMarket::UpdateFinnhubStockFromFinnhubSocket(const CFinnhubSocketPtr& pFinnhubData) {
	if (gl_dataContainerFinnhubStock.IsSymbol(pFinnhubData->m_sSymbol)) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(pFinnhubData->m_sSymbol);
		pStock->SetActive(true);
		pStock->SetNew(pFinnhubData->m_dLastPrice * 1000);
	}
}

void CWorldMarket::UpdateMarketStatus(const CMarketStatusesPtr& pv) const {
	for (const auto& p : *pv) {
		m_pvMarketStatus->push_back(p);
	}
}

void CWorldMarket::UpdateMarketHoliday(const CMarketHolidaysPtr& pv) const {
	for (const auto& p : *pv) {
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
	using namespace StockMarket;
	const auto& t = TiingoStockDayline{};

	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	db(remove_from(t).where(t.Symbol == pStock->GetSymbol()));
	tx.commit();
}

void CWorldMarket::DeleteTiingoFinancialStatement(const CTiingoStockPtr& pStock) {
	using namespace StockMarket;
	const auto& t = TiingoCompanyFinancialState{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	db(sqlpp::remove_from(t).where(t.Symbol == pStock->GetSymbol()));
	tx.commit();
}

bool CWorldMarket::IsTodayMarketEnded() {
	return false;
}
