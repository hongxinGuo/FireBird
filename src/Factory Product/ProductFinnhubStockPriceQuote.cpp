#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubStockPriceQuote.h"

#include "TimeConvert.h"

CProductFinnhubStockPriceQuote::CProductFinnhubStockPriceQuote() {
	m_strClassName = _T("Finnhub stock price quote");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/quote?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubStockPriceQuote::CreateMessage() {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = dynamic_pointer_cast<CWorldMarket>(GetMarket())->GetStock(m_lIndex);
	const auto strParam = pStock->GetSymbol();

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

bool CProductFinnhubStockPriceQuote::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = dynamic_pointer_cast<CWorldMarket>(GetMarket())->GetStock(m_lIndex);
	if (ParseFinnhubStockQuote(pWebData, pStock)) {
		if ((pStock->GetTransactionTime() + 3600 * 12 - GetUTCTime()) > 0) {
			// 交易时间不早于12小时，则设置此股票为活跃股票
			pStock->SetActive(true);
			if (!pStock->IsIPOed()) {
				pStock->SetIPOStatus(_STOCK_IPOED_);
				pStock->SetUpdateProfileDB(true);
			}
		}
		return true;
	}
	return false;
}

bool CProductFinnhubStockPriceQuote::ParseFinnhubStockQuote(const CWebDataPtr& pWebData, const CWorldStockPtr& pStock) {
	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return false;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = VOID_DATA_;
		return false;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return false;
	}
	const auto pjs = pWebData->GetJSon();
	try {
		double dTemp = jsonGetDouble(pjs, _T("c"));
		pStock->SetNew(dTemp * 1000);
		dTemp = jsonGetDouble(pjs, _T("h"));
		pStock->SetHigh(dTemp * 1000);
		dTemp = jsonGetDouble(pjs, _T("l"));
		pStock->SetLow(dTemp * 1000);
		dTemp = jsonGetDouble(pjs, _T("o"));
		pStock->SetOpen(dTemp * 1000);
		dTemp = jsonGetDouble(pjs, _T("pc"));
		pStock->SetLastClose(dTemp * 1000);
		const auto tt = jsonGetLongLong(pjs,_T("t"));
		pStock->SetTransactionTime(tt);
	}
	catch (json::exception& e) {
		// 数据格式不对，跳过。
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Quote "), e.what());
		return false;
	}
	return true;
}
