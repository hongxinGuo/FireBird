#pragma once

using std::chrono::local_days;
using std::chrono::year_month_day;
using std::chrono::days;
using std::chrono::local_seconds;
using std::chrono::seconds;
using std::chrono::minutes;
using std::chrono::hours;
using std::chrono::day;
using std::chrono::month;
using std::chrono::year;

using std::chrono::hh_mm_ss;
using std::string;

inline bool IsEarlyThen(local_days earlyDate, local_days latelyDate, long lTimeSpawnOfDays) {
	return (earlyDate + days{ lTimeSpawnOfDays } < latelyDate);
}

local_days XferToLocalDays(const string& sDate);

year_month_day GetNextMonth(year_month_day ymd) noexcept;
local_days GetNextMonth(local_days ld) noexcept;

inline local_days GetNextDay(local_days ld, long lTimeSpanDays = 1) noexcept { return ld + days{ lTimeSpanDays }; }
inline local_days GetNextDay(year_month_day ymd, long lTimeSpanDays = 1) noexcept { return local_days{ ymd } + days{ lTimeSpanDays }; }

inline local_days GetPrevDay(local_days ld, long lTimeSpanDays = 1) noexcept { return ld - days{ lTimeSpanDays }; }
inline local_days GetPrevDay(year_month_day ymd, long lTimeSpanDays = 1) noexcept { return local_days{ ymd } - days{ lTimeSpanDays }; }

local_days GetNextMonday(local_days ld);
local_days GetPrevMonday(local_days ld);
local_days GetCurrentMonday(local_days ld);

inline local_seconds GetNextSecond(const hh_mm_ss<seconds>& time) { return local_seconds(time.to_duration() + seconds(1)); }
inline local_seconds GetNextSecond(local_seconds time) { return local_seconds(time + seconds(1)); }

inline local_seconds GetNextTime(const hh_mm_ss<seconds>& time, hours hour, minutes minute, seconds second) {
	return { local_seconds(time.to_duration() + hour + minute + second) };
}
inline local_seconds GetNextTime(local_seconds time, hours hour, minutes minute, seconds second) {
	return local_seconds(time.time_since_epoch() + hour + minute + second);
}

inline local_seconds GetPrevTime(const hh_mm_ss<seconds>& time, hours hour, minutes minute, seconds second) {
	return local_seconds(time.to_duration() - hour - minute - second);
}
inline local_seconds GetPrevTime(local_seconds time, hours hour, minutes minute, seconds second) {
	return local_seconds(time.time_since_epoch() - hour - minute - second);
}

inline local_days toLocalDays(unsigned date) { return local_days{ year{ static_cast<int>(date) / 10000 } / month{ (date % 10000) / 100 } / day{ date % 100 } }; }
inline local_days toLocalDays(year_month_day ymd) { return local_days{ ymd }; }

inline local_seconds toLocalTime(const hh_mm_ss<seconds>& time) { return local_seconds{ time.to_duration() }; }
inline local_seconds toLocalTime(unsigned time) { return local_seconds{ seconds{ (time / 10000) * 3600 + ((time % 10000) / 100) * 60 + (time % 100) } }; }

inline year_month_day toYearMonthDay(local_days ld) { return year_month_day{ ld }; }
inline year_month_day toYearMonthDay(unsigned date) { return year_month_day{ year{ static_cast<int>(date) / 10000 } / month{ (date % 10000) / 100 } / day{ date % 100 } }; }

inline hh_mm_ss<seconds> toTodayClock(local_seconds ls) { return hh_mm_ss{ ls - floor<days>(ls) }; }

// Combine a local date and a local time-of-day into a full local_seconds datetime.
inline local_seconds toLocalDateTime(local_days ld, local_seconds ls) noexcept {
	auto tod = ls - floor<days>(ls); // 提取当天的 duration（seconds）
	return local_seconds{ ld + tod };
}

inline int toFormattedDate(local_days ld) {
	year_month_day ymd{ ld };
	return static_cast<int>(static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day()));
}
inline int toFormattedDate(year_month_day ymd) { return static_cast<int>(static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day())); }

inline unsigned toFormattedTime(const hh_mm_ss<seconds>& time) { return static_cast<unsigned>(time.hours().count() * 10000 + time.minutes().count() * 100 + time.seconds().count()); }
inline unsigned toFormattedTime(local_seconds ls) { return toFormattedTime(toTodayClock(ls)); }

[[nodiscard]] string FormatToMK(int64_t iNumber);

// 时间变换。将buffer中的字符串根据strFormat的制式变换成time_t制式的日期时间，采用UTC（GMT）标准时间
inline string ConvertDateToTimeStamp(local_days date) { return std::format("{:%F}", date); }
inline string toFormattedDateString(int date) { return std::format("{:8d}", date); }
inline string toFormattedDateString(local_days ld) { return toFormattedDateString(toFormattedDate(ld)); }
