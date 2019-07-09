#include "stdafx.h"

#include "SystemTime.h"

CSystemTime::CSystemTime(void)
{
	m_tCurrentTime = 0;
}

CSystemTime::~CSystemTime(void)
{
}

time_t CSystemTime::GetCurrentTime() {
	return m_tCurrentTime;
}

void CSystemTime::SetCurrentTime( time_t Time ) {
	m_tCurrentTime = Time; 
}

long CSystemTime::ChangeTimeToDay( time_t time ) {
	ASSERT( time >= 0 );
	CTime ct( time );
	long lDay = ct.GetYear() * 10000 + ct.GetMonth() * 100 + ct.GetDay();
	return ( lDay );
}

time_t CSystemTime::ChangeDayToMarketCloseTime( long lDay ) {
  ASSERT ( lDay > 19700000 );
  long lYear = lDay / 10000;
  long lMonth = (lDay - lYear * 10000)/100;
  long lD = (lDay - lYear * 10000 - lMonth * 100);
	CTime ct( lYear, lMonth, lD, 15, 0, 0 );	// 北京时间15时即UTC7时
  return ( ct.GetTime() );
}

