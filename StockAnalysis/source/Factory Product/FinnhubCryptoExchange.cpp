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
	vector<CString> vCryptoExchange;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	if (((CWorldMarket*)m_pMarket)->ParseFinnhubCryptoExchange(pWebData, vCryptoExchange)) {
		for (int i = 0; i < vCryptoExchange.size(); i++) {
			if (!((CWorldMarket*)m_pMarket)->IsCryptoExchange(vCryptoExchange.at(i))) {
				((CWorldMarket*)m_pMarket)->AddCryptoExchange(vCryptoExchange.at(i));
			}
		}
		((CWorldMarket*)m_pMarket)->SetFinnhubCryptoExchangeUpdated(true);
	}
	return true;
}