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
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	CString strMiddle = pStock->GetSymbol();

	return m_strInquiringStr + strMiddle;
}

bool CFinnhubStockPriceQuote::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	if (ParseFinnhubStockQuote(pWebData, pStock)) {
		if ((pStock->GetTransactionTime() + 3600 * 12 - ((CWorldMarket*)m_pMarket)->GetUTCTime()) > 0) { // 交易时间不早于12小时，则设置此股票为活跃股票
			pStock->SetActive(true);
			if (!pStock->IsIPOed()) {
				pStock->SetIPOStatus(__STOCK_IPOED__);
				pStock->SetUpdateProfileDB(true);
			}
		}
		return true;
	}
	else return false;
}

bool CFinnhubStockPriceQuote::ParseFinnhubStockQuote(CWebDataPtr pWebData, CWorldStockPtr pStock) {
	string s;
	ptree pt;
	double dTemp = 0;
	time_t tt = 0;

	if (!ConvertToJSON(pt, pWebData)) {
		return false;
	}
	try {
		dTemp = pt.get<double>(_T("c"));
		pStock->SetNew(dTemp * 1000);
		dTemp = pt.get<double>(_T("h"));
		pStock->SetHigh(dTemp * 1000);
		dTemp = pt.get<double>(_T("l"));
		pStock->SetLow(dTemp * 1000);
		dTemp = pt.get<double>(_T("o"));
		pStock->SetOpen(dTemp * 1000);
		dTemp = pt.get<double>(_T("pc"));
		pStock->SetLastClose(dTemp * 1000);
		tt = pt.get<time_t>(_T("t"));
		pStock->SetTransactionTime(tt);
	}
	catch (ptree_error& e) { // 数据格式不对，跳过。
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Quote "), e);
		return false;
	}

	return true;
}