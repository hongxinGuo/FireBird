#include "pch.h"

#include"jsonParse.h"
#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubStockPriceQuote.h"

IMPLEMENT_DYNCREATE(CProductFinnhubStockPriceQuote, CProductFinnhub)

CProductFinnhubStockPriceQuote::CProductFinnhubStockPriceQuote() {
	m_strClassName = _T("Finnhub stock price quote");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/quote?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubStockPriceQuote::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	CString strMiddle = pStock->GetSymbol();

	return m_strInquiringStr + strMiddle;
}

bool CProductFinnhubStockPriceQuote::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	if (ParseFinnhubStockQuote(pWebData, pStock)) {
		if ((pStock->GetTransactionTime() + 3600 * 12 - ((CWorldMarket*)m_pMarket)->GetUTCTime()) > 0) { // ����ʱ�䲻����12Сʱ�������ô˹�ƱΪ��Ծ��Ʊ
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

bool CProductFinnhubStockPriceQuote::ParseFinnhubStockQuote(CWebDataPtr pWebData, CWorldStockPtr pStock) {
	string s;
	double dTemp = 0;
	time_t tt = 0;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return false;
	if (pWebData->IsVoidJSon()) { m_iReceivedDataStatus = __VOID_DATA__; return false; }
	if (pWebData->NoRightToAccess()) { m_iReceivedDataStatus = __NO_ACCESS_RIGHT__; return false; }
	ppt = pWebData->GetPTree();
	try {
		dTemp = ptreeGetDouble(*ppt, _T("c"));
		pStock->SetNew(dTemp * 1000);
		dTemp = ptreeGetDouble(*ppt, _T("h"));
		pStock->SetHigh(dTemp * 1000);
		dTemp = ptreeGetDouble(*ppt, _T("l"));
		pStock->SetLow(dTemp * 1000);
		dTemp = ptreeGetDouble(*ppt, _T("o"));
		pStock->SetOpen(dTemp * 1000);
		dTemp = ptreeGetDouble(*ppt, _T("pc"));
		pStock->SetLastClose(dTemp * 1000);
		tt = ppt->get<time_t>(_T("t"));
		pStock->SetTransactionTime(tt);
	}
	catch (ptree_error& e) { // ���ݸ�ʽ���ԣ�������
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Quote "), e);
		return false;
	}
	return true;
}