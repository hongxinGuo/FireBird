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
	CString strMiddle = gl_pWorldMarket->GetCryptoExchange(m_lIndex);

	return m_strInquiringStr + strMiddle;
}

bool CFinnhubCryptoSymbolProduct::ProcessWebData(CWebDataPtr pWebData) {
	vector<CCryptoSymbolPtr> vCryptoSymbol;

	if (gl_pWorldMarket->ParseFinnhubCryptoSymbol(pWebData, vCryptoSymbol)) {
		for (auto& pSymbol : vCryptoSymbol) {
			if (!gl_pWorldMarket->IsCryptoSymbol(pSymbol->GetSymbol())) {
				pSymbol->SetExchangeCode(gl_pWorldMarket->GetCryptoExchange(m_lIndex));
				gl_pWorldMarket->AddCryptoSymbol(pSymbol);
			}
		}
	}

	return true;
}