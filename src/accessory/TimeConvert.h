#pragma once

// ʱ��ת������������time_tʹ��UTC��GMT����׼��������ʽ��ʱ��Ϊ���г��ı�׼ʱ�䣬Ĭ�ϲ��ö�������׼ʱ�䡣
time_t ConvertBufferToTime(const string& strFormat, const char* BufferMarketTime, time_t tTimeZoneOffset); // Ĭ�ϲ��ö�������׼ʱ��
time_t ConvertToTTime(long lYear, long lMonth, long lDay, long lHour, long lMinute, long lSecond, time_t tTimeZone); // ������(YYYYMMDD)ת��Ϊtime_t��ʽ.
time_t ConvertToTTime(long lDate, time_t tTimeZone, long lTime); // ������(YYYYMMDD)ת��Ϊtime_t��ʽ.
long ConvertToDate(time_t tUTC, time_t tTimeZone) noexcept;// ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
long ConvertToTime(time_t tUTC, time_t tTimeZone) noexcept; // ��ʱ��ת��Ϊ������HHMMSS)��ʽ
INT64 ConvertToDateTime(time_t tUTC, time_t tTimeZone) noexcept; // ��ʱ��ת��Ϊ������YYYYMMDDHHMMSS)��ʽ
INT64 ConvertToDateTime(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDDHHMMSS)��ʽ
bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays);
inline long XferYearMonthDayToYYYYMMDD(long lYear, long lMonth, long lDay) { return lYear * 10000 + lMonth * 100 + lDay; } //todo ����������
inline INT64 XferYearMonthDayToYYYYMMDDHHMMSS(long lYear, long lMonth, long lDay, long lHour, long lMinute, long lSecond) { return ((INT64)lYear) * 10000000000 + (INT64)lMonth * 100000000 + (INT64)lDay * 1000000 + lHour * 10000 + lMinute * 100 + lSecond; }
inline long XferToYYYYMMDD(const chrono::year_month_day& ymd) { return static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day()); }
long XferToYYYYMMDD(const string& sDate);
long TimeSpawn(long lLatelyDate, long lEarlyDate);

void XferDateToYearMonthDay(long lDate, int& year, int& month, int& day);

long GetNextDay(long lDate, long lTimeSpanDays = 1) noexcept;
long GetPrevDay(long lDate, long lTimeSpanDays = 1) noexcept;

long GetNextMonday(long lDate); // �ҵ�lDate����һ������һ����ֵ
long GetPrevMonday(long lDate); // �ҵ�lDate����һ������һ����ֵ
long GetCurrentMonday(long lDate); // �ҵ�����lDate���Ǹ����ڵ�����һ
long GetNextSecond(long lTime); // �õ�lTime����һ��ʱ�䣨HHMMSS��ʽ��
long GetNextTime(long lTime, long hh, long mm, long ss);
long GetPrevTime(long lTime, long hh, long mm, long ss);

// ʱ��֧�ֺ���
void GetUTCTimeStruct(tm* tm_, const time_t* tUTC);
void GetMarketTimeStruct(tm* tm_, time_t tUTC, time_t tTimeZone);

// ʱ��任����buffer�е��ַ�������strFormat����ʽ�任��time_t��ʽ������ʱ�䣬����UTC��GMT����׼ʱ��
string ConvertDateToTimeStamp(long lDate);
string ConvertDateToChineseTimeStampString(long lDate);

int XferChinaMarketTimeToIndex(long lTime); // lTimeΪHHMMSS��ʽ
int XferChinaMarketTimeToIndex(const tm* ptm); // lTimeΪHHMMSS��ʽ
