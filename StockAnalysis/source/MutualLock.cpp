#include "stdafx.h"

#include "MutualLock.h"

CMutualLock::CMutualLock( ) : CObject() {
	m_fWaitDayKLineReply = false;	
	m_fWaitPankouReply = false;

	m_fShowUpdownStock = false;
	m_fShowSelfChoicedStock = false;
	m_fShowShowStock = false;

	m_fDebugRTData = false;
}
