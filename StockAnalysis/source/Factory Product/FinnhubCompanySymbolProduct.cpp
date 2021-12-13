#include "pch.h"

#include"globedef.h"

#include "FinnhubCompanySymbolProduct.h"

IMPLEMENT_DYNCREATE(CFinnhubCompanySymbolProduct, CWebSourceDataProduct)

CFinnhubCompanySymbolProduct::CFinnhubCompanySymbolProduct() {
	m_strClassName = _T("Finnhub company symbols");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/symbol?exchange=");
	m_lIndex = -1;
}

CString CFinnhubCompanySymbolProduct::CreatMessage(void)
{
	CString strMiddle = gl_pWorldMarket->GetExchangeCode(m_lIndex);

	return m_strInquiringStr + strMiddle;
}

bool CFinnhubCompanySymbolProduct::ProcessWebData(CWebDataPtr pWebData) {
	CString str;
	vector<CWorldStockPtr> vStock;

	if (gl_pWorldMarket->ParseFinnhubStockSymbol(pWebData, vStock)) {
		// 加上交易所代码。
		for (auto& pStock3 : vStock) {
			pStock3->SetExchangeCode(gl_pWorldMarket->GetExchangeCode(m_lIndex));
		}
		for (auto& pStock2 : vStock) {
			if (!gl_pWorldMarket->IsStock(pStock2->GetSymbol())) {
				gl_pWorldMarket->AddStock(pStock2);
				str = _T("Finnhub发现新代码:") + pStock2->GetSymbol();
				gl_systemMessage.PushInnerSystemInformationMessage(str);
			}
		}
	}
	return true;
}