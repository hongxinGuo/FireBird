#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"
#include"WorldStock.h"
#include"CallableFunction.h"

#include "ProductFinnhubStockEstimatesEPSSurprise.h"

using namespace std;

IMPLEMENT_DYNCREATE(CProductFinnhubStockEstimatesEPSSurprise, CProductFinnhub)

CProductFinnhubStockEstimatesEPSSurprise::CProductFinnhubStockEstimatesEPSSurprise() {
	m_strClassName = _T("Finnhub estimates EPS surprise");
	m_strInquiry = _T("https://finnhub.io/api/v1/stock/earnings?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubStockEstimatesEPSSurprise::CreateMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	const auto strMiddle = pStock->GetSymbol();

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strTotalInquiryMessage = m_strInquiry + strMiddle;
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubStockEstimatesEPSSurprise::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	const auto pvEPSSurprise = ParseFinnhubEPSSurprise(pWebData);
	if (!pvEPSSurprise->empty()) { pStock->UpdateEPSSurprise(*pvEPSSurprise); }
	else {
		pStock->SetLastEPSSurpriseUpdateDate(19700101); // 将日期设置为更早。
		pStock->SetUpdateProfileDB(true);
	}
	pStock->m_fEPSSurpriseUpdated = true;
	pStock->m_fEPSSurpriseNeedSave = true;

	return true;
}

CEPSSurpriseVectorPtr CProductFinnhubStockEstimatesEPSSurprise::ParseFinnhubEPSSurprise(CWebDataPtr pWebData) {
	auto pvEPSSurprise = make_shared<vector<CEPSSurprisePtr>>();
	string s;
	CEPSSurprisePtr pEPSSurprise = nullptr;
	long year = 0, month = 0, day = 0;
	string sError;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvEPSSurprise;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvEPSSurprise;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvEPSSurprise;
	}
	const auto pjs = pWebData->GetJSon();
	try {
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
			pEPSSurprise = make_shared<CEPSSurprise>();
			s = jsonGetString(it, _T("symbol"));
			pEPSSurprise->m_strSymbol = s.c_str();
			s = jsonGetString(it,_T("period"));
			CString str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			pEPSSurprise->m_lDate = year * 10000 + month * 100 + day;
			pEPSSurprise->m_dEstimate = jsonGetDouble(it, _T("estimate"));
			pEPSSurprise->m_dActual = jsonGetDouble(it, _T("actual"));
			pvEPSSurprise->push_back(pEPSSurprise);
		}
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub EPS Surprise "), e.what());
		return pvEPSSurprise;
	}
	ranges::sort(pvEPSSurprise->begin(), pvEPSSurprise->end(), CompareEPSSurprise); // 以日期早晚顺序排列。
	return pvEPSSurprise;
}
