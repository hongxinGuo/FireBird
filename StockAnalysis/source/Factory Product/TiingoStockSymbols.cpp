#include "pch.h"

#include"globedef.h"

#include"TiingoStock.h"
#include "TiingoStockSymbols.h"

CTiingoStockSymbols::CTiingoStockSymbols() : CWebSourceDataProduct() {
	m_strClassName = _T("Tiingo stock symbols");
	m_strInquiringStr = _T("https://api.tiingo.com/tiingo/fundamentals/meta?");
}

CString CTiingoStockSymbols::CreatMessage(void) {
	return m_strInquiringStr;
}

bool CTiingoStockSymbols::ProcessWebData(CWebDataPtr pWebData) {
	vector<CTiingoStockPtr> vTiingoStock;
	CWorldStockPtr pStock = nullptr;
	char buffer[100];
	CString strNumber, str;
	long lTemp = 0;

	if (gl_pWorldMarket->ParseTiingoStockSymbol(pWebData, vTiingoStock)) {
		lTemp = 0;
		for (auto& pTiingoStock : vTiingoStock) {
			if (!gl_pWorldMarket->IsTiingoStock(pTiingoStock->m_strTicker)) {
				gl_pWorldMarket->AddTiingoStock(pTiingoStock);
			}
			if (gl_pWorldMarket->IsStock(pTiingoStock->m_strTicker)) { // Tiingo的Symbol信息只是用于Finnhub的一个补充。
				lTemp++;
				pStock = gl_pWorldMarket->GetStock(pTiingoStock->m_strTicker);
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
	gl_pWorldMarket->SetTiingoSymbolUpdated(true);

	return true;
}