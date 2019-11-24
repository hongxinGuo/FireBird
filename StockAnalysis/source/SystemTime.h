#pragma once

#include"afxmt.h"

class CSystemTime final {
public:
  CSystemTime(void);
  ~CSystemTime(void);

  time_t Gett_time(void) { return m_ttime; }

  long GetDay(void) noexcept { return m_lToday; }
  long GetTime(void) noexcept { return m_lTime; }
  long GetLastTradeDay(void) noexcept { CalculateLastTradeDay(); return m_lLastTradeDay; }
  long GetDayOfWeek(void) noexcept { return m_tm.tm_wday; }

  void CalculateTime(void) noexcept;
  void CalculateLastTradeDay(void) noexcept;

  CString GetTimeString(void) noexcept;

  bool IsWorkingDay(void) noexcept;
  bool IsWorkingDay(CTime timeCurrent) noexcept;

  // ʱ��ת����������
  time_t FormatToTTime(long lDay, long lTime = 150000) noexcept; // ������(YYYYMMDD)ת��Ϊtime_t��ʽ
  long FormatToDay(time_t tt) noexcept;// ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
  long FormatToDay(tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ

public:
  // �����ú���
  void Sett_time(time_t Time) noexcept;
  void SetTime(long lTime); // �˺���ֻ���ڲ���

protected:
  long m_lToday; // �������ڡ���ʽΪ��19990102
  long m_lTime;	 // ����ʱ�䡣��ʽΪ��hhmmss
  long m_lLastTradeDay; // ��һ�������գ������ж��Ƿ������������ݡ��������ݵ������գ�������һ�������գ��������յ�������ʵʱ���ݲ�����
  tm m_tm; // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������
  time_t m_ttime; // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������
};
