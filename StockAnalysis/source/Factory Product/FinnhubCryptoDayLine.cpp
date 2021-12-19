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
	long lTemp = 0;

	CDayLineVectorPtr pvDayLine = nullptr;
	CCryptoSymbolPtr pCryptoSymbol = ((CWorldMarket*)m_pMarket)->GetCryptoSymbol(m_lIndex);
	pvDayLine = ((CWorldMarket*)m_pMarket)->ParseFinnhubCryptoCandle(pWebData);
	if (pvDayLine->size() > 0) {
		for (auto& pDayLine : *pvDayLine) {
			pDayLine->SetExchange(pCryptoSymbol->GetExchangeCode());
			pDayLine->SetStockSymbol(pCryptoSymbol->GetSymbol());
			lTemp = TransferToDate(pDayLine->m_time, ((CWorldMarket*)m_pMarket)->GetMarketTimeZone());
			pDayLine->SetDate(lTemp);
		}
		pCryptoSymbol->UpdateDayLine(*pvDayLine);
		pCryptoSymbol->SetIPOStatus(__STOCK_IPOED__);
		pCryptoSymbol->SetDayLineNeedUpdate(false);
		pCryptoSymbol->SetDayLineNeedSaving(true);
		pCryptoSymbol->SetUpdateProfileDB(true);
	}
	return true;
}