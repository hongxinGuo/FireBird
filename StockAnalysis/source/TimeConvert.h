#pragma once

// ʱ��ת������������time_tʹ��UTC��GMT����׼��������ʽ��ʱ��Ϊ���г��ı�׼ʱ�䣬Ĭ�ϲ��ö�������׼ʱ�䡣
long GetDayOfWeek(void);
time_t TransferToTTime(long lDate, time_t tTimeZone, long lTime); // ������(YYYYMMDD)ת��Ϊtime_t��ʽ.
long TransferToDate(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept;// ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ, Ĭ�϶�������׼ʱ��
long TransferToTime(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept; // ��ʱ��ת��Ϊ������HHMMSS)��ʽ, Ĭ�϶�������׼ʱ��
INT64 TransferToDateTime(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept; // ��ʱ��ת��Ϊ������YYYYMMDDHHMMSS)��ʽ, Ĭ�϶�������׼ʱ��
long TransferToDate(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
long TransferToTime(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(HHMMSS)��ʽ
INT64 TransferToDateTime(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDDHHMMSS)��ʽ
bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays);

long GetNextMonday(long lDate); // �ҵ�lDate����һ������һ����ֵ
long GetPrevMonday(long lDate); // �ҵ�lDate����һ������һ����ֵ
long GetCurrentMonday(long lDate); // �ҵ�����lDate���Ǹ����ڵ�����һ

// ʱ��֧�ֺ���
void GetUTCTimeStruct(tm* tm_, const time_t* tUTC);
void GetMarketTimeStruct(tm* tm_, time_t tUTC, const time_t tTimeZone);

// ʱ��任����buffer�е��ַ�������strFormat����ʽ�任��time_t��ʽ������ʱ�䣬����UTC��GMT����׼ʱ��
CString ConvertDateToString(long lDate);
time_t ConvertBufferToTime(CString strFormat, const char* bufferMarketTime, time_t tTimeZoneOffset = -8 * 3600); // Ĭ�ϲ��ö�������׼ʱ��
time_t ConvertStringToTime(CString strFormat, CString strMarketTime, time_t tTimeZoneOffset = -8 * 3600); // Ĭ�ϲ��ö�������׼ʱ��
