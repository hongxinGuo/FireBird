#pragma once

class CCurrentTime {
public:
	CCurrentTime(void);
	~CCurrentTime(void);

	time_t				GetCurrentTime();
	void					SetCurrentTime( time_t Time );

	long					ChangeTimeToDay( time_t time ); // 将时间转变为整型(YYYYMMDD)形式
	time_t				ChangeDayToMarketCloseTime( long lDay );// 将整型(YYYYMMDD)转变为time_t形式

protected:
	time_t													m_tCurrentTime;				// 当前时间

};
