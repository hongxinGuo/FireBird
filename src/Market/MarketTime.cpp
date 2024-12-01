#include "pch.h"

#include "MarketTime.h"

#include "TimeConvert.h"

CMarketTime::CMarketTime() {
	m_lMarketLastTradeDate = 0;
	m_lMarketTime = 0;
	m_lMarketDate = 0;
	m_tmMarket.tm_year = 1970;
	m_tmMarket.tm_yday = 1;

	m_lMarketTimeZone = -8 * 3600; // 本系统默认标准时间为东八区（北京标准时间）。 //Note timeZoneOffset
}

CMarketTime::CMarketTime(const CString& strLocalPosition) {
	m_lMarketTimeZone = GetMarketLocalTimeOffset(strLocalPosition); //
}

void CMarketTime::Initialize(const CString& strLocalPosition, long OpenTime) {
	m_lMarketTimeZone = GetMarketLocalTimeOffset(strLocalPosition); //
	m_lOpenMarketTime = OpenTime;
}

void CMarketTime::CalculateTime() noexcept {
	time(&gl_tUTCTime);

	GetMarketTimeStruct(&m_tmMarket, gl_tUTCTime, m_lMarketTimeZone);
	m_lMarketDate = ConvertToDate(&m_tmMarket);
	m_lMarketTime = ConvertToTime(&m_tmMarket);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//bug 这个函数导致内存泄漏，估计是时区数据库初始化后，程序退出时没有卸载。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
time_t CMarketTime::GetMarketLocalTimeOffset(CString strLocalNameOfMarket) {
	m_tzMarket = chrono::locate_zone(strLocalNameOfMarket.GetBuffer());
	m_localMarketTimeZoneSystemInformation = m_tzMarket->get_info(chrono::sys_seconds());
	m_localMarketTimeZoneLocalInformation = m_tzMarket->get_info(chrono::local_seconds());
	m_lMarketTimeZone = -m_localMarketTimeZoneSystemInformation.offset.count(); //Note timeZoneOffset

	return m_lMarketTimeZone;
}

tm CMarketTime::GetMarketTime(time_t tUTC) const {
	tm tm_{};

	GetMarketTimeStruct(&tm_, tUTC, m_lMarketTimeZone);

	return tm_;
}

time_t CMarketTime::TransferToUTCTime(tm* tmMarketTime) const {
	return _mkgmtime(tmMarketTime) + m_lMarketTimeZone;
}

time_t CMarketTime::TransferToUTCTime(long lMarketDate, long lMarketTime) const {
	return ConvertToTTime(lMarketDate, m_lMarketTimeZone, lMarketTime);
}

long CMarketTime::GetMarketDate(time_t tUTC) const {
	tm tm_{};

	GetMarketTimeStruct(&tm_, tUTC, m_lMarketTimeZone);

	return ConvertToDate(&tm_);
}

long CMarketTime::CalculateNextTradeDate() noexcept {
	time_t tMarket;
	const tm tmMarketTime2 = GetMarketTime(gl_tUTCTime - m_lOpenMarketTime);

	switch (tmMarketTime2.tm_wday) {
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

long CMarketTime::CalculateCurrentTradeDate() noexcept {
	time_t tMarket;

	const tm tmMarketTime2 = GetMarketTime(gl_tUTCTime - m_lOpenMarketTime);

	switch (tmMarketTime2.tm_wday) {
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

long CMarketTime::CalculateLastTradeDate() noexcept {
	time_t tMarket;
	const tm tmMarketTime2 = GetMarketTime(gl_tUTCTime - m_lOpenMarketTime);

	switch (tmMarketTime2.tm_wday) {
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

bool CMarketTime::IsWorkingDay() const noexcept {
	if (m_tmMarket.tm_wday == 0 || m_tmMarket.tm_wday == 6) {
		return false;
	}
	return true;
}

bool CMarketTime::IsWorkingDay(CTime timeCurrent) noexcept {
	if ((timeCurrent.GetDayOfWeek() == 1) || (timeCurrent.GetDayOfWeek() == 7)) {
		return false;
	}
	return true;
}

bool CMarketTime::IsWorkingDay(long lDate) noexcept {
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;
	const CTime ct(year, month, day, 12, 0, 0);

	if ((ct.GetDayOfWeek() == 1) || (ct.GetDayOfWeek() == 7)) {
		return false;
	}
	return true;
}

CString CMarketTime::GetStringOfLocalTime() const {
	char buffer[30];
	tm tmLocal;

	localtime_s(&tmLocal, &gl_tUTCTime);
	sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	CString str = buffer;
	return (str);
}

CString CMarketTime::GetStringOfLocalDateTime() const {
	char buffer[100];
	tm tmLocal;

	localtime_s(&tmLocal, &gl_tUTCTime);
	sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	CString str = buffer;
	return (str);
}

CString CMarketTime::GetStringOfMarketTime() const {
	char buffer[30];

	sprintf_s(buffer, _T("%02d:%02d:%02d "), m_tmMarket.tm_hour, m_tmMarket.tm_min, m_tmMarket.tm_sec);
	CString str = buffer;
	return (str);
}

CString CMarketTime::GetStringOfMarketDateTime() const {
	char buffer[100];

	sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), m_tmMarket.tm_year + 1900, m_tmMarket.tm_mon + 1, m_tmMarket.tm_mday, m_tmMarket.tm_hour, m_tmMarket.tm_min, m_tmMarket.tm_sec);
	CString str = buffer;
	return (str);
}

CString CMarketTime::GetStringOfMarketDate() const {
	return ConvertDateToChineseTimeStampString(m_lMarketDate);
}

long CMarketTime::ConvertToDate(const time_t tUTC) const noexcept {
	tm tm_;
	GetMarketTimeStruct(&tm_, tUTC, m_lMarketTimeZone);
	return ((tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// 将逝去的时间转换成UTC时间。
//
////////////////////////////////////////////////////////////////////////////////////////////
time_t CMarketTime::ConvertBufferToTime(CString strFormat, const char* BufferMarketTime) {
	tm tm_{ 0, 0, 0, 0, 0, 0 };
	int year, month, day, hour, minute, second;

	sscanf_s(BufferMarketTime, strFormat.GetBuffer(), &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	time_t tt = _mkgmtime(&tm_); // 先变成GMT时间
	if (tt > -1) {
		tt += GetMarketTimeZone(); // 然后改成本市场UTC时间
	}
	return tt;
}

time_t CMarketTime::ConvertStringToTime(CString strFormat, CString strMarketTime) {
	tm tm_{ 0, 0, 0, 0, 0, 0 };
	int year, month, day, hour, minute, second;

	sscanf_s(strMarketTime.GetBuffer(), strFormat.GetBuffer(), &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	time_t tt = _mkgmtime(&tm_);
	if (tt > -1) {
		tt += GetMarketTimeZone(); // 然后改成本市场UTC时间
	}
	return tt;
}
