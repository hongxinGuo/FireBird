#pragma once

// 时间转换辅助函数。time_t使用UTC（GMT）标准，其他形式的时间为该市场的标准时间，默认采用东八区标准时间。
bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays);
long XferToYYYYMMDD(const string& sDate);

void XferDateToYearMonthDay(long lDate, int& year, int& month, int& day);

long GetNextMonth(long lDate) noexcept; // 得到下个月的第一天日期
chrono::year_month_day GetNextMonth(chrono::year_month_day ymd) noexcept;

long GetNextDay(long lDate, long lTimeSpanDays = 1) noexcept;
inline chrono::local_days GetNextDay(chrono::local_days ld, long lTimeSpanDays) noexcept { return ld + chrono::days{ lTimeSpanDays }; }
inline chrono::year_month_day GetNextDay(chrono::year_month_day ymd, long lTimeSpanDays) noexcept { return chrono::year_month_day{ chrono::local_days{ ymd } + chrono::days{ lTimeSpanDays } }; }

long GetPrevDay(long lDate, long lTimeSpanDays = 1) noexcept;
inline chrono::local_days GetPrevDay(chrono::local_days ld, long lTimeSpanDays) noexcept { return ld - chrono::days{ lTimeSpanDays }; }
inline chrono::year_month_day GetPrevDay(chrono::year_month_day ymd, long lTimeSpanDays) noexcept { return chrono::year_month_day{ chrono::local_days{ ymd } - chrono::days{ lTimeSpanDays } }; }

long GetNextMonday(long lDate); // 找到lDate的下一个星期一的数值
chrono::local_days GetNextMonday(chrono::local_days ld);
chrono::year_month_day GetNextMonday(chrono::year_month_day ymd);

long GetPrevMonday(long lDate); // 找到lDate的上一个星期一的数值
long GetCurrentMonday(long lDate); // 找到包含lDate的那个星期的星期一

chrono::local_seconds GetNextSecond(chrono::hh_mm_ss<chrono::seconds> time);
chrono::local_seconds GetNextSecond(chrono::local_seconds time);

chrono::local_seconds GetNextTime(chrono::hh_mm_ss<chrono::seconds> time, chrono::hours hour, chrono::minutes minute, chrono::seconds second);
chrono::local_seconds GetNextTime(chrono::local_seconds time, chrono::hours hour, chrono::minutes minute, chrono::seconds second);

chrono::local_seconds GetPrevTime(chrono::hh_mm_ss<chrono::seconds> time, chrono::hours hour, chrono::minutes minute, chrono::seconds second);
chrono::local_seconds GetPrevTime(chrono::local_seconds time, chrono::hours hour, chrono::minutes minute, chrono::seconds second);

// 时间变换。将buffer中的字符串根据strFormat的制式变换成time_t制式的日期时间，采用UTC（GMT）标准时间
string ConvertDateToTimeStamp(long lDate);
string ConvertDateToChineseTimeStampString(long lDate);

[[nodiscard]] string FormatToMK(int64_t iNumber);

inline chrono::local_days toLocalDays(unsigned date) { return chrono::local_days{ chrono::year{ static_cast<int>(date) / 10000 } / chrono::month{ (date % 10000) / 100 } / chrono::day{ date % 100 } }; }
inline chrono::local_days toLocalDays(chrono::year_month_day ymd) { return chrono::local_days{ ymd }; }

inline unsigned toUnsignedDate(chrono::local_days ld) {
	chrono::year_month_day ymd{ ld };
	return static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day());
}
inline unsigned toUnsignedDate(chrono::year_month_day ymd) { return static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day()); }

inline chrono::year_month_day toYearMonthDay(chrono::local_days ld) { return chrono::year_month_day{ ld }; }
inline chrono::year_month_day toYearMonthDay(unsigned date) { return chrono::year_month_day{ chrono::year{ static_cast<int>(date) / 10000 } / chrono::month{ (date % 10000) / 100 } / chrono::day{ date % 100 } }; }

inline chrono::hh_mm_ss<chrono::seconds> toTodayClock(chrono::local_seconds ls) { return chrono::hh_mm_ss{ ls - chrono::floor<chrono::days>(ls) }; }
inline chrono::hh_mm_ss<chrono::seconds> toTodayClock(unsigned time) { return chrono::hh_mm_ss{ chrono::seconds{ (time / 10000) * 3600 + ((time % 10000) / 100) * 60 + (time % 100) } }; }

inline chrono::local_seconds toTimeOfDay(const chrono::hh_mm_ss<chrono::seconds>& time) { return chrono::local_seconds{ time.to_duration() }; }
inline chrono::local_seconds toTimeOfDay(unsigned time) { return chrono::local_seconds{ chrono::seconds{ (time / 10000) * 3600 + ((time % 10000) / 100) * 60 + (time % 100) } }; }

inline unsigned toUnsignedTime(const chrono::hh_mm_ss<chrono::seconds>& time) { return (time.hours().count() * 10000) + (time.minutes().count() * 100) + time.seconds().count(); }
inline unsigned toUnsignedTime(chrono::local_seconds ls) { return toUnsignedTime(toTodayClock(ls)); }
