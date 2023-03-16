#include"pch.h"

#include"TimeConvert.h"

#include "VirtualMarket.h"

CVirtualMarket::CVirtualMarket(void) {
	m_tLastTime = 0;
	m_fResetMarketPermission = true; // ����ϵͳ�����ñ�ʶ��Ψ���˱�ʶ������ϵͳ���á���ʼʱ����Ϊ�棺��������ϵͳ��
	m_fResetMarket = true;
	m_fReadyToRun = true;
	m_fSystemReady = true; // Ĭ��Ϊ��

	m_lMarketLastTradeDate = 0;
	m_lMarketTime = 0;
	m_lMarketDate = 0;
	m_tmMarket.tm_year = 1970;
	m_tmMarket.tm_yday = 1;

	m_strMarketId = _T("Warning: CVirtualMarket Called.");
	m_lMarketTimeZone = -8 * 3600; // ��ϵͳĬ�ϱ�׼ʱ��Ϊ��������������׼ʱ�䣩��

	m_i10SecondCounter = 9; // ʮ��һ�εļ�����
	m_i1MinuteCounter = 59; // һ����һ�εļ�����
	m_i5MinuteCounter = 299; // �����һ�εļ�����
	m_i1HourCounter = 3599; // һСʱһ�εļ�����

	m_lastTimeSchedulingTask = 0;
	m_iCount1Hour = 3576; // �������ÿ�εĴ�11���ӣ���һ����ÿ�εĴ�22����
	m_iCount5Minute = 287; // ��һ����ÿ�εĴ�11����
	m_iCount1Minute = 58; // ��10��ÿ�εĴ�1����
	m_iCount10Second = 9;
}

/// <summary>
/// ���벢����Data source�����ݣ��������������SchedulingTask���ȡ�
/// �˺�����VirtualMarket�ж��壬�������������������ã�ӦΪlCurrentTime����Ϊ��������ĵ�ǰ�г�ʱ�䡣
/// </summary>
/// <param name="lCurrentTime"></param>
void CVirtualMarket::RunDataSource(long lCurrentTime) const {
	for (const auto& pDataSource : m_vDataSource) {
		if (pDataSource->IsEnable()) pDataSource->Run(lCurrentTime);
	}
}

void CVirtualMarket::ResetMarket(void) {
	ASSERT(0); // ��������û��������г�����������ֻ��Ϊ�˲��Է����ԭ��Ŷ���һ��ʵ�֡�
}

bool CVirtualMarket::UpdateMarketInfo(void) {
	// do nothing
	return true;
}

void CVirtualMarket::AddTask(long lTaskType, long lExecuteTime) {
	const auto pTask = make_shared<CMarketTask>();
	pTask->SetType(lTaskType);
	pTask->SetTime(lExecuteTime);
	StoreMarketTask(pTask);
}

tm CVirtualMarket::TransferToMarketTime(time_t tUTC) const {
	tm tm_{};

	GetMarketTimeStruct(&tm_, tUTC, m_lMarketTimeZone);

	return tm_;
}

time_t CVirtualMarket::TransferToUTCTime(tm* tmMarketTime) const {
	return _mkgmtime(tmMarketTime) + m_lMarketTimeZone;
}

time_t CVirtualMarket::TransferToUTCTime(long lMarketDate, long lMarketTime) const {
	return ConvertToTTime(lMarketDate, m_lMarketTimeZone, lMarketTime);
}

long CVirtualMarket::TransferToMarketDate(time_t tUTC) const {
	tm tm_{};

	GetMarketTimeStruct(&tm_, tUTC, m_lMarketTimeZone);

	return (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
}

void CVirtualMarket::CalculateTime(void) noexcept {
	time(&gl_tUTC);

	m_tmMarket = TransferToMarketTime();
	m_lMarketDate = (m_tmMarket.tm_year + 1900) * 10000 + (m_tmMarket.tm_mon + 1) * 100 + m_tmMarket.tm_mday;
	m_lMarketTime = m_tmMarket.tm_hour * 10000 + m_tmMarket.tm_min * 100 + m_tmMarket.tm_sec;
}

void CVirtualMarket::CalculateLastTradeDate(void) noexcept {
	time_t tMarket;

	switch (m_tmMarket.tm_wday) {
	case 1: // ����һ
		tMarket = gl_tUTC - 3 * 24 * 3600; //
		break;
	case 0: //������
		tMarket = gl_tUTC - 3 * 24 * 3600; //
		break;
	case 6: // ������
		tMarket = gl_tUTC - 2 * 24 * 3600; //
		break;
	default: // ����
		tMarket = gl_tUTC - 24 * 3600; //
	}
	const tm tmMarketTime = TransferToMarketTime(tMarket);
	m_lMarketLastTradeDate = (tmMarketTime.tm_year + 1900) * 10000 + (tmMarketTime.tm_mon + 1) * 100 + tmMarketTime.tm_mday;
}

bool CVirtualMarket::IsWorkingDay(void) const noexcept {
	if ((m_tmMarket.tm_wday == 0) || (m_tmMarket.tm_wday == 6)) {
		return false;
	}
	return true;
}

bool CVirtualMarket::IsWorkingDay(CTime timeCurrent) const noexcept {
	if ((timeCurrent.GetDayOfWeek() == 1) || (timeCurrent.GetDayOfWeek() == 7)) {
		return false;
	}
	return true;
}

bool CVirtualMarket::IsWorkingDay(long lDate) const noexcept {
	const long year = lDate / 10000;
	const long month = lDate / 100 - year * 100;
	const long day = lDate - year * 10000 - month * 100;
	const CTime ct(year, month, day, 12, 0, 0);

	if ((ct.GetDayOfWeek() == 1) || (ct.GetDayOfWeek() == 7)) {
		return false;
	}
	return true;
}

CString CVirtualMarket::GetStringOfLocalTime(void) const {
	char buffer[30];
	tm tmLocal;

	localtime_s(&tmLocal, &gl_tUTC);
	sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfLocalDateTime(void) const {
	char buffer[100];
	tm tmLocal;

	localtime_s(&tmLocal, &gl_tUTC);
	sprintf_s(buffer, _T("%04d��%02d��%02d�� %02d:%02d:%02d "), tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfMarketTime(void) const {
	char buffer[30];

	sprintf_s(buffer, _T("%02d:%02d:%02d "), m_tmMarket.tm_hour, m_tmMarket.tm_min, m_tmMarket.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfMarketDateTime(void) const {
	char buffer[100];

	sprintf_s(buffer, _T("%04d��%02d��%02d�� %02d:%02d:%02d "), m_tmMarket.tm_year + 1900, m_tmMarket.tm_mon + 1, m_tmMarket.tm_mday, m_tmMarket.tm_hour, m_tmMarket.tm_min, m_tmMarket.tm_sec);
	CString str = buffer;
	return (str);
}

CString CVirtualMarket::GetStringOfMarketDate(void) const {
	return ConvertDateToChineseTimeStampString(m_lMarketDate);
}

void CVirtualMarket::ResetMarketFlagAtMidnight(long lCurrentTime) {
	// ��ҹ�������ø��ֱ�ʶ
	if (!HaveResetMarketPermission() && lCurrentTime <= 100) {	// ����㵽���ʮ��֣�����ϵͳ��ʶ
		m_fResetMarketPermission = true;
		CString str = m_strMarketId + _T("����ϵͳ���ñ�ʶ");
		TRACE(_T("%S \n"), str.GetBuffer());
		gl_systemMessage.PushInformationMessage(str);
	}
}
