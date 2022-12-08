#include "pch.h"

#include"StockCodeConverter.h"

#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubCompanyPeer.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyPeer, CProductFinnhub)

CProductFinnhubCompanyPeer::CProductFinnhubCompanyPeer() {
	m_strClassName = _T("Finnhub company peer");
	m_strInquiry = _T("https://finnhub.io/api/v1/stock/peers?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyPeer::CreateMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strTotalInquiryMessage = m_strInquiry + pStock->GetSymbol();
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubCompanyPeer::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	CString strPeer = ParseFinnhubStockPeer(pWebData);
	pStock->SetPeer(strPeer);
	pStock->SetPeerUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
	pStock->SetPeerUpdated(true);
	pStock->SetUpdateProfileDB(true);

	return true;
}

CString CProductFinnhubCompanyPeer::ParseFinnhubStockPeer(CWebDataPtr pWebData) {
	CString strPeer = _T("{}"); // 默认的空状态（没有竞争对手)
	char buffer[1000]{};
	int i = 0;
	string sError;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return strPeer;
	if (pWebData->IsVoidJson()) { m_iReceivedDataStatus = __VOID_DATA__; return strPeer; }
	if (pWebData->CheckNoRightToAccess()) { m_iReceivedDataStatus = __NO_ACCESS_RIGHT__; return strPeer; }
	for (i = 0; i < pWebData->GetBufferLength(); i++) {
		buffer[i] = pWebData->GetData(i);
	}
	if (i > 200) {
		buffer[200] = 0x000;
	}
	else {
		buffer[pWebData->GetBufferLength()] = 0x000;
	}
	strPeer = buffer;
	return strPeer;
}