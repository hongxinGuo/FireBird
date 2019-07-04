#include "stdafx.h"

#include "CurrentTime.h"

CCurrentTime::CCurrentTime(void)
{
	m_tCurrentTime = 0;
}

CCurrentTime::~CCurrentTime(void)
{
}

time_t CCurrentTime::GetCurrentTime() {
	return m_tCurrentTime;
}

void CCurrentTime::SetCurrentTime( time_t Time ) {
	m_tCurrentTime = Time; 
}

long CCurrentTime::ChangeTimeToDay( time_t time ) {
	ASSERT( time >= 0 );
	CTime ct( time );
	long lDay = ct.GetYear() * 10000 + ct.GetMonth() * 100 + ct.GetDay();
	return ( lDay );
}

time_t CCurrentTime::ChangeDayToMarketCloseTime( long lDay ) {
  ASSERT ( lDay > 19700000 );
  long lYear = lDay / 10000;
  long lMonth = (lDay - lYear * 10000)/100;
  long lD = (lDay - lYear * 10000 - lMonth * 100);
	CTime ct( lYear, lMonth, lD, 15, 0, 0 );	// 北京时间15时即UTC7时
  return ( ct.GetTime() );
}

