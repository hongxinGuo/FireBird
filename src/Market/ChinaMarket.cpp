#include"pch.h"

#include"ConvertToString.h"
#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"
#include"Thread.h"

#include"ThreadStatus.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

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

using namespace std;

#include<gsl/gsl>
using namespace gsl;

CChinaMarket::CChinaMarket() : CVirtualMarket() {
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

	AddTask(CREATE_TASK__, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 全局变量的解析位于程序退出的最后，要晚于CMainFrame的解析。故而如果要想将系统退出的过程放在这里，需要研究。
// 目前不允许此析构函数完成任何功能。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CChinaMarket::~CChinaMarket() {}

void CChinaMarket::ResetMarket() {
	CString str = _T("重置中国股市于北京标准时间：");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);
	gl_ProcessChinaMarketRTData.acquire();
	while (gl_ThreadStatus.IsSavingThreadRunning()) { Sleep(1); }

	Reset();

	LoadStockProfileDB();
	LoadOptionDB();
	LoadChosenStockDB();
	Load10DaysRSStrong1StockSet();
	Load10DaysRSStrong2StockSet();
	LoadCalculatingRSOption();
	Load10DaysRSStrongStockDB();

	gl_ProcessChinaMarketRTData.release();
}

void CChinaMarket::Reset() {
	CalculateTime(); // 初始化市场时间
	SetSystemReady(false); // 市场初始状态为未设置好。

	m_containerChinaStock.Reset();
	m_containerStockSymbol.Reset();

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
	m_lRTDataReceivedInOrdinaryTradeTime = 0;
	m_lNewRTDataReceivedInOrdinaryTradeTime = 0;

	while (m_qSinaRT.Size() > 0) m_qSinaRT.PopData();
	while (m_qNeteaseRT.Size() > 0) m_qNeteaseRT.PopData();
	while (m_qTengxunRT.Size() > 0) m_qTengxunRT.PopData();
	while (m_qDayLine.Size() > 0) m_qDayLine.PopData();

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

	m_fLoadedSelectedStock = false;

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

bool CChinaMarket::PreparingExitMarket() {
	ASSERT(gl_systemStatus.IsExitingSystem());

	return true;
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

bool CChinaMarket::IsOrdinaryTradeTime() {
	return IsOrdinaryTradeTime(GetMarketTime());
}

bool CChinaMarket::IsWorkingTime(const long lTime) {
	if (!IsWorkingDay()) return false;
	if (lTime < 91200) return false;
	if ((lTime > 114500) && (lTime < 124500)) return false;
	if (lTime > 150630) return false;
	return true;
}

bool CChinaMarket::IsWorkingTime() {
	return IsWorkingTime(GetMarketTime());
}

bool CChinaMarket::IsDummyTime(long lTime) {
	return !IsWorkingTime(lTime);
}

bool CChinaMarket::IsDummyTime() {
	return !IsWorkingTime();
}

bool CChinaMarket::TaskCheckMarketReady(long lCurrentTime) {
	if (!IsSystemReady()) {
		if (IsResetMarket()) return false;
		const auto lMax = GetTotalStock() > 12000 ? GetTotalStock() * 2 : 24000;
		if (m_llRTDataReceived > lMax) {
			SetSystemReady(true);
			gl_systemMessage.PushInformationMessage(_T("中国股票市场初始化完毕"));
		}
	}
	if (!IsSystemReady()) AddTask(CHINA_MARKET_CHECK_SYSTEM_READY__, GetNextSecond(lCurrentTime));

	return IsSystemReady();
}

bool CChinaMarket::ChangeToNextStock() {
	ASSERT(m_pCurrentStock != nullptr);
	long lIndex = GetStockIndex(m_pCurrentStock);
	CChinaStockPtr pStock = m_pCurrentStock;

	if (IsTotalStockSetSelected()) {
		bool fFound = false;
		while (!fFound) {
			if (++lIndex == GetTotalStock()) {
				lIndex = 0;
			}
			pStock = GetStock(lIndex);
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
	return true;
}

bool CChinaMarket::ChangeToPrevStock() {
	ASSERT(m_pCurrentStock != nullptr);
	long lIndex = GetStockIndex(m_pCurrentStock);
	CChinaStockPtr pStock = m_pCurrentStock;

	if (IsTotalStockSetSelected()) {
		bool fFound = false;
		while (!fFound) {
			if (--lIndex < 0) {
				lIndex = GetTotalStock() - 1;
			}
			pStock = GetStock(lIndex);
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
	return true;
}

bool CChinaMarket::ChangeToPrevStockSet() {
	do {
		if (m_lCurrentSelectedStockSet > -1) m_lCurrentSelectedStockSet--;
		else { m_lCurrentSelectedStockSet = c_10DaysRSStockSetStartPosition + 9; }
		ASSERT(m_lCurrentSelectedStockSet < 20);
	}
	while ((m_lCurrentSelectedStockSet != -1) && (m_avChosenStock.at(m_lCurrentSelectedStockSet).empty()));

	return true;
}

bool CChinaMarket::ChangeToNextStockSet() {
	do {
		if (m_lCurrentSelectedStockSet == (c_10DaysRSStockSetStartPosition + 9)) m_lCurrentSelectedStockSet = -1;
		else { m_lCurrentSelectedStockSet++; }
		ASSERT(m_lCurrentSelectedStockSet < 20);
	}
	while ((m_lCurrentSelectedStockSet != -1) && (m_avChosenStock.at(m_lCurrentSelectedStockSet).empty()));

	return true;
}

size_t CChinaMarket::GetCurrentStockSetSize() {
	if (IsTotalStockSetSelected()) return GetTotalStock();
	return m_avChosenStock.at(m_lCurrentSelectedStockSet).size();
}

bool CChinaMarket::CreateStock(CString strStockCode, CString strStockName, bool fProcessRTData) {
	const auto pStock = make_shared<CChinaStock>();
	pStock->SetTodayNewStock(true);
	pStock->SetSymbol(strStockCode);
	pStock->SetDisplaySymbol(strStockName);
	pStock->SetIPOStatus(_STOCK_NOT_CHECKED_);
	pStock->SetDayLineEndDate(19900101);
	pStock->SetDayLineStartDate(19900101);
	pStock->SetUpdateProfileDB(true);
	pStock->SetNeedProcessRTData(fProcessRTData);
	AddStock(pStock);
	ASSERT(pStock->IsDayLineNeedUpdate());
	const CString str = _T("china Market生成新代码") + pStock->GetSymbol();
	gl_systemMessage.PushInnerSystemInformationMessage(str);
	return true;
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
long CChinaMarket::GetMinLineOffset(time_t tUTC) {
	ASSERT(tUTC >= 0);
	tm tmMarketTime;

	tmMarketTime = TransferToMarketTime(tUTC);
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
// 处理实时数据等，由SchedulingTaskPerSecond函数调用,每三秒执行一次。
// 将实时数据暂存队列中的数据分别存放到各自股票的实时队列中。
// 分发数据时，只分发新的（交易时间晚于之前数据的）实时数据。
//
// 此函数用到大量的全局变量，还是放在主线程为好。工作线程目前还是只做计算个股的挂单情况。
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::DistributeSinaRTDataToStock() {
	const size_t lTotalNumber = SinaRTSize();
	CString strVolume;
	CString strStandardStockCode;
	CChinaStockPtr pStock = nullptr;

	if (IsOrdinaryTradeTime()) m_lRTDataReceivedInOrdinaryTradeTime += lTotalNumber;
	gl_pChinaMarket->IncreaseRTDataReceived(lTotalNumber);

	for (int iCount = 0; iCount < lTotalNumber; iCount++) {
		const CWebRTDataPtr pRTData = PopSinaRT();
		if (pRTData->GetDataSource() == INVALID_RT_WEB_DATA_) {
			gl_systemMessage.PushInnerSystemInformationMessage(_T("新浪实时数据源设置有误"));
			continue;
		}
		DistributeRTDataToStock(pRTData);
	}
	SetRTDataNeedCalculate(true); // 设置接收到实时数据标识
	// 由于使用线程Parse新浪实时数据，故此处不再检测队列为零
	//ASSERT(gl_WebRTDataContainer.SinaDataSize() == 0); // 必须一次处理全体数据。

	return true;
}

bool CChinaMarket::DistributeRTDataToStock(CWebRTDataPtr pRTData) {
	const CString strSymbol = pRTData->GetSymbol();
	if (IsCheckingActiveStock()) {
		if (!IsStock(strSymbol) && pRTData->IsActive()) {
			if (strSymbol.GetLength() == 9) {
				CreateStock(strSymbol, pRTData->GetStockName(), true);
			}
		}
	}
	else if (!IsStock(pRTData->GetSymbol())) { return false; }
	if (pRTData->IsActive()) { // 此实时数据有效？
		if (m_ttNewestTransactionTime < pRTData->GetTransactionTime()) { m_ttNewestTransactionTime = pRTData->GetTransactionTime(); }
		const auto pStock = GetStock(pRTData->GetSymbol());
		if (!pStock->IsActive()) {
			if (pRTData->IsValidTime(14)) {
				pStock->UpdateProfile(pRTData);
			}
		}
		if (pRTData->GetTransactionTime() > pStock->GetTransactionTime()) { // 新的数据？
			if (IsOrdinaryTradeTime()) m_lNewRTDataReceivedInOrdinaryTradeTime++;
			pStock->PushRTData(pRTData); // 存储新的数据至数据池
			pStock->SetTransactionTime(pRTData->GetTransactionTime()); // 设置最新接受到实时数据的时间
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 处理实时数据等，由SchedulingTaskPerSecond函数调用,每三秒执行一次。
// 将实时数据暂存队列中的数据分别存放到各自股票的实时队列中。
// 分发数据时，只分发新的（交易时间晚于之前数据的）实时数据。
//
// 此函数用到大量的全局变量，还是放在主线程为好。工作线程目前还是只做计算个股的挂单情况。
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::DistributeNeteaseRTDataToStock() {
	CChinaStockPtr pStock;
	const size_t lTotalNumber = NeteaseRTSize();
	CString strVolume;

	if (IsOrdinaryTradeTime()) m_lRTDataReceivedInOrdinaryTradeTime += lTotalNumber;
	gl_pChinaMarket->IncreaseRTDataReceived(lTotalNumber);

	for (int iCount = 0; iCount < lTotalNumber; iCount++) {
		const CWebRTDataPtr pRTData = PopNeteaseRT();
		if (pRTData->GetDataSource() == INVALID_RT_WEB_DATA_) {
			gl_systemMessage.PushErrorMessage(_T("网易实时数据源设置有误"));
			continue;
		}
		DistributeRTDataToStock(pRTData);
	}
	SetRTDataNeedCalculate(true); // 设置接收到实时数据标识
	// 由于使用线程Parse网易实时数据，故此处不再检测队列为零
	//ASSERT(gl_WebRTDataContainer.NeteaseDataSize() == 0); // 必须一次处理全体数据。

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 生成每次查询新浪实时股票数据的字符串
//
//////////////////////////////////////////////////////////////////////////////////////////
CString CChinaMarket::GetSinaStockInquiringStr(long lTotalNumber, bool fUsingTotalStockSet) {
	if (fUsingTotalStockSet) {
		return GetNextSinaStockInquiringMiddleStrFromTotalStockSet(lTotalNumber);
	}
	return GetNextSinaStockInquiringMiddleStr(lTotalNumber);
}

CString CChinaMarket::GetNeteaseStockInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet) {
	if (fUsingTotalStockSet) {
		return GetNextNeteaseStockInquiringMiddleStrFromTotalStockSet(lTotalNumber);
	}
	return GetNextNeteaseStockInquiringMiddleStr(lTotalNumber);
}

bool CChinaMarket::CheckValidOfNeteaseDayLineInquiringStr(const CString& str) {
	const CString strNetease = str.Left(7);
	CString strStockCode = XferNeteaseToStandard(strNetease);
	if (!IsStock(strStockCode)) {
		CString strReport = _T("网易日线查询股票代码错误：");
		TRACE(_T("网易日线查询股票代码错误：%s\n"), strStockCode.GetBuffer());
		strReport += strStockCode;
		gl_systemMessage.PushInnerSystemInformationMessage(strReport);
		return false;
	}

	return true;
}

bool CChinaMarket::ProcessTengxunRTData() {
	const size_t lTotalData = TengxunRTSize();

	for (int i = 0; i < lTotalData; i++) {
		const CWebRTDataPtr pRTData = PopTengxunRT();
		if (pRTData->IsActive()) {
			ASSERT(IsStock(pRTData->GetSymbol()));
			const auto pStock = GetStock(pRTData->GetSymbol());
			pStock->SetTotalValue(pRTData->GetTotalValue());
			pStock->SetCurrentValue(pRTData->GetCurrentValue());
			pStock->SetHighLimit(pRTData->GetHighLimit());
			pStock->SetLowLimit(pRTData->GetLowLimit());
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// 定时调度函数，每秒一次。
//
/////////////////////////////////////////////////////////////////////////////////////////////
//void CChinaMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
//ResetMarketFlagAtMidnight(lCurrentTime);
//CreateTaskOfReset();

//TaskShowCurrentTransaction();
//}

void CChinaMarket::TaskChoiceRSSet(long lCurrentTime) {
	if (m_fCalculateChosen10RS) {
		if (GetDayLineNeedUpdateNumber() <= 0 && GetDayLineNeedSaveNumber() <= 0) {
			TaskChoice10RSStrongStockSet(lCurrentTime);
			TaskChoice10RSStrong1StockSet(lCurrentTime);
			TaskChoice10RSStrong2StockSet(lCurrentTime);
		}
		else {
			AddTask(CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__, GetNextTime(lCurrentTime, 0, 1, 0));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将接收到的实时数据分发至各相关股票的实时数据队列中。
// 由于有多个数据源，故而需要等待各数据源都执行一次后，方可以分发至相关股票处，需要每三秒执行一次，以保证各数据源至少都能提供一次数据。
// 实时数据的计算过程必须位于分配过程之后，这样才能保证不会出现数据同步问题
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CChinaMarket::TaskDistributeAndCalculateRTData(long lCurrentTime) {
	DistributeSinaRTDataToStock();
	DistributeNeteaseRTDataToStock();
	if (IsSystemReady() && IsTodayTempRTDataLoaded() && IsRTDataNeedCalculate()) {
		CreateThreadProcessRTData();
		SetRTDataNeedCalculate(false);
	}
	AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, GetNextSecond(lCurrentTime));
}

bool CChinaMarket::ProcessTask(long lCurrentTime) {
	if (IsMarketTaskEmpty()) return false;
	const auto pTask = GetMarketTask();
	if (lCurrentTime >= pTask->GetTime()) {
		DiscardMarketTask();
		switch (pTask->GetType()) {
		case CREATE_TASK__: // 生成其他任务
			TaskCreateTask(lCurrentTime);
			break;
		case CHINA_MARKET_CHECK_SYSTEM_READY__:
			TaskCheckMarketReady(lCurrentTime);
			break;
		case CHINA_MARKET_RESET__: // 91300重启系统
			TaskResetMarket(lCurrentTime);
			break;
		case CHINA_MARKET_LOAD_TEMP_RT_DATA__:
			TaskLoadTempRTData(GetMarketDate(), lCurrentTime);
			break;
		case CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__:
			TaskDistributeAndCalculateRTData(lCurrentTime);
			break;
		case CHINA_MARKET_SAVE_TEMP_RT_DATA__:
			TaskSaveTempData(lCurrentTime);
			break;
		case CHINA_MARKET_BUILD_TODAY_DATABASE__:
			TaskProcessTodayStock(lCurrentTime);
			break;
		case CHINA_MARKET_VALIDATE_TODAY_DATABASE__:
			// todo not implemented
			break;
		case CHINA_MARKET_UPDATE_OPTION_DB__:
			TaskUpdateOptionDB(lCurrentTime);
			break;
		case CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__:
			TaskUpdateStockProfileDB(lCurrentTime);
			break;
		case CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__:
			TaskUpdateChosenStockDB();
			break;
		case CHINA_MARKET_UPDATE_STOCK_SECTION__:
			TaskUpdateStockSection();
			break;
		case CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__:
			TaskProcessAndSaveDayLine(lCurrentTime);
			break;
		case CHINA_MARKET_LOAD_CURRENT_STOCK_DAY_LINE__:
			TaskLoadCurrentStockHistoryData();
			break;
		case CHINA_MARKET_ACCESSORY_TASK__:
			TaskAccessoryTask(lCurrentTime);
			break;
		case CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__:
			TaskChoiceRSSet(lCurrentTime);
			break;
		default:
			ASSERT(0); // 非法任务
			break;
		}
		return true;
	}
	return false;
}

bool CChinaMarket::TaskCreateTask(long lCurrentTime) {
	const long lTimeMinute = (lCurrentTime / 100) * 100; // 当前小时和分钟

	while (!IsMarketTaskEmpty()) DiscardMarketTask();
	SetResetMarketPermission(false);

	// 系统初始化检查
	AddTask(CHINA_MARKET_CHECK_SYSTEM_READY__, 1);
	// 每秒一次分配实时数据
	AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, 1); // 开始执行时间为：1

	// 辅助任务
	AddTask(CHINA_MARKET_ACCESSORY_TASK__, lCurrentTime);

	// 初始化系统
	if (lCurrentTime < 91300) {
		AddTask(CHINA_MARKET_RESET__, 91300); // 执行时间为：91300
	}
	// 再次初始化系统
	if (lCurrentTime < 92600) {
		AddTask(CHINA_MARKET_RESET__, 92600); // 执行时间为：92600
	}

	// 装载本日之前存储的实时数据（如果有的话）
	if (lCurrentTime > 93000) { // 重启系统时的时间是1000，故而不设置此任务
		AddTask(CHINA_MARKET_LOAD_TEMP_RT_DATA__, 92630);
	}

	// 每十秒钟存储一次日线历史数据。
	AddTask(CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, 113510); // 中午休市时开始。

	// 每五分钟存储一次系统选项数据库
	AddTask(CHINA_MARKET_UPDATE_OPTION_DB__, GetNextTime(lTimeMinute + 5, 0, 3, 0)); // 开始执行时间为启动之后的三分钟。

	// 每五分钟存储一次股票简要数据库
	AddTask(CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__, GetNextTime(lTimeMinute + 10, 0, 4, 0)); // 开始执行时间为启动之后的四分钟。

	if (IsWorkingDay()) {
		// 每五分钟存储一次临时数据
		AddTask(CHINA_MARKET_SAVE_TEMP_RT_DATA__, 50230); // 开始执行时间为：92730.要确保第一次执行的时间早于93000，这样启动数据库的时间较短，否则容易导致系统崩溃（原因不明）。
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

	return true;
}

////////////////////////////////////////////////////////////////////////////////////
//
// 开市时每五分钟存储一次当前状态。这是一个备用措施，防止退出系统后就丢掉了所有的数据，不必太频繁。
//
/////////////////////////////////////////////////////////////////////////////////////////
void CChinaMarket::TaskSaveTempData(long lCurrentTime) {
	if (lCurrentTime < 170000) { // 中国市场股票交易截止时间为150000。
		long lNextTime = GetNextTime(lCurrentTime, 0, 5, 0);
		//if ((lNextTime >= 113500) && (lNextTime < 125730)) lNextTime = 125730;
		AddTask(CHINA_MARKET_SAVE_TEMP_RT_DATA__, lNextTime);
	}
	if (IsSystemReady()) {
		const CString str = "存储临时数据";
		gl_systemMessage.PushDayLineInfoMessage(str);
		CreateThreadUpdateTempRTData();
	}
}

void CChinaMarket::TaskLoadCurrentStockHistoryData() {
	if (m_pCurrentStock != nullptr) {
		if (!m_pCurrentStock->IsDayLineLoaded()) {
			CreateThreadLoadDayLine(m_pCurrentStock.get());
			m_pCurrentStock->SetDayLineLoaded(true);
		}
		if (!m_pCurrentStock->IsWeekLineLoaded()) {
			CreateThreadLoadWeekLine(m_pCurrentStock.get());
			m_pCurrentStock->SetWeekLineLoaded(true);
		}
	}
}

void CChinaMarket::TaskAccessoryTask(long lCurrentTime) {
	CheckFastReceivingData(lCurrentTime);
	CheckMarketOpen(lCurrentTime);// 判断中国股票市场开市状态
	SetCheckActiveStockFlag(lCurrentTime);

	AddTask(CHINA_MARKET_ACCESSORY_TASK__, GetNextTime(lCurrentTime, 0, 1, 0)); // 每分钟整点执行一次
}

bool CChinaMarket::AddChosenStock(CChinaStockPtr pStock) {
	if (ranges::count(m_avChosenStock.at(0).begin(), m_avChosenStock.at(0).end(), pStock) == 0) {
		m_avChosenStock.at(0).push_back(pStock);
		ASSERT(!pStock->IsSaveToChosenStockDB());
		return true;
	}
	return false;
}

bool CChinaMarket::DeleteChosenStock(CChinaStockPtr pStock) {
	const auto it = ranges::find(m_avChosenStock.at(0).cbegin(), m_avChosenStock.at(0).cend(), pStock);
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
		CreateThreadChoice10RSStrong1StockSet();
		m_fChosen10RSStrong1StockSet = true;
		return true;
	}
	return false;
}

bool CChinaMarket::TaskChoice10RSStrong2StockSet(long lCurrentTime) {
	if (IsSystemReady() && !m_fChosen10RSStrong2StockSet && (lCurrentTime > 151200) && IsWorkingDay()) {
		CreateThreadChoice10RSStrong2StockSet();
		m_fChosen10RSStrong2StockSet = true;
		return true;
	}
	return false;
}

bool CChinaMarket::TaskChoice10RSStrongStockSet(long lCurrentTime) {
	if (IsSystemReady() && !m_fChosen10RSStrongStockSet && (lCurrentTime > 151000) && IsWorkingDay()) {
		CreateThreadChoice10RSStrongStockSet();
		m_fChosen10RSStrongStockSet = true;
		return true;
	}
	return false;
}

bool CChinaMarket::TaskProcessTodayStock(long lCurrentTime) {
	if (IsSystemReady()) {
		CreateThreadProcessTodayStock();
		return true;
	}
	return false;
}

void CChinaMarket::ProcessTodayStock() {
	ASSERT(IsSystemReady()); // 调用本工作线程时必须设置好市场。

	const long lDate = TransferToMarketDate(GetNewestTransactionTime());
	if (lDate == GetMarketDate()) {
		BuildDayLine(lDate);
		// 计算本日日线相对强度
		BuildDayLineRS(lDate);
		// 生成周线数据
		BuildWeekLineOfCurrentWeek();
		BuildWeekLineRS(GetCurrentMonday(lDate));
		UpdateStockProfileDB();
		if (GetMarketTime() > 150400) {	// 如果中国股市闭市了
			SetRSEndDate(GetMarketDate());
			SetUpdateOptionDB(true); // 更新状态
		}
	}
	CString str = ConvertDateToChineseTimeStampString(lDate);
	str += _T("的实时数据处理完毕");
	gl_systemMessage.PushInformationMessage(str);
}

bool CChinaMarket::IsFinishedSavingDayLineDB() {
	static bool s_bHaveSavedDayLine = false;
	if (s_bHaveSavedDayLine && (!IsDayLineNeedSaving()) && (!IsDayLineNeedUpdate()) && (!IsDayLineNeedProcess())) {
		s_bHaveSavedDayLine = false;
		TRACE("日线历史数据更新完毕\n");
		const CString str = "中国市场日线历史数据更新完毕";
		gl_systemMessage.PushInformationMessage(str);
		if (IsDayLineDBUpdated()) { // 更新股票池数据库
			ClearDayLineDBUpdatedFlag();
		}
		return true;
	}
	if (!s_bHaveSavedDayLine) {
		if (IsDayLineNeedUpdate() || IsDayLineNeedProcess() || IsDayLineNeedSaving()) {
			s_bHaveSavedDayLine = true;
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
	SetResetMarket(true); // 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
	SetSystemReady(false);
	AddTask(CHINA_MARKET_CHECK_SYSTEM_READY__, lCurrentTime); // 每次重置系统时，必须进行系统初始化状态检查
	AddTask(CHINA_MARKET_UPDATE_STOCK_SECTION__, GetNextTime(lCurrentTime, 0, 5, 0)); // 五分钟后再更新此数据库

	return true;
}

bool CChinaMarket::TaskUpdateStockProfileDB(long lCurrentTime) {
	AddTask(CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__, GetNextTime(lCurrentTime, 0, 5, 0));

	if (IsUpdateStockProfileDB()) {
		CreateThreadUpdateStockProfileDB();
		return true;
	}
	return false;
}

bool CChinaMarket::TaskUpdateOptionDB(long lCurrentTime) {
	AddTask(CHINA_MARKET_UPDATE_OPTION_DB__, GetNextTime(lCurrentTime, 0, 5, 0));

	CreateThreadUpdateOptionDB();

	return true;
}

bool CChinaMarket::TaskUpdateChosenStockDB() {
	if (IsUpdateChosenStockDB()) {
		CreateThreadUpdateChoseStockDB();
		return true;
	}
	return false;
}

void CChinaMarket::CreateThreadUpdateChoseStockDB() {
	thread thread1(ThreadAppendChosenStockDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

bool CChinaMarket::TaskShowCurrentTransaction() {
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
	if (m_containerStockSymbol.IsUpdateStockSection()) {
		CreateThreadSaveStockSection();
		m_containerStockSymbol.SetUpdateStockSection(false);
		return true;
	}
	return false;
}

void CChinaMarket::CreateThreadSaveStockSection() {
	thread thread1(ThreadSaveStockSection, this);
	thread1.detach();
}

bool CChinaMarket::ChangeDayLineStockCodeToStandard() {
	CSetDayLineExtendInfo setDayLineExtendInfo;

	setDayLineExtendInfo.Open();
	setDayLineExtendInfo.m_pDatabase->BeginTrans();
	while (!setDayLineExtendInfo.IsEOF()) {
		setDayLineExtendInfo.Edit();
		setDayLineExtendInfo.m_Symbol = XferSinaToStandard(setDayLineExtendInfo.m_Symbol);
		setDayLineExtendInfo.Update();
		setDayLineExtendInfo.MoveNext();
	}
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.Close();
	gl_systemMessage.PushInformationMessage("Updated dayLine extend info");
	return false;
}

bool CChinaMarket::TaskProcessAndSaveDayLine(long lCurrentTime) {
	if (IsDayLineNeedProcess()) {
		ProcessDayLine();
	}

	// 判断是否存储日线库和股票代码库
	if (IsDayLineNeedSaving()) {
		SaveDayLineData();
	}

	if (!IsFinishedSavingDayLineDB()) {// 当尚未更新完日线历史数据时
		AddTask(CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, GetNextTime(lCurrentTime, 0, 0, 10));
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//	通过股票代码和市场代码设置当前选择股票
//
//////////////////////////////////////////////////////////////////////////////////////
void CChinaMarket::SetCurrentStock(const CString& strStockCode) {
	ASSERT(IsStock(strStockCode));
	const CChinaStockPtr pStock = GetStock(strStockCode);
	SetCurrentStock(pStock);
	ASSERT(m_pCurrentStock != nullptr);
	AddTask(CHINA_MARKET_LOAD_CURRENT_STOCK_DAY_LINE__, 1); // 装载日线历史数据
}

//////////////////////////////////////////////////////////////////////////
//
// 设置当前操作的股票
//
// 设置相应的股票指针，装载其日线数据。
//
/////////////////////////////////////////////////////////////////////////
void CChinaMarket::SetCurrentStock(CChinaStockPtr pStock) {
	bool fSet = false;

	if (pStock != nullptr) {
		if (m_pCurrentStock != nullptr) {
			if (!m_pCurrentStock->IsSameStock(pStock)) {
				fSet = true;
			}
		}
		else {
			fSet = true;
		}
	}
	else {
		m_pCurrentStock = nullptr;
	}
	if (fSet) {
		m_pCurrentStock = pStock;
		SetCurrentStockChanged(true);
		m_pCurrentStock->SetDayLineLoaded(false); // 这里只是设置标识，实际装载日线由调度程序执行。
	}
	AddTask(CHINA_MARKET_LOAD_CURRENT_STOCK_DAY_LINE__, 1); // 装载日线历史数据
}

void CChinaMarket::ResetCurrentStock() {
	if (m_pCurrentStock != nullptr) {
		m_pCurrentStock = nullptr;
	}
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
			dataChinaWeekLine.StoreData(pWeekLine);
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
			dataChinaWeekLine.StoreData(pWeekLine);
			setWeekLineExtendInfo.MoveNext();
		}
		else { setWeekLineExtendInfo.MoveFirst(); }
		setWeekLineBasicInfo.MoveNext();
	}

	dataChinaWeekLine.SaveCurrentWeekLine();

	return true;
}

bool CChinaMarket::LoadDayLine(CContainerChinaDayLine& dataChinaDayLine, long lDate) {
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
		dataChinaDayLine.StoreData(pDayLine);
		setDayLineBasicInfo.MoveNext();
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();
	setDayLineExtendInfo.Close();

	return true;
}

bool CChinaMarket::DeleteWeekLine(long lMonday) {
	DeleteWeekLineBasicInfo(lMonday);
	DeleteWeekLineExtendInfo(lMonday);

	return true;
}

bool CChinaMarket::DeleteWeekLine() {
	if (!gl_systemStatus.IsWorkingMode()) {
		ASSERT(0); // 由于处理实际数据库，故不允许测试此函数
		exit(1);
	}
	DeleteWeekLineBasicInfo();
	DeleteWeekLineExtendInfo();
	return true;
}

bool CChinaMarket::DeleteWeekLineBasicInfo() {
	CDatabase database;

	if (!gl_systemStatus.IsWorkingMode()) {
		ASSERT(0); // 由于处理实际数据库，故不允许测试此函数
		exit(1);
	}

	database.Open(_T("ChinaMarket"), FALSE, FALSE, _T("ODBC;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `chinamarket`.`weekline`;"));
	database.CommitTrans();
	database.Close();

	return true;
}

bool CChinaMarket::DeleteWeekLineExtendInfo() {
	CDatabase database;

	if (!gl_systemStatus.IsWorkingMode()) {
		ASSERT(0); // 由于处理实际数据库，故不允许测试此函数
		exit(1);
	}

	database.Open(_T("ChinaMarket"), FALSE, FALSE, _T("ODBC;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `chinamarket`.`weeklineinfo`;"));
	database.CommitTrans();
	database.Close();

	return true;
}

bool CChinaMarket::DeleteWeekLineBasicInfo(long lMonday) {
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

	return true;
}

bool CChinaMarket::DeleteWeekLineExtendInfo(long lMonday) {
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
	return GetStock(0);
}

bool CChinaMarket::IsDayLineNeedProcess() {
	if (DayLineQueueSize() > 0) return true;
	return false;
}

bool CChinaMarket::ProcessDayLine() {
	while (DayLineQueueSize() > 0) {
		CDayLineWebDataPtr pData = PopDayLine();
		ASSERT(gl_pChinaMarket->IsStock(pData->GetStockCode()));
		const CChinaStockPtr pStock = gl_pChinaMarket->GetStock(pData->GetStockCode());
		pStock->UpdateDayLine(pData->GetProcessedDayLine()); // pData的日线数据是正序的，最新日期的在最后面。
		pStock->UpdateStatusByDownloadedDayLine();

		pStock->SetDayLineLoaded(true);
		pStock->SetDayLineNeedSaving(true); // 设置存储日线标识

		pData = nullptr;
	}
	return true;
}

void CChinaMarket::CreateThreadProcessRTData() {
	thread thread1(ThreadProcessRTData, this);
	thread1.detach();
}

void CChinaMarket::CreateThreadProcessTodayStock() {
	thread thread1(ThreadProcessTodayStock, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CChinaMarket::CreateThreadBuildDayLineRS(long lStartCalculatingDay) {
	thread thread1(ThreadBuildDayLineRS, this, lStartCalculatingDay);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CChinaMarket::CreateThreadBuildDayLineRSOfDate(long lThisDay) {
	thread thread1(ThreadBuildDayLineRSOfDate, this, lThisDay);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CChinaMarket::CreateThreadBuildWeekLineRSOfDate(long lThisDay) {
	thread thread1(ThreadBuildWeekLineRSOfDate, this, lThisDay);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CChinaMarket::CreateThreadLoadDayLine(CChinaStock* pCurrentStock) {
	thread thread1(ThreadLoadDayLine, pCurrentStock);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CChinaMarket::CreateThreadLoadWeekLine(CChinaStock* pCurrentStock) {
	thread thread1(ThreadLoadWeekLine, pCurrentStock);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CChinaMarket::CreateThreadUpdateStockProfileDB() {
	thread thread1(ThreadUpdateChinaStockProfileDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CChinaMarket::CreateThreadUpdateOptionDB() {
	thread thread1(ThreadUpdateOptionDB, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CChinaMarket::CreateThreadChoice10RSStrong2StockSet() {
	thread thread1(ThreadChoice10RSStrong2StockSet, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CChinaMarket::CreateThreadChoice10RSStrong1StockSet() {
	thread thread1(ThreadChoice10RSStrong1StockSet, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

void CChinaMarket::CreateThreadChoice10RSStrongStockSet() {
	for (int i = 0; i < 10; i++) {
		if (m_aRSStrongOption.at(i).m_fActive) {
			thread thread1(ThreadChoice10RSStrongStockSet, this, &(m_aRSStrongOption.at(i)), i);
			thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
		}
	}
	SetUpdatedDateFor10DaysRS(GetMarketDate());
	SetUpdateOptionDB(true); // 更新选项数据库.此时计算工作线程只是刚刚启动，需要时间去完成。
}

void CChinaMarket::CreateThreadBuildWeekLine(long lStartDate) {
	thread thread1(ThreadBuildWeekLine, this, lStartDate);
	thread1.detach();
}

void CChinaMarket::CreateThreadBuildWeekLineOfStock(CChinaStock* pStock, long lStartDate) {
	thread thread1(ThreadBuildWeekLineOfStock, pStock, lStartDate);
	thread1.detach();
}

void CChinaMarket::CreateThreadBuildWeekLineRS() {
	thread thread1(ThreadBuildWeekLineRS, this, _CHINA_MARKET_BEGIN_DATE_);
	thread1.detach();
}

void CChinaMarket::CreateThreadBuildWeekLineOfCurrentWeek() {
	thread thread1(ThreadBuildWeekLineOfCurrentWeek, this);
	thread1.detach();
}

void CChinaMarket::CreateThreadBuildCurrentWeekWeekLineTable() {
	thread thread1(ThreadBuildCurrentWeekWeekLineTable, this);
	thread1.detach();
}

bool CChinaMarket::DeleteDayLine(long lDate) {
	DeleteDayLineBasicInfo(lDate);
	DeleteDayLineExtendInfo(lDate);

	return true;
}

bool CChinaMarket::DeleteDayLineBasicInfo(long lDate) {
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

	return true;
}

bool CChinaMarket::DeleteDayLineExtendInfo(long lDate) {
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

	return true;
}

bool CChinaMarket::TaskLoadTempRTData(long lTheDay, long lCurrentTime) {
	ASSERT(!m_fTodayTempDataLoaded);

	if (IsSystemReady()) {
		LoadTempRTData(lTheDay);
		return true;
	}
	else {
		AddTask(CHINA_MARKET_LOAD_TEMP_RT_DATA__, GetNextSecond(lCurrentTime));
	}
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
void CChinaMarket::LoadTempRTData(long lTheDay) {
	CSetDayLineTodaySaved setDayLineTemp;
	// 读取今日生成的数据于DayLineToday表中。
	setDayLineTemp.m_strSort = _T("[ID]");
	setDayLineTemp.Open();
	if (!setDayLineTemp.IsEOF()) {
		while (!setDayLineTemp.IsEOF()) {
			if (setDayLineTemp.m_Date == lTheDay && IsStock(setDayLineTemp.m_Symbol)) {// 如果是当天的行情，则载入，否则放弃
				const CChinaStockPtr pStock = GetStock(setDayLineTemp.m_Symbol);
				ASSERT(!pStock->HaveFirstRTData()); // 确保没有开始计算实时数据
				pStock->LoadTodaySavedInfo(&setDayLineTemp);
			}
			setDayLineTemp.MoveNext();
		}
	}
	setDayLineTemp.Close();

	m_fTodayTempDataLoaded = true;
}

bool CChinaMarket::Load10DaysRSStrong1StockSet() {
	CSetRSStrong1Stock setRSStrong1;

	m_v10RSStrong1Stock.clear();
	setRSStrong1.Open();
	while (!setRSStrong1.IsEOF()) {
		if (IsStock(setRSStrong1.m_Symbol)) {
			CChinaStockPtr pStock = gl_pChinaMarket->GetStock(setRSStrong1.m_Symbol);
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
		if (IsStock(setRSStrong2.m_Symbol)) {
			CChinaStockPtr pStock = gl_pChinaMarket->GetStock(setRSStrong2.m_Symbol);
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

void CChinaMarket::SaveCalculatingRSOption() {
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
		if (IsStock(setRSStrongStock.m_Symbol)) {
			CChinaStockPtr pStock = gl_pChinaMarket->GetStock(setRSStrongStock.m_Symbol);
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
bool CChinaMarket::UpdateOptionDB() {
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
	return true;
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
			if (GetRSEndDate() == _CHINA_MARKET_BEGIN_DATE_) {
				// 当日线历史数据库中存在旧数据时，采用单线程模式存储新数据。使用多线程模式时，MySQL会出现互斥区Exception，估计是数据库重入时发生同步问题）。
				// 故而修补数据时同时只运行一个存储线程，其他都处于休眠状态。此种问题不会出现于生成所有日线数据时，故而新建日线数据时可以使用多线程（目前为4个）。
				// 使用8.0.27测试，发现可以采用4个线程了（20211103）
				// 目前使用8.0.30，偶尔又出现Exception了。(20220829)
				for (int i = 0; i < 3; i++) {
					gl_SaveDayLineThreadPermitted.release();
				}
			}
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

bool CChinaMarket::UpdateChosenStockDB() {
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

	return true;
}

bool CChinaMarket::AppendChosenStockDB() {
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

	return true;
}

void CChinaMarket::LoadChosenStockDB() {
	CSetChinaChosenStock setChinaChosenStock;

	setChinaChosenStock.Open();
	// 装入股票代码数据库
	while (!setChinaChosenStock.IsEOF()) {
		CChinaStockPtr pStock = nullptr;
		if (IsStock(setChinaChosenStock.m_Symbol)) {
			pStock = GetStock(setChinaChosenStock.m_Symbol);
			if (ranges::count(m_avChosenStock.at(0).begin(), m_avChosenStock.at(0).end(), pStock) == 0) {
				m_avChosenStock.at(0).push_back(pStock);
			}
			pStock->SetChosen(true);
			pStock->SetSaveToChosenStockDB(true);
		}
		setChinaChosenStock.MoveNext();
	}
	setChinaChosenStock.Close();
}

void CChinaMarket::CreateThreadUpdateTempRTData() {
	thread thread1(ThreadSaveTempRTData, this);
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}
