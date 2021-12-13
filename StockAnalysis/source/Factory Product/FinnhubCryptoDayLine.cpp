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
	CCryptoSymbolPtr pCryptoSymbol = gl_pWorldMarket->GetCryptoSymbol(m_lIndex);
	CString strMessage = m_strInquiringStr + pCryptoSymbol->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime());

	return strMessage;
}

bool CFinnhubCryptoDayLine::ProcessWebData(CWebDataPtr pWebData)
{
	CCryptoSymbolPtr pCryptoSymbol = gl_pWorldMarket->GetCryptoSymbol(m_lIndex);
	if (gl_pWorldMarket->ParseFinnhubCryptoCandle(pWebData, pCryptoSymbol)) {
		//TRACE("处理%s日线数据\n", m_vCryptoSymbol.at(m_CurrentFinnhubInquiry.m_lStockIndex)->GetSymbol().GetBuffer());
	}
	return true;
}