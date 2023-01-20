#include "pch.h"

#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubCompanyPeer.h"

CProductFinnhubCompanyPeer::CProductFinnhubCompanyPeer() {
	m_strClassName = _T("Finnhub company peer");
	m_strInquiry = _T("https://finnhub.io/api/v1/stock/peers?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyPeer::CreateMessage(void) {
	ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strTotalInquiryMessage = m_strInquiry + pStock->GetSymbol();
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubCompanyPeer::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	const json jsonPeer = ParseFinnhubStockPeer(pWebData);
	pStock->SetPeer(jsonPeer);
	pStock->SetPeerUpdateDate(m_pMarket->GetMarketDate());
	pStock->SetUpdatePeer(false);
	pStock->SetUpdateProfileDB(true);

	return true;
}

json CProductFinnhubCompanyPeer::ParseFinnhubStockPeer(CWebDataPtr pWebData) {
	json jsonPeer; // 默认的空状态（没有竞争对手)

	string sError;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return jsonPeer;

	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return jsonPeer;
	}

	return *pWebData->GetJSon();
}
