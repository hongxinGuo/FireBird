module;

module FireBirdLib.Product.Finnhub.StockEstimatesEPSSurprise;
import FireBirdLib.Container.Stock.FinnhubStock;
import FireBirdLib.Accessory.NlohmannJsonGetValue;
import FireBirdLib.Accessory.JsonParse;

using std::make_shared;
using std::vector;
using std::istringstream;
using std::string;
using std::chrono::local_days;
using std::chrono::days;

CProductFinnhubStockEstimatesEPSSurprise::CProductFinnhubStockEstimatesEPSSurprise() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/earnings?symbol=";
}

string CProductFinnhubStockEstimatesEPSSurprise::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
	const auto strParam = pStock->GetSymbol();

	m_strInquiringExchange = pStock->GetExchange();
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductFinnhubStockEstimatesEPSSurprise::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
	const auto pvEPSSurprise = ParseFinnhubEPSSurprise(pWebData);
	if (!pvEPSSurprise->empty()) { pStock->UpdateEPSSurprise(pvEPSSurprise); }
	else {
		pStock->SetLastEPSSurpriseUpdateDate(local_days(days(0))); // 将日期设置为更早。
		pStock->SetUpdateProfileDB(true);
	}
	pStock->SetUpdateEPSSurprise(false);
	pStock->m_fUpdateEPSSurpriseDB = true;
}

CEPSSurprisesPtr CProductFinnhubStockEstimatesEPSSurprise::ParseFinnhubEPSSurprise(const CWebDataPtr& pWebData) {
	auto pvEPSSurprise = make_shared<vector<CEPSSurprise>>();
	string sError;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvEPSSurprise;
	if (!IsValidData(pWebData)) return pvEPSSurprise;

	try {
		CEPSSurprise pEPSSurprise;
		for (auto it = js.begin(); it != js.end(); ++it) {
			string s = jsonGetString(it, "symbol");
			pEPSSurprise.m_strSymbol = s;
			s = jsonGetString(it, "period");
			istringstream ss(s);
			local_days ld;
			ss >> parse("%F", ld);
			pEPSSurprise.m_lDate = ld;
			pEPSSurprise.m_dEstimate = jsonGetDouble(it, "estimate");
			pEPSSurprise.m_dActual = jsonGetDouble(it, "actual");
			pvEPSSurprise->push_back(pEPSSurprise);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub EPS Surprise ", e.what());
		return pvEPSSurprise;
	}
	std::ranges::sort(pvEPSSurprise->begin(), pvEPSSurprise->end(),
	                  [](const CEPSSurprise& p1, const CEPSSurprise& p2) { return (p1.m_lDate < p2.m_lDate); }); // 以日期早晚顺序排列。
	return pvEPSSurprise;
}
