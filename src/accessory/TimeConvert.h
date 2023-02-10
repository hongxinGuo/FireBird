#pragma once

#include"globedef.h"

// ʱ��ת������������time_tʹ��UTC��GMT����׼��������ʽ��ʱ��Ϊ���г��ı�׼ʱ�䣬Ĭ�ϲ��ö�������׼ʱ�䡣
[[nodiscard]] long GetDayOfWeek(void);
[[nodiscard]] time_t ConvertToTTime(const long lDate, const time_t tTimeZone, const long lTime); // ������(YYYYMMDD)ת��Ϊtime_t��ʽ.
[[nodiscard]] long ConvertToDate(const time_t tUTC, const time_t tTimeZone = -8 * 3600) noexcept;// ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ, Ĭ�϶�������׼ʱ��
[[nodiscard]] long ConvertToTime(const time_t tUTC, const time_t tTimeZone = -8 * 3600) noexcept; // ��ʱ��ת��Ϊ������HHMMSS)��ʽ, Ĭ�϶�������׼ʱ��
[[nodiscard]] INT64 ConvertToDateTime(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept; // ��ʱ��ת��Ϊ������YYYYMMDDHHMMSS)��ʽ, Ĭ�϶�������׼ʱ��
[[nodiscard]] long ConvertToDate(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
[[nodiscard]] long ConvertToTime(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(HHMMSS)��ʽ
[[nodiscard]] INT64 ConvertToDateTime(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDDHHMMSS)��ʽ
[[nodiscard]] bool IsEarlyThen(const long lEarlyDate, const long lLatelyDate, const long lTimeSpawnOfDays);
long TimeSpawn(long lLatelyDate, long lEarlyDate);

void XferDateToYearMonthDay(long lDate, int& year, int& month, int& day);

long GetNextDay(long lDate, long lTimeSpanDays = 1) noexcept;
long GetPrevDay(long lDate, long lTimeSpanDays = 1) noexcept;

[[nodiscard]] long GetNextMonday(const long lDate); // �ҵ�lDate����һ������һ����ֵ
[[nodiscard]] long GetPrevMonday(const long lDate); // �ҵ�lDate����һ������һ����ֵ
[[nodiscard]] long GetCurrentMonday(const long lDate); // �ҵ�����lDate���Ǹ����ڵ�����һ

// ʱ��֧�ֺ���
void GetUTCTimeStruct(tm* tm_, const time_t* tUTC);
void GetMarketTimeStruct(tm* tm_, time_t tUTC, const time_t tTimeZone);

// ʱ��任����buffer�е��ַ�������strFormat����ʽ�任��time_t��ʽ������ʱ�䣬����UTC��GMT����׼ʱ��
[[nodiscard]] CString ConvertDateToString(const long lDate, CString strFormat);
[[nodiscard]] CString ConvertDateToTimeStamp(const long lDate);
[[nodiscard]] CString ConvertDateToChineseTimeStampString(const long lDate);
[[nodiscard]] time_t ConvertBufferToTime(CString strFormat, char* bufferMarketTime, const time_t tTimeZoneOffset = -8 * 3600); // Ĭ�ϲ��ö�������׼ʱ��
[[nodiscard]] time_t ConvertStringToTime(CString strFormat, CString strMarketTime, const time_t tTimeZoneOffset = -8 * 3600); // Ĭ�ϲ��ö�������׼ʱ��

[[nodiscard]] time_t XferToTTime(CString strTime, CString strFormat);

inline time_t GetUTCTime(void) noexcept { return gl_tUTC; }
inline void TestSetUTCTime(time_t time) noexcept { gl_tUTC = time; }
