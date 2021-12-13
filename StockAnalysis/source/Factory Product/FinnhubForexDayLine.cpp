#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"FinnhubForexSymbol.h"

#include "FinnhubForexDayLine.h"

IMPLEMENT_DYNCREATE(CFinnhubForexDayLine, CWebSourceDataProduct)

CFinnhubForexDayLine::CFinnhubForexDayLine() {
	m_strClassName = _T("Finnhub forex dayline");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/forex/candle?symbol=");
	m_lIndex = -1;
}

CString CFinnhubForexDayLine::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CForexSymbolPtr pForexSymbol = ((CWorldMarket*)m_pMarket)->GetForexSymbol(m_lIndex);
	CString strMessage = m_strInquiringStr + pForexSymbol->GetFinnhubDayLineInquiryString(((CWorldMarket*)m_pMarket)->GetUTCTime());
	pForexSymbol->SetDayLineNeedUpdate(false);

	return strMessage;
}

bool CFinnhubForexDayLine::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CForexSymbolPtr pForexSymbol = ((CWorldMarket*)m_pMarket)->GetForexSymbol(m_lIndex);
	if (((CWorldMarket*)m_pMarket)->ParseFinnhubForexCandle(pWebData, pForexSymbol)) {
		TRACE("处理%s日线数据\n", pForexSymbol->GetSymbol().GetBuffer());
	}
	return true;
}