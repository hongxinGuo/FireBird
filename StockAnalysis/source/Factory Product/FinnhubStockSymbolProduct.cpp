#include "pch.h"

#include"globedef.h"

#include "FinnhubStockSymbolProduct.h"

IMPLEMENT_DYNCREATE(CFinnhubStockSymbolProduct, CWebSourceDataProduct)

CFinnhubStockSymbolProduct::CFinnhubStockSymbolProduct() {
	m_strClassName = _T("Finnhub company symbols");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/symbol?exchange=");
	m_lIndex = -1;
}

CString CFinnhubStockSymbolProduct::CreatMessage(void)
{
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMiddle = ((CWorldMarket*)m_pMarket)->GetExchangeCode(m_lIndex);

	return m_strInquiringStr + strMiddle;
}

bool CFinnhubStockSymbolProduct::ProcessWebData(CWebDataPtr pWebData) {
	CString str;
	CWorldStockVectorPtr  pvStock = nullptr;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	pvStock = ((CWorldMarket*)m_pMarket)->ParseFinnhubStockSymbol(pWebData);
	// 加上交易所代码。
	for (auto& pStock3 : *pvStock) {
		pStock3->SetExchangeCode(((CWorldMarket*)m_pMarket)->GetExchangeCode(m_lIndex));
	}
	for (auto& pStock2 : *pvStock) {
		if (!((CWorldMarket*)m_pMarket)->IsStock(pStock2->GetSymbol())) {
			((CWorldMarket*)m_pMarket)->AddStock(pStock2);
			str = _T("Finnhub发现新代码:") + pStock2->GetSymbol();
			gl_systemMessage.PushInnerSystemInformationMessage(str);
		}
	}

	return true;
}