#pragma once

#include"afxmt.h"

class CSystemTime {
public:
  CSystemTime(void);
  ~CSystemTime(void);

  time_t				Gett_time(void) { return m_ttime; }
  void					Sett_time(time_t Time) noexcept;

  long          GetDay(void) noexcept { return m_lToday; }
  long          GetTime(void) noexcept { return m_lTime; }
  void          SetTime(long lTime) noexcept { m_lTime = lTime; } // �˺���ֻ���ڲ���
  long          GetLastTradeDay(void) noexcept { CalculateLastTradeDay(); return m_lLastTradeDay; }
  long          GetDayOfWeek(void) noexcept { return m_tm.tm_wday; }

  void          CalculateTime(void) noexcept;
  void          CalculateLastTradeDay(void) noexcept;

  long					ChangeTimeToDay(time_t time) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
  time_t				ChangeDayToMarketCloseTime(long lDay) noexcept;// ������(YYYYMMDD)ת��Ϊtime_t��ʽ
  CString       GetTimeString(void) noexcept;

  bool          IsWorkingDay(void);
  bool          IsWorkingDay(CTime timeCurrent);

protected:
  long    m_lToday;         // �������ڡ���ʽΪ��19990102
  long    m_lTime;				  // ����ʱ�䡣��ʽΪ��hhmmss
  long    m_lLastTradeDay;  // ��һ�������գ������ж��Ƿ������������ݡ��������ݵ������գ�������һ�������գ��������յ�������ʵʱ���ݲ�����
  tm      m_tm;						  // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������
  time_t  m_ttime;          // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������
};
