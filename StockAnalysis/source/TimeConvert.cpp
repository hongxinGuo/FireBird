#include"pch.h"

#include "TimeConvert.h"

//using namespace std;
//#include<codecvt>

long GetDayOfWeek(void) {
	time_t tUTC = 0;
	time(&tUTC);
	tm tm_;
	GetMarketTimeStruct(&tm_, tUTC, 0);
	return tm_.tm_wday;
}

time_t TransferToTTime(long lDate, time_t tTimeZone, long lTime) {
	tm tmMarket{ 0, 0, 0, 0,0, 0 };

	ASSERT(lDate >= 19700101);
	tmMarket.tm_year = lDate / 10000 - 1900;
	tmMarket.tm_mon = lDate / 100 - (lDate / 10000) * 100 - 1;
	tmMarket.tm_mday = lDate - (lDate / 100) * 100;
	tmMarket.tm_hour = lTime / 10000;
	tmMarket.tm_min = lTime / 100 - (lTime / 10000) * 100;
	tmMarket.tm_sec = lTime - (lTime / 100) * 100;

	return _mkgmtime(&tmMarket) + tTimeZone;
}

long TransferToDate(time_t const tUTC, time_t tTimeZone) noexcept {
	tm tm_;
	GetMarketTimeStruct(&tm_, tUTC, tTimeZone);
	return((tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday);
}

long TransferToTime(time_t const tUTC, time_t tTimeZone) noexcept {
	tm tm_;
	GetMarketTimeStruct(&tm_, tUTC, tTimeZone);
	return(tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec);
}

INT64 TransferToDateTime(time_t const tUTC, time_t tTimeZone) noexcept {
	tm tm_;
	GetMarketTimeStruct(&tm_, tUTC, tTimeZone);
	return((static_cast<INT64>(tm_.tm_year) + 1900) * 10000000000 + (static_cast<INT64>(tm_.tm_mon) + 1) * 100000000 + static_cast<INT64>(tm_.tm_mday) * 1000000 + tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec);
}

long TransferToDate(const tm* ptm) noexcept {
	return((ptm->tm_year + 1900) * 10000 + (ptm->tm_mon + 1) * 100 + ptm->tm_mday);
}

long TransferToTime(const tm* ptm) noexcept {
	return(ptm->tm_hour * 10000 + ptm->tm_min * 100 + ptm->tm_sec);
}

INT64 TransferToDateTime(const tm* ptm) noexcept {
	return((static_cast<INT64>(ptm->tm_year) + 1900) * 10000000000 + (static_cast<INT64>(ptm->tm_mon) + 1) * 100000000 + static_cast<INT64>(ptm->tm_mday) * 1000000 + ptm->tm_hour * 10000 + ptm->tm_min * 100 + ptm->tm_sec);
}

bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays) {
	ASSERT(lEarlyDate >= 19700101);
	ASSERT(lLatelyDate >= 19700101);
	CTimeSpan ts(lTimeSpawnOfDays, 0, 0, 0);
	const long year = lEarlyDate / 10000;
	const long month = lEarlyDate / 100 - year * 100;
	const long day = lEarlyDate - year * 10000 - month * 100;
	CTime ctEarly(year, month, day, 12, 0, 0);
	ctEarly += ts;
	long lNewDate = ctEarly.GetYear() * 10000 + ctEarly.GetMonth() * 100 + ctEarly.GetDay();
	return (lNewDate < lLatelyDate);
}

const static CTimeSpan s_1Day(1, 0, 0, 0);
const static CTimeSpan s_2Day(2, 0, 0, 0);
const static CTimeSpan s_3Day(3, 0, 0, 0);
const static CTimeSpan s_4Day(4, 0, 0, 0);
const static CTimeSpan s_5Day(5, 0, 0, 0);
const static CTimeSpan s_6Day(6, 0, 0, 0);
const static CTimeSpan s_7Day(7, 0, 0, 0);

long GetNextMonday(long lDate) {
	const long year = lDate / 10000;
	const long month = lDate / 100 - (lDate / 10000) * 100;
	const long mday = lDate - (lDate / 100) * 100;
	const CTime ctCurrent(year, month, mday, 12, 0, 0);
	CTime ctNext;

	ctNext = ctCurrent;
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
	const long mday = lDate - (lDate / 100) * 100;
	const CTime ctCurrent(year, month, mday, 12, 0, 0);
	CTime ctNext;

	ctNext = ctCurrent;
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
	const long mday = lDate - (lDate / 100) * 100;
	const CTime ctCurrent(year, month, mday, 12, 0, 0);
	CTime ctNext;

	ctNext = ctCurrent;
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
		break;
	default: // 不可能
		break;
	}
	const long lCurrentMonday = ctNext.GetYear() * 10000 + ctNext.GetMonth() * 100 + ctNext.GetDay();

	return lCurrentMonday;
}

void GetUTCTimeStruct(tm* tm_, const time_t* tUTC) {
	gmtime_s(tm_, tUTC);
}

void GetMarketTimeStruct(tm* tm_, time_t tUTC, const time_t tTimeZone) {
	time_t tMarket;
	tMarket = tUTC - tTimeZone;
	gmtime_s(tm_, &tMarket);
}

CString ConvertDateToString(long lDate) {
	char buffer[30];
	long year = lDate / 10000;
	long month = lDate / 100 - year * 100;
	long day = lDate - year * 10000 - month * 100;

	sprintf_s(buffer, _T("%4d年%2d月%2d日"), year, month, day);
	CString str;
	str = buffer;
	return(str);
}

time_t ConvertBufferToTime(CString strFormat, const char* bufferMarketTime, time_t tTimeZoneOffset) {
	time_t tt{ 0 };
	tm tm_{ 0,0,0,0,0,0 };
	int year, month, day, hour, minute, second;

	sscanf_s(bufferMarketTime, strFormat.GetBuffer(), &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	tt = _mkgmtime(&tm_); // 先变成GMT时间
	if (tt > -1) {
		tt += tTimeZoneOffset; // 然后改成本市场UTC时间
	}
	return tt;
}

time_t ConvertStringToTime(CString strFormat, CString strMarketTime, time_t tTimeZoneOffset) {
	time_t tt{ 0 };
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
	tt = _mkgmtime(&tm_);
	if (tt > -1) {
		tt += tTimeZoneOffset; //
	}
	return tt;
}