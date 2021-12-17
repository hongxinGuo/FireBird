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
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	shared_ptr<vector<CString>> pvForexExchange = nullptr;

	pvForexExchange = ((CWorldMarket*)m_pMarket)->ParseFinnhubForexExchange(pWebData);
	for (int i = 0; i < pvForexExchange->size(); i++) {
		if (!((CWorldMarket*)m_pMarket)->IsForexExchange(pvForexExchange->at(i))) {
			((CWorldMarket*)m_pMarket)->AddForexExchange(pvForexExchange->at(i));
		}
	}
	((CWorldMarket*)m_pMarket)->SetFinnhubForexExchangeUpdated(true);

	return true;
}