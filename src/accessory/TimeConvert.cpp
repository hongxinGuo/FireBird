#include"pch.h"

#include "TimeConvert.h"

////////////////////////////////////////////////////////////////////////////////////////////
//
// 将逝去的时间转换成UTC时间。默认时区为东八区。
//
////////////////////////////////////////////////////////////////////////////////////////////
time_t ConvertBufferToTime(const string& strFormat, const char* BufferMarketTime, const time_t tTimeZoneOffset) {
	tm tm_{ 0, 0, 0, 0, 0, 0 };
	int year = 1970, month = 1, day = 0, hour = 15, minute = 0, second = 0;

	sscanf_s(BufferMarketTime, strFormat.c_str(), &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	time_t tt = _mkgmtime(&tm_); // 先变成GMT时间
	if (tt > -1) {
		tt += tTimeZoneOffset; // 然后改成本市场UTC时间
	}
	return tt;
}
time_t ConvertToTTime(long lYear, long lMonth, long lDay, long lHour, long lMinute, long lSecond, time_t tTimeZone) {
	ASSERT(lYear > 1970);
	ASSERT(lMonth > 0);
	tm tmMarket{ lSecond, lMinute, lHour, lDay, lMonth - 1, lYear - 1900 };

	return _mkgmtime(&tmMarket) + tTimeZone;
}

time_t ConvertToTTime(const long lDate, const time_t tTimeZone, const long lTime) {
	tm tmMarket{ 0, 0, 0, 0, 0, 0 };

	ASSERT(lDate >= 19700101);
	tmMarket.tm_year = lDate / 10000 - 1900;
	tmMarket.tm_mon = lDate / 100 - (lDate / 10000) * 100 - 1;
	tmMarket.tm_mday = lDate - (lDate / 100) * 100;
	tmMarket.tm_hour = lTime / 10000;
	tmMarket.tm_min = lTime / 100 - (lTime / 10000) * 100;
	tmMarket.tm_sec = lTime - (lTime / 100) * 100;

	return _mkgmtime(&tmMarket) - tTimeZone;
}

long ConvertToDate(const time_t tUTC, const time_t tTimeZone) noexcept {
	tm tm_;
	GetMarketTimeStruct(&tm_, tUTC, tTimeZone);
	return ((tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday);
}

long ConvertToTime(const time_t tUTC, const time_t tTimeZone) noexcept {
	tm tm_;
	GetMarketTimeStruct(&tm_, tUTC, tTimeZone);
	return (tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec);
}

INT64 ConvertToDateTime(const time_t tUTC, const time_t tTimeZone) noexcept {
	tm tm_;
	GetMarketTimeStruct(&tm_, tUTC, tTimeZone);
	return ((static_cast<INT64>(tm_.tm_year) + 1900) * 10000000000 + (static_cast<INT64>(tm_.tm_mon) + 1) * 100000000 +
		static_cast<INT64>(tm_.tm_mday) * 1000000 + tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec);
}

INT64 ConvertToDateTime(const tm* ptm) noexcept {
	return ((static_cast<INT64>(ptm->tm_year) + 1900) * 10000000000 + (static_cast<INT64>(ptm->tm_mon) + 1) * 100000000 +
		static_cast<INT64>(ptm->tm_mday) * 1000000 + ptm->tm_hour * 10000 + ptm->tm_min * 100 + ptm->tm_sec);
}

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
	return XferToYYYYMMDD(ymd);
}

long TimeSpawn(long lLatelyDate, long lEarlyDate) {
	ASSERT(lEarlyDate >= 19700101);
	ASSERT(lLatelyDate >= 19700101);
	ASSERT(lLatelyDate >= lEarlyDate);
	const long year = lEarlyDate / 10000;
	const long month = lEarlyDate / 100 - year * 100;
	const long day = lEarlyDate - year * 10000 - month * 100;
	const CTime ctEarly(year, month, day, 12, 0, 0);
	const long year2 = lLatelyDate / 10000;
	const long month2 = lLatelyDate / 100 - year2 * 100;
	const long day2 = lLatelyDate - year2 * 10000 - month2 * 100;
	const CTime ctLately(year2, month2, day2, 12, 0, 0);
	const CTimeSpan ts = ctLately - ctEarly;
	return (ts.GetDays());
}

void XferDateToYearMonthDay(long lDate, int& year, int& month, int& day) {
	year = lDate / 10000;
	month = (lDate - year * 10000) / 100;
	day = lDate - year * 10000 - month * 100;
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

long GetNextSecond(const long lTime) {
	const long hourMinute = lTime / 100;
	const long hh = lTime / 10000;
	const long ss = lTime - hourMinute * 100;
	if (ss == 59) {
		const long mm = hourMinute - hh * 100;
		if (mm == 59) {
			ASSERT(hh < 24);
			return (hh + 1) * 10000;
		}
		return (hourMinute + 1) * 100;
	}
	return lTime + 1;
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

void GetUTCTimeStruct(tm* tm_, const time_t* tUTC) { gmtime_s(tm_, tUTC); }

void GetMarketTimeStruct(tm* tm_, const time_t tUTC, const time_t tTimeZone) {
	time_t tMarket;
	tMarket = tUTC + tTimeZone;
	gmtime_s(tm_, &tMarket);
}

string ConvertDateToTimeStamp(const long lDate) {
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;
	return fmt::format("{:04Ld}-{:02Ld}-{:02Ld}", year, month, day);
}

string ConvertDateToChineseTimeStampString(const long lDate) {
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;

	return fmt::format("{:04Ld}年{:02Ld}月{:02Ld}日", year, month, day);
}

int XferChinaMarketTimeToIndex(long lTime) {
	const long hhmm = lTime / 100;
	const long hh = lTime / 10000;
	const long mm = hhmm - hh * 100;
	if (hh < 9) return 0;
	int i = (hh - 9) * 60 + mm - 30;
	if (i < 0) return 0;
	if (i < 120) return i;
	if (i < 210) return 120;
	if (i < 330) return i - 90;
	return 239;
}

int XferChinaMarketTimeToIndex(const tm* ptm) {
	if (ptm->tm_hour < 9) return 0;
	int i = (ptm->tm_hour - 9) * 60 + ptm->tm_min - 30;
	if (i < 0) return 0;
	if (i < 120) return i;
	if (i < 210) return 120;
	if (i < 330) return i - 90;
	return 239;
}
