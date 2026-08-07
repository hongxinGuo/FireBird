#include "pch.h"

#include"WorldMarket.h"
#include"FinnhubStock.h"
#include "ContainerFinnhubStock.h"

#include "ProductFinnhubCompanyPeer.h"

#include "WebData.h"

CProductFinnhubCompanyPeer::CProductFinnhubCompanyPeer() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/peers?symbol=";
}

shared_ptr<vector<string>> CProductFinnhubCompanyPeer::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);

	m_strInquiringExchange = pStock->GetExchange();
	m_inquiryString = m_strInquiryFunction + pStock->GetSymbol();
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
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
