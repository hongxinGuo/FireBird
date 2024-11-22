#include "WorldMarket.h"
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

#include"SetFinnhubStockDayLine.h"

#include "ChinaMarket.h"
#include "InfoReport.h"
#include "QuandlDataSource.h"
#include "ThreadStatus.h"
#include "TiingoDataSource.h"
#include "TiingoInaccessibleStock.h"
#include "TimeConvert.h"

CWorldMarket::CWorldMarket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE("CWorldMarket市场变量只允许存在一个实例\n");
	}

	m_strMarketId = _T("美国市场");

	//m_lMarketTimeZone = GetMarketLocalTimeOffset(_T("America/New_York")); // 美国股市使用美东标准时间
	m_lMarketTimeZone = 4 * 3600;
	m_lOpenMarketTime = 9 * 3600 + 1800; // 美国股市开市时间为九点三十分

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
}

void CWorldMarket::ResetQuandl() {
}

void CWorldMarket::ResetTiingo() const {
	if (IsEarlyThen(gl_tiingoInaccessibleStock.GetUpdateDate(), GetMarketDate(), 7)) {
		gl_tiingoInaccessibleStock.Clear(); // 不使用更新时间早于一周的数据。清除之，让系统自动查验新的状态。
		gl_tiingoInaccessibleStock.SetUpdateDate(GetMarketDate());
	}
}

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

	gl_dataContainerFinnhubStockExchange.LoadDB(); // 装入世界交易所信息
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

	CString str = _T("重置World Market于美东标准时间：");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);

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
		case WORLD_MARKET_CONNECT_FINNHUB_WEB_SOCKET__:
			ASSERT(!gl_systemConfiguration.IsUsingFinnhubWebSocket());
			gl_systemConfiguration.SetUsingFinnhubWebSocket(true); // 只设置标识，实际启动由其他任务完成。
			break;
		case WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__:
			gl_pTiingoDataSource->SetUpdateIEXTopOfBook(true); // 
			break;
		case WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__:
			gl_pWorldMarket->TaskCreateTiingoTradeDayDayLine(lCurrentTime);
			break;
		case WORLD_MARKET_TIINGO_PROCESS_DAYLINE__:
			gl_pWorldMarket->TaskProcessTiingoDayLine(lCurrentTime);
			break;
		case WORLD_MARKET_TIINGO_UPDATE_52WEEK_HIGH_LOW__:
			gl_pWorldMarket->TaskUpdateTiingoStock52WeekHighLowDB(lCurrentTime);
			break;
		case WORLD_MARKET_TIINGO_CALCULATE__:
			gl_pWorldMarket->TaskTiingoCalculate(lCurrentTime);
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
	return XferChinaMarketTimeToIndex(&m_tmMarket);
}

void CWorldMarket::TaskCreateTask(long lCurrentTime) {
	const long lTimeMinute = (lCurrentTime / 100) * 100; // 当前小时和分钟

	while (!IsMarketTaskEmpty()) DiscardCurrentMarketTask();

	// 系统初始化检查
	AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, 1);

	// 市场重置
	if (lCurrentTime < gl_systemConfiguration.GetWorldMarketResettingTime()) {
		AddTask(WORLD_MARKET_RESET__, gl_systemConfiguration.GetWorldMarketResettingTime()); // 默认执行时间为：170000
	}

	AddTask(WORLD_MARKET_UPDATE_DB__, lTimeMinute + 40);// 更新股票简介数据库的任务

	AddTask(WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__, lCurrentTime);

	AddTask(WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__, GetNextTime(lTimeMinute + 60, 0, 1, 0)); // 两分钟后开始监测WebSocket

	AddTask(WORLD_MARKET_TIINGO_PROCESS_DAYLINE__, GetNextTime(lTimeMinute, 0, 5, 0)); // 五分钟后处理日线数据

	AddTask(WORLD_MARKET_CREATE_TASK__, 240000); // 重启市场任务的任务于每日零时执行
}

void CWorldMarket::TaskProcessWebSocketData(long lCurrentTime) {
	ASSERT(!IsTimeToResetSystem(lCurrentTime));	// 下午五时重启系统，各数据库需要重新装入，故而此时不允许更新。

	ProcessWebSocketData();
	UpdateFinnhubStockFromWebSocket();

	long lNextTime = GetNextSecond(lCurrentTime);
	if (IsTimeToResetSystem(lNextTime)) lNextTime = GetResetTime() + 501;
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
		gl_runtime.thread_executor()->post([this] {
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateTiingoIndustry();
			gl_UpdateWorldMarketDB.release();
		});
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateSicIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// 更新tiingo stock profile与finnhub更新stock profile互斥
		gl_runtime.thread_executor()->post([this] {
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateSicIndustry();
			gl_UpdateWorldMarketDB.release();
		});
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateNaicsIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// 更新tiingo stock profile与finnhub更新stock profile互斥
		gl_runtime.thread_executor()->post([this] {
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateNaicsIndustry();
			gl_UpdateWorldMarketDB.release();
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
bool CWorldMarket::TaskUpdateTiingoStockDayLineDB() {
	bool fUpdated = false;
	CTiingoStockPtr pTiingoStock = nullptr;
	const size_t symbolSize = gl_dataContainerTiingoStock.Size();

	for (int i = 0; i < symbolSize; i++) {
		pTiingoStock = gl_dataContainerTiingoStock.GetStock(i);
		if (pTiingoStock->IsUpdateDayLineDBAndClearFlag()) {	// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			if (pTiingoStock->GetDayLineSize() > 0) {
				if (pTiingoStock->HaveNewDayLineData()) {
					gl_BackgroundWorkingThread.acquire();
					gl_runtime.thread_executor()->post([pTiingoStock] {
						gl_ThreadStatus.IncreaseBackGroundWorkingThread();
						if (gl_systemConfiguration.IsExitingSystem()) return;// 如果程序正在退出，则停止存储。
						pTiingoStock->UpdateDayLineDB();
						pTiingoStock->UpdateDayLineStartEndDate();
						pTiingoStock->SetUpdateProfileDB(true);
						pTiingoStock->UnloadDayLine();
						const CString str = pTiingoStock->GetSymbol() + _T("日线资料存储完成");
						gl_systemMessage.PushDayLineInfoMessage(str);
						gl_ThreadStatus.DecreaseBackGroundWorkingThread();
						gl_BackgroundWorkingThread.release();
					});
					fUpdated = true;
				}
				else pTiingoStock->UnloadDayLine(); // 当无需执行存储函数时，这里还要单独卸载日线数据。因存储日线数据线程稍后才执行，故而不能在此统一执行删除函数。
			}
		}
	}

	return (fUpdated);
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

	for (int i = 0; i < symbolSize; i++) {
		pSymbol = gl_dataFinnhubForexSymbol.GetSymbol(i);
		if (pSymbol->IsUpdateDayLineDBAndClearFlag()) {	// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					gl_runtime.thread_executor()->post([pSymbol] {
						gl_ThreadStatus.IncreaseBackGroundWorkingThread();
						gl_UpdateWorldMarketDB.acquire();
						if (!gl_systemConfiguration.IsExitingSystem()) {// 如果程序正在退出，则停止存储。
							pSymbol->UpdateDayLineDB();
							pSymbol->UpdateDayLineStartEndDate();
							pSymbol->SetUpdateProfileDB(true);
							pSymbol->UnloadDayLine();
							const CString str = pSymbol->GetSymbol() + _T("日线资料存储完成");
							gl_systemMessage.PushDayLineInfoMessage(str);
						}
						gl_UpdateWorldMarketDB.release();
						gl_ThreadStatus.DecreaseBackGroundWorkingThread();
					});
					fUpdated = true;
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
//
// 无论是否执行了存储函数，都需要将下载的日线历史数据删除，这样能够节省内存的占用。由于实际存储功能使用线程模式实现，
// 故而其执行时间可能晚于主线程，导致主线程删除日线数据时出现同步问题。解决的方法是让工作线程独立删除存储后的日线数据，
// 主线程的删除函数只在不调用工作线程（无需存储日线数据）的情况下方才执行。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskUpdateCryptoDayLineDB() {
	CString str;
	bool fUpdated = false;
	CFinnhubCryptoPtr pSymbol = nullptr;
	const size_t symbolSize = gl_dataFinnhubCryptoSymbol.Size();

	for (int i = 0; i < symbolSize; ++i) {
		pSymbol = gl_dataFinnhubCryptoSymbol.GetSymbol(i);
		if (pSymbol->IsUpdateDayLineDBAndClearFlag()) {	// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					gl_runtime.thread_executor()->post([pSymbol] {
						gl_ThreadStatus.IncreaseBackGroundWorkingThread();
						gl_UpdateWorldMarketDB.acquire();
						if (!gl_systemConfiguration.IsExitingSystem()) { // 如果程序正在退出，则停止存储。
							pSymbol->UpdateDayLineDB();
							pSymbol->UpdateDayLineStartEndDate();
							pSymbol->SetUpdateProfileDB(true);
							pSymbol->UnloadDayLine();
							const CString str2 = pSymbol->GetSymbol() + _T("日线资料存储完成");
							gl_systemMessage.PushDayLineInfoMessage(str2);
						}
						gl_UpdateWorldMarketDB.release();
						gl_ThreadStatus.DecreaseBackGroundWorkingThread();
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
					CString str1 = pSymbol->GetSymbol();
					str1 += _T(" 为未上市股票代码");
					gl_systemMessage.PushDayLineInfoMessage(str1);
				}
			}
		}
	}
	return fUpdated;
}

void CWorldMarket::TaskCreateTiingoTradeDayDayLine(long lCurrentTime) {
	if (gl_systemConfiguration.GetTiingoIEXTopOfBookUpdateDate() < gl_pWorldMarket->GetCurrentTradeDate()
		&& !gl_pTiingoDataSource->IsUpdateIEXTopOfBook()
		&& lCurrentTime > 180500) { // 当前交易日未处理过IEX、已经接收到数据且已过休市时间？
		gl_runtime.thread_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerTiingoStock.BuildDayLine(gl_pWorldMarket->GetCurrentTradeDate());
			gl_UpdateWorldMarketDB.release();
		});
	}
	else {
		AddTask(WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__, GetNextTime(lCurrentTime, 0, 1, 0)); // 一分钟后执行下一次
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理tiingo日线数据。
// 目前采用的方法尚未优化，导致处理所有股票的时间很长，故而决定暂时不使用此函数。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CWorldMarket::TaskProcessTiingoDayLine(long lCurrentTime) {
	if (gl_systemConfiguration.GetTiingoStockDayLineProcessedDate() >= GetCurrentTradeDate()) return; // 已更新完52WeekHighLow,不再自动更新。
	if (gl_systemConfiguration.IsPaidTypeTiingoAccount()) {
		if (!gl_pTiingoDataSource->IsUpdateDayLine()) { // 接收完日线数据后方可处理
			gl_runtime.thread_executor()->post([] {
				gl_dataContainerTiingoStock.TaskProcessDayLine();
			});
		}
		else {
			AddTask(WORLD_MARKET_TIINGO_PROCESS_DAYLINE__, GetNextTime(lCurrentTime, 0, 1, 0)); // 一分钟后执行下一次
		}
	}
	else {
		if (!gl_pTiingoDataSource->IsUpdateIEXTopOfBook()) { // 接收完IEX日线数据后方可处理
			gl_runtime.thread_executor()->post([] {
				gl_dataContainerTiingoStock.TaskProcessDayLine();
			});
		}
		else {
			AddTask(WORLD_MARKET_TIINGO_PROCESS_DAYLINE__, GetNextTime(lCurrentTime, 0, 1, 0)); // 一分钟后执行下一次
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CWorldMarket::TaskTiingoCalculate(long lCurrentTime) {
	gl_runtime.thread_executor()->post([] {
		gl_systemMessage.PushInnerSystemInformationMessage("calculateing 52 week low");
		gl_dataContainerTiingoStock.TaskCalculate();
		gl_systemMessage.PushInnerSystemInformationMessage("52 week low Calculated");
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
	CString str;
	const size_t stockSize = gl_dataContainerFinnhubStock.Size();

	CFinnhubStockPtr pStock = nullptr;
	for (long l = 0; l < stockSize; ++l) {
		pStock = gl_dataContainerFinnhubStock.GetStock(l);
		if (pStock->IsUpdateEPSSurpriseDBAndClearFlag()) {// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			pStock->UpdateEPSSurpriseDB();
		}
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}

	return (true);
}

void CWorldMarket::UpdateSECFilingsDB() {
	CString str;
	const size_t stockSize = gl_dataContainerFinnhubStock.Size();

	CFinnhubStockPtr pStock = nullptr;
	for (long l = 0; l < stockSize; ++l) {
		pStock = gl_dataContainerFinnhubStock.GetStock(l);
		if (pStock->IsUpdateSECFilingsDBAndClearFlag()) {// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			ASSERT(pStock->UpdateSECFilingsDB());
		}
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}
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
	if (!IsSystemReady()) {
		AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, GetNextSecond(lCurrentTime));
	}
	return IsSystemReady();
}

void CWorldMarket::TaskUpdateWorldMarketDB(long lCurrentTime) {
	if (gl_dataContainerFinnhubCountry.GetLastTotalCountry() < gl_dataContainerFinnhubCountry.GetTotalCountry()) { // 国家名称
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubCountry.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubForexExchange.IsNeedUpdate()) { // ForexExchange
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubForexExchange.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataFinnhubForexSymbol.IsUpdateProfileDB()) { // Forex symbol
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataFinnhubForexSymbol.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubCryptoExchange.IsNeedUpdate()) { // Crypto Exchange
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubCryptoExchange.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataFinnhubCryptoSymbol.IsUpdateProfileDB()) { // crypto symbol
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataFinnhubCryptoSymbol.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateInsiderTransactionDB()) { // Insider Transaction
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateInsiderTransactionDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateInsiderSentimentDB()) { // Insider Sentiment
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateInsiderSentimentDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateDayLineDB()) { // stock dayLine
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateFinnhubStockDayLineDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubEconomicCalendar.IsUpdateDB()) { // Economic Calendar
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubEconomicCalendar.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateCompanyNewsDB()) { // Company News
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateCompanyNewsDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateBasicFinancialDB()) { // Basic financial
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubStock.UpdateBasicFinancialDB(); // 此任务很费时，原因待查。目前先不使用此隔绝区
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateEPSSurpriseDB()) { // stock EPS surprise
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateEPSSurpriseDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateSECFilingsDB()) { // stock EPS surprise
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateSECFilingsDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}

	// Tiingo部分
	if (gl_dataContainerTiingoStock.IsUpdateProfileDB()) { // Tiingo Stock
		static int s_counter = 0;
		if (s_counter > 30) {
			gl_runtime.thread_executor()->post([] {
				gl_ThreadStatus.IncreaseBackGroundWorkingThread();
				gl_UpdateWorldMarketDB.acquire();
				gl_dataContainerTiingoStock.UpdateDB();
				gl_UpdateWorldMarketDB.release();
				gl_ThreadStatus.DecreaseBackGroundWorkingThread();
			});
			s_counter = 0;
		}
		s_counter++;
	}
	if (gl_dataContainerTiingoCryptoSymbol.IsUpdateProfileDB()) { // Tiingo crypto symbol
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerTiingoCryptoSymbol.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerTiingoFundamentalDefinition.IsUpdateDB()) { // Tiingo crypto symbol
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerTiingoFundamentalDefinition.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerTiingoStock.IsUpdateFinancialStateDB()) {
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerTiingoStock.UpdateFinancialStateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}

	if (gl_dataContainerTiingoStock.IsUpdateDayLineDB()) {
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->TaskUpdateTiingoStockDayLineDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}

	TaskUpdateForexDayLineDB(); // 这个函数内部继续生成工作线程
	TaskUpdateCryptoDayLineDB(); // 这个函数内部继续生成工作线程

	if (!gl_pFinnhubDataSource->IsUpdateSymbol() && gl_dataContainerFinnhubStock.IsUpdateProfileDB()) { // stock profile
		static int s_counter2 = 0;
		if (s_counter2 > 30) {
			gl_runtime.thread_executor()->post([] {
				gl_ThreadStatus.IncreaseBackGroundWorkingThread();
				gl_UpdateWorldMarketDB.acquire();
				gl_dataContainerFinnhubStock.UpdateProfileDB();
				gl_UpdateWorldMarketDB.release();
				gl_ThreadStatus.DecreaseBackGroundWorkingThread();
			});
			s_counter2 = 0;
		}
		s_counter2++;
	}

	if (gl_finnhubInaccessibleExchange.IsUpdateDB()) { // 更新禁止访问证券交易所名单
		gl_ThreadStatus.IncreaseBackGroundWorkingThread();
		gl_runtime.thread_executor()->post([] {
			gl_finnhubInaccessibleExchange.UpdateDB();
			gl_finnhubInaccessibleExchange.SetUpdateDB(false);
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}

	if (gl_tiingoInaccessibleStock.IsUpdateDB()) { // 更新禁止访问证券交易所名单
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_tiingoInaccessibleStock.UpdateDB();
			gl_tiingoInaccessibleStock.SetUpdateDB(false);
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}

	long lNextTime = GetNextTime(lCurrentTime, 0, 1, 0);
	if (IsTimeToResetSystem(lNextTime)) lNextTime = GetResetTime() + 510;
	ASSERT(!IsTimeToResetSystem(lNextTime));// 重启系统时各数据库需要重新装入，故而此时不允许更新数据库。
	AddTask(WORLD_MARKET_UPDATE_DB__, lNextTime); // 每五分钟更新一次
}

void CWorldMarket::TaskUpdateTiingoStock52WeekHighLowDB(long lCurrentTime) {
	if (gl_dataContainerTiingoStock.IsUpdate52WeekHighLowDB()) { // stock dayLine
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerTiingoStock.TaskUpdate52WeekHighLowDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
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

bool CWorldMarket::UpdateFinnhubStockDayLineDB() {
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
		pStock->UpdateDayLineDB();
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}
	return true;
}

bool CWorldMarket::UpdateCompanyNewsDB() {
	for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
		const auto pStock = gl_dataContainerFinnhubStock.GetStock(l);
		if (pStock->IsUpdateCompanyNewsDBAndClearFlag()) {// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			pStock->UpdateCompanyNewsDB();
		}
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}

	return (true);
}

bool CWorldMarket::UpdateInsiderTransactionDB() {
	int iCounter = 0;
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
		if (pStock->IsUpdateInsiderTransactionDBAndClearFlag()) {
			if (pStock->HaveInsiderTransaction()) {
				iCounter++;
				pStock->UpdateInsiderTransactionDB();
			}
		}
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}
	return true;
}

bool CWorldMarket::UpdateInsiderSentimentDB() {
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
		if (pStock->IsUpdateInsiderSentimentDBAndClearFlag()) {
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
		const CString strFilterPrefix = _T("[Symbol] = '");
		CSetFinnhubStockDayLine setFinnhubStockDayLine;

		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetStock(l);
			setFinnhubStockDayLine.m_strFilter = strFilterPrefix + pStock->GetSymbol() + _T("'");
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
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
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
	for (size_t i = 0; i < total; i++) {
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
	for (size_t i = 0; i < total; i++) {
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
void CWorldMarket::UpdateFinnhubStockFromWebSocket() {
	auto total = gl_SystemData.GetTiingoIEXSocketSize();
	for (size_t i = 0; i < total; i++) {
		const CTiingoIEXSocketPtr pIEXData = gl_SystemData.PopTiingoIEXSocket();
		UpdateFinnhubStockFromTiingoIEXSocket(pIEXData);
		gl_systemMessage.SetCurrentTiingoWebSocketIEX(pIEXData->m_sSymbol.c_str());
	}

	total = gl_SystemData.GetTiingoCryptoSocketSize();
	for (size_t i = 0; i < total; i++) {
		const CTiingoCryptoSocketPtr pCryptoData = gl_SystemData.PopTiingoCryptoSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketCrypto(pCryptoData->m_sSymbol.c_str());
	}

	total = gl_SystemData.GetTiingoForexSocketSize();
	for (size_t i = 0; i < total; i++) {
		const CTiingoForexSocketPtr pForexData = gl_SystemData.PopTiingoForexSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketForex(pForexData->m_sSymbol.c_str());
	}

	total = gl_SystemData.GetFinnhubSocketSize();
	for (size_t i = 0; i < total; i++) {
		const CFinnhubSocketPtr pFinnhubData = gl_SystemData.PopFinnhubSocket();
		UpdateFinnhubStockFromFinnhubSocket(pFinnhubData);
		gl_systemMessage.SetCurrentFinnhubWebSocketStake(pFinnhubData->m_sSymbol.c_str());
	}
}

void CWorldMarket::UpdateFinnhubStockFromTiingoIEXSocket(const CTiingoIEXSocketPtr& pTiingoIEXbData) {
	if (gl_dataContainerFinnhubStock.IsSymbol(pTiingoIEXbData->m_sSymbol.c_str())) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(pTiingoIEXbData->m_sSymbol.c_str());
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
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(pFinnhubData->m_sSymbol.c_str());
		pStock->SetActive(true);
		pStock->SetNew(pFinnhubData->m_dLastPrice * 1000);
	}
}

void CWorldMarket::UpdateMarketStatus(const CMarketStatussPtr& pv) const {
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
	while (gl_dataContainerTiingoDelistedSymbol.Size() > 0) {
		auto pTiingoDelistedStock = gl_dataContainerTiingoDelistedSymbol.GetStock(0);
		DeleteTiingoDayLine(pTiingoDelistedStock);
		DeleteTiingoFinancialStatement(pTiingoDelistedStock);
		ASSERT(gl_dataContainerTiingoStock.IsSymbol(pTiingoDelistedStock));
		gl_dataContainerTiingoStock.Delete(pTiingoDelistedStock->GetSymbol());
		gl_dataContainerTiingoDelistedSymbol.Delete(pTiingoDelistedStock);
	}
}

void CWorldMarket::DeleteTiingoDayLine(const CTiingoStockPtr& pStock) {
	CSetTiingoStockDayLine setDayLine;
	setDayLine.m_strFilter = _T("[Symbol] = '");
	setDayLine.m_strFilter += pStock->GetSymbol() + _T("'");

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
	setDayLine.m_strFilter += pStock->GetSymbol() + _T("'");

	setDayLine.Open();
	setDayLine.m_pDatabase->BeginTrans();
	while (!setDayLine.IsEOF()) {
		setDayLine.Delete();
		setDayLine.MoveNext();
	}
	setDayLine.m_pDatabase->CommitTrans();
	setDayLine.Close();
}
