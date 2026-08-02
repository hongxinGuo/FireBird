module;
module FireBirdLib.Product.Finnhub.CompanyPeer;
import FireBirdLib.Container.Stock.FinnhubStock;
import FireBirdLib.Market.WorldMarket;

using std::string;

CProductFinnhubCompanyPeer::CProductFinnhubCompanyPeer() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/peers?symbol=";
}

string CProductFinnhubCompanyPeer::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);

	m_strInquiringExchange = pStock->GetExchange();
	m_strInquiry = m_strInquiryFunction + pStock->GetSymbol();
	return m_strInquiry;
}

void CProductFinnhubCompanyPeer::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
	const nlohmannJson jsonPeer = ParseFinnhubStockPeer(pWebData);
	pStock->SetPeer(jsonPeer);
	pStock->SetPeerUpdateDate(gl_pWorldMarket->GetMarketDate());
	pStock->SetUpdatePeer(false);
	pStock->SetUpdateProfileDB(true);
}

nlohmannJson CProductFinnhubCompanyPeer::ParseFinnhubStockPeer(const CWebDataPtr& pWebData) {
	nlohmannJson jsonPeer; // 默认的空状态（没有竞争对手)
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return jsonPeer;
	if (!IsValidData(pWebData)) return jsonPeer;

	return js;
}
