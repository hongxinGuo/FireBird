#include"pch.h"

#include"TimeConvert.h"
#include "VirtualMarket.h"
#include"VirtualDataSource.h"

CVirtualMarket::CVirtualMarket() {
	GetMarketLocalTimeOffset(m_strLocalMarketTimeZone);
	m_fResetMarket = true;

	m_strMarketId = "Warning: CVirtualMarket Called.";
	m_exchange = gl_dataContainerStockExchange.GetItem("L"); // 默认使用伦敦交易所
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 唯一的调度函数。所有的市场皆使用此函数，具体的差异由数据源（DataSource）和不同的任务序列来区分。
// 由MainFrame中的定时任务调度，每100毫秒执行一次。
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualMarket::ScheduleTask() {
	CalculateTime();

	const long lCurrentMarketTime = GetMarketTime();

	if (lCurrentMarketTime < 100) { // 每日最初的一分钟内。保证每日执行一次
		if (!m_marketTask.Empty()) {
			if (m_marketTask.GetTask()->GetTime() >= 240000) { // 当任务队列中都是明日的时间时，将时间调减24小时
				AdjustTaskTime();
			}
		}
	}

	// 调用本市场的各data source，进行网络数据的接收和处理。在市场重置时间内暂停
	if (IsReadyToInquireWebData(lCurrentMarketTime)) {
		RunDataSource(lCurrentMarketTime);
	}

	// 执行本市场各项定时任务。
	if (IsResetting()) return; // 当市场正在重置时暂停

	auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
	int taskType = ProcessTask(lCurrentMarketTime); // 执行定时任务
	auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
#ifdef _TRACE_SCHEDULE_TASK___
	if (taskType != 0) gl_traceLogger->trace("{} ms {}", gl_mapMarketMapIndex.at(taskType), (end - start).count());
#endif

	auto immediateTaskSize = m_marketImmediateTask.Size();
	vector<int> vTaskType;
	for (size_t i = 0; i < immediateTaskSize; i++) {
		auto pTask = m_marketImmediateTask.GetTask();
		vTaskType.push_back(ProcessCurrentImmediateTask(lCurrentMarketTime));// 执行所有即时任务
	}
	ASSERT(vTaskType.size() == immediateTaskSize);
#ifdef _TRACE_SCHEDULE_TASK___
	if (immediateTaskSize > 0) {
		for (size_t i = 0; i < immediateTaskSize; i++) {
			gl_traceLogger->trace("{}", gl_mapMarketMapIndex.at(vTaskType.at(i)));
		}
		gl_traceLogger->trace("{}ms", (end - start).count());
	}
#endif
}

void CVirtualMarket::RunDataSource(long lMarketTime) const {
	for (const auto& pDataSource : m_vDataSource) {
		if (pDataSource->IsEnable()) {
			if (!pDataSource->IsInquiring()) {
				pDataSource->Run(lMarketTime);
			}
		}
	}
}

void CVirtualMarket::ResetMarket() {
	ASSERT(0); // 不允许调用基类重置市场函数。这里只是为了测试方便的原因才定义一个实现。
}

bool CVirtualMarket::IsResetTime(long lCurrentTime) {
	return lCurrentTime > GetPrevTime(GetResetTime(), 0, 10, 0) && lCurrentTime < GetNextTime(GetResetTime(), 0, 5, 0);
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
	ASSERT(m_marketTask.Empty());
	for (const auto& pMarketTask : vTask) {
		ASSERT(pMarketTask->GetTime() >= 240000);
		pMarketTask->SetTime(pMarketTask->GetTime() - 240000);
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

vector<CMarketTaskPtr> CVirtualMarket::DiscardOutDatedTask(long m_lCurrentMarketTime) {
	vector<CMarketTaskPtr> validTasks;
	CMarketTaskPtr pTask = nullptr;

	while (m_qMarketDisplayTask.try_dequeue(pTask)) {
		if (pTask->GetTime() > m_lCurrentMarketTime) {
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
	m_marketTime = m_marketTimeZone->to_local(gl_tpNow);
	m_marketTimeOfDay = chrono::hh_mm_ss<chrono::seconds>(m_marketTime - chrono::floor<chrono::days>(m_marketTime));
	m_marketYearMonthDay = chrono::year_month_day(chrono::floor<chrono::days>(m_marketTime));
	m_marketWeekDay = chrono::weekday(m_marketYearMonthDay);
	m_lMarketDate = static_cast<int>(m_marketYearMonthDay.year()) * 10000
	+ static_cast<unsigned>(m_marketYearMonthDay.month()) * 100
	+ static_cast<unsigned>(m_marketYearMonthDay.day());
	m_lMarketTime = m_marketTimeOfDay.hours().count() * 10000 + m_marketTimeOfDay.minutes().count() * 100 + m_marketTimeOfDay.seconds().count();
}

long CVirtualMarket::GetMarketDate(time_t tUTC) const {
	tm tm_{};

	GetMarketTimeStruct(&tm_, tUTC);

	return ConvertToDate(&tm_);
}

bool CVirtualMarket::IsWorkingDay() const noexcept {
	if (m_marketWeekDay == chrono::weekday(0) || m_marketWeekDay == chrono::weekday(6)) { // Sunday or Saturday
		return false;
	}
	return true;
}

bool CVirtualMarket::IsWorkingDay(long lDate) noexcept {
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;
	const CTime ct(year, month, day, 12, 0, 0);

	if ((ct.GetDayOfWeek() == 1) || (ct.GetDayOfWeek() == 7)) {
		return false;
	}
	return true;
}

long CVirtualMarket::GetNextTradeDate() {
	time_t tMarket;
	tm tmMarketTime2;
	GetMarketTimeStruct(&tmMarketTime2, GetUTCTime() - m_exchange->m_lMarketOpenTime);

	switch (tmMarketTime2.tm_wday) {
	case 6: // 星期六
		tMarket = GetUTCTime() + 2 * 24 * 3600; // 下周一
		break;
	case 5: // 周五
		tMarket = GetUTCTime() + 3 * 24 * 3600; // 下周一
		break;
	default: // 其他
		tMarket = GetUTCTime() + 24 * 3600; // 次日
	}
	tMarket -= m_exchange->m_lMarketOpenTime; // 减去开市时间，具体值由各市场预先设定
	tm tmMarketTime;
	GetMarketTimeStruct(&tmMarketTime, tMarket);
	return ConvertToDate(&tmMarketTime);
}

long CVirtualMarket::GetNextTradeDate2() {
	chrono::days day{ 1 };
	if (m_marketWeekDay == chrono::Saturday) {
		++day; // 下周一
	}
	else if (m_marketWeekDay == chrono::Friday) {
		++day; // 下周一
		++day;
	}
	auto next_trade_date = chrono::floor<chrono::days>(m_marketTime) + day;
	chrono::year_month_day ymd{ next_trade_date };
	return static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day());
}

long CVirtualMarket::GetCurrentTradeDate() {
	time_t tMarket;

	tm tmMarketTime2;
	GetMarketTimeStruct(&tmMarketTime2, GetUTCTime() - m_exchange->m_lMarketOpenTime);

	switch (tmMarketTime2.tm_wday) {
	case 0: //星期日
		tMarket = GetUTCTime() - 2 * 24 * 3600; // 周五
		break;
	case 6: // 星期六
		tMarket = GetUTCTime() - 1 * 24 * 3600; // 周五
		break;
	default: // 其他
		tMarket = GetUTCTime(); // 本日
	}
	tMarket -= m_exchange->m_lMarketOpenTime; // 减去开市时间，具体值由各市场预先设定
	tm tmMarketTime;
	GetMarketTimeStruct(&tmMarketTime, tMarket);
	return ConvertToDate(&tmMarketTime);
}

long CVirtualMarket::GetCurrentTradeDate2() {
	chrono::days day;
	if (m_marketWeekDay == chrono::Saturday) {
		day = chrono::days(1); // 周五
	}
	else if (m_marketWeekDay == chrono::Sunday) {
		day = chrono::days(2); // 周五
	}
	else {
		day = chrono::days(0); // 本日
	}
	auto current_trade_date = chrono::floor<chrono::days>(m_marketTime) - day;
	chrono::year_month_day ymd{ current_trade_date };
	return static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day());
}

long CVirtualMarket::GetLastTradeDate() {
	time_t tMarket;
	tm tmMarketTime2;
	GetMarketTimeStruct(&tmMarketTime2, GetUTCTime() - m_exchange->m_lMarketOpenTime);

	switch (tmMarketTime2.tm_wday) {
	case 1: // 星期一
		tMarket = GetUTCTime() - 3 * 24 * 3600; // 上周五
		break;
	case 0: //星期日
		tMarket = GetUTCTime() - 3 * 24 * 3600; // 周四
		break;
	case 6: // 星期六
		tMarket = GetUTCTime() - 2 * 24 * 3600; // 周四
		break;
	default: // 其他
		tMarket = GetUTCTime() - 24 * 3600; // 上一日
	}
	tMarket -= m_exchange->m_lMarketOpenTime; // 减去开市时间，具体值由各市场预先设定
	tm tmMarketTime;
	GetMarketTimeStruct(&tmMarketTime, tMarket);
	return ConvertToDate(&tmMarketTime);
}

long CVirtualMarket::GetLastTradeDate2() {
	chrono::days day;
	if (m_marketWeekDay == chrono::Monday) {
		day = chrono::days(3); // 周五
	}
	else if (m_marketWeekDay == chrono::Sunday) {
		day = chrono::days(3); // 周四
	}
	else if (m_marketWeekDay == chrono::Saturday) {
		day = chrono::days(2); // 周四
	}
	else {
		day = chrono::days(1); // 上一日
	}
	auto current_trade_date = chrono::floor<chrono::days>(m_marketTime) - day;
	chrono::year_month_day ymd{ current_trade_date };
	return static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day());
}

string CVirtualMarket::GetStringOfMarketDate() const {
	return ConvertDateToChineseTimeStampString(m_lMarketDate);
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
	return std::format("{:%T}", m_marketTime);
}

string CVirtualMarket::GetStringOfMarketDateTime() const {
	return std::format("{:%F %T}", m_marketTime);
}

chrono::sys_seconds CVirtualMarket::ConvertToUTCTime(const chrono::hh_mm_ss<chrono::seconds>& hhMmSs) {
	chrono::local_days ld{ m_marketYearMonthDay };
	auto local_tp = ld + hhMmSs.to_duration();
	chrono::sys_time utc_time = m_marketTimeZone->to_sys(local_tp);
	return utc_time;
}

chrono::sys_seconds CVirtualMarket::ConvertToUTCTime(long lMarketDate, long lMarketTime) const {
	int year = lMarketDate / 10000;
	int month = lMarketDate / 100 - year * 100;
	int day = lMarketDate - year * 10000 - month * 100;
	int hour = lMarketTime / 10000;
	int minute = lMarketTime / 100 - hour * 100;
	int second = lMarketTime - hour * 10000 - minute * 100;
	chrono::local_seconds local_time{ chrono::local_days{ chrono::year{ year } / month / day } + chrono::hours{ hour } + chrono::minutes{ minute } + chrono::seconds{ second } };
	return m_marketTimeZone->to_sys(local_time);
}

long CVirtualMarket::ConvertToDate(const time_t tUTC) const noexcept {
	tm tm_;
	GetMarketTimeStruct(&tm_, tUTC);
	return ((tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday);
}

long CVirtualMarket::ConvertToDate(const chrono::sys_seconds tp) const noexcept {
	auto local_time = m_marketTimeZone->to_local(tp);
	chrono::year_month_day ymd{ chrono::floor<chrono::days>(local_time) };
	return static_cast<int>(ymd.year()) * 10000
	+ static_cast<unsigned>(ymd.month()) * 100
	+ static_cast<unsigned>(ymd.day());
}

void CVirtualMarket::GetMarketTimeStruct(tm* tm_, time_t tUTC) const {
	time_t tMarket = tUTC + GetTimeZone();
	gmtime_s(tm_, &tMarket);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 这个函数导致内存泄漏，估计是调用的时区数据库函数locate_zone()初始化后，程序退出时没有卸载。
///Note:微软实现的chrono库中，time_zone静态变量保留至程序退出之后，导致调试系统误报内存泄露。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualMarket::GetMarketLocalTimeOffset(const string& strLocalNameOfMarket) {
	m_marketTimeZone = chrono::locate_zone(strLocalNameOfMarket);
	m_TimeZoneOffset = m_marketTimeZone->get_info(chrono::sys_seconds()).offset;
}
