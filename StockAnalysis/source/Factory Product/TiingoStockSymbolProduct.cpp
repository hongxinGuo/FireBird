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
			if (((CWorldMarket*)m_pMarket)->IsStock(pTiingoStock->m_strTicker)) { // Tiingo��Symbol��Ϣֻ������Finnhub��һ�����䡣
				lTemp++;
				pStock = ((CWorldMarket*)m_pMarket)->GetStock(pTiingoStock->m_strTicker);
				pStock->UpdateStockProfile(pTiingoStock);
				pStock->SetUpdateProfileDB(true);
			}
			else { // new stock��
				// do nothing now.
				int iiii = 0;
				iiii++; // for debug
			}
		}
		TRACE("����Tiingo��Ծ��Ʊ��Ϊ��%d\n", lTemp);
		sprintf_s(buffer, _T("%6d"), lTemp);
		strNumber = buffer;
		str = _T("����Tiingo Symbol��Ծ��Ʊ����Ϊ") + strNumber;
		gl_systemMessage.PushInnerSystemInformationMessage(str);
	}
	((CWorldMarket*)m_pMarket)->SetTiingoSymbolUpdated(true);

	return true;
}