#include"pch.h"

#include"WorldMarket.h"
#include "FinnhubForexSymbol.h"

CFinnhubForexSymbol::CFinnhubForexSymbol() {
	CFinnhubForexSymbol::Reset();
}

void CFinnhubForexSymbol::Reset() {
	CVirtualStock::Reset();
	m_strDescription = _T(" ");
	m_strDisplaySymbol = _T(" ");
	m_strExchangeCode = _T(" ");
	m_strSymbol = _T(" ");
	m_lDayLineStartDate = 29900101;
	m_lDayLineEndDate = 19800101;
}

void CFinnhubForexSymbol::SetCheckingDayLineStatus() {
	ASSERT(IsUpdateDayLine()); // Ĭ��״̬Ϊ����������Ҫ����
	if (m_lIPOStatus == _STOCK_NULL_) {
		SetUpdateDayLine(false);
	}
	// ���ٸ����������ݱ��ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
	else if (gl_pWorldMarket->GetLastTradeDate() <= GetDayLineEndDate()) {
		// ������������Ϊ���ջ�����һ�������յ����ݡ�
		SetUpdateDayLine(false); // �������ݲ���Ҫ����
	}
}

CString CFinnhubForexSymbol::GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const {
	CString strParam = _T("");
	char buffer[50];

	strParam += m_strSymbol;
	strParam += _T("&resolution=D");
	strParam += _T("&from=");
	time_t tStartTime = gl_pWorldMarket->TransferToUTCTime(m_lDayLineEndDate);
	if (tStartTime < (tCurrentTime - static_cast<time_t>(365) * 24 * 3600)) {
		// ����˻�ֻ�ܶ�ȡһ�����ڵ��������ݡ�
		tStartTime = (tCurrentTime - static_cast<time_t>(365) * 24 * 3600);
	}
	sprintf_s(buffer, _T("%I64i"), tStartTime);
	CString strTemp = buffer;
	strParam += strTemp;
	strParam += _T("&to=");
	sprintf_s(buffer, _T("%I64i"), tCurrentTime);
	strTemp = buffer;
	strParam += strTemp;

	return strParam;
}

void CFinnhubForexSymbol::UpdateDayLineStartEndDate() {
	long lStartDate = 0, lEndDate = 0;
	const bool fSucceed = m_dataDayLine.GetStartEndDate(lStartDate, lEndDate);
	if (!fSucceed) {
		SetDayLineStartDate(29900101);
		SetDayLineEndDate(19800101);
	}
	else {
		if (lStartDate < GetDayLineStartDate()) {
			SetDayLineStartDate(lStartDate);
			m_fUpdateProfileDB = true;
		}
		if (lEndDate > m_lDayLineEndDate) {
			SetDayLineEndDate(lEndDate);
			m_fUpdateProfileDB = true;
		}
	}
}

bool CFinnhubForexSymbol::HaveNewDayLineData() const {
	if (m_dataDayLine.Empty()) return false;
	if (m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate() > m_lDayLineEndDate) return true;
	return false;
}
