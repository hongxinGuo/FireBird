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
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMiddle = ((CWorldMarket*)m_pMarket)->GetForexExchange(m_lIndex);

	return m_strInquiringStr + strMiddle;
}

bool CFinnhubForexSymbolProduct::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	vector<CForexSymbolPtr> vForexSymbol;

	if (((CWorldMarket*)m_pMarket)->ParseFinnhubForexSymbol(pWebData, vForexSymbol)) {
		for (auto& pSymbol : vForexSymbol) {
			if (!((CWorldMarket*)m_pMarket)->IsForexSymbol(pSymbol->GetSymbol())) {
				pSymbol->SetExchangeCode(((CWorldMarket*)m_pMarket)->GetForexExchange(m_lIndex));
				((CWorldMarket*)m_pMarket)->AddForexSymbol(pSymbol);
			}
		}
	}

	return true;
}