#pragma once

// 时间转换辅助函数。time_t使用UTC（GMT）标准，其他形式的时间为该市场的标准时间，默认采用东八区标准时间。
time_t ConvertBufferToTime(const string& strFormat, const char* BufferMarketTime, time_t tTimeZoneOffset); // 默认采用东八区标准时间
time_t ConvertToTTime(long lYear, long lMonth, long lDay, long lHour, long lMinute, long lSecond, time_t tTimeZone); // 将整型(YYYYMMDD)转变为time_t形式.
time_t ConvertToTTime(long lDate, time_t tTimeZone, long lTime); // 将整型(YYYYMMDD)转变为time_t形式.
long ConvertToDate(time_t tUTC, time_t tTimeZone) noexcept;// 将时间转变为整型(YYYYMMDD)形式
long ConvertToTime(time_t tUTC, time_t tTimeZone) noexcept; // 将时间转变为整数（HHMMSS)形式
INT64 ConvertToDateTime(time_t tUTC, time_t tTimeZone) noexcept; // 将时间转变为整数（YYYYMMDDHHMMSS)形式
INT64 ConvertToDateTime(const tm* ptm) noexcept; // 将时间转变为整型(YYYYMMDDHHMMSS)形式
bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays);
inline long XferYearMonthDayToYYYYMMDD(long lYear, long lMonth, long lDay) { return lYear * 10000 + lMonth * 100 + lDay; } //todo 换掉这两个
inline INT64 XferYearMonthDayToYYYYMMDDHHMMSS(long lYear, long lMonth, long lDay, long lHour, long lMinute, long lSecond) { return ((INT64)lYear) * 10000000000 + (INT64)lMonth * 100000000 + (INT64)lDay * 1000000 + lHour * 10000 + lMinute * 100 + lSecond; }
inline long XferToYYYYMMDD(const chrono::year_month_day& ymd) { return static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day()); }
long XferToYYYYMMDD(const string& sDate);
long TimeSpawn(long lLatelyDate, long lEarlyDate);

void XferDateToYearMonthDay(long lDate, int& year, int& month, int& day);

long GetNextDay(long lDate, long lTimeSpanDays = 1) noexcept;
long GetPrevDay(long lDate, long lTimeSpanDays = 1) noexcept;

long GetNextMonday(long lDate); // 找到lDate的下一个星期一的数值
long GetPrevMonday(long lDate); // 找到lDate的上一个星期一的数值
long GetCurrentMonday(long lDate); // 找到包含lDate的那个星期的星期一
long GetNextSecond(long lTime); // 得到lTime的下一秒时间（HHMMSS制式）
long GetNextTime(long lTime, long hh, long mm, long ss);
long GetPrevTime(long lTime, long hh, long mm, long ss);

// 时间支持函数
void GetUTCTimeStruct(tm* tm_, const time_t* tUTC);
void GetMarketTimeStruct(tm* tm_, time_t tUTC, time_t tTimeZone);

// 时间变换。将buffer中的字符串根据strFormat的制式变换成time_t制式的日期时间，采用UTC（GMT）标准时间
string ConvertDateToTimeStamp(long lDate);
string ConvertDateToChineseTimeStampString(long lDate);

int XferChinaMarketTimeToIndex(long lTime); // lTime为HHMMSS制式
int XferChinaMarketTimeToIndex(const tm* ptm); // lTime为HHMMSS制式
