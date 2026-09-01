#include"pch.h"
#include"globedef.h"


#include"WebRTData.h"

using namespace std;

CWebRTData::CWebRTData() {
	m_tpTime = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(0));
}

bool CWebRTData::CheckSinaRTDataActive() {
	m_fActive = IsValidTime(14);
	return m_fActive;
}

bool CWebRTData::CheckTengxunRTDataActive() {
	if (!IsValidTime(14)) {// 如果交易时间在14天前
		m_fActive = false;
	}
	else if ((m_lOpen == 0) && (m_llVolume == 0) && (m_lHigh == 0) && (m_lLow == 0)) {// 腾讯非活跃股票的m_lNew不为零，故而不能使用其作为判断依据
		m_fActive = false; // 腾讯非活跃股票的实时数据也具有所有的字段，故而在此确认其为非活跃
	}
	else m_fActive = true;

	return m_fActive;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 实时数据的有效时间范围为最近两周内。当股市放假时，其最新数据是放假前的最后一天数据。春节放假时间最长，有十一天时间，加上三天富裕，
// 故而十四天内的数据都被认为是有效时间数据，这样能够保证生成当日活动股票集。
//
//////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::IsValidTime(long lDays) const {
	chrono::days day{ lDays };
	if (GetTime() < gl_tpNow - day) {// 确保实时数据不早于当前时间的14天前（春节放假最长为7天，加上前后的休息日，共十一天）
		return false;
	}
	if (m_tpTime > gl_tpNow) {
		return false;
	}
	return true;
}
