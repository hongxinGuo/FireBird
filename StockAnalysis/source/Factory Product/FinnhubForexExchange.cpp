#include "pch.h"

#include"globedef.h"

#include "FinnhubForexExchange.h"

IMPLEMENT_DYNCREATE(CFinnhubForexExchange, CWebSourceDataProduct)

CFinnhubForexExchange::CFinnhubForexExchange() {
	m_strClassName = _T("Finnhub forex exchange");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/forex/exchange?");
	m_lIndex = -1;
}

CString CFinnhubForexExchange::CreatMessage(void) {
	return m_strInquiringStr;
}

bool CFinnhubForexExchange::ProcessWebData(CWebDataPtr pWebData) {
	vector<CString> vForexExchange;

	if (gl_pWorldMarket->ParseFinnhubForexExchange(pWebData, vForexExchange)) {
		for (int i = 0; i < vForexExchange.size(); i++) {
			if (!gl_pWorldMarket->IsForexExchange(vForexExchange.at(i))) {
				gl_pWorldMarket->AddForexExchange(vForexExchange.at(i));
			}
		}
		gl_pWorldMarket->SetFinnhubForexExchangeUpdated(true);
	}

	return true;
}