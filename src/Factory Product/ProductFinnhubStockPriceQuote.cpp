#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldStock.h"

#include "ProductFinnhubStockPriceQuote.h"

CProductFinnhubStockPriceQuote::CProductFinnhubStockPriceQuote() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/quote?symbol=");
}

CString CProductFinnhubStockPriceQuote::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);
	const auto strParam = pStock->GetSymbol();

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductFinnhubStockPriceQuote::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);
	if (ParseFinnhubStockQuote(pWebData, pStock)) {
		if ((pStock->GetTransactionTime() + 3600 * 12 - GetUTCTime()) > 0) {
			// 交易时间不早于12小时，则设置此股票为活跃股票
			pStock->SetActive(true);
			if (!pStock->IsIPOed()) {
				pStock->SetIPOStatus(_STOCK_IPOED_);
				pStock->SetUpdateProfileDB(true);
			}
		}
		return;
	}
}

bool CProductFinnhubStockPriceQuote::ParseFinnhubStockQuote(const CWebDataPtr& pWebData, const CWorldStockPtr& pStock) {
	json js;

	if (!pWebData->CreateJson(js)) return false;
	if (!IsValidData(pWebData)) return false;

	try {
		double dTemp = jsonGetDouble(js, _T("c"));
		pStock->SetNew(dTemp * 1000);
		dTemp = jsonGetDouble(js, _T("h"));
		pStock->SetHigh(dTemp * 1000);
		dTemp = jsonGetDouble(js, _T("l"));
		pStock->SetLow(dTemp * 1000);
		dTemp = jsonGetDouble(js, _T("o"));
		pStock->SetOpen(dTemp * 1000);
		dTemp = jsonGetDouble(js, _T("pc"));
		pStock->SetLastClose(dTemp * 1000);
		const auto tt = jsonGetLongLong(js,_T("t"));
		pStock->SetTransactionTime(tt);
	}
	catch (json::exception& e) {
		// 数据格式不对，跳过。
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Quote "), e.what());
		return false;
	}
	return true;
}
