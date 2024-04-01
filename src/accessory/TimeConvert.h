#pragma once

// ʱ��ת������������time_tʹ��UTC��GMT����׼��������ʽ��ʱ��Ϊ���г��ı�׼ʱ�䣬Ĭ�ϲ��ö�������׼ʱ�䡣
[[nodiscard]] long GetDayOfWeek();
[[nodiscard]] time_t ConvertToTTime(long lDate, time_t tTimeZone, long lTime); // ������(YYYYMMDD)ת��Ϊtime_t��ʽ.
[[nodiscard]] long ConvertToDate(time_t tUTC, time_t tTimeZone) noexcept;// ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
[[nodiscard]] long ConvertToTime(time_t tUTC, time_t tTimeZone) noexcept; // ��ʱ��ת��Ϊ������HHMMSS)��ʽ
[[nodiscard]] INT64 ConvertToDateTime(time_t tUTC, time_t tTimeZone) noexcept; // ��ʱ��ת��Ϊ������YYYYMMDDHHMMSS)��ʽ
[[nodiscard]] long ConvertToDate(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
[[nodiscard]] long ConvertToTime(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(HHMMSS)��ʽ
[[nodiscard]] INT64 ConvertToDateTime(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDDHHMMSS)��ʽ
[[nodiscard]] bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays);
[[nodiscard]] inline long XferYearMonthDayToYYYYMMDD(long lYear, long lMonth, long lDay) { return lYear * 10000 + lMonth * 100 + lDay; }
long TimeSpawn(long lLatelyDate, long lEarlyDate);

void XferDateToYearMonthDay(long lDate, int& year, int& month, int& day);

long GetNextDay(long lDate, long lTimeSpanDays = 1) noexcept;
long GetPrevDay(long lDate, long lTimeSpanDays = 1) noexcept;

[[nodiscard]] long GetNextMonday(long lDate); // �ҵ�lDate����һ������һ����ֵ
[[nodiscard]] long GetPrevMonday(long lDate); // �ҵ�lDate����һ������һ����ֵ
[[nodiscard]] long GetCurrentMonday(long lDate); // �ҵ�����lDate���Ǹ����ڵ�����һ
[[nodiscard]] long GetNextSecond(long lTime); // �õ�lTime����һ��ʱ�䣨HHMMSS��ʽ��
[[nodiscard]] long GetNextTime(long lTime, long hh, long mm, long ss);

// ʱ��֧�ֺ���
void GetUTCTimeStruct(tm* tm_, const time_t* tUTC);
void GetMarketTimeStruct(tm* tm_, time_t tUTC, time_t tTimeZone);

// ʱ��任����buffer�е��ַ�������strFormat����ʽ�任��time_t��ʽ������ʱ�䣬����UTC��GMT����׼ʱ��
[[nodiscard]] CString ConvertDateToString(long lDate, const CString& strFormat);
[[nodiscard]] CString ConvertDateToTimeStamp(long lDate);
[[nodiscard]] CString ConvertDateToChineseTimeStampString(long lDate);
[[nodiscard]] time_t ConvertBufferToTime(CString strFormat, const char* BufferMarketTime, time_t tTimeZoneOffset);
[[nodiscard]] time_t ConvertStringToTime(CString strFormat, CString strMarketTime, time_t tTimeZoneOffset);

[[nodiscard]] time_t XferToTTime(CString strTime, const CString& strFormat);
