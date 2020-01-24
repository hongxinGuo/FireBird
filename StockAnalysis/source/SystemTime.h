#pragma once

class CSystemTime final {
public:
  CSystemTime(void);
  ~CSystemTime(void);

  time_t Gett_time(void) noexcept { return m_ttime; }

  long GetDay(void) noexcept { return m_lLocalToday; }
  long GetDayOfWeek(void) noexcept { return m_tmLocal.tm_wday; }
  long GetMonthOfYear(void) noexcept { return m_tmLocal.tm_mon + 1; }
  long GetDayOfMonth(void) noexcept { return m_tmLocal.tm_mday; }
  long GetYear(void) noexcept { return m_tmLocal.tm_year + 1900; }
  long GetTime(void) noexcept { return m_lLocalTime; }
  long GetLastTradeDay(void) noexcept { CalculateLastTradeDay(); return m_lLocalLastTradeDay; }

  void CalculateLocalTime(void);
  void CalculateGMTTime(void);
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
  void __Test_SetTime(long lTime) noexcept { m_lLocalTime = lTime; }// �˺���ֻ���ڲ���
  void __Test_SetTM(tm tm_);
  void __Test_SetDay(long lDay) noexcept { m_lLocalToday = lDay; }

protected:
  time_t m_ttime; // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������

  long m_lLocalToday; // �������ڡ���ʽΪ��19990102
  long m_lLocalTime;	 // ����ʱ�䡣��ʽΪ��hhmmss
  long m_lLocalLastTradeDay; // ��һ�������գ������ж��Ƿ������������ݡ��������ݵ������գ�������һ�������գ��������յ�������ʵʱ���ݲ�����
  tm m_tmLocal; // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������

  long m_lGMTToday; //
  long m_lGMTTime;
  long m_lGMTLastTradeDay;
  tm m_tmGMT; // �������α�׼ʱ��
};
