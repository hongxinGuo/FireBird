#include"pch.h"

#include"ConvertToString.h"
#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"
#include"Thread.h"

#include"ThreadStatus.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

#include <fmt/format.h>

#include "CharSetTransfer.h"
#include "InfoReport.h"
#include"SetOption.h"
#include"SetChinaChosenStock.h"

#include"SetCurrentWeekLine.h"

#include "dataBaseConnector.h"
#include "NeteaseRTDataSource.h"
#include "SinaRTDataSource.h"
#include "TengxunDayLineDataSource.h"
#include "TengxunRTDataSource.h"

CChinaMarket::CChinaMarket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE(_T("ChinaMarket市场变量只允许存在一个实例\n"));
	}
	m_strMarketId = "SS";
	m_exchange = gl_dataContainerStockExchange.GetItem(m_strMarketId);
	ASSERT(m_exchange != nullptr);
	m_strLocalMarketTimeZone = "Asia/Shanghai";
	GetMarketLocalTimeOffset(m_strLocalMarketTimeZone);// 北京标准时间位于东八区， 中国股市开市时间为九点十五分

	m_fUsingSinaRTDataReceiver = true; // 使用新浪实时数据提取器
	m_fUsingTengxunRTDataReceiver = true; // 使用腾讯实时数据提取器
	m_fUsingNeteaseRTDataReceiver = false; // 不使用网易实时数据提取器

	m_avChosenStock.resize(30);

	m_fUpdateOptionDB = false;

	Reset();

	AddTask(CHINA_MARKET_CREATE_TASK__, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 全局变量的解析位于程序退出的最后，要晚于CMainFrame的解析。故而如果要想将系统退出的过程放在这里，需要研究。
// 目前不允许此析构函数完成任何功能。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CChinaMarket::~CChinaMarket() {
	if (gl_pCurrentStock == nullptr) {
		if (!gl_systemConfiguration.GetCurrentStock().empty()) {
			gl_systemConfiguration.SetCurrentStock("");
			gl_systemConfiguration.SetUpdateDB(true);
		}
	}
	else if (gl_pCurrentStock->GetSymbol() != gl_systemConfiguration.GetCurrentStock()) {
		gl_systemConfiguration.SetCurrentStock(gl_pCurrentStock->GetSymbol());
		gl_systemConfiguration.SetUpdateDB(true);
	}
}

void CChinaMarket::ResetMarket() {
	m_fResettingMarket = true;
	string s = "重置中国股市于北京标准时间：" + GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(s);
	gl_ProcessChinaMarketRTData.acquire();
	while (gl_ThreadStatus.IsSavingThreadRunning()) { Sleep(1); }

	Reset();

	gl_dataContainerChinaStock.LoadProfileDB();
	LoadOptionDB();
	LoadChosenStockDB();

	if (!gl_systemConfiguration.GetCurrentStock().empty()) {
		AddImmediateTask(CHINA_MARKET_UPDATE_CURRENT_STOCK__);
	}

	gl_ProcessChinaMarketRTData.release();
	m_fResettingMarket = false;
}

void CChinaMarket::Reset() {
	CalculateTime(); // 初始化市场时间
	SetSystemReady(false); // 市场初始状态为未设置好。

	gl_dataContainerChinaStock.Reset();
	gl_dataContainerChinaStockSymbol.Reset();

	m_lCurrentSelectedPosition = 0;
	m_lCurrentRSStrongIndex = 0;
	m_lCurrentSelectedStockSet = -1; // 选择使用全体股票集、
	m_fChosen10RSStrong1StockSet = false;
	m_fChosen10RSStrongStockSet = false;
	m_fCalculateChosen10RS = false;

	m_iCurrentDayRTDataCounter = 0;
	m_llRTDataReceived = 0;
	m_lRTDataReceivedInCurrentMinute = 0;
	m_lNewRTDataReceivedInCurrentMinute = 0;

	gl_SystemData.ClearDataQueue();

	m_RTDataNeedCalculate = false;

	m_fFastReceivingRTData = true;
	m_fRTDataSetCleared = false;
	m_fUpdateTempDataDB = true;

	m_tpNewTransactionTime = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(0));

	m_iCountDownTengxunNumber = 10;

	m_lLastLoginTime = 0;

	m_fSelectedStockLoaded = false;

	m_lTotalMarketBuy = m_lTotalMarketSell = 0;

	m_fCheckActiveStock = true; //检查当日活跃股票，必须为真。
	m_fTodayTempDataLoaded = false;

	m_fMarketOpened = false;

	m_fUpdateChosenStockDB = false;

	for (const auto& pDataSource : m_vDataSource) {
		pDataSource->Reset();
	}
}

void CChinaMarket::PrepareToCloseMarket() {
	ASSERT(gl_systemConfiguration.IsExitingSystem());
	// do nothing
}

bool CChinaMarket::IsTimeToResetSystem(long lCurrentTime) {
	if (((lCurrentTime > 91259) && (lCurrentTime < 91401)) || ((lCurrentTime > 92459) && (lCurrentTime < 92701))) return true;
	return false;
}

bool CChinaMarket::IsOrdinaryTradeTime(long lTime) {
	if (!IsWorkingDay()) return false;
	if (lTime < 93000) return false;
	if (lTime > 113000 && lTime < 130000) return false;
	if (lTime > 150000) return false;
	return true;
}

bool CChinaMarket::IsWorkingTime(long lTime) {
	if (!IsWorkingDay()) return false;
	if (lTime < 91200) return false;
	if (lTime > 114500 && lTime < 124500) return false;
	if (lTime > 150630) return false;
	return true;
}

int CChinaMarket::XferMarketTimeToIndex() {
	return XferChinaMarketTimeToIndex(GetMarketTM());
}

///////////////////////////////////////////////////////////////////////////////////////
//
// 各任务调度处理函数
//
// todo 采用concurrencpp::timer后，可以将此函数的各项任务分配为定时器所驱动的工作线程任务。
//
//
///////////////////////////////////////////////////////////////////////////////////////
int CChinaMarket::ProcessTask(long lCurrentTime) {
	if (IsMarketTaskEmpty()) return false;
	const auto pTask = GetMarketTask();
	if (lCurrentTime >= pTask->GetTime()) { // time to executive?
		DiscardCurrentMarketTask();
		switch (pTask->GetType()) {
		case CHINA_MARKET_CREATE_TASK__: // 生成其他任务
			TaskCreateTask(lCurrentTime);
			break;
		case RELOAD_SYSTEM__: // 重启系统？
			ReportSchedulingExitToWatchdog();
			TaskExitSystem();
			break;
		case CHINA_MARKET_CHECK_SYSTEM_READY__:
			TaskCheckMarketReady(lCurrentTime);
			break;
		case CHINA_MARKET_RESET__: // 市场重置
			TaskResetMarket(lCurrentTime);
			break;
		case CHINA_MARKET_CHECK_SYSTEM: // 系统检查
			TaskCheckSystem(lCurrentTime);
			break;
		case CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__:
			TaskPerSecond(lCurrentTime);
			TaskDistributeAndCalculateRTData(lCurrentTime);
			break;
		case CHINA_MARKET_BUILD_TODAY_DATABASE__:
			TaskProcessTodayStock(lCurrentTime);
			break;
		case CHINA_MARKET_VALIDATE_TODAY_DATABASE__:
			//not implemented
			break;
		case CHINA_MARKET_UPDATE_OPTION_DB__:
			TaskUpdateOptionDB(lCurrentTime);
			break;
		case CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__:
			TaskUpdateStockProfileDB(lCurrentTime);
			break;
		case CHINA_MARKET_UPDATE_STOCK_SECTION__:
			TaskUpdateStockSection();
			break;
		case CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__:
			TaskProcessAndSaveDayLine(lCurrentTime);
			break;
		case CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__:
			TaskAccessoryPerMinuteTask(lCurrentTime);
			break;
		case CHINA_MARKET_PREPARING_MARKET_OPEN__:
			TaskPreparingMarketOpen(lCurrentTime);
			break;
		default:
			ASSERT(0); // 错误的任务号
			break;
		}
		return pTask->GetType();
	}
	return 0;
}

int CChinaMarket::ProcessCurrentImmediateTask(long lMarketTime) {
	ASSERT(!m_marketImmediateTask.Empty());

	auto pTask = m_marketImmediateTask.GetTask();
	auto taskType = pTask->GetType();
	m_marketImmediateTask.DiscardCurrentTask();
	switch (taskType) {
	case CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__:
		TaskUpdateChosenStockDB();
		break;
	case CHINA_MARKET_UPDATE_CURRENT_STOCK__: // 
		TaskSetCurrentStock();
		break;
	default:
		ASSERT(0); // 错误的任务号
		break;
	}
	return pTask->GetType();
}

bool CChinaMarket::TaskCheckMarketReady(long lCurrentTime) {
	if (!IsSystemReady()) {
		const auto lMax = gl_dataContainerChinaStock.Size() > 12000 ? gl_dataContainerChinaStock.Size() * 2 : 24000;
		if (m_llRTDataReceived > lMax) {
			SetSystemReady(true);
			gl_systemMessage.PushInformationMessage("中国股票市场初始化完毕");
		}
	}
	if (!IsSystemReady()) AddTask(CHINA_MARKET_CHECK_SYSTEM_READY__, GetNextSecond(lCurrentTime));

	return IsSystemReady();
}

void CChinaMarket::ChangeToNextStock() {
	ASSERT(gl_pCurrentStock != nullptr);
	size_t lIndex = gl_dataContainerChinaStock.GetOffset(gl_pCurrentStock);
	CChinaStockPtr pStock = dynamic_pointer_cast<CChinaStock>(gl_pCurrentStock);

	if (IsTotalStockSetSelected()) {
		bool fFound = false;
		while (!fFound) {
			if (++lIndex == gl_dataContainerChinaStock.Size()) {
				lIndex = 0;
			}
			pStock = gl_dataContainerChinaStock.GetStock(lIndex);
			if (!pStock->IsNullStock()) fFound = true;
		}
	}
	else {
		ASSERT(!m_avChosenStock.at(m_lCurrentSelectedStockSet).empty()); //
		if (m_lCurrentSelectedPosition >= (m_avChosenStock.at(m_lCurrentSelectedStockSet).size() - 1)) {
			m_lCurrentSelectedPosition = 0;
			pStock = m_avChosenStock.at(m_lCurrentSelectedStockSet).at(m_lCurrentSelectedPosition);
		}
		else {
			m_lCurrentSelectedPosition++;
			pStock = m_avChosenStock.at(m_lCurrentSelectedStockSet).at(m_lCurrentSelectedPosition);
		}
	}
	pStock->SetSelected(true);
	gl_pCurrentStock = pStock;
}

void CChinaMarket::ChangeToPrevStock() {
	ASSERT(gl_pCurrentStock != nullptr);
	size_t lIndex = gl_dataContainerChinaStock.GetOffset(gl_pCurrentStock);
	CChinaStockPtr pStock = dynamic_pointer_cast<CChinaStock>(gl_pCurrentStock);

	if (IsTotalStockSetSelected()) {
		bool fFound = false;
		while (!fFound) {
			if (lIndex == 0) {
				lIndex = gl_dataContainerChinaStock.Size() - 1;
			}
			else lIndex--;
			pStock = gl_dataContainerChinaStock.GetStock(lIndex);
			if (!pStock->IsNullStock()) fFound = true;
		}
	}
	else {
		ASSERT(!m_avChosenStock.at(m_lCurrentSelectedStockSet).empty()); //
		if (m_lCurrentSelectedPosition == 0) {
			m_lCurrentSelectedPosition = m_avChosenStock.at(m_lCurrentSelectedStockSet).size() - 1;
			pStock = m_avChosenStock.at(m_lCurrentSelectedStockSet).at(m_lCurrentSelectedPosition);
		}
		else {
			m_lCurrentSelectedPosition--;
			pStock = m_avChosenStock.at(m_lCurrentSelectedStockSet).at(m_lCurrentSelectedPosition);
		}
	}
	gl_pCurrentStock = pStock;
}

size_t CChinaMarket::GetCurrentStockSetSize() const {
	if (IsTotalStockSetSelected()) return gl_dataContainerChinaStock.Size();
	return m_avChosenStock.at(m_lCurrentSelectedStockSet).size();
}

void CChinaMarket::CreateStock(const string& strStockCode, const string& strStockName, const bool fProcessRTData) {
	const auto pStock = make_shared<CChinaStock>();
	pStock->SetTodayNewStock(true);
	pStock->SetSymbol(strStockCode);
	pStock->SetDisplaySymbol(strStockName);
	pStock->SetIPOStatus(_STOCK_NOT_CHECKED_);
	pStock->SetDayLineEndDate(CHINA_MARKET_BEGIN_DATE_);
	pStock->SetDayLineStartDate(CHINA_MARKET_BEGIN_DATE_);
	pStock->SetUpdateProfileDB(true);
	pStock->SetNeedProcessRTData(fProcessRTData);
	gl_dataContainerChinaStock.Add(pStock);
	ASSERT(pStock->IsUpdateDayLine());
	string str = "china Market生成新代码";
	str += pStock->GetSymbol();
	gl_systemMessage.PushInnerSystemInformationMessage(str);
}

size_t CChinaMarket::IncreaseStockInquiringIndex(size_t& lIndex, size_t lEndPosition) {
	if (++lIndex >= lEndPosition) { lIndex = 0; }
	return lIndex;
}

/////////////////////////////////////////////////////////////////////////
//
//	得到分时线偏移量。09:30为0，15:00为240,步长为1分钟
//
//
////////////////////////////////////////////////////////////////////////
long CChinaMarket::GetMinLineOffset(time_t tUTC) const {
	ASSERT(tUTC >= 0);
	tm tmMarketTime;

	GetMarketTimeStruct(&tmMarketTime, tUTC);
	tmMarketTime.tm_hour = 9;
	tmMarketTime.tm_min = 30;
	tmMarketTime.tm_sec = 0;
	const time_t tUTC2 = TransferToUTCTime(&tmMarketTime);
	long lIndex = (tUTC - tUTC2) / 60;
	lIndex = std::max<long>(lIndex, 0);
	if ((lIndex >= 120) && (lIndex < 209)) lIndex = 119;
	if (lIndex >= 210) lIndex -= 90;
	if (lIndex >= 240) lIndex = 239;

	ASSERT((lIndex >= 0) && (lIndex < 240));
	return lIndex;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 处理实时数据等，至少每三秒执行一次。
// 将实时数据暂存队列中的数据分别存放到各自股票的实时队列中。
// 分发数据时，只分发新的（交易时间晚于之前数据的）实时数据。
//
// 此函数由工作线程调用，注意同步问题。
//
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::DistributeRTDataToStock(const CWebRTDataPtr& pRTData) {
	const string strSymbol = pRTData->GetSymbol();
	if (IsCheckingActiveStock()) {
		if (!gl_dataContainerChinaStock.IsSymbol(strSymbol) && pRTData->HaveName()) {
			//Note： 有股票代码和名称，则该股票已经上市了，现状（退市等）不明。
			ASSERT(strSymbol.length() == 9);
			CreateStock(strSymbol, pRTData->GetStockName(), true);
		}
	}
	else if (!gl_dataContainerChinaStock.IsSymbol(strSymbol)) {
		return false;
	}
	if (pRTData->IsActive()) { // 此实时数据有效？
		IncreaseRTDataReceived();
		if (m_tpNewTransactionTime < pRTData->GetTimePoint()) {
			m_tpNewTransactionTime = pRTData->GetTimePoint();
		}
		const auto pStock = gl_dataContainerChinaStock.GetStock(pRTData->GetSymbol());
		if (!pStock->IsActive()) {
			if (pRTData->IsValidTime(14)) {
				pStock->UpdateStatus(pRTData);
			}
		}
		if (pRTData->GetTimePoint() > pStock->GetTimePoint()) {// 新的数据？
			m_lNewRTDataReceivedInCurrentMinute++;
			pStock->PushRTData(pRTData); // 存储新的数据至数据池
			pStock->SetTransactionTime(pRTData->GetTransactionTime());
			pStock->SetTimePoint(pRTData->GetTimePoint()); // 设置最新接受到实时数据的时间
		}
	}
	return true;
}

bool CChinaMarket::IsRealTimeDataSourceEnable() noexcept {
	switch (gl_systemConfiguration.GetChinaMarketRealtimeServer()) {
	case SinaRealTime_:
		return gl_pSinaRTDataSource->IsEnable();
	case NeteaseRealTime_:
		return gl_pNeteaseRTDataSource->IsEnable();
	case TengxunRealTime_:
		return gl_pTengxunRTDataSource->IsEnable();
	default:
		ASSERT(0);
		return true;
	}
}

void CChinaMarket::EnableRealTimeDataSource(bool fEnable) noexcept {
	switch (gl_systemConfiguration.GetChinaMarketRealtimeServer()) {
	case SinaRealTime_:
		gl_pSinaRTDataSource->Enable(fEnable);
		break;
	case NeteaseRealTime_:
		gl_pNeteaseRTDataSource->Enable(fEnable);
		break;
	case TengxunRealTime_:
		gl_pTengxunRTDataSource->Enable(fEnable);
		break;
	default:
		ASSERT(0);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 生成每次查询新浪实时股票数据的字符串
//
//////////////////////////////////////////////////////////////////////////////////////////
string CChinaMarket::GetSinaStockInquiringStr(long lTotalNumber, bool fUsingTotalStockSet) {
	if (fUsingTotalStockSet) {
		return gl_dataContainerChinaStockSymbol.GetNextSinaStockInquiringMiddleStr(lTotalNumber);
	}
	return gl_dataContainerChinaStock.GetNextSinaStockInquiringMiddleStr(lTotalNumber);
}

string CChinaMarket::GetNeteaseStockInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet) {
	if (fUsingTotalStockSet) {
		return gl_dataContainerChinaStockSymbol.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber);
	}
	return gl_dataContainerChinaStock.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber);
}

bool CChinaMarket::CheckValidOfNeteaseDayLineInquiringStr(const string& str) const {
	const string strNetease = str.substr(0, 7);
	string strStockCode = XferNeteaseToStandard(strNetease);
	if (!gl_dataContainerChinaStock.IsSymbol(strStockCode)) {
		string strReport = "网易日线查询股票代码错误：";
		TRACE(_T("网易日线查询股票代码错误：%s\n"), Utf8ToWstring(strStockCode).c_str());
		strReport += strStockCode;
		gl_systemMessage.PushInnerSystemInformationMessage(strReport);
		return false;
	}

	return true;
}

void CChinaMarket::TaskSetCurrentStock() {
	if (!gl_systemConfiguration.GetCurrentStock().empty()) { // 当前有选择股票
		if (gl_dataContainerChinaStock.IsSymbol(gl_systemConfiguration.GetCurrentStock())) {
			auto pStock = gl_dataContainerChinaStock.GetStock(gl_systemConfiguration.GetCurrentStock());
			pStock->SetSelected(true);
			gl_pCurrentStock = pStock;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将接收到的实时数据分发至各相关股票的实时数据队列中。
// 由于有多个数据源，故而需要等待各数据源都执行一次后，方可以分发至相关股票处，需要至少每三秒执行一次，以保证各数据源至少都能提供一次数据。
// 实时数据的计算过程必须位于分配过程之后，这样才能保证不会出现数据同步问题。
//
// 使用线程池改写。
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CChinaMarket::TaskDistributeAndCalculateRTData(long lCurrentTime) {
	gl_runtime.thread_pool_executor()->post([this] { // 无需等待结果，直接返回
			gl_ProcessChinaMarketRTData.acquire();
			auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());

			this->DistributeRTData();
			this->CalculateRTData();

			auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			this->SetDistributeAndCalculateTime((end - start).count());
			gl_ProcessChinaMarketRTData.release();
		});

	AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, GetNextSecond(lCurrentTime)); // 每秒执行一次
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 处理实时数据等，至少每三秒执行一次。
// 将实时数据暂存队列中的数据分别存放到各自股票的实时队列中。
// 分发数据时，只分发新的（交易时间晚于之前数据的）实时数据。
//
// 此函数由工作线程调用，注意同步问题。
// 由于新浪实时数据可能由多个数据申请线程申请，执行此函数时不允许同时将实时数据加入队列中，故而采用并行队列来存储。
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChinaMarket::DistributeRTData() {
	CWebRTDataPtr pRTData;
	bool succeed = gl_qChinaMarketRTData.try_dequeue(pRTData);
	const bool queueNotEmpty = succeed;
	while (succeed) {
		DistributeRTDataToStock(pRTData);
		pRTData = nullptr;
		m_lRTDataReceivedInCurrentMinute++;
		succeed = gl_qChinaMarketRTData.try_dequeue(pRTData);
	}
	pRTData = nullptr;
	if (queueNotEmpty) SetRTDataNeedCalculate(true); // 设置接收到实时数据标识
}

void CChinaMarket::CalculateRTData() {
	if (IsSystemReady() && IsRTDataNeedCalculate()) {
		gl_dataContainerChinaStock.ProcessRTData();
		SetRTDataNeedCalculate(false);
	}
}

void CChinaMarket::TaskCreateTask(long lCurrentTime) {
	const long lTimeMinute = (lCurrentTime / 100) * 100; // 当前小时和分钟

	while (!IsMarketTaskEmpty()) DiscardCurrentMarketTask();

	// 系统初始化检查
	AddTask(CHINA_MARKET_CHECK_SYSTEM_READY__, 1);
	// 每秒一次分配实时数据
	AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, 1); // 开始执行时间为：1

	// 辅助任务。在随后的正点分钟执行。
	AddTask(CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__, GetNextTime(lTimeMinute, 0, 1, 0));

	// 市场重置
	if (lCurrentTime < 91300) {
		AddTask(CHINA_MARKET_RESET__, 91300); // 执行时间为：91300
	}

	// 第一次系统检查
	if (lCurrentTime < 91800) {
		AddTask(CHINA_MARKET_CHECK_SYSTEM, 91800); // 执行时间为：91800
	}

	// 准备开市任务。每日执行一次，于92959执行。
	if (lCurrentTime < 92959) {
		AddTask(CHINA_MARKET_PREPARING_MARKET_OPEN__, 92959);
	}

	// 每十秒钟存储一次日线历史数据。
	AddTask(CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, 93000); // 中午休市时开始更新日线历史数据。

	// 每五分钟存储一次系统选项数据库
	AddTask(CHINA_MARKET_UPDATE_OPTION_DB__, GetNextTime(lTimeMinute, 0, 3, 5)); // 开始执行时间为启动之后的三分钟。

	// 每五分钟存储一次股票简要数据库
	AddTask(CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__, GetNextTime(lTimeMinute, 0, 4, 10)); // 开始执行时间为启动之后的四分钟。

	if (IsWorkingDay()) {
		if (lCurrentTime < 150300) {
			// 生成本日历史数据
			AddTask(CHINA_MARKET_BUILD_TODAY_DATABASE__, 150530); // 开始执行时间为：150530
		}
		else {
			AddTask(CHINA_MARKET_VALIDATE_TODAY_DATABASE__, 151000); // 检查今日数据完整性
		}
	}

	// 如果设定为周期性重启系统，则在星期天晚上9时重启。
	if (gl_systemConfiguration.IsReloadSystem() && (GetDayOfWeek() == 0) && (lCurrentTime < 210000)) {
		AddTask(RELOAD_SYSTEM__, 210000);
	}

	AddTask(CHINA_MARKET_CREATE_TASK__, 240000); // 重启市场任务的任务于每日零时执行
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Note 系统关闭时要执行一系列关闭系统前的准备工作，不允许使用exit(0)函数或PostQuitMessage()直接退出系统，
/// 故而采用向主框架窗口发送关闭窗口系统消息（WM_SYSCOMMAND SC_CLOSE）的方法。
///
/// 本函数只是发出关闭系统的消息，系统关闭由关闭函数执行。系统重新载入由Watchdog监控程序完成。
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CChinaMarket::TaskExitSystem() {
	// 向主窗口发送关闭窗口系统消息，通知框架窗口执行关闭任务。
	// 由于系统需要顺序关闭各项任务，故而不允许直接退出系统。
	PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), WM_SYSCOMMAND, SC_CLOSE, 0);
}

void CChinaMarket::TaskPerSecond(long lCurrentTime) {
	IsWebBusy();
}

void CChinaMarket::TaskAccessoryPerMinuteTask(long lCurrentTime) {
	CheckFastReceivingData(lCurrentTime);
	CheckMarketOpen(lCurrentTime);// 判断中国股票市场开市状态
	SetCheckActiveStockFlag(lCurrentTime);
	ResetEffectiveRTDataRatio(); // 重置有效实时数据比率

	if (gl_systemConfiguration.IsUpdateDB()) { // 每分钟检查一次系统配置是否需要存储。
		gl_runtime.thread_executor()->post([] {
			gl_systemConfiguration.UpdateDB();
		});
		gl_systemConfiguration.SetUpdateDB(false);
	}

	AddTask(CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__, GetNextTime(lCurrentTime, 0, 1, 0)); // 每分钟整点执行一次
}

void CChinaMarket::TaskPreparingMarketOpen(long lCurrentTime) {
	ASSERT(lCurrentTime == 92959); // 每日执行一次
	// 目前尚未有需执行的任务
}

bool CChinaMarket::AddChosenStock(const CChinaStockPtr& pStock) {
	if (std::ranges::count(m_avChosenStock.at(0).begin(), m_avChosenStock.at(0).end(), pStock) == 0) {
		m_avChosenStock.at(0).push_back(pStock);
		ASSERT(!pStock->IsUpdateChosenStockDB());
		return true;
	}
	return false;
}

bool CChinaMarket::DeleteChosenStock(const CChinaStockPtr& pStock) {
	const auto it = std::ranges::find(m_avChosenStock.at(0).cbegin(), m_avChosenStock.at(0).cend(), pStock);
	if (it == m_avChosenStock.at(0).end()) { return false; }
	(*it)->SetChosen(false);
	(*it)->SetUpdateChosenStockDB(false);
	m_avChosenStock.at(0).erase(it);
	return true;
}

bool CChinaMarket::SetCheckActiveStockFlag(long lCurrentTime) {
	if (!IsSystemReady()) {
		m_fCheckActiveStock = true;
		return true;
	}
	if (((lCurrentTime > 91459) && (lCurrentTime < 92700))
		|| ((lCurrentTime > 113259) && (lCurrentTime < 125900))
		|| (lCurrentTime > 150300)) {
		m_fCheckActiveStock = true;
		return true;
	}
	m_fCheckActiveStock = false;
	return false;
}

bool CChinaMarket::TaskProcessTodayStock(long lCurrentTime) {
	if (IsSystemReady()) {
		gl_runtime.thread_executor()->post([this] {
			gl_UpdateChinaMarketDB.acquire();
			gl_systemMessage.SetChinaMarketSavingFunction("process today stock");
			this->ProcessTodayStock();
			gl_UpdateChinaMarketDB.release();
		});
		return true;
	}
	return false;
}

void CChinaMarket::ProcessTodayStock() {
	ASSERT(IsSystemReady()); // 调用本工作线程时必须设置好市场。

	const long lDate = GetMarketDate(GetTransactionTime());
	if (lDate == GetMarketDate()) {
		gl_dataContainerChinaStock.BuildDayLine(lDate);
		// 生成周线数据
		BuildCurrentWeekLine();
		gl_dataContainerChinaStock.UpdateProfileDB();
		if (GetMarketTime() > 150400) {	// 如果中国股市闭市了
			SetUpdateOptionDB(true); // 更新状态
		}
	}
	string s = ConvertDateToChineseTimeStampString(lDate) + "的实时数据处理完毕";
	gl_systemMessage.PushInformationMessage(s);
}

bool CChinaMarket::IsSavingDayLineDBTaskFinished() {
	static atomic_bool s_bTaskOfSavingDayLineFinished = false;
	if (s_bTaskOfSavingDayLineFinished) {
		if ((!gl_dataContainerChinaStock.IsUpdateDayLineDB()) && (!gl_dataContainerChinaStock.IsUpdateDayLine()) && (!IsDayLineNeedProcess())) {
			s_bTaskOfSavingDayLineFinished = false;
			const string str = "中国市场日线历史数据更新完毕";
			gl_systemMessage.PushInformationMessage(str);
			if (gl_dataContainerChinaStock.IsDayLineDBUpdated()) { // 更新股票池数据库
				gl_dataContainerChinaStock.ClearDayLineDBUpdatedFlag();
			}
			return true;
		}
	}
	else {
		if (gl_dataContainerChinaStock.IsUpdateDayLine() || IsDayLineNeedProcess() || gl_dataContainerChinaStock.IsUpdateDayLineDB()) {
			s_bTaskOfSavingDayLineFinished = true;
		}
	}
	return false;
}

bool CChinaMarket::CheckFastReceivingData(long lCurrentTime) {
	if (gl_systemConfiguration.IsFastInquiringRTData()) {
		m_fFastReceivingRTData = true;
	}
	else {
		m_fFastReceivingRTData = IsWorkingTime(lCurrentTime);
	}

	return m_fFastReceivingRTData;
}

bool CChinaMarket::IsWebBusy() {
	static std::atomic_bool s_bWebBusy = false;
	bool bWebBusy = false;
	switch (gl_systemConfiguration.GetChinaMarketRealtimeServer()) {
	case SinaRealTime_: // 新浪实时数据
		bWebBusy = gl_pSinaRTDataSource->IsWebBusy();
		break;
	case NeteaseRealTime_: // 更新网易实时数据读取时间
		bWebBusy = gl_pNeteaseRTDataSource->IsWebBusy();
		break;
	case TengxunRealTime_: // 更新腾讯实时数据读取时间
		bWebBusy = gl_pTengxunRTDataSource->IsWebBusy();
		break;
	default: // error
		ASSERT(0);
		break;
	}
	if (bWebBusy) {
		if (!s_bWebBusy) {
			gl_dailyLogger->warn("Web busy");
		}
	}
	else {
		if (s_bWebBusy) {
			gl_dailyLogger->info("Web busy resolved");
		}
	}
	s_bWebBusy = bWebBusy;
	return bWebBusy;
}

long long CChinaMarket::GetHTTPStatus() {
	long long httpStatus = 200;
	switch (gl_systemConfiguration.GetChinaMarketRealtimeServer()) {
	case SinaRealTime_: // 新浪实时数据
		httpStatus = gl_pSinaRTDataSource->GetHTTPStatusCode();
		break;
	case NeteaseRealTime_: // 更新网易实时数据读取时间
		httpStatus = gl_pNeteaseRTDataSource->GetHTTPStatusCode();
		break;
	case TengxunRealTime_: // 更新腾讯实时数据读取时间
		httpStatus = gl_pTengxunRTDataSource->GetHTTPStatusCode();
		break;
	default: // error
		ASSERT(0);
		break;
	}
	return httpStatus;
}

bool CChinaMarket::IsWebError() {
	bool webError = false;
	switch (gl_systemConfiguration.GetChinaMarketRealtimeServer()) {
	case SinaRealTime_: // 新浪实时数据
		webError = gl_pSinaRTDataSource->IsWebError();
		break;
	case NeteaseRealTime_: // 更新网易实时数据读取时间
		webError = gl_pNeteaseRTDataSource->IsWebError();
		break;
	case TengxunRealTime_: // 更新腾讯实时数据读取时间
		webError = gl_pTengxunRTDataSource->IsWebError();
		break;
	default: // error
		ASSERT(0);
		break;
	}
	return webError;
}

long long CChinaMarket::GetWebErrorCode() {
	long long errorCode = 0;
	switch (gl_systemConfiguration.GetChinaMarketRealtimeServer()) {
	case SinaRealTime_: // 新浪实时数据
		errorCode = gl_pSinaRTDataSource->GetWebErrorCode();
		break;
	case NeteaseRealTime_: // 更新网易实时数据读取时间
		errorCode = gl_pNeteaseRTDataSource->GetWebErrorCode();
		break;
	case TengxunRealTime_: // 更新腾讯实时数据读取时间
		errorCode = gl_pTengxunRTDataSource->GetWebErrorCode();
		break;
	default: // error
		ASSERT(0);
		break;
	}
	return errorCode;
}

bool CChinaMarket::CheckMarketOpen(long lCurrentTime) {
	if (IsWorkingDay() && (lCurrentTime > 92759 && lCurrentTime < 150600)) {	// 市场结束接收数据的时间，皆定为150600（与停止存储临时数据的时间一样）
		m_fMarketOpened = true;
	}
	else m_fMarketOpened = false;

	return m_fMarketOpened;
}

bool CChinaMarket::TaskResetMarket(long lCurrentTime) {
	// 九点十三分重启系统。 必须在此时间段内重启，如果更早的话容易出现数据不全的问题。
	SetSystemReady(false);
	AddTask(CHINA_MARKET_CHECK_SYSTEM_READY__, lCurrentTime); // 每次重置系统时，必须进行系统初始化状态检查
	AddTask(CHINA_MARKET_UPDATE_STOCK_SECTION__, GetNextTime(lCurrentTime, 0, 5, 0)); // 五分钟后再更新此数据库
	ResetMarket();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 系统检查。如果发现有不满意的地方则重新启动系统。
// 目前如果发现新浪实时数据接收时间超过300毫秒则重启系统。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskCheckSystem(long lCurrentTime) {
	if ((gl_systemConfiguration.IsUsingSinaRTServer() && gl_pSinaRTDataSource->GetCurrentInquiryTime() > 300)
		|| (gl_systemConfiguration.IsUsingTengxunRTServer() && gl_pTengxunRTDataSource->GetCurrentInquiryTime() > 300)) { // 如果接受网络数据时间超过300毫秒
		AddTask(RELOAD_SYSTEM__, GetNextTime(lCurrentTime, 0, 0, 10)); // 十秒后重启系统
	}
	return true;
}

bool CChinaMarket::TaskUpdateStockProfileDB(long lCurrentTime) {
	AddTask(CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__, GetNextTime(lCurrentTime, 0, 5, 0));

	if (gl_dataContainerChinaStock.IsUpdateProfileDB()) {
		gl_runtime.thread_executor()->post([] {
			gl_UpdateChinaMarketDB.acquire();
			gl_systemMessage.SetChinaMarketSavingFunction("update stock profile");
			gl_dataContainerChinaStock.UpdateProfileDB();
			gl_UpdateChinaMarketDB.release();
		});
		return true;
	}
	return false;
}

bool CChinaMarket::TaskUpdateOptionDB(long lCurrentTime) {
	AddTask(CHINA_MARKET_UPDATE_OPTION_DB__, GetNextTime(lCurrentTime, 0, 5, 0));

	gl_runtime.thread_executor()->post([this] {
		gl_UpdateChinaMarketDB.acquire();
		gl_systemMessage.SetChinaMarketSavingFunction("update option");
		this->UpdateOptionDB();
		gl_UpdateChinaMarketDB.release();
	});

	return true;
}

bool CChinaMarket::TaskUpdateChosenStockDB() {
	if (IsUpdateChosenStockDB()) {
		gl_runtime.thread_executor()->post([this] {
			gl_UpdateChinaMarketDB.acquire();
			gl_systemMessage.SetChinaMarketSavingFunction("update chose stock");
			this->AppendChosenStockDB();
			gl_UpdateChinaMarketDB.release();
		});
		return true;
	}
	return false;
}

bool CChinaMarket::TaskUpdateStockSection() {
	if (gl_dataContainerChinaStockSymbol.IsUpdateStockSection()) {
		gl_runtime.thread_executor()->post([] {
			gl_UpdateChinaMarketDB.acquire();
			gl_systemMessage.SetChinaMarketSavingFunction("update stockSection");
			gl_dataContainerChinaStockSymbol.UpdateStockSectionDB();
			gl_UpdateChinaMarketDB.release();
		});
		gl_dataContainerChinaStockSymbol.SetUpdateStockSection(false);
		return true;
	}
	return false;
}

void CChinaMarket::TaskProcessAndSaveDayLine(long lCurrentTime) {
	gl_runtime.thread_executor()->post([this] {
		if (IsDayLineNeedProcess()) {
			this->ProcessDayLine();
		}

		// 判断是否存储日线库和股票代码库
		if (gl_dataContainerChinaStock.IsUpdateDayLineDB()) {
			gl_dataContainerChinaStock.TaskUpdateDayLineDB();
		}
	});

	if (!IsSavingDayLineDBTaskFinished()) {// 当尚未更新完日线历史数据时
		AddTask(CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, GetNextTime(lCurrentTime, 0, 0, 10));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 使用当前日期的日线数据生成本周的周线数据。
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::BuildCurrentWeekLine() {
	CContainerChinaDayLine dataChinaDayLine;
	CContainerChinaWeekLine dataChinaWeekLine;
	set<string> setDayLineStockCode;
	set<string> setWeekLineStockCode;
	const long lCurrentMonday = GetCurrentMonday(GetMarketDate());

	if (!LoadDayLine(dataChinaDayLine, GetMarketDate())) {
		return true; // 加载本日日线数据失败，周线数据无需处理。
	}
	const auto pDayLineData = dataChinaDayLine.GetContainer();

	gl_systemMessage.PushInformationMessage("开始生成今日周线");

	CreateStockCodeSet(setDayLineStockCode, dataChinaDayLine.GetContainer());

	DeleteCurrentWeekWeekLineBeforeTheDate(lCurrentMonday); // 从当前周周线表中清除掉本星期一之前的数据
	dataChinaWeekLine.LoadCurrentWeekLine();
	CreateStockCodeSet(setWeekLineStockCode, dataChinaWeekLine.GetContainer());

	for (const auto& data : *pDayLineData) {
		if (!setWeekLineStockCode.contains(data.GetStockSymbol())) {
			//周线数据容器中无此日线数据
			// 存储此日线数据至周线数据容器
			CWeekLine pWeekLine;
			pWeekLine.UpdateWeekLine(&data);
			dataChinaWeekLine.Add(pWeekLine);
		}
		else {
			// 更新周线数据容器
			dataChinaWeekLine.UpdateData(&data);
		}
	}

	// 清除之前的周线数据
	DeleteWeekLine(lCurrentMonday);
	// 存储周线数据值周线数据表
	dataChinaWeekLine.SaveDB("");
	// 清除当前周的数据
	DeleteCurrentWeekWeekLine();
	// 存储当前周数据于当前周数据表
	dataChinaWeekLine.SaveCurrentWeekLine();

	gl_systemMessage.PushInformationMessage("生成今日周线任务完成");

	return true;
}

bool CChinaMarket::CreateStockCodeSet(set<string>& setStockCode, vector<CVirtualHistoryCandle>* pvData) {
	vector<string> vectorStockCode;

	for (auto& pData : *pvData) {
		string strStockSymbol = pData.GetStockSymbol();
		vectorStockCode.push_back(strStockSymbol);
	}
	setStockCode.insert(vectorStockCode.begin(), vectorStockCode.end());

	return true;
}

bool CChinaMarket::BuildCurrentWeekWeekLineTable() {
	try {
		const long lCurrentMonday = GetCurrentMonday(GetMarketDate());
		CSetWeekLineInfo setWeekLineBasicInfo;
		CWeekLinePtr pWeekLine = nullptr;
		CContainerChinaWeekLine dataChinaWeekLine;

		DeleteCurrentWeekWeekLine();

		string sDate = fmt::format("[Date] = {:08Ld}", lCurrentMonday);
		setWeekLineBasicInfo.m_strFilter = sDate.c_str();
		setWeekLineBasicInfo.m_strSort = "[Symbol]";
		setWeekLineBasicInfo.Open();

		dataChinaWeekLine.SaveCurrentWeekLine();
	} catch (CException& e) {
		ReportInformation(e);
	}

	return true;
}

bool CChinaMarket::LoadDayLine(CContainerChinaDayLine& dataChinaDayLine, long lDate) const {
	CSetChinaMarketDayLineInfo setDayLineBasicInfo;

	string sDate = fmt::format("{:08Ld}", lDate);
	setDayLineBasicInfo.m_strSort = "[Symbol]";
	setDayLineBasicInfo.m_strFilter = "[Date] =";
	setDayLineBasicInfo.m_strFilter += sDate.c_str();
	setDayLineBasicInfo.Open();
	if (setDayLineBasicInfo.IsEOF()) {
		// 数据集为空，表明此日没有交易
		setDayLineBasicInfo.Close();
		string str = sDate;
		str += "日数据集为空，无需处理周线数据";
		gl_systemMessage.PushDayLineInfoMessage(str); // 采用同步机制报告信息
		return false;
	}
	setDayLineBasicInfo.m_pDatabase->BeginTrans();

	while (!setDayLineBasicInfo.IsEOF()) {
		CDayLine dayLine;
		dayLine.LoadBasicData(&setDayLineBasicInfo);
		while (!setDayLineBasicInfo.IsEOF() && (setDayLineBasicInfo.m_Symbol.Compare(setDayLineBasicInfo.m_Symbol) < 0)) {
			setDayLineBasicInfo.MoveNext();
		}
		if (!setDayLineBasicInfo.IsEOF() && (setDayLineBasicInfo.m_Symbol.Compare(setDayLineBasicInfo.m_Symbol) == 0)) {
			dayLine.LoadBasicData(&setDayLineBasicInfo);
		}
		dataChinaDayLine.Add(dayLine);
		setDayLineBasicInfo.MoveNext();
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();

	return true;
}

void CChinaMarket::DeleteWeekLine(long lMonday) {
	CSetWeekLineInfo setWeekLineBasicInfo;

	string sDate = fmt::format("[Date] = {:08Ld}", lMonday);
	setWeekLineBasicInfo.m_strFilter = sDate.c_str();
	setWeekLineBasicInfo.Open();
	setWeekLineBasicInfo.m_pDatabase->BeginTrans();
	while (!setWeekLineBasicInfo.IsEOF()) {
		setWeekLineBasicInfo.Delete();
		setWeekLineBasicInfo.MoveNext();
	}
	setWeekLineBasicInfo.m_pDatabase->CommitTrans();
	setWeekLineBasicInfo.Close();
}

bool CChinaMarket::DeleteWeekLine() {
	if (!gl_systemConfiguration.IsWorkingMode()) {
		ASSERT(0); // 由于处理实际数据库，故不允许测试此函数
		exit(1);
	}
	CDatabase database;

	database.Open(_T("stock_market"), FALSE, FALSE, _T("ODBC;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `stock_market`.`china_stock_weekline`;"));
	database.CommitTrans();
	database.Close();
	return true;
}

bool CChinaMarket::DeleteCurrentWeekWeekLine() {
	CSetCurrentWeekLine setCurrentWeekLineInfo;

	setCurrentWeekLineInfo.Open();
	setCurrentWeekLineInfo.m_pDatabase->BeginTrans();
	while (!setCurrentWeekLineInfo.IsEOF()) {
		setCurrentWeekLineInfo.Delete();
		setCurrentWeekLineInfo.MoveNext();
	}
	setCurrentWeekLineInfo.m_pDatabase->CommitTrans();
	setCurrentWeekLineInfo.Close();

	return true;
}

bool CChinaMarket::DeleteCurrentWeekWeekLineBeforeTheDate(long lCutOffDate) {
	CSetCurrentWeekLine setCurrentWeekLineInfo;

	setCurrentWeekLineInfo.Open();
	setCurrentWeekLineInfo.m_pDatabase->BeginTrans();
	while (!setCurrentWeekLineInfo.IsEOF()) {
		if (setCurrentWeekLineInfo.m_Date < lCutOffDate) {
			setCurrentWeekLineInfo.Delete();
		}
		setCurrentWeekLineInfo.MoveNext();
	}
	setCurrentWeekLineInfo.m_pDatabase->CommitTrans();
	setCurrentWeekLineInfo.Close();

	return true;
}

CChinaStockPtr CChinaMarket::GetCurrentSelectedStock() {
	if (m_lCurrentSelectedStockSet >= 0) {
		return m_avChosenStock.at(m_lCurrentSelectedStockSet).at(0);
	}
	return gl_dataContainerChinaStock.GetStock(0);
}

bool CChinaMarket::ProcessDayLine() {
	CDayLineWebDataPtr pData;
	bool succeed = gl_qDayLine.try_dequeue(pData);
	while (succeed) {
		ASSERT(gl_dataContainerChinaStock.IsSymbol(pData->GetStockCode()));
		const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(pData->GetStockCode());
		pStock->UpdateDayLine(pData->GetProcessedDayLine()); // pData的日线数据是正序的，最新日期的在最后面。
		pStock->UpdateStatusByDownloadedDayLine();

		pStock->SetDayLineLoaded(true);
		pStock->SetUpdateDayLineDB(true); // 设置存储日线标识

		succeed = gl_qDayLine.try_dequeue(pData);
	}
	return true;
}

void CChinaMarket::UpdateOneYearStockDayLine() {
	long lOneYearAgoDate = GetPrevDay(GetMarketDate(), 365);
	for (size_t index = 0; index < gl_dataContainerChinaStock.Size(); index++) {
		auto pStock = gl_dataContainerChinaStock.GetStock(index);
		if (pStock->GetDayLineEndDate() > lOneYearAgoDate) {
			pStock->SetDayLineEndDate(lOneYearAgoDate);
			pStock->SetUpdateDayLine(true);
		}
	}
	gl_pTengxunDayLineDataSource->SetUpdateDayLine(true); // 启动数据源的日线数据更新任务
	AddTask(CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, GetNextTime(GetMarketTime(), 0, 0, 100));
}

void CChinaMarket::UpdateAllStockDayLine() {
	for (size_t index = 0; index < gl_dataContainerChinaStock.Size(); index++) {
		auto pStock = gl_dataContainerChinaStock.GetStock(index);
		pStock->SetDayLineEndDate(CHINA_MARKET_BEGIN_DATE_);
		pStock->SetUpdateDayLine(true);
	}
	gl_pTengxunDayLineDataSource->SetUpdateDayLine(true); // 启动数据源的日线数据更新任务
	AddTask(CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, GetNextTime(GetMarketTime(), 0, 0, 100));
}

void CChinaMarket::DeleteDayLine(long lDate) const {
	DeleteDayLineBasicInfo(lDate);
}

void CChinaMarket::DeleteDayLineBasicInfo(long lDate) const {
	char buffer[20]{};
	CSetChinaMarketDayLineInfo setDayLineBasicInfo;

	_ltoa_s(lDate, buffer, 10);
	const string strDate = buffer;
	setDayLineBasicInfo.m_strFilter = "[Date] =";
	setDayLineBasicInfo.m_strFilter += strDate.c_str();
	setDayLineBasicInfo.Open();
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	while (!setDayLineBasicInfo.IsEOF()) {
		setDayLineBasicInfo.Delete();
		setDayLineBasicInfo.MoveNext();
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();
}

///////////////////////////////////////////////////////////////////////////////////
//
// 更新选项数据库
//
//
//
//////////////////////////////////////////////////////////////////////////////////
void CChinaMarket::UpdateOptionDB() {
	try {
		CSetOption setOption;

		setOption.Open();
		setOption.m_pDatabase->BeginTrans();
		if (setOption.IsEOF()) {
			setOption.AddNew();
			setOption.m_LastLoginDate = GetMarketDate();
			setOption.m_LastLoginTime = GetMarketTime();
			setOption.Update();
		}
		else {
			setOption.Edit();
			setOption.m_LastLoginDate = GetMarketDate();
			setOption.m_LastLoginTime = GetMarketTime();
			setOption.Update();
		}
		setOption.m_pDatabase->CommitTrans();
		setOption.Close();
	} catch (CException& e) {
		ReportInformation(e);
	}
}

void CChinaMarket::LoadOptionDB() {
	using namespace StockMarket;
	const auto& t = ChinaMarketOptions{};

	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);
	auto result = db(select(all_of(t)).from(t).unconditionally());
	if (result.begin() == result.end()) {
		SetLastLoginDate(CHINA_MARKET_BEGIN_DATE_);
	}
	else {
		const auto& row = result.front();
		if (static_cast<int>(row.LastLoginDate) == 0) {
			SetLastLoginDate(CHINA_MARKET_BEGIN_DATE_);
		}
		else {
			SetLastLoginDate(row.LastLoginDate);
		}
		SetLastLoginTime(row.LastLoginTime);
	}
	tx.commit();
}

void CChinaMarket::UpdateChosenStockDB() const {
	try {
		CSetChinaChosenStock setChinaChosenStock;

		setChinaChosenStock.Open();
		setChinaChosenStock.m_pDatabase->BeginTrans();
		while (!setChinaChosenStock.IsEOF()) {
			setChinaChosenStock.Delete();
			setChinaChosenStock.MoveNext();
		}
		setChinaChosenStock.m_pDatabase->CommitTrans();
		setChinaChosenStock.m_pDatabase->BeginTrans();
		for (const auto& pStock : m_avChosenStock.at(0)) {
			ASSERT(pStock->IsChosen());
			setChinaChosenStock.AddNew();
			setChinaChosenStock.m_Symbol = pStock->GetSymbol().c_str();
			setChinaChosenStock.Update();
			pStock->SetUpdateChosenStockDB(true);
		}
		setChinaChosenStock.m_pDatabase->CommitTrans();
		setChinaChosenStock.Close();
	} catch (CException& e) {
		ReportInformation(e);
	}
}

void CChinaMarket::AppendChosenStockDB() {
	try {
		CSetChinaChosenStock setChinaChosenStock;

		setChinaChosenStock.Open();
		setChinaChosenStock.m_pDatabase->BeginTrans();
		for (const auto& pStock : m_avChosenStock.at(0)) {
			ASSERT(pStock->IsChosen());
			if (!pStock->IsUpdateChosenStockDB()) {
				setChinaChosenStock.AddNew();
				setChinaChosenStock.m_Symbol = pStock->GetSymbol().c_str();
				setChinaChosenStock.Update();
				pStock->SetUpdateChosenStockDB(true);
			}
		}
		setChinaChosenStock.m_pDatabase->CommitTrans();
		setChinaChosenStock.Close();
	} catch (CException& e) {
		ReportInformation(e);
	}
}

void CChinaMarket::LoadChosenStockDB() {
	CSetChinaChosenStock setChinaChosenStock;

	setChinaChosenStock.Open();
	// 装入股票代码数据库
	while (!setChinaChosenStock.IsEOF()) {
		CChinaStockPtr pStock = nullptr;
		if (gl_dataContainerChinaStock.IsSymbol(T2Utf8(setChinaChosenStock.m_Symbol))) {
			pStock = gl_dataContainerChinaStock.GetStock(T2Utf8(setChinaChosenStock.m_Symbol));
			if (std::ranges::count(m_avChosenStock.at(0).begin(), m_avChosenStock.at(0).end(), pStock) == 0) {
				m_avChosenStock.at(0).push_back(pStock);
			}
			pStock->SetChosen(true);
			pStock->SetUpdateChosenStockDB(true);
		}
		else {
			setChinaChosenStock.Delete();
		}
		setChinaChosenStock.MoveNext();
	}
	setChinaChosenStock.Close();
}

void CChinaMarket::ResetEffectiveRTDataRatio() {
	ResetCurrentEffectiveRTDataRatio();
}
