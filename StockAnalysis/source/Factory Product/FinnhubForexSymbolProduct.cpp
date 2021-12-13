#include "pch.h"

#include"globedef.h"

#include "FinnhubForexSymbolProduct.h"

IMPLEMENT_DYNCREATE(CFinnhubForexSymbolProduct, CWebSourceDataProduct)

CFinnhubForexSymbolProduct::CFinnhubForexSymbolProduct() {
	m_strClassName = _T("Finnhub forex symbols");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/forex/symbol?exchange=");
	m_lIndex = -1;
}

CString CFinnhubForexSymbolProduct::CreatMessage(void) {
	CString strMiddle = gl_pWorldMarket->GetForexExchange(m_lIndex);

	return m_strInquiringStr + strMiddle;
}

bool CFinnhubForexSymbolProduct::ProcessWebData(CWebDataPtr pWebData) {
	vector<CForexSymbolPtr> vForexSymbol;

	if (gl_pWorldMarket->ParseFinnhubForexSymbol(pWebData, vForexSymbol)) {
		for (auto& pSymbol : vForexSymbol) {
			if (!gl_pWorldMarket->IsForexSymbol(pSymbol->GetSymbol())) {
				pSymbol->SetExchangeCode(gl_pWorldMarket->GetForexExchange(m_lIndex));
				gl_pWorldMarket->AddForexSymbol(pSymbol);
			}
		}
	}

	return true;
}