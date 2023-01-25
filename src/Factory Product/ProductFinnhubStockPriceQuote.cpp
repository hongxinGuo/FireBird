#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubStockPriceQuote.h"

CProductFinnhubStockPriceQuote::CProductFinnhubStockPriceQuote() {
	m_strClassName = _T("Finnhub stock price quote");
	m_strInquiry = _T("https://finnhub.io/api/v1/quote?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubStockPriceQuote::CreateMessage(void) {
	ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = static_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	const auto strMiddle = pStock->GetSymbol();

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strTotalInquiryMessage = m_strInquiry + strMiddle;
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubStockPriceQuote::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	if (ParseFinnhubStockQuote(pWebData, pStock)) {
		if ((pStock->GetTransactionTime() + 3600 * 12 - m_pMarket->GetUTCTime()) > 0) {
			// ����ʱ�䲻����12Сʱ�������ô˹�ƱΪ��Ծ��Ʊ
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

bool CProductFinnhubStockPriceQuote::ParseFinnhubStockQuote(CWebDataPtr pWebData, CWorldStockPtr pStock) {
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
		// ���ݸ�ʽ���ԣ�������
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Quote "), e.what());
		return false;
	}
	return true;
}
