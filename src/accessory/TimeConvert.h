#pragma once

#include"globedef.h"

// 时间转换辅助函数。time_t使用UTC（GMT）标准，其他形式的时间为该市场的标准时间，默认采用东八区标准时间。
[[nodiscard]] long GetDayOfWeek(void);
[[nodiscard]] time_t ConvertToTTime(const long lDate, const time_t tTimeZone, const long lTime); // 将整型(YYYYMMDD)转变为time_t形式.
[[nodiscard]] long ConvertToDate(const time_t tUTC, const time_t tTimeZone = -8 * 3600) noexcept;// 将时间转变为整型(YYYYMMDD)形式, 默认东八区标准时间
[[nodiscard]] long ConvertToTime(const time_t tUTC, const time_t tTimeZone = -8 * 3600) noexcept; // 将时间转变为整数（HHMMSS)形式, 默认东八区标准时间
[[nodiscard]] INT64 ConvertToDateTime(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept; // 将时间转变为整数（YYYYMMDDHHMMSS)形式, 默认东八区标准时间
[[nodiscard]] long ConvertToDate(const tm* ptm) noexcept; // 将时间转变为整型(YYYYMMDD)形式
[[nodiscard]] long ConvertToTime(const tm* ptm) noexcept; // 将时间转变为整型(HHMMSS)形式
[[nodiscard]] INT64 ConvertToDateTime(const tm* ptm) noexcept; // 将时间转变为整型(YYYYMMDDHHMMSS)形式
[[nodiscard]] bool IsEarlyThen(const long lEarlyDate, const long lLatelyDate, const long lTimeSpawnOfDays);
long TimeSpawn(long lLatelyDate, long lEarlyDate);

void XferDateToYearMonthDay(long lDate, int& year, int& month, int& day);

long GetNextDay(long lDate, long lTimeSpanDays = 1) noexcept;
long GetPrevDay(long lDate, long lTimeSpanDays = 1) noexcept;

[[nodiscard]] long GetNextMonday(const long lDate); // 找到lDate的下一个星期一的数值
[[nodiscard]] long GetPrevMonday(const long lDate); // 找到lDate的上一个星期一的数值
[[nodiscard]] long GetCurrentMonday(const long lDate); // 找到包含lDate的那个星期的星期一

// 时间支持函数
void GetUTCTimeStruct(tm* tm_, const time_t* tUTC);
void GetMarketTimeStruct(tm* tm_, time_t tUTC, const time_t tTimeZone);

// 时间变换。将buffer中的字符串根据strFormat的制式变换成time_t制式的日期时间，采用UTC（GMT）标准时间
[[nodiscard]] CString ConvertDateToString(const long lDate, CString strFormat);
[[nodiscard]] CString ConvertDateToTimeStamp(const long lDate);
[[nodiscard]] CString ConvertDateToChineseTimeStampString(const long lDate);
[[nodiscard]] time_t ConvertBufferToTime(CString strFormat, char* bufferMarketTime, const time_t tTimeZoneOffset = -8 * 3600); // 默认采用东八区标准时间
[[nodiscard]] time_t ConvertStringToTime(CString strFormat, CString strMarketTime, const time_t tTimeZoneOffset = -8 * 3600); // 默认采用东八区标准时间

[[nodiscard]] time_t XferToTTime(CString strTime, CString strFormat);

inline time_t GetUTCTime(void) noexcept { return gl_tUTC; }
inline void TestSetUTCTime(time_t time) noexcept { gl_tUTC = time; }
