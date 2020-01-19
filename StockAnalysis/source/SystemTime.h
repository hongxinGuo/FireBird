#pragma once

class CSystemTime final {
public:
  CSystemTime(void);
  ~CSystemTime(void);

  time_t Gett_time(void) noexcept { return m_ttime; }

  long GetDay(void) noexcept { return m_lToday; }
  long GetDayOfWeek(void) noexcept { return m_tm.tm_wday; }
  long GetMonthOfYear(void) noexcept { return m_tm.tm_mon + 1; }
  long GetDayOfMonth(void) noexcept { return m_tm.tm_mday; }
  long GetYear(void) noexcept { return m_tm.tm_year + 1900; }
  long GetTime(void) noexcept { return m_lTime; }
  long GetLastTradeDay(void) noexcept { CalculateLastTradeDay(); return m_lLastTradeDay; }

  void CalculateTime(void);
  void CalculateLastTradeDay(void);

  CString GetTimeString(void);

  bool IsWorkingDay(void);
  bool IsWorkingDay(CTime timeCurrent);
  bool IsWorkingDay(long lDay);

  bool IsEarlyThen(long lEarlyDay, long lLatelyDay, long lTimeSpawnOfDays);
  long GetNextDay(long lDay, long lTimeSpanDays = 1);

public:
  // �����ú���
  void __Test_Sett_time(time_t Time);
  void __Test_SetTime(long lTime) noexcept { m_lTime = lTime; }// �˺���ֻ���ڲ���
  void __Test_SetTM(tm tm_);
  void __Test_SetDay(long lDay) noexcept { m_lToday = lDay; }

protected:
  long m_lToday; // �������ڡ���ʽΪ��19990102
  long m_lTime;	 // ����ʱ�䡣��ʽΪ��hhmmss
  long m_lLastTradeDay; // ��һ�������գ������ж��Ƿ������������ݡ��������ݵ������գ�������һ�������գ��������յ�������ʵʱ���ݲ�����
  tm m_tm; // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������
  time_t m_ttime; // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������
};
