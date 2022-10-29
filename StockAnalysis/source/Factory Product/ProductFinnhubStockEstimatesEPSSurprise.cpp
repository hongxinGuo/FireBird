#include "pch.h"

#include"jsonParse.h"
#include"StockCodeConverter.h"
#include"WorldMarket.h"
#include"WorldStock.h"
#include"CallableFunction.h"

#include "ProductFinnhubStockEstimatesEPSSurprise.h"

IMPLEMENT_DYNCREATE(CProductFinnhubStockEstimatesEPSSurprise, CProductFinnhub)

CProductFinnhubStockEstimatesEPSSurprise::CProductFinnhubStockEstimatesEPSSurprise() {
	m_strClassName = _T("Finnhub estimates EPS surprise");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/earnings?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubStockEstimatesEPSSurprise::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	CString strMiddle = pStock->GetSymbol();

	m_strInquiringExchange = GetStockExchange(pStock->GetSymbol());
	m_strTotalInquiryMessage = m_strInquiringStr + strMiddle;
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubStockEstimatesEPSSurprise::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CEPSSurpriseVectorPtr pvEPSSurprise;

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	pvEPSSurprise = ParseFinnhubEPSSurprise(pWebData);
	if (pvEPSSurprise->size() > 0) {
		pStock->UpdateEPSSurprise(*pvEPSSurprise);
	}
	else {
		pStock->SetLastEPSSurpriseUpdateDate(19700101); // 将日期设置为更早。
		pStock->SetUpdateProfileDB(true);
	}
	pStock->m_fEPSSurpriseUpdated = true;
	pStock->m_fEPSSurpriseNeedSave = true;

	return true;
}

CEPSSurpriseVectorPtr CProductFinnhubStockEstimatesEPSSurprise::ParseFinnhubEPSSurprise(CWebDataPtr pWebData) {
	CEPSSurpriseVectorPtr pvEPSSurprise = make_shared<vector<CEPSSurprisePtr>>();
	ptree pt2;
	string s;
	CEPSSurprisePtr pEPSSurprise = nullptr;
	long year = 0, month = 0, day = 0;
	CString str;
	string sError;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvEPSSurprise;
	if (pWebData->IsVoidJSon()) { m_iReceivedDataStatus = __VOID_DATA__; return pvEPSSurprise; }
	if (pWebData->NoRightToAccess()) { m_iReceivedDataStatus = __NO_ACCESS_RIGHT__; return pvEPSSurprise; }
	ppt = pWebData->GetPTree();
	try {
		for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
			pEPSSurprise = make_shared<CEPSSurprise>();
			pt2 = it->second;
			s = pt2.get<string>(_T("symbol"));
			pEPSSurprise->m_strSymbol = s.c_str();
			s = pt2.get<string>(_T("period"));
			str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			pEPSSurprise->m_lDate = year * 10000 + month * 100 + day;
			pEPSSurprise->m_dEstimate = ptreeGetDouble(pt2, _T("estimate"));
			pEPSSurprise->m_dActual = ptreeGetDouble(pt2, _T("actual"));
			pvEPSSurprise->push_back(pEPSSurprise);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub EPS Surprise "), e);
		return pvEPSSurprise;
	}
	sort(pvEPSSurprise->begin(), pvEPSSurprise->end(), CompareEPSSurprise); // 以日期早晚顺序排列。
	return pvEPSSurprise;
}