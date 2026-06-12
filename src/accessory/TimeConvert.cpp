#include"pch.h"

#include "TimeConvert.h"

bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays) {
	ASSERT(lEarlyDate >= 19700101);
	ASSERT(lLatelyDate >= 19700101);
	const CTimeSpan ts(lTimeSpawnOfDays, 0, 0, 0);
	const long year = lEarlyDate / 10000;
	const long month = lEarlyDate / 100 - year * 100;
	const long day = lEarlyDate - year * 10000 - month * 100;
	CTime ctEarly(year, month, day, 12, 0, 0);
	ctEarly += ts;
	const long lNewDate = ctEarly.GetYear() * 10000 + ctEarly.GetMonth() * 100 + ctEarly.GetDay();
	return (lNewDate < lLatelyDate);
}

long XferToYYYYMMDD(const string& sDate) {
	chrono::year_month_day ymd;
	stringstream ss(sDate);
	chrono::from_stream(ss, "%F", ymd);
	return static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day());
}

using namespace std::chrono;

void XferDateToYearMonthDay(long lDate, int& year, int& month, int& day) {
	year = lDate / 10000;
	month = (lDate - year * 10000) / 100;
	day = lDate - year * 10000 - month * 100;
}

long GetNextMonth(long lDate) noexcept {
	long year = lDate / 10000;
	unsigned int month = lDate / 100 - year * 100;
	chrono::year y{ year };
	chrono::month m{ month };
	auto ymd = year_month_day{ y, m, day(1) } + months(1);

	return static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + 01;
}

long GetNextDay(long lDate, long lTimeSpanDays) noexcept {
	const CTimeSpan ts(lTimeSpanDays, 0, 0, 0);
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;
	CTime ctDay(year, month, day, 12, 0, 0);
	ctDay += ts;
	const long lNewDate = ctDay.GetYear() * 10000 + ctDay.GetMonth() * 100 + ctDay.GetDay();
	return (lNewDate);
}

long GetPrevDay(long lDate, long lTimeSpanDays) noexcept {
	const CTimeSpan ts(lTimeSpanDays, 0, 0, 0);
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;
	CTime ctDay(year, month, day, 12, 0, 0);
	ctDay -= ts;
	const long lNewDate = ctDay.GetYear() * 10000 + ctDay.GetMonth() * 100 + ctDay.GetDay();
	return (lNewDate);
}

static const CTimeSpan s_1Day(1, 0, 0, 0);
static const CTimeSpan s_2Day(2, 0, 0, 0);
static const CTimeSpan s_3Day(3, 0, 0, 0);
static const CTimeSpan s_4Day(4, 0, 0, 0);
static const CTimeSpan s_5Day(5, 0, 0, 0);
static const CTimeSpan s_6Day(6, 0, 0, 0);
static const CTimeSpan s_7Day(7, 0, 0, 0);

long GetNextMonday(long lDate) {
	const long year = lDate / 10000;
	const long month = lDate / 100 - (lDate / 10000) * 100;
	const long mDay = lDate - (lDate / 100) * 100;
	const CTime ctCurrent(year, month, mDay, 12, 0, 0);

	CTime ctNext = ctCurrent;
	switch (ctCurrent.GetDayOfWeek()) {
	case 2: // 星期一
		ctNext += s_7Day;
		break;
	case 3:
		ctNext += s_6Day;
		break;
	case 4:
		ctNext += s_5Day;
		break;
	case 5:
		ctNext += s_4Day;
		break;
	case 6:
		ctNext += s_3Day;
		break;
	case 7:
		ctNext += s_2Day;
		break;
	case 1:
		ctNext += s_1Day;
		break;
	default: // 不可能
		break;
	}
	const long lNextDay = ctNext.GetYear() * 10000 + ctNext.GetMonth() * 100 + ctNext.GetDay();

	return lNextDay;
}

long GetPrevMonday(long lDate) {
	const long year = lDate / 10000;
	const long month = lDate / 100 - (lDate / 10000) * 100;
	const long monthOfDay = lDate - (lDate / 100) * 100;
	const CTime ctCurrent(year, month, monthOfDay, 12, 0, 0);

	CTime ctNext = ctCurrent;
	switch (ctCurrent.GetDayOfWeek()) {
	case 2: // 星期一
		ctNext -= s_7Day;
		break;
	case 3: // 星期二
		ctNext -= s_1Day;
		break;
	case 4: // 星期三
		ctNext -= s_2Day;
		break;
	case 5: // 星期四
		ctNext -= s_3Day;
		break;
	case 6:
		ctNext -= s_4Day;
		break;
	case 7: // 星期六
		ctNext -= s_5Day;
		break;
	case 1: // 星期日
		ctNext -= s_6Day;
		break;
	default: // 不可能
		break;
	}
	const long lPrevMonday = ctNext.GetYear() * 10000 + ctNext.GetMonth() * 100 + ctNext.GetDay();

	return lPrevMonday;
}

long GetCurrentMonday(long lDate) {
	const long year = lDate / 10000;
	const long month = lDate / 100 - (lDate / 10000) * 100;
	const long monthOfDay = lDate - (lDate / 100) * 100;
	const CTime ctCurrent(year, month, monthOfDay, 12, 0, 0);

	CTime ctNext = ctCurrent;
	switch (ctCurrent.GetDayOfWeek()) {
	case 2: // 星期一
		break;
	case 3: // 星期二
		ctNext -= s_1Day;
		break;
	case 4: // 星期三
		ctNext -= s_2Day;
		break;
	case 5: // 星期四
		ctNext -= s_3Day;
		break;
	case 6:
		ctNext -= s_4Day;
		break;
	case 7: // 星期六
		ctNext -= s_5Day;
		break;
	case 1: // 星期日
		ctNext -= s_6Day;
		break;
	default: // 不可能
		break;
	}
	const long lCurrentMonday = ctNext.GetYear() * 10000 + ctNext.GetMonth() * 100 + ctNext.GetDay();

	return lCurrentMonday;
}

chrono::local_seconds GetNextSecond(chrono::hh_mm_ss<chrono::seconds> time) {
	return chrono::local_seconds(time.to_duration() + chrono::seconds(1));
}

chrono::local_seconds GetNextSecond(chrono::local_seconds time) {
	return chrono::local_seconds(time + chrono::seconds(1));
}

long GetNextTime(const long lTime, const long hh, const long mm, const long ss) {
	const long h = lTime / 10000;
	const long m = (lTime - h * 10000) / 100;
	const long s = lTime - h * 10000 - m * 100;
	long mTemp = 0;
	long hTemp = 0;
	long mEnd, sEnd;
	if (s + ss > 59) {
		mTemp = 1;
		sEnd = s + ss - 60;
	}
	else {
		sEnd = s + ss;
	}
	if (m + mm + mTemp > 59) {
		hTemp = 1;
		mEnd = m + mm + mTemp - 60;
	}
	else {
		mEnd = m + mm + mTemp;
	}
	const long hEnd = h + hh + hTemp;

	return hEnd * 10000 + mEnd * 100 + sEnd;
}

chrono::local_seconds GetNextTime(chrono::hh_mm_ss<chrono::seconds> time, chrono::hours hour, chrono::minutes minute, chrono::seconds second) {
	chrono::local_seconds newTime{ chrono::local_seconds(time.to_duration() + hour + minute + second) };
	return newTime;
}

chrono::local_seconds GetNextTime(chrono::local_seconds time, chrono::hours hour, chrono::minutes minute, chrono::seconds second) {
	auto newTime = time.time_since_epoch() + hour + minute + second;
	return chrono::local_seconds(newTime);
}

long GetPrevTime(long lTime, long hh, long mm, long ss) {
	const long h = lTime / 10000;
	const long m = (lTime - h * 10000) / 100;
	const long s = lTime - h * 10000 - m * 100;
	long mTemp = 0;
	long hTemp = 0;
	long mEnd, sEnd;
	if (s - ss < 0) {
		mTemp = 1;
		sEnd = s - ss + 60;
	}
	else {
		sEnd = s - ss;
	}
	if (m - mm - mTemp < 0) {
		hTemp = 1;
		mEnd = m - mm - mTemp + 60;
	}
	else {
		mEnd = m - mm - mTemp;
	}
	const long hEnd = h - hh - hTemp;

	return hEnd * 10000 + mEnd * 100 + sEnd;
}

chrono::local_seconds GetPrevTime(chrono::hh_mm_ss<chrono::seconds> time, chrono::hours hour, chrono::minutes minute, chrono::seconds second) {
	chrono::local_seconds newTime{ chrono::local_seconds(time.to_duration() - hour - minute - second) };
	return newTime;
}

chrono::local_seconds GetPrevTime(chrono::local_seconds time, chrono::hours hour, chrono::minutes minute, chrono::seconds second) {
	auto newTime = time.time_since_epoch() - hour - minute - second;
	return chrono::local_seconds(newTime);
}

string ConvertDateToTimeStamp(const long lDate) {
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;
	return std::format("{:04Ld}-{:02Ld}-{:02Ld}", year, month, day);
}

string ConvertDateToChineseTimeStampString(const long lDate) {
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;

	return std::format("{:04Ld}年{:02Ld}月{:02Ld}日", year, month, day);
}

string FormatToMK(int64_t iNumber) {
	string s;
	if (iNumber > 1024 * 1024) { // 1M以上的流量？
		s = std::format("{:L}M", iNumber / (1024 * 1024));
	}
	else if (iNumber > 1024) { // 1K以上的流量？
		s = std::format("{:4L}K", iNumber / 1024);
	}
	else {
		s = std::format("{:4L}", iNumber);
	}

	return s;
}
