#include"pch.h"

#include"TimeConvert.h"

#include "VirtualMarket.h"

#include <spdlog/stopwatch.h>

#include"VirtualDataSource.h"

CVirtualMarket::CVirtualMarket() {
	m_fResetMarket = true;

	m_lMarketLastTradeDate = 0;
	m_lMarketTime = 0;
	m_lMarketDate = 0;
	m_tmMarket.tm_year = 1970;
	m_tmMarket.tm_yday = 1;

	m_strMarketId = _T("Warning: CVirtualMarket Called.");
	m_lMarketTimeZone = -8 * 3600; // 本系统默认标准时间为东八区（北京标准时间）。
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

	// 执行本市场各项定时任务。当市场正在重置时暂停
	if (!IsResetting()) {
		spdlog::stopwatch sw;
		int taskType = ProcessTask(lCurrentMarketTime); // 执行定时任务
#ifdef _TRACE_SCHEDULE_TASK___
		if (taskType != 0) gl_traceLogger->trace("{} ms {}", gl_mapMarketMapIndex.at(taskType), sw.elapsed_ms());
#endif

		sw.reset();
		auto immediateTaskSize = m_marketImmediateTask.Size();
		vector<int> vTaskType;
		for (int i = 0; i < immediateTaskSize; i++) {
			auto pTask = m_marketImmediateTask.GetTask();
			auto p = ProcessCurrentImmediateTask(lCurrentMarketTime); // 执行所有即时任务
			vTaskType.push_back(p);
		}
		ASSERT(vTaskType.size() == immediateTaskSize);
#ifdef _TRACE_SCHEDULE_TASK___
		if (immediateTaskSize > 0) {
			for (int i = 0; i < immediateTaskSize; i++) {
				gl_traceLogger->trace("{}", gl_mapMarketMapIndex.at(vTaskType.at(i)));
			}
			gl_traceLogger->trace("{}ms", sw.elapsed_ms());
		}
#endif
	}
}

void CVirtualMarket::CalculateTime() noexcept {
	time(&gl_tUTCTime);

	GetMarketTimeStruct(&m_tmMarket, gl_tUTCTime, m_lMarketTimeZone);
	m_lMarketDate = ConvertToDate(&m_tmMarket);
	m_lMarketTime = ConvertToTime(&m_tmMarket);
}

void CVirtualMarket::RunDataSource(long lMarketTime) const {
	for (const auto& pDataSource : m_vDataSource) {
		if (pDataSource->IsEnable()) pDataSource->Run(lMarketTime);
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

shared_ptr<vector<CMarketTaskPtr>> CVirtualMarket::DiscardOutDatedTask(long m_lCurrentMarketTime) {
	shared_ptr<vector<CMarketTaskPtr>> pvTask = make_shared<vector<CMarketTaskPtr>>();
	CMarketTaskPtr pTask = nullptr;

	while (m_qMarketDisplayTask.try_dequeue(pTask)) {
		if (pTask->GetTime() > m_lCurrentMarketTime) {
			pvTask->push_back(pTask);
		}
	}
	m_lLastQueueLength = pvTask->size();
	for (auto pTaskRemained : *pvTask) {
		m_qMarketDisplayTask.enqueue(pTaskRemained);
	}

	std::ranges::sort(*pvTask, [](const CMarketTaskPtr& p1, const CMarketTaskPtr& p2) { return p1->GetTime() < p2->GetTime(); });
	return pvTask;
}

vector<CMarketTaskPtr> CVirtualMarket::GetDisplayMarketTask() {
	return vector<CMarketTaskPtr>();
}

tm CVirtualMarket::GetMarketTime(time_t tUTC) const {
	tm tm_{};

	GetMarketTimeStruct(&tm_, tUTC, m_lMarketTimeZone);

	return tm_;
}

time_t CVirtualMarket::TransferToUTCTime(tm* tmMarketTime) const {
	return _mkgmtime(tmMarketTime) + m_lMarketTimeZone;
}

time_t CVirtualMarket::TransferToUTCTime(long lMarketDate, long lMarketTime) const {
	return ConvertToTTime(lMarketDate, m_lMarketTimeZone, lMarketTime);
}

long CVirtualMarket::GetMarketDate(time_t tUTC) const {
	tm tm_{};

	GetMarketTimeStruct(&tm_, tUTC, m_lMarketTimeZone);

	return (ConvertToDate(&tm_));
}

long CVirtualMarket::CalculateNextTradeDate() noexcept {
	time_t tMarket;

	switch (m_tmMarket.tm_wday) {
	case 6: // 星期六
		tMarket = gl_tUTCTime + 2 * 24 * 3600; // 下周一
		break;
	case 5: // 周五
		tMarket = gl_tUTCTime + 3 * 24 * 3600; // 下周一
		break;
	default: // 其他
		tMarket = gl_tUTCTime + 24 * 3600; // 次日
	}
	tMarket -= m_lOpenMarketTime; // 减去开市时间，具体值由各市场预先设定
	const tm tmMarketTime = GetMarketTime(tMarket);
	m_lMarketNextTradeDate = ConvertToDate(&tmMarketTime);
	return m_lMarketNextTradeDate;
}

long CVirtualMarket::CalculateCurrentTradeDate() noexcept {
	time_t tMarket;

	switch (m_tmMarket.tm_wday) {
	case 0: //星期日
		tMarket = gl_tUTCTime - 2 * 24 * 3600; // 周五
		break;
	case 6: // 星期六
		tMarket = gl_tUTCTime - 1 * 24 * 3600; // 周五
		break;
	default: // 其他
		tMarket = gl_tUTCTime; // 本日
	}
	tMarket -= m_lOpenMarketTime; // 减去开市时间，具体值由各市场预先设定
	const tm tmMarketTime = GetMarketTime(tMarket);
	m_lMarketCurrentTradeDate = ConvertToDate(&tmMarketTime);
	return m_lMarketCurrentTradeDate;
}

long CVirtualMarket::CalculateLastTradeDate() noexcept {
	time_t tMarket;

	switch (m_tmMarket.tm_wday) {
	case 1: // 星期一
		tMarket = gl_tUTCTime - 3 * 24 * 3600; // 上周五
		break;
	case 0: //星期日
		tMarket = gl_tUTCTime - 3 * 24 * 3600; // 周四
		break;
	case 6: // 星期六
		tMarket = gl_tUTCTime - 2 * 24 * 3600; // 周四
		break;
	default: // 其他
		tMarket = gl_tUTCTime - 24 * 3600; // 上一日
	}
	tMarket -= m_lOpenMarketTime; // 减去开市时间，具体值由各市场预先设定
	const tm tmMarketTime = GetMarketTime(tMarket);
	m_lMarketLastTradeDate = ConvertToDate(&tmMarketTime);
	return m_lMarketLastTradeDate;
}

bool CVirtualMarket::IsWorkingDay() const noexcept {
	if (m_tmMarket.tm_wday == 0 || m_tmMarket.tm_wday == 6) {
		return false;
	}
	return true;
}

bool CVirtualMarket::IsWorkingDay(CTime timeCurrent) noexcept {
	if ((timeCurrent.GetDayOfWeek() == 1) || (timeCurrent.GetDayOfWeek() == 7)) {
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

CString CVirtualMarket::GetStringOfLocalTime() const {
	char buffer[30];
	tm tmLocal;

	localtime_s(&tmLocal, &gl_tUTCTime);
	sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfLocalDateTime() const {
	char buffer[100];
	tm tmLocal;

	localtime_s(&tmLocal, &gl_tUTCTime);
	sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfMarketTime() const {
	char buffer[30];

	sprintf_s(buffer, _T("%02d:%02d:%02d "), m_tmMarket.tm_hour, m_tmMarket.tm_min, m_tmMarket.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfMarketDateTime() const {
	char buffer[100];

	sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), m_tmMarket.tm_year + 1900, m_tmMarket.tm_mon + 1, m_tmMarket.tm_mday, m_tmMarket.tm_hour, m_tmMarket.tm_min, m_tmMarket.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfMarketDate() const {
	return ConvertDateToChineseTimeStampString(m_lMarketDate);
}
