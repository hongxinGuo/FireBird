#include"pch.h"

#include"WorldMarket.h"
#include "FinnhubCryptoSymbol.h"

CFinnhubCryptoSymbol::CFinnhubCryptoSymbol() {
	CFinnhubCryptoSymbol::Reset();
}

void CFinnhubCryptoSymbol::Reset() {
	CVirtualStock::Reset();
	m_strDescription = _T(" ");
	m_strDisplaySymbol = _T(" ");
	m_strExchangeCode = _T(" ");
	m_strSymbol = _T(" ");
	m_lDayLineStartDate = 29900101;
	m_lDayLineEndDate = 19800101;
}

void CFinnhubCryptoSymbol::SetCheckingDayLineStatus() {
	ASSERT(IsDayLineNeedUpdate()); // 默认状态为日线数据需要更新
	if (IsNullStock()) {
		SetDayLineNeedUpdate(false);
	}
	else if ((IsDelisted() || IsNotYetList()) && (gl_pWorldMarket->GetDayOfWeek() != 2)) {
		// 每星期二检查退市或未上市证券
		SetDayLineNeedUpdate(false);
	}
	// 不再更新日线数据比上上个交易日要新的股票。其他所有的股票都查询一遍，以防止出现新股票或者老的股票重新活跃起来。
	else if (gl_pWorldMarket->GetLastTradeDate() <= GetDayLineEndDate()) {
		// 最新日线数据为今日或者上一个交易日的数据。
		SetDayLineNeedUpdate(false); // 日线数据不需要更新
	}
}

CString CFinnhubCryptoSymbol::GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const {
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

void CFinnhubCryptoSymbol::UpdateDayLineStartEndDate() {
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

bool CFinnhubCryptoSymbol::HaveNewDayLineData() const {
	if (m_dataDayLine.Empty()) return false;
	if (m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate() > m_lDayLineEndDate) return true;
	return false;
}
