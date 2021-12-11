#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"FinnhubForexSymbol.h"

#include "FinnhubForexDayLine.h"

CFinnhubForexDayLine::CFinnhubForexDayLine() {
	m_strClassName = _T("Finnhub forex dayline");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/forex/candle?symbol=");
	m_lIndex = -1;
}

CString CFinnhubForexDayLine::CreatMessage(void) {
	CForexSymbolPtr pForexSymbol = gl_pWorldMarket->GetForexSymbol(m_lIndex);
	CString strMessage = m_strInquiringStr + pForexSymbol->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime());

	return strMessage;
}

bool CFinnhubForexDayLine::ProcessWebData(CWebDataPtr pWebData) {
	CForexSymbolPtr pForexSymbol = gl_pWorldMarket->GetForexSymbol(m_lIndex);
	if (gl_pWorldMarket->ParseFinnhubForexCandle(pWebData, pForexSymbol)) {
		TRACE("处理%s日线数据\n", pForexSymbol->GetSymbol().GetBuffer());
	}
	return true;
}