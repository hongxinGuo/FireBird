#include "pch.h"

#include"SystemData.h"
#include"WorldMarket.h"
#include"FinnhubStock.h"

#include "ProductFinnhubCompanyPeer.h"

CProductFinnhubCompanyPeer::CProductFinnhubCompanyPeer() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/peers?symbol=");
}

CString CProductFinnhubCompanyPeer::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + pStock->GetSymbol();
	return m_strInquiry;
}

void CProductFinnhubCompanyPeer::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);
	const nlohmann::ordered_json jsonPeer = ParseFinnhubStockPeer(pWebData);
	pStock->SetPeer(jsonPeer);
	pStock->SetPeerUpdateDate(GetMarket()->GetMarketDate());
	pStock->SetUpdatePeer(false);
	pStock->SetUpdateProfileDB(true);
}

nlohmann::ordered_json CProductFinnhubCompanyPeer::ParseFinnhubStockPeer(const CWebDataPtr& pWebData) {
	nlohmann::ordered_json jsonPeer; // 默认的空状态（没有竞争对手)
	nlohmann::ordered_json js;

	if (!pWebData->CreateJson(js)) return jsonPeer;
	if (!IsValidData(pWebData)) return jsonPeer;

	return js;
}
