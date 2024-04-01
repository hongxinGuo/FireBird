#pragma once

// 时间转换辅助函数。time_t使用UTC（GMT）标准，其他形式的时间为该市场的标准时间，默认采用东八区标准时间。
[[nodiscard]] long GetDayOfWeek();
[[nodiscard]] time_t ConvertToTTime(long lDate, time_t tTimeZone, long lTime); // 将整型(YYYYMMDD)转变为time_t形式.
[[nodiscard]] long ConvertToDate(time_t tUTC, time_t tTimeZone) noexcept;// 将时间转变为整型(YYYYMMDD)形式
[[nodiscard]] long ConvertToTime(time_t tUTC, time_t tTimeZone) noexcept; // 将时间转变为整数（HHMMSS)形式
[[nodiscard]] INT64 ConvertToDateTime(time_t tUTC, time_t tTimeZone) noexcept; // 将时间转变为整数（YYYYMMDDHHMMSS)形式
[[nodiscard]] long ConvertToDate(const tm* ptm) noexcept; // 将时间转变为整型(YYYYMMDD)形式
[[nodiscard]] long ConvertToTime(const tm* ptm) noexcept; // 将时间转变为整型(HHMMSS)形式
[[nodiscard]] INT64 ConvertToDateTime(const tm* ptm) noexcept; // 将时间转变为整型(YYYYMMDDHHMMSS)形式
[[nodiscard]] bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays);
[[nodiscard]] inline long XferYearMonthDayToYYYYMMDD(long lYear, long lMonth, long lDay) { return lYear * 10000 + lMonth * 100 + lDay; }
long TimeSpawn(long lLatelyDate, long lEarlyDate);

void XferDateToYearMonthDay(long lDate, int& year, int& month, int& day);

long GetNextDay(long lDate, long lTimeSpanDays = 1) noexcept;
long GetPrevDay(long lDate, long lTimeSpanDays = 1) noexcept;

[[nodiscard]] long GetNextMonday(long lDate); // 找到lDate的下一个星期一的数值
[[nodiscard]] long GetPrevMonday(long lDate); // 找到lDate的上一个星期一的数值
[[nodiscard]] long GetCurrentMonday(long lDate); // 找到包含lDate的那个星期的星期一
[[nodiscard]] long GetNextSecond(long lTime); // 得到lTime的下一秒时间（HHMMSS制式）
[[nodiscard]] long GetNextTime(long lTime, long hh, long mm, long ss);

// 时间支持函数
void GetUTCTimeStruct(tm* tm_, const time_t* tUTC);
void GetMarketTimeStruct(tm* tm_, time_t tUTC, time_t tTimeZone);

// 时间变换。将buffer中的字符串根据strFormat的制式变换成time_t制式的日期时间，采用UTC（GMT）标准时间
[[nodiscard]] CString ConvertDateToString(long lDate, const CString& strFormat);
[[nodiscard]] CString ConvertDateToTimeStamp(long lDate);
[[nodiscard]] CString ConvertDateToChineseTimeStampString(long lDate);
[[nodiscard]] time_t ConvertBufferToTime(CString strFormat, const char* BufferMarketTime, time_t tTimeZoneOffset);
[[nodiscard]] time_t ConvertStringToTime(CString strFormat, CString strMarketTime, time_t tTimeZoneOffset);

[[nodiscard]] time_t XferToTTime(CString strTime, const CString& strFormat);
