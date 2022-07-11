#pragma once

// 时间转换辅助函数。time_t使用UTC（GMT）标准，其他形式的时间为该市场的标准时间，默认采用东八区标准时间。
long GetDayOfWeek(void);
time_t TransferToTTime(long lDate, time_t tTimeZone, long lTime); // 将整型(YYYYMMDD)转变为time_t形式.
long TransferToDate(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept;// 将时间转变为整型(YYYYMMDD)形式, 默认东八区标准时间
long TransferToTime(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept; // 将时间转变为整数（HHMMSS)形式, 默认东八区标准时间
INT64 TransferToDateTime(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept; // 将时间转变为整数（YYYYMMDDHHMMSS)形式, 默认东八区标准时间
long TransferToDate(const tm* ptm) noexcept; // 将时间转变为整型(YYYYMMDD)形式
long TransferToTime(const tm* ptm) noexcept; // 将时间转变为整型(HHMMSS)形式
INT64 TransferToDateTime(const tm* ptm) noexcept; // 将时间转变为整型(YYYYMMDDHHMMSS)形式
bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays);

long GetNextMonday(long lDate); // 找到lDate的下一个星期一的数值
long GetPrevMonday(long lDate); // 找到lDate的上一个星期一的数值
long GetCurrentMonday(long lDate); // 找到包含lDate的那个星期的星期一

// 时间支持函数
void GetUTCTimeStruct(tm* tm_, const time_t* tUTC);
void GetMarketTimeStruct(tm* tm_, time_t tUTC, const time_t tTimeZone);
