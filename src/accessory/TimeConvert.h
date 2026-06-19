#pragma once

inline bool IsEarlyThen(chrono::local_days earlyDate, chrono::local_days latelyDate, long lTimeSpawnOfDays) {
	return (earlyDate + chrono::days{ lTimeSpawnOfDays } < latelyDate);
}

chrono::local_days XferToLocalDays(const string& sDate);

chrono::year_month_day GetNextMonth(chrono::year_month_day ymd) noexcept;
chrono::local_days GetNextMonth(chrono::local_days ld) noexcept;

inline chrono::local_days GetNextDay(chrono::local_days ld, long lTimeSpanDays = 1) noexcept { return ld + chrono::days{ lTimeSpanDays }; }
inline chrono::local_days GetNextDay(chrono::year_month_day ymd, long lTimeSpanDays = 1) noexcept { return chrono::local_days{ ymd } + chrono::days{ lTimeSpanDays }; }

inline chrono::local_days GetPrevDay(chrono::local_days ld, long lTimeSpanDays = 1) noexcept { return ld - chrono::days{ lTimeSpanDays }; }
inline chrono::local_days GetPrevDay(chrono::year_month_day ymd, long lTimeSpanDays = 1) noexcept { return chrono::local_days{ ymd } - chrono::days{ lTimeSpanDays }; }

chrono::local_days GetNextMonday(chrono::local_days ld);
chrono::local_days GetPrevMonday(chrono::local_days ld);
chrono::local_days GetCurrentMonday(chrono::local_days ld);

inline chrono::local_seconds GetNextSecond(chrono::hh_mm_ss<chrono::seconds> time) { return chrono::local_seconds(time.to_duration() + chrono::seconds(1)); }
inline chrono::local_seconds GetNextSecond(chrono::local_seconds time) { return chrono::local_seconds(time + chrono::seconds(1)); }

inline chrono::local_seconds GetNextTime(chrono::hh_mm_ss<chrono::seconds> time, chrono::hours hour, chrono::minutes minute, chrono::seconds second) {
	return { chrono::local_seconds(time.to_duration() + hour + minute + second) };
}
inline chrono::local_seconds GetNextTime(chrono::local_seconds time, chrono::hours hour, chrono::minutes minute, chrono::seconds second) {
	return chrono::local_seconds(time.time_since_epoch() + hour + minute + second);
}

inline chrono::local_seconds GetPrevTime(chrono::hh_mm_ss<chrono::seconds> time, chrono::hours hour, chrono::minutes minute, chrono::seconds second) {
	return chrono::local_seconds(time.to_duration() - hour - minute - second);
}
inline chrono::local_seconds GetPrevTime(chrono::local_seconds time, chrono::hours hour, chrono::minutes minute, chrono::seconds second) {
	return chrono::local_seconds(time.time_since_epoch() - hour - minute - second);
}

inline chrono::local_days toLocalDays(unsigned date) { return chrono::local_days{ chrono::year{ static_cast<int>(date) / 10000 } / chrono::month{ (date % 10000) / 100 } / chrono::day{ date % 100 } }; }
inline chrono::local_days toLocalDays(chrono::year_month_day ymd) { return chrono::local_days{ ymd }; }

inline chrono::local_seconds toLocalTime(const chrono::hh_mm_ss<chrono::seconds>& time) { return chrono::local_seconds{ time.to_duration() }; }
inline chrono::local_seconds toLocalTime(unsigned time) { return chrono::local_seconds{ chrono::seconds{ (time / 10000) * 3600 + ((time % 10000) / 100) * 60 + (time % 100) } }; }

inline chrono::year_month_day toYearMonthDay(chrono::local_days ld) { return chrono::year_month_day{ ld }; }
inline chrono::year_month_day toYearMonthDay(unsigned date) { return chrono::year_month_day{ chrono::year{ static_cast<int>(date) / 10000 } / chrono::month{ (date % 10000) / 100 } / chrono::day{ date % 100 } }; }

inline chrono::hh_mm_ss<chrono::seconds> toTodayClock(chrono::local_seconds ls) { return chrono::hh_mm_ss{ ls - chrono::floor<chrono::days>(ls) }; }

// Combine a local date and a local time-of-day into a full local_seconds datetime.
inline chrono::local_seconds toLocalDateTime(chrono::local_days ld, chrono::local_seconds ls) noexcept {
	auto tod = ls - chrono::floor<chrono::days>(ls); // 提取当天的 duration（seconds）
	return chrono::local_seconds{ ld + tod };
}

inline long toFormattedDate(chrono::local_days ld) {
	chrono::year_month_day ymd{ ld };
	return static_cast<long>(static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day()));
}
inline long toFormattedDate(chrono::year_month_day ymd) { return static_cast<long>(static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day())); }

inline unsigned toFormattedTime(const chrono::hh_mm_ss<chrono::seconds>& time) { return (time.hours().count() * 10000) + (time.minutes().count() * 100) + time.seconds().count(); }
inline unsigned toFormattedTime(chrono::local_seconds ls) { return toFormattedTime(toTodayClock(ls)); }

[[nodiscard]] string FormatToMK(int64_t iNumber);

// 时间变换。将buffer中的字符串根据strFormat的制式变换成time_t制式的日期时间，采用UTC（GMT）标准时间
inline string ConvertDateToTimeStamp(chrono::local_days date) { return std::format("{:%F}", date); }
