#pragma once

#include"afxmt.h"

class CSystemTime final {
public:
  CSystemTime(void);
  ~CSystemTime(void);

  time_t Gett_time(void) { return m_ttime; }

  long GetDay(void) { return m_lToday; }
  long GetTime(void) { return m_lTime; }
  long GetLastTradeDay(void) { CalculateLastTradeDay(); return m_lLastTradeDay; }
  long GetDayOfWeek(void) { return m_tm.tm_wday; }

  void CalculateTime(void);
  void CalculateLastTradeDay(void);

  CString GetTimeString(void);

  bool IsWorkingDay(void);
  bool IsWorkingDay(CTime timeCurrent);

public:
  // �����ú���
  void __Test_Sett_time(time_t Time);
  void __Test_SetTime(long lTime); // �˺���ֻ���ڲ���
  void __Test_SetTM(tm tm_);

protected:
  long m_lToday; // �������ڡ���ʽΪ��19990102
  long m_lTime;	 // ����ʱ�䡣��ʽΪ��hhmmss
  long m_lLastTradeDay; // ��һ�������գ������ж��Ƿ������������ݡ��������ݵ������գ�������һ�������գ��������յ�������ʵʱ���ݲ�����
  tm m_tm; // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������
  time_t m_ttime; // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������
};
