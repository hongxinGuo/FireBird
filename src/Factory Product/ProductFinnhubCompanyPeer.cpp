#include "pch.h"

#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubCompanyPeer.h"

CProductFinnhubCompanyPeer::CProductFinnhubCompanyPeer() {
	m_strClassName = _T("Finnhub company peer");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/peers?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyPeer::CreateMessage() {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = GetMarket()->GetStock(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + pStock->GetSymbol();
	return m_strInquiry;
}

bool CProductFinnhubCompanyPeer::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = GetMarket()->GetStock(m_lIndex);
	const json jsonPeer = ParseFinnhubStockPeer(pWebData);
	pStock->SetPeer(jsonPeer);
	pStock->SetPeerUpdateDate(GetMarket()->GetMarketDate());
	pStock->SetUpdatePeer(false);
	pStock->SetUpdateProfileDB(true);

	return true;
}

json CProductFinnhubCompanyPeer::ParseFinnhubStockPeer(const CWebDataPtr& pWebData) {
	json jsonPeer; // 默认的空状态（没有竞争对手)

	if (!IsValidData(pWebData)) return jsonPeer;

	return *pWebData->GetJSon();
}
