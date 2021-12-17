#include "pch.h"

#include"globedef.h"

#include "FinnhubCryptoExchange.h"

IMPLEMENT_DYNCREATE(CFinnhubCryptoExchange, CWebSourceDataProduct)

CFinnhubCryptoExchange::CFinnhubCryptoExchange() {
	m_strClassName = _T("Finnhub crypto exchange");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/crypto/exchange?");
	m_lIndex = -1;
}

CString CFinnhubCryptoExchange::CreatMessage(void) {
	return m_strInquiringStr;
}

bool CFinnhubCryptoExchange::ProcessWebData(CWebDataPtr pWebData) {
	shared_ptr<vector<CString>> pvCryptoExchange = nullptr;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	pvCryptoExchange = ((CWorldMarket*)m_pMarket)->ParseFinnhubCryptoExchange(pWebData);
	for (int i = 0; i < pvCryptoExchange->size(); i++) {
		if (!((CWorldMarket*)m_pMarket)->IsCryptoExchange(pvCryptoExchange->at(i))) {
			((CWorldMarket*)m_pMarket)->AddCryptoExchange(pvCryptoExchange->at(i));
		}
	}
	((CWorldMarket*)m_pMarket)->SetFinnhubCryptoExchangeUpdated(true);

	return true;
}