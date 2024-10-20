﻿#include"pch.h"

#include"ConvertToString.h"
#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"
#include"Thread.h"

#include"ThreadStatus.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

#include "HighPerformanceCounter.h"
#include "InfoReport.h"
#include"SetDayLineExtendInfo.h"
#include"SetDayLineTodaySaved.h"
#include"SetOption.h"
#include"SetChinaChosenStock.h"
#include"SetRSStrong2Stock.h"
#include"SetRSStrong1Stock.h"
#include"SetRSStrongStock.h"
#include"SetRSOption.h"
#include"SetWeekLineExtendInfo.h"

#include"SetCurrentWeekLine.h"

#include "NeteaseRTDataSource.h"
#include "SinaRTDataSource.h"
#include "TengxunRTDataSource.h"

#define WM_FIREBIRD_EXIT                0X500

#include<gsl/gsl>

using namespace gsl;

CChinaMarket::CChinaMarket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE(_T("ChinaMarket市场变量只允许存在一个实例\n"));
	}
	m_strMarketId = _T("中国股票市场");
	m_lMarketTimeZone = -8 * 3600; // 北京标准时间位于东八区，超前GMT8小时

	m_fUsingSinaRTDataReceiver = true; // 使用新浪实时数据提取器
	m_fUsingTengxunRTDataReceiver = true; // 使用腾讯实时数据提取器
	m_fUsingNeteaseRTDataReceiver = false; // 不使用网易实时数据提取器

	m_avChosenStock.resize(30);
	m_aRSStrongOption.resize(10);

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
	if (GetCurrentStock() == nullptr) {
		if (gl_systemConfiguration.GetCurrentStock().GetLength() > 0) {
			gl_systemConfiguration.SetCurrentStock(_T(""));
			gl_systemConfiguration.NeedUpdate(true);
		}
	}
	else if (GetCurrentStock()->GetSymbol() != gl_systemConfiguration.GetCurrentStock()) {
		gl_systemConfiguration.SetCurrentStock(gl_pChinaMarket->GetCurrentStock()->GetSymbol());
		gl_systemConfiguration.NeedUpdate(true);
	}
}

void CChinaMarket::ResetMarket() {
	CString str = _T("重置中国股市于北京标准时间：");
	m_fResettingMarket = true;
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);
	gl_ProcessChinaMarketRTData.acquire();
	while (gl_ThreadStatus.IsSavingThreadRunning()) { Sleep(1); }

	Reset();

	gl_dataContainerChinaStock.LoadStockProfileDB();
	LoadOptionDB();
	LoadChosenStockDB();
	Load10DaysRSStrong1StockSet();
	Load10DaysRSStrong2StockSet();
	LoadCalculatingRSOption();
	Load10DaysRSStrongStockDB();

	if (gl_systemConfiguration.GetCurrentStock() != _T("")) {
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

	m_v10RSStrong1Stock.resize(0);
	m_v10RSStrong2Stock.resize(0);
	m_lCurrentSelectedPosition = 0;
	m_lCurrentRSStrongIndex = 0;
	m_lCurrentSelectedStockSet = -1; // 选择使用全体股票集、
	m_fChosen10RSStrong1StockSet = false;
	m_fChosen10RSStrong2StockSet = false;
	m_fChosen10RSStrongStockSet = false;
	m_fCalculateChosen10RS = false;

	m_llRTDataReceived = 0;
	m_lRTDataReceivedInCurrentMinute = 0;
	m_lNewRTDataReceivedInCurrentMinute = 0;

	gl_SystemData.ClearRTDataQueue();

	m_RTDataNeedCalculate = false;
	m_CalculatingDayLineRS = false;
	m_CalculatingWeekLineRS = false;

	m_fCurrentEditStockChanged = false;
	m_fFastReceivingRTData = true;
	m_fRTDataSetCleared = false;
	m_fSaveTempData = true;
	m_pCurrentStock = nullptr;

	m_ttNewestTransactionTime = 0;

	m_iCountDownTengxunNumber = 10;

	m_lRSEndDate = m_lRSStartDate = m_lLastLoginDate = _CHINA_MARKET_BEGIN_DATE_;
	m_lLastLoginTime = 0;
	m_lUpdatedDateFor10DaysRS2 = m_lUpdatedDateFor10DaysRS1 = m_lUpdatedDateFor10DaysRS = _CHINA_MARKET_BEGIN_DATE_;

	m_fSelectedStockLoaded = false;

	m_fCurrentStockChanged = false;
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
	if ((lTime > 113000) && (lTime < 130000)) return false;
	if (lTime > 150000) return false;
	return true;
}

bool CChinaMarket::IsWorkingTime(long lTime) {
	if (!IsWorkingDay()) return false;
	if (lTime < 91200) return false;
	if ((lTime > 114500) && (lTime < 124500)) return false;
	if (lTime > 150630) return false;
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
			TaskExitSystem(lCurrentTime);
			break;
		case CHINA_MARKET_CHECK_SYSTEM_READY__:
			TaskCheckMarketReady(lCurrentTime);
			break;
		case CHINA_MARKET_RESET__: // 市场重置
			TaskResetMarket(lCurrentTime);
			break;
		case CHINA_MARKET_LOAD_TEMP_RT_DATA__:
			TaskLoadTempRTData(GetMarketDate(), lCurrentTime);
			break;
		case CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__:
			TaskPerSecond(lCurrentTime);
			TaskDistributeAndCalculateRTData(lCurrentTime);
			break;
		case CHINA_MARKET_SAVE_TEMP_RT_DATA__:
			TaskSaveTempData(lCurrentTime);
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
		case CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__:
			TaskChoiceRSSet(lCurrentTime);
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
	case CHINA_MARKET_LOAD_CURRENT_STOCK_DAY_LINE__:
		TaskLoadCurrentStockHistoryData();
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
			gl_systemMessage.PushInformationMessage(_T("中国股票市场初始化完毕"));
		}
	}
	if (!IsSystemReady()) AddTask(CHINA_MARKET_CHECK_SYSTEM_READY__, GetNextSecond(lCurrentTime));

	return IsSystemReady();
}

void CChinaMarket::ChangeToNextStock() {
	ASSERT(m_pCurrentStock != nullptr);
	long lIndex = gl_dataContainerChinaStock.GetOffset(m_pCurrentStock);
	CChinaStockPtr pStock = m_pCurrentStock;

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

	SetCurrentStock(pStock);
}

void CChinaMarket::ChangeToPrevStock() {
	ASSERT(m_pCurrentStock != nullptr);
	long lIndex = gl_dataContainerChinaStock.GetOffset(m_pCurrentStock);
	CChinaStockPtr pStock = m_pCurrentStock;

	if (IsTotalStockSetSelected()) {
		bool fFound = false;
		while (!fFound) {
			if (--lIndex < 0) {
				lIndex = gl_dataContainerChinaStock.Size() - 1;
			}
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
	SetCurrentStock(pStock);
}

void CChinaMarket::ChangeToPrevStockSet() {
	do {
		if (m_lCurrentSelectedStockSet > -1) m_lCurrentSelectedStockSet--;
		else { m_lCurrentSelectedStockSet = c_10DaysRSStockSetStartPosition + 9; }
		ASSERT(m_lCurrentSelectedStockSet < 20);
	} while ((m_lCurrentSelectedStockSet != -1) && (m_avChosenStock.at(m_lCurrentSelectedStockSet).empty()));
}

void CChinaMarket::ChangeToNextStockSet() {
	do {
		if (m_lCurrentSelectedStockSet == (c_10DaysRSStockSetStartPosition + 9)) m_lCurrentSelectedStockSet = -1;
		else { m_lCurrentSelectedStockSet++; }
		ASSERT(m_lCurrentSelectedStockSet < 20);
	} while ((m_lCurrentSelectedStockSet != -1) && (m_avChosenStock.at(m_lCurrentSelectedStockSet).empty()));
}

size_t CChinaMarket::GetCurrentStockSetSize() const {
	if (IsTotalStockSetSelected()) return gl_dataContainerChinaStock.Size();
	return m_avChosenStock.at(m_lCurrentSelectedStockSet).size();
}

void CChinaMarket::CreateStock(const CString& strStockCode, const CString& strStockName, const bool fProcessRTData) {
	const auto pStock = make_shared<CChinaStock>();
	pStock->SetTodayNewStock(true);
	pStock->SetSymbol(strStockCode);
	pStock->SetDisplaySymbol(strStockName);
	pStock->SetIPOStatus(_STOCK_NOT_CHECKED_);
	pStock->SetDayLineEndDate(19900101);
	pStock->SetDayLineStartDate(19900101);
	pStock->SetUpdateProfileDB(true);
	pStock->SetNeedProcessRTData(fProcessRTData);
	gl_dataContainerChinaStock.Add(pStock);
	ASSERT(pStock->IsDayLineNeedUpdate());
	const CString str = _T("china Market生成新代码") + pStock->GetSymbol();
	gl_systemMessage.PushInnerSystemInformationMessage(str);
}

long CChinaMarket::IncreaseStockInquiringIndex(long& lIndex, long lEndPosition) {
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

	tmMarketTime = GetMarketTime(tUTC);
	tmMarketTime.tm_hour = 9;
	tmMarketTime.tm_min = 30;
	tmMarketTime.tm_sec = 0;
	const time_t tUTC2 = TransferToUTCTime(&tmMarketTime);
	long lIndex = (tUTC - tUTC2) / 60;
	if (lIndex < 0) lIndex = 0;
	if ((lIndex >= 120) && (lIndex < 209)) lIndex = 119;
	if (lIndex >= 210) lIndex -= 90;
	if (lIndex >= 240) lIndex = 239;

	ASSERT((lIndex >= 0) && (lIndex < 240));
	return (lIndex);
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
	const CString strSymbol = pRTData->GetSymbol();
	if (IsCheckingActiveStock()) {
		if (!gl_dataContainerChinaStock.IsSymbol(strSymbol) && pRTData->IsActive()) {
			ASSERT(strSymbol.GetLength() == 9);
			CreateStock(strSymbol, pRTData->GetStockName(), true);
		}
	}
	else if (!gl_dataContainerChinaStock.IsSymbol(strSymbol)) {
		return false;
	}
	if (pRTData->IsActive()) { // 此实时数据有效？
		IncreaseRTDataReceived();
		if (m_ttNewestTransactionTime < pRTData->GetTransactionTime()) {
			m_ttNewestTransactionTime = pRTData->GetTransactionTime();
		}
		const auto pStock = gl_dataContainerChinaStock.GetStock(pRTData->GetSymbol());
		if (!pStock->IsActive()) {
			if (pRTData->IsValidTime(14)) {
				pStock->UpdateStatus(pRTData);
			}
		}
		if (pRTData->GetTransactionTime() > pStock->GetTransactionTime()) { // 新的数据？
			m_lNewRTDataReceivedInCurrentMinute++;
			pStock->PushRTData(pRTData); // 存储新的数据至数据池
			pStock->SetTransactionTime(pRTData->GetTransactionTime()); // 设置最新接受到实时数据的时间
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 生成每次查询新浪实时股票数据的字符串
//
//////////////////////////////////////////////////////////////////////////////////////////
CString CChinaMarket::GetSinaStockInquiringStr(long lTotalNumber, bool fUsingTotalStockSet) {
	if (fUsingTotalStockSet) {
		return gl_dataContainerChinaStockSymbol.GetNextSinaStockInquiringMiddleStr(lTotalNumber);
	}
	return gl_dataContainerChinaStock.GetNextSinaStockInquiringMiddleStr(lTotalNumber);
}

CString CChinaMarket::GetNeteaseStockInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet) {
	if (fUsingTotalStockSet) {
		return gl_dataContainerChinaStockSymbol.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber);
	}
	return gl_dataContainerChinaStock.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber);
}

bool CChinaMarket::CheckValidOfNeteaseDayLineInquiringStr(const CString& str) const {
	const CString strNetease = str.Left(7);
	CString strStockCode = XferNeteaseToStandard(strNetease);
	if (!gl_dataContainerChinaStock.IsSymbol(strStockCode)) {
		CString strReport = _T("网易日线查询股票代码错误：");
		TRACE(_T("网易日线查询股票代码错误：%s\n"), strStockCode.GetBuffer());
		strReport += strStockCode;
		gl_systemMessage.PushInnerSystemInformationMessage(strReport);
		return false;
	}

	return true;
}

void CChinaMarket::TaskChoiceRSSet(long lCurrentTime) {
	if (m_fCalculateChosen10RS) {
		if (gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber() <= 0 && gl_dataContainerChinaStock.GetDayLineNeedSaveNumber() <= 0) {
			gl_runtime.background_executor()->post([this, lCurrentTime] {
				gl_ProcessChinaMarketRTData.acquire();
				this->TaskChoice10RSStrongStockSet(lCurrentTime);
				this->TaskChoice10RSStrong1StockSet(lCurrentTime);
				this->TaskChoice10RSStrong2StockSet(lCurrentTime);
				gl_ProcessChinaMarketRTData.release();
			});
		}
		else {
			AddTask(CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__, GetNextTime(lCurrentTime, 0, 1, 0));
		}
	}
}

void CChinaMarket::TaskSetCurrentStock() {
	if (gl_systemConfiguration.GetCurrentStock() != _T("")) { // 当前有选择股票
		if (gl_dataContainerChinaStock.IsSymbol(gl_systemConfiguration.GetCurrentStock())) {
			auto pStock = gl_dataContainerChinaStock.GetStock(gl_systemConfiguration.GetCurrentStock());
			SetCurrentStock(pStock);
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
			CHighPerformanceCounter counter;
			counter.start();

			this->DistributeRTData();
			this->CalculateRTData();

			counter.stop();
			this->SetDistributeAndCalculateTime(counter.GetElapsedMillisecond());
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
	if (IsSystemReady() && IsTodayTempRTDataLoaded() && IsRTDataNeedCalculate()) {
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
	// 再次市场重置
	if (lCurrentTime < 92600) {
		AddTask(CHINA_MARKET_RESET__, 92600); // 执行时间为：92600
	}

	// 装载本日存储的实时数据。必须于市场两次重启后（92600后）再执行，这样才能保证正常设置m_fLoadTodayRTData变量
	AddTask(CHINA_MARKET_LOAD_TEMP_RT_DATA__, 92700);

	// 准备开市任务。每日执行一次，于92959执行。
	if (lCurrentTime < 92959) {
		AddTask(CHINA_MARKET_PREPARING_MARKET_OPEN__, 92959);
	}

	// 每十秒钟存储一次日线历史数据。
	AddTask(CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, 113510); // 中午休市时开始更新日线历史数据。

	// 每五分钟存储一次系统选项数据库
	AddTask(CHINA_MARKET_UPDATE_OPTION_DB__, GetNextTime(lTimeMinute + 5, 0, 3, 0)); // 开始执行时间为启动之后的三分钟。

	// 每五分钟存储一次股票简要数据库
	AddTask(CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__, GetNextTime(lTimeMinute + 10, 0, 4, 0)); // 开始执行时间为启动之后的四分钟。

	if (IsWorkingDay()) {
		// 每五分钟存储一次临时数据
		AddTask(CHINA_MARKET_SAVE_TEMP_RT_DATA__, 93230); // 开始执行时间为：92730.要确保第一次执行的时间早于93000，这样启动数据库的时间较短，否则容易导致系统崩溃（原因不明）。
		AddTask(CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__, 150700); // 
	}

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
void CChinaMarket::TaskExitSystem(long lCurrentTime) {
	// 向主窗口发送关闭窗口系统消息，通知框架窗口执行关闭任务。
	// 由于系统需要顺序关闭各项任务，故而不允许直接退出系统。
	PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), WM_SYSCOMMAND, SC_CLOSE, 0);
}

////////////////////////////////////////////////////////////////////////////////////
//
// 开市时每五分钟存储一次当前状态。这是一个备用措施，防止退出系统后就丢掉了所有的数据，不必太频繁。
//
/////////////////////////////////////////////////////////////////////////////////////////
void CChinaMarket::TaskSaveTempData(long lCurrentTime) {
	if (lCurrentTime < 150500) { // 中国市场股票交易截止时间为150000。
		long lNextTime = GetNextTime(lCurrentTime, 0, 5, 0);
		if ((lNextTime >= 113500) && (lNextTime < 125730)) lNextTime = 125730;
		AddTask(CHINA_MARKET_SAVE_TEMP_RT_DATA__, lNextTime);
	}
	if (IsSystemReady()) {
		const CString str = "存储临时数据";
		gl_systemMessage.PushDayLineInfoMessage(str);
		gl_runtime.background_executor()->post([] {
			gl_UpdateChinaMarketDB.acquire();
			gl_ProcessChinaMarketRTData.acquire();
			gl_dataContainerChinaStock.SaveTempRTData();
			gl_ProcessChinaMarketRTData.release();
			gl_UpdateChinaMarketDB.release();
		});
	}
}

void CChinaMarket::TaskLoadCurrentStockHistoryData() const {
	ASSERT(m_pCurrentStock != nullptr);
	ASSERT(!m_pCurrentStock->IsDayLineLoaded());
	ASSERT(!m_pCurrentStock->IsWeekLineLoaded());
	auto pStock = m_pCurrentStock;
	gl_runtime.background_executor()->post([pStock] {
		pStock->UnloadDayLine();
		// 装入日线数据
		pStock->LoadDayLine(pStock->GetSymbol());
		// 计算各相对强度（以指数相对强度为默认值）
		pStock->CalculateDayLineRSIndex();
		pStock->SetDayLineLoaded(true);
	});
	gl_runtime.background_executor()->post([pStock] {
		pStock->UnloadWeekLine();
		// 装入周线数据
		pStock->LoadWeekLine();
		// 计算各相对强度（以指数相对强度为默认值）
		pStock->CalculateWeekLineRSIndex();
		pStock->SetWeekLineLoaded(true);
	});
}

void CChinaMarket::TaskPerSecond(long lCurrentTime) {
	IsWebBusy();
}

void CChinaMarket::TaskAccessoryPerMinuteTask(long lCurrentTime) {
	CheckFastReceivingData(lCurrentTime);
	CheckMarketOpen(lCurrentTime);// 判断中国股票市场开市状态
	SetCheckActiveStockFlag(lCurrentTime);
	ResetEffectiveRTDataRatio(); // 重置有效实时数据比率

	if (gl_systemConfiguration.IsNeedUpdate()) { // 每分钟检查一次系统配置是否需要存储。
		gl_runtime.background_executor()->post([] {
			gl_systemConfiguration.UpdateDB();
		});
		gl_systemConfiguration.NeedUpdate(false);
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
		ASSERT(!pStock->IsSaveToChosenStockDB());
		return true;
	}
	return false;
}

bool CChinaMarket::DeleteChosenStock(const CChinaStockPtr& pStock) {
	const auto it = std::ranges::find(m_avChosenStock.at(0).cbegin(), m_avChosenStock.at(0).cend(), pStock);
	if (it == m_avChosenStock.at(0).end()) { return false; }
	(*it)->SetChosen(false);
	(*it)->SetSaveToChosenStockDB(false);
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

bool CChinaMarket::TaskChoice10RSStrong1StockSet(long lCurrentTime) {
	if (IsSystemReady() && !m_fChosen10RSStrong1StockSet && (lCurrentTime > 151100) && IsWorkingDay()) {
		gl_runtime.background_executor()->post([this] {
			gl_UpdateChinaMarketDB.acquire();
			gl_systemMessage.PushInformationMessage(_T("开始计算10日RS1\n"));

			// 添加一个注释
			if (gl_dataContainerChinaStock.Choice10RSStrong1StockSet()) {
				gl_systemMessage.PushInformationMessage(_T("10日RS1计算完毕\n"));
				this->SetUpdatedDateFor10DaysRS1(this->GetMarketDate());
				this->SetUpdateOptionDB(true); // 更新选项数据库
			}
			gl_UpdateChinaMarketDB.release();
		});
		m_fChosen10RSStrong1StockSet = true;
		return true;
	}
	return false;
}

bool CChinaMarket::TaskChoice10RSStrong2StockSet(long lCurrentTime) {
	if (IsSystemReady() && !m_fChosen10RSStrong2StockSet && (lCurrentTime > 151200) && IsWorkingDay()) {
		gl_runtime.background_executor()->post([this] {
			gl_UpdateChinaMarketDB.acquire();
			gl_systemMessage.PushInformationMessage(_T("开始计算10日RS2\n"));

			// 添加一个注释
			if (gl_dataContainerChinaStock.Choice10RSStrong2StockSet()) {
				gl_systemMessage.PushInformationMessage(_T("10日RS2计算完毕\n"));
				this->SetUpdatedDateFor10DaysRS2(this->GetMarketDate());
				this->SetUpdateOptionDB(true); // 更新选项数据库
			}
			gl_UpdateChinaMarketDB.release();
		});
		m_fChosen10RSStrong2StockSet = true;
		return true;
	}
	return false;
}

bool CChinaMarket::TaskChoice10RSStrongStockSet(long lCurrentTime) {
	if (IsSystemReady() && !m_fChosen10RSStrongStockSet && (lCurrentTime > 151000) && IsWorkingDay()) {
		Choice10RSStrongStockSet();
		m_fChosen10RSStrongStockSet = true;
		return true;
	}
	return false;
}

bool CChinaMarket::TaskProcessTodayStock(long lCurrentTime) {
	if (IsSystemReady()) {
		gl_runtime.background_executor()->post([this] {
			gl_UpdateChinaMarketDB.acquire();
			this->ProcessTodayStock();
			gl_UpdateChinaMarketDB.release();
		});
		return true;
	}
	return false;
}

void CChinaMarket::ProcessTodayStock() {
	ASSERT(IsSystemReady()); // 调用本工作线程时必须设置好市场。

	const long lDate = GetMarketDate(GetNewestTransactionTime());
	if (lDate == GetMarketDate()) {
		gl_dataContainerChinaStock.BuildDayLine(lDate);
		// 计算本日日线相对强度
		gl_dataContainerChinaStock.BuildDayLineRS(lDate);
		// 生成周线数据
		BuildWeekLineOfCurrentWeek();
		gl_dataContainerChinaStock.BuildWeekLineRS(GetCurrentMonday(lDate));
		gl_dataContainerChinaStock.UpdateStockProfileDB();
		if (GetMarketTime() > 150400) {	// 如果中国股市闭市了
			SetRSEndDate(GetMarketDate());
			SetUpdateOptionDB(true); // 更新状态
		}
	}
	CString str = ConvertDateToChineseTimeStampString(lDate);
	str += _T("的实时数据处理完毕");
	gl_systemMessage.PushInformationMessage(str);
}

bool CChinaMarket::IsTaskOfSavingDayLineDBFinished() {
	static bool s_bTaskOfSavingDayLineFinished = false;
	if (s_bTaskOfSavingDayLineFinished) {
		if ((!gl_dataContainerChinaStock.IsDayLineNeedSaving()) && (!gl_dataContainerChinaStock.IsDayLineNeedUpdate()) && (!IsDayLineNeedProcess())) {
			s_bTaskOfSavingDayLineFinished = false;
			TRACE("日线历史数据更新完毕\n");
			const CString str = "中国市场日线历史数据更新完毕";
			gl_systemMessage.PushInformationMessage(str);
			if (gl_dataContainerChinaStock.IsDayLineDBUpdated()) { // 更新股票池数据库
				gl_dataContainerChinaStock.ClearDayLineDBUpdatedFlag();
			}
			return true;
		}
	}
	else {
		if (gl_dataContainerChinaStock.IsDayLineNeedUpdate() || IsDayLineNeedProcess() || gl_dataContainerChinaStock.IsDayLineNeedSaving()) {
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
	static bool s_bWebBusy = false;
	bool bWebBusy = false;
	switch (gl_systemConfiguration.GetChinaMarketRealtimeServer()) {
	case 0: // 新浪实时数据
		bWebBusy = gl_pSinaRTDataSource->IsWebBusy();
		break;
	case 1: // 更新网易实时数据读取时间
		bWebBusy = gl_pNeteaseRTDataSource->IsWebBusy();
		break;
	case 2: // 更新腾讯实时数据读取时间
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

bool CChinaMarket::CheckMarketOpen(long lCurrentTime) {
	if (!IsWorkingDay()) { //周六或者周日闭市。结构tm用0--6表示星期日至星期六
		m_fMarketOpened = false;
	}
	else if ((lCurrentTime > 92759) && (lCurrentTime < 150600)) {	// 市场结束接收数据的时间，皆定为150600（与停止存储临时数据的时间一样）
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

bool CChinaMarket::TaskUpdateStockProfileDB(long lCurrentTime) {
	AddTask(CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__, GetNextTime(lCurrentTime, 0, 5, 0));

	if (gl_dataContainerChinaStock.IsUpdateProfileDB()) {
		gl_runtime.background_executor()->post([] {
			gl_UpdateChinaMarketDB.acquire();
			gl_dataContainerChinaStock.UpdateStockProfileDB();
			gl_UpdateChinaMarketDB.release();
		});
		return true;
	}
	return false;
}

bool CChinaMarket::TaskUpdateOptionDB(long lCurrentTime) {
	AddTask(CHINA_MARKET_UPDATE_OPTION_DB__, GetNextTime(lCurrentTime, 0, 5, 0));

	gl_runtime.background_executor()->post([this] {
		gl_UpdateChinaMarketDB.acquire();
		this->UpdateOptionDB();
		gl_UpdateChinaMarketDB.release();
	});

	return true;
}

bool CChinaMarket::TaskUpdateChosenStockDB() {
	if (IsUpdateChosenStockDB()) {
		gl_runtime.background_executor()->post([this] {
			gl_UpdateChinaMarketDB.acquire();
			this->AppendChosenStockDB();
			gl_UpdateChinaMarketDB.release();
		});
		return true;
	}
	return false;
}

bool CChinaMarket::TaskShowCurrentTransaction() const {
	// 显示当前交易情况
	const CChinaStockPtr pCurrentStock = GetCurrentStock();

	if (pCurrentStock != nullptr) {
		if (pCurrentStock->IsRTDataCalculated()) {
			if (pCurrentStock->GetCurrentTransactionVolume() > 0) pCurrentStock->ReportGuadanTransaction();
			pCurrentStock->ReportGuadan();
			pCurrentStock->SetRTDataCalculated(false);
		}
	}

	return true;
}

bool CChinaMarket::TaskUpdateStockSection() {
	if (gl_dataContainerChinaStockSymbol.IsUpdateStockSection()) {
		gl_runtime.background_executor()->post([] {
			gl_UpdateChinaMarketDB.acquire();
			gl_dataContainerChinaStockSymbol.UpdateStockSectionDB();
			gl_UpdateChinaMarketDB.release();
		});
		gl_dataContainerChinaStockSymbol.SetUpdateStockSection(false);
		return true;
	}
	return false;
}

bool CChinaMarket::ChangeDayLineStockCodeTypeToStandard() {
	CSetDayLineExtendInfo setDayLineExtendInfo;

	setDayLineExtendInfo.Open();
	setDayLineExtendInfo.m_pDatabase->BeginTrans();
	while (!setDayLineExtendInfo.IsEOF()) {
		setDayLineExtendInfo.Edit();
		setDayLineExtendInfo.m_Symbol = XferSinaToStandard(setDayLineExtendInfo.m_Symbol).c_str();
		setDayLineExtendInfo.Update();
		setDayLineExtendInfo.MoveNext();
	}
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.Close();
	gl_systemMessage.PushInformationMessage("Updated dayLine extend info");
	return false;
}

void CChinaMarket::TaskProcessAndSaveDayLine(long lCurrentTime) {
	if (gl_systemConfiguration.IsExitingSystem()) return; // 如果退出系统的话则不再处理日线

	gl_runtime.background_executor()->post([this] {
		gl_UpdateChinaMarketDB.acquire();
		if (IsDayLineNeedProcess()) {
			this->ProcessDayLine();
		}

		// 判断是否存储日线库和股票代码库
		if (gl_dataContainerChinaStock.IsDayLineNeedSaving()) {
			gl_dataContainerChinaStock.SaveDayLineData();
		}
		gl_UpdateChinaMarketDB.release();
	});

	if (!IsTaskOfSavingDayLineDBFinished()) {// 当尚未更新完日线历史数据时
		AddTask(CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, GetNextTime(lCurrentTime, 0, 0, 10));
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
//	通过股票代码和市场代码设置当前选择股票
//
//////////////////////////////////////////////////////////////////////////////////////
void CChinaMarket::SetCurrentStock(const CString& strStockCode) {
	ASSERT(gl_dataContainerChinaStock.IsSymbol(strStockCode));
	const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(strStockCode);
	SetCurrentStock(pStock);
	ASSERT(m_pCurrentStock != nullptr);
	if (!m_pCurrentStock->IsDayLineLoaded()) {
		ASSERT(!m_pCurrentStock->IsWeekLineLoaded());
		AddImmediateTask(CHINA_MARKET_LOAD_CURRENT_STOCK_DAY_LINE__); // 装载日线历史数据
	}
}

//////////////////////////////////////////////////////////////////////////
//
// 设置当前操作的股票
//
// 设置相应的股票指针，装载其日线数据。
//
/////////////////////////////////////////////////////////////////////////
void CChinaMarket::SetCurrentStock(const CChinaStockPtr& pStock) {
	if (pStock == nullptr) {
		m_pCurrentStock = nullptr;
		return;
	}
	if (m_pCurrentStock != nullptr) {
		if (m_pCurrentStock->IsSameStock(pStock)) return;
	}

	m_pCurrentStock = pStock;
	SetCurrentStockChanged(true);
	m_pCurrentStock->SetDayLineLoaded(false); // 这里只是设置标识，实际装载日线由调度程序执行。
	m_pCurrentStock->SetWeekLineLoaded(false); // 这里只是设置标识，实际装载日线由调度程序执行。
	AddImmediateTask(CHINA_MARKET_LOAD_CURRENT_STOCK_DAY_LINE__); // 装载日线历史数据
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 使用当前日期的日线数据生成本周的周线数据。
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::BuildWeekLineOfCurrentWeek() {
	CContainerChinaDayLine dataChinaDayLine;
	CContainerChinaWeekLine dataChinaWeekLine;
	set<CString> setDayLineStockCode;
	set<CString> setWeekLineStockCode;
	const long lCurrentMonday = GetCurrentMonday(GetMarketDate());

	if (!LoadDayLine(dataChinaDayLine, GetMarketDate())) {
		return true; // 加载本日日线数据失败，周线数据无需处理。
	}
	const auto pDayLineData = dataChinaDayLine.GetContainer();

	gl_systemMessage.PushInformationMessage(_T("开始生成今日周线"));

	CreateStockCodeSet(setDayLineStockCode, dataChinaDayLine.GetContainer());

	DeleteCurrentWeekWeekLineBeforeTheDate(lCurrentMonday); // 从当前周周线表中清除掉本星期一之前的数据
	dataChinaWeekLine.LoadCurrentWeekLine();
	CreateStockCodeSet(setWeekLineStockCode, dataChinaWeekLine.GetContainer());

	for (const auto& pData : *pDayLineData) {
		if (!setWeekLineStockCode.contains(pData->GetStockSymbol())) {
			//周线数据容器中无此日线数据
			// 存储此日线数据至周线数据容器
			const auto pWeekLine = make_shared<CWeekLine>();
			pWeekLine->UpdateWeekLine(pData);
			dataChinaWeekLine.Add(pWeekLine);
		}
		else {
			// 更新周线数据容器
			dataChinaWeekLine.UpdateData(pData);
		}
	}

	// 清除之前的周线数据
	DeleteWeekLine(lCurrentMonday);
	// 存储周线数据值周线数据表
	dataChinaWeekLine.SaveDB(_T(""));
	// 清除当前周的数据
	DeleteCurrentWeekWeekLine();
	// 存储当前周数据于当前周数据表
	dataChinaWeekLine.SaveCurrentWeekLine();

	gl_systemMessage.PushInformationMessage(_T("生成今日周线任务完成"));

	return true;
}

bool CChinaMarket::CreateStockCodeSet(set<CString>& setStockCode, not_null<vector<CVirtualHistoryCandleExtendPtr>*> pvData) {
	vector<CString> vectorStockCode;

	for (const auto& pData : *pvData) {
		CString strStockSymbol = pData->GetStockSymbol();
		vectorStockCode.push_back(strStockSymbol);
	}
	setStockCode.insert(vectorStockCode.begin(), vectorStockCode.end());

	return true;
}

bool CChinaMarket::BuildCurrentWeekWeekLineTable() {
	try {
		const long lCurrentMonday = GetCurrentMonday(GetMarketDate());
		CSetWeekLineBasicInfo setWeekLineBasicInfo;
		CSetWeekLineExtendInfo setWeekLineExtendInfo;
		char buffer[10];
		CWeekLinePtr pWeekLine = nullptr;
		CContainerChinaWeekLine dataChinaWeekLine;

		DeleteCurrentWeekWeekLine();

		static_cast<void>(sprintf_s(buffer, _T("%08d"), lCurrentMonday));
		const CString strDate = buffer;
		setWeekLineBasicInfo.m_strFilter = _T("[Date] = ");
		setWeekLineBasicInfo.m_strFilter += strDate;
		setWeekLineBasicInfo.m_strSort = _T("[Symbol]");
		setWeekLineBasicInfo.Open();

		setWeekLineExtendInfo.m_strFilter = _T("[Date] = ");
		setWeekLineExtendInfo.m_strFilter += strDate;
		setWeekLineExtendInfo.m_strSort = _T("[Symbol]");
		setWeekLineExtendInfo.Open();

		while (!setWeekLineBasicInfo.IsEOF()) {
			pWeekLine = make_shared<CWeekLine>();
			pWeekLine->LoadBasicData(&setWeekLineBasicInfo);
			while (!setWeekLineExtendInfo.IsEOF() && (setWeekLineBasicInfo.m_Symbol > setWeekLineExtendInfo.m_Symbol)) { setWeekLineExtendInfo.MoveNext(); }
			if (setWeekLineExtendInfo.IsEOF()) { setWeekLineExtendInfo.MoveFirst(); }
			else if (setWeekLineBasicInfo.m_Symbol == setWeekLineExtendInfo.m_Symbol) {
				// 由于存在事后补数据的缘故，此两个表的股票可能不是一一对应
				pWeekLine->LoadExtendData(&setWeekLineExtendInfo);
				dataChinaWeekLine.Add(pWeekLine);
				setWeekLineExtendInfo.MoveNext();
			}
			else { setWeekLineExtendInfo.MoveFirst(); }
			setWeekLineBasicInfo.MoveNext();
		}

		dataChinaWeekLine.SaveCurrentWeekLine();
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}

	return true;
}

bool CChinaMarket::LoadDayLine(CContainerChinaDayLine& dataChinaDayLine, long lDate) const {
	CString strSQL;
	char pch[30];
	//CTime ctTime;
	CSetDayLineBasicInfo setDayLineBasicInfo;
	CSetDayLineExtendInfo setDayLineExtendInfo;

	static_cast<void>(sprintf_s(pch, _T("%08d"), lDate));
	const CString strDate = pch;
	setDayLineBasicInfo.m_strSort = _T("[Symbol]");
	setDayLineBasicInfo.m_strFilter = _T("[Date] =");
	setDayLineBasicInfo.m_strFilter += strDate;
	setDayLineBasicInfo.Open();
	if (setDayLineBasicInfo.IsEOF()) {
		// 数据集为空，表明此日没有交易
		setDayLineBasicInfo.Close();
		CString str = strDate;
		str += _T("日数据集为空，无需处理周线数据");
		gl_systemMessage.PushDayLineInfoMessage(str); // 采用同步机制报告信息
		return false;
	}
	setDayLineExtendInfo.m_strSort = _T("[Symbol]");
	setDayLineExtendInfo.m_strFilter = _T("[Date] =");
	setDayLineExtendInfo.m_strFilter += strDate;
	setDayLineExtendInfo.Open();
	setDayLineExtendInfo.m_pDatabase->BeginTrans();
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	while (!setDayLineBasicInfo.IsEOF()) {
		auto pDayLine = make_shared<CDayLine>();
		pDayLine->LoadBasicData(&setDayLineBasicInfo);
		while (!setDayLineExtendInfo.IsEOF() && (strcmp(setDayLineExtendInfo.m_Symbol, setDayLineBasicInfo.m_Symbol) < 0)) {
			setDayLineExtendInfo.MoveNext();
		}
		if (!setDayLineExtendInfo.IsEOF() && (strcmp(setDayLineExtendInfo.m_Symbol, setDayLineBasicInfo.m_Symbol) == 0)) {
			pDayLine->LoadExtendData(&setDayLineExtendInfo);
		}
		dataChinaDayLine.Add(pDayLine);
		setDayLineBasicInfo.MoveNext();
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();
	setDayLineExtendInfo.Close();

	return true;
}

void CChinaMarket::DeleteWeekLine(long lMonday) {
	DeleteWeekLineBasicInfo(lMonday);
	DeleteWeekLineExtendInfo(lMonday);
}

bool CChinaMarket::DeleteWeekLine() {
	if (!gl_systemConfiguration.IsWorkingMode()) {
		ASSERT(0); // 由于处理实际数据库，故不允许测试此函数
		exit(1);
	}
	DeleteWeekLineBasicInfo();
	DeleteWeekLineExtendInfo();
	return true;
}

void CChinaMarket::DeleteWeekLineBasicInfo() {
	CDatabase database;

	if (!gl_systemConfiguration.IsWorkingMode()) {
		ASSERT(0); // 由于处理实际数据库，故不允许测试此函数
		exit(1);
	}

	database.Open(_T("ChinaMarket"), FALSE, FALSE, _T("ODBC;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `chinamarket`.`weekline`;"));
	database.CommitTrans();
	database.Close();
}

void CChinaMarket::DeleteWeekLineExtendInfo() {
	CDatabase database;

	if (!gl_systemConfiguration.IsWorkingMode()) {
		ASSERT(0); // 由于处理实际数据库，故不允许测试此函数
		exit(1);
	}

	database.Open(_T("ChinaMarket"), FALSE, FALSE, _T("ODBC;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `chinamarket`.`weeklineinfo`;"));
	database.CommitTrans();
	database.Close();
}

void CChinaMarket::DeleteWeekLineBasicInfo(long lMonday) const {
	CString strSQL;
	char pch[30];
	//CTime ctTime;
	CSetWeekLineBasicInfo setWeekLineBasicInfo;

	sprintf_s(pch, _T("%08d"), lMonday);
	const CString strDate = pch;
	setWeekLineBasicInfo.m_strFilter = _T("[Date] =");
	setWeekLineBasicInfo.m_strFilter += strDate;
	setWeekLineBasicInfo.Open();
	setWeekLineBasicInfo.m_pDatabase->BeginTrans();
	while (!setWeekLineBasicInfo.IsEOF()) {
		setWeekLineBasicInfo.Delete();
		setWeekLineBasicInfo.MoveNext();
	}
	setWeekLineBasicInfo.m_pDatabase->CommitTrans();
	setWeekLineBasicInfo.Close();
}

void CChinaMarket::DeleteWeekLineExtendInfo(long lMonday) const {
	CString strSQL;
	char pch[30];
	CSetWeekLineExtendInfo setWeekLineExtendInfo;

	sprintf_s(pch, _T("%08d"), lMonday);
	const CString strDate = pch;
	setWeekLineExtendInfo.m_strFilter = _T("[Date] =");
	setWeekLineExtendInfo.m_strFilter += strDate;
	setWeekLineExtendInfo.Open();
	setWeekLineExtendInfo.m_pDatabase->BeginTrans();
	while (!setWeekLineExtendInfo.IsEOF()) {
		setWeekLineExtendInfo.Delete();
		setWeekLineExtendInfo.MoveNext();
	}
	setWeekLineExtendInfo.m_pDatabase->CommitTrans();
	setWeekLineExtendInfo.Close();
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
		pStock->SetDayLineNeedSaving(true); // 设置存储日线标识

		succeed = gl_qDayLine.try_dequeue(pData);
	}
	return true;
}

void CChinaMarket::Choice10RSStrongStockSet() {
	for (int i = 0; i < 10; i++) {
		if (m_aRSStrongOption.at(i).m_fActive) {
			auto pref = &m_aRSStrongOption.at(i);
			gl_runtime.background_executor()->post([pref, i] {
				ThreadChoice10RSStrongStockSet(pref, i);
			});
		}
	}
	SetUpdatedDateFor10DaysRS(GetMarketDate());
	SetUpdateOptionDB(true); // 更新选项数据库.此时计算工作线程只是刚刚启动，需要时间去完成。
}

void CChinaMarket::DeleteDayLine(long lDate) const {
	DeleteDayLineBasicInfo(lDate);
	DeleteDayLineExtendInfo(lDate);
}

void CChinaMarket::DeleteDayLineBasicInfo(long lDate) const {
	char buffer[20]{0x000};
	CSetDayLineBasicInfo setDayLineBasicInfo;

	_ltoa_s(lDate, buffer, 10);
	const CString strDate = buffer;
	setDayLineBasicInfo.m_strFilter = _T("[Date] =");
	setDayLineBasicInfo.m_strFilter += strDate;
	setDayLineBasicInfo.Open();
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	while (!setDayLineBasicInfo.IsEOF()) {
		setDayLineBasicInfo.Delete();
		setDayLineBasicInfo.MoveNext();
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();
}

void CChinaMarket::DeleteDayLineExtendInfo(long lDate) const {
	char buffer[20]{0x000};
	CSetDayLineExtendInfo setDayLineExtendInfo;

	_ltoa_s(lDate, buffer, 10);
	const CString strDate = buffer;
	setDayLineExtendInfo.m_strFilter = _T("[Date] =");
	setDayLineExtendInfo.m_strFilter += strDate;
	setDayLineExtendInfo.Open();
	setDayLineExtendInfo.m_pDatabase->BeginTrans();
	while (!setDayLineExtendInfo.IsEOF()) {
		setDayLineExtendInfo.Delete();
		setDayLineExtendInfo.MoveNext();
	}
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.Close();
}

bool CChinaMarket::TaskLoadTempRTData(long lTheDate, long lCurrentTime) {
	ASSERT(!m_fTodayTempDataLoaded);

	if (IsSystemReady()) {
		gl_runtime.background_executor()->post([this, lTheDate] {
			gl_ProcessChinaMarketRTData.acquire();
			this->LoadTempRTData(lTheDate);
			gl_ProcessChinaMarketRTData.release();
		});
		return true;
	}
	AddTask(CHINA_MARKET_LOAD_TEMP_RT_DATA__, GetNextSecond(lCurrentTime));
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入暂存的当前数据，重置分析的初始态。这样当在开市时系统退出时，不至于损失掉所有已分析的数据
//
// 在设置m_lUnknownVolume为记录集中的m_UnknownVolume - m_Volume，这是因为第一次计算时只是初始化系统。
// 需要设置m_lUnknownVolume = pRTData->m_lVolume - setDayLineTemp.m_Volume + setDayLineTemp.m_UnknownVolume
// 而第一次执行计算实时数据时，只是初始化系统环境，其中设置m_lUnknownVolume += pRTData->GetVolume
// 故而此处这样计算。
/////////////////////////////////////////////////////////////////////////////////////////////
void CChinaMarket::LoadTempRTData(long lTheDate) {
	CSetDayLineTodaySaved setDayLineTemp;
	// 读取今日生成的数据于DayLineToday表中。
	setDayLineTemp.m_strSort = _T("[ID]");
	setDayLineTemp.Open();
	if (!setDayLineTemp.IsEOF()) {
		while (!setDayLineTemp.IsEOF()) {
			if (setDayLineTemp.m_Date == lTheDate && gl_dataContainerChinaStock.IsSymbol(setDayLineTemp.m_Symbol)) {// 如果是当天的行情，则载入，否则放弃
				const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(setDayLineTemp.m_Symbol);
				ASSERT(!pStock->HaveFirstRTData()); // 确保没有开始计算实时数据
				pStock->LoadTodaySavedInfo(&setDayLineTemp);
			}
			setDayLineTemp.MoveNext();
		}
	}
	setDayLineTemp.Close();

	ASSERT(m_fTodayTempDataLoaded == false);
	m_fTodayTempDataLoaded = true;
}

bool CChinaMarket::Load10DaysRSStrong1StockSet() {
	CSetRSStrong1Stock setRSStrong1;

	m_v10RSStrong1Stock.clear();
	setRSStrong1.Open();
	while (!setRSStrong1.IsEOF()) {
		if (gl_dataContainerChinaStock.IsSymbol(setRSStrong1.m_Symbol)) {
			CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(setRSStrong1.m_Symbol);
			m_v10RSStrong1Stock.push_back(pStock);
		}
		setRSStrong1.MoveNext();
	}
	setRSStrong1.Close();

	return true;
}

bool CChinaMarket::Load10DaysRSStrong2StockSet() {
	CSetRSStrong2Stock setRSStrong2;

	m_v10RSStrong2Stock.clear();
	setRSStrong2.Open();
	while (!setRSStrong2.IsEOF()) {
		if (gl_dataContainerChinaStock.IsSymbol(setRSStrong2.m_Symbol)) {
			CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(setRSStrong2.m_Symbol);
			m_v10RSStrong2Stock.push_back(pStock);
		}
		setRSStrong2.MoveNext();
	}
	setRSStrong2.Close();

	return true;
}

bool CChinaMarket::LoadCalculatingRSOption() {
	CSetRSOption setRSOption;

	setRSOption.Open();
	while (!setRSOption.IsEOF()) {
		m_aRSStrongOption.at(setRSOption.m_Index).m_fActive = setRSOption.m_Active;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lDayLength[0] = setRSOption.m_DayLengthFirst;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lDayLength[1] = setRSOption.m_DayLengthSecond;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lDayLength[2] = setRSOption.m_DayLengthThird;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lDayLength[3] = setRSOption.m_DayLengthFourth;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lStrongDayLength[0] = setRSOption.m_StrongDayLengthFirst;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lStrongDayLength[1] = setRSOption.m_StrongDayLengthSecond;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lStrongDayLength[2] = setRSOption.m_StrongDayLengthThird;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lStrongDayLength[3] = setRSOption.m_StrongDayLengthFourth;
		m_aRSStrongOption.at(setRSOption.m_Index).m_dRSStrong[0] = atof(setRSOption.m_RSStrongFirst);
		m_aRSStrongOption.at(setRSOption.m_Index).m_dRSStrong[1] = atof(setRSOption.m_RSStrongSecond);
		m_aRSStrongOption.at(setRSOption.m_Index).m_dRSStrong[2] = atof(setRSOption.m_RSStrongThird);
		m_aRSStrongOption.at(setRSOption.m_Index).m_dRSStrong[3] = atof(setRSOption.m_RSStrongFourth);
		setRSOption.MoveNext();
	}
	setRSOption.Close();
	return true;
}

void CChinaMarket::SaveCalculatingRSOption() const {
	CSetRSOption setRSOption;

	setRSOption.Open();
	setRSOption.m_pDatabase->BeginTrans();
	while (!setRSOption.IsEOF()) {
		setRSOption.Delete();
		setRSOption.MoveNext();
	}
	setRSOption.m_pDatabase->CommitTrans();
	setRSOption.Close();

	setRSOption.m_strFilter = _T("[ID] = 1");
	setRSOption.Open();
	setRSOption.m_pDatabase->BeginTrans();
	for (int i = 0; i < 10; i++) {
		setRSOption.AddNew();
		setRSOption.m_Index = i;
		setRSOption.m_Active = m_aRSStrongOption.at(i).m_fActive;
		setRSOption.m_DayLengthFirst = m_aRSStrongOption.at(i).m_lDayLength[0];
		setRSOption.m_DayLengthSecond = m_aRSStrongOption.at(i).m_lDayLength[1];
		setRSOption.m_DayLengthThird = m_aRSStrongOption.at(i).m_lDayLength[2];
		setRSOption.m_DayLengthFourth = m_aRSStrongOption.at(i).m_lDayLength[3];
		setRSOption.m_StrongDayLengthFirst = m_aRSStrongOption.at(i).m_lStrongDayLength[0];
		setRSOption.m_StrongDayLengthSecond = m_aRSStrongOption.at(i).m_lStrongDayLength[1];
		setRSOption.m_StrongDayLengthThird = m_aRSStrongOption.at(i).m_lStrongDayLength[2];
		setRSOption.m_StrongDayLengthFourth = m_aRSStrongOption.at(i).m_lStrongDayLength[3];
		setRSOption.m_RSStrongFirst = ConvertValueToString(m_aRSStrongOption.at(i).m_dRSStrong[0]);
		setRSOption.m_RSStrongSecond = ConvertValueToString(m_aRSStrongOption.at(i).m_dRSStrong[1]);
		setRSOption.m_RSStrongThird = ConvertValueToString(m_aRSStrongOption.at(i).m_dRSStrong[2]);
		setRSOption.m_RSStrongFourth = ConvertValueToString(m_aRSStrongOption.at(i).m_dRSStrong[3]);
		setRSOption.Update();
	}
	setRSOption.m_pDatabase->CommitTrans();
	setRSOption.Close();
}

bool CChinaMarket::Load10DaysRSStrongStockDB() {
	for (int i = 0; i < 10; i++) { LoadOne10DaysRSStrongStockDB(i); }
	return true;
}

bool CChinaMarket::LoadOne10DaysRSStrongStockDB(long lIndex) {
	m_lCurrentRSStrongIndex = lIndex;
	CSetRSStrongStock setRSStrongStock(lIndex);

	setRSStrongStock.Open();
	while (!setRSStrongStock.IsEOF()) {
		if (gl_dataContainerChinaStock.IsSymbol(setRSStrongStock.m_Symbol)) {
			CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(setRSStrongStock.m_Symbol);
			m_avChosenStock.at(m_lCurrentRSStrongIndex + c_10DaysRSStockSetStartPosition).push_back(pStock);
			// 10日RS股票集起始位置为第10个。
		}
		setRSStrongStock.MoveNext();
	}
	setRSStrongStock.Close();

	return true;
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
			setOption.m_RSEndDate = GetRSEndDate();
			setOption.m_RSStartDate = GetRSStartDate();
			setOption.m_LastLoginDate = GetMarketDate();
			setOption.m_LastLoginTime = GetMarketTime();
			setOption.m_UpdatedDateFor10DaysRS1 = GetUpdatedDateFor10DaysRS1();
			setOption.m_UpdatedDateFor10DaysRS2 = GetUpdatedDateFor10DaysRS2();
			setOption.m_UpdatedDateFor10DaysRS = GetUpdatedDateFor10DaysRS();
			setOption.Update();
		}
		else {
			setOption.Edit();
			setOption.m_RSEndDate = GetRSEndDate();
			setOption.m_RSStartDate = GetRSStartDate();
			setOption.m_LastLoginDate = GetMarketDate();
			setOption.m_LastLoginTime = GetMarketTime();
			setOption.m_UpdatedDateFor10DaysRS1 = GetUpdatedDateFor10DaysRS1();
			setOption.m_UpdatedDateFor10DaysRS2 = GetUpdatedDateFor10DaysRS2();
			setOption.m_UpdatedDateFor10DaysRS = GetUpdatedDateFor10DaysRS();
			setOption.Update();
		}
		setOption.m_pDatabase->CommitTrans();
		setOption.Close();
	}
	//catch (CException* e) {
	//ReportInformationAndDeleteException(e);
	//}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

void CChinaMarket::LoadOptionDB() {
	CSetOption setOption;
	setOption.Open();
	if (setOption.IsEOF()) {
		SetRSStartDate(_CHINA_MARKET_BEGIN_DATE_);
		SetRSEndDate(_CHINA_MARKET_BEGIN_DATE_);
		SetLastLoginDate(_CHINA_MARKET_BEGIN_DATE_);
		SetUpdatedDateFor10DaysRS1(_CHINA_MARKET_BEGIN_DATE_);
		SetUpdatedDateFor10DaysRS2(_CHINA_MARKET_BEGIN_DATE_);
	}
	else {
		if (setOption.m_RSEndDate == 0) {
			SetRSEndDate(_CHINA_MARKET_BEGIN_DATE_);
		}
		else {
			SetRSEndDate(setOption.m_RSEndDate);
		}
		if (setOption.m_RSStartDate == 0) {
			SetRSStartDate(_CHINA_MARKET_BEGIN_DATE_);
		}
		else {
			SetRSStartDate(setOption.m_RSStartDate);
		}
		if (setOption.m_LastLoginDate == 0) {
			SetLastLoginDate(_CHINA_MARKET_BEGIN_DATE_);
		}
		else {
			SetLastLoginDate(setOption.m_LastLoginDate);
		}
		SetLastLoginTime(setOption.m_LastLoginTime);
		SetUpdatedDateFor10DaysRS1(setOption.m_UpdatedDateFor10DaysRS1);
		SetUpdatedDateFor10DaysRS2(setOption.m_UpdatedDateFor10DaysRS2);
		SetUpdatedDateFor10DaysRS(setOption.m_UpdatedDateFor10DaysRS);
		if (setOption.m_UpdatedDateFor10DaysRS1 < GetMarketDate()) m_fChosen10RSStrong1StockSet = false;
		else m_fChosen10RSStrong1StockSet = true;
		if (setOption.m_UpdatedDateFor10DaysRS2 < GetMarketDate()) m_fChosen10RSStrong2StockSet = false;
		else m_fChosen10RSStrong2StockSet = true;
		if (setOption.m_UpdatedDateFor10DaysRS < GetMarketDate()) m_fChosen10RSStrongStockSet = false;
		else m_fChosen10RSStrongStockSet = true;
	}

	setOption.Close();
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
			setChinaChosenStock.m_Symbol = pStock->GetSymbol();
			setChinaChosenStock.Update();
			pStock->SetSaveToChosenStockDB(true);
		}
		setChinaChosenStock.m_pDatabase->CommitTrans();
		setChinaChosenStock.Close();
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

void CChinaMarket::AppendChosenStockDB() {
	try {
		CSetChinaChosenStock setChinaChosenStock;

		setChinaChosenStock.Open();
		setChinaChosenStock.m_pDatabase->BeginTrans();
		for (const auto& pStock : m_avChosenStock.at(0)) {
			ASSERT(pStock->IsChosen());
			if (!pStock->IsSaveToChosenStockDB()) {
				setChinaChosenStock.AddNew();
				setChinaChosenStock.m_Symbol = pStock->GetSymbol();
				setChinaChosenStock.Update();
				pStock->SetSaveToChosenStockDB(true);
			}
		}
		setChinaChosenStock.m_pDatabase->CommitTrans();
		setChinaChosenStock.Close();
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

void CChinaMarket::LoadChosenStockDB() {
	CSetChinaChosenStock setChinaChosenStock;

	setChinaChosenStock.Open();
	// 装入股票代码数据库
	while (!setChinaChosenStock.IsEOF()) {
		CChinaStockPtr pStock = nullptr;
		if (gl_dataContainerChinaStock.IsSymbol(setChinaChosenStock.m_Symbol)) {
			pStock = gl_dataContainerChinaStock.GetStock(setChinaChosenStock.m_Symbol);
			if (std::ranges::count(m_avChosenStock.at(0).begin(), m_avChosenStock.at(0).end(), pStock) == 0) {
				m_avChosenStock.at(0).push_back(pStock);
			}
			pStock->SetChosen(true);
			pStock->SetSaveToChosenStockDB(true);
		}
		setChinaChosenStock.MoveNext();
	}
	setChinaChosenStock.Close();
}

void CChinaMarket::ResetEffectiveRTDataRatio() {
	ResetCurrentEffectiveRTDataRatio();
}
