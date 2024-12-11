#include"pch.h"
module;
import FireBird.Market.World;
module FireBird.Stock.FinnhubCrypto;

CFinnhubCrypto::CFinnhubCrypto() {
}

void CFinnhubCrypto::SetCheckingDayLineStatus() {
	ASSERT(IsUpdateDayLine()); // Ĭ��״̬Ϊ����������Ҫ����
	if (IsNullStock()) {
		SetUpdateDayLine(false);
	}
	else if ((IsDelisted() || IsNotYetList()) && (gl_pWorldMarket->GetDayOfWeek() != 2)) {
		// ÿ���ڶ�������л�δ����֤ȯ
		SetUpdateDayLine(false);
	}
	// ���ٸ����������ݱ����ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
	else if (gl_pWorldMarket->GetLastTradeDate() <= GetDayLineEndDate()) {
		// ������������Ϊ���ջ�����һ�������յ����ݡ�
		SetUpdateDayLine(false); // �������ݲ���Ҫ����
	}
}

CString CFinnhubCrypto::GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const {
	CString strParam = _T("");
	char buffer[50];

	strParam += m_strSymbol;
	strParam += _T("&resolution=D");
	strParam += _T("&from=");
	const time_t tStartTime = (tCurrentTime - static_cast<time_t>(365) * 24 * 3600);
	sprintf_s(buffer, _T("%I64i"), tStartTime);
	CString strTemp = buffer;
	strParam += strTemp;
	strParam += _T("&to=");
	sprintf_s(buffer, _T("%I64i"), tCurrentTime);
	strTemp = buffer;
	strParam += strTemp;

	return strParam;
}

void CFinnhubCrypto::UpdateDayLineStartEndDate() {
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
		if (lEndDate > GetDayLineEndDate()) {
			SetDayLineEndDate(lEndDate);
			m_fUpdateProfileDB = true;
		}
	}
}

bool CFinnhubCrypto::HaveNewDayLineData() {
	if (m_dataDayLine.Empty()) return false;
	if (m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate() > GetDayLineEndDate()) return true;
	return false;
}
