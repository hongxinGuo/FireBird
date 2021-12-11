#include "pch.h"

#include"globedef.h"

#include "FinnhubCryptoExchange.h"

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

	if (gl_pWorldMarket->ParseFinnhubCryptoExchange(pWebData, vCryptoExchange)) {
		for (int i = 0; i < vCryptoExchange.size(); i++) {
			if (!gl_pWorldMarket->IsCryptoExchange(vCryptoExchange.at(i))) {
				gl_pWorldMarket->AddCryptoExchange(vCryptoExchange.at(i));
			}
		}
		gl_pWorldMarket->SetFinnhubCryptoExchangeUpdated(true);
	}
	return true;
}