#pragma once

class CSystemTime {
public:
	CSystemTime(void);
	~CSystemTime(void);

  time_t				Gett_time(void) { return m_ttime; }
  void					Sett_time(time_t Time) { m_ttime = Time; }

  long          GetDay(void) { return m_lToday; }
  long          GetTime(void) { return m_lTime; }
  long          GetLastTradeDay(void) { return m_lLastTradeDay; }
  long          GetDayOfWeek(void) { return m_tm.tm_wday; }

  CString       GetTimeStr(void);

  void          CalculateTime(void);
  void          CalculateLastTradeDay(void);

	long					ChangeTimeToDay( time_t time ); // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
	time_t				ChangeDayToMarketCloseTime( long lDay );// ������(YYYYMMDD)ת��Ϊtime_t��ʽ

protected:
  long    m_lToday;          // �������ڡ���ʽΪ��19990102
  long    m_lTime;				  // ����ʱ�䡣��ʽΪ��hhmmss
  long    m_lLastTradeDay; // ��һ�������գ������ж��Ƿ������������ݡ��������ݵ������գ�������һ�������գ��������յ�������ʵʱ���ݲ�����
  tm      m_tm;						  // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������
  time_t  m_ttime;       // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������

};
