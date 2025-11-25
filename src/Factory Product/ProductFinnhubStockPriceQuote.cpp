#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"FinnhubStock.h"

#include "ProductFinnhubStockPriceQuote.h"

#include "WebData.h"

CProductFinnhubStockPriceQuote::CProductFinnhubStockPriceQuote() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/quote?symbol=";
}

string CProductFinnhubStockPriceQuote::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
	const auto strParam = pStock->GetSymbol();

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductFinnhubStockPriceQuote::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
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

bool CProductFinnhubStockPriceQuote::ParseFinnhubStockQuote(const CWebDataPtr& pWebData, const CFinnhubStockPtr& pStock) {
	json js;

	if (!pWebData->CreateJson(js)) return false;
	if (!IsValidData(pWebData)) return false;

	try {
		double dTemp = jsonGetDouble(js, "c");
		pStock->SetNew(dTemp * 1000);
		dTemp = jsonGetDouble(js, "h");
		pStock->SetHigh(dTemp * 1000);
		dTemp = jsonGetDouble(js, "l");
		pStock->SetLow(dTemp * 1000);
		dTemp = jsonGetDouble(js, "o");
		pStock->SetOpen(dTemp * 1000);
		dTemp = jsonGetDouble(js, "pc");
		pStock->SetLastClose(dTemp * 1000);
		const auto tt = jsonGetLongLong(js,"t");
		pStock->SetTransactionTime(tt);
	} catch (json::exception& e) {
		// 数据格式不对，跳过。
		ReportJSonErrorToSystemMessage("Finnhub Stock Quote ", e.what());
		return false;
	}
	return true;
}
