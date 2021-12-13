#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"FinnhubCryptoSymbol.h"

#include "FinnhubCryptoDayLine.h"

IMPLEMENT_DYNCREATE(CFinnhubCryptoDayLine, CWebSourceDataProduct)

CFinnhubCryptoDayLine::CFinnhubCryptoDayLine() {
	m_strClassName = _T("Finnhub crypto dayline");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/crypto/candle?symbol=");
	m_lIndex = -1;
}

CString CFinnhubCryptoDayLine::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));
	CCryptoSymbolPtr pCryptoSymbol = ((CWorldMarket*)m_pMarket)->GetCryptoSymbol(m_lIndex);
	CString strMessage = m_strInquiringStr + pCryptoSymbol->GetFinnhubDayLineInquiryString(((CWorldMarket*)m_pMarket)->GetUTCTime());

	return strMessage;
}

bool CFinnhubCryptoDayLine::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CCryptoSymbolPtr pCryptoSymbol = ((CWorldMarket*)m_pMarket)->GetCryptoSymbol(m_lIndex);
	if (((CWorldMarket*)m_pMarket)->ParseFinnhubCryptoCandle(pWebData, pCryptoSymbol)) {
		//TRACE("处理%s日线数据\n", m_vCryptoSymbol.at(m_CurrentFinnhubInquiry.m_lStockIndex)->GetSymbol().GetBuffer());
	}
	return true;
}