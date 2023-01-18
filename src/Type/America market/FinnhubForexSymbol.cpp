#include"pch.h"

#include"WorldMarket.h"
#include "FinnhubForexSymbol.h"

CFinnhubForexSymbol::CFinnhubForexSymbol() : CVirtualStock() {
	CFinnhubForexSymbol::Reset();
}

void CFinnhubForexSymbol::Reset(void) {
	CVirtualStock::Reset();
	m_strDescription = _T(" ");
	m_strDisplaySymbol = _T(" ");
	m_strExchangeCode = _T(" ");
	m_strSymbol = _T(" ");
	m_lDayLineStartDate = 29900101;
	m_lDayLineEndDate = 19800101;
}

void CFinnhubForexSymbol::SetCheckingDayLineStatus(void) {
	ASSERT(IsDayLineNeedUpdate()); // 默认状态为日线数据需要更新
	if (m_lIPOStatus == _STOCK_NULL_) {
		SetDayLineNeedUpdate(false);
	}
	// 不再更新日线数据比上个交易日要新的股票。其他所有的股票都查询一遍，以防止出现新股票或者老的股票重新活跃起来。
	else if (gl_pWorldMarket->GetLastTradeDate() <= GetDayLineEndDate()) {
		// 最新日线数据为今日或者上一个交易日的数据。
		SetDayLineNeedUpdate(false); // 日线数据不需要更新
	}
}

CString CFinnhubForexSymbol::GetFinnhubDayLineInquiryString(time_t tCurrentTime) {
	CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
	char buffer[50];

	strMiddle += m_strSymbol;
	strMiddle += _T("&resolution=D");
	strMiddle += _T("&from=");
	time_t tStartTime = gl_pWorldMarket->TransferToUTCTime(m_lDayLineEndDate);
	if (tStartTime < (tCurrentTime - static_cast<time_t>(365) * 24 * 3600)) {
		// 免费账户只能读取一年以内的日线数据。
		tStartTime = (tCurrentTime - static_cast<time_t>(365) * 24 * 3600);
	}
	sprintf_s(buffer, _T("%I64i"), tStartTime);
	CString strTemp = buffer;
	strMiddle += strTemp;
	strMiddle += _T("&to=");
	sprintf_s(buffer, _T("%I64i"), tCurrentTime);
	strTemp = buffer;
	strMiddle += strTemp;

	return strMiddle;
}

void CFinnhubForexSymbol::UpdateDayLineStartEndDate(void) {
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

bool CFinnhubForexSymbol::HaveNewDayLineData(void) {
	if (m_dataDayLine.Size() == 0) return false;
	if (m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate() > m_lDayLineEndDate) return true;
	return false;
}
