#include"pch.h"
module;
module FireBird.Stock.FinnhubForex;

import FireBird.Market.World;

CFinnhubForex::CFinnhubForex() {
}

void CFinnhubForex::SetCheckingDayLineStatus() {
	ASSERT(IsUpdateDayLine()); // 默认状态为日线数据需要更新
	if (m_lIPOStatus == _STOCK_NULL_) {
		SetUpdateDayLine(false);
	}
	// 不再更新日线数据比上个交易日要新的股票。其他所有的股票都查询一遍，以防止出现新股票或者老的股票重新活跃起来。
	else if (gl_pWorldMarket->GetLastTradeDate() <= GetDayLineEndDate()) {
		// 最新日线数据为今日或者上一个交易日的数据。
		SetUpdateDayLine(false); // 日线数据不需要更新
	}
}

CString CFinnhubForex::GetFinnhubDayLineInquiryParam(time_t tCurrentTime) {
	CString strParam = _T("");
	char buffer[50];

	strParam += m_strSymbol;
	strParam += _T("&resolution=D");
	strParam += _T("&from=");
	time_t tStartTime = gl_pWorldMarket->TransferToUTCTime(GetDayLineEndDate());
	if (tStartTime < (tCurrentTime - static_cast<time_t>(365) * 24 * 3600)) {
		// 免费账户只能读取一年以内的日线数据。
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

void CFinnhubForex::UpdateDayLineStartEndDate() {
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

bool CFinnhubForex::HaveNewDayLineData() {
	if (m_dataDayLine.Empty()) return false;
	if (m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate() > GetDayLineEndDate()) return true;
	return false;
}
