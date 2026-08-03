module;
#include <absl/log/absl_check.h>

module FireBirdLib.Market;

import FireBirdLib.Accessory.TimeConvert;

import FireBirdLib.GlobeDef;
import FireBirdLib.ContainerStockExchange;
import FireBirdLib.SystemConfiguration;
import FireBirdLib.DataSource.Virtual;
import FireBirdLib.Type.StockExchange;

import std;
using std::chrono::time_point_cast;
using std::chrono::milliseconds;
using std::chrono::local_seconds;
using std::chrono::local_days;
using std::chrono::weekday;
using std::chrono::Sunday;
using std::chrono::Monday;
using std::chrono::Saturday;
using std::chrono::Friday;
using std::chrono::hours;
using std::chrono::minutes;
using std::chrono::years;
using std::chrono::locate_zone;
using std::chrono::steady_clock;
using std::chrono::year;
using std::make_shared;
using std::literals::chrono_literals::operator""h;
using std::literals::chrono_literals::operator""min;
using std::literals::chrono_literals::operator""s;

CVirtualMarket::CVirtualMarket() {
	CreateLocalTimeZone(m_strLocalMarketTimeZone);
	m_fResetMarket = true;

	m_strMarketId = "Warning: CVirtualMarket Called.";
	m_exchange = gl_dataContainerStockExchange.GetItem("L"); // 默认使用伦敦交易所
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 唯一的调度函数。所有的市场皆使用此函数，具体的差异由数据源（FireBirdLib.DataSource）和不同的任务序列来区分。
// 由MainFrame中的定时任务调度，每100毫秒执行一次。
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualMarket::ScheduleTask() {
	CalculateTime();

	if (GetMarketTime() < toLocalTime(100)) { // 每日最初的一分钟内。保证每日执行一次
		if (!m_marketTask.Empty()) {
			if (m_marketTask.GetTask()->GetTime() >= toLocalTime(240000)) { // 当任务队列中都是明日的时间时，将时间调减24小时
				AdjustTaskTime();
			}
		}
	}

	// 调用本市场的各data source，进行网络数据的接收和处理。在市场重置时间内暂停
	if (IsReadyToInquireWebData()) {
		RunDataSource();
	}

	// 执行本市场各项定时任务。
	if (IsResetting()) return; // 当市场正在重置时暂停

	auto start = time_point_cast<milliseconds>(steady_clock::now());
	int taskType = ProcessTask(); // 执行定时任务
	auto end = time_point_cast<milliseconds>(steady_clock::now());
#ifdef _TRACE_SCHEDULE_TASK___
	if (taskType != 0) gl_traceLogger->trace("{} ms {}", gl_mapMarketMapIndex.at(taskType), (end - start).count());
#endif

	auto immediateTaskSize = m_marketImmediateTask.Size();
	vector<int> vTaskType;
	for (size_t i = 0; i < immediateTaskSize; i++) {
		auto pTask = m_marketImmediateTask.GetTask();
		vTaskType.push_back(ProcessCurrentImmediateTask());// 执行所有即时任务
	}
	ABSL_CHECK(vTaskType.size() == immediateTaskSize);
#ifdef _TRACE_SCHEDULE_TASK___
	if (immediateTaskSize > 0) {
		for (size_t i = 0; i < immediateTaskSize; i++) {
			gl_traceLogger->trace("{}", gl_mapMarketMapIndex.at(vTaskType.at(i)));
		}
		gl_traceLogger->trace("{}ms", (end - start).count());
	}
#endif
}

void CVirtualMarket::RunDataSource() const {
	for (const auto& pDataSource : m_vDataSource) {
		if (pDataSource->IsEnable()) {
			if (!pDataSource->IsInquiring()) {
				pDataSource->Run(GetMarketTime());
			}
		}
	}
}

void CVirtualMarket::ResetMarket() {
	ABSL_CHECK(0); // 不允许调用基类重置市场函数。这里只是为了测试方便的原因才定义一个实现。
}

bool CVirtualMarket::IsResetTime() {
	return GetMarketTime() > GetPrevTime(GetResetTime(), 0h, 10min, 0s) && GetMarketTime() < GetNextTime(GetResetTime(), 0h, 5min, 0s);
}

local_seconds CVirtualMarket::GetResetTime() {
	if (gl_systemConfiguration.IsWorkingMode()) // 不允许在运行状态时调用此函数
		ABSL_CHECK(0);
	return local_seconds{};
}

bool CVirtualMarket::UpdateMarketInfo() {
	// do nothing
	return true;
}

void CVirtualMarket::AddTask(const CMarketTaskPtr& pTask) {
	m_marketTask.AddTask(pTask);
	m_qMarketDisplayTask.enqueue(pTask);
}

void CVirtualMarket::AddTask(const long lTaskType, const long lExecuteTime) {
	const auto pTask = make_shared<CMarketTask>();
	pTask->SetType(lTaskType);
	pTask->SetTime(lExecuteTime);
	AddTask(pTask);
}

void CVirtualMarket::AddTask(const long lTaskType, const local_seconds executeTime) {
	const auto pTask = make_shared<CMarketTask>();
	pTask->SetType(lTaskType);
	pTask->SetTime(executeTime);
	AddTask(pTask);
}

///////////////////////////////////////////////////////////////
//
// 如果任务队列中的时间皆超过240000，则将所有的时间减去240000.
//
///////////////////////////////////////////////////////////////
void CVirtualMarket::AdjustTaskTime() {
	vector<CMarketTaskPtr> vTask;
	while (!m_marketTask.Empty()) {
		vTask.push_back(m_marketTask.GetTask());
		m_marketTask.DiscardCurrentTask();
	}
	ABSL_CHECK(m_marketTask.Empty());
	for (const auto& pMarketTask : vTask) {
		ABSL_CHECK(pMarketTask->GetTime() >= toLocalTime(240000));
		pMarketTask->SetTime(pMarketTask->GetTime() - 24h);
		m_marketTask.AddTask(pMarketTask);
	}
}

void CVirtualMarket::AddImmediateTask(const CMarketTaskPtr& pTask) {
	m_marketImmediateTask.AddTask(pTask);
}

void CVirtualMarket::AddImmediateTask(const long lTaskType) {
	const auto pTask = make_shared<CMarketTask>();
	pTask->SetType(lTaskType);
	pTask->SetTime(0);
	AddImmediateTask(pTask);
}

bool CVirtualMarket::HaveNewTask() const {
	return m_qMarketDisplayTask.size_approx() > m_lLastQueueLength;
}

vector<CMarketTaskPtr> CVirtualMarket::DiscardOutDatedTask(local_seconds lCurrentMarketTime) {
	vector<CMarketTaskPtr> validTasks;
	CMarketTaskPtr pTask = nullptr;

	while (m_qMarketDisplayTask.try_dequeue(pTask)) {
		if (pTask->GetTime() > lCurrentMarketTime) {
			validTasks.emplace_back(pTask);
		}
	}
	m_lLastQueueLength = validTasks.size();
	for (const auto& pTaskRemained : validTasks) {
		m_qMarketDisplayTask.enqueue(pTaskRemained);
	}

	std::ranges::sort(validTasks, [](const CMarketTaskPtr& p1, const CMarketTaskPtr& p2) { return p1->GetTime() < p2->GetTime(); });
	return validTasks;
}

vector<CMarketTaskPtr> CVirtualMarket::GetDisplayMarketTask() {
	return vector<CMarketTaskPtr>();
}

void CVirtualMarket::CalculateTime() noexcept {
	m_marketClock = m_marketTimeZone->to_local(gl_tpNow);
}

bool CVirtualMarket::IsWorkingDay() const noexcept {
	if (GetWeekDay() == Sunday || GetWeekDay() == Saturday) { // Sunday or Saturday
		return false;
	}
	return true;
}

bool CVirtualMarket::IsWorkingDay(const local_days& date) noexcept {
	weekday weekday{ date };
	if (weekday == Sunday || weekday == Saturday) { // Sunday or Saturday
		return false;
	}
	return true;
}

local_days CVirtualMarket::GetNextTradeDate() {
	days day{ 1 };
	if (GetWeekDay() == Saturday) {
		++day; // 下周一
	}
	else if (GetWeekDay() == Friday) {
		++day; // 下周一
		++day;
	}
	return GetMarketDate() + day;
}

local_days CVirtualMarket::GetCurrentTradeDate() {
	days day(0);
	if (GetWeekDay() == Saturday) {
		day = days(1); // 周五
	}
	else if (GetWeekDay() == Sunday) {
		day = days(2); // 周五
	}
	return GetMarketDate() - day;
}

local_days CVirtualMarket::GetLastTradeDate() {
	days day;
	if (GetWeekDay() == Monday) {
		day = days(3); // 周五
	}
	else if (GetWeekDay() == Sunday) {
		day = days(3); // 周四
	}
	else if (GetWeekDay() == Saturday) {
		day = days(2); // 周四
	}
	else {
		day = days(1); // 上一日
	}
	return GetMarketDate() - day;
}

string CVirtualMarket::GetStringOfMarketDate() const {
	return std::format("{:%F}", GetMarketDate());
}

string CVirtualMarket::GetStringOfLocalTime() const {
	auto localTime = gl_pTimeZoneLocal->to_local(gl_tpNow);
	return std::format("{:%T}", localTime);
}

string CVirtualMarket::GetStringOfLocalDateTime() const {
	auto localTime = gl_pTimeZoneLocal->to_local(gl_tpNow);
	return std::format("{:%F %T}", localTime);
}

string CVirtualMarket::GetStringOfMarketTime() const {
	return std::format("{:%T}", m_marketClock);
}

string CVirtualMarket::GetStringOfMarketDateTime() const {
	return std::format("{:%F %T}", m_marketClock);
}

sys_seconds CVirtualMarket::ConvertToUTCTime(int marketDate, long lMarketTime) const {
	int year1 = marketDate / 10000;
	int month = marketDate / 100 - year1 * 100;
	int day = marketDate - year1 * 10000 - month * 100;
	int hour = lMarketTime / 10000;
	int minute = lMarketTime / 100 - hour * 100;
	int second = lMarketTime - hour * 10000 - minute * 100;
	local_seconds local_time{ local_days{ year{ year1 } / month / day } + hours{ hour } + minutes{ minute } + seconds{ second } };
	return m_marketTimeZone->to_sys(local_time);
}

long CVirtualMarket::ConvertToDate(const sys_seconds tp) const noexcept {
	auto local_time = m_marketTimeZone->to_local(tp);
	year_month_day ymd{ floor<days>(local_time) };
	return static_cast<int>(ymd.year()) * 10000
	+ static_cast<unsigned>(ymd.month()) * 100
	+ static_cast<unsigned>(ymd.day());
}

void CVirtualMarket::TEST_SetMarketTime(local_seconds ls) noexcept {
	local_days day = floor<days>(m_marketClock);
	m_marketClock = toLocalDateTime(day, ls);
}

void CVirtualMarket::TEST_SetMarketDate(local_days ld) noexcept {
	local_seconds second{ m_marketClock - floor<days>(m_marketClock) };
	m_marketClock = toLocalDateTime(ld, second);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 这个函数导致内存泄漏，估计是调用的时区数据库函数locate_zone()初始化后，程序退出时没有卸载。
///Note:微软实现的chrono库中，time_zone静态变量保留至程序退出之后，导致调试系统误报内存泄露。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualMarket::CreateLocalTimeZone(const string& strLocalNameOfMarket) {
	m_marketTimeZone = locate_zone(strLocalNameOfMarket);
}

local_seconds CVirtualMarket::GetMarketOpenTime() const {
	return m_exchange->m_marketOpenTime;
}

local_seconds CVirtualMarket::GetMarketCloseTime() const {
	return m_exchange->m_marketCloseTime;
}
