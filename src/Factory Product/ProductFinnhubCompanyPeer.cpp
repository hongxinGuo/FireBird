#include "pch.h"

#include"WorldMarket.h"
#include"FinnhubStock.h"

#include "ProductFinnhubCompanyPeer.h"

#include "WebData.h"

CProductFinnhubCompanyPeer::CProductFinnhubCompanyPeer() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/peers?symbol=";
}

string CProductFinnhubCompanyPeer::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + pStock->GetSymbol();
	return m_strInquiry;
}

void CProductFinnhubCompanyPeer::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(typeid(*GetMarket()) == typeid(CWorldMarket));

	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
	const nlohmannJson jsonPeer = ParseFinnhubStockPeer(pWebData);
	pStock->SetPeer(jsonPeer);
	pStock->SetPeerUpdateDate(GetMarket()->GetMarketDate());
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
