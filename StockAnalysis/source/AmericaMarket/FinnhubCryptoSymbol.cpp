#include"pch.h"
#include"globedef.h"
#include"WorldMarket.h"

#include "FinnhubCryptoSymbol.h"
#include"SetFinnhubCryptoSymbol.h"
#include"SetCryptoDayLine.h"

CFinnhubCryptoSymbol::CFinnhubCryptoSymbol() : CVirtualStock() {
	Reset();
}

void CFinnhubCryptoSymbol::Reset(void) {
	CVirtualStock::Reset();
	m_strDescription = _T(" ");
	m_strDisplaySymbol = _T(" ");
	m_strExchangeCode = _T(" ");
	m_strSymbol = _T(" ");
	m_lDayLineStartDate = 29900101;
	m_lDayLineEndDate = 19800101;
}

void CFinnhubCryptoSymbol::SetCheckingDayLineStatus(void) {
	ASSERT(IsDayLineNeedUpdate()); // Ĭ��״̬Ϊ����������Ҫ����
	if (IsNullStock()) {
		SetDayLineNeedUpdate(false);
	}
	else if ((IsDelisted() || IsNotYetList()) && (gl_pWorldMarket->GetDayOfWeek() != 2)) {
		SetDayLineNeedUpdate(false);
	}
	// ���ٸ����������ݱ����ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
	else if (gl_pWorldMarket->GetLastTradeDate() <= GetDayLineEndDate()) { // ������������Ϊ���ջ�����һ�������յ����ݡ�
		SetDayLineNeedUpdate(false); // �������ݲ���Ҫ����
	}
}

CString CFinnhubCryptoSymbol::GetFinnhubDayLineInquiryString(time_t tCurrentTime) {
	CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
	CString strTemp;
	char buffer[50];
	time_t tStartTime = 0;

	strMiddle += m_strSymbol;
	strMiddle += _T("&resolution=D");
	strMiddle += _T("&from=");
	tStartTime = (tCurrentTime - (time_t)(365) * 24 * 3600);
	sprintf_s(buffer, _T("%I64i"), (INT64)tStartTime);
	strTemp = buffer;
	strMiddle += strTemp;
	strMiddle += _T("&to=");
	sprintf_s(buffer, _T("%I64i"), tCurrentTime);
	strTemp = buffer;
	strMiddle += strTemp;

	return strMiddle;
}

void CFinnhubCryptoSymbol::UpdateDayLineStartEndDate(void) {
	long lStartDate = 0, lEndDate = 0;
	bool fSucceed = m_dataDayLine.GetStartEndDate(lStartDate, lEndDate);
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

bool CFinnhubCryptoSymbol::HaveNewDayLineData(void) {
	if (m_dataDayLine.Size() == 0) return false;
	if (m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate() > m_lDayLineEndDate) return true;
	else return false;
}