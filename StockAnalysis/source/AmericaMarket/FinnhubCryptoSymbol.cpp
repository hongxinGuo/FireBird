#include"pch.h"
#include"globedef.h"

#include "FinnhubCryptoSymbol.h"

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

bool CFinnhubCryptoSymbol::UpdateDayLineDB(void) {
	SaveDayLine();
	UpdateDayLineStartEndDate();
	SetUpdateProfileDB(true);

	return true;
}

bool CFinnhubCryptoSymbol::SaveDayLine() {
	CSetCryptoDayLine setCryptoDayLine;
	size_t lSize = 0;
	vector<CDayLinePtr> vDayLine;
	CDayLinePtr pDayLine = nullptr;
	long lCurrentPos = 0, lSizeOfOldDayLine = 0;
	bool fNeedUpdate = false;

	ASSERT(m_vDayLine.size() > 0);

	lSize = m_vDayLine.size();
	setCryptoDayLine.m_strFilter = _T("[Symbol] = '");
	setCryptoDayLine.m_strFilter += m_strSymbol + _T("'");
	setCryptoDayLine.m_strSort = _T("[Date]");

	setCryptoDayLine.Open();
	while (!setCryptoDayLine.IsEOF()) {
		pDayLine = make_shared<CDayLine>();
		pDayLine->LoadHistoryCandleBasic(&setCryptoDayLine);
		vDayLine.push_back(pDayLine);
		lSizeOfOldDayLine++;
		setCryptoDayLine.MoveNext();
	}
	setCryptoDayLine.Close();
	if (lSizeOfOldDayLine > 0) {
		if (vDayLine.at(0)->GetMarketDate() < m_lDayLineStartDate) {
			m_lDayLineStartDate = vDayLine.at(0)->GetMarketDate();
		}
	}

	lCurrentPos = 0;
	setCryptoDayLine.m_strFilter = _T("[ID] = 1");
	setCryptoDayLine.Open();
	setCryptoDayLine.m_pDatabase->BeginTrans();
	for (int i = 0; i < lSize; i++) { // 数据是正序存储的，需要从头部开始存储
		pDayLine = m_vDayLine.at(i);
		while ((lCurrentPos < lSizeOfOldDayLine) && (vDayLine.at(lCurrentPos)->GetMarketDate() < pDayLine->GetMarketDate())) lCurrentPos++;
		if (lCurrentPos < lSizeOfOldDayLine) {
			if (vDayLine.at(lCurrentPos)->GetMarketDate() > pDayLine->GetMarketDate()) {
				pDayLine->AppendHistoryCandleBasic(&setCryptoDayLine);
				fNeedUpdate = true;
			}
		}
		else {
			pDayLine->AppendHistoryCandleBasic(&setCryptoDayLine);
			fNeedUpdate = true;
		}
	}
	setCryptoDayLine.m_pDatabase->CommitTrans();
	setCryptoDayLine.Close();

	return fNeedUpdate;
}

void CFinnhubCryptoSymbol::SetCheckingDayLineStatus(void) {
	ASSERT(IsDayLineNeedUpdate()); // 默认状态为日线数据需要更新
	if (m_lIPOStatus == __STOCK_NULL__) {
		SetDayLineNeedUpdate(false);
	}
	// 不再更新日线数据比上个交易日要新的股票。其他所有的股票都查询一遍，以防止出现新股票或者老的股票重新活跃起来。
	else if (gl_pWorldMarket->GetLastTradeDate() <= GetDayLineEndDate()) { // 最新日线数据为今日或者上一个交易日的数据。
		SetDayLineNeedUpdate(false); // 日线数据不需要更新
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
	tStartTime = TransferToTTime(m_lDayLineEndDate, gl_pWorldMarket->GetMarketTimeZone());
	if (tStartTime < (tCurrentTime - (time_t)(365) * 24 * 3600)) {// 免费账户只能读取一年以内的日线数据。
		tStartTime = (tCurrentTime - (time_t)(365) * 24 * 3600);
	}
	sprintf_s(buffer, _T("%I64i"), (INT64)tStartTime);
	strTemp = buffer;
	strMiddle += strTemp;
	strMiddle += _T("&to=");
	sprintf_s(buffer, _T("%I64i"), tCurrentTime);
	strTemp = buffer;
	strMiddle += strTemp;

	return strMiddle;
}

void CFinnhubCryptoSymbol::UpdateDayLine(vector<CDayLinePtr>& vDayLine) {
	m_vDayLine.resize(0);
	for (auto& pDayLine : vDayLine) {
		m_vDayLine.push_back(pDayLine);
	}
}

void CFinnhubCryptoSymbol::UpdateDayLineStartEndDate(void) {
	if (m_vDayLine.size() == 0) {
		SetDayLineStartDate(29900101);
		SetDayLineEndDate(19800101);
	}
	else {
		if (m_vDayLine.at(0)->GetMarketDate() < GetDayLineStartDate()) {
			SetDayLineStartDate(m_vDayLine.at(0)->GetMarketDate());
			m_fUpdateProfileDB = true;
		}
		if (m_vDayLine.at(m_vDayLine.size() - 1)->GetMarketDate() > m_lDayLineEndDate) {
			SetDayLineEndDate(m_vDayLine.at(m_vDayLine.size() - 1)->GetMarketDate());
			m_fUpdateProfileDB = true;
		}
	}
}

bool CFinnhubCryptoSymbol::HaveNewDayLineData(void) {
	if (m_vDayLine.size() <= 0) return false;
	if (m_vDayLine.at(m_vDayLine.size() - 1)->GetMarketDate() > m_lDayLineEndDate) return true;
	else return false;
}