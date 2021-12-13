#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"WorldStock.h"

#include "FinnhubStockPriceQuote.h"

IMPLEMENT_DYNCREATE(CFinnhubStockPriceQuote, CWebSourceDataProduct)

CFinnhubStockPriceQuote::CFinnhubStockPriceQuote() {
	m_strClassName = _T("Finnhub stock price quote");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/quote?symbol=");
	m_lIndex = -1;
}

CString CFinnhubStockPriceQuote::CreatMessage(void) {
	CWorldStockPtr pStock = gl_pWorldMarket->GetStock(m_lIndex);
	CString strMiddle = pStock->GetSymbol();

	return m_strInquiringStr + strMiddle;
}

bool CFinnhubStockPriceQuote::ProcessWebData(CWebDataPtr pWebData) {
	CWorldStockPtr pStock = gl_pWorldMarket->GetStock(m_lIndex);
	gl_pWorldMarket->ParseFinnhubStockQuote(pWebData, pStock);
	if ((pStock->GetTransactionTime() + 3600 * 12 - gl_pWorldMarket->GetUTCTime()) > 0) { // 交易时间不早于12小时，则设置此股票为活跃股票
		pStock->SetActive(true);
		if (!pStock->IsIPOed()) {
			pStock->SetIPOStatus(__STOCK_IPOED__);
			pStock->SetUpdateProfileDB(true);
		}
	}

	return true;
}