#include "pch.h"

#include"globedef.h"

#include "FinnhubCryptoSymbolProduct.h"

IMPLEMENT_DYNCREATE(CFinnhubCryptoSymbolProduct, CWebSourceDataProduct)

CFinnhubCryptoSymbolProduct::CFinnhubCryptoSymbolProduct() {
	m_strClassName = _T("Finnhub crypto symbols");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/crypto/symbol?exchange=");
	m_lIndex = -1;
}

CString CFinnhubCryptoSymbolProduct::CreatMessage(void) {
	CString strMiddle = ((CWorldMarket*)m_pMarket)->GetCryptoExchange(m_lIndex);

	return m_strInquiringStr + strMiddle;
}

bool CFinnhubCryptoSymbolProduct::ProcessWebData(CWebDataPtr pWebData) {
	CCryptoSymbolVectorPtr pvCryptoSymbol;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	pvCryptoSymbol = ((CWorldMarket*)m_pMarket)->ParseFinnhubCryptoSymbol(pWebData);
	for (auto& pSymbol : *pvCryptoSymbol) {
		if (!((CWorldMarket*)m_pMarket)->IsCryptoSymbol(pSymbol->GetSymbol())) {
			pSymbol->SetExchangeCode(((CWorldMarket*)m_pMarket)->GetCryptoExchange(m_lIndex));
			((CWorldMarket*)m_pMarket)->AddCryptoSymbol(pSymbol);
		}
	}

	return true;
}