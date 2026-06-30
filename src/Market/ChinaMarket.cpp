#include"pch.h"

#include"TimeConvert.h"
#include"Thread.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

#include "EastmoneyDayLineDataSource.h"
#include "InfoReport.h"

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
	CreateLocalTimeZone(m_strLocalMarketTimeZone);// 北京标准时间位于东八区， 中国股市开市时间为九点十五分

	m_fUsingSinaRTDataReceiver = true; // 使用新浪实时数据提取器
	m_fUsingTengxunRTDataReceiver = true; // 使用腾讯实时数据提取器

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

	m_tpNewTransactionTime = toSysTime(0);

	m_iCountDownTengxunNumber = 10;

	m_lLastLoginTime = toLocalTime(0);

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

bool CChinaMarket::IsTimeToResetSystem(chrono::local_seconds ls) {
	if (((ls > chrono::local_seconds(9h + 12min + 59s)) && (ls < chrono::local_seconds(9h + 14min + 01s)))
		|| ((ls > chrono::local_seconds(9h + 24min + 59s)) && (ls < chrono::local_seconds(9h + 27min + 01s))))
		return true;
	return false;
}

bool CChinaMarket::IsOrdinaryTradeTime(chrono::local_seconds lTime) {
	if (!IsWorkingDay()) return false;
	if (lTime < chrono::local_seconds(9h + 30min + 00s)) return false;
	if (lTime > chrono::local_seconds(11h + 30min + 00s) && lTime < chrono::local_seconds(13h + 00min + 00s)) return false;
	if (lTime > chrono::local_seconds(15h + 00min + 00s)) return false;
	return true;
}

bool CChinaMarket::IsWorkingTime(chrono::local_seconds lTime) {
	if (!IsWorkingDay()) return false;
	if (lTime < chrono::local_seconds(9h + 12min + 00s)) return false;
	if (lTime > chrono::local_seconds(11h + 45min + 00s) && lTime < chrono::local_seconds(12h + 45min + 00s)) return false;
	if (lTime > chrono::local_seconds(15h + 06min + 30s)) return false;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
//
// 各任务调度处理函数
//
// todo 采用concurrencpp::timer后，可以将此函数的各项任务分配为定时器所驱动的工作线程任务。
//
//
///////////////////////////////////////////////////////////////////////////////////////
int CChinaMarket::ProcessTask() {
	if (IsMarketTaskEmpty()) return false;
	const auto pTask = GetMarketTask();
	if (GetMarketTime() >= pTask->GetTime()) { // time to executive?
		DiscardCurrentMarketTask();
		switch (pTask->GetType()) {
		case CHINA_MARKET_CREATE_TASK__: // 生成其他任务
			TaskCreateTask();
			break;
		case RELOAD_SYSTEM__: // 重启系统？
			ReportSchedulingExitToWatchdog();
			TaskExitSystem();
			break;
		case CHINA_MARKET_CHECK_SYSTEM_READY__:
			TaskCheckMarketReady();
			break;
		case CHINA_MARKET_RESET__: // 市场重置
			TaskResetMarket();
			break;
		case CHINA_MARKET_CHECK_SYSTEM: // 系统检查
			TaskCheckSystem();
			break;
		case CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__:
			TaskPerSecond();
			TaskDistributeAndCalculateRTData();
			break;
		case CHINA_MARKET_BUILD_TODAY_DATABASE__:
			TaskProcessTodayStock();
			break;
		case CHINA_MARKET_VALIDATE_TODAY_DATABASE__:
			//not implemented
			break;
		case CHINA_MARKET_UPDATE_OPTION_DB__:
			TaskUpdateOptionDB();
			break;
		case CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__:
			TaskUpdateStockProfileDB();
			break;
		case CHINA_MARKET_UPDATE_STOCK_SECTION__:
			TaskUpdateStockSection();
			break;
		case CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__:
			TaskProcessAndSaveDayLine();
			break;
		case CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__:
			TaskAccessoryPerMinuteTask();
			break;
		case CHINA_MARKET_PREPARING_MARKET_OPEN__:
			TaskPreparingMarketOpen();
			break;
		default:
			ASSERT(0); // 错误的任务号
			break;
		}
		return pTask->GetType();
	}
	return 0;
}

int CChinaMarket::ProcessCurrentImmediateTask() {
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

bool CChinaMarket::TaskCheckMarketReady() {
	if (!IsSystemReady()) {
		const auto lMax = gl_dataContainerChinaStock.Size() > 12000 ? gl_dataContainerChinaStock.Size() * 2 : 24000;
		if (m_llRTDataReceived > lMax) {
			SetSystemReady(true);
			gl_systemMessage.PushInformationMessage("中国股票市场初始化完毕");
		}
	}
	if (!IsSystemReady()) AddTask(CHINA_MARKET_CHECK_SYSTEM_READY__, GetNextSecond(GetMarketTime()));

	return IsSystemReady();
}

size_t CChinaMarket::GetCurrentStockSetSize() const {
	if (IsTotalStockSetSelected()) return gl_dataContainerChinaStock.Size();
	return m_avChosenStock.at(m_lCurrentSelectedStockSet).size();
}

void CChinaMarket::CreateStock(const string& strStockCode, const string& strStockName, const bool fProcessRTData) {
	const auto pStock = make_shared<CChinaStock>();
	pStock->SetNewStock(true);
	pStock->SetSymbol(strStockCode);
	pStock->SetDisplaySymbol(strStockName);
	pStock->SetDayLineEndDate(toLocalDays(CHINA_MARKET_BEGIN_DATE_));
	pStock->SetDayLineStartDate(toLocalDays(CHINA_MARKET_BEGIN_DATE_));
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
		if (m_tpNewTransactionTime < pRTData->GetTime()) {
			m_tpNewTransactionTime = pRTData->GetTime();
		}
		const auto pStock = gl_dataContainerChinaStock.GetStock(pRTData->GetSymbol());
		if (!pStock->IsActive()) {
			if (pRTData->IsValidTime(14)) {
				pStock->UpdateStatus(pRTData);
			}
		}
		if (pRTData->GetTime() > pStock->GetTimePoint()) {// 新的数据？
			m_lNewRTDataReceivedInCurrentMinute++;
			pStock->PushRTData(pRTData); // 存储新的数据至数据池
			pStock->SetTimePoint(pRTData->GetTime()); // 设置最新接受到实时数据的时间
		}
	}
	return true;
}

bool CChinaMarket::IsRealTimeDataSourceEnable() noexcept {
	switch (gl_systemConfiguration.GetChinaMarketRealtimeServer()) {
	case SinaRealTime_:
		return gl_pSinaRTDataSource->IsEnable();
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
void CChinaMarket::TaskDistributeAndCalculateRTData() {
	gl_runtime.thread_pool_executor()->post([this] { // 无需等待结果，直接返回
			gl_ProcessChinaMarketRTData.acquire();
			auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());

			this->DistributeRTData();
			this->CalculateRTData();

			auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
			this->SetDistributeAndCalculateTime((end - start).count());
			gl_ProcessChinaMarketRTData.release();
		});

	AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, GetNextSecond(GetMarketTime())); // 每秒执行一次
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

void CChinaMarket::TaskCreateTask() {
	chrono::hh_mm_ss<chrono::seconds> hms = toTodayClock(GetMarketTime());
	chrono::seconds seconds = hms.seconds();
	while (!IsMarketTaskEmpty()) DiscardCurrentMarketTask();

	// 系统初始化检查
	AddTask(CHINA_MARKET_CHECK_SYSTEM_READY__, 1);
	// 每秒一次分配实时数据
	AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, 1); // 开始执行时间为：1

	// 辅助任务。在随后的正点分钟执行。
	AddTask(CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__, GetNextTime(GetMarketTime(), 0h, 1min, -seconds));

	// 市场重置
	if (GetMarketTime() < toLocalTime(91300)) {
		AddTask(CHINA_MARKET_RESET__, toLocalTime(91300)); // 执行时间为：91300
	}

	// 第一次系统检查
	if (GetMarketTime() < toLocalTime(91800)) {
		AddTask(CHINA_MARKET_CHECK_SYSTEM, toLocalTime(91800)); // 执行时间为：91800
	}

	// 准备开市任务。每日执行一次，于92959执行。
	if (GetMarketTime() < toLocalTime(92959)) {
		AddTask(CHINA_MARKET_PREPARING_MARKET_OPEN__, 92959);
	}

	// 每十秒钟存储一次日线历史数据。
	AddTask(CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, 93000); // 中午休市时开始更新日线历史数据。

	// 每五分钟存储一次系统选项数据库
	AddTask(CHINA_MARKET_UPDATE_OPTION_DB__, GetNextTime(GetMarketTime(), 0h, 3min, 5s - seconds)); // 开始执行时间为启动之后的三分钟。

	// 每五分钟存储一次股票简要数据库
	AddTask(CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__, GetNextTime(GetMarketTime(), 0h, 4min, 10s - seconds)); // 开始执行时间为启动之后的四分钟。

	if (IsWorkingDay() && GetMarketTime() < toLocalTime(150300)) {
		// 生成本日历史数据
		AddTask(CHINA_MARKET_BUILD_TODAY_DATABASE__, 150530); // 开始执行时间为：150530
	}

	// 如果设定为周期性重启系统，则在星期天晚上9时重启。
	if (gl_systemConfiguration.IsReloadSystem() && (GetWeekDay() == chrono::Sunday) && (GetMarketTime() < toLocalTime(210000))) {
		AddTask(RELOAD_SYSTEM__, toLocalTime(210000));
	}

	AddTask(CHINA_MARKET_CREATE_TASK__, toLocalTime(240000)); // 重启市场任务的任务于每日零时执行
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

void CChinaMarket::TaskPerSecond() {
	IsWebBusy();
}

void CChinaMarket::TaskAccessoryPerMinuteTask() {
	CheckFastReceivingData();
	CheckMarketOpen();// 判断中国股票市场开市状态
	SetCheckActiveStockFlag();
	ResetEffectiveRTDataRatio(); // 重置有效实时数据比率

	if (gl_systemConfiguration.IsUpdateDB()) { // 每分钟检查一次系统配置是否需要存储。
		gl_runtime.thread_executor()->post([] {
			gl_systemConfiguration.UpdateDB();
		});
		gl_systemConfiguration.SetUpdateDB(false);
	}

	AddTask(CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__, GetNextTime(GetMarketTime(), 0h, 1min, 0s)); // 每分钟整点执行一次
}

void CChinaMarket::TaskPreparingMarketOpen() {
	ASSERT(GetMarketTime() == toLocalTime(92959)); // 每日执行一次
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

bool CChinaMarket::SetCheckActiveStockFlag() {
	if (!IsSystemReady()) {
		m_fCheckActiveStock = true;
		return true;
	}
	if (((GetMarketTime() > toLocalTime(91459)) && (GetMarketTime() < toLocalTime(92700)))
		|| ((GetMarketTime() > toLocalTime(113259)) && (GetMarketTime() < toLocalTime(125900)))
		|| (GetMarketTime() > toLocalTime(150300))) {
		m_fCheckActiveStock = true;
		return true;
	}
	m_fCheckActiveStock = false;
	return false;
}

bool CChinaMarket::TaskProcessTodayStock() {
	if (IsSystemReady() && GetMarketTimeHMS().to_duration() > 15h + 04min) {
		gl_runtime.thread_executor()->post([this] {
			gl_systemMessage.SetChinaMarketSavingFunction("process today stock");
			this->ProcessTodayStock();
		});
		return true;
	}
	return false;
}

void CChinaMarket::ProcessTodayStock() {
	ASSERT(IsSystemReady()); // 调用本工作线程时必须设置好市场。

	gl_dataContainerChinaStock.BuildDayLine(GetMarketDate());
	gl_dataContainerChinaStock.UpdateProfileDB();
	if (GetMarketTimeHMS().to_duration() > 15h + 04min) {	// 如果中国股市闭市了
		SetUpdateOptionDB(true); // 更新状态
	}
	string s = std::format("{:%F}的实时数据处理完毕", GetMarketDate());
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

void CChinaMarket::EnableDayLineDataSource() {
	switch (gl_systemConfiguration.GetChinaMarketDayLineServer()) {
	case TengxunDayLine_:
		gl_pTengxunDayLineDataSource->Enable(true);
		TRACE(_T("启动腾讯日线数据源\n"));
		break;
	default:
		gl_pTengxunDayLineDataSource->Enable(true);
		break;
	}
}

bool CChinaMarket::CheckFastReceivingData() {
	if (gl_systemConfiguration.IsFastInquiringRTData()) {
		m_fFastReceivingRTData = true;
	}
	else {
		m_fFastReceivingRTData = IsWorkingTime(GetMarketTime());
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
	case TengxunRealTime_: // 更新腾讯实时数据读取时间
		httpStatus = gl_pTengxunRTDataSource->GetHTTPStatusCode();
		break;
	default: // error
		ASSERT(0);
		break;
	}
	return httpStatus;
}

long long CChinaMarket::GetDayLineHTTPStatus() {
	return gl_pTengxunDayLineDataSource->GetHTTPStatusCode();
}

bool CChinaMarket::IsWebReaTimeDataError() {
	bool webError = false;
	switch (gl_systemConfiguration.GetChinaMarketRealtimeServer()) {
	case SinaRealTime_: // 新浪实时数据
		webError = gl_pSinaRTDataSource->IsWebError();
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

long long CChinaMarket::GetWebRealTimeDataErrorCode() {
	long long errorCode = 0;
	switch (gl_systemConfiguration.GetChinaMarketRealtimeServer()) {
	case SinaRealTime_: // 新浪实时数据
		errorCode = gl_pSinaRTDataSource->GetWebErrorCode();
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

bool CChinaMarket::IsDayLineDataSourceEnable() {
	return gl_pTengxunDayLineDataSource->IsEnable() || gl_pEastmoneyDayLineDataSource->IsEnable();
}

bool CChinaMarket::IsWebDayLineDataError() {
	return gl_pTengxunDayLineDataSource->IsWebError();
}

long long CChinaMarket::GetWebDayLineDataErrorCode() {
	return gl_pTengxunDayLineDataSource->GetWebErrorCode();
}

bool CChinaMarket::CheckMarketOpen() {
	if (IsWorkingDay() && (GetMarketTime() > toLocalTime(92759) && GetMarketTime() < toLocalTime(150600))) {	// 市场结束接收数据的时间，皆定为150600（与停止存储临时数据的时间一样）
		m_fMarketOpened = true;
	}
	else m_fMarketOpened = false;

	return m_fMarketOpened;
}

bool CChinaMarket::TaskResetMarket() {
	// 九点十三分重启系统。 必须在此时间段内重启，如果更早的话容易出现数据不全的问题。
	SetSystemReady(false);
	AddTask(CHINA_MARKET_CHECK_SYSTEM_READY__, GetMarketTime()); // 每次重置系统时，必须进行系统初始化状态检查
	AddTask(CHINA_MARKET_UPDATE_STOCK_SECTION__, GetNextTime(GetMarketTime(), 0h, 5min, 0s)); // 五分钟后再更新此数据库
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
bool CChinaMarket::TaskCheckSystem() {
	if ((gl_systemConfiguration.IsUsingSinaRTServer() && gl_pSinaRTDataSource->GetCurrentInquiryTime() > 300)
		|| (gl_systemConfiguration.IsUsingTengxunRTServer() && gl_pTengxunRTDataSource->GetCurrentInquiryTime() > 300)) { // 如果接受网络数据时间超过300毫秒
		gl_systemMessage.PushInnerSystemInformationMessage("china stock RT data web busy\n");
#ifndef _DEBUG
		ReportCheckRunningExitToWatchdog();
		TaskExitSystem();
#endif
	}
	return true;
}

bool CChinaMarket::TaskUpdateStockProfileDB() {
	AddTask(CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__, GetNextTime(GetMarketTime(), 0h, 5min, 0s));

	if (gl_dataContainerChinaStock.IsUpdateProfileDB()) {
		gl_runtime.thread_executor()->post([] {
			gl_systemMessage.SetChinaMarketSavingFunction("update china stock profile");
			gl_dataContainerChinaStock.UpdateProfileDB();
		});
		return true;
	}
	return false;
}

bool CChinaMarket::TaskUpdateOptionDB() {
	AddTask(CHINA_MARKET_UPDATE_OPTION_DB__, GetNextTime(GetMarketTime(), 0h, 5min, 0s));

	gl_runtime.thread_executor()->post([this] {
		gl_systemMessage.SetChinaMarketSavingFunction("update option");
		this->UpdateOptionDB();
	});

	return true;
}

bool CChinaMarket::TaskUpdateChosenStockDB() {
	if (IsUpdateChosenStockDB()) {
		gl_runtime.thread_executor()->post([this] {
			gl_systemMessage.SetChinaMarketSavingFunction("update chose stock");
			this->AppendChosenStockDB();
		});
		return true;
	}
	return false;
}

bool CChinaMarket::TaskUpdateStockSection() {
	if (gl_dataContainerChinaStockSymbol.IsUpdateStockSection()) {
		gl_runtime.thread_executor()->post([] {
			gl_systemMessage.SetChinaMarketSavingFunction("update stockSection");
			gl_dataContainerChinaStockSymbol.UpdateStockSectionDB();
		});
		gl_dataContainerChinaStockSymbol.SetUpdateStockSection(false);
		return true;
	}
	return false;
}

void CChinaMarket::TaskProcessAndSaveDayLine() {
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
		AddTask(CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, GetNextTime(GetMarketTime(), 0h, 1min, 0s));
	}
}

bool CChinaMarket::CreateStockCodeSet(set<string>& setStockCode, const vector<CVirtualHistoryCandle>* pvData) {
	vector<string> vectorStockCode;

	for (auto& pData : *pvData) {
		vectorStockCode.push_back(pData.GetStockSymbol());
	}
	setStockCode.insert(vectorStockCode.begin(), vectorStockCode.end());

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

		pStock->SetDayLineLoaded(true);
		pStock->SetUpdateDayLineDB(true); // 设置存储日线标识

		succeed = gl_qDayLine.try_dequeue(pData);
	}
	return true;
}

void CChinaMarket::UpdateOneYearStockDayLine() {
	chrono::local_days lOneYearAgoDate = GetPrevDay(GetMarketDate(), 365);
	for (size_t index = 0; index < gl_dataContainerChinaStock.Size(); index++) {
		auto pStock = gl_dataContainerChinaStock.GetStock(index);
		if (pStock->GetDayLineEndDate() > lOneYearAgoDate) {
			pStock->SetDayLineEndDate(lOneYearAgoDate);
			pStock->SetUpdateDayLine(true);
		}
	}
	gl_pTengxunDayLineDataSource->SetUpdateDayLine(true); // 启动数据源的日线数据更新任务
	AddTask(CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, GetNextTime(GetMarketTime(), 0h, 1min, 0s));
}

void CChinaMarket::UpdateAllStockDayLine() {
	for (size_t index = 0; index < gl_dataContainerChinaStock.Size(); index++) {
		auto pStock = gl_dataContainerChinaStock.GetStock(index);
		pStock->SetDayLineEndDate(toLocalDays(CHINA_MARKET_BEGIN_DATE_));
		pStock->SetUpdateDayLine(true);
	}
	gl_pTengxunDayLineDataSource->SetUpdateDayLine(true); // 启动数据源的日线数据更新任务
	AddTask(CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, GetNextTime(GetMarketTime(), 0h, 1min, 0s));
}

void CChinaMarket::DeleteDayLine(chrono::local_days lDate) const {
	using namespace StockMarket;
	const auto& t = ChinaStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	db(sqlpp::remove_from(t).where(t.Date == toFormattedDate(lDate)));
	tx.commit();
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
		using namespace StockMarket;
		const auto& t = ChinaMarketOptions{};

		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);

		auto result = db(select(all_of(t)).from(t).unconditionally());
		if (result.size() == 0) {
			db(sqlpp::insert_into(t).set(
				t.LastLoginDate = toFormattedDate(GetMarketDate()),
				t.LastLoginTime = toFormattedTime(GetMarketTime())
			));
		}
		else {
			db(update(t).set(
				t.LastLoginDate = toFormattedDate(GetMarketDate()),
				t.LastLoginTime = toFormattedTime(GetMarketTime())
			).unconditionally());
		}
		tx.commit();
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
		SetLastLoginDate(toLocalDays(CHINA_MARKET_BEGIN_DATE_));
	}
	else {
		const auto& row = result.front();
		if (static_cast<int>(row.LastLoginDate) == 0) {
			SetLastLoginDate(toLocalDays(CHINA_MARKET_BEGIN_DATE_));
		}
		else {
			SetLastLoginDate(toLocalDays(row.LastLoginDate));
		}
		SetLastLoginTime(toLocalTime(row.LastLoginTime));
	}
	tx.commit();
}

void CChinaMarket::UpdateChosenStockDB() const {
	try {
		using namespace StockMarket;
		const auto& t = ChinaChoiceStock{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		db(remove_from(t).unconditionally());

		for (const auto& pStock : m_avChosenStock.at(0)) {
			db(sqlpp::insert_into(t).set(
				t.Symbol = pStock->GetSymbol()
			));
		}
		tx.commit();
	} catch (CException& e) {
		ReportInformation(e);
	}
}

void CChinaMarket::AppendChosenStockDB() {
	try {
		using namespace StockMarket;
		const auto& t = ChinaChoiceStock{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		for (const auto& pStock : m_avChosenStock.at(0)) {
			if (!pStock->IsUpdateChosenStockDB()) {
				db(insert_into(t).set(
					t.Symbol = pStock->GetSymbol()
				));
			}
		}
		tx.commit();
	} catch (CException& e) {
		ReportInformation(e);
	}
}

void CChinaMarket::LoadChosenStockDB() {
	using namespace StockMarket;
	const auto& t = ChinaChoiceStock{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).unconditionally());
	auto rows = result.size();

	for (const auto& row : result) {
		CChinaStockPtr pStock = nullptr;
		if (gl_dataContainerChinaStock.IsSymbol(row.Symbol)) {
			pStock = gl_dataContainerChinaStock.GetStock(row.Symbol);
			if (std::ranges::count(m_avChosenStock.at(0).begin(), m_avChosenStock.at(0).end(), pStock) == 0) {
				m_avChosenStock.at(0).push_back(pStock);
			}
			pStock->SetChosen(true);
			pStock->SetUpdateChosenStockDB(true);
		}
	}
	tx.commit();
}

void CChinaMarket::ResetEffectiveRTDataRatio() {
	ResetCurrentEffectiveRTDataRatio();
}
