#include "pch.h"

#include"globedef.h"

#include"TiingoStock.h"
#include "TiingoStockSymbolProduct.h"

IMPLEMENT_DYNCREATE(CTinngoStockSymbolProduct, CWebSourceDataProduct)

CTinngoStockSymbolProduct::CTinngoStockSymbolProduct() : CWebSourceDataProduct() {
	m_strClassName = _T("Tiingo stock symbols");
	m_strInquiringStr = _T("https://api.tiingo.com/tiingo/fundamentals/meta?");
}

CString CTinngoStockSymbolProduct::CreatMessage(void) {
	return m_strInquiringStr;
}

bool CTinngoStockSymbolProduct::ProcessWebData(CWebDataPtr pWebData) {
	CTiingoStockVectorPtr pvTiingoStock = nullptr;
	CWorldStockPtr pStock = nullptr;
	char buffer[100];
	CString strNumber, str;
	long lTemp = 0;

	pvTiingoStock = ((CWorldMarket*)m_pMarket)->ParseTiingoStockSymbol(pWebData);
	if (pvTiingoStock->size() > 0) {
		lTemp = 0;
		for (auto& pTiingoStock : *pvTiingoStock) {
			if (!((CWorldMarket*)m_pMarket)->IsTiingoStock(pTiingoStock->m_strTicker)) {
				((CWorldMarket*)m_pMarket)->AddTiingoStock(pTiingoStock);
			}
			if (((CWorldMarket*)m_pMarket)->IsStock(pTiingoStock->m_strTicker)) { // Tiingo的Symbol信息只是用于Finnhub的一个补充。
				lTemp++;
				pStock = ((CWorldMarket*)m_pMarket)->GetStock(pTiingoStock->m_strTicker);
				pStock->UpdateStockProfile(pTiingoStock);
				pStock->SetUpdateProfileDB(true);
			}
			else { // new stock，
				// do nothing now.
				int iiii = 0;
				iiii++; // for debug
			}
		}
		TRACE("今日Tiingo活跃股票数为：%d\n", lTemp);
		sprintf_s(buffer, _T("%6d"), lTemp);
		strNumber = buffer;
		str = _T("今日Tiingo Symbol活跃股票总数为") + strNumber;
		gl_systemMessage.PushInnerSystemInformationMessage(str);
	}
	((CWorldMarket*)m_pMarket)->SetTiingoSymbolUpdated(true);

	return true;
}