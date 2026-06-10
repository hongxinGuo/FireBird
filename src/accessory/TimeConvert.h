#pragma once

// 时间转换辅助函数。time_t使用UTC（GMT）标准，其他形式的时间为该市场的标准时间，默认采用东八区标准时间。
bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays);
long XferToYYYYMMDD(const string& sDate);

void XferDateToYearMonthDay(long lDate, int& year, int& month, int& day);

long GetNextMonth(long lDate) noexcept; // 得到下个月的第一天日期

long GetNextDay(long lDate, long lTimeSpanDays = 1) noexcept;
long GetPrevDay(long lDate, long lTimeSpanDays = 1) noexcept;

long GetNextMonday(long lDate); // 找到lDate的下一个星期一的数值
long GetPrevMonday(long lDate); // 找到lDate的上一个星期一的数值
long GetCurrentMonday(long lDate); // 找到包含lDate的那个星期的星期一
long GetNextSecond(long lTime); // 得到lTime的下一秒时间（HHMMSS制式）
long GetNextTime(long lTime, long hh, long mm, long ss);
chrono::hh_mm_ss<chrono::seconds> GetNextTime(chrono::hh_mm_ss<chrono::seconds> time, chrono::hours hour, chrono::minutes minute, chrono::seconds second);
long GetPrevTime(long lTime, long hh, long mm, long ss);

// 时间变换。将buffer中的字符串根据strFormat的制式变换成time_t制式的日期时间，采用UTC（GMT）标准时间
string ConvertDateToTimeStamp(long lDate);
string ConvertDateToChineseTimeStampString(long lDate);

[[nodiscard]] string FormatToMK(int64_t iNumber);
