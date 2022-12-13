#include"pch.h"

#include"TimeConvert.h"

#include "VirtualMarket.h"

// 所有的市场使用同一个当地时间。
time_t CVirtualMarket::sm_tUTC = 0;

IMPLEMENT_DYNCREATE(CVirtualMarket, CObject)

CVirtualMarket::CVirtualMarket(void) : CObject() {
	m_fResetMarketPermission = true; // 允许系统被重置标识，唯独此标识不允许系统重置。初始时设置为真：允许重置系统。
	m_fResetMarket = true;
	m_fReadyToRun = true;
	m_fSystemReady = true; // 默认为真

	m_lMarketLastTradeDate = 0;
	m_lMarketTime = 0;
	m_lMarketDate = 0;
	m_tmMarket.tm_year = 1970;
	m_tmMarket.tm_yday = 1;

	m_strMarketId = _T("Warning: CVirtualMarket Called.");
	m_lMarketTimeZone = -8 * 3600; // 本系统默认标准时间为东八区（北京标准时间）。

	m_i10SecondCounter = 9; // 十秒一次的计数器
	m_i1MinuteCounter = 59; // 一分钟一次的计数器
	m_i5MinuteCounter = 299; // 五分钟一次的计数器
	m_i1HourCounter = 3599; // 一小时一次的计数器
}

#ifdef _DEBUG
void CVirtualMarket::AssertValid() const {
	CObject::AssertValid();
}

void CVirtualMarket::Dump(CDumpContext& dc) const {
	CObject::Dump(dc);
}
#endif //_DEBUG

bool CVirtualMarket::SchedulingTask(void) {
	CalculateTime();

	static time_t stLastTime = 0;
	const time_t tDiffer = sm_tUTC - stLastTime;
	//根据时间，调度各项定时任务.每秒调度一次
	if (tDiffer > 0) {
		SchedulingTaskPerSecond(tDiffer);
		stLastTime = sm_tUTC;
		return true;
	}
	else return false;
}

/// <summary>
/// 申请并处理Data source的数据，由最终衍生类的SchedlingTask调度。
/// 此函数在VirtualMarket中定义，但由最终衍生类来调用，应为lCurrentTime必须为该衍生类的当前市场时间。
/// </summary>
/// <param name="lCurrentTime"></param>
void CVirtualMarket::InquireAndProcessDataSource(long lCurrentTime) {
	for (const auto& pDataSource : m_vDataSource) {
		if (pDataSource->IsEnable()) pDataSource->Run(lCurrentTime);
	}
}

void CVirtualMarket::ResetMarket(void) {
	ASSERT(0); // 不允许调用基类重置市场函数。这里只是为了测试方便的原因才定义一个实现。
}

bool CVirtualMarket::UpdateMarketInfo(void) {
	// do nothing
	return true;
}

tm CVirtualMarket::TransferToMarketTime(time_t tUTC) {
	tm tm_{};

	GetMarketTimeStruct(&tm_, tUTC, m_lMarketTimeZone);

	return tm_;
}

time_t CVirtualMarket::TransferToUTCTime(tm* tmMarketTime) {
	return _mkgmtime(tmMarketTime) + m_lMarketTimeZone;
}

time_t CVirtualMarket::TransferToUTCTime(long lMarketDate, long lMarketTime) {
	return ::TransferToTTime(lMarketDate, m_lMarketTimeZone, lMarketTime);
}

long CVirtualMarket::TransferToMarketDate(time_t tUTC) {
	tm tm_{};

	GetMarketTimeStruct(&tm_, tUTC, m_lMarketTimeZone);

	return (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
}

void CVirtualMarket::CalculateTime(void) noexcept {
	time(&sm_tUTC);

	m_tmMarket = TransferToMarketTime();
	m_lMarketDate = (m_tmMarket.tm_year + 1900) * 10000 + (m_tmMarket.tm_mon + 1) * 100 + m_tmMarket.tm_mday;
	m_lMarketTime = m_tmMarket.tm_hour * 10000 + m_tmMarket.tm_min * 100 + m_tmMarket.tm_sec;
}

void CVirtualMarket::CalculateLastTradeDate(void) noexcept {
	time_t tMarket = 0;

	switch (m_tmMarket.tm_wday) {
	case 1: // 星期一
		tMarket = sm_tUTC - 3 * 24 * 3600; //
		break;
	case 0: //星期日
		tMarket = sm_tUTC - 3 * 24 * 3600; //
		break;
	case 6: // 星期六
		tMarket = sm_tUTC - 2 * 24 * 3600; //
		break;
	default: // 其他
		tMarket = sm_tUTC - 24 * 3600; //
	}
	const tm tmMarketTime = TransferToMarketTime(tMarket);
	m_lMarketLastTradeDate = (tmMarketTime.tm_year + 1900) * 10000 + (tmMarketTime.tm_mon + 1) * 100 + tmMarketTime.tm_mday;
}

bool CVirtualMarket::IsWorkingDay(void) const noexcept {
	if ((m_tmMarket.tm_wday == 0) || (m_tmMarket.tm_wday == 6)) {
		return false;
	}
	else return true;
}

bool CVirtualMarket::IsWorkingDay(CTime timeCurrent) const noexcept {
	if ((timeCurrent.GetDayOfWeek() == 1) || (timeCurrent.GetDayOfWeek() == 7)) {
		return false;
	}
	else return true;
}

bool CVirtualMarket::IsWorkingDay(long lDate) const noexcept {
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;
	const CTime ct(year, month, day, 12, 0, 0);

	if ((ct.GetDayOfWeek() == 1) || (ct.GetDayOfWeek() == 7)) {
		return false;
	}
	else return true;
}

bool CVirtualMarket::IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays) const noexcept {
	const CTimeSpan ts(lTimeSpawnOfDays, 0, 0, 0);
	const long year = lEarlyDate / 10000;
	const long month = lEarlyDate / 100 - year * 100;
	const long day = lEarlyDate - year * 10000 - month * 100;
	CTime ctEarly(year, month, day, 12, 0, 0);
	ctEarly += ts;
	const long lNewDate = ctEarly.GetYear() * 10000 + ctEarly.GetMonth() * 100 + ctEarly.GetDay();
	return (lNewDate < lLatelyDate);
}

long CVirtualMarket::GetNextDay(long lDate, long lTimeSpanDays) const noexcept {
	const CTimeSpan ts(lTimeSpanDays, 0, 0, 0);
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;
	CTime ctDay(year, month, day, 12, 0, 0);
	ctDay += ts;
	const long lNewDate = ctDay.GetYear() * 10000 + ctDay.GetMonth() * 100 + ctDay.GetDay();
	return (lNewDate);
}

long CVirtualMarket::GetPrevDay(long lDate, long lTimeSpanDays) const noexcept {
	const CTimeSpan ts(lTimeSpanDays, 0, 0, 0);
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;
	CTime ctDay(year, month, day, 12, 0, 0);
	ctDay -= ts;
	const long lNewDate = ctDay.GetYear() * 10000 + ctDay.GetMonth() * 100 + ctDay.GetDay();
	return (lNewDate);
}

CString CVirtualMarket::GetStringOfLocalTime(void) const {
	char buffer[30];
	tm tmLocal;

	localtime_s(&tmLocal, &sm_tUTC);
	sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfLocalDateTime(void) const {
	char buffer[100];
	tm tmLocal;

	localtime_s(&tmLocal, &sm_tUTC);
	sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfMarketTime(void) {
	char buffer[30];

	sprintf_s(buffer, _T("%02d:%02d:%02d "), m_tmMarket.tm_hour, m_tmMarket.tm_min, m_tmMarket.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfMarketDateTime(void) {
	char buffer[100];

	sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), m_tmMarket.tm_year + 1900, m_tmMarket.tm_mon + 1, m_tmMarket.tm_mday, m_tmMarket.tm_hour, m_tmMarket.tm_min, m_tmMarket.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfMarketDate(void) const {
	return ConvertDateToString(m_lMarketDate);
}

void CVirtualMarket::TaskResetMarketFlagAtMidnight(long lCurrentTime) {
	// 午夜过后重置各种标识
	if (lCurrentTime <= 1500 && !HaveResetMarketPermission()) {
		// 在零点到零点十五分，重置系统标识
		m_fResetMarketPermission = true;
		CString str = m_strMarketId + _T("重置系统重置标识");
		TRACE(_T("%S \n"), str.GetBuffer());
		gl_systemMessage.PushInformationMessage(str);
	}
}

bool CVirtualMarket::SchedulingTaskPerSecond(long lSeconds) {
	const long lCurrentTime = GetMarketTime();

	// 各调度程序按间隔时间大小顺序排列，间隔时间长的必须位于间隔时间短的之前。
	SchedulingTaskPerMinute(lSeconds, lCurrentTime);

	return true;
}

bool CVirtualMarket::SchedulingTaskPerMinute(long lSeconds, long lCurrentTime) {
	// 计算每分钟一次的任务。所有的定时任务，要按照时间间隔从长到短排列，即现执行每分钟一次的任务，再执行每秒钟一次的任务，这样能够保证长间隔的任务优先执行。
	m_i1MinuteCounter -= lSeconds;
	if (m_i1MinuteCounter < 0) {
		m_i1MinuteCounter = 59; // 重置计数器
		TaskResetMarketFlagAtMidnight(lCurrentTime);
		return true;
	}
	else {
		return false;
	}
}
