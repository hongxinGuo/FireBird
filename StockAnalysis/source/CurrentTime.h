#pragma once

class CCurrentTime {
public:
	CCurrentTime(void);
	~CCurrentTime(void);

	time_t				GetCurrentTime();
	void					SetCurrentTime( time_t Time );

	long					ChangeTimeToDay( time_t time ); // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
	time_t				ChangeDayToMarketCloseTime( long lDay );// ������(YYYYMMDD)ת��Ϊtime_t��ʽ

protected:
	time_t													m_tCurrentTime;				// ��ǰʱ��

};
